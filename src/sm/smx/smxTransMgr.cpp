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
 * $Id: smxTransMgr.cpp 82186 2018-02-05 05:17:56Z lswhh $
 **********************************************************************/

#include <idl.h>
#include <ida.h>
#include <ideErrorMgr.h>
#include <smErrorCode.h>
#include <smDef.h>
#include <smm.h>
#include <smr.h>
#include <smx.h>
#include <sdc.h>
#include <smxReq.h>

#define SMX_MIN_TRANS_PER_FREE_TRANS_LIST (2)

smxTransFreeList       * smxTransMgr::mArrTransFreeList;
smxTrans               * smxTransMgr::mArrTrans;
UInt                     smxTransMgr::mTransCnt;
UInt                     smxTransMgr::mTransFreeListCnt;
UInt                     smxTransMgr::mCurAllocTransFreeList;
iduMutex                 smxTransMgr::mMutex;
idBool                   smxTransMgr::mEnabledTransBegin;
UInt                     smxTransMgr::mSlotMask;
UInt                     smxTransMgr::mActiveTransCnt;
smxTrans                 smxTransMgr::mActiveTrans;
UInt                     smxTransMgr::mPreparedTransCnt;
smxTrans                 smxTransMgr::mPreparedTrans;
smxMinSCNBuild           smxTransMgr::mMinSCNBuilder;
smxGetSmmViewSCNFunc     smxTransMgr::mGetSmmViewSCN;
smxGetSmmCommitSCNFunc   smxTransMgr::mGetSmmCommitSCN;
UInt                     smxTransMgr::mTransTableFullCount;

IDE_RC  smxTransMgr::calibrateTransCount(UInt *aTransCount)
{
    mTransFreeListCnt = smuUtility::getPowerofTwo(ID_SCALABILITY_CPU);

    /* BUG-31862 resize transaction table without db migration
     * ������Ƽ�� TRANSACTION_TABLE_SIZE �� 16 ~ 16384 ������ 2^n ���� ����
     */
    mTransCnt = smuProperty::getTransTblSize();

    IDE_ASSERT( (mTransCnt & (mTransCnt -1) ) == 0 );

    // BUG-28565 Prepared Tx�� Undo ���� free trans list rebuild �� ������ ����
    // 1. free trans list �� ���� �ּ� 2�� �̻��� free trans�� ���� �� �ֵ���
    //    free trans list�� ������ ������
    // 2. �ּ� transaction table size�� 16���� ����(���� 0)
    if ( mTransCnt < ( mTransFreeListCnt * SMX_MIN_TRANS_PER_FREE_TRANS_LIST ) )
    {
        mTransFreeListCnt = mTransCnt / SMX_MIN_TRANS_PER_FREE_TRANS_LIST;
    }

    /* BUG-31862 resize transaction table without db migration
     * TRANSACTION_TABLE_SIZE �� 16 ~ 16384 ������ 2^n ���� �����ϹǷ�
     * �缳�� ���ʿ�, ���� �ڵ� ����
     */

    if ( aTransCount != NULL )
    {
        *aTransCount = mTransCnt;
    }

    return IDE_SUCCESS;
}

IDE_RC smxTransMgr::initialize()
{

    UInt i;
    UInt sFreeTransCnt;
    UInt sFstFreeTrans;
    UInt sLstFreeTrans;

    unsetSmmCallbacks();

    IDE_TEST( smxOIDList::initializeStatic() != IDE_SUCCESS );

    IDE_TEST( smxTouchPageList::initializeStatic() != IDE_SUCCESS );

    IDE_TEST( mMutex.initialize( (SChar*)"TRANS_MGR_MUTEX",
                                 IDU_MUTEX_KIND_NATIVE,
                                 IDV_WAIT_INDEX_NULL ) != IDE_SUCCESS );
    mEnabledTransBegin = ID_TRUE;

    mSlotMask = mTransCnt - 1;


    mCurAllocTransFreeList = 0;

    mActiveTransCnt         = 0;
    mPreparedTransCnt  = 0;

    mArrTrans = NULL;
    mTransTableFullCount = 0;

    IDE_TEST( smxTrans::initializeStatic() != IDE_SUCCESS );

    /* TC/FIT/Limit/sm/smx/smxTransMgr_initialize_malloc1.sql */
    IDU_FIT_POINT_RAISE( "smxTransMgr::initialize::malloc1",
                          insufficient_memory );

    IDE_TEST_RAISE( iduMemMgr::malloc( IDU_MEM_SM_TRANSACTION_TABLE,
                                       (ULong)ID_SIZEOF(smxTrans) * mTransCnt,
                                       (void**)&mArrTrans ) != IDE_SUCCESS,
                    insufficient_memory );

    mArrTransFreeList = NULL;

    /* TC/FIT/Limit/sm/smx/smxTransMgr_initialize_malloc2.sql */
    IDU_FIT_POINT_RAISE( "smxTransMgr::initialize::malloc2",
                          insufficient_memory );

    IDE_TEST_RAISE( iduMemMgr::malloc( IDU_MEM_SM_TRANSACTION_TABLE,
                                       (ULong)ID_SIZEOF(smxTransFreeList) * mTransFreeListCnt,
                                       (void**)&mArrTransFreeList ) != IDE_SUCCESS,
                    insufficient_memory );

    i = 0;

    new ( mArrTrans + i ) smxTrans;

    IDE_TEST( mArrTrans[i].initialize( mTransCnt, mSlotMask )
              != IDE_SUCCESS );

    i++;

    for( ; i < mTransCnt; i++ )
    {
        new ( mArrTrans + i ) smxTrans;

        IDE_TEST( mArrTrans[i].initialize( i, mSlotMask )
                  != IDE_SUCCESS );
    }

    sFreeTransCnt = mTransCnt / mTransFreeListCnt;

    for(i = 0; i < mTransFreeListCnt; i++)
    {
        new (mArrTransFreeList + i) smxTransFreeList;

        sFstFreeTrans = i * sFreeTransCnt;
        sLstFreeTrans = (i + 1) * sFreeTransCnt -1;

        IDE_TEST( mArrTransFreeList[i].initialize( mArrTrans,
                                                   i,
                                                   sFstFreeTrans,
                                                   sLstFreeTrans )
                  != IDE_SUCCESS );
    }

    IDE_TEST( smxSavepointMgr::initializeStatic() != IDE_SUCCESS );

    initATLAnPrepareLst();

    smxFT::initializeFixedTableArea();

    return IDE_SUCCESS;

    IDE_EXCEPTION( insufficient_memory );
    {
        IDE_SET(ideSetErrorCode(idERR_ABORT_InsufficientMemory));
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;

}

/*************************************************************************
 * BUG-38962
 * Description : MinViewSCN Builder�� �����Ѵ�.
 *************************************************************************/
IDE_RC smxTransMgr::initializeMinSCNBuilder()
{
    new ( &mMinSCNBuilder ) smxMinSCNBuild;

    IDE_TEST( mMinSCNBuilder.initialize()  != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/*************************************************************************
 * BUG-38962
 *      MinViewSCN �������� ������ ������ �и��Ѵ�.
 *      ���� : smxTransMgr::initializeMinSCNBuilder
 *      ���� : smxTransMgr::startupMinSCNBuilder
 *
 * Description : MinViewSCN Builder�� ������Ų��.
 *************************************************************************/
IDE_RC smxTransMgr::startupMinSCNBuilder()
{
    IDE_TEST( mMinSCNBuilder.startThread() != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/*************************************************************************
 *
 * Description : MinViewSCN Builder�� �����Ű�� �����Ѵ�.
 *
 *************************************************************************/
IDE_RC smxTransMgr::shutdownMinSCNBuilder()
{
    IDE_TEST( mMinSCNBuilder.shutdown() != IDE_SUCCESS );
    IDE_TEST( mMinSCNBuilder.destroy()  != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smxTransMgr::destroy()
{

    UInt      i;

    for(i = 0; i < mTransCnt; i++)
    {
        IDE_TEST( mArrTrans[i].destroy() != IDE_SUCCESS );
    }

    for(i = 0; i < mTransFreeListCnt; i++)
    {
        IDE_TEST( mArrTransFreeList[i].destroy() != IDE_SUCCESS );
    }

    IDE_TEST( iduMemMgr::free(mArrTrans) != IDE_SUCCESS );
    IDE_TEST( iduMemMgr::free(mArrTransFreeList) != IDE_SUCCESS );

    IDE_TEST( smxTrans::destroyStatic() != IDE_SUCCESS );

    IDE_TEST( smxSavepointMgr::destroyStatic() != IDE_SUCCESS );

    IDE_TEST( mMutex.destroy() != IDE_SUCCESS );

    IDE_TEST( smxOIDList::destroyStatic() != IDE_SUCCESS );

    IDE_TEST( smxTouchPageList::destroyStatic() != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;

}

IDE_RC smxTransMgr::alloc(smxTrans **aTrans,
                          idvSQL    *aStatistics,
                          idBool     aIgnoreRetry)
{

    UInt i;
    UInt sTransAllocWait;
    SInt sStartIdx;
    PDL_Time_Value sWaitTime;

    *aTrans = NULL;
    sTransAllocWait = smuProperty::getTransAllocWait();
    sStartIdx  = (idlOS::getParallelIndex() % mTransFreeListCnt);
    i          = sStartIdx;

    while(1)
    {
        if ( mEnabledTransBegin == ID_TRUE )
        {
            IDE_TEST( mArrTransFreeList[i].allocTrans(aTrans)
                      != IDE_SUCCESS );

            if ( *aTrans != NULL ) 
            {
                break;
            }
            i++;

            if ( i >= mTransFreeListCnt )
            {
                i = 0;
            }

            // transaction free list���� �ѹ�����
            // transaction�� alloc���޴� ���, property micro�ʸ�ŭ sleep�Ѵ�.
            if ( i == (UInt)sStartIdx )
            {
                /* BUG-33873 TRANSACTION_TABLE_SIZE �� �����߾����� trc �α׿� ����� */
                if ( (mTransTableFullCount % smuProperty::getCheckOverflowTransTblSize()) == 0 )
                {
                    ideLog::log( IDE_SERVER_0,
                                 " TRANSACTION_TABLE_SIZE is full !!\n"
                                 " Current TRANSACTION_TABLE_SIZE is %"ID_UINT32_FMT"\n"
                                 " Please check TRANSACTION_TABLE_SIZE\n",
                                 smuProperty::getTransTblSize() );
                    mTransTableFullCount++;
                }
                else
                {
                    /* ��� �����ص� ���� ��ȯ�ϹǷ� ������� */
                    mTransTableFullCount++;
                }

                /* BUG-27709 receiver�� ���� �ݿ� ��, Ʈ����� alloc���� �� �ش� receiver����. */
                IDE_TEST_RAISE( aIgnoreRetry == ID_TRUE, ERR_ALLOC_TIMEOUT );

                IDE_TEST( iduCheckSessionEvent( aStatistics )
                          != IDE_SUCCESS );

                sWaitTime.set( 0, sTransAllocWait );
                idlOS::sleep( sWaitTime );
            }
        }
        else
        {
            idlOS::thr_yield();
        }
    }

    IDE_ASSERT( ((*aTrans)->mTransID != SM_NULL_TID) &&
                ((*aTrans)->mTransID != 0));

    IDE_ASSERT( (*aTrans)->mIsFree == ID_FALSE );

    return IDE_SUCCESS;

    IDE_EXCEPTION(ERR_ALLOC_TIMEOUT);
    {
        IDE_SET(ideSetErrorCode(smERR_ABORT_TX_ALLOC));
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;

}

//fix BUG-13175
// nested transaction���� ���ܴ�� �ϱ� �ʰ�,
// transaction free list���� �Ҵ� ���� ���ϸ� return�� �Ѵ�.
IDE_RC smxTransMgr::allocNestedTrans(smxTrans **aTrans)
{

    SInt i;
    SInt sStartIdx;

    *aTrans = NULL;

    sStartIdx  = (idlOS::getParallelIndex() % mTransFreeListCnt);
    i = sStartIdx;
    while ( 1 )
    {
        if ( mEnabledTransBegin == ID_TRUE )
        {
            IDE_TEST( mArrTransFreeList[i].allocTrans(aTrans)
                      != IDE_SUCCESS );

            if ( *aTrans != NULL )
            {
                break;
            }

            i++;

            if ( i >= (SInt)mTransFreeListCnt )
            {
                i = 0;
            }
            // transaction free list�� �� ���� �� ����,
            // transaction�� �Ҵ� ���� ���Ͽ��� ������,
            // ��ٸ��� �ʰ� ���� ������.
            IDE_TEST_RAISE(i == sStartIdx , error_no_free_trans);


        }//if
        else
        {
            idlOS::thr_yield();
        }//else
    }//while

    IDE_ASSERT( ((*aTrans)->mTransID != SM_NULL_TID) &&
                ((*aTrans)->mTransID != 0));

    IDE_ASSERT( (*aTrans)->mIsFree == ID_FALSE );

    return IDE_SUCCESS;

    IDE_EXCEPTION(error_no_free_trans);
    {
        IDE_SET(ideSetErrorCode( smERR_ABORT_CantAllocTrans));
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;

}

void smxTransMgr::dump()
{

    UInt i;

    for(i = 0; i < mTransFreeListCnt; i++)
    {
        mArrTransFreeList[i].dump();
    }
}

/*************************************************************************
 *
 * Description : SysMemViewSCN �� ��ȯ�Ѵ�.
 *
 * memory GC�� LogicalAger���� GC ������  minumSCN�� ���Ѵ�. minumSCN�̶� ����
 * active transaction���� minum ViewSCN(mMinMemViewScn)�� ���� ���� ���̴�.
 * unpin�̳� alter table add column�ϰ� �ִ� tx�� �����Ѵ�.
 *
 * aMinSCN      - [OUT] Min( Transaction View SCN��, System SCN )
 * aTID         - [OUT] Minimum View SCN�� ������ �ִ� TID.
 *
 *************************************************************************/
void smxTransMgr::getMinMemViewSCNofAll( smSCN *aMinSCN,
                                         smTID *aTID )
{
    UInt       i;
    smSCN      sCurSCN;
    smxTrans   *sTrans;

    *aTID = SM_NULL_TID;

    /* BUG-17600: [HP MCM] Ager�� Transacation�� ���� �ִ� View�� ���Ƿ�
     * �����մϴ�. */

    /* BUG-22232: [SM] IndexŽ���� �����ϴ� Row�� �����Ǵ� ��� �� �߻���
     *
     * ���⼭ Active Transaction�� ������ Min SCN���� System View SCN��
     * �Ѱ��ش�. �������� ���Ѵ밪�� �Ѱܼ� Ager�� KeyFreeSCN�� �����ϰ�
     * Aging�ϴ� ������ �־���.
     * */
    smxTransMgr::mGetSmmViewSCN( aMinSCN );

    for(i = 0; i < mTransCnt; i++)
    {
        sTrans  = mArrTrans + i;

        while( sTrans->mLSLockFlag == SMX_TRANS_LOCKED )
        {
            idlOS::thr_yield();
        }

        sTrans->getMinMemViewSCN4LOB(&sCurSCN);

        // unpin�̳� alter table add column�� �ϰ� �ִ� Tx�� ����.
        // Memory Resident Table�� ���ϼ��� �ǹ̸� ����.
        if ( sTrans->mDoSkipCheckSCN == ID_TRUE )
        {
            continue;
        }

        if ( sTrans->mStatus != SMX_TX_END )
        {
            if ( SM_SCN_IS_LE( &sCurSCN, aMinSCN ) )
            {
                *aTID = sTrans->mTransID;
                SM_SET_SCN( aMinSCN, &sCurSCN );
            }//if SM_SCN_IS_LT
        }
    }
}

/*************************************************************************
 *
 * Description: SysDskViewSCN, SysFstDskViewSCN,
 *              SysOldestFstViewSCN �� ��ȯ�Ѵ�.
 *     BUG-24885�� ���� SysFstDskViewSCN�� �߰���.
 *     BUG-26881�� ���� SysOldestFstViewSCN�� �߰���.
 *
 * SysDskViewSCN �̶� ���� Active Ʈ����ǵ��� DskStmt �߿��� ���� ���� ViewSCN
 * �� �ǹ��Ѵ�. LobCursor�� ViewSCN�� �Բ� ����Ǹ�,
 * Create Disk Index Ʈ������� �����Ѵ�.
 *
 * SysFstDskViewSCN�� ���� Active Ʈ����� �� �� ���� ����
 * FstDskViewSCN�� �ǹ��Ѵ�.
 *
 * SysOldestFstViewSCN�� ���� Active Ʈ����� �鿡 ������ ���� ������
 * OldestFstViewSCN�� �ǹ��Ѵ�
 *
 * BUG-24885 : wrong delayed stamping
 *             active CTS�� ���� delayed stamping ��/�θ� �Ǵ��ϱ� ����
 *             ��� Active Ʈ����ǵ��� fstDskViewSCN �߿��� �ּҰ��� ��ȯ�Ѵ�.
 *
 * BUG-26881 :
 *             active CTS�� ���� delayed stamping ��/�θ� �Ǵ��ϱ� ����
 *             ��� Active Ʈ����ǵ��� fstDskViewSCN �߿��� �ּҰ��� ��ȯ�Ѵ�.
 *
 * aMinSCN       - [OUT] Min( Transaction View SCN��, System SCN )
 * aMinDskFstSCN - [OUT] Min( Transaction Fst Dsk SCN��, System SCN )
 *
 *************************************************************************/
void smxTransMgr::getDskSCNsofAll( smSCN   * aMinViewSCN,
                                   smSCN   * aMinDskFstViewSCN,
                                   smSCN   * aMinOldestFstViewSCN )
{
    UInt         i;
    smSCN        sCurViewSCN;
    smSCN        sCurDskFstViewSCN;
    smSCN        sCurOldestFstViewSCN;
    smxTrans   * sTrans;

    /* BUG-17600: [HP MCM] Ager�� Transacation�� ���� �ִ� View�� ���Ƿ�
     * �����մϴ�. */

    /* BUG-22232: [SM] IndexŽ���� �����ϴ� Row�� �����Ǵ� ��� �� �߻���
     *
     * ���⼭ Active Transaction�� ������ Min SCN���� System View SCN��
     * �Ѱ��ش�. �������� ���Ѵ밪�� �Ѱܼ� Ager�� KeyFreeSCN�� �����ϰ�
     * Aging�ϴ� ������ �־���.
     * */
    smxTransMgr::mGetSmmViewSCN( aMinViewSCN );
    SM_GET_SCN( aMinDskFstViewSCN, aMinViewSCN );
    SM_GET_SCN( aMinOldestFstViewSCN, aMinViewSCN );

    for(i = 0; i < mTransCnt; i++)
    {
        sTrans  = mArrTrans + i;

        while( sTrans->mLSLockFlag == SMX_TRANS_LOCKED )
        {
            idlOS::thr_yield();
        }

        sTrans->getMinDskViewSCN4LOB(&sCurViewSCN);
        sCurDskFstViewSCN    = smxTrans::getFstDskViewSCN( sTrans );
        sCurOldestFstViewSCN = smxTrans::getOldestFstViewSCN( sTrans );

        if ( sTrans->mDoSkipCheckSCN == ID_TRUE )
        {
            continue;
        }

        if ( sTrans->mStatus != SMX_TX_END )
        {
            if ( SM_SCN_IS_LT( &sCurViewSCN, aMinViewSCN ) )
            {
                SM_SET_SCN( aMinViewSCN, &sCurViewSCN );
            }

            // BUG-24885 wrong delayed stamping
            // set the min disk FstSCN
            if ( SM_SCN_IS_LT( &sCurDskFstViewSCN, aMinDskFstViewSCN ) )
            {
                SM_SET_SCN( aMinDskFstViewSCN, &sCurDskFstViewSCN );
            }

            // BUG-26881 �߸��� CTS stamping���� acces�� �� ���� row�� ������
            // set the oldestViewSCN
            if ( SM_SCN_IS_LT( &sCurOldestFstViewSCN, aMinOldestFstViewSCN ) )
            {
                SM_SET_SCN( aMinOldestFstViewSCN, &sCurOldestFstViewSCN );
            }
        }
    }
}

void smxTransMgr::dumpActiveTrans()
{
    UInt       i;
    smSCN      sCurSCN;
    smSCN      sCommitSCN;
    smTID      sTransID1;
    smTID      sTransID2;
    smxTrans   *sTrans;


    for(i = 0; i < mTransCnt; i++)
    {
        sTrans  = mArrTrans + i;
        sTransID1 = sTrans->mTransID;
        sTransID2 = sTransID1;

        while ( ( sTrans->mLSLockFlag == SMX_TRANS_LOCKED ) &&
                ( sTransID1 == sTransID2 ) )
        {
            idlOS::thr_yield();

            sTransID2 = sTrans->mTransID;
        }

        if ( sTransID1 != sTransID2 )
        {
            continue;
        }

        IDL_MEM_BARRIER;

        SM_GET_SCN(&sCurSCN,    &sTrans->mMinMemViewSCN);
        SM_GET_SCN(&sCommitSCN, &sTrans->mCommitSCN);

        if ( (sTrans->mStatus != SMX_TX_END) && (sTransID1 == sTrans->mTransID))
        {
            idlOS::printf("%"ID_UINT32_FMT" slot,%"ID_UINT32_FMT")M: %"ID_UINT64_FMT",D:%"ID_UINT64_FMT"\n",
                          i,
                          sTrans->mTransID,
                          sTrans->mMinMemViewSCN,
                          sTrans->mMinDskViewSCN);
        }
    }
}

/*************************************************************************
 Description: Active Transaction���� ���� ���� Begin LSN�� ���Ѵ�.
 Argument:
              aLSN: Min LSN�� ���� LSN
************************************************************************/
IDE_RC smxTransMgr::getMinLSNOfAllActiveTrans( smLSN *aLSN )
{
    UInt         sTransCnt;
    smxTrans    *sCurTrans;
    UInt         i;
    smTID        sTransID;
    SInt         sState = 0;

    sTransCnt = smxTransMgr::mTransCnt;
    sCurTrans = smxTransMgr::mArrTrans;

    SM_LSN_MAX( *aLSN );

    /* ������ Transaction�� Begin LSN�߿���
       �������� Begin LSN�� ���Ѵ�.*/
    for ( i = 0 ; i < sTransCnt ; i++ )
    {
        /* mFstUndoNxtLSN�� ID_UINT_MAX�� �ƴϸ�
           ���� Ʈ������� ��� �ѹ� Update�ߴ�.*/
        if ( ( !SM_IS_LSN_MAX( sCurTrans->mFstUndoNxtLSN ) ) )
        {
            sTransID = sCurTrans->mTransID;

            IDE_TEST( sCurTrans->lock() != IDE_SUCCESS );
            sState = 1;

            if ( ( !SM_IS_LSN_MAX( sCurTrans->mFstUndoNxtLSN ) )  && 
                 ( sCurTrans->mTransID == sTransID ) )
            {
                if ( smrCompareLSN::isLT( &(sCurTrans->mFstUndoNxtLSN ),
                                          aLSN ) == ID_TRUE )
                {
                    SM_GET_LSN( *aLSN, sCurTrans->mFstUndoNxtLSN );
                }
            }

            sState = 0;
            IDE_TEST( sCurTrans->unlock() != IDE_SUCCESS );
        }

        sCurTrans  += 1;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    if ( sState != 0 )
    {
        (void)sCurTrans->unlock();
    }

    return IDE_FAILURE;
}

IDE_RC smxTransMgr::existPreparedTrans( idBool *aExistFlag )
{
    UInt         sCurSlotID;
    smxTrans    *sCurTrans;

    *aExistFlag = ID_FALSE;

    sCurSlotID = 0;
    for (; sCurSlotID < mTransCnt; sCurSlotID++)
    {
        sCurTrans = getTransBySID(sCurSlotID);
        if ( sCurTrans->isPrepared() == ID_TRUE )
        {
            *aExistFlag = ID_TRUE;
            break;
        }
    }

    return IDE_SUCCESS;

}

IDE_RC smxTransMgr::recover(SInt           *aSlotID,
                            /* BUG-18981 */
                            ID_XID       *aXID,
                            timeval        *aPreparedTime,
                            smxCommitState *aState)
{

    UInt         sCurSlotID;
    smxTrans    *sCurTrans;

    sCurSlotID = (*aSlotID) + 1;

    for (; sCurSlotID < mTransCnt; sCurSlotID++)
    {
        sCurTrans = getTransBySID(sCurSlotID);
        if ( sCurTrans->isPrepared() == ID_TRUE )
        {
            *aXID = sCurTrans->mXaTransID;
            *aSlotID = sCurSlotID;
            idlOS::memcpy( aPreparedTime,
                           &(sCurTrans->mPreparedTime),
                           ID_SIZEOF(timeval) );
            *aState = sCurTrans->mCommitState;
            break;
        }
    }

    if (sCurSlotID == mTransCnt)
    {
        *aSlotID = -1;
    }

    return IDE_SUCCESS;

}

/* BUG-18981 */
IDE_RC smxTransMgr::getXID(smTID  aTID, ID_XID *aXID)
{

    smxTrans    *sCurTrans;

    sCurTrans = getTransByTID(aTID);
    IDE_ASSERT( sCurTrans->mTransID == aTID );
    IDE_TEST( sCurTrans->getXID(aXID) != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;
    return IDE_FAILURE;

}

/* ----------------------------------------------
   recovery ���Ŀ��� ȣ��ɼ� ����
   recovery ���� prepare Ʈ������� �����ϴ� ���
   �̵��� free list���� �����ϱ� ������
   ---------------------------------------------- */
IDE_RC smxTransMgr::rebuildTransFreeList()
{

    UInt i;
    UInt sFreeTransCnt;
    UInt sFstFreeTrans;
    UInt sLstFreeTrans;

    sFreeTransCnt = mTransCnt / mTransFreeListCnt;

    for(i = 0; i < mTransFreeListCnt; i++)
    {
        sFstFreeTrans = i * sFreeTransCnt;
        sLstFreeTrans = (i + 1) * sFreeTransCnt -1;

        IDE_TEST( mArrTransFreeList[i].rebuild(i, sFstFreeTrans, sLstFreeTrans)
                  != IDE_SUCCESS );
    }


    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;

}

/*****************************************************************
 * Description:
 *  [BUG-20861] ���� hash resize�� �ϱ����ؼ� �ٸ� Ʈ����ǵ��� ��� ��������
 *  ���ϰ� �ؾ� �մϴ�.
 *  [BUG-42927] TABLE_LOCK_ENABLE ���� ����Ǵ� ����
 *  ���ο� active transaction�� BLOCK �ϰ� active transaction�� ��� ����Ǿ������� Ȯ���մϴ�.
 *
 *  �� �Լ��� ������ ���Ŀ��� ���ڷ� �Ѱ��� aTrans�ܿ� � Ʈ����ǵ�
 *  �������� �ƴϴ�. ��, �ڽſܿ� ��� Ʈ������� ����Ǿ������� ����.
 *
 *  aTrans      - [IN]  smxTransMgr::block�� �����ϴ� Ʈ����� �ڽ�
 *  aTryMicroSec- [IN]  �� �ð����� Ʈ������� ��� ����Ǳ⸦ ��ٸ���.
 *  aSuccess    - [OUT] aTryMicroSec���� Ʈ������� ��� ������� ������
 *                      ID_FALSE�� �����Ѵ�.
 ****************************************************************/
void smxTransMgr::block( void    *aTrans,
                         UInt     aTryMicroSec,
                         idBool  *aSuccess )
{
    PDL_Time_Value  sSleepTime;
    UInt            sWaitMax;
    UInt            sSleepCnt=0;

    IDE_DASSERT( aSuccess != NULL);

    disableTransBegin();

    /* sWaitMax�� ����Ƚ���� ����Ű��, sSleepTime�� �� ������ sleep�ϴ�
     * �ð��̴�. sSleepTime�� ���Ƿ� 50000���� ���ߴ�.
     * �� ���� Ŀ���� block�Լ��� ���� �ӵ��� ��������,
     * �� ���� �۾�����, block�Լ��� cpu�� ���� ����Ѵ�.*/
    sWaitMax = aTryMicroSec / 50000;
    sSleepTime.set(0, 50000);

    while( existActiveTrans((smxTrans*)aTrans) == ID_TRUE )
    {
        if ( sSleepCnt > sWaitMax ) 
        {
            break;
        }
        else
        {
            sSleepCnt++;
            idlOS::sleep(sSleepTime);
        }
    }

    if ( existActiveTrans( (smxTrans*)aTrans) == ID_TRUE )
    {
        *aSuccess = ID_FALSE;
    }
    else
    {
        *aSuccess = ID_TRUE;
    }

    return;
}

/*****************************************************************
 * Description:
 *  [BUG-20861] ���� hash resize�� �ϱ����ؼ� �ٸ� Ʈ����ǵ��� ��� ��������
 *  ���ϰ� �ؾ� �մϴ�.
 *
 *  Ʈ����� block�� ����, �� �Լ� ������, Ʈ������� �ٽ� ����� �� �ִ�.
 ****************************************************************/
void smxTransMgr::unblock(void)
{
    enableTransBegin();
}

void smxTransMgr::enableTransBegin()
{
    mEnabledTransBegin = ID_TRUE;
}

void smxTransMgr::disableTransBegin()
{
    mEnabledTransBegin = ID_FALSE;
}

idBool  smxTransMgr::existActiveTrans( smxTrans  * aTrans )
{

    UInt       i;
    smxTrans   *sTrans;
    for(i = 0; i < mTransCnt; i++)
    {
        sTrans  = mArrTrans + i;
        //�˻��ϴ� ��ü�� Ʈ������� �����Ѵ�.
        if ( aTrans == sTrans )
        {
            continue;
        }

        if ( ( sTrans->mStatus != SMX_TX_END ) && 
             ( sTrans->mLogTypeFlag == SMR_LOG_TYPE_NORMAL ) )
        {

            return ID_TRUE;
        }//if
    }//for
    return ID_FALSE;

}




// add active transaction to list if necessary.
void smxTransMgr::addActiveTrans( void  * aTrans,
                                  smTID   aTID,
                                  UInt    aFlag,
                                  idBool  aIsBeginLog,
                                  smLSN * aCurLSN )
{
    smxTrans* sCurTrans     = (smxTrans*)aTrans;
    SChar     sOutputMsg[256];

    if ( sCurTrans->mStatus != SMX_TX_BEGIN )
    {
        if ( aIsBeginLog == ID_TRUE )
        {
            IDE_ASSERT(sCurTrans->mStatus == SMX_TX_END);
            sCurTrans->init(aTID);

            sCurTrans->begin( NULL, aFlag, SMX_NOT_REPL_TX_ID );

            sCurTrans->setLstUndoNxtLSN( *aCurLSN );
            sCurTrans->mIsFirstLog = ID_FALSE;
            /* Add trans to Active Transaction List for Undo-Phase */
            addAT(sCurTrans);
        }
        else
        {
            /* RecoverLSN ������ ������ Ʈ����ǵ��� �α�
             * ������ commit �ȴٴ� ����.
             * do nothing */
        }
    }
    else
    {
        /* BUG-31862 resize transaction table without db migration
         * TRANSACTION TABLE SIZE�� Ȯ���� ��,
         * Ȯ�� �� ��� ���ϰ� ������Ƽ�� Recovery �ϴ� ���
         * slot number �� �ߺ��� �� �ֽ��ϴ�.
         * TID�� ���� */
        if ( sCurTrans->mTransID == aTID )
        {
            sCurTrans->setLstUndoNxtLSN( *aCurLSN );
        }
        else
        {
            /* slot�� ��ġ�� ���� ��Ȳ */
            idlOS::snprintf( sOutputMsg, ID_SIZEOF(sOutputMsg), "ERROR: \n"
                             "Transaction Slot is conflict beetween %"ID_UINT32_FMT""
                             " and %"ID_UINT32_FMT" \n"
                             "FileNo = %"ID_UINT32_FMT" \n"
                             "Offset = %"ID_UINT32_FMT" \n",
                             (UInt)sCurTrans->mTransID,
                             (UInt)aTID,
                             aCurLSN->mFileNo,
                             aCurLSN->mOffset );

            ideLog::log(IDE_SERVER_0,"%s\n",sOutputMsg);

            IDE_ASSERT( sCurTrans->mTransID == aTID );
        }
    }
}

// set XA information add Active Transaction
IDE_RC smxTransMgr::setXAInfoAnAddPrepareLst( void     * aTrans,
                                              timeval    aTimeVal,
                                              ID_XID     aXID, /* BUG-18981 */
                                              smSCN    * aFstDskViewSCN )
{
    smxTrans        * sCurTrans;

    sCurTrans  = (smxTrans*) aTrans;

    sCurTrans->mCommitState  = SMX_XA_PREPARED;
    sCurTrans->mPreparedTime = aTimeVal;
    sCurTrans->mXaTransID    = aXID;

    // BUG-27024 XA���� Prepare �� Commit���� ���� Disk Row��
    //           Server Restart �� ����Ῡ�� �մϴ�.
    // Restart ������ XA Trans�� FstDskViewSCN��
    // Restart ���� �籸���� XA Prepare Trans�� �ݿ���
    SM_SET_SCN( &sCurTrans->mFstDskViewSCN, aFstDskViewSCN );

    if ( smuProperty::getLogBufferType() == SMU_LOG_BUFFER_TYPE_MEMORY )
    {
        // Log Buffer Type�� memory�� ���, update transaction �� ����
        // XA Transaction�� Restart Redo�� ���� �Ŀ���
        // �״�� ��Ƽ� �����ϴٰ� ���� ��Ȳ���� Commit/Rollback�� �Ѵ�.
        // �׷��Ƿ�, Restart Redo�߿� �߻��� XA Transaction�� ���ؼ�
        // Update Tx Count�� �ϳ� �������� �־�� �Ѵ�.
        smxTrans::setRSGroupID( sCurTrans, 0 );

        IDE_TEST( smrLogMgr::incUpdateTxCount() != IDE_SUCCESS );
    }

    /* Add trans to Prepared List */
    removeAT(sCurTrans);
    addPreparedList(sCurTrans);

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/*****************************************************************
 * Description: BUG-27024 [SD] XA���� Prepare �� Commit���� ���� Disk
 *              Row�� Server Restart �� ����Ῡ�� �մϴ�.
 *
 * Restart Recovery ����, Prepare Trans���� mFstDskViewSCN ��
 * ���� ���� ������ Prepare Trans���� OldestFstViewSCN�� ����մϴ�.
 *
 * �̴� XA Trans�� Restart ������ Prepare �ϰ� Commit���� ���� Row��
 * DskFstViewSCN�� System Agable SCN���� �۾Ƽ� Commit�Ǿ��ٰ� �����ϴ�
 * ������ ���� ����
 *****************************************************************/
void smxTransMgr::rebuildPrepareTransOldestSCN()
{
    smxTrans * sCurTrans;
    smSCN      sMinFstDskViewSCN;

    SM_MAX_SCN( &sMinFstDskViewSCN );

    for( sCurTrans = mPreparedTrans.mNxtPT ;
         &mPreparedTrans != sCurTrans ;
         sCurTrans = sCurTrans->mNxtPT )
    {
        IDE_ASSERT( sCurTrans != NULL );

        if ( SM_SCN_IS_LT( &sCurTrans->mFstDskViewSCN, &sMinFstDskViewSCN ) )
        {
            SM_SET_SCN( &sMinFstDskViewSCN, &sCurTrans->mFstDskViewSCN );
        }
    }

    if ( !SM_SCN_IS_MAX( sMinFstDskViewSCN ) )
    {
        for( sCurTrans = mPreparedTrans.mNxtPT ;
             &mPreparedTrans != sCurTrans ;
             sCurTrans = sCurTrans->mNxtPT )
        {
            IDE_ASSERT( sCurTrans != NULL );

            SM_SET_SCN( &sCurTrans->mOldestFstViewSCN, &sMinFstDskViewSCN );
        }
    }
}

/***********************************************************************
 *
 * Description :
 *
 * BUG-27122 Restart Recovery �� Undo Trans�� �����ϴ� �ε����� ����
 * Integrity üũ��� �߰� (__SM_CHECK_DISK_INDEX_INTEGRITY=2)
 *
 **********************************************************************/
IDE_RC smxTransMgr::verifyIndex4ActiveTrans(idvSQL * aStatistics)
{
    smxTrans  * sCurTrans;
    SChar       sStrBuffer[128];

    if ( isEmptyActiveTrans() == ID_TRUE )
    {
        idlOS::snprintf(
            sStrBuffer,
            128,
            "      Active Transaction Count : 0\n" );
        IDE_CALLBACK_SEND_SYM( sStrBuffer );

        IDE_CONT( skip_verify );
    }

    sCurTrans = mActiveTrans.mNxtAT;

    while ( sCurTrans != &mActiveTrans )
    {
        IDE_ASSERT( sCurTrans               != NULL );
        // XA Ʈ������� ���µ� SMX_TX_BEGIN�� ����
        IDE_ASSERT( sCurTrans->mStatus == SMX_TX_BEGIN );
        IDE_ASSERT( sCurTrans->mOIDToVerify != NULL );

        if ( sCurTrans->mOIDToVerify->isEmpty() == ID_TRUE )
        {
            sCurTrans = sCurTrans->mNxtAT;
            continue;
        }

        IDE_TEST( sCurTrans->mOIDToVerify->processOIDListToVerify(
                             aStatistics ) != IDE_SUCCESS );

        sCurTrans = sCurTrans->mNxtAT;
    }

    IDE_EXCEPTION_CONT( skip_verify );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}


/*
   ��� :
   redoAll �������� SMR_LT_COMMIT, SMR_LT_ABORT �α׸�
   ������Ͽ� Ʈ������� �Ϸ�ó���Ѵ�.

   PRJ-1548, BUG-14978
   RESTART RECOVERY �������� Commit Pending ������ �����ϵ���
   ������.

   [IN] aTrans    : Ʈ����� ��ü
   [IN] aIsCommit : Ʈ����� Ŀ�� ����
*/
IDE_RC smxTransMgr::makeTransEnd( void * aTrans,
                                  idBool aIsCommit )
{

    smxTrans       * sCurTrans = (smxTrans*) aTrans;

    if ( sCurTrans->mStatus == SMX_TX_BEGIN )
    {
        // PROJ-1704 MVCC Renewal
        if ( sCurTrans->mTXSegEntry != NULL )
        {
            IDE_ASSERT( smxTrans::getTSSlotSID( sCurTrans )
                        != SD_NULL_SID );

            sdcTXSegMgr::freeEntry( sCurTrans->mTXSegEntry,
                                    ID_TRUE /* aMoveToFirst */ );
            sCurTrans->mTXSegEntry = NULL;
        }

        IDE_TEST( sCurTrans->freeOIDList() != IDE_SUCCESS );

        // PRJ-1548 User Memory Tablespace
        // Ʈ����ǿ� ��ϵ� ���̺����̽��� ����
        // Commit Pending ������ �����Ѵ�.
        IDE_TEST( sCurTrans->executePendingList( aIsCommit ) != IDE_SUCCESS );

        if ( sCurTrans->isPrepared() == ID_TRUE )
        {
            IDE_TEST( sCurTrans->end() != IDE_SUCCESS );
            removePreparedList( sCurTrans );
        }
        else
        {
            IDE_TEST( sCurTrans->end() != IDE_SUCCESS );
            removeAT( sCurTrans);
        }
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;

}

IDE_RC smxTransMgr::insertUndoLSNs( smrUTransQueue*  aTransQueue )
{

    smxTrans* sTrans;
    smLSN     sStopLSN;

    sTrans = mActiveTrans.mNxtAT;
    SM_LSN_MAX( sStopLSN );

    /* -----------------------------------------------
        [1] �� active transaction��
        ���������� ������ �α��� LSN���� queue ����
      ----------------------------------------------- */
    while ( sTrans != &mActiveTrans )
    {
#ifdef DEBUG
        IDE_ASSERT( smrCompareLSN::isEQ( &(sTrans->mLstUndoNxtLSN),
                                         &sStopLSN ) == ID_FALSE );
#endif
        aTransQueue->insertActiveTrans(sTrans);
        sTrans = sTrans->mNxtAT;
     }

    return IDE_SUCCESS;

}

IDE_RC smxTransMgr::abortAllActiveTrans()
{

    smxTrans* sTrans;
    sTrans = mActiveTrans.mNxtAT;
    while ( sTrans != &mActiveTrans )
    {
        IDE_TEST( sTrans->freeOIDList() != IDE_SUCCESS );
        IDE_TEST( sTrans->abort() != IDE_SUCCESS );
        sTrans = sTrans->mNxtAT;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;

}

/* BUG-42724 : XA Ʈ����ǿ� ���� insert/update�� ���ڵ��� OID ����Ʈ��
 * ��ȸ�Ͽ� �÷��׸� �����Ѵ�.
 */
IDE_RC smxTransMgr::setOIDFlagForInDoubtTrans()
{
    smxTrans* sTrans;

    if ( mPreparedTransCnt > 0 )
    {
        sTrans = mPreparedTrans.mNxtPT;
        while ( sTrans != &mPreparedTrans )
        {
            IDE_ASSERT( sTrans->mCommitState == SMX_XA_PREPARED );

            IDE_TEST( sTrans->mOIDList->setOIDFlagForInDoubt() != IDE_SUCCESS );

            sTrans = sTrans->mNxtPT;
        }
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;

}

IDE_RC smxTransMgr::setRowSCNForInDoubtTrans()
{
    smxTrans* sTrans;

    if ( mPreparedTransCnt > 0 )
    {
        sTrans = mPreparedTrans.mNxtPT;
        while ( sTrans != &mPreparedTrans )
        {
            IDE_ASSERT( sTrans->mCommitState == SMX_XA_PREPARED );

            IDE_TEST( sTrans->mOIDList->setSCNForInDoubt(sTrans->mTransID) != IDE_SUCCESS );

            sTrans = sTrans->mNxtPT;
        }

        /* ---------------------------------------------
           recovery ���� Ʈ����� ���̺��� ��� ��Ʈ����
           transaction free list�� ����Ǿ� �����Ƿ�
           prepare transaction�� ���� �̸� �籸���Ѵ�.
           --------------------------------------------- */
        IDE_TEST( smxTransMgr::rebuildTransFreeList() != IDE_SUCCESS );
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;

}

/* ��� : XA Prepare Transaction�� ������ Table�� ���Ͽ�
 *       Transaction�� Table Info���� Record Count�� 1 ���� ��Ų��.
 *
 *       [BUG-26415] XA Ʈ������� Partial Rollback(Unique Volation)��
 *       Prepare Ʈ������� �����ϴ� ��� ���� �籸���� �����մϴ�.
 */
IDE_RC smxTransMgr::incRecCnt4InDoubtTrans( smTID aTransID,
                                            smOID aTableOID )
{
    smxTrans     * sTrans;
    smxTableInfo * sTableInfo = NULL;

    // BUG-31521 �� �Լ��� Prepare Transaction�� �����ϴ� ��Ȳ������
    //           ȣ�� �Ǿ�� �մϴ�.

    /* BUG-38151 Prepare Tx�� �ƴ� ���¿��� SCN�� ���� ��쿡��
     * __SM_SKIP_CHECKSCN_IN_STARTUP ������Ƽ�� ���� �ִٸ�
     * ������ ������ �ʰ� ���� ������ ����ϰ� �״�� �����Ѵ�. */
    IDE_TEST( mPreparedTransCnt <= 0 );

    sTrans = mPreparedTrans.mNxtPT;
    while ( sTrans != &mPreparedTrans )
    {
        IDE_ASSERT( sTrans->mCommitState == SMX_XA_PREPARED );

        if ( sTrans->mTransID == aTransID )
        {
            break;
        }

        sTrans = sTrans->mNxtPT;
    }

    IDE_ASSERT( sTrans != &mPreparedTrans );

    IDE_TEST( sTrans->getTableInfo( aTableOID, &sTableInfo ) != IDE_SUCCESS );
    IDE_ASSERT( sTableInfo != NULL );

    smxTableInfoMgr::incRecCntOfTableInfo( sTableInfo );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;

}

/* ��� : XA Prepare Transaction�� ������ Table�� ���Ͽ�
 *       Transaction�� Table Info���� Record Count�� 1 ���ҽ�Ų��.
 *
 *       [BUG-26415] XA Ʈ������� Partial Rollback(Unique Volation)��
 *       Prepare Ʈ������� �����ϴ� ��� ���� �籸���� �����մϴ�.
 */
IDE_RC smxTransMgr::decRecCnt4InDoubtTrans( smTID aTransID,
                                            smOID aTableOID )
{
    smxTrans     * sTrans;
    smxTableInfo * sTableInfo = NULL;

    // BUG-31521 �� �Լ��� Prepare Transaction�� �����ϴ� ��Ȳ������
    //           ȣ�� �Ǿ�� �մϴ�.

    /* BUG-38151 Prepare Tx�� �ƴ� ���¿��� SCN�� ���� ��쿡��
     * __SM_SKIP_CHECKSCN_IN_STARTUP ������Ƽ�� ���� �ִٸ�
     * ������ ������ �ʰ� ���� ������ ����ϰ� �״�� �����Ѵ�. */
    IDE_TEST( mPreparedTransCnt <= 0 );

    sTrans = mPreparedTrans.mNxtPT;
    while ( sTrans != &mPreparedTrans )
    {
        IDE_ASSERT( sTrans->mCommitState == SMX_XA_PREPARED );
        
        if ( sTrans->mTransID == aTransID )
        {
            break;
        }

        sTrans = sTrans->mNxtPT;
    }

    IDE_ASSERT( sTrans != &mPreparedTrans );

    IDE_TEST( sTrans->getTableInfo( aTableOID, &sTableInfo ) != IDE_SUCCESS );
    IDE_ASSERT( sTableInfo != NULL );

    smxTableInfoMgr::decRecCntOfTableInfo( sTableInfo );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;

}

UInt   smxTransMgr::getPrepareTransCount()
{
    return mPreparedTransCnt;
}

void * smxTransMgr::getNxtPreparedTrx( void  * aTrans )
{
    smxTrans  * sTrans = (smxTrans*)aTrans;

    if ( sTrans == &mPreparedTrans )
    {
        return NULL;
    }
    else
    {
        if ( sTrans->mNxtPT == &mPreparedTrans )
        {
            return NULL;
        }
        else
        {
            return sTrans->mNxtPT;
        }
    }

}

/*********************************************************
  function description: waitForLock, table lock  wait function�̸�,
                        smlLockMgr::lockTable�� ���Ͽ� �Ҹ���.
***********************************************************/
IDE_RC smxTransMgr::waitForLock( void     *aTrans,
                                 iduMutex *aMutex,
                                 ULong     aLockWaitMicroSec )
{
    SLong      sLockTimeOut = smuProperty::getLockTimeOut();
    smxTrans * sTrans       = (smxTrans*) aTrans;

    sTrans->mStatus    = SMX_TX_BLOCKED;
    sTrans->mStatus4FT = SMX_TX_BLOCKED;

    while(1)
    {
        /*
         * ��� ��ٷ� ����. ����� �� ���� ��û�� Resource��
         * ������ ���� �ʾҴٸ� Deadlock�� üũ�ϰ� ��Ӵ�� �Ѵ�.
         * PROJ-2620 
         * spin ��忡���� waitForLock �Լ��� ȣ����� �ʴ´�.
         */
        IDE_TEST( sTrans->suspendMutex( NULL, aMutex, sLockTimeOut )
                  != IDE_SUCCESS );

        if ( sTrans->mStatus == SMX_TX_BEGIN )
        {
            break;
        }

        /* Deadlock�� �߻����� ���� ��쿡 ���ؼ��� ���Ѵ�� �Ѵ�. �̸�
         * ���� Deadlock�� �߻����� �ʴ��� Check�Ѵ�. */
        IDE_TEST_RAISE( smLayerCallback::isCycle( sTrans->mSlotN ) == ID_TRUE,
                        err_deadlock );

        aLockWaitMicroSec = sTrans->getLockTimeoutByUSec( aLockWaitMicroSec );

        IDE_TEST( sTrans->suspendMutex( NULL /* Target Transaction */,
                                        aMutex,
                                        aLockWaitMicroSec )
                  != IDE_SUCCESS );

        IDE_TEST_RAISE( sTrans->mStatus != SMX_TX_BEGIN,
                        err_exceed_wait_time );

        break;
    }

    smLayerCallback::clearWaitItemColsOfTrans( ID_TRUE, sTrans->mSlotN );
    sTrans->mStatus    = SMX_TX_BEGIN;
    sTrans->mStatus4FT = SMX_TX_BEGIN;

    return IDE_SUCCESS;

    IDE_EXCEPTION(err_deadlock);
    {
        IDE_SET(ideSetErrorCode(smERR_ABORT_Aborted));
    }
    IDE_EXCEPTION(err_exceed_wait_time);
    {
        IDE_SET(ideSetErrorCode(smERR_ABORT_smcExceedLockTimeWait));
    }
    IDE_EXCEPTION_END;

    sTrans->mStatus    = SMX_TX_BEGIN;
    sTrans->mStatus4FT = SMX_TX_BEGIN;

    return IDE_FAILURE;
}

idBool smxTransMgr::isWaitForTransCase( void   * aTrans,
                                        smTID    aWaitTransID )
{
    smxTrans      *sTrans;
    smxTrans      *sWaitTrans;

    sTrans = (smxTrans *)aTrans;
    sWaitTrans = smxTransMgr::getTransByTID(aWaitTransID);

    /* replication self deadlock avoidance:
     * transactions that were begun by a receiver does not wait each other*/
    if ( ( sTrans->mReplID == sWaitTrans->mReplID ) &&
         ( sTrans->isReplTrans() == ID_TRUE ) &&
         ( sWaitTrans->isReplTrans() == ID_TRUE ) )
    {
        return ID_FALSE;
    }
    else
    {
        return ID_TRUE;
    }
}

/*********************************************************
  function description: waitForTrans
   record  lock  wait function�̸�,
  smcRecord, sdcRecord�� update ,delete �� ���Ͽ� �Ҹ���.
  ���� index module�� lockAllRow�� ���ؼ��� �Ҹ���.
***********************************************************/
IDE_RC smxTransMgr::waitForTrans( void     *aTrans,
                                  smTID     aWaitTransID,
                                  ULong     aLockWaitTime )
{
    smxTrans          *sTrans       = NULL;
    smxTrans          *sWaitTrans   = NULL;
    UInt               sState       = 0;
    SLong              sLockTimeOut = 0;

    sTrans       = (smxTrans *)aTrans;
    sWaitTrans   = smxTransMgr::getTransByTID( aWaitTransID );
    sLockTimeOut = smuProperty::getLockTimeOut();

    IDE_TEST( sWaitTrans->lock() != IDE_SUCCESS );
    sState = 1;

    sTrans->mStatus    = SMX_TX_BLOCKED;
    sTrans->mStatus4FT = SMX_TX_BLOCKED;

    if ( ( sWaitTrans->mTransID != aWaitTransID ) ||
         ( sWaitTrans->mStatus == SMX_TX_END ) )
    {
        IDE_TEST( sWaitTrans->unlock() != IDE_SUCCESS );
        sState = 0;
    }
    else
    {
        smLayerCallback::registRecordLockWait( sTrans->mSlotN,
                                               sWaitTrans->mSlotN );

        IDE_TEST( sWaitTrans->unlock() != IDE_SUCCESS );
        sState = 0;

        IDU_FIT_POINT( "1.BUG-42154@smxTransMgr::waitForTrans::afterRegistLockWait" );

        IDE_TEST( sTrans->suspend( sWaitTrans,
                                   aWaitTransID,
                                   NULL,
                                   sLockTimeOut )
                  != IDE_SUCCESS );

        if ( sTrans->mStatus == SMX_TX_BEGIN )
        {
            //waiting table���� aSlotN�� �࿡ ��⿭�� clear�Ѵ�.
            smLayerCallback::clearWaitItemColsOfTrans( ID_TRUE,
                                                       sTrans->mSlotN );

            return IDE_SUCCESS;
        }

        //dead lock test
        IDE_TEST_RAISE( smLayerCallback::isCycle( sTrans->mSlotN )
                        == ID_TRUE, err_deadlock );

        aLockWaitTime = sTrans->getLockTimeoutByUSec( aLockWaitTime );
        IDE_TEST( sTrans->suspend( sWaitTrans,
                                   aWaitTransID,
                                   NULL /* Mutex */,
                                   aLockWaitTime )
                  != IDE_SUCCESS );

        IDE_TEST_RAISE( sTrans->mStatus != SMX_TX_BEGIN,
                        err_exceed_wait_time );
    }

    //clear waiting tbl...
    smLayerCallback::clearWaitItemColsOfTrans( ID_TRUE,
                                               sTrans->mSlotN );

    sTrans->mStatus    = SMX_TX_BEGIN;
    sTrans->mStatus4FT = SMX_TX_BEGIN;

    return IDE_SUCCESS;

    IDE_EXCEPTION( err_deadlock );
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_Aborted ) );
    }
    IDE_EXCEPTION(err_exceed_wait_time);
    {
        IDE_SET(ideSetErrorCode(smERR_ABORT_smcExceedLockTimeWait));
    }
    IDE_EXCEPTION_END;

    // fix BUG-21402 Commit �ϴ� Transaction�� freeAllRecordLock���� �߿�
    // �̹� rollback�ؼ� free�� Transaction�� resume�õ��ϴ� ���� �����Ѵ�.
    if ( sState == 0 )
    {
        IDE_PUSH();
        IDE_ASSERT( sWaitTrans->lock() == IDE_SUCCESS );
        IDE_POP();
    }

    smLayerCallback::clearWaitItemColsOfTrans( ID_TRUE,
                                               sTrans->mSlotN );

    IDE_PUSH();
    IDE_ASSERT( sWaitTrans->unlock() == IDE_SUCCESS );
    IDE_POP();

    sTrans->mStatus    = SMX_TX_BEGIN;
    sTrans->mStatus4FT = SMX_TX_BEGIN;


    return IDE_FAILURE;
}

IDE_RC smxTransMgr::addTouchedPage( void      * aTrans,
                                    scSpaceID   aSpaceID,
                                    scPageID    aPageID,
                                    SShort      aCTSlotNum )
{

    smxTrans * sTrans = NULL;

    sTrans = (smxTrans *)aTrans;

    IDE_TEST( sTrans->addTouchedPage( aSpaceID,
                                      aPageID,
                                      aCTSlotNum )
              != IDE_SUCCESS );

    return IDE_SUCCESS;
    IDE_EXCEPTION_END;
    return IDE_FAILURE;
}

/***********************************************************************
 * Description : Free List�� �����ϴ� Transactino�� Free������ �����Ѵ�.
 ***********************************************************************/
void smxTransMgr::checkFreeTransList()
{
    UInt i;

    for( i = 0; i < mTransFreeListCnt; i++ )
    {
        mArrTransFreeList[i].validateTransFreeList();
    }
}

//for fix bug 8084
IDE_RC smxTransMgr::alloc4LayerCall(void** aTrans)
{

    return alloc((smxTrans**) aTrans);
}

//for fix bug 8084
IDE_RC smxTransMgr::freeTrans4LayerCall(void* aTrans)
{

    return freeTrans((smxTrans*)aTrans);
}


void smxTransMgr::getDummySmmViewSCN(smSCN * aSCN)
{

    SM_INIT_SCN(aSCN);

}


IDE_RC smxTransMgr::getDummySmmCommitSCN( void    * aTrans,
                                          idBool    /* aIsLegacyTrans */,
                                          void    * aStatus )
{

    smSCN sDummySCN;

    SM_INIT_SCN(&sDummySCN);
    smxTrans::setTransCommitSCN(aTrans,sDummySCN,aStatus);

    return IDE_SUCCESS;

}

void smxTransMgr::setSmmCallbacks( )
{

    mGetSmmViewSCN = smmDatabase::getViewSCN;
    mGetSmmCommitSCN = smmDatabase::getCommitSCN;

}

void smxTransMgr::unsetSmmCallbacks( )
{

    mGetSmmViewSCN = smxTransMgr::getDummySmmViewSCN;
    mGetSmmCommitSCN = smxTransMgr::getDummySmmCommitSCN;

}

/*************************************************************************
 * Description : ���� ������� Transaction�� �ִ��� Ȯ���մϴ�.
 *               BUG-29633 Recovery�� ��� Transaction�� End�������� �����ʿ�.
 *               Recovery���� ������ ������� Transaction�� �־�� �ȵȴ�.
 *
 *************************************************************************/
idBool smxTransMgr::existActiveTrans()
{
    SInt      sCurSlotID;
    smxTrans *sCurTrans;
    idBool    sExistUsingTrans = ID_FALSE;

    for ( sCurSlotID = 0 ; sCurSlotID < getCurTransCnt() ; sCurSlotID++ )
    {
        sCurTrans = getTransBySID( sCurSlotID );

        if ( sCurTrans->mStatus != SMX_TX_END )
        {
            sExistUsingTrans = ID_TRUE;

            ideLog::log( IDE_SERVER_0,
                         SM_TRC_TRANS_EXIST_ACTIVE_TRANS,
                         sCurTrans->mTransID,
                         sCurTrans->mStatus );
        }
    }
    return sExistUsingTrans;
}
