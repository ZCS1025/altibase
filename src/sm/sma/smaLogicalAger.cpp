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
 

/***********************************************************************
 * $Id: smaLogicalAger.cpp 82075 2018-01-17 06:39:52Z jina.kim $
 **********************************************************************/

#include <idl.h>
#include <ide.h>
#include <iduSync.h>
#include <idu.h>
#include <smErrorCode.h>
#include <smm.h>
#include <sma.h>
#include <smx.h>
#include <smi.h>
#include <sgmManager.h>

smmSlotList*    smaLogicalAger::mSlotList[3];
iduMutex        smaLogicalAger::mBlock;
iduMutex        smaLogicalAger::mCheckMutex;
smaOidList*     smaLogicalAger::mHead;
smaOidList*     smaLogicalAger::mTailLogicalAger;
smaOidList*     smaLogicalAger::mTailDeleteThread;
smaLogicalAger* smaLogicalAger::mLogicalAgerList;
ULong           smaLogicalAger::mAddCount;
ULong           smaLogicalAger::mHandledCount;
UInt            smaLogicalAger::mCreatedThreadCount;
UInt            smaLogicalAger::mRunningThreadCount;
idBool          smaLogicalAger::mIsInitialized = ID_FALSE;
iduMutex        smaLogicalAger::mMetaMtxForAddRequest;
iduMutex        smaLogicalAger::mMetaMtxForAddProcess;
ULong           smaLogicalAger::mAgingRequestOIDCnt;
ULong           smaLogicalAger::mAgingProcessedOIDCnt;
ULong           smaLogicalAger::mSleepCountOnAgingCondition;
iduMutex        smaLogicalAger::mAgerCountChangeMutex;
iduMutex        smaLogicalAger::mFreeNodeListMutex;
SInt            smaLogicalAger::mBlockFreeNodeCount;
UInt            smaLogicalAger::mIsParallelMode;

smaLogicalAger::smaLogicalAger() : idtBaseThread()
{

}


IDE_RC smaLogicalAger::initializeStatic()
{

    UInt i;

    mSlotList[0]     = NULL;
    mSlotList[1]     = NULL;
    mSlotList[2]     = NULL;
    mLogicalAgerList = NULL;

    /* TC/FIT/Limit/sm/sma/smaLogicalAger_alloc_malloc1.sql */
    IDU_FIT_POINT_RAISE( "smaLogicalAger::alloc::malloc1",
                          insufficient_memory );

    //fix bug-23007
    IDE_TEST_RAISE(iduMemMgr::malloc(IDU_MEM_SM_SMA,
                               ID_SIZEOF(smmSlotList),
                               (void**)&(mSlotList[0])) != IDE_SUCCESS,
                   insufficient_memory );


    IDE_TEST( mSlotList[0]->initialize( ID_SIZEOF(smaOidList),
                                        SMA_NODE_POOL_MAXIMUM,
                                        SMA_NODE_POOL_CACHE,
                                        NULL )
              != IDE_SUCCESS );

    /* TC/FIT/Limit/sm/sma/smaLogicalAger_alloc_malloc2.sql */
    IDU_FIT_POINT_RAISE( "smaLogicalAger::alloc::malloc2",
                          insufficient_memory );

    IDE_TEST_RAISE(iduMemMgr::malloc(IDU_MEM_SM_SMA,
                               ID_SIZEOF(smmSlotList),
                               (void**)&(mSlotList[1])) != IDE_SUCCESS,
                   insufficient_memory );


    IDE_TEST( mSlotList[1]->initialize( ID_SIZEOF(smaOidList),
                                        SMA_NODE_POOL_MAXIMUM,
                                        SMA_NODE_POOL_CACHE,
                                        mSlotList[0] )
              != IDE_SUCCESS );

    /* TC/FIT/Limit/sm/sma/smaLogicalAger_alloc_malloc3.sql */
    IDU_FIT_POINT_RAISE( "smaLogicalAger::alloc::malloc3",
                          insufficient_memory );

    IDE_TEST_RAISE(iduMemMgr::malloc(IDU_MEM_SM_SMA,
                               ID_SIZEOF(smmSlotList),
                               (void**)&(mSlotList[2])) != IDE_SUCCESS,
                   insufficient_memory );

    IDE_TEST( mSlotList[2]->initialize( ID_SIZEOF(smaOidList),
                                        SMA_NODE_POOL_MAXIMUM,
                                        SMA_NODE_POOL_CACHE,
                                        mSlotList[0] )
              != IDE_SUCCESS );

    IDE_TEST( mBlock.initialize((SChar*)"MEMORY_LOGICAL_GC_MUTEX",
                                IDU_MUTEX_KIND_NATIVE,
                                IDV_WAIT_INDEX_NULL ) != IDE_SUCCESS );

    IDE_TEST( mMetaMtxForAddRequest.initialize(
                  (SChar*)"MEMORY_LOGICAL_GC_META_MUTEX_FOR_ADD_REQUEST",
                  IDU_MUTEX_KIND_NATIVE,
                  IDV_WAIT_INDEX_NULL)
              != IDE_SUCCESS );

    IDE_TEST( mMetaMtxForAddProcess.initialize(
                  (SChar*)"MEMORY_LOGICAL_GC_META_MUTEX_FOR_ADD_PROCESS",
                  IDU_MUTEX_KIND_NATIVE,
                  IDV_WAIT_INDEX_NULL)
              != IDE_SUCCESS );

    IDE_TEST( mCheckMutex.initialize((SChar*)"MEMORY_LOGICAL_GC_CHECK_MUTEX",
                                     IDU_MUTEX_KIND_NATIVE,
                                     IDV_WAIT_INDEX_NULL)
              != IDE_SUCCESS );

    IDE_TEST( mAgerCountChangeMutex.initialize(
                  (SChar*)"MEMORY_LOGICAL_GC_COUNT_CHANGE_MUTEX",
                  IDU_MUTEX_KIND_NATIVE,
                  IDV_WAIT_INDEX_NULL) 
              != IDE_SUCCESS );

    IDE_TEST( mFreeNodeListMutex.initialize(
                (SChar*)"MEMORY_LOGICAL_GC_FREE_NODE_LIST_MUTEX",
                IDU_MUTEX_KIND_NATIVE,
                IDV_WAIT_INDEX_NULL) 
            != IDE_SUCCESS );

    mHead             = NULL;
    mTailLogicalAger  = NULL;
    mTailDeleteThread = NULL;

    mBlockFreeNodeCount = 0;

    mAddCount = 0;
    mHandledCount = 0;
    mSleepCountOnAgingCondition = 0;

    mAgingRequestOIDCnt = 0;
    mAgingProcessedOIDCnt = 0;


    /* TC/FIT/Limit/sm/sma/smaLogicalAger_alloc_malloc4.sql */
    IDU_FIT_POINT_RAISE( "smaLogicalAger::alloc::malloc4",
                          insufficient_memory );

    IDE_TEST_RAISE(iduMemMgr::malloc(IDU_MEM_SM_SMA,
                               ID_SIZEOF(smaLogicalAger) *
                               smuProperty::getMaxLogicalAgerCount(),
                               (void**)&(mLogicalAgerList)) != IDE_SUCCESS,
                   insufficient_memory );

    mCreatedThreadCount = smuProperty::getLogicalAgerCount();
    mRunningThreadCount = mCreatedThreadCount;


    IDE_ASSERT( mCreatedThreadCount <= smuProperty::getMaxLogicalAgerCount() );

    /* BUG-35179 Add property for parallel logical ager */
    mIsParallelMode = smuProperty::getParallelLogicalAger();
        
    for(i = 0; i < mCreatedThreadCount; i++)
    {
        new (mLogicalAgerList + i) smaLogicalAger;
        IDE_TEST(mLogicalAgerList[i].initialize(i) != IDE_SUCCESS);
    }

    mIsInitialized = ID_TRUE;

    return IDE_SUCCESS;

    IDE_EXCEPTION( insufficient_memory );
    {
        IDE_SET(ideSetErrorCode(idERR_ABORT_InsufficientMemory));
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;

}

IDE_RC smaLogicalAger::destroyStatic()
{

    UInt i;

    for(i = 0; i <  mCreatedThreadCount; i++)
    {
        IDE_TEST(mLogicalAgerList[i].destroy()
                 != IDE_SUCCESS);
    }

    IDE_TEST( mBlock.destroy() != IDE_SUCCESS );
    IDE_TEST( mMetaMtxForAddRequest.destroy() != IDE_SUCCESS );
    IDE_TEST( mMetaMtxForAddProcess.destroy() != IDE_SUCCESS );
    IDE_TEST( mCheckMutex.destroy() != IDE_SUCCESS );
    IDE_TEST( mAgerCountChangeMutex.destroy() != IDE_SUCCESS );
    IDE_TEST( mFreeNodeListMutex.destroy() != IDE_SUCCESS );
    
    IDE_TEST( mSlotList[2]->release( ) != IDE_SUCCESS );
    IDE_TEST( mSlotList[2]->destroy( ) != IDE_SUCCESS );

    IDE_TEST(iduMemMgr::free(mSlotList[2])
             != IDE_SUCCESS);
    mSlotList[2] = NULL;

    IDE_TEST( mSlotList[1]->release( ) != IDE_SUCCESS );
    IDE_TEST( mSlotList[1]->destroy( ) != IDE_SUCCESS );

    IDE_TEST(iduMemMgr::free(mSlotList[1])
             != IDE_SUCCESS);
    mSlotList[1] = NULL;

    IDE_TEST( mSlotList[0]->release( ) != IDE_SUCCESS );
    IDE_TEST( mSlotList[0]->destroy( ) != IDE_SUCCESS );

    IDE_TEST(iduMemMgr::free(mSlotList[0])
             != IDE_SUCCESS);
    mSlotList[0] = NULL;

    IDE_TEST(iduMemMgr::free(mLogicalAgerList)
             != IDE_SUCCESS);
    mLogicalAgerList = NULL;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;

}

IDE_RC smaLogicalAger::shutdownAll()
{
    UInt i;

    for(i = 0; i < mCreatedThreadCount; i++)
    {
        IDE_TEST(mLogicalAgerList[i].shutdown() != IDE_SUCCESS);
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smaLogicalAger::initialize( UInt aThreadID )
{
    SChar           sMutexName[128];

    mContinue         = ID_TRUE;

    mTimeOut = smuProperty::getAgerWaitMax();

    idlOS::snprintf( sMutexName,
                     128,
                     "MEMORY_LOGICAL_GC_THREAD_MUTEX_%"ID_UINT32_FMT,
                     aThreadID );

    /* BUG-35179 Add property for parallel logical ager
     * drop table, drop tablespace���� ���ü� ������ �ذ��ϱ� ���� �� thread����
     * lock�� �߰��Ѵ�. */
    IDE_TEST( mWaitForNoAccessAftDropTblLock.initialize(
                                        sMutexName,
                                        IDU_MUTEX_KIND_NATIVE,
                                        IDV_WAIT_INDEX_NULL ) != IDE_SUCCESS );

    IDE_TEST( start() != IDE_SUCCESS );
    IDE_TEST(waitToStart(0) != IDE_SUCCESS);

    SM_MAX_SCN( &mViewSCN );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;

}

IDE_RC smaLogicalAger::destroy( void )
{

    /* BUG-35179 Add property for parallel logical ager
     * drop table, drop tablespace���� ���ü� ������ �ذ��ϱ� ���� �� thread����
     * lock�� �߰��Ѵ�. */
    IDE_TEST( mWaitForNoAccessAftDropTblLock.destroy() != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}


IDE_RC smaLogicalAger::shutdown( void )
{


    // BUGBUG smi interface layer�����Ǹ� �׶� ����.
    //IDE_RC _smiSetAger( idBool aValue );

    mContinue = ID_FALSE;
    // BUGBUG smi interface layer�����Ǹ� �׶� ����.
    //IDE_TEST( _smiSetAger( ID_TRUE ) != IDE_SUCCESS );

    IDE_TEST( setAger( ID_TRUE ) != IDE_SUCCESS );

    IDE_TEST_RAISE( join() != IDE_SUCCESS, ERR_THREAD_JOIN );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_THREAD_JOIN );
    IDE_SET( ideSetErrorCode( smERR_FATAL_Systhrjoin ) );

    IDE_EXCEPTION_END;

    return IDE_FAILURE;

}

IDE_RC smaLogicalAger::setAger( idBool aValue )
{
    static idBool sValue = ID_TRUE;

    if( aValue != sValue )
    {
        if( aValue == ID_TRUE )
        {
            IDE_TEST( smaLogicalAger::unblock() != IDE_SUCCESS );
        }
        else
        {
            IDE_TEST( smaLogicalAger::block() != IDE_SUCCESS );
        }
        sValue = aValue;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;

}

/*
    Ager�� �ʱ�ȭ�Ǿ����� ���θ� �����Ѵ�.

    BLOCK/UNBLOCK�� ȣ���ϱ� ���� �� �Լ��� ȣ���Ͽ�
    AGER�� �ʱ�ȭ�Ǿ������� �˻��� �� �ִ�.

    Ager�� �ʱ�ȭ���� ���� META���� �ܰ迡�� BLOCK/UNBLOCK��
    ȣ������ �ʵ��� �ϱ� ���� ���ȴ�.
*/
idBool smaLogicalAger::isInitialized( void )
{
    return mIsInitialized;
}


IDE_RC smaLogicalAger::block( void )
{
    UInt sStage = 0;

    // Ager�� �ʱ�ȭ �� META���¿����� Block/Unblock�� ȣ��Ǿ�� �Ѵ�.
    IDE_ASSERT( mIsInitialized == ID_TRUE );

    IDE_TEST( lock() != IDE_SUCCESS );
    sStage = 1;

    IDE_TEST( smaDeleteThread::lock() != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    IDE_PUSH();
    switch( sStage )
    {
        case 1:
            (void)unlock();
    }
    IDE_POP();

    return IDE_FAILURE;

}

IDE_RC smaLogicalAger::unblock( void )
{

    UInt sStage = 1;

    // Ager�� �ʱ�ȭ �� META���¿����� Block/Unblock�� ȣ��Ǿ�� �Ѵ�.
    IDE_ASSERT( mIsInitialized == ID_TRUE );

    IDE_TEST( smaDeleteThread::unlock() != IDE_SUCCESS );

    sStage = 0;
    IDE_TEST( unlock() != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    IDE_PUSH();
    switch( sStage )
    {
        case 1:
            (void)unlock();
    }
    IDE_POP();

    return IDE_FAILURE;

}

IDE_RC smaLogicalAger::addList(smTID         aTID,
                               idBool        aIsDDL,
                               smSCN        *aSCN,
                               smLSN        *aLSN,
                               UInt          aCondition,
                               smxOIDList*   aList,
                               void**        aAgerListPtr)
{

    UInt        sCondition;
    smaOidList* sOidList;

    if(aAgerListPtr != NULL)
    {
        *(smaOidList**)aAgerListPtr = NULL;
    }

    if( mSlotList[1] != NULL )
    {
        IDE_TEST( mSlotList[1]->allocateSlots( 1,
                                               (smmSlot**)&sOidList,
                                               SMM_SLOT_LIST_MUTEX_NEEDLESS )
                  != IDE_SUCCESS );

        sOidList->mSCN       = *aSCN;
        sOidList->mErasable  = ID_FALSE;
        sOidList->mCondition = aCondition;
        sOidList->mNext      = NULL;
        sOidList->mTransID   = aTID;
        SM_SET_SCN_INFINITE_AND_TID(&(sOidList->mKeyFreeSCN), aTID);


        if(aAgerListPtr != NULL)
        {
            sOidList->mFinished  = ID_FALSE;
        }
        else
        {
            sOidList->mFinished  = ID_TRUE;
        }

        sOidList->mLSN       = *aLSN;

        // BUG-15306
        // DummyOID�� ��쿣 add count�� ������Ű�� �ʴ´�.
        if( isDummyOID( sOidList ) != ID_TRUE )
        {
            mAddCount++;
        }

        if(aList != NULL)
        {
            IDE_ASSERT(aList->mOIDNodeListHead.mNxtNode != &(aList->mOIDNodeListHead));
            sOidList->mHead = aList->mOIDNodeListHead.mNxtNode;
            sOidList->mTail = aList->mOIDNodeListHead.mPrvNode;
        }
        else
        {
            sOidList->mHead = NULL;
            sOidList->mTail = NULL;
        }

        IDL_MEM_BARRIER;

        if( mHead != NULL )
        {
            mHead->mNext = sOidList;
            mHead       = sOidList;
        }
        else
        {
            mHead             = sOidList;
            mTailLogicalAger  = mHead;
            mTailDeleteThread = mHead;
        }

        if(aIsDDL == ID_TRUE)
        {
            sCondition = aCondition;


            IDE_TEST(addList(aTID, ID_FALSE, aSCN, aLSN, sCondition, NULL, NULL)
                     != IDE_SUCCESS);
        }

        if(aAgerListPtr != NULL)
        {
            *(smaOidList**)aAgerListPtr = sOidList;

        }
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;

}

/************************************************************************
 * Description :
 *    OID list�� OID�� �ϳ��� �޷� ������ �� �Լ��� ���ؼ�
 *    dummy OID�� �ϳ� �߰��Ѵ�. �̷��� �����ν�
 *    OID list�� ó�� �ȵǰ� ���� �ִ� �ϳ��� OID�� ó���ϵ��� �Ѵ�.
 *
 *    BUG-15306�� ���� �߰��� �Լ�
 ************************************************************************/
IDE_RC smaLogicalAger::addDummyOID( void )
{
    SInt  sState = 0;
    smLSN sNullLSN;
    smSCN sNullSCN;

    // OID list�� �ϳ��� ������ �� �Լ��� �ҷ�����.
    // ������ OID list�� ���÷� add�ǰ� �ֱ� ������
    // mutex�� ��� �ٽ��ѹ� �˻��ؾ� �Ѵ�.

    IDE_TEST(smxTransMgr::lock() != IDE_SUCCESS);
    sState = 1;

    if( ( mHead != NULL )                   &&
        ( mTailLogicalAger == mHead )       &&
        ( mTailLogicalAger->mNext == NULL ) &&
        ( isDummyOID( mHead ) == ID_FALSE ) )
    {
        SM_LSN_INIT( sNullLSN );
        SM_INIT_SCN( &sNullSCN );

        IDE_TEST( addList( 0,              // aTID,
                           ID_FALSE,       // aIsDDL
                           &sNullSCN,      // aSCN
                           &sNullLSN,      // aLSN
                           0,              // aCondition
                           NULL,           // aList,
                           NULL )          // aAgerListPtr
                  != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do...
    }

    sState = 0;
    IDE_TEST(smxTransMgr::unlock() != IDE_SUCCESS);

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    if( sState == 1 )
    {
        IDE_ASSERT(smxTransMgr::unlock() == IDE_SUCCESS);
    }

    return IDE_FAILURE;
}

void smaLogicalAger::setOIDListFinished( void* aOIDList, idBool aFlag)
{
    ((smaOidList*)aOIDList)->mFinished = aFlag;
}

/*
    Aging�� OID�� ����Ű�� Index Slot�� �����Ѵ�.

    [IN] aOIDList          - Aging�� OID List
    [IN] aAgingFilter      - Instant Aging�� ������ Filter
                             ( smaDef.h�ּ����� )
    [OUT] aDidDelOldKey -
 */
IDE_RC smaLogicalAger::deleteIndex(smaOidList            * aOIDList,
                                   smaInstantAgingFilter * aAgingFilter,
                                   UInt                  * aDidDelOldKey )
{

    smxOIDNode*     sNode;
    smxOIDInfo*     sCursor = NULL;
    smxOIDInfo*     sFence;
    UInt            sCondition;
    smcTableHeader* sLastTable;
    smcTableHeader* sTable;
    smcTableHeader* sNextTable;
    smcTableHeader* sCurTable;
    UInt            sStage = 0;
    
    sCondition  = aOIDList->mCondition|SM_OID_ACT_AGING_INDEX;
    sLastTable  = NULL;
    sTable      = NULL;
    sNode       = aOIDList->mTail;

    if(sNode != NULL)
    {
        do
        {
            sCursor = sNode->mArrOIDInfo + sNode->mOIDCnt - 1;
            sFence  = sNode->mArrOIDInfo - 1 ;
            for( ; sCursor != sFence; sCursor-- )
            {
                /* BUG-17417 V$Ager������ Add OID������ ���� Ager��
                 *           �ؾ��� �۾��� ������ �ƴϴ�.
                 *
                 * �ϳ��� OID�� ���ؼ� Aging�� �����ϸ�
                 * mAgingProcessedOIDCnt�� 1���� ��Ų��. */
                if( smxOIDList::checkIsAgingTarget( aOIDList->mCondition,
                                                    sCursor ) == ID_TRUE )
                {
                    if( aAgingFilter == NULL ) // Instant Aging Filter �ƴ�?
                    {
                        IDE_ASSERT( lockMetaAddProcMtx() == IDE_SUCCESS );
                        mAgingProcessedOIDCnt++;
                        IDE_DASSERT( mAgingRequestOIDCnt >= mAgingProcessedOIDCnt );
                        IDE_ASSERT( unlockMetaAddProcMtx() == IDE_SUCCESS );
                    }
                    else
                    {
                        /* InstantAging�� ���.  Aging Filter���ǿ� ����? */
                        if ( isAgingFilterTrue(
                                 aAgingFilter,
                                 sCursor->mSpaceID,
                                 sCursor->mTableOID ) == ID_TRUE )
                        {
                            IDE_ASSERT( lockMetaAddProcMtx() == IDE_SUCCESS );
                            mAgingProcessedOIDCnt++;
                            IDE_DASSERT( mAgingRequestOIDCnt >= mAgingProcessedOIDCnt );
                            IDE_ASSERT( unlockMetaAddProcMtx() == IDE_SUCCESS );
                        }
                    }
                }
                else
                {
                    /* nothing to do ... */
                }

                if ( (( sCursor->mFlag & sCondition ) == sCondition ) &&
                     (( sCursor->mFlag & SM_OID_ACT_COMPRESSION ) == 0) )
                {
                    IDE_ASSERT( sCursor->mTableOID != SM_NULL_OID );
                    IDE_ASSERT( smcTable::getTableHeaderFromOID( sCursor->mTableOID,
                                                                 (void**)&sCurTable )
                                == IDE_SUCCESS );

                    if( sCurTable != sLastTable )
                    {
                        sNextTable = sCurTable;

                        if(smcTable::isDropedTable(sNextTable) == ID_TRUE)
                        {
                            continue;
                        }

                        if( sStage == 1)
                        {
                            sStage = 0;
                            IDE_TEST( smcTable::unlatch( sTable )
                                      != IDE_SUCCESS );
                        }

                        sLastTable  = sNextTable;
                        sTable      = sNextTable;

                        IDE_TEST( smcTable::latchShared( sTable )
                                  != IDE_SUCCESS );
                        sStage = 1;
                    }

                    if( aAgingFilter != NULL ) // Instant Aging Filter ����?
                    {
                        // Aging Filter���ǿ� ����?
                        if ( isAgingFilterTrue(
                                 aAgingFilter,
                                 sCursor->mSpaceID,
                                 sCursor->mTableOID ) == ID_TRUE )
                        {
                            // Aging�� �ǽ�.
                        }
                        else
                        {
                            continue;
                        }
                    }
                    else
                    {
                        /* BUG-41026 */
                        IDU_FIT_POINT( "2.BUG-41026@smaLogicalAger::run::wakeup_1" );
                        IDU_FIT_POINT( "3.BUG-41026@smaLogicalAger::run::sleep_2" );
                        
                        // Aging Filter�� ������� �ʴ� �Ϲ����� ����̴�.
                        // �Ʒ��� �ڵ带 �����Ͽ� Aging�ǽ�.
                        sCursor->mFlag &= ~SM_OID_ACT_AGING_INDEX;
                    }

                    IDE_ASSERT( sCursor->mTargetOID != SM_NULL_OID );
                    IDE_ASSERT( sTable != NULL );

                    IDU_FIT_POINT( "1.PROJ-1407@smaLogicalAger::deleteIndex" );

                    //old version row�� �����״� index���� key slot�� �����Ѵ�.
                    IDE_TEST( freeOldKeyFromIndexes(sTable,
                                                    sCursor->mTargetOID,
                                                    aDidDelOldKey)
                              != IDE_SUCCESS);

                    // �������� Aging�� �� Index Slot�� ����
                    // Aging�� �ѹ� �� �ϴ����� ������ Flag�� ����.
                    sCursor->mFlag &= ~SM_OID_ACT_AGING_INDEX;
                }
            }
            sNode = sNode->mPrvNode;
        }
        while( sNode != NULL );

        if( sStage == 1)
        {
            sStage = 0;
            IDE_TEST( smcTable::unlatch( sTable ) != IDE_SUCCESS );
        }
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    /* BUG-32655 [sm-mem-index] The MMDB Ager must not ignore the failure of
     * index aging. */
    if( sCursor != NULL )
    {
        ideLog::log( IDE_SM_0, 
                     "smaLogicalAger::deleteIndex\n"
                     "sCursor->mTableOID  : %"ID_UINT64_FMT"\n"
                     "sCursor->mTargetOID : %"ID_UINT64_FMT"\n"
                     "sCursor->mSpaceID   : %"ID_UINT32_FMT"\n"
                     "sCursor->mFlag      : %"ID_UINT32_FMT"\n",
                     sCursor->mTableOID,
                     sCursor->mTargetOID,
                     sCursor->mSpaceID,
                     sCursor->mFlag );
    }

    IDE_PUSH();
    
    switch( sStage )
    {
        case 1:
            IDE_ASSERT( smcTable::unlatch( sTable ) == IDE_SUCCESS );
    }

    IDE_POP();

    return IDE_FAILURE;

}


/*********************************************************************
  Name: sdaGC::freeOldKeyFromIndexes
  Description:
  old verion  row�� �����״� old index key slot��
  table �� �ε���(��)���� ���� free�Ѵ�.
 *********************************************************************/
IDE_RC smaLogicalAger::freeOldKeyFromIndexes(smcTableHeader*  aTableHeader,
                                             smOID            aOldVersionRowOID,
                                             UInt*            aDidDelOldKey)
{
    UInt             i ;
    UInt             sIndexCnt;
    smnIndexHeader * sIndexHeader;
    SChar          * sRowPtr;
    idBool           sIsExistFreeKey = ID_FALSE;
   
    IDE_ASSERT( sgmManager::getOIDPtr( aTableHeader->mSpaceID,
                                       aOldVersionRowOID, 
                                       (void**)&sRowPtr)
                == IDE_SUCCESS );

    sIndexCnt =  smcTable::getIndexCount(aTableHeader);

    for(i = 0; i < sIndexCnt ; i++)
    {
        sIndexHeader = (smnIndexHeader*)smcTable::getTableIndex(aTableHeader,i);

        if( smnManager::isIndexEnabled( sIndexHeader ) == ID_TRUE )
        {
            IDE_ASSERT( sIndexHeader->mHeader != NULL );
            IDE_ASSERT( sIndexHeader->mModule != NULL );
            IDE_ASSERT( sIndexHeader->mModule->mFreeSlot != NULL );

            IDE_TEST( sIndexHeader->mModule->mFreeSlot( sIndexHeader, // aIndex
                                                        sRowPtr,
                                                        ID_FALSE, /*aIgnoreNotFoundKey*/
                                                        &sIsExistFreeKey )
                      != IDE_SUCCESS );

            IDE_ASSERT( sIsExistFreeKey == ID_TRUE );
            
            *aDidDelOldKey |= 1;
        }//if
    }//for

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/*********************************************************************
  Name: sdaGC::checkOldKeyFromIndexes
  Description:
  old verion  row�� �����״� old index key slot�� �����ϴ��� �˻��Ѵ�.
 *********************************************************************/
IDE_RC smaLogicalAger::checkOldKeyFromIndexes(smcTableHeader*  aTableHeader,
                                              smOID            aOldVersionRowOID,
                                              idBool         * aExistKey )
{
    UInt             i ;
    UInt             sIndexCnt;
    smnIndexHeader * sIndexHeader;
    SChar          * sRowPtr;
    UInt             sState = 0;

    IDE_TEST( smcTable::latchShared( aTableHeader )
              != IDE_SUCCESS );
    sState = 1;

    sIndexCnt    = smcTable::getIndexCount(aTableHeader);
    (*aExistKey) = ID_FALSE;

    IDE_ASSERT( sgmManager::getOIDPtr( aTableHeader->mSpaceID,
                                       aOldVersionRowOID, 
                                       (void**)&sRowPtr)
                == IDE_SUCCESS );

    for(i = 0; i < sIndexCnt ; i++)
    {
        sIndexHeader = (smnIndexHeader*)smcTable::getTableIndex(aTableHeader,i);

        if( ( smnManager::isIndexEnabled( sIndexHeader ) == ID_TRUE ) )
        {
            IDE_ASSERT( sIndexHeader->mHeader != NULL );
            IDE_ASSERT( sIndexHeader->mModule != NULL );

            if( sIndexHeader->mModule->mExistKey == NULL )
            {
                /* MRDB R-Tree�� mExistKey �Լ��� �����Ǿ� ���� �ʴ�. */
                if( sIndexHeader->mType == SMI_ADDITIONAL_RTREE_INDEXTYPE_ID )
                {
                    continue;
                }
                else
                {
                    IDE_ASSERT(0);
                }
            }

            IDE_TEST( sIndexHeader->mModule->mExistKey(
                            sIndexHeader, // aIndex
                            sRowPtr,
                            aExistKey )
                      != IDE_SUCCESS )
            if( *aExistKey == ID_TRUE )
            {
                /* Key�� �����ϸ� �ȵ�. �ٷ� ���� ����. */
                break;
            }
            else
            {
                /* �����ϴ� ���� ���� ���̽�.
                 * ���� Index�� ���� �Ѿ */
            }
        }//if
    }//for

    sState = 0;
    IDE_TEST( smcTable::unlatch( aTableHeader )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    if( sState == 1 )
    {
        (void) smcTable::unlatch( aTableHeader );
    }

    return IDE_FAILURE;
}



// added for A4
// smaPhysical ager�� ���ŵʿ� ����
// �߰��� �Լ��̸�, free node list�� �����Ѵ�.
// ���� �����ʹ� smnManager�� �ִ� List�� ����Ѵ�.
// �ε��� ����(memory b+ tree,R tree)�� ����
// free node list�� ���� ������ �ȴ�.
void smaLogicalAger::addFreeNodes(void*     aFreeNodeList,
                                  smnNode*   aNodes)
{
    smnFreeNodeList * sFreeNodeList;

    IDE_DASSERT( aFreeNodeList != NULL);
    IDE_DASSERT( aNodes != NULL);

    IDE_ASSERT( mFreeNodeListMutex.lock( NULL ) == IDE_SUCCESS );
     
    sFreeNodeList = (smnFreeNodeList*) aFreeNodeList;

    if( aNodes != NULL )
    {
        sFreeNodeList->mAddCnt++;
        if(sFreeNodeList->mAddCnt == 0)
        {
            sFreeNodeList->mHandledCnt = 0;
        }
        // ���� system view scn�� �����´�.
        // node�� index���� ������ �Ŀ� view SCN�� �����ؾ� �Ѵ�.
        smmDatabase::getViewSCN(&(aNodes->mSCN));
        if( sFreeNodeList->mHead != NULL )
        {
            // empty node list�� �߰�.
            aNodes->mNext        = NULL;
            sFreeNodeList->mHead->mNext = aNodes;
            sFreeNodeList->mHead       = aNodes;
        }
        else
        {
            aNodes->mNext  = NULL;
            IDL_MEM_BARRIER;
            sFreeNodeList->mHead = aNodes;
            sFreeNodeList->mTail  = aNodes;
        }
    }

    IDE_ASSERT( mFreeNodeListMutex.unlock() == IDE_SUCCESS );
}

//added for A4.
//index�������� �ִ� free node list���� traverse�ϸ鼭
//���� ��尡 ������ ��带 �����.
void smaLogicalAger::freeNodesIfPossible()
{

    smnNode*         sTail;
    smnNode*         sHead = NULL;
    smnFreeNodeList* sCurFreeNodeList;
    smSCN            sMinSCN;
    smTID            sTID;
    UInt             i;

    IDE_ASSERT( mFreeNodeListMutex.lock( NULL ) == IDE_SUCCESS );

    if( mBlockFreeNodeCount > 0 )
    {
        /* TASK-4990 changing the method of collecting index statistics
         * ������� �籸�� ������ Index�� ��ȸ�ϴ� ��.
         * FreeNode�� �������ν� Node�� ��Ȱ���� ����,
         * �̸� ���� ������� �籸���� ���� Index Node
         * ��ȸ�� TreeLock�� ���̵� ������ �� �ֵ��� �Ѵ�.*/
    }
    else
    {

        for(sCurFreeNodeList =  smnManager::mBaseFreeNodeList.mNext;
            sCurFreeNodeList != &smnManager::mBaseFreeNodeList;
            sCurFreeNodeList = sCurFreeNodeList->mNext)
        {
            if(sCurFreeNodeList->mTail == NULL)
            {
                continue;
            }

            // fix BUG-9620.
            ID_SERIAL_BEGIN(sHead = sCurFreeNodeList->mHead);
            ID_SERIAL_END(smxTransMgr::getMinMemViewSCNofAll(&sMinSCN, &sTID));

            // BUG-39122 parallel logical aging������ logical ager��
            // view SCN�� ����ؾ� �մϴ�.
            if( mIsParallelMode == SMA_PARALLEL_LOGICAL_AGER_ON )
            {
                for( i = 0; i < mCreatedThreadCount; i++ )
                {
                    if( SM_SCN_IS_LT( &(mLogicalAgerList[i].mViewSCN) ,&sMinSCN ))
                    {
                        SM_SET_SCN( &sMinSCN, &(mLogicalAgerList[i].mViewSCN) );
                    }
                }
            }

            while( sCurFreeNodeList->mTail->mNext != NULL  )
            {
                if(SM_SCN_IS_LT(&(sCurFreeNodeList->mTail->mSCN),&sMinSCN))
                {
                    sTail = sCurFreeNodeList->mTail;
                    sCurFreeNodeList->mTail = sCurFreeNodeList->mTail->mNext;
                    sCurFreeNodeList->mHandledCnt++;
                    IDE_TEST( sCurFreeNodeList->mFreeNodeFunc( sTail )
                              != IDE_SUCCESS );
                    if(sTail == sHead)
                    {
                        break;
                    }

                }//if
                else
                {
                    break;
                }
            }//while
        }//for
    } // if( mBlockFreeNodeCount > 0 )

    IDE_ASSERT( mFreeNodeListMutex.unlock() == IDE_SUCCESS );

    return;

    IDE_EXCEPTION_END;

    IDE_CALLBACK_FATAL( "smaLogicalAger" );

}

void smaLogicalAger::run()
{
    PDL_Time_Value   sTimeOut;
    idBool           sHandled;
    smSCN            sMinSCN;
    smaOidList*      sTail = NULL;
    smTID            sTID;
    idBool           sState = ID_FALSE;
    idBool           sDropTableLockState = ID_FALSE;
    UInt             sAgerWaitMin;
    UInt             sAgerWaitMax;
    UInt             sDidDelOldKey = 0;

  startPos:
    sState              = ID_FALSE;
    sDropTableLockState = ID_FALSE;

    sAgerWaitMin = smuProperty::getAgerWaitMin();
    sAgerWaitMax = smuProperty::getAgerWaitMax();

    while( mContinue == ID_TRUE )
    {
        if( mTimeOut > sAgerWaitMin )
        {
            sTimeOut.set( 0, mTimeOut );
            idlOS::sleep( sTimeOut );
        }

        if ( smuProperty::isRunMemGCThread() == SMU_THREAD_OFF )
        {
            // To Fix PR-14783
            // System Thread�� �۾��� �������� �ʵ��� �Ѵ�.
            mTimeOut = sAgerWaitMax;
            continue;
        }
        else
        {
            // Go Go
        }

        /* BUG-35179 Add property for parallel logical ager */
        IDE_TEST( lockWaitForNoAccessAftDropTbl() != IDE_SUCCESS );
        sDropTableLockState = ID_TRUE;

        IDU_FIT_POINT("1.smaLogicalAger::run");

        IDE_TEST( lock() != IDE_SUCCESS );
        sState = ID_TRUE;

        sHandled = ID_FALSE;

        if( mTailLogicalAger != NULL )
        {
            // BUG-15306
            // mTailLogicalAger�� �ϳ� �ۿ� ������ ó������ ���ϹǷ�
            // DummyOID�� �ϳ� �޾Ƽ� �ΰ��� �ǰ� �Ѵ�.
            if( ( isDummyOID( mHead ) != ID_TRUE ) && ( mTailLogicalAger->mNext == NULL ) )
            {
                IDE_TEST( addDummyOID() != IDE_SUCCESS );
            }

            // fix BUG-9620.
            smxTransMgr::getMinMemViewSCNofAll( &sMinSCN, &sTID );

            /* ���ġ�� ��Ȯ���� �˻��غ� */
#if defined(DEBUG)

            /* 
             * BUG-35179 Add property for parallel logical ager
             * ���� ���ġ �˻�� LogicalAger�� parallel�� ��������
             * ���� ���� ��ȿ�ϴ�. parallel�� �����Ұ�� ���ü������� ��񵿾�
             * ���ġ�� ��ġ���� ���� �� �ֱ� �����̴�.
             */
            if( ( mTailLogicalAger->mNext == NULL ) &&
                ( isDummyOID( mTailLogicalAger ) == ID_TRUE ) &&
                ( mIsParallelMode == SMA_PARALLEL_LOGICAL_AGER_OFF ) )
            {
                /* Transaction���� ���ü��� ���� Lock */
                IDE_TEST( smxTransMgr::lock() != IDE_SUCCESS );
                /* Lock �� �ٽ� Ȯ���غ� */
                if( ( mTailLogicalAger->mNext == NULL ) &&
                    ( isDummyOID( mTailLogicalAger ) == ID_TRUE ) )
                {
                    IDE_ASSERT( mAddCount == mHandledCount );
                    IDE_ASSERT( mAgingRequestOIDCnt == mAgingProcessedOIDCnt );
                }
                IDE_TEST( smxTransMgr::unlock() != IDE_SUCCESS );
            }
#endif

            while( (mTailLogicalAger->mNext != NULL) &&
                   (mTailLogicalAger->mFinished == ID_TRUE) )
            {
                if( SM_SCN_IS_LT( &(mTailLogicalAger->mSCN), &sMinSCN ) )
                {
                    sTail            = mTailLogicalAger;
                    mTailLogicalAger = mTailLogicalAger->mNext;

                    sDidDelOldKey = 0;

                    /* BUG-35179 Add property for parallel logical ager */
                    if( mIsParallelMode == SMA_PARALLEL_LOGICAL_AGER_ON )
                    {
                        sState = ID_FALSE;
                        IDE_TEST( unlock() != IDE_SUCCESS );

                        /* BUG-39122 parallel logical aging������
                         * logical ager�� view SCN�� ����ؾ� �մϴ�.
                         * ���� system view scn�� �����´�.*/
                        smmDatabase::getViewSCN( &mViewSCN );
                    }

                    IDE_TEST(deleteIndex( sTail,
                                          NULL, /* No Instant Aging Filter */
                                          &sDidDelOldKey )
                             != IDE_SUCCESS );

                    // delete key slot�� �Ϸ��� ��������
                    // system view scn�� ����.
                    if( sDidDelOldKey != 0 )
                    {
                        smmDatabase::getViewSCN( &( sTail->mKeyFreeSCN ) );
                    }
                    else
                    {
                        // old key�� ���� ���� ���� ��Ͻ�����
                        // physical aging���� �Ϸ��� �����̴�.
                        SM_SET_SCN( &( sTail->mKeyFreeSCN ), &( sTail->mSCN ) );
                    }

                    // BUG-15306
                    // dummy oid�� ó���� �� mHandledCount�� ������Ű�� �ʴ´�.
                    if( isDummyOID( sTail ) != ID_TRUE )
                    {
                        /* BUG-41026 */
                        acpAtomicInc( &mHandledCount );
                    }

                    sTail->mErasable  = ID_TRUE;
                    sHandled         = ID_TRUE;
                    
                    /* BUG-35179 Add property for parallel logical ager */
                    /* BUG-41026 lock ��ġ ���� */
                    if ( mIsParallelMode == SMA_PARALLEL_LOGICAL_AGER_ON )
                    {
                        SM_MAX_SCN( &mViewSCN );

                        IDE_TEST( lock() != IDE_SUCCESS );
                        sState = ID_TRUE;
                    }
                    else
                    {
                        /* nothing to do */
                    }
                }//if
                else
                {
                    //BUG-17371 [MMDB] Aging�� �и���� System�� ����ȭ ��
                    //                 Aging�� �и��� ������ �� ��ȭ��.
                    mSleepCountOnAgingCondition++;
                    break;
                }//else
            }//while

            //free nodes�鿡 ����  �����õ�.
            freeNodesIfPossible();
        }//if

        sState = ID_FALSE;
        IDE_TEST( unlock() != IDE_SUCCESS );

        /* BUG-35179 Add property for parallel logical ager */
        sDropTableLockState = ID_FALSE;
        IDE_TEST( unlockWaitForNoAccessAftDropTbl() != IDE_SUCCESS );

        if( sHandled == ID_TRUE )
        {
            mTimeOut >>= 1;
            mTimeOut = mTimeOut < sAgerWaitMin ?  sAgerWaitMin  : mTimeOut;
        }
        else
        {
            mTimeOut <<= 1;
            mTimeOut = mTimeOut > sAgerWaitMax ? sAgerWaitMax : mTimeOut;
        }
    }

    IDE_TEST( lock() != IDE_SUCCESS );
    sState = ID_TRUE;

    mRunningThreadCount--;
    while( (mTailLogicalAger != NULL) && (mRunningThreadCount == 0) )
    {
        sTail            = mTailLogicalAger;
        mTailLogicalAger = mTailLogicalAger->mNext;
        smmDatabase::getViewSCN((&sTail->mKeyFreeSCN));
        sTail->mErasable  = ID_TRUE;
        sHandled         = ID_TRUE;
    }

    sState = ID_FALSE;
    IDE_TEST( unlock() != IDE_SUCCESS );

    return;

    IDE_EXCEPTION_END;

    /* BUG-32655 [sm-mem-index] The MMDB Ager must not ignore the failure of
     * index aging. */
    if( sTail != NULL )
    {
        ideLog::log( IDE_SM_0, 
                     "smaLogicalAger::run\n"
                     "sTail->mSCN         : %"ID_UINT64_FMT"\n"
                     "sTail->mLSN         : %"ID_UINT32_FMT",%"ID_UINT32_FMT"\n"
                     "sTail->mKeyFreeSCN  : %"ID_UINT64_FMT"\n"
                     "sTail->mErasable    : %"ID_UINT32_FMT"\n"
                     "sTail->mFinished    : %"ID_UINT32_FMT"\n"
                     "sTail->mCondition   : %"ID_UINT32_FMT"\n"
                     "sTail->mTransID     : %"ID_UINT32_FMT"\n",
                     SM_SCN_TO_LONG( sTail->mSCN ),
                     sTail->mLSN.mFileNo,
                     sTail->mLSN.mOffset,
                     SM_SCN_TO_LONG( sTail->mKeyFreeSCN ),
                     sTail->mErasable,
                     sTail->mFinished,
                     sTail->mCondition,
                     sTail->mTransID );
    }

    IDE_ASSERT( 0 );

    IDE_PUSH();
    
    if( sState == ID_TRUE )
    {
        IDE_ASSERT( unlock() == IDE_SUCCESS );
    }

    if( sDropTableLockState == ID_TRUE )
    {
        IDE_ASSERT( unlockWaitForNoAccessAftDropTbl() == IDE_SUCCESS );
    }

    IDE_POP();

    IDE_WARNING(SM_TRC_LOG_LEVEL_WARNNING,
                SM_TRC_MAGER_WARNNING2);

    goto startPos;

}

/***********************************************************************
 * Description : Table Drop�� Commit�Ŀ� Transaction�� ���� �����ϱ⶧����
 *               Ager�� Drop Table�� ���ؼ� Aging�۾��� �ϸ� �ȵȴ�. �̸� ����
 *               Ager�� �ش� Table�� ���� Aging�۾��� �ϱ����� Table�� Drop��
 *               ������ �����Ѵ�. �׷��� �� Table�� Drop�Ǿ����� Check�ϰ� ������
 *               �� Table�� Transaction�� ���ؼ� Drop�� �� �ֱ� ������ �������ȴ�.
 *               �̶����� Ager�� ���� ����� mCheckMutex�� �ɰ� �۾��� �����Ѵ�.
 *               ���� Table Drop�� ������ Transaction�� mCheckMutex�� ��Ҵٴ�
 *               �̾߱�� �ƹ��� Ager�� Table�� �������� �ʰ� �ִٴ� ���� �����ϰ�
 *               ���� �� ���Ŀ� Ager�� �����Ҷ��� ���̺� Set�� Drop Flag��
 *               ���� �Ǿ� Table�� ���� Aging�۾��� �������� �ʴ´�.
 *
 * ���� BUG: 15047
 **********************************************************************/
void smaLogicalAger::waitForNoAccessAftDropTbl()
{
    UInt i;

    /* BUG-35179 Add property for parallel logical ager 
     * drop table, drop tablespace���� ���ü� ������ �ذ��ϱ� ���� �� thread����
     * lock�� �߰��Ѵ�.
     *
     * parallel logical ager�ڵ� �߰������� ������ lock(), unlock()�Լ���
     * OidList�� ��������� ������ ���� lock���Ҹ� �Ѵ�. ����
     * �� logical ager thread ���� �����ϴ� mWaitForNoAccessAftDropTblLock��
     * ��ƾ����� drop table, drop tablespace���� ���ü� ��� �ȴ�.
     */ 

    for( i = 0; i < mRunningThreadCount; i++ )
    {
        IDE_ASSERT( mLogicalAgerList[i].lockWaitForNoAccessAftDropTbl()
                    == IDE_SUCCESS );

        IDE_ASSERT( mLogicalAgerList[i].unlockWaitForNoAccessAftDropTbl()
                    == IDE_SUCCESS );
    }

    /* BUG-15969: �޸� Delete Thread���� ������ ����
     * Delete Thread�� �������� �ʴ´ٴ� ���� �����ؾ� �Ѵ�.*/
    smaDeleteThread::waitForNoAccessAftDropTbl();

    /*
       BUG-42760
       LEGACY Transaction�� OID LIst�� AGER ���� ó������ �ʰ�,
       Transaction�� �����Ҷ� Transactino���� ���� ó���Ѵ�.
       ����, AGER�� ���������� LEGACY Transaction������ Drop�� Table�� OID��
       ó������ �ʵ��� lock�� �߰��Ѵ�.
     */
    smxLegacyTransMgr::waitForNoAccessAftDropTbl();
}



/*
    Aging Filter���ǿ� �����ϴ��� üũ

    [IN] aAgingFilter - �˻��� Filter ����
    [IN] aTBSID       - Aging�Ϸ��� OID�� ���� Tablespace�� ID
    [IN] aTableOID    - Aging�Ϸ��� OID�� ���� Table�� OID

    <RETURN> - ���ǿ� ������ ��� ID_TRUE, �ƴϸ� ID_FALSE
 */
idBool smaLogicalAger::isAgingFilterTrue( smaInstantAgingFilter * aAgingFilter,
                                          scSpaceID               aTBSID,
                                          smOID                   aTableOID )
{
    IDE_DASSERT( aAgingFilter != NULL );

    idBool sIsFilterTrue = ID_FALSE;

    // Ư�� Tablespace���� OID�� ���ؼ��� Aging�ǽ�?
    if ( aAgingFilter->mTBSID != SC_NULL_SPACEID )
    {
        if(aTBSID == aAgingFilter->mTBSID)
        {
            sIsFilterTrue = ID_TRUE;
        }
    }
    else
    {
        // Ư�� Table���� OID�� ���ؼ��� Aging�ǽ�?
        if ( aAgingFilter->mTableOID != SM_NULL_OID )
        {
            if(aTableOID == aAgingFilter->mTableOID)
            {
                sIsFilterTrue = ID_TRUE;
            }
        }
        else
        {
            // Aging Filter�� mTableOID�� mSpaceID�� �ϳ���
            // �����Ǿ� �־�� �Ѵ�.

            // �� ���� �Ѵ� �������� ���� ����̹Ƿ�
            // ASSERT�� ���δ�.
            IDE_ASSERT(0);
        }
    }

    return sIsFilterTrue;
}



/*
    Instant Aging Filter�� �ʱ�ȭ�Ѵ� ( smaDef.h ���� )
 */
void smaLogicalAger::initInstantAgingFilter( smaInstantAgingFilter * sAgingFilter )
{
    IDE_DASSERT( sAgingFilter != NULL );

    sAgingFilter->mTBSID    = SC_NULL_SPACEID;
    sAgingFilter->mTableOID = SM_NULL_OID;
}



/*
    Ư�� Table���� OID�� Ư�� Tablespace���� OID�� ���� ��� Aging�� �ǽ��Ѵ�.

    [Ư�̻���]
       Aging�Ϸ��� OID�� SCN�� Active Transaction���� Minimum ViewSCN ����
       ũ���� Aging�� �ǽ��Ѵ�.

    [���ǻ���]
       �� Function�� ���� �ٸ� �ʿ��� Aging Filter�� ������ Table�̳�
       Tablespace�� ���ؼ� �ٸ� Transaction�� �������� �ʴ´ٴ� ���� ����
       �ؾ� �Ѵ�.

       Table�̳� Tablespace��  X-Lock�� ��� ������ �̸� ������ �� �ִ�.

    [IN] aAgingFilter - Aging�� ���� ( smaDef.h ���� )
 */
IDE_RC smaLogicalAger::agingInstantly( smaInstantAgingFilter * aAgingFilter )
{
    smaOidList*      sCurOIDList;
    UInt             sDidDelOldKey = 0;
    SInt             sState = 0;

    IDE_DASSERT( aAgingFilter != NULL );

    /* BUG-41026 */
    IDU_FIT_POINT( "1.BUG-41026@smaLogicalAger::agingInstantly::sleep_1" );

    IDE_TEST( lock() != IDE_SUCCESS );
    sState = 1;

    /* BUG-32655 [sm-mem-index] The MMDB Ager must not ignore the failure of
     * index aging.
     *
     * Ager�� AgingJob�� Head�� �޸��ϴ�. �׷��� Tail���� �Ѿư��� �մϴ�.
     * �׸����� ǥ�����ڸ�....
     *
     *      Next Next Next
     * mTail -> A -> B -> C -> mHead
     * ���� mHead���� mNext�� �Ѿư��� �ƹ��͵� �����ϴ�. */
    sCurOIDList = mTailLogicalAger;

    while(sCurOIDList != NULL)
    {
        IDE_TEST(deleteIndex(sCurOIDList,
                             aAgingFilter,
                             &sDidDelOldKey)
                 != IDE_SUCCESS);

        sCurOIDList = sCurOIDList->mNext;
    }

    /* BUG-32780 */
    IDE_TEST(smaDeleteThread::deleteInstantly( aAgingFilter )
             != IDE_SUCCESS);

    sState = 0;
    IDE_TEST( unlock() != IDE_SUCCESS );


    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    if(sState != 0)
    {
        (void)unlock();
    }

    return IDE_FAILURE;
}


/*
    Ư�� Tablespace�� ���� OID�� ���ؼ� ��� Aging�ǽ�

    [IN] aTBSID - Aging�� OID�� ���� Tablespace�� ID
 */
IDE_RC smaLogicalAger::doInstantAgingWithTBS( scSpaceID aTBSID )
{
    smaInstantAgingFilter sAgingFilter;

    initInstantAgingFilter( & sAgingFilter );

    sAgingFilter.mTBSID = aTBSID;

    IDE_TEST( agingInstantly( & sAgingFilter ) != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}



/*
    Ư�� Table�� ���� OID�� ���ؼ� ��� Aging�ǽ�

    [IN] aTBSID - Aging�� OID�� ���� Tablespace�� ID
 */
IDE_RC smaLogicalAger::doInstantAgingWithTable( smOID aTableOID )
{
    smaInstantAgingFilter sAgingFilter;

    initInstantAgingFilter( & sAgingFilter );

    sAgingFilter.mTableOID = aTableOID;

    IDE_TEST( agingInstantly( & sAgingFilter ) != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/*
    Ager Thread�� �ϳ� �߰��Ѵ�.
 */
IDE_RC smaLogicalAger::addOneAger()
{
    smaLogicalAger * sEmptyAgerObj = NULL;

    // Shutdown�߿��� �� �Լ��� ȣ��� �� ����.
    // mRunningThreadCount�� Shutdown�ÿ��� ���ҵǸ�
    // �ý��� ��߿��� mCreatedThreadCount�� �׻� ���� ���̾�� �Ѵ�.
    IDE_ASSERT( mCreatedThreadCount == mRunningThreadCount );
    
    // �ϳ� ���� �����̹Ƿ� smuProperty::getMaxAgerCount() ���ٴ� �۾ƾ���
    IDE_ASSERT( mCreatedThreadCount < smuProperty::getMaxLogicalAgerCount() );
    IDE_ASSERT( mCreatedThreadCount >= smuProperty::getMinLogicalAgerCount() );

    sEmptyAgerObj = & mLogicalAgerList[ mCreatedThreadCount ];

    new (sEmptyAgerObj) smaLogicalAger;
    
    IDE_TEST( sEmptyAgerObj->initialize( mCreatedThreadCount ) != IDE_SUCCESS );

    mCreatedThreadCount++;
    mRunningThreadCount++;

    return IDE_SUCCESS;
    
    IDE_EXCEPTION_END;
    
    return IDE_FAILURE;
}
    
/*
    Ager Thread�� �ϳ� �����Ѵ�.
 */
IDE_RC smaLogicalAger::removeOneAger()
{
    smaLogicalAger * sLastAger = NULL;

    // Shutdown�߿��� �� �Լ��� ȣ��� �� ����.
    // mRunningThreadCount�� Shutdown�ÿ��� ���ҵǸ�
    // �ý��� ��߿��� mCreatedThreadCount�� �׻� ���� ���̾�� �Ѵ�.
    IDE_ASSERT( mCreatedThreadCount == mRunningThreadCount );

    IDE_ASSERT( mCreatedThreadCount <= smuProperty::getMaxLogicalAgerCount() );
    // �ϳ� ���� �����̹Ƿ� smuProperty::getMinAgerCount() ���ٴ� Ŀ����
    IDE_ASSERT( mCreatedThreadCount > smuProperty::getMinLogicalAgerCount() );

    mCreatedThreadCount--;
    sLastAger = & mLogicalAgerList[ mCreatedThreadCount ];

    // run�Լ� �����Ű�� thread join���� �ǽ�
    IDE_TEST( sLastAger->shutdown() != IDE_SUCCESS );

    // run�Լ��� ���������鼭 mRunningThreadCount�� ���ҵȴ�
    // �׷��Ƿ� join�Ŀ� Created�� Running������ ���ƾ� �Ѵ�.
    IDE_ASSERT( mCreatedThreadCount == mRunningThreadCount );
    
    IDE_TEST( sLastAger->destroy() != IDE_SUCCESS );

    return IDE_SUCCESS;
    
    IDE_EXCEPTION_END;
    
    return IDE_FAILURE;
}


/*
    Ager Thread�� ������ Ư�� ������ �ǵ��� ����,�����Ѵ�.
    
    [IN] aAgerCount - Ager thread ����,�������� Ager����

    [����] �� �Լ��� ������ ���� �������
            LOGICAL AGER Thread �� ���� ��ɿ� ���� ����ȴ�.

            ALTER SYSTEM SET LOGICA_AGER_COUNT_ = 10;
 */
IDE_RC smaLogicalAger::changeAgerCount( UInt aAgerCount )
{

    UInt sStage = 0;

    IDE_ASSERT( mCreatedThreadCount == mRunningThreadCount );

    // �� �Լ� ȣ������ aAgerCount�� ���� üũ �Ϸ��� ����.
    // AgerCount�� Min,Max���� ���ʿ� �־�� �Ѵ�.
    IDE_ASSERT( aAgerCount <= smuProperty::getMaxLogicalAgerCount() );
    IDE_ASSERT( aAgerCount >= smuProperty::getMinLogicalAgerCount() );

    // mAgerCountChangeMutex �� ����
    // - ALTER SYSTEM SET LOGICA_AGER_COUNT_ ������ ���ÿ� ������� �ʵ���
    IDE_TEST( lockChangeMtx() != IDE_SUCCESS );
    sStage = 1;

    while ( mRunningThreadCount < aAgerCount )
    {
        IDE_TEST( addOneAger() != IDE_SUCCESS );
    }

    while ( mRunningThreadCount > aAgerCount )
    {
        IDE_TEST( removeOneAger() != IDE_SUCCESS );
    }

    sStage = 0;
    IDE_TEST( unlockChangeMtx() != IDE_SUCCESS );
    
    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    IDE_PUSH();

    if ( sStage != 0 )
    {
        IDE_ASSERT( unlockChangeMtx() == IDE_SUCCESS );
    }
    
    IDE_POP();
    
    return IDE_FAILURE;
}

/*
 * aAgingRequestCnt�� Logical Aging Request Count�� ���Ѵ�.
 *
 * aAgingRequestCnt - [IN] ������ Aging Request����
 *
 */
void smaLogicalAger::addAgingRequestCnt( ULong aAgingRequestCnt )
{
    IDE_ASSERT( lockMetaAddReqMtx() == IDE_SUCCESS );

    mAgingRequestOIDCnt += aAgingRequestCnt;

    IDE_ASSERT( unlockMetaAddReqMtx() == IDE_SUCCESS );
}


