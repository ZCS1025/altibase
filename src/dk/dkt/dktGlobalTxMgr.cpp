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
 * $id$
 **********************************************************************/

#include <dktGlobalTxMgr.h>
#include <iduCheckLicense.h>

#define EPOCHTIME_20170101   ( 1483228800 ) /* ( ( ( ( (2017) - (1970) ) * 365 ) * 24 ) *3600 ) + a */

UInt        dktGlobalTxMgr::mActiveGlobalCoordinatorCnt;
UInt        dktGlobalTxMgr::mUniqueGlobalTxSeq;
iduList     dktGlobalTxMgr::mGlobalCoordinatorList;
iduMutex    dktGlobalTxMgr::mDktMutex;
dktNotifier dktGlobalTxMgr::mNotifier;
UChar       dktGlobalTxMgr::mMacAddr[ACP_SYS_MAC_ADDR_LEN];
UInt        dktGlobalTxMgr::mInitTime = 0;
/************************************************************************
 * Description : Global transaction manager �� �ʱ�ȭ�Ѵ�.
 *
 ************************************************************************/
IDE_RC  dktGlobalTxMgr::initializeStatic()
{
    mUniqueGlobalTxSeq = 0;

    /* Global coordinator ��ü�� ������ 0 ���� �ʱ�ȭ */
    mActiveGlobalCoordinatorCnt = 0;
    /* Global coordinator ��ü���� ������ ���� list �ʱ�ȭ */
    IDU_LIST_INIT( &mGlobalCoordinatorList );
    /* Mutex �ʱ�ȭ */
    IDE_TEST_RAISE( mDktMutex.initialize( (SChar *)"DKT_GLOBAL_TX_MANAGER_MUTEX", 
                                          IDU_MUTEX_KIND_POSIX, 
                                          IDV_WAIT_INDEX_NULL )
                    != IDE_SUCCESS, ERR_MUTEX_INIT );

    IDE_TEST( mNotifier.initialize() != IDE_SUCCESS );

    if ( ( DKU_DBLINK_ENABLE == DK_ENABLE ) ||
         ( qci::isShardMetaEnable() == ID_TRUE ) )
    {
        IDE_TEST_RAISE( mNotifier.start() != IDE_SUCCESS, ERR_NOTIFIER_START );
    }
    else
    {
        /* Nothing to do */
    }

    idlOS::memcpy( mMacAddr, iduCheckLicense::mLicense.mMacAddr[0].mAddr, ACP_SYS_MAC_ADDR_LEN );

    mInitTime = (UInt) ( idlOS::time(NULL) - EPOCHTIME_20170101 );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_MUTEX_INIT );
    {
        IDE_SET( ideSetErrorCode( dkERR_FATAL_ThrMutexInit ) );
    }
    IDE_EXCEPTION( ERR_NOTIFIER_START );
    {
        ideLog::log( DK_TRC_LOG_FORCE, DK_TRC_T_NOTIFIER_THREAD );
        IDE_SET( ideSetErrorCode( dkERR_ABORT_DK_INTERNAL_ERROR,
                                  "[dktGlobalTxMgr::initializeStatic] ERROR NOTIFIER START" ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/************************************************************************
 * Description : Global transaction manager �� �����Ѵ�.
 *
 ************************************************************************/
IDE_RC  dktGlobalTxMgr::finalizeStatic()
{
    if ( ( DKU_DBLINK_ENABLE == DK_ENABLE ) ||
         ( qci::isShardMetaEnable() == ID_TRUE ) )
    {
        mNotifier.setExit( ID_TRUE );
        IDE_TEST( mNotifier.join() != IDE_SUCCESS );
    }
    else
    {
        /* Nothing to do */
    }

    mNotifier.finalize();

    IDE_TEST( mDktMutex.destroy() != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/************************************************************************
 * Description : Global coordinator ��ü�� �����Ѵ�.
 *
 *  aSession            - [IN] Global coordinator �� �����ϴ� ��������
 *  aGlobalCoordinator  - [OUT] ������ global coordinator �� ����Ű�� 
 *                              ������
 ************************************************************************/
IDE_RC  dktGlobalTxMgr::createGlobalCoordinator( dksDataSession        * aSession,
                                                 UInt                    aLocalTxId,
                                                 dktGlobalCoordinator ** aGlobalCoordinator )
{
    dktGlobalCoordinator * sGlobalCoordinator = NULL;
    UInt                   sState = 0;
    UInt                   sGlobalTxId = DK_INIT_GTX_ID;

    IDU_FIT_POINT_RAISE( "dktGlobalTxMgr::createGlobalCoordinator::malloc::GlobalCoordinator",
                          ERR_MEMORY_ALLOC_GLOBAL_COORDINATOR );
    IDE_TEST_RAISE( iduMemMgr::malloc( IDU_MEM_DK,
                                       ID_SIZEOF( dktGlobalCoordinator ),
                                       (void **)&sGlobalCoordinator,
                                       IDU_MEM_IMMEDIATE )
                    != IDE_SUCCESS, ERR_MEMORY_ALLOC_GLOBAL_COORDINATOR );
    sState = 1;

    IDE_TEST( sGlobalCoordinator->initialize( aSession ) != IDE_SUCCESS );
    sState = 2;

    sGlobalTxId = generateGlobalTxId();
   
    sGlobalCoordinator->setLocalTxId( aLocalTxId );
    sGlobalCoordinator->setGlobalTxId( sGlobalTxId );

    IDE_TEST( addGlobalCoordinatorToList( sGlobalCoordinator ) != IDE_SUCCESS );
    sState = 3;

    if ( sGlobalCoordinator->getAtomicTxLevel() == DKT_ADLP_TWO_PHASE_COMMIT )
    {
        IDE_TEST( sGlobalCoordinator->createDtxInfo( aLocalTxId,
                                                     sGlobalCoordinator->getGlobalTxId() )
                  != IDE_SUCCESS );
    }
    else
    {
        /* Nothing to do */
    }

    sGlobalCoordinator->setGlobalTxStatus( DKT_GTX_STATUS_BEGIN );

    *aGlobalCoordinator = sGlobalCoordinator;

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_MEMORY_ALLOC_GLOBAL_COORDINATOR );
    {
        
        IDE_SET( ideSetErrorCode( dkERR_ABORT_MEMORY_ALLOCATION ) );
    }
    IDE_EXCEPTION_END;

    switch ( sState )
    {
        case 3:
            removeGlobalCoordinatorFromList( sGlobalCoordinator );
        case 2:
            (void)sGlobalCoordinator->finalize();
            /* fall through */
        case 1:
            (void)iduMemMgr::free( sGlobalCoordinator );
            break;
        default:
            break;
    }

    return IDE_FAILURE;
}

/************************************************************************
 * Description : �Է¹��� global coordinator ��ü�� �����Ѵ�.
 *
 *  aGlobalCoordinator  - [IN] ������ global coordinator �� ����Ű�� 
 *                             ������
 *
 ************************************************************************/
void  dktGlobalTxMgr::destroyGlobalCoordinator( dktGlobalCoordinator * aGlobalCoordinator )
{
    dktNotifier * sNotifier = NULL;

    IDE_ASSERT( aGlobalCoordinator != NULL );

    /* PROJ-2569 notifier���� �̰��ؾ��ϹǷ�,
     *  �޸� ������ commit/rollback�� ���� �� ���� ���θ� ���� �װ����� �Ѵ�. */
    if ( aGlobalCoordinator->mDtxInfo != NULL  )
    {
       if (  aGlobalCoordinator->mDtxInfo->isEmpty() != ID_TRUE ) 
       {
            sNotifier = getNotifier();
            sNotifier->addDtxInfo( aGlobalCoordinator->mDtxInfo );
    
            IDE_ASSERT( aGlobalCoordinator->mCoordinatorDtxInfoMutex.lock( NULL /*idvSQL* */ ) == IDE_SUCCESS );
            aGlobalCoordinator->mDtxInfo = NULL;
            IDE_ASSERT( aGlobalCoordinator->mCoordinatorDtxInfoMutex.unlock() == IDE_SUCCESS );
        }
        else
        {
            aGlobalCoordinator->removeDtxInfo();
        }
    }
    else
    {
        /* Nothing to do */
    }

    removeGlobalCoordinatorFromList( aGlobalCoordinator );
    /* BUG-37487 : void */
    aGlobalCoordinator->finalize();

    (void)iduMemMgr::free( aGlobalCoordinator );

    aGlobalCoordinator = NULL;

    return;
}

/************************************************************************
 * Description : ���� DK ������ ����ǰ� �ִ� ��� �۷ι� Ʈ����ǵ��� 
 *               ������ ���´�.
 *              
 *  aInfo       - [IN] �۷ι� Ʈ����ǵ��� ������ ���� �迭������
 ************************************************************************/
IDE_RC  dktGlobalTxMgr::getAllGlobalTransactonInfo( dktGlobalTxInfo ** aInfo, UInt * aGTxCnt )
{
    UInt   i      =  0;
    UInt   sGTxCnt = 0;
    idBool sIsLock = ID_FALSE;    

    iduListNode           * sIterator           = NULL;
    dktGlobalCoordinator  * sGlobalCoordinator  = NULL;
    dktGlobalTxInfo       * sInfo               = NULL;

    IDE_ASSERT( mDktMutex.lock( NULL /*idvSQL* */ ) == IDE_SUCCESS );
    sIsLock = ID_TRUE;

    sGTxCnt = dktGlobalTxMgr::getActiveGlobalCoordinatorCnt();

    if ( sGTxCnt > 0 )
    {
        IDU_FIT_POINT_RAISE( "dkmGetGlobalTransactionInfo::malloc::Info",
                             ERR_MEMORY_ALLOC );
        IDE_TEST_RAISE( iduMemMgr::malloc( IDU_MEM_DK,
                                           ID_SIZEOF( dktGlobalTxInfo ) * sGTxCnt,
                                           (void **)&sInfo,
                                           IDU_MEM_IMMEDIATE )
                        != IDE_SUCCESS, ERR_MEMORY_ALLOC );

        IDU_LIST_ITERATE( &mGlobalCoordinatorList, sIterator )
        {
            sGlobalCoordinator = (dktGlobalCoordinator *)sIterator->mObj;

            IDE_TEST_CONT( i == sGTxCnt, FOUND_COMPLETE );

            if ( sGlobalCoordinator != NULL )
            {
                IDE_TEST( sGlobalCoordinator->getGlobalTransactionInfo( &sInfo[i] )
                          != IDE_SUCCESS );
                i++;
            }
            else
            {
                /* no more global coordinator */
            }
        }
    }
    else
    {
        /* nothing to do */
    }

    IDE_EXCEPTION_CONT( FOUND_COMPLETE );

    *aInfo = sInfo;
    *aGTxCnt = sGTxCnt;

    sIsLock = ID_FALSE;
    IDE_ASSERT( mDktMutex.unlock() == IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_MEMORY_ALLOC )
    {
        IDE_SET( ideSetErrorCode( dkERR_ABORT_MEMORY_ALLOCATION ) );
    }
    IDE_EXCEPTION_END;

    if ( sInfo != NULL )
    {
        (void)iduMemMgr::free( sInfo );
        sInfo = NULL;
    }
    else
    {
        /* do nothing */
    }

    if ( sIsLock == ID_TRUE )
    {
        sIsLock = ID_FALSE;
        IDE_ASSERT( mDktMutex.unlock() == IDE_SUCCESS );
    }
    else
    {
        /* nothing to do */
    }
    return IDE_FAILURE;
}

/************************************************************************
 * Description : ���� DK ������ ����ǰ� �ִ� ��� �۷ι� Ʈ����ǵ��� 
 *               ������ ���´�.
 *              
 *  aInfo       - [IN/OUT] �۷ι� Ʈ����ǵ��� ������ ���� �迭������
 *  aRTxCnt     - [IN] Caller �� ��û�� ������ ������ remote transaction
 *                     ������ŭ�� ������ �������� ���� �Է°�
 *
 ************************************************************************/
IDE_RC  dktGlobalTxMgr::getAllRemoteTransactonInfo( dktRemoteTxInfo ** aInfo,
                                                    UInt             * aRTxCnt )
{
    idBool sIsLock  = ID_FALSE;
    UInt   sInfoCnt	= 0;
    UInt   sRTxCnt  = 0;

    iduListNode          * sIterator          = NULL;
    dktGlobalCoordinator * sGlobalCoordinator = NULL;
    dktRemoteTxInfo      * sInfo              = NULL;

    IDE_ASSERT( mDktMutex.lock( NULL /*idvSQL* */ ) == IDE_SUCCESS );
    sIsLock = ID_TRUE;     

    IDE_TEST( dktGlobalTxMgr::getAllRemoteTransactionCountWithoutLock( &sRTxCnt )
              != IDE_SUCCESS );

    if ( sRTxCnt > 0 )
    {
        IDU_FIT_POINT_RAISE( "dkmGetRemoteTransactionInfo::malloc::Info",
                             ERR_MEMORY_ALLOC );
        IDE_TEST_RAISE( iduMemMgr::malloc( IDU_MEM_DK,
                                           ID_SIZEOF( dktRemoteTxInfo ) * sRTxCnt,
                                           (void **)&sInfo,
                                           IDU_MEM_IMMEDIATE )
                        != IDE_SUCCESS, ERR_MEMORY_ALLOC );

        IDU_LIST_ITERATE( &mGlobalCoordinatorList, sIterator )
        {
            sGlobalCoordinator = (dktGlobalCoordinator *)sIterator->mObj;

            IDE_TEST_CONT( sInfoCnt >= sRTxCnt, FOUND_COMPLETE );

            if ( sGlobalCoordinator != NULL )
            {
                IDE_TEST( sGlobalCoordinator->getRemoteTransactionInfo( sInfo,
                                                                        sRTxCnt,
                                                                        &sInfoCnt )
                          != IDE_SUCCESS );
            }
            else
            {
                /* no more global coordinator */
            }
        }
    }
    else
    {
        /* nothing to do */
    }

    IDE_EXCEPTION_CONT( FOUND_COMPLETE );

    *aInfo = sInfo;
    *aRTxCnt = sInfoCnt;

    sIsLock = ID_FALSE;
    IDE_ASSERT( mDktMutex.unlock() == IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_MEMORY_ALLOC )
    {
        IDE_SET( ideSetErrorCode( dkERR_ABORT_MEMORY_ALLOCATION ) );
    }

    IDE_EXCEPTION_END;

    if ( sInfo != NULL )
    {
        (void)iduMemMgr::free( sInfo );
        sInfo = NULL;
    }
    else
    {
        /* do nothing */
    }

    if ( sIsLock == ID_TRUE )
    {
        sIsLock = ID_FALSE;
        IDE_ASSERT( mDktMutex.unlock() == IDE_SUCCESS );
    }
    else
	{
		/* nothing to do */
	}

    return IDE_FAILURE;
}

/************************************************************************
 * Description : ���� DK ������ ����ǰ� �ִ� ��� �۷ι� Ʈ����ǵ��� 
 *               ������ ���´�.
 *              
 *  aInfo       - [IN/OUT] Remote statement ���� ������ ���� �迭������
 *  aStmtCnt    - [IN] Caller �� ��û�� ������ ������ remote statement
 *                     ������ŭ�� ������ �������� ���� �Է°�
 *
 ************************************************************************/
IDE_RC  dktGlobalTxMgr::getAllRemoteStmtInfo( dktRemoteStmtInfo * aInfo,
                                              UInt              * aStmtCnt )
{
    idBool                  sIsLock             = ID_FALSE;
    UInt                    sRemoteStmtCnt      = *aStmtCnt;
    UInt                    sInfoCnt            = 0;
    iduListNode            *sIterator           = NULL;
    dktGlobalCoordinator   *sGlobalCoordinator  = NULL;

    IDE_ASSERT( mDktMutex.lock( NULL /*idvSQL* */ ) == IDE_SUCCESS );
    sIsLock = ID_TRUE;

    IDU_LIST_ITERATE( &mGlobalCoordinatorList, sIterator )
    {
        sGlobalCoordinator = (dktGlobalCoordinator *)sIterator->mObj;

        IDE_TEST_CONT( sInfoCnt >= sRemoteStmtCnt, FOUND_COMPLETE );

        if ( sGlobalCoordinator != NULL )
        {
            IDE_TEST( sGlobalCoordinator->getRemoteStmtInfo( aInfo,
                                                             sRemoteStmtCnt,
                                                             &sInfoCnt )
                      != IDE_SUCCESS );
        }
        else
        {
            /* no more global coordinator */
        }
    }

    IDE_EXCEPTION_CONT( FOUND_COMPLETE );

    *aStmtCnt = sInfoCnt;

    sIsLock = ID_FALSE;
    IDE_ASSERT( mDktMutex.unlock() == IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    if ( sIsLock == ID_TRUE )
    {
        sIsLock = ID_FALSE;
        IDE_ASSERT( mDktMutex.unlock() == IDE_SUCCESS );
    }
    else
    {
        /* nothing to do */
    }

    return IDE_FAILURE;
}

/************************************************************************
 * Description : ���� DK ������ ����ǰ� �ִ� ��� remote transaction �� 
 *               ������ ���Ѵ�.
 *              
 *  aCount       - [OUT] ��� remote transaction �� ����
 *
 ************************************************************************/
IDE_RC  dktGlobalTxMgr::getAllRemoteTransactionCount( UInt  *aCount )
{
    UInt                    sRemoteTxCnt        = 0;
    iduListNode            *sIterator           = NULL;
    dktGlobalCoordinator   *sGlobalCoordinator  = NULL;

    IDE_ASSERT( mDktMutex.lock( NULL /*idvSQL* */ ) == IDE_SUCCESS );
    
    IDU_LIST_ITERATE( &mGlobalCoordinatorList, sIterator )
    {
        sGlobalCoordinator = (dktGlobalCoordinator *)sIterator->mObj;

        if ( sGlobalCoordinator != NULL )
        {
            sRemoteTxCnt += sGlobalCoordinator->getRemoteTxCount();
        }
        else
        {
            /* no more global coordinator */
        }
    }

    IDE_ASSERT( mDktMutex.unlock() == IDE_SUCCESS );

    *aCount = sRemoteTxCnt;

    return IDE_SUCCESS;
}

IDE_RC  dktGlobalTxMgr::getAllRemoteTransactionCountWithoutLock( UInt  *aCount )
{
    UInt                    sRemoteTxCnt        = 0;
    iduListNode            *sIterator           = NULL;
    dktGlobalCoordinator   *sGlobalCoordinator  = NULL;

    IDU_LIST_ITERATE( &mGlobalCoordinatorList, sIterator )
    {
        sGlobalCoordinator = (dktGlobalCoordinator *)sIterator->mObj;

        if ( sGlobalCoordinator != NULL )
        {
            sRemoteTxCnt += sGlobalCoordinator->getRemoteTxCount();
        }
        else
        {
            /* no more global coordinator */
        }
    }

    *aCount = sRemoteTxCnt;

    return IDE_SUCCESS;
}

/************************************************************************
 * Description : ���� DK ������ ����ǰ� �ִ� ��� remote statement �� 
 *               ������ ���Ѵ�.
 *              
 *  aCount       - [OUT] ��� remote statement �� ����
 *
 ************************************************************************/
IDE_RC  dktGlobalTxMgr::getAllRemoteStmtCount( UInt *aCount )
{
    UInt                    sRemoteStmtCnt      = 0;
    iduListNode            *sIterator           = NULL;
    dktGlobalCoordinator   *sGlobalCoordinator  = NULL;

    IDE_ASSERT( mDktMutex.lock( NULL /*idvSQL* */ ) == IDE_SUCCESS );

    IDU_LIST_ITERATE( &mGlobalCoordinatorList, sIterator )
    {
        sGlobalCoordinator = (dktGlobalCoordinator *)sIterator->mObj;

        if ( sGlobalCoordinator != NULL )
        {
            sRemoteStmtCnt += sGlobalCoordinator->getAllRemoteStmtCount();
        }
        else
        {
            /* no more global coordinator */
        }
    }

    IDE_ASSERT( mDktMutex.unlock() == IDE_SUCCESS );

    *aCount = sRemoteStmtCnt;

    return IDE_SUCCESS;
}

/************************************************************************
 * Description : �Է¹��� global coordinator �� ����������� �߰��Ѵ�. 
 *              
 *  aGlobalCoordinator  - [IN] List �� �߰��� global coordinator
 *
 ************************************************************************/
IDE_RC  dktGlobalTxMgr::addGlobalCoordinatorToList(
                            dktGlobalCoordinator    *aGlobalCoordinator )
{
    IDE_ASSERT( mDktMutex.lock( NULL /*idvSQL* */ ) == IDE_SUCCESS );

    IDU_LIST_INIT_OBJ( &(aGlobalCoordinator->mNode), aGlobalCoordinator );
    IDU_LIST_ADD_LAST( &mGlobalCoordinatorList, &(aGlobalCoordinator->mNode) );

    mActiveGlobalCoordinatorCnt++;

    IDE_ASSERT( mDktMutex.unlock() == IDE_SUCCESS );

    return IDE_SUCCESS;
}

void dktGlobalTxMgr::removeGlobalCoordinatorFromList( dktGlobalCoordinator * aGlobalCoordinator )
{
    IDE_ASSERT( mDktMutex.lock( NULL /*idvSQL* */ ) == IDE_SUCCESS );

    IDU_LIST_REMOVE( &aGlobalCoordinator->mNode );

    mActiveGlobalCoordinatorCnt--;

    IDE_ASSERT( mDktMutex.unlock() == IDE_SUCCESS );

    return;
}

/************************************************************************
 * Description : Global transaction id �� �Է¹޾� �ش� �۷ι� Ʈ�������
 *               �����ϴ� global coordinator �� list ���� ã�� ��ȯ�Ѵ�.
 *              
 *  aGTxId      - [IN] global transaction id
 *  aGlobalCrd  - [OUT] �ش� �۷ι� Ʈ������� �����ϴ� global coordinator
 *
 ************************************************************************/
IDE_RC  dktGlobalTxMgr::findGlobalCoordinator( UInt                  aGlobalTxId,
                                               dktGlobalCoordinator **aGlobalCrd )
{
    idBool                  sIsExist            = ID_FALSE;
    iduListNode            *sIterator           = NULL;
    dktGlobalCoordinator   *sGlobalCoordinator  = NULL;

    if ( aGlobalTxId != DK_INIT_GTX_ID )
    {
        IDE_ASSERT( mDktMutex.lock( NULL /*idvSQL* */ ) == IDE_SUCCESS );

        IDU_LIST_ITERATE( &mGlobalCoordinatorList, sIterator )
        {
            sGlobalCoordinator = (dktGlobalCoordinator *)sIterator->mObj;

            if ( sGlobalCoordinator->getGlobalTxId() == aGlobalTxId )
            {
                sIsExist = ID_TRUE;
                break;
            }
            else
            {
                /* iterate next node */
            }
        }

        IDE_ASSERT( mDktMutex.unlock() == IDE_SUCCESS );

        if ( sIsExist == ID_TRUE )
        {
            *aGlobalCrd = sGlobalCoordinator;
        }
        else
        {
            *aGlobalCrd = NULL;
        }
    }
    else
    {
        *aGlobalCrd = NULL;
    }

    return IDE_SUCCESS;
}

/************************************************************************
 * Description : Linker data session id �� �Է¹޾� �ش� session �� ���� 
 *               global coordinator �� list ���� ã�� ��ȯ�Ѵ�.
 *              
 *  aSessionId  - [IN] Linker data session id
 *  aGlobalCrd  - [OUT] �ش� �۷ι� Ʈ������� �����ϴ� global coordinator
 *
 ************************************************************************/
IDE_RC  dktGlobalTxMgr::findGlobalCoordinatorWithSessionId( 
                                        UInt                    aSessionId, 
                                        dktGlobalCoordinator  **aGlobalCrd )
{
    idBool                  sIsExist            = ID_FALSE;
    iduListNode            *sIterator           = NULL;
    dktGlobalCoordinator   *sGlobalCoordinator  = NULL;

    IDE_ASSERT( mDktMutex.lock( NULL /*idvSQL* */ ) == IDE_SUCCESS );
   
    IDU_LIST_ITERATE( &mGlobalCoordinatorList, sIterator )
    {
        sGlobalCoordinator = (dktGlobalCoordinator *)sIterator->mObj;

        if ( sGlobalCoordinator->getCurrentSessionId() == aSessionId )
        {
            sIsExist = ID_TRUE;
            break;
        }
        else
        {
            /* iterate next node */
        }
    }

    IDE_ASSERT( mDktMutex.unlock() == IDE_SUCCESS );

    if ( sIsExist == ID_TRUE )
    {
        *aGlobalCrd = sGlobalCoordinator;
    }
    else
    {
        *aGlobalCrd = NULL;
    }

    return IDE_SUCCESS;
}

void dktGlobalTxMgr::findGlobalCoordinatorByLocalTxId( UInt                    aLocalTxId,
                                                       dktGlobalCoordinator ** aGlobalCrd )
{
    idBool                  sIsExist            = ID_FALSE;
    iduListNode            *sIterator           = NULL;
    dktGlobalCoordinator   *sGlobalCoordinator  = NULL;

    IDE_ASSERT( mDktMutex.lock( NULL /*idvSQL* */ ) == IDE_SUCCESS );

    IDU_LIST_ITERATE( &mGlobalCoordinatorList, sIterator )
    {
        sGlobalCoordinator = (dktGlobalCoordinator *)sIterator->mObj;

        if ( sGlobalCoordinator->getLocalTxId() == aLocalTxId )
        {
            sIsExist = ID_TRUE;
            break;
        }
        else
        {
            /* iterate next node */
        }
    }

    IDE_ASSERT( mDktMutex.unlock() == IDE_SUCCESS );

    if ( sIsExist == ID_TRUE )
    {
        *aGlobalCrd = sGlobalCoordinator;
    }
    else
    {
        *aGlobalCrd = NULL;
    }

    return;
}

UInt dktGlobalTxMgr::generateGlobalTxId()
{
    UInt sGlobalTxId = 0;

    IDE_ASSERT( mDktMutex.lock( NULL /*idvSQL* */ ) == IDE_SUCCESS );

    mUniqueGlobalTxSeq += 1;
    if ( mUniqueGlobalTxSeq == DK_INIT_GTX_ID )
    {
        mUniqueGlobalTxSeq = 0;
    }
    else
    {
        /*do nothing*/
    }
    sGlobalTxId = mUniqueGlobalTxSeq;

    IDE_ASSERT( mDktMutex.unlock() == IDE_SUCCESS );

    return sGlobalTxId;
}

smLSN dktGlobalTxMgr::getDtxMinLSN( void )
{
    iduListNode          * sIterator           = NULL;
    dktGlobalCoordinator * sGlobalCoordinator  = NULL;
    dktDtxInfo           * sDtxInfo = NULL;
    smLSN                  sCompareLSN;
    smLSN                * sTempLSN = NULL;
    smLSN                  sCurrentLSN;

    SMI_LSN_MAX( sCompareLSN );
    SMI_LSN_INIT( sCurrentLSN );

    /* getMinLSN From Coordinator */
    IDE_ASSERT( mDktMutex.lock( NULL /*idvSQL* */ ) == IDE_SUCCESS );
    IDU_LIST_ITERATE( &mGlobalCoordinatorList, sIterator )
    {
        sGlobalCoordinator = (dktGlobalCoordinator *)sIterator->mObj;

        IDE_ASSERT( sGlobalCoordinator->mCoordinatorDtxInfoMutex.lock( NULL /*idvSQL* */ ) == IDE_SUCCESS );

        if ( sGlobalCoordinator->mDtxInfo != NULL )
        {
            sTempLSN = sGlobalCoordinator->mDtxInfo->getPrepareLSN();

            if ( ( SMI_IS_LSN_INIT( *sTempLSN ) == ID_FALSE ) &&
                 ( isGT( &sCompareLSN, sTempLSN ) == ID_TRUE ) )
            {
                idlOS::memcpy( &sCompareLSN,
                               sTempLSN,
                               ID_SIZEOF( smLSN ) );
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
        IDE_ASSERT( sGlobalCoordinator->mCoordinatorDtxInfoMutex.unlock() == IDE_SUCCESS );
    }
    IDE_ASSERT( mDktMutex.unlock() == IDE_SUCCESS );

    /* getMinLSN From Notifier */
    IDE_ASSERT( mNotifier.mNotifierDtxInfoMutex.lock( NULL /*idvSQL* */ ) == IDE_SUCCESS );
    IDU_LIST_ITERATE( &(mNotifier.mDtxInfo), sIterator )
    {
        sDtxInfo = (dktDtxInfo *)sIterator->mObj;
        sTempLSN = sDtxInfo->getPrepareLSN();

        if ( isGT( &sCompareLSN, sTempLSN )
             == ID_TRUE )
        {
            idlOS::memcpy( &sCompareLSN,
                           sTempLSN,
                           ID_SIZEOF( smLSN ) );
        }
        else
        {
            /* Nothing to do */
        }
    }
    IDE_ASSERT( mNotifier.mNotifierDtxInfoMutex.unlock() == IDE_SUCCESS );

    if ( DKU_DBLINK_RECOVERY_MAX_LOGFILE > 0 )
    {
        (void)smiGetLstLSN( &sCurrentLSN ); /* always return success  */

        if ( sCurrentLSN.mFileNo > sCompareLSN.mFileNo )
        {
            if ( ( sCurrentLSN.mFileNo - sCompareLSN.mFileNo ) <= DKU_DBLINK_RECOVERY_MAX_LOGFILE )
            {
                /* Nothing to do */
            }
            else
            {
                idlOS::memcpy( &sCompareLSN, &sCurrentLSN, ID_SIZEOF( smLSN ) );
            }
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

    return sCompareLSN;
}

IDE_RC dktGlobalTxMgr::getNotifierTransactionInfo( dktNotifierTransactionInfo ** aInfo,
                                                   UInt                        * aInfoCount )
{
    IDE_TEST( mNotifier.getNotifierTransactionInfo( aInfo,
                                                    aInfoCount )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

UInt dktGlobalTxMgr::getDtxGlobalTxId( UInt aLocalTxId )
{
    dktGlobalCoordinator * sGlobalCoordinator = NULL;
    UInt                          sGlobalTxId = DK_INIT_GTX_ID;

    if ( ( DKU_DBLINK_ENABLE == DK_ENABLE ) ||
         ( qci::isShardMetaEnable() == ID_TRUE ) )
    {
        findGlobalCoordinatorByLocalTxId( aLocalTxId, &sGlobalCoordinator );

        if ( sGlobalCoordinator != NULL )
        {
            if ( sGlobalCoordinator->getAtomicTxLevel() == DKT_ADLP_TWO_PHASE_COMMIT )
            {
                sGlobalTxId = sGlobalCoordinator->getGlobalTxId();
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
    }
    else
    {
        /* Nothing to do */
    }

    return sGlobalTxId;
}
