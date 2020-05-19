/**
 *  Copyright (c) 1999~2017, Altibase Corp. and/or its affiliates. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License, version 3,
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <smErrorCode.h>
#include <mmErrorCode.h>
#include <smi.h>
#include <mmcSession.h>
#include <mmcTrans.h>
#include <dki.h>

iduMemPool mmcTrans::mPool;
iduMemPool mmcTrans::mSharePool;

static inline void initTransShareInfo(mmcTransShareInfo *aShareInfo)
{
    IDE_ASSERT(aShareInfo->mTransMutex.initialize((SChar*)"Transaction Share Mutex",
                                       IDU_MUTEX_KIND_POSIX,
                                       IDV_WAIT_INDEX_NULL) == IDE_SUCCESS);
    aShareInfo->mIsGlobalTx = ID_FALSE;
    aShareInfo->mAllocRefCnt = 0;
    aShareInfo->mTransRefCnt = 0;
    aShareInfo->mDelegatedSessions = NULL;
    aShareInfo->mNodeName[0] = '\0';
}

static inline void finiTransShareInfo(mmcTransShareInfo *aShareInfo)
{
    (void)aShareInfo->mTransMutex.destroy();
    aShareInfo->mIsGlobalTx = ID_FALSE;
    aShareInfo->mAllocRefCnt = 0;
    aShareInfo->mTransRefCnt = 0;
    aShareInfo->mDelegatedSessions = NULL;
    aShareInfo->mNodeName[0] = '\0';
}

/*
 * beginRaw: Session에 대한 후속 처리 없이 기본적인 sm의 begin 및 event block만 처리하는 경우 
 * 사용하는 함수로 internal 로직에서 주로 사용
 */
void mmcTrans::beginRaw(mmcTransObj *aTrans, 
                        idvSQL      *aStatistics, 
                        UInt         aFlag, 
                        ULong       *aSessionEventFlag)
{
    smiStatement *sDummySmiStmt = NULL;
    if ( aTrans->mSmiTrans.isBegin() == ID_FALSE )
    {
        /* session event에 의한 fail을 방지한다. */
        IDU_SESSION_SET_BLOCK(*aSessionEventFlag);
        IDE_ASSERT(aTrans->mSmiTrans.begin(&sDummySmiStmt, 
                                            aStatistics, /* PROJ-2446 */
                                            aFlag) 
                    == IDE_SUCCESS);
        IDU_SESSION_CLR_BLOCK(*aSessionEventFlag);
    }
    else
    {
        /* Nothing to do */
    }
}

/*
 * clearAndSetSessionInfoAfterBegin: Transaction의 begin이후 mmcSession의 정보의 변경이 필요한 경우
 * 사용하는 함수로 internal 로직에서 주로 사용
 */
void mmcTrans::clearAndSetSessionInfoAfterBegin( mmcSession  * aSession,
                                                 mmcTransObj * aTrans )
{
    //PROJ-1677 DEQUEUE
    aSession->clearPartialRollbackFlag();

    /* BUG-42853 LOCK TABLE에 UNTIL NEXT DDL 기능 추가 */
    if ( aSession->getLockTableUntilNextDDL() == ID_TRUE )
    {
        aSession->setLockTableUntilNextDDL( ID_FALSE );
        aSession->setTableIDOfLockTableUntilNextDDL( 0 );
    }
    else
    {
        /* Nothing to do */
    }

    /* BUG-44967 */
    aSession->getInfo()->mTransID = getTransID( aTrans );
}

/*
 * commitRaw: Session에 대한 후속 처리 없이 기본적인 sm의 commit 및 event block만 처리하는 경우 
 * 사용하는 함수로 internal 로직에서 주로 사용
 */
IDE_RC mmcTrans::commitRaw( mmcTransObj *aTrans,
                            ULong       *aEventFlag,
                            UInt         aTransReleasePolicy,
                            smSCN       *aCommitSCN )
{
    if ( aTrans->mSmiTrans.isBegin() == ID_TRUE )
    {
        IDU_SESSION_SET_BLOCK(*aEventFlag);

        SMI_INIT_SCN( aCommitSCN );
        IDE_TEST(aTrans->mSmiTrans.commit(aCommitSCN, aTransReleasePolicy) != IDE_SUCCESS);

        IDU_SESSION_CLR_BLOCK(*aEventFlag);
    }
    else
    {
        /*do nothing*/
    }
    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    IDU_SESSION_CLR_BLOCK(*aEventFlag);

    return IDE_FAILURE;
}

/*
 * rollbackRaw: Session에 대한 후속 처리 없이 기본적인 sm의 rollback 및 event block만 처리하는 경우
 * 사용하는 함수로 internal 로직에서 주로 사용
 */
IDE_RC mmcTrans::rollbackRaw( mmcTransObj *aTrans,
                              ULong       *aEventFlag,
                              UInt         aTransReleasePolicy )
{
    if ( aTrans->mSmiTrans.isBegin() == ID_TRUE )
    {
        IDU_SESSION_SET_BLOCK(*aEventFlag);

        IDE_TEST(aTrans->mSmiTrans.rollback(NULL, aTransReleasePolicy) != IDE_SUCCESS);

        IDU_SESSION_CLR_BLOCK(*aEventFlag);
    }
    else
    {
        /*nothing to do*/
    }
    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    IDU_SESSION_CLR_BLOCK(*aEventFlag);

    return IDE_FAILURE;
}

/*
 * PROJ-2701 share transaction commit
 */
IDE_RC mmcTrans::commitShareableTrans( mmcTransObj *aTrans,
                                       mmcSession  *aSession,
                                       UInt         aTransReleasePolicy,
                                       smSCN       *aCommitSCN )
{
    UInt   sTransRefCnt = 0;
    idBool sIsLock      = ID_FALSE;

    lockRecursive( aTrans );
    sIsLock = ID_TRUE;
    sTransRefCnt = aTrans->mShareInfo->mTransRefCnt;
    aTrans->mShareInfo->mTransRefCnt--;

    switch ( aTrans->mShareInfo->mTransRefCnt )
    {
        case 0:
            IDE_TEST( mmcTrans::commitRaw( aTrans,
                                           aSession->getEventFlag(),
                                           aTransReleasePolicy,
                                           aCommitSCN )
                      != IDE_SUCCESS );

            break;

        case 1:
            if ( ( aTrans->mShareInfo->mIsGlobalTx == ID_TRUE ) &&
                 ( aTrans->mShareInfo->mDelegatedSessions != NULL ) )
            {
                /* global tx에서는 share session에 직접 커밋한다. */
                IDE_TEST( commitSessions( aTrans->mShareInfo ) != IDE_SUCCESS );
            }
            break;

        default:
            /* Do Nothing */
            break;
    }

    if ( aSession->isShardLibrarySession() == ID_TRUE )
    {
        removeDelegatedSession( aTrans->mShareInfo, aSession );
    }

    sIsLock = ID_FALSE;
    unlockRecursive( aTrans );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    if ( sIsLock == ID_TRUE )
    {
        aTrans->mShareInfo->mTransRefCnt = sTransRefCnt;
        sIsLock = ID_FALSE;
        unlockRecursive( aTrans );
    }

    return IDE_FAILURE;
}

/*
 * after transaction commit, process related session
 */
IDE_RC mmcTrans::doAfterCommit( mmcSession *aSession,
                                UInt        aTransReleasePolicy,
                                idBool      aIsSqlPrepare,
                                smSCN      *aCommitSCN )
{
    if ( aIsSqlPrepare == ID_FALSE )
    {
        if ( aSession->getCommitMode() == MMC_COMMITMODE_NONAUTOCOMMIT )
        {
            aSession->setTransBegin( ID_FALSE );
        }
        else
        {
            /* Nothing to do */
        }

        if ( aSession->getTransPrepared() == ID_TRUE )
        {
            aSession->setTransPrepared( NULL );
        }
        else
        {
            /* Nothing to do */
        }

        (void)aSession->clearLobLocator();

        (void)aSession->flushEnqueue( aCommitSCN );

        // PROJ-1677 DEQUEUE
        if ( aSession->getPartialRollbackFlag() == MMC_DID_PARTIAL_ROLLBACK )
        {
            (void)aSession->flushDequeue( aCommitSCN );
        }
        else
        {
            (void)aSession->clearDequeue();
        }

        // PROJ-1407 Temporary Table
        qci::endTransForSession( aSession->getQciSession() );

        // BUG-42464 dbms_alert package
        IDE_TEST( aSession->getInfo()->mEvent.commit() != IDE_SUCCESS );
    }
    else
    {
        /* Nothing to do */
    }

    if ( aTransReleasePolicy == SMI_RELEASE_TRANSACTION )
    {
        aSession->getInfo()->mTransID = 0;
    }
    else
    {
        /* Nothing to do */
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/*
 * follow function(removeDelegatedSession) must be located in critical section
 */
void mmcTrans::removeDelegatedSession(mmcTransShareInfo *aShareInfo, mmcSession *aSession)
{
    if ( aShareInfo->mDelegatedSessions == aSession )
    {
        aShareInfo->mDelegatedSessions = NULL;
    }
    else
    {
        IDE_WARNING(IDE_SERVER_0, "Invalid delegated session");
        IDE_DASSERT(aShareInfo->mDelegatedSessions == aSession);
    }
}

/*
 * follow function(addSession) must be located in critical section
 */
void mmcTrans::addDelegatedSession(mmcTransShareInfo *aShareInfo, mmcSession *aSession)
{
    IDE_DASSERT(aShareInfo->mDelegatedSessions == NULL);
    aShareInfo->mDelegatedSessions = aSession;
}

/*
 * follow function(commitSessions) must be located in critical section
 */
IDE_RC mmcTrans::commitSessions(mmcTransShareInfo *aShareInfo)
{
    if ( aShareInfo->mDelegatedSessions != NULL )
    {
        IDE_TEST(aShareInfo->mDelegatedSessions->commit() != IDE_SUCCESS);
        IDE_DASSERT(aShareInfo->mDelegatedSessions == NULL);
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC mmcTrans::initialize()
{
    IDE_TEST(mPool.initialize(IDU_MEM_MMC,
                              (SChar *)"MMC_TRANS_POOL",
                              ID_SCALABILITY_SYS,
                              ID_SIZEOF(mmcTransObj),
                              4,
                              IDU_AUTOFREE_CHUNK_LIMIT,           /* ChunkLimit */
                              ID_TRUE,                            /* UseMutex */
                              IDU_MEM_POOL_DEFAULT_ALIGN_SIZE,    /* AlignByte */
                              ID_FALSE,                           /* ForcePooling */
                              ID_TRUE,                            /* GarbageCollection */
                              ID_TRUE,                            /* HWCacheLine */
                              IDU_MEMPOOL_TYPE_LEGACY) != IDE_SUCCESS);

    IDE_TEST(mSharePool.initialize(IDU_MEM_MMC,
                                   (SChar *)"MMC_SHARE_TRANS_POOL",
                                   ID_SCALABILITY_SYS,
                                   ID_SIZEOF(mmcTransObj) + ID_SIZEOF(mmcTransShareInfo),
                                   4,
                                   IDU_AUTOFREE_CHUNK_LIMIT,           /* ChunkLimit */
                                   ID_TRUE,                            /* UseMutex */
                                   IDU_MEM_POOL_DEFAULT_ALIGN_SIZE,    /* AlignByte */
                                   ID_FALSE,                           /* ForcePooling */
                                   ID_TRUE,                            /* GarbageCollection */
                                   ID_TRUE,                            /* HWCacheLine */
                                   IDU_MEMPOOL_TYPE_LEGACY) != IDE_SUCCESS);

    return IDE_SUCCESS;
    IDE_EXCEPTION_END;
    return IDE_FAILURE;
}

IDE_RC mmcTrans::finalize()
{
    IDE_TEST(mPool.destroy() != IDE_SUCCESS);
    IDE_TEST(mSharePool.destroy() != IDE_SUCCESS);

    return IDE_SUCCESS;
    IDE_EXCEPTION_END;
    return IDE_FAILURE;
}

/*
 * only transaction alloc from mPool
 */
IDE_RC mmcTrans::allocRaw( mmcTransObj **aTrans )
{
    mmcTransObj *sTrans = NULL;

    IDE_TEST(mPool.alloc((void **)&sTrans) != IDE_SUCCESS);
    sTrans->mShareInfo = NULL;
    *aTrans = sTrans;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    if ( sTrans != NULL )
    {
        (void)mPool.memfree(sTrans);
        sTrans = NULL;
    }

    return IDE_FAILURE;
}

/*
 * only transaction free from mPool
 */
IDE_RC mmcTrans::freeRaw( mmcTransObj *aTrans )
{
    IDE_DASSERT(aTrans->mShareInfo == NULL);
    IDE_TEST(mPool.memfree(aTrans) != IDE_SUCCESS);
    aTrans = NULL;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC mmcTrans::alloc(mmcSession *aSession, mmcTransObj **aTrans)
{
    //fix BUG-18117
    mmcTransObj * sTrans = NULL;
    mmcTransObj * sNewShareTrans = NULL;
    mmcTransObj * sNewTrans = NULL;
    mmcTransObj * sShareTrans = NULL;
    idBool        sIsSharePoolAlloc = ID_FALSE;
    idBool        sIsTransInit = ID_FALSE;
    idBool        sIsPoolAlloc = ID_FALSE;

    IDU_FIT_POINT( "mmcTrans::alloc::alloc::Trans" );

    if ( aSession != NULL )
    {
        if ( aSession->isShareableTrans() == ID_TRUE )
        {
            IDE_TEST(mSharePool.alloc((void **)&sNewShareTrans) != IDE_SUCCESS);
            sIsSharePoolAlloc = ID_TRUE;
            IDE_TEST(sNewShareTrans->mSmiTrans.initialize() != IDE_SUCCESS);
            sIsTransInit = ID_TRUE;
            sNewShareTrans->mShareInfo = (mmcTransShareInfo*)((SChar*)sNewShareTrans + ID_SIZEOF(mmcTransObj));
            initTransShareInfo(sNewShareTrans->mShareInfo);
            setTransShardNodeName(sNewShareTrans, aSession->getShardNodeName());

            /* prevent new session add/remove */
            mmtSessionManager::lock();
            mmtSessionManager::findShareTransLockNeeded(aSession, &sShareTrans);
            if ( sShareTrans != NULL )
            {
                sShareTrans->mShareInfo->mAllocRefCnt++;
                aSession->mTrans = sShareTrans;
                mmtSessionManager::unlock();

                /*already exist share transaction, preallocated transaction free*/
                unsetTransShardNodeName(sNewShareTrans);
                finiTransShareInfo(sNewShareTrans->mShareInfo);
                sNewShareTrans->mShareInfo = NULL;
                sIsTransInit = ID_FALSE;
                IDE_TEST(sNewShareTrans->mSmiTrans.destroy(NULL) != IDE_SUCCESS);
                sIsSharePoolAlloc = ID_FALSE;
                IDE_TEST(mSharePool.memfree(sNewShareTrans) != IDE_SUCCESS);
                /*use already exist share transaction*/
                sTrans = sShareTrans;
            }
            else
            {
                /*no exist share transaction, use allocated new shareable trans*/
                sNewShareTrans->mShareInfo->mAllocRefCnt++;
                aSession->mTrans = sNewShareTrans;
                mmtSessionManager::unlock();
                sTrans = sNewShareTrans;
            }
        }
        else
        {
            /*sTrans = NULL, do not share trans*/
        }
    }
    else
    {
        /*sTrans = NULL*/
    }

    /*no shard or not share trans autocommit or sql prepare*/
    if ( sTrans == NULL )
    {
        IDE_TEST(mmcTrans::allocRaw(&sNewTrans) != IDE_SUCCESS);
        sIsPoolAlloc = ID_TRUE;
        IDE_TEST(sNewTrans->mSmiTrans.initialize() != IDE_SUCCESS);
        sIsTransInit = ID_TRUE;
        sTrans = sNewTrans;
    }
    *aTrans = sTrans;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    if ( sIsPoolAlloc == ID_TRUE )
    {
        if ( sIsTransInit == ID_TRUE )
        {
            (void)sNewTrans->mSmiTrans.destroy(NULL);
        }
        (void)mmcTrans::freeRaw(sNewTrans);
        sNewTrans = NULL;
    }

    if ( sIsSharePoolAlloc == ID_TRUE )
    {
        if ( sIsTransInit == ID_TRUE )
        {
            (void)sNewShareTrans->mSmiTrans.destroy(NULL);
        }
        (void)mSharePool.memfree(sNewShareTrans);
        sNewShareTrans = NULL;
    }

    return IDE_FAILURE;
}

IDE_RC mmcTrans::free(mmcSession *aSession, mmcTransObj *aTrans)
{
    UInt sAllocRefCnt = 0;

    if ( ( aSession != NULL ) && ( isShareableTrans(aTrans) == ID_TRUE ) )
    {
        IDE_DASSERT(aSession->mTrans == aTrans);
        /* prevent new session add & later must raise concurrency*/
        mmtSessionManager::lock();
        sAllocRefCnt = --(aTrans->mShareInfo->mAllocRefCnt);
        aSession->mTrans = NULL;
        mmtSessionManager::unlock();
        if ( sAllocRefCnt == 0 )
        {
            IDE_DASSERT(aTrans->mSmiTrans.isBegin() == ID_FALSE);
            unsetTransShardNodeName(aTrans);
            finiTransShareInfo(aTrans->mShareInfo);
            aTrans->mShareInfo = NULL;
            IDE_TEST(aTrans->mSmiTrans.destroy(NULL) != IDE_SUCCESS);
            IDE_TEST(mSharePool.memfree(aTrans) != IDE_SUCCESS);
        }
        else
        {
            /*do nothing, using trans in the other session*/
        }
    }
    else
    {
        IDE_DASSERT(isShareableTrans(aTrans) != ID_TRUE);
        IDE_TEST(aTrans->mSmiTrans.destroy( NULL ) != IDE_SUCCESS);
        aTrans->mShareInfo = NULL;
        IDE_TEST(mPool.memfree(aTrans) != IDE_SUCCESS);
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/*
 * beginXA: xa 프로토콜을 통해서 들어온 트랜잭션의 sm 모듈에 대한 트랜잭션 begin 처리로 
 * Session의 일반 트랜잭션에 영향을 주지 않음. 
 */
void mmcTrans::beginXA( mmcTransObj *aTrans,
                        idvSQL      *aStatistics,
                        UInt         aFlag )
{
    ULong sDummyEventFlag = 0;
    mmcTrans::beginRaw(aTrans, aStatistics, aFlag, &sDummyEventFlag);
}

/*
 * begin: Session과 연관하여 들어온 일반적인 트랜잭션 begin 상황에서 사용하는 함수
 */
void mmcTrans::begin( mmcTransObj *aTrans,
                      idvSQL      *aStatistics,
                      UInt         aFlag,
                      mmcSession  *aSession )
{
    if ( isShareableTrans(aTrans) == ID_TRUE )
    {
        lock(aTrans);

        mmcTrans::beginRaw(aTrans, aStatistics, aFlag, aSession->getEventFlag());
        aTrans->mShareInfo->mTransRefCnt++;

        if ( aSession->isShardLibrarySession() == ID_TRUE )
        {
            addDelegatedSession(aTrans->mShareInfo,aSession);
        }

        unlock(aTrans);
    }
    else
    {
        mmcTrans::beginRaw(aTrans, aStatistics, aFlag, aSession->getEventFlag());
    }

    if ( aSession->getCommitMode() == MMC_COMMITMODE_NONAUTOCOMMIT )
    {
        IDE_DASSERT(aSession->getTransBegin() == ID_FALSE);
        aSession->setTransBegin(ID_TRUE);
    }
    else
    {
        /* Nothing to do */
    }

    /* BUG-46913 */
    clearAndSetSessionInfoAfterBegin( aSession,
                                      aTrans );
}

/**
 * 설명 :
 *   서버사이드 샤딩과 관련된 함수.
 *   사용 가능한 XA 트랜잭션을 찾는다.
 *
 * @param aXID[OUT]    XA Transaction ID
 * @param aFound[OUT]  성공/실패 플래그
 * @param aSlotID[OUT] Slot ID
 * @return 성공하면 IDE_SUCCESS, 아니면 IDE_FAILURE
 **/
IDE_RC mmcTrans::findPreparedTrans( ID_XID *aXID,
                                    idBool *aFound,
                                    SInt   *aSlotID )
{
    SInt            sSlotID = -1;
    ID_XID          sXid;
    timeval         sTime;
    smiCommitState  sTxState;
    idBool          sFound = ID_FALSE;

    while ( 1 )
    {
        IDE_TEST( smiXaRecover( &sSlotID, &sXid, &sTime, &sTxState )
                  != IDE_SUCCESS );

        if ( sSlotID < 0 )
        {
            break;
        }
        else
        {
            /* Nothing to do */
        }

        if ( ( sTxState == SMX_XA_PREPARED ) &&
             ( mmdXid::compFunc( &sXid, aXID ) == 0 ) )
        {
            sFound = ID_TRUE;
            break;
        }
        else
        {
            /* Nothing to do */
        }
    }

    if ( aSlotID != NULL )
    {
        *aSlotID = sSlotID;
    }
    else
    {
        /* Nothing to do */
    }

    *aFound = sFound;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**
 * 설명 :
 *   서버사이드 샤딩과 관련된 함수.
 *   트랜잭이 sIsReadOnly이 아닌 경우에, 사용할 수 있는 2PC용 XA 트랜잭션을
 *   찾아서 XID를 Transaction에 설정하고, 반환한다.
 *
 * @param aTrans[IN]     smiTrans
 * @param aSession[IN]   mmcSession
 * @param aXID[OUT]      XA Transaction ID
 * @param aReadOnly[OUT] 트랜잭션의 ReadOnly 속성
 * @return 성공하면 IDE_SUCCESS, 아니면 IDE_FAILURE
 **/
IDE_RC mmcTrans::prepareForShard( mmcTransObj *aTrans,
                                  mmcSession  *aSession,
                                  ID_XID      *aXID,
                                  idBool      *aReadOnly )
{
    idBool  sIsReadOnly = ID_FALSE;
    idBool  sFound      = ID_FALSE;

    IDE_TEST( aTrans->mSmiTrans.isReadOnly( &sIsReadOnly ) != IDE_SUCCESS );

    if ( sIsReadOnly == ID_TRUE )
    {
        /* dblink나 shard로 인해 commit이 필요할 수 있다. */
        sIsReadOnly = dkiIsReadOnly( aSession->getDatabaseLinkSession() );
    }
    else
    {
        /* Nothing to do */
    }

    *aReadOnly = sIsReadOnly;

    if ( sIsReadOnly == ID_FALSE )
    {
        IDE_TEST( findPreparedTrans( aXID, &sFound ) != IDE_SUCCESS );
        IDE_TEST_RAISE( sFound == ID_TRUE, ERR_XID_INUSE );

        if ( isShareableTrans(aTrans) == ID_TRUE )
        {
            IDE_TEST( mmcTrans::prepareXA( aTrans, 
                                           aXID, 
                                           aSession ) != IDE_SUCCESS );
        }
        else
        {
            IDE_TEST( aTrans->mSmiTrans.prepare( aXID ) != IDE_SUCCESS );
            aSession->setTransPrepared( aXID );
        }
    }
    else
    {
        if ( isShareableTrans(aTrans) == ID_TRUE )
        {
            lockRecursive( aTrans );

            aTrans->mShareInfo->mIsGlobalTx = ID_TRUE;

            unlockRecursive( aTrans );
        }
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION(ERR_XID_INUSE)
    {
        IDE_SET(ideSetErrorCode(mmERR_ABORT_XID_INUSE));
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/*
 * prepareXA: xa 프로토콜을 통해서 들어온 트랜잭션의 sm/dk 모듈에 대한 트랜잭션 prepare 처리로
 * Session이 갖고 있는 트랜잭션에 영향을 주지 않음.
 */
IDE_RC mmcTrans::prepareXA( mmcTransObj *aTrans, 
                            ID_XID      *aXID, 
                            mmcSession  *aSession )
{
    smiTrans *sTrans  = mmcTrans::getSmiTrans(aTrans);
    idBool    sIsLock = ID_FALSE;

    IDE_DASSERT(aSession != NULL);

    sTrans->setStatistics(aSession->getStatSQL());

    if ( aSession->getTransPrepared() == ID_FALSE )
    {
        IDE_TEST(dkiCommitPrepare(aSession->getDatabaseLinkSession()) != IDE_SUCCESS);
    }

    if ( isShareableTrans(aTrans) == ID_TRUE )
    {
        lockRecursive( aTrans );
        sIsLock = ID_TRUE;

        IDE_TEST(mmcTrans::getSmiTrans(aTrans)->prepare(aXID) != IDE_SUCCESS);
        
        aTrans->mShareInfo->mIsGlobalTx = ID_TRUE;
        sIsLock = ID_FALSE;
        unlockRecursive( aTrans );
        
        aSession->setTransPrepared(aXID);
    }
    else
    {
        IDE_TEST(mmcTrans::getSmiTrans(aTrans)->prepare(aXID) != IDE_SUCCESS);
        aSession->setTransPrepared(aXID);
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;
    
    if ( sIsLock == ID_TRUE )
    {
        unlockRecursive( aTrans );
    }

    return IDE_FAILURE;
}

IDE_RC mmcTrans::endPending( ID_XID *aXID,
                             idBool  aIsCommit )
{
    smiTrans        sTrans;
    SInt            sSlotID;
    smSCN           sDummySCN;
    UChar           sXidString[XID_DATA_MAX_LEN];
    idBool          sFound = ID_FALSE;

    IDE_TEST( findPreparedTrans( aXID, &sFound, &sSlotID ) != IDE_SUCCESS );

    if ( sFound == ID_TRUE )
    {
        /* 해당 세션이 살아있는 경우 commit/rollback할 수 없다. */
        IDE_TEST_RAISE( mmtSessionManager::existSessionByXID( aXID ) == ID_TRUE,
                        ERR_XID_INUSE );

        IDE_ASSERT( sTrans.initialize() == IDE_SUCCESS );

        IDE_ASSERT( sTrans.attach(sSlotID) == IDE_SUCCESS );

        if ( aIsCommit == ID_TRUE )
        {
            if ( sTrans.commit( &sDummySCN ) != IDE_SUCCESS )
            {
                (void)idaXaConvertXIDToString(NULL, aXID, sXidString, XID_DATA_MAX_LEN);

                (void)ideLog::logLine( IDE_SERVER_0,
                                       "#### mmcTrans::commitPending (XID:%s) commit failed",
                                       sXidString );
            }
            else
            {
                /* Nothing to do */
            }
        }
        else
        {
            if ( sTrans.rollback( NULL ) != IDE_SUCCESS )
            {
                (void)idaXaConvertXIDToString(NULL, aXID, sXidString, XID_DATA_MAX_LEN);

                (void)ideLog::logLine( IDE_SERVER_0,
                                       "#### mmcTrans::commitPending (XID:%s) rollback failed",
                                       sXidString );
            }
            else
            {
                /* Nothing to do */
            }
        }

        IDE_ASSERT( sTrans.destroy( NULL ) == IDE_SUCCESS );
    }
    else
    {
        /* Nothing to do */
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION(ERR_XID_INUSE)
    {
        IDE_SET(ideSetErrorCode(mmERR_ABORT_XID_INUSE));
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/*
 * commit: Session과 연관하여 들어온 일반적인 트랜잭션 commit 상황에서 사용하는 함수
 */
IDE_RC mmcTrans::commit( mmcTransObj *aTrans,
                         mmcSession  *aSession,
                         UInt         aTransReleasePolicy,
                         idBool       aIsSqlPrepare )
{
    if ( aSession->getTransPrepared() == ID_FALSE )
    {
        IDE_TEST( dkiCommitPrepare( aSession->getDatabaseLinkSession() )
                  != IDE_SUCCESS );
    }
    else
    {
        // BUG-45411, BUG-45826
        IDU_FIT_POINT( "mmcTrans::commit::AlreadyPrepared" );
    }

    IDE_TEST( commitLocal( aTrans, aSession, aTransReleasePolicy, aIsSqlPrepare )
              != IDE_SUCCESS );

    /* BUG-46092 */
    IDU_FIT_POINT( "mmcTrans::commit::AfterCommitLocal" );

    dkiCommit( aSession->getDatabaseLinkSession() );

    /* BUG-46100 Session SMN Update */
    aSession->checkAndFinalizeShardCoordinator();

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/*
 * commitXA: xa 프로토콜을 통해서 들어온 트랜잭션의 sm/dk 모듈에 대한 트랜잭션 commit 처리로 
 * Session이 갖고 있는 트랜잭션에 영향을 주지 않음.
 */
IDE_RC mmcTrans::commitXA( mmcTransObj *aTrans,
                           mmcSession  *aSession,
                           UInt         aTransReleasePolicy )
{
    ULong sDummyEventFlag = 0;
    smSCN          sDummySCN;

    if ( aSession != NULL )
    {
        if ( aSession->getTransPrepared() == ID_FALSE )
        {
            IDE_TEST( dkiCommitPrepare( aSession->getDatabaseLinkSession() )
                    != IDE_SUCCESS );
        }
        else
        {
            /*do nothing*/
        }

        IDE_TEST( mmcTrans::commitRaw( aTrans, 
                                       aSession->getEventFlag(), 
                                       aTransReleasePolicy, 
                                       &sDummySCN )
                != IDE_SUCCESS );

        dkiCommit( aSession->getDatabaseLinkSession() );

        /* BUG-46100 Session SMN Update */
        aSession->checkAndFinalizeShardCoordinator();

    }
    else
    {
        IDE_TEST( mmcTrans::commitRaw( aTrans, 
                                       &sDummyEventFlag, 
                                       aTransReleasePolicy, 
                                       &sDummySCN ) != IDE_SUCCESS );
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC mmcTrans::commit4Prepare( mmcTransObj *aTrans, 
                                 mmcSession  *aSession, 
                                 UInt         aTransReleasePolicy ) 
{
    IDE_TEST( commitLocal( aTrans, aSession, aTransReleasePolicy, ID_TRUE )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC mmcTrans::commit4PrepareWithFree( mmcTransObj *aTrans,
                                         mmcSession  *aSession,
                                         UInt         aTransReleasePolicy )
{
    smSCN sDummySCN;

    IDE_TEST( commitRaw( aTrans,
                         aSession->getEventFlag(),
                         aTransReleasePolicy,
                         &sDummySCN )
              != IDE_SUCCESS );
    (void)mmcTrans::free( NULL, aTrans );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC mmcTrans::commitForceDatabaseLink( mmcTransObj *aTrans,
                                          mmcSession  *aSession,
                                          UInt         aTransReleasePolicy,
                                          idBool       aIsSqlPrepare )
{
    IDE_TEST( dkiCommitPrepareForce( aSession->getDatabaseLinkSession() ) != IDE_SUCCESS );

    IDE_TEST( commitLocal( aTrans, aSession, aTransReleasePolicy, aIsSqlPrepare )
              != IDE_SUCCESS );

    dkiCommit( aSession->getDatabaseLinkSession() );

    /* BUG-46100 Session SMN Update */
    aSession->checkAndFinalizeShardCoordinator();

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

//PROJ-1436 SQL-Plan Cache.
IDE_RC mmcTrans::commit4Null( mmcTransObj */*aTrans*/,
                              mmcSession  */*aSession*/,
                              UInt         /*aTransReleasePolicy*/ )
{
    return IDE_SUCCESS;
}

/*
 * rollback; Session과 연관하여 들어온 일반적인 트랜잭션 rollback 상황에서 사용하는 함수
 */
IDE_RC mmcTrans::rollback( mmcTransObj *aTrans,
                           mmcSession  *aSession,
                           const SChar *aSavePoint,
                           idBool       aIsSqlPrepare,
                           UInt         aTransReleasePolicy )
{
    IDE_TEST( dkiRollbackPrepare( aSession->getDatabaseLinkSession(), aSavePoint ) != IDE_SUCCESS );

    // BUG-45411, BUG-45826
    IDU_FIT_POINT( "mmcTrans::rollback::afterDkiRollbackPrepare" );

    IDE_TEST( rollbackLocal( aTrans,
                             aSession,
                             aSavePoint,
                             aTransReleasePolicy,
                             aIsSqlPrepare )
              != IDE_SUCCESS );

    dkiRollback( aSession->getDatabaseLinkSession(), aSavePoint ) ;

    /* BUG-46100 Session SMN Update */
    aSession->checkAndFinalizeShardCoordinator();

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/*
 * rollbackXA: xa 프로토콜을 통해서 들어온 트랜잭션의 sm/dk 모듈에 대한 트랜잭션 rollback 처리로 
 * Session이 갖고 있는 트랜잭션에 영향을 주지 않음.
 */
IDE_RC mmcTrans::rollbackXA( mmcTransObj *aTrans,
                             mmcSession  *aSession,
                             UInt         aTransReleasePolicy )
{
    ULong sDummyEventFlag = 0;

    if ( aSession != NULL )
    {
        dkiRollbackPrepareForce( aSession->getDatabaseLinkSession() );

        IDE_TEST( rollbackRaw( aTrans, &sDummyEventFlag, aTransReleasePolicy) != IDE_SUCCESS );

        dkiRollback( aSession->getDatabaseLinkSession(), NULL ) ;

        /* BUG-46100 Session SMN Update */
        aSession->checkAndFinalizeShardCoordinator();
    }
    else
    {
        IDE_TEST( rollbackRaw( aTrans, &sDummyEventFlag, aTransReleasePolicy) != IDE_SUCCESS );
    }
    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC mmcTrans::rollbackForceDatabaseLink( mmcTransObj *aTrans,
                                            mmcSession  *aSession,
                                            UInt         aTransReleasePolicy )
{
    dkiRollbackPrepareForce( aSession->getDatabaseLinkSession() );

    IDE_TEST( rollbackLocal( aTrans,
                             aSession,
                             NULL,
                             aTransReleasePolicy )
              != IDE_SUCCESS );

    dkiRollback( aSession->getDatabaseLinkSession(), NULL  );

    /* BUG-46100 Session SMN Update */
    aSession->checkAndFinalizeShardCoordinator();

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC mmcTrans::savepoint( mmcTransObj *aTrans,
                            mmcSession  *aSession,
                            const SChar *aSavePoint )
{
    IDE_TEST( dkiSavepoint( aSession->getDatabaseLinkSession(), aSavePoint ) != IDE_SUCCESS );

    IDE_TEST( aTrans->mSmiTrans.savepoint(aSavePoint) != IDE_SUCCESS );

    // BUG-42464 dbms_alert package
    IDE_TEST( aSession->getInfo()->mEvent.savepoint( (SChar *)aSavePoint ) != IDE_SUCCESS );

    return IDE_SUCCESS;
    IDE_EXCEPTION_END;
    return IDE_FAILURE;
}

/* BUG-46785 Shard Stmt Partial rollback */
IDE_RC mmcTrans::shardStmtPartialRollback( mmcTransObj * aTrans )
{
    IDU_FIT_POINT_RAISE( "mmcTrans::shardStmtPartialRollback::checkImpSVP4Shard", 
                         ERR_NO_IMPSVP_SHARD );
    IDE_TEST_RAISE( smiTrans::checkImpSVP4Shard( &(aTrans->mSmiTrans) ) != ID_TRUE,
                    ERR_NO_IMPSVP_SHARD );

    IDE_TEST( smiTrans::abortToImpSVP4Shard( &(aTrans->mSmiTrans) ) != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_NO_IMPSVP_SHARD )
    {
        IDE_SET( ideSetErrorCode( mmERR_ABORT_INTERNAL_SERVER_ERROR_ARG,
                                  "there is no implicit savepoint for shard" )  );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/*
 *
 */ 
IDE_RC mmcTrans::commitLocal( mmcTransObj *aTrans,
                              mmcSession  *aSession,
                              UInt aTransReleasePolicy,
                              idBool aIsSqlPrepare  )
{
    smSCN  sCommitSCN;
    idBool sIsSMNCacheUpdated = ID_FALSE;
    ULong  sNewSMN            = ID_ULONG(0);

    /* non-autocommit 모드의 begin된 tx이거나, autocommit 모드의 tx이거나,
       xa의 tx이거나, prepare tx인 경우 */
    if ( ( aSession->getTransBegin() == ID_TRUE ) ||
         ( aSession->getCommitMode() == MMC_COMMITMODE_AUTOCOMMIT ) ||
         ( aIsSqlPrepare == ID_TRUE ) )
    {
        if ( ( ( aSession->getQciSession()->mQPSpecific.mFlag & QC_SESSION_SHARD_META_TOUCH_MASK ) ==
               QC_SESSION_SHARD_META_TOUCH_TRUE ) &&
             ( SDU_SHARD_SMN_CACHE_APPLY_ENABLE == 1 ) )
        {
            /* BUG-46090 Meta Node SMN 전파 */
            IDE_TEST( aSession->applyShardMetaChange( &(aTrans->mSmiTrans), &sNewSMN ) != IDE_SUCCESS );

            sIsSMNCacheUpdated = ID_TRUE;
        }
        else
        {
            /* Nothing to do */
        }

        if ( isShareableTrans( aTrans ) == ID_TRUE )
        {
            IDE_TEST( mmcTrans::commitShareableTrans( aTrans,
                                                      aSession,
                                                      aTransReleasePolicy,
                                                      & sCommitSCN )
                      != IDE_SUCCESS );
        }
        else
        {
            IDE_TEST( mmcTrans::commitRaw( aTrans, 
                                           aSession->getEventFlag(), 
                                           aTransReleasePolicy,
                                           &sCommitSCN  )
                        != IDE_SUCCESS );
        }

        IDV_SESS_ADD_DIRECT(aSession->getStatistics(), IDV_STAT_INDEX_COMMIT_COUNT, 1);

        if ( ( sIsSMNCacheUpdated == ID_TRUE ) &&
             ( sNewSMN != ID_ULONG(0) ) )
        {
            sdi::setSMNCacheForMetaNode( sNewSMN );

            /* BUG-46090 Meta Node SMN 전파 */
            aSession->clearShardDataInfo();
            sdi::finalizeSession( aSession->getQciSession() );
            aSession->setShardMetaNumber( sNewSMN );
        }
        else
        {
            /* Nothing to do */
        }
    }
    else
    {
        /* Nothing to do */
    }

    IDE_TEST( mmcTrans::doAfterCommit( aSession,
                                       aTransReleasePolicy,
                                       aIsSqlPrepare,
                                       & sCommitSCN )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC mmcTrans::rollbackLocal( mmcTransObj *aTrans,
                                mmcSession  *aSession,
                                const SChar *aSavePoint,
                                UInt aTransReleasePolicy,
                                idBool aIsSqlPrepare  )
{
    idBool sSetBlock = ID_FALSE;
    idBool sIsLock   = ID_FALSE;

    if (aSavePoint == NULL)
    {
        /* non-autocommit 모드의 begin된 tx이거나, autocommit 모드의 tx이거나,
           xa의 tx이거나, prepare tx인 경우 */
        if ( ( aSession->getTransBegin() == ID_TRUE ) ||
             ( aSession->getCommitMode() == MMC_COMMITMODE_AUTOCOMMIT ) ||
             ( aIsSqlPrepare == ID_TRUE ) )
        {
            if ( isShareableTrans(aTrans) == ID_TRUE )
            {
                lock(aTrans);
                sIsLock = ID_TRUE;

                if ( aTrans->mShareInfo->mTransRefCnt == 1 )
                {
                    IDE_TEST( mmcTrans::rollbackRaw( aTrans,
                                                     aSession->getEventFlag(),
                                                     aTransReleasePolicy )
                              != IDE_SUCCESS );
                }
                else
                {
                    /*nothing to do*/
                }
                if ( aSession->isShardLibrarySession() == ID_TRUE )
                {
                    removeDelegatedSession(aTrans->mShareInfo, aSession);
                }

                aTrans->mShareInfo->mTransRefCnt--;

                sIsLock = ID_FALSE;
                unlock(aTrans);
            }
            else
            {
                IDE_TEST( mmcTrans::rollbackRaw( aTrans,
                                                 aSession->getEventFlag(),
                                                 aTransReleasePolicy )
                          != IDE_SUCCESS );
            }
            IDV_SESS_ADD_DIRECT(aSession->getStatistics(), IDV_STAT_INDEX_ROLLBACK_COUNT, 1);
        }
    }
    else
    {
        //PROJ-1677 DEQUEUE  
        aSession->setPartialRollbackFlag();

        IDU_SESSION_SET_BLOCK(*aSession->getEventFlag());
        sSetBlock = ID_TRUE;

        IDE_TEST(aTrans->mSmiTrans.rollback(aSavePoint, aTransReleasePolicy) != IDE_SUCCESS);

        sSetBlock = ID_FALSE;
        IDU_SESSION_CLR_BLOCK(*aSession->getEventFlag());

        IDV_SESS_ADD_DIRECT(aSession->getStatistics(), IDV_STAT_INDEX_ROLLBACK_COUNT, 1);
    }

    if (aSavePoint == NULL)
    {
        if ( aIsSqlPrepare == ID_FALSE )
        {
            if ( aSession->getCommitMode() == MMC_COMMITMODE_NONAUTOCOMMIT )
            {
                aSession->setTransBegin(ID_FALSE);
            }
            else
            {
                /* Nothing to do */
            }

            if ( aSession->getTransPrepared() == ID_TRUE )
            {
                aSession->setTransPrepared(NULL);
            }
            else
            {
                /* Nothing to do */
            }

            (void)aSession->clearLobLocator();

            (void)aSession->clearEnqueue();

            //PROJ-1677 DEQUEUE 
            (void)aSession->flushDequeue();

            // PROJ-1407 Temporary Table
            qci::endTransForSession( aSession->getQciSession() );
        }
        else
        {
            /* Nothing to do */
        }

        if (aTransReleasePolicy == SMI_RELEASE_TRANSACTION)
        {
            aSession->getInfo()->mTransID = 0;
        }
        else
        {
            /* Nothing to do */
        }
    }
    else
    {
        /* Nothing to do */
    }

    if ( aIsSqlPrepare == ID_FALSE )
    {
        // BUG-42464 dbms_alert package
        IDE_TEST( aSession->getInfo()->mEvent.rollback( (SChar *)aSavePoint ) != IDE_SUCCESS );
    }
    else
    {
        /* Nothing to do */
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    if (sSetBlock == ID_TRUE)
    {
        IDU_SESSION_CLR_BLOCK(*aSession->getEventFlag());
    }
    else
    {
        /* Nothing to do */
    }

    if ( sIsLock == ID_TRUE )
    {
        unlock(aTrans);
        sIsLock = ID_FALSE;
    }

    return IDE_FAILURE;
}
