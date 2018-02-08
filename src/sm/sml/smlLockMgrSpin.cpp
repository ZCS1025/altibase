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
 * $Id: smlLockMgrSpin.cpp 82075 2018-01-17 06:39:52Z jina.kim $
 **********************************************************************/
/**************************************************************
 * FILE DESCRIPTION : smlLockMgr.cpp                          *
 * -----------------------------------------------------------*
 �� ��⿡�� �����ϴ� ����� ������ ũ�� 4�����̴�.

 1. lock table
 2. unlock table
 3. record lockó��
 4. dead lock detection


 - lock table
  �⺻������ table�� ��ǥ���� ���� ����� �ϴ� ���� ȣȯ�����ϸ�,
  grant list�� �ް� table lock�� ��� �ǰ�,
  lock conflict�� �߻��ϸ� table lock��� ����Ʈ�� request list
  �� �ް� �Ǹ�, lock waiting table�� ����ϰ� dead lock�˻��Ŀ�
  waiting�ϰ� �ȴ�.

  altibase������ lock  optimization�� ������ ���� �Ͽ�,
  �ڵ尡 �����ϰ� �Ǿ���.

  : grant lock node ������ ���̱� ���Ͽ�  lock node����
    lock slot���Թ� �̿�.
    -> ������ Ʈ�������  table�� ���Ͽ� lock�� ��Ұ�,
      ���� �䱸�ϴ� table lock mode�� ȣȯ�����ϸ�, ���ο�
      grant node�� �����ϰ� grant list�� ���� �ʰ�,
      ���� grant node�� lock mode�� conversion�Ͽ� �����Ѵ�.
    ->lock conflict������,   grant�� lock node�� 1���̰�,
      �װ��� �ٷ� ��  Ʈ������� ���, ���� grant lock node��
      lock mode�� conversion�Ͽ� �����Ѵ�.

  : unlock table�� request list�� �ִ� node��
    grant list���� move�Ǵ� ����� ���̱� ���Ͽ� lock node�ȿ�
    cvs lock node pointer�� �����Ͽ���.
    -> lock conflict �̰� Ʈ������� ������ table�� ���Ͽ� grant��
      lock node�� ������ �ִ� ���, request list�� �� ���ο�
      lock node�� cvs lock�� ������ grant lock node�� pointing
      �ϰ���.
   %���߿� �ٸ� Ʈ������� unlock table�� request�� �־��� lock
   node�� ���� ���ŵ� grant mode�� ȣȯ�����Ҷ� , �� lock node��
   grant list���� move�ϴ� ���, ���� grant lock node�� lock mode
   �� conversion�Ѵ�.

 - unlock table.
   lock node�� grant�Ǿ� �ִ� ��쿡�� ������ ���� �����Ѵ�.
    1> ���ο� table�� ��ǥ���� grant lock mode�� �����Ѵ�.
    2>  grant list���� lock node�� ���Ž�Ų��.
      -> Lock node�ȿ� lock slot�� 2�� �̻��ִ� ��쿡��
       ���ž���.
   lock node�� request�Ǿ� �־��� ��쿡�� request list����
   �����Ѵ�.

   request list�� �ִ� lock node�߿�
   ���� ���ŵ� grant lock mode�� ȣȯ������
   Transaction�� �� ������ ���� �����.
   1.  request list���� lock node����.
   2.  table lock�������� grant lock mode�� ����.
   3.  cvs lock node�� ������,�� lock node��
      grant list���� move�ϴ� ���, ���� grant lock node��
      lock mode �� conversion�Ѵ�.
   4. cvs lock node�� ������ grant list�� lock node add.
   5.  waiting table���� �ڽ��� wainting �ϰ� �ִ� Ʈ�����
       �� ��� ���� clear.
   6.  waiting�ϰ� �ִ� Ʈ������� resume��Ų��.
   7.  lock slot, lock node ���� �õ�.

 - waiting table ǥ��.
   waiting table�� chained matrix�̰�, ������ ���� ǥ���ȴ�.

     T1   T2   T3   T4   T5   T6

  T1                                        |
                                            | record lock
  T2                                        | waiting list
                                            |
  T3                6          USHORT_MAX   |
                                            |
  T4      6                                 |
                                            |
  T5                                        v

  T6     USHORT_MAX
    --------------------------------->
    table lock waiting or transaction waiting list

    T3�� T4, T6�� ���Ͽ� table lock waiting�Ǵ�
    transaction waiting(record lock�� ���)�ϰ� �ִ�.

    T2�� ���Ͽ�  T4,T6�� record lock waiting�ϰ� ������,
    T2�� commit or rollback�ÿ� T4,T6 ���� T2���� ���
    ���¸� clear�ϰ� resume��Ų��.

 -  record lockó��
   recod lock grant, request list�� node�� ����.
   �ٸ� waiting table����  ����Ϸ��� transaction A�� column��
   record lock ��⸦ ��Ͻ�Ű��, transaction A abort,commit��
   �߻��ϸ� �ڽſ��� ��ϵ� record lock ��� list�� ��ȸ�ϸ�
   record lock�����¸� clear�ϰ�, Ʈ����ǵ��� �����.


 - dead lock detection.
  dead lock dectioin�� ������ ���� �ΰ��� ��쿡
  ���Ͽ� �����Ѵ�.

   1. Tx A�� table lock�� conflict�� �߻��Ͽ� request list�� �ް�,
     Tx A�� ���� ���� waiting list�� ����Ҷ�,waiting table����
     Tx A�� ���Ͽ� cycle�� �߻��ϸ� transaction�� abort��Ų��.


   2.Tx A��  record R1�� update�õ��ϴٰ�,
   �ٸ� Tx B�� ���Ͽ� �̹�  active���� record lock�� ����Ҷ�.
    -  Tx B�� record lock ��⿭����  Tx A�� ����ϰ�,
       Tx A�� Tx B�� ������� ����ϰ� ����  waiting table����
       Tx A�� ���Ͽ� cycle�� �߻��ϸ� transaction�� abort��Ų��.


*************************************************************************/
#include <idl.h>
#include <idu.h>
#include <ideErrorMgr.h>
#include <smErrorCode.h>
#include <smlDef.h>
#include <smDef.h>
#include <smr.h>
#include <smc.h>
#include <sml.h>
#include <smlReq.h>
#include <smu.h>
//#include <smrLogHeadI.h>
#include <sct.h>
#include <smx.h>
//#include <smxTransMgr.h>

smlLockMode smlLockMgr::getLockMode( const SLong aLock )
{
    SInt sMode;
    SInt sFlag = 0;

    for ( sMode = 0 ; sMode < SML_NUMLOCKTYPES ; sMode++ )
    {
        if ( (aLock & mLockMask[sMode]) != 0 )
        {
            sFlag |= mLockModeToMask[sMode];
        }
        else
        {
            /* continue */
        }
    }

    return mDecisionTBL[sFlag];
}

smlLockMode smlLockMgr::getLockMode( const smlLockItemSpin* aItem )
{
    return getLockMode(aItem->mLock);
}

smlLockMode smlLockMgr::getLockMode( const smlLockNode* aNode )
{
    return mDecisionTBL[aNode->mFlag];
}

SLong smlLockMgr::getGrantedCnt( const SLong aLock, const smlLockMode aMode )
{
    SInt    i;
    SLong   sSum;

    if ( aMode == SML_NLOCK )
    {
        sSum = 0;
        for ( i = 0 ; i < SML_NUMLOCKTYPES ; i++ )
        {
            sSum += (aLock & mLockMask[i]) >> mLockBit[i];
        }
    }
    else
    {
        sSum = (aLock & mLockMask[aMode]) >> mLockBit[aMode];
    }

    return sSum;
}

SLong smlLockMgr::getGrantedCnt( const smlLockItemSpin* aItem, const smlLockMode aMode )
{
    return getGrantedCnt( aItem->mLock, aMode );
}



IDE_RC smlLockMgr::initializeSpin( UInt aTransCnt )
{
    SInt i;
    SInt j;
    IDE_ASSERT( smuProperty::getLockMgrType() == 1 );

    mTransCnt = aTransCnt;
    mSpinSlotCnt = (mTransCnt + 63) / 64;

    IDE_ASSERT( mTransCnt > 0 );
    IDE_ASSERT( mSpinSlotCnt > 0 );

    IDE_TEST_RAISE( iduMemMgr::malloc( IDU_MEM_SM_SML,
                                       mTransCnt * mTransCnt * sizeof(SInt),
                                       (void**)&mPendingArray ) != IDE_SUCCESS,
                    insufficient_memory );
    IDE_TEST_RAISE(iduMemMgr::malloc( IDU_MEM_SM_SML,
                                      mTransCnt * sizeof(SInt*),
                                      (void**)&mPendingMatrix ) != IDE_SUCCESS,
                   insufficient_memory );
    IDE_TEST_RAISE( iduMemMgr::calloc( IDU_MEM_SM_SML,
                                       mTransCnt, sizeof(SInt),
                                       (void**)&mPendingCount ) != IDE_SUCCESS,
                    insufficient_memory );
    IDE_TEST_RAISE( iduMemMgr::malloc( IDU_MEM_SM_SML,
                                       mTransCnt * mTransCnt * sizeof(SInt),
                                       (void**)&mIndicesArray ) != IDE_SUCCESS,
                    insufficient_memory );
    IDE_TEST_RAISE( iduMemMgr::malloc( IDU_MEM_SM_SML,
                                       mTransCnt * sizeof(SInt*),
                                       (void**)&mIndicesMatrix ) != IDE_SUCCESS,
                    insufficient_memory );
    IDE_TEST_RAISE( iduMemMgr::malloc( IDU_MEM_SM_SML,
                                       mTransCnt * sizeof(idBool),
                                       (void**)&mIsCycle ) != IDE_SUCCESS,
                    insufficient_memory );
    IDE_TEST_RAISE( iduMemMgr::malloc( IDU_MEM_SM_SML,
                                       mTransCnt * sizeof(idBool),
                                       (void**)&mIsChecked ) != IDE_SUCCESS,
                    insufficient_memory );
    IDE_TEST_RAISE( iduMemMgr::malloc( IDU_MEM_SM_SML,
                                       mTransCnt * sizeof(SInt),
                                       (void**)&mDetectQueue ) != IDE_SUCCESS,
                    insufficient_memory );
    IDE_TEST_RAISE( iduMemMgr::calloc( IDU_MEM_SM_SML,
                                       mTransCnt,
                                       sizeof(ULong),
                                       (void**)&mSerialArray ) != IDE_SUCCESS,
                    insufficient_memory );

    for ( i = 0 ; i < mTransCnt ; i++ )
    {
        mIsCycle[i]         = ID_FALSE;
        mIsChecked[i]       = ID_FALSE;

        mPendingMatrix[i]   = &(mPendingArray[mTransCnt * i]);
        mIndicesMatrix[i]   = &(mIndicesArray[mTransCnt * i]);

        for ( j = 0 ; j < mTransCnt ; j++ )
        {
            mPendingMatrix[i][j] = SML_EMPTY;
            mIndicesMatrix[i][j] = SML_EMPTY;
        }
    }

    mStopDetect     = 0;
    mPendSerial     = 0;
    mTXPendCount    = 0;
    IDE_TEST( mDetectDeadlockThread.launch( detectDeadlockSpin, NULL )
              != IDE_SUCCESS );

    mLockTableFunc                  = lockTableSpin;
    mUnlockTableFunc                = unlockTableSpin;
    mInitLockItemFunc               = initLockItemSpin;
    mAllocLockItemFunc              = allocLockItemSpin;
    mDidLockReleasedFunc            = didLockReleasedSpin;
    mRegistRecordLockWaitFunc       = registLockWaitSpin;
    mFreeAllRecordLockFunc          = freeAllRecordLockSpin;
    mClearWaitItemColsOfTransFunc   = clearWaitItemColsOfTransSpin;

    return IDE_SUCCESS;

    IDE_EXCEPTION( insufficient_memory );
    {
        IDE_SET(ideSetErrorCode(idERR_ABORT_InsufficientMemory));
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}


IDE_RC smlLockMgr::destroySpin()
{
    (void)acpAtomicSet32( &mStopDetect, 1 );
    IDE_TEST( mDetectDeadlockThread.join()      != IDE_SUCCESS );
    IDE_TEST( iduMemMgr::free( mIndicesMatrix ) != IDE_SUCCESS );
    mIndicesMatrix = NULL;
    IDE_TEST( iduMemMgr::free( mIndicesArray  ) != IDE_SUCCESS );
    mIndicesArray = NULL;
    IDE_TEST( iduMemMgr::free( mPendingCount  ) != IDE_SUCCESS );
    mPendingCount = NULL;
    IDE_TEST( iduMemMgr::free( mPendingArray  ) != IDE_SUCCESS );
    mPendingArray = NULL;
    IDE_TEST( iduMemMgr::free( mPendingMatrix ) != IDE_SUCCESS );
    mPendingMatrix =  NULL;
    IDE_TEST( iduMemMgr::free( mIsCycle       ) != IDE_SUCCESS );
    mIsCycle = NULL;
    IDE_TEST( iduMemMgr::free( mIsChecked     ) != IDE_SUCCESS );
    mIsChecked = NULL; 

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smlLockMgr::allocLockItemSpin( void ** aLockItem )
{
    ssize_t sAllocSize;

    sAllocSize = sizeof(smlLockItemSpin) 
                 + ( (ID_SIZEOF(ULong) * mSpinSlotCnt) )
                 - ID_SIZEOF(ULong);

    IDE_TEST_RAISE( iduMemMgr::malloc( IDU_MEM_SM_SML,
                                       sAllocSize,
                                       aLockItem ) != IDE_SUCCESS,
                    insufficient_memory );

    return IDE_SUCCESS;

    IDE_EXCEPTION( insufficient_memory );
    {
        IDE_SET(ideSetErrorCode(idERR_ABORT_InsufficientMemory));
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smlLockMgr::initLockItemSpin( scSpaceID          aSpaceID,
                                     ULong              aItemID,
                                     smiLockItemType    aLockItemType,
                                     void             * aLockItem )
{
    smlLockItemSpin*    sLockItem;
    SInt                i;

    IDE_TEST( initLockItemCore( aSpaceID, 
                                aItemID,
                                aLockItemType, 
                                aLockItem )
            != IDE_SUCCESS );

    sLockItem = (smlLockItemSpin*) aLockItem;

    sLockItem->mLock = 0;
    for ( i = 0 ; i < SML_NUMLOCKTYPES ; i++ )
    {
        sLockItem->mPendingCnt[i] = 0;
    }
    idlOS::memset( sLockItem->mOwner, 0, sizeof(ULong) * mSpinSlotCnt );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;
    return IDE_FAILURE;
}

IDE_RC smlLockMgr::lockTableSpin( SInt          aSlot,
                                  smlLockItem  *aLockItem,
                                  smlLockMode   aLockMode,
                                  ULong         aLockWaitMicroSec,
                                  smlLockMode  *aCurLockMode,
                                  idBool       *aLocked,
                                  smlLockNode **aLockNode,
                                  smlLockSlot **aLockSlot,
                                  idBool        aIsExplicit )
{
    smlLockItemSpin*    sLockItem = (smlLockItemSpin*)aLockItem;
    smlLockNode*        sCurTransLockNode = NULL;
    smlLockSlot*        sNewSlot;
    idvSQL*             sStat;
    idvSession*         sSession;
    UInt                sLockEnable = 1;

    SLong       sOldLock;
    SLong       sCurLock;
    SLong       sNewLock;
    SLong       sLockDelta;
    smlLockMode sOldMode;
    smlLockMode sCurMode;
    smlLockMode sNewMode;
    idBool      sIsNewLock = ID_FALSE;
    idBool      sDone;
    SLong       sLoops = 0;
    SInt        sIndex;
    SInt        sBitIndex;
    SInt        i;

    SInt        sSleepTime = smuProperty::getLockMgrMinSleep();
    acp_time_t  sBeginTime = acpTimeNow();
    acp_time_t  sCurTime;

    ULong       sOwner;
    ULong       sOwnerDelta;
    SInt        sOwnerIndex;

    PDL_Time_Value      sTimeOut;

    /* BUG-32237 [sm_transaction] Free lock node when dropping table.
     * DropTablePending ���� �����ص� freeLockNode�� �����մϴ�. */
    /* �ǵ��� ��Ȳ�� �ƴϱ⿡ Debug��忡���� DASSERT�� �����Ŵ.
     * ������ release ��忡���� �׳� rebuild �ϸ� ���� ����. */
    if ( sLockItem == NULL )
    {
        IDE_DASSERT( 0 );

        IDE_RAISE( error_table_modified );
    }
    else
    {
        /* nothing to do... */
    }

    sStat = smLayerCallback::getStatisticsBySID( aSlot );

    sSession = ( ( sStat == NULL ) ? NULL : sStat->mSess );

    // To fix BUG-14951
    // smuProperty::getTableLockEnable�� TABLE���� ����Ǿ�� �Ѵ�.
    // (TBSLIST, TBS �� DBF���� �ش� �ȵ�)
    /* BUG-35453 -  add TABLESPACE_LOCK_ENABLE property
     * TABLESPACE_LOCK_ENABLE �� TABLE_LOCK_ENABLE �� �����ϰ�
     * tablespace lock�� ���� ó���Ѵ�. */
    switch ( sLockItem->mLockItemType )
    {
        case SMI_LOCK_ITEM_TABLE:
            sLockEnable = smuProperty::getTableLockEnable();
            break;

        case SMI_LOCK_ITEM_TABLESPACE:
            sLockEnable = smuProperty::getTablespaceLockEnable();
            break;

        default:
            break;
    }

    if ( sLockEnable == 0 )
    {
        if ( ( aLockMode != SML_ISLOCK ) && ( aLockMode != SML_IXLOCK ) )
        {
            if ( sLockItem->mLockItemType == SMI_LOCK_ITEM_TABLESPACE )
            {
                IDE_RAISE( error_lock_tablespace_use );
            }
            else
            {
                IDE_RAISE( error_lock_table_use );
            }
        }
        else
        {
            /* nothing to do */ 
        }

        if ( aLocked != NULL )
        {
            *aLocked = ID_TRUE;
        }
        else
        {
            /* nothing to do */ 
        }

        if ( aLockSlot != NULL )
        {
            *aLockSlot = NULL;
        }
        else
        {
            /* nothing to do */ 
        }

        if ( aCurLockMode != NULL )
        {
            *aCurLockMode = aLockMode;
        }
        else
        {
            /* nothing to do */ 
        }

        return IDE_SUCCESS;
    }
    else  /* sLockEnable != 0 */
    {
        /* nothing to do */ 
    } 

    if ( aLocked != NULL )
    {
        *aLocked = ID_TRUE;
    }
    else
    {
        /* nothing to do */ 
    }

    if ( aLockSlot != NULL )
    {
        *aLockSlot = NULL;
    }
    else
    {
        /* nothing to do */ 
    }

    // Ʈ������� ���� statement�� ���Ͽ�,
    // ���� table A�� ���Ͽ� lock�� ��Ҵ� lock node�� ã�´�.
    sCurTransLockNode = findLockNode( sLockItem,aSlot );
    // case 1: ������ Ʈ�������  table A��  lock�� ��Ұ�,
    // ������ ���� lock mode�� ���� ����� �ϴ� ����� ��ȯ�����
    // ������ �ٷ� return!
    if ( sCurTransLockNode != NULL )
    {
        sIsNewLock = ID_FALSE;

        if ( mConversionTBL[getLockMode(sCurTransLockNode)][aLockMode]
             == getLockMode( sCurTransLockNode ) )
        {
            /* PROJ-1381 Fetch Across Commits
             * 1. IS Lock�� �ƴ� ���
             * 2. ������ �������� Lock Mode�� Lock�� ���� ���
             * => Lock�� ���� �ʰ� �ٷ� return �Ѵ�. */
            if ( ( aLockMode != SML_ISLOCK ) ||
                 ( sCurTransLockNode->mArrLockSlotList[aLockMode].mLockSequence != 0 ) )
            {
                if ( aCurLockMode != NULL )
                {
                    *aCurLockMode = getLockMode( sCurTransLockNode );
                }
                else
                {
                    /* nothing to do */ 
                }

                IDV_SESS_ADD( sSession, IDV_STAT_INDEX_LOCK_ACQUIRED, 1 );

                return IDE_SUCCESS;
            }
            else
            {
                /* nothing to do */ 
            }
        }
    }
    else /* sCurTransLockNode == NULL */
    {
        sIsNewLock = ID_TRUE;

        /* allocate lock node and initialize */
        IDE_TEST( allocLockNodeAndInit( aSlot,
                                        aLockMode,
                                        sLockItem,
                                        &sCurTransLockNode,
                                        aIsExplicit )
                  != IDE_SUCCESS );

        sCurTransLockNode->mBeGrant = ID_FALSE;
        sCurTransLockNode->mDoRemove = ID_FALSE;
        /* Add Lock Node to a transaction */
        addLockNode( sCurTransLockNode, aSlot );
    }

    IDE_EXCEPTION_CONT(BEGIN_LOOP);
    sCurLock = sLockItem->mLock;
    sCurMode = getLockMode(sCurLock);

    if ( mCompatibleTBL[sCurMode][aLockMode] == ID_TRUE )
    {
        IDE_TEST_CONT( getGrantedCnt( sCurLock, aLockMode ) >= mLockMax[aLockMode],
                       TRY_TIMEOUT );
        for ( i = 0 ; i < SML_NUMLOCKTYPES ; i++ )
        {
            if ( mPriority[i] == aLockMode )
            {
                break;
            }
            IDE_TEST_CONT( sLockItem->mPendingCnt[mPriority[i]] != 0, TRY_TIMEOUT );
        }

        if ( ( sIsNewLock == ID_TRUE ) || ( aLockMode == SML_ISLOCK ) )
        {
            sOldMode    = SML_NLOCK;
            sNewMode    = aLockMode;
            sLockDelta  = mLockDelta[aLockMode];
        }
        else
        {
            sOldMode    = getLockMode(sCurTransLockNode);
            sNewMode    = mConversionTBL[sCurMode][aLockMode];
            sLockDelta  = mLockDelta[sNewMode] - mLockDelta[sOldMode];
        }

        do
        {
            sNewLock = sCurLock + sLockDelta;
            sOldLock = acpAtomicCas64( &(sLockItem->mLock), sNewLock, sCurLock );

            if ( sOldLock == sCurLock )
            {
                /* CAS Successful, lock held */
                sDone = ID_TRUE;
            }
            else
            {
                sCurLock = sOldLock;
                sCurMode = getLockMode(sCurLock);

                IDE_TEST_CONT( mCompatibleTBL[sCurMode][aLockMode] == ID_FALSE,
                               TRY_TIMEOUT );
        
                sDone = ID_FALSE;
            }
        } while ( sDone == ID_FALSE );
    }
    else if ( ( getGrantedCnt(sLockItem) == 1 ) && ( sIsNewLock == ID_FALSE ) )
    {
        /* ---------------------------------------
         * lock conflict������, grant�� lock node�� 1���̰�,
         * �װ��� �ٷ� ��  Ʈ������� ��쿡�� request list�� ���� �ʰ�
         * ���� grant��  lock node�� lock mode�� table�� ��ǥ��,
         * grant lock mode��  �����Ѵ�.
         * --------------------------------------- */
        if ( aLockMode == SML_ISLOCK )
        {
            sOldMode = SML_NLOCK;
            sNewMode = SML_ISLOCK;
            sNewLock = sCurLock + mLockDelta[SML_ISLOCK];
        }
        else
        {
            sOldMode = getLockMode(sCurTransLockNode);
            sNewMode = mConversionTBL[sCurMode][aLockMode];
            sNewLock = sCurLock + mLockDelta[sNewMode] - mLockDelta[sOldMode];
        }
        sOldLock = acpAtomicCas64( &(sLockItem->mLock), sNewLock, sCurLock );

        IDE_TEST_CONT( sOldLock != sCurLock, TRY_TIMEOUT );
    }
    else
    {
        IDE_CONT(TRY_TIMEOUT);
    }

    sCurTransLockNode->mLockCnt++;

    setLockModeAndAddLockSlotSpin( aSlot, 
                                   sCurTransLockNode, 
                                   aCurLockMode,
                                   sNewMode, 
                                   ID_TRUE, 
                                   &sNewSlot,
                                   sOldMode, 
                                   sNewMode );

    if ( aLockNode != NULL )
    {
        *aLockNode = sCurTransLockNode;
    }
    else
    {
        /* nothing to do */ 
    }

    if ( aLockSlot != NULL )
    {
        *aLockSlot = sNewSlot;
    }
    else
    {
        /* nothing to do */ 
    }

    if ( sLoops != 0 )
    {
        (void)acpAtomicDec32( &(sLockItem->mPendingCnt[aLockMode]) );
        clearWaitItemColsOfTransSpin( ID_FALSE, aSlot );
        decTXPendCount();
        smxTransMgr::setTransBegunBySID(aSlot);
    }
    else
    {
        /* nothing to do */ 
    }

    IDE_DASSERT( sLockItem->mLock != 0 );

    if ( sIsNewLock == ID_TRUE )
    {
        calcIndex(aSlot, sIndex, sBitIndex);
        (void)acpAtomicAdd64( &(sLockItem->mOwner[sIndex]),
                              ID_ULONG(1) << sBitIndex );
        sCurTransLockNode->mBeGrant = ID_TRUE;
    }
    else
    {
        /* nothing to do */ 
    }
                
    IDV_SESS_ADD(sSession, IDV_STAT_INDEX_LOCK_ACQUIRED, 1);

    return IDE_SUCCESS;

    IDE_EXCEPTION_CONT(TRY_TIMEOUT);
    {
        /* check timeout */
        sCurTime = acpTimeNow();
        IDE_TEST_RAISE( (ULong)(sCurTime - sBeginTime) > aLockWaitMicroSec,
                        err_exceed_wait_time );
        IDE_TEST( iduCheckSessionEvent( sStat ) != IDE_SUCCESS );

        if ( sLoops == 0 )
        {
            /* Increase pending count */
            (void)acpAtomicInc32( &(sLockItem->mPendingCnt[aLockMode]) );
            smxTransMgr::setTransBlockedBySID( aSlot );
            beginPending( aSlot );
            incTXPendCount();
        }
        else
        {
            /* Another loop */
        }

        for ( i = 0 ; i < mSpinSlotCnt ; i++ )
        {
            sOwner = sLockItem->mOwner[i];

            while ( sOwner != 0 )
            {
                sOwnerIndex = acpBitFfs32(sOwner);

                if ( sOwnerIndex != aSlot )
                {
                    sOwnerDelta = ID_ULONG(1) << sOwnerIndex;

                    registLockWaitSpin( aSlot, i * 64 + sOwnerIndex );
                    sOwner &= ~sOwnerDelta;
                }
            }
        }

        /*
         * add myself to deadlock detection matrix
         * and check deadlock
         */
        IDE_TEST_RAISE( isCycle(aSlot) == ID_TRUE, err_deadlock );

        /* loop to try again */
        sLoops++;

        if ( ( sLoops % smuProperty::getLockMgrSpinCount() ) == 0 )
        {
            sTimeOut.set(0, sSleepTime);
            idlOS::sleep(sTimeOut);
            sSleepTime = IDL_MIN(sSleepTime * 2, smuProperty::getLockMgrMaxSleep());
        }
        else
        {
            idlOS::thr_yield();
        }

        /* try again */
        IDE_CONT(BEGIN_LOOP);
    }

    IDE_EXCEPTION( error_table_modified );
    {
        IDE_SET( ideSetErrorCode( smERR_REBUILD_smiTableModified ) );
    }
    IDE_EXCEPTION(error_lock_table_use);
    {
        IDE_SET(ideSetErrorCode(smERR_ABORT_TableLockUse));
    }
    IDE_EXCEPTION(error_lock_tablespace_use);
    {
        IDE_SET(ideSetErrorCode(smERR_ABORT_TablespaceLockUse));
    }
    IDE_EXCEPTION(err_deadlock);
    {
        IDE_SET(ideSetErrorCode(smERR_ABORT_Aborted));
    }
    IDE_EXCEPTION(err_exceed_wait_time);
    {
        IDE_SET(ideSetErrorCode(smERR_ABORT_smcExceedLockTimeWait));
    }

    IDE_EXCEPTION_END;

    clearWaitItemColsOfTransSpin( ID_FALSE, aSlot );
    if ( sLoops != 0 )
    {
        (void)acpAtomicDec32( &(sLockItem->mPendingCnt[aLockMode]) );
        smxTransMgr::setTransBegunBySID(aSlot);
        decTXPendCount();
    }
    if ( sIsNewLock == ID_TRUE )
    {
        /* tried new lock but not held */
        removeLockNode( sCurTransLockNode );
        (void)freeLockNode( sCurTransLockNode );
    }

    return IDE_FAILURE;
}

IDE_RC smlLockMgr::unlockTableSpin( SInt          aSlot,
                                    smlLockNode  *aLockNode,
                                    smlLockSlot  *aLockSlot,
                                    idBool     /* aDoMutexLock */ )
{
    smlLockItemSpin*    sLockItem;
    smlLockNode*    sLockNode;
    smlLockSlot*    sLockSlot = NULL;
    smlLockSlot*    sLockSlotAfter;
    smlLockMode     sOldMode;
    smlLockMode     sNewMode;

    SLong           sLockDelta;
    SInt            sIndex;
    SInt            sBitIndex;
    SInt            i;

    idvSQL      *sStat;
    idvSession  *sSession;

    sStat = smLayerCallback::getStatisticsBySID( aSlot );

    sSession = ( (sStat == NULL) ? NULL : sStat->mSess );

    if ( aLockNode == NULL )
    {
        sLockNode = aLockSlot->mLockNode;
        sLockSlot = aLockSlot;
    
        sOldMode    = sLockSlot->mOldMode;
        sNewMode    = sLockSlot->mNewMode;
        /* check whether future lock exists */
        sLockDelta  = 0;
        for ( i = 0 ; i < SML_NUMLOCKTYPES ; i++ )
        {
            sLockSlotAfter = &(sLockNode->mArrLockSlotList[i]);

            if ( (sLockSlotAfter != sLockSlot) &&
                 (sLockSlotAfter->mOldMode == sNewMode) )
            {
                IDE_DASSERT( sLockSlotAfter->mLockSequence != 0 );
                sLockSlotAfter->mOldMode = sOldMode;

                sNewMode = sOldMode = SML_NLOCK;
                break;
            }
            else
            {
                /* continue */
            }
        }
        sLockDelta  = mLockDelta[sOldMode] - mLockDelta[sNewMode];

        removeLockSlot(sLockSlot);
        sLockNode->mFlag &= ~(sLockSlot->mMask);
        sLockNode->mLockCnt--;
    }
    else /* aLockNode == NULL */
    {
        sLockNode   = aLockNode;
        sLockDelta  = 0;

        for ( i = 0 ; i < SML_NUMLOCKTYPES ; i++ )
        {
            sLockSlot = &(sLockNode->mArrLockSlotList[i]);
            if ( sLockSlot->mLockSequence != 0 )
            {
                sOldMode    = sLockSlot->mOldMode;
                sNewMode    = sLockSlot->mNewMode;
                sLockDelta += mLockDelta[sOldMode] - mLockDelta[sNewMode];
                removeLockSlot(sLockSlot);
        
                sLockNode->mFlag &= ~(sLockSlot->mMask);
            }
            else
            {
                /* continue */
            }
        }

        IDE_DASSERT( sLockNode->mFlag == 0 );
        sLockNode->mLockCnt = 0;
    }

    IDE_ASSERT( sLockNode != NULL );
    sLockItem = (smlLockItemSpin*)sLockNode->mLockItem;
    IDE_DASSERT( sLockItem->mLock != 0 );

    calcIndex(aSlot, sIndex, sBitIndex);
    IDE_DASSERT( (sLockItem->mOwner[sIndex] & (ID_ULONG(1) << sBitIndex)) != 0 );

    if ( sLockDelta != 0 )
    {
        (void)acpAtomicAdd64( &(sLockItem->mLock), sLockDelta );
    }

    if ( sLockNode->mLockCnt == 0 )
    {
        (void)acpAtomicSub64( &(sLockItem->mOwner[sIndex] ), ID_ULONG(1) << sBitIndex);
        removeLockNode( sLockNode );
        IDE_TEST( freeLockNode(sLockNode) != IDE_SUCCESS );
    
        IDV_SESS_ADD( sSession, IDV_STAT_INDEX_LOCK_RELEASED, 1 );
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;
    return IDE_FAILURE;
}

void* smlLockMgr::detectDeadlockSpin( void* /* aArg */ )
{
    SInt            i;
    SInt            j;
    SInt            k;
    SInt            l;
    SInt            sWaitSlotNo;
    SInt            sQueueHead;
    SInt            sQueueTail;
    SInt            sLastDetected;
    idBool          sDetected;
    idBool          sDetectAgain;
    ULong           sSerialMax;
    SInt            sSlotIDMax;
    SInt            sSleepTime;
    PDL_Time_Value  sTimeVal;

    sDetectAgain = ID_FALSE;

    sLastDetected = -1;
    while ( mStopDetect == 0 )
    {
    
        sSleepTime = smuProperty::getLockMgrDetectDeadlockInterval();
        if ( sSleepTime != 0 )
        {
            sTimeVal.set(sSleepTime, 0);
            idlOS::sleep(sTimeVal);
        }

        if ( mTXPendCount == 0 )
        {
            continue;
        }
        else
        {
            /* fall through */
        }

        IDL_MEM_BARRIER;
        if ( sLastDetected != -1 )
        {
            if ( mIsCycle[sLastDetected] == ID_TRUE )
            {
                continue;
            }
            else
            {
                sLastDetected = -1;
            }
        }

        for ( i = 0 ; i < mTransCnt ; i++ )
        {
            mIsChecked[i] = ID_FALSE;
        }

        for ( i = 0 ; i < mTransCnt ; i++ )
        {
            if ( ( mIsChecked[i] == ID_TRUE ) ||
                 ( smxTransMgr::getTransStatus(i) != SMX_TX_BLOCKED ) )
            {
                continue;
            }
            else
            {
                /* fall through */
            }

            mDetectQueue[0] = i;
            sQueueHead = 1;
            sQueueTail = 1;

            /* peek all slot IDs that are waiting on TX[i] */
            for ( j = 0 ; j < mPendingCount[i] ; j++ )
            {
                sWaitSlotNo = mPendingMatrix[i][j];

                if ( (sWaitSlotNo !=  i) &&
                     (sWaitSlotNo != -1) &&
                     (mIsChecked[sWaitSlotNo] == ID_FALSE) )
                {
                    mIsChecked[sWaitSlotNo] = ID_TRUE;
                    mDetectQueue[sQueueHead] = sWaitSlotNo;
                    sQueueHead++;
                }
                else
                {
                    /* continue */
                }

            }

            /* extract graph and check cycle */
            sDetected = ID_FALSE;
            while ( sQueueHead != sQueueTail )
            {
                k = mDetectQueue[sQueueTail];
                sQueueTail++;

                /* peek all slot IDs that are waiting on TX[i] */
                for ( j = 0 ; j < mPendingCount[k] ; j++ )
                {
                    sWaitSlotNo = mPendingMatrix[k][j];

                    if ( (sWaitSlotNo != -1) &&
                         (sWaitSlotNo !=  k) &&
                         (mIsChecked[sWaitSlotNo] == ID_FALSE) )
                    {
                        for ( l = 0 ; l < sQueueHead ; l++ )
                        {
                            if ( mDetectQueue[l] == sWaitSlotNo )
                            {
                                sDetected = ID_TRUE;
                                break;
                            }
                            else
                            {
                                /* continue */
                            }
                        }

                        if ( sDetected != ID_TRUE )
                        {
                            mIsChecked[sWaitSlotNo] = ID_TRUE;
                            mDetectQueue[sQueueHead] = sWaitSlotNo;
                            sQueueHead++;
                        }
                    }
                    else
                    {
                        /* continue */
                    }
                } /* for j */
            } /* while for queue */

            if ( sDetected == ID_TRUE )
            {
                if ( sDetectAgain == ID_TRUE )
                {
                    /* find smallest serial in blocked transactions */
                    sSlotIDMax = -1;
                    sSerialMax = 0;
                    for ( j = 0 ; j < sQueueHead ; j++ )
                    {
                        k = mDetectQueue[j];
                        if ( sSerialMax < mSerialArray[k] )
                        {
                            sSerialMax = mSerialArray[k];
                            sSlotIDMax = k;
                        }
                    }

                    IDE_DASSERT(sSlotIDMax != -1);
                    if ( sSlotIDMax != -1 )
                    {
                        mIsCycle[sSlotIDMax] = ID_TRUE;
                        sDetectAgain = ID_FALSE;
                        sLastDetected = sSlotIDMax;
                    }

                    /* stop detect and loop again */
                    break;
                }
                else
                {
                    sDetectAgain = ID_TRUE;
                    mIsCycle[i] = ID_FALSE;
                }
            }
            else
            {
                mIsCycle[i] = ID_FALSE;
            }
        } /* for i */
    } /* thread while */

    return NULL;
}

void smlLockMgr::registLockWaitSpin( SInt aSlot, SInt aWaitSlot )
{
    SInt sIndex;

    /* simple deadlock verify */
    sIndex = mIndicesMatrix[aWaitSlot][aSlot];
    IDE_TEST_RAISE(sIndex != -1, err_deadlock);

    sIndex = mIndicesMatrix[aSlot][aWaitSlot];
    if ( sIndex == -1 )
    {
        sIndex = acpAtomicInc32( &(mPendingCount[aWaitSlot]) ) - 1;
        mPendingMatrix[aWaitSlot][sIndex] = aSlot;
    }
    else
    {
        /* fall through */
    }

    mIndicesMatrix[aSlot][aWaitSlot] = sIndex;

    return;

    IDE_EXCEPTION(err_deadlock);
    {
        mIsCycle[aSlot] = ID_TRUE;
    }

    IDE_EXCEPTION_END;
}

idBool smlLockMgr::didLockReleasedSpin( SInt aSlot, SInt aWaitSlot )
{
    SInt sIndex;

    IDE_DASSERT(aSlot != aWaitSlot);
    sIndex = mIndicesMatrix[aSlot][aWaitSlot];
    return (mPendingMatrix[aWaitSlot][sIndex] == aSlot)? ID_FALSE : ID_TRUE;
}

IDE_RC smlLockMgr::freeAllRecordLockSpin( SInt aSlot )
{
    SInt i;

    for ( i = 0 ; i < mPendingCount[aSlot] ; i++ )
    {
        mPendingMatrix[aSlot][i] = SML_EMPTY;
    }
    (void)acpAtomicSet32(&(mPendingCount[aSlot]), 0);

    return IDE_SUCCESS;
}

void smlLockMgr::setLockModeAndAddLockSlotSpin( SInt             aSlot,
                                                smlLockNode  *   aTxLockNode,
                                                smlLockMode  *   aCurLockMode,
                                                smlLockMode      aLockMode,
                                                idBool           aIsLocked,
                                                smlLockSlot **   aLockSlot,
                                                smlLockMode      aOldMode,
                                                smlLockMode      aNewMode )
{
    if ( aTxLockNode != NULL )
    {
        // Ʈ������� lock slot list��  lock slot  �߰� .
        if ( aIsLocked == ID_TRUE )
        {
            aTxLockNode->mFlag |= mLockModeToMask[aLockMode];
            addLockSlot( &(aTxLockNode->mArrLockSlotList[aLockMode]),
                         aSlot );

            if ( aLockSlot != NULL )
            {
                *aLockSlot = &(aTxLockNode->mArrLockSlotList[aLockMode]);
                (*aLockSlot)->mOldMode = aOldMode;
                (*aLockSlot)->mNewMode = aNewMode;
            }//if aLockSlot

        }//if aIsLocked
        else
        {
            /* nothing to do */
        }

        if ( aCurLockMode != NULL )
        {
            *aCurLockMode = getLockMode( aTxLockNode );
        }
        else
        {
            /* nothing to do */
        }
    }//aTxLockNode != NULL
    else
    {
        /* nothing to do */
    }
}

void smlLockMgr::clearWaitItemColsOfTransSpin( idBool aDoInit, SInt aSlot )
{
    SInt sWaitSlot;
    SInt sWaitIndex;

    PDL_UNUSED_ARG(aDoInit);

    IDE_DASSERT(aSlot < mTransCnt);

    for ( sWaitSlot = 0 ; sWaitSlot < mTransCnt ; sWaitSlot++ )
    {
        sWaitIndex = mIndicesMatrix[aSlot][sWaitSlot];

        if ( sWaitIndex != -1 )
        {
            (void)acpAtomicSet32( &(mPendingMatrix[sWaitSlot][sWaitIndex]), -1 );
            mIndicesMatrix[aSlot][sWaitSlot] = -1;
        }
        else
        {
            /* nothing to do */
        }
    }

    mSerialArray[aSlot] = ID_ULONG(0);
    mIsCycle[aSlot]     = ID_FALSE;
}


void smlLockMgr::calcIndex( const SInt aSlotNo, SInt& aIndex, SInt& aBitIndex )
{
    IDE_DASSERT( aSlotNo < mTransCnt );

    aIndex      = aSlotNo / 64;
    aBitIndex   = aSlotNo % 64;
}

void smlLockMgr::beginPending(SInt aSlot)
{
    mSerialArray[aSlot] = acpAtomicInc64( &mPendSerial );
}
