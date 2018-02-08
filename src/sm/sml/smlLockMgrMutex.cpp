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
 * $Id: smlLockMgrMutex.cpp 82075 2018-01-17 06:39:52Z jina.kim $
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
#include <smlLockMgr.h>
#include <smlReq.h>
#include <smu.h>
#include <smrLogHeadI.h>
#include <sct.h>
#include <smxTrans.h>

/*********************************************************
  function description: decTblLockModeAndTryUpdate
  table lock ������ aLockItem����
  Lock node�� lock mode�� �ش��ϴ� lock mode�� ������ ���̰�,
  ���� 0�� �ȴٸ�, table�� ��ǥ���� �����Ѵ�.
***********************************************************/
void smlLockMgr::decTblLockModeAndTryUpdate( smlLockItemMutex*   aLockItem,
                                             smlLockMode         aLockMode )
{
    --(aLockItem->mArrLockCount[aLockMode]);
    IDE_ASSERT(aLockItem->mArrLockCount[aLockMode] >= 0);
    // table�� ��ǥ ���� �����Ѵ�.
    if ( aLockItem->mArrLockCount[aLockMode] ==  0 )
    {
        aLockItem->mFlag &= ~(mLockModeToMask[aLockMode]);
    }
}

/*********************************************************
  function description: incTblLockModeUpdate
  table lock ������ aLockItem����
  Lock node�� lock mode�� �ش��ϴ� lock mode�� ������ ���̰�,
   table�� ��ǥ���� �����Ѵ�.
***********************************************************/
void smlLockMgr::incTblLockModeAndUpdate( smlLockItemMutex*  aLockItem,
                                          smlLockMode        aLockMode )
{
    ++(aLockItem->mArrLockCount[aLockMode]);
    IDE_ASSERT(aLockItem->mArrLockCount[aLockMode] >= 0);
    aLockItem->mFlag |= mLockModeToMask[aLockMode];
}


void smlLockMgr::addLockNodeToHead( smlLockNode *&aFstLockNode, 
                                    smlLockNode *&aLstLockNode, 
                                    smlLockNode *&aNewLockNode )
{
    if ( aFstLockNode != NULL ) 
    { 
        aFstLockNode->mPrvLockNode = aNewLockNode; 
    } 
    else 
    { 
        aLstLockNode = aNewLockNode; 
    } 
    aNewLockNode->mPrvLockNode = NULL; 
    aNewLockNode->mNxtLockNode = aFstLockNode; 
    aFstLockNode = aNewLockNode;
    aNewLockNode->mDoRemove= ID_FALSE;
}

void smlLockMgr::addLockNodeToTail( smlLockNode *&aFstLockNode, 
                                    smlLockNode *&aLstLockNode, 
                                    smlLockNode *&aNewLockNode )
{
    if ( aLstLockNode != NULL ) 
    { 
        aLstLockNode->mNxtLockNode = aNewLockNode; 
    } 
    else 
    { 
        aFstLockNode = aNewLockNode; 
    } 
    aNewLockNode->mPrvLockNode = aLstLockNode;
    aNewLockNode->mNxtLockNode = NULL; 
    aLstLockNode = aNewLockNode;
    aNewLockNode->mDoRemove= ID_FALSE;
}

void smlLockMgr::removeLockNode( smlLockNode *&aFstLockNode,
                                 smlLockNode *&aLstLockNode,
                                 smlLockNode *&aLockNode ) 
{
    if ( aLockNode == aFstLockNode ) 
    { 
        aFstLockNode = aLockNode->mNxtLockNode; 
    } 
    else 
    { 
        aLockNode->mPrvLockNode->mNxtLockNode = aLockNode->mNxtLockNode; 
    } 
    if ( aLockNode == aLstLockNode ) 
    { 
        aLstLockNode = aLockNode->mPrvLockNode; 
    } 
    else 
    { 
        aLockNode->mNxtLockNode->mPrvLockNode = aLockNode->mPrvLockNode; 
    } 
    aLockNode->mNxtLockNode = NULL; 
    aLockNode->mPrvLockNode = NULL;

    aLockNode->mDoRemove= ID_TRUE;
}

IDE_RC smlLockMgr::initializeMutex( UInt aTransCnt )
{

    SInt i, j;

    IDE_ASSERT( smuProperty::getLockMgrType() == 0 );

    mTransCnt = aTransCnt;

    IDE_ASSERT(mTransCnt > 0);

    //Alloc wait table
    IDE_TEST_RAISE( iduMemMgr::calloc( IDU_MEM_SM_SML,
                                       mTransCnt,
                                       ID_SIZEOF(smlLockMatrixItem *),
                                       (void**)&mWaitForTable ) != IDE_SUCCESS,
                    insufficient_memory );

    for ( i = 0 ; i < mTransCnt ; i++ )
    {
        IDE_TEST_RAISE( iduMemMgr::calloc( IDU_MEM_SM_SML,
                                           mTransCnt,
                                           ID_SIZEOF(smlLockMatrixItem),
                                           (void**)&(mWaitForTable[i] ) ) != IDE_SUCCESS,
                        insufficient_memory );


    }

    for ( i = 0 ; i < mTransCnt ; i++ )
    {
        for ( j = 0 ; j < mTransCnt ; j++ )
        {
            mWaitForTable[i][j].mIndex = 0;
            mWaitForTable[i][j].mNxtWaitTransItem = ID_USHORT_MAX;
            mWaitForTable[i][j].mNxtWaitRecTransItem = ID_USHORT_MAX;
        }
    }

    mLockTableFunc                  = lockTableMutex;
    mUnlockTableFunc                = unlockTableMutex;
    mInitLockItemFunc               = initLockItemMutex;
    mAllocLockItemFunc              = allocLockItemMutex;
    mDidLockReleasedFunc            = didLockReleasedMutex;
    mRegistRecordLockWaitFunc       = registRecordLockWaitMutex;
    mFreeAllRecordLockFunc          = freeAllRecordLockMutex;
    mClearWaitItemColsOfTransFunc   = clearWaitItemColsOfTransMutex;

    return IDE_SUCCESS;

    IDE_EXCEPTION( insufficient_memory );
    {
        IDE_SET(ideSetErrorCode(idERR_ABORT_InsufficientMemory));
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}


IDE_RC smlLockMgr::destroyMutex()
{
    SInt i;

    for ( i = 0 ; i < mTransCnt ; i++ )
    {
        IDE_TEST( iduMemMgr::free(mWaitForTable[i]) != IDE_SUCCESS );
    }

    IDE_TEST( iduMemMgr::free(mWaitForTable) != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smlLockMgr::allocLockItemMutex( void ** aLockItem )
{
    IDE_TEST_RAISE( iduMemMgr::malloc( IDU_MEM_SM_SML,
                                       ID_SIZEOF(smlLockItemMutex),
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

IDE_RC smlLockMgr::initLockItemMutex( scSpaceID       aSpaceID,
                                      ULong           aItemID,
                                      smiLockItemType aLockItemType,
                                      void           *aLockItem )
{
    SInt                sLockType;
    smlLockItemMutex*   sLockItem;

    IDE_TEST( initLockItemCore( aSpaceID, 
                                aItemID,
                                aLockItemType, 
                                aLockItem )
              != IDE_SUCCESS );

    sLockItem = (smlLockItemMutex*) aLockItem;

    sLockItem->mGrantLockMode   = SML_NLOCK;
    sLockItem->mFstLockGrant    = NULL;
    sLockItem->mFstLockRequest  = NULL;
    sLockItem->mLstLockGrant    = NULL;
    sLockItem->mLstLockRequest  = NULL;
    sLockItem->mGrantCnt        = 0;
    sLockItem->mRequestCnt      = 0;
    sLockItem->mFlag            = 0;

    for ( sLockType = 0 ; sLockType < SML_NUMLOCKTYPES ; sLockType++ )
    {
        sLockItem->mArrLockCount[sLockType] = 0;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/*********************************************************
  function description: lockTable
  table�� lock�� �ɶ� ������ ���� case�� ���� ���� ó���Ѵ�.

  1. ������ Ʈ�������  table A��  lock�� ��Ұ�,
    ������ ���� lock mode�� ���� ����� �ϴ� ����� ��ȯ�����
    ������ �ٷ� return.

  2. table�� grant lock mode�� ���� ����� �ϴ� ���� ȣȯ�����Ѱ��.

     2.1  table �� lock waiting�ϴ� Ʈ������� �ϳ��� ����,
          ������ Ʈ������� table�� ���Ͽ� lock�� ���� �ʴ� ���.

       -  lock node�� �Ҵ��ϰ� �ʱ�ȭ�Ѵ�.
       -  table lock�� grant list tail�� add�Ѵ�.
       -  lock node�� grant�Ǿ����� �����ϰ�,
          table lock�� grant count�� 1���� ��Ų��.
       -  Ʈ������� lock list��  lock node��  add�Ѵ�.

     2.2  table �� lock waiting�ϴ� Ʈ����ǵ��� ������,
          ������ Ʈ�������  table�� ���Ͽ� lock�� ���� ���.
         -  table���� grant�� lock mode array����
            Ʈ������� lock node��   lock mode�� 1����.
            table �� ��ǥ�� ���Žõ�.

     2.1, 2.2�� ����������  ������ �����ϰ� 3��° �ܰ�� �Ѿ.
     - ���� ��û�� lock mode�� ����  lock mode��
      conversion�Ͽ�, lock node�� lock mode�� ����.
     - lock node�� ���ŵ�  lock mode�� �̿��Ͽ�
      table���� grant�� lock mode array����
      ���ο� lockmode �� �ش��ϴ� lock mode�� 1�����ϰ�
      table�� ��ǥ���� ����.
     - table lock�� grant mode�� ����.
     - grant lock node�� lock���� ����.

     2.3  ������ Ʈ������� table�� ���Ͽ� lock�� ���� �ʾҰ�,
          table�� request list�� ��� ���� �ʴ� ���.
         - lock�� grant���� �ʰ� 3.2���� ���� lock���ó����
           �Ѵ�.
           % ���� ���� ������ �̷��� ó���Ѵ�.

  3. table�� grant lock mode�� ���� ����� �ϴ� ���� ȣȯ�Ұ����� ���.
     3.1 lock conflict������,   grant�� lock node�� 1���̰�,
         �װ��� �ٷ� ��  Ʈ������� ���.
      - request list�� ���� �ʰ�
        ���� grant��  lock node�� lock mode�� table�� ��ǥ��,
        grant lock mode��   �����Ѵ�.

     3.2 lock conflict �̰� lock ��� �ð��� 0�� �ƴ� ���.
        -  request list�� �� lock node�� �����ϰ�
         �Ʒ� ���� case�� ���Ͽ�, �б�.
        3.2.1 Ʈ������� ������ table�� ���Ͽ� grant��
              lock node�� ������ �ִ� ���.
           -  request list�� ����� ������ lock node�� �߰�.
           -  request list�� �Ŵ� lock node�� cvs node��
               grant�� lock node�� �ּҷ� assgin.
            -> ���߿� �ٸ� Ʈ����ǿ���  unlock table�ÿ�
                request list�� �ִ� lock node��
               ���ŵ� table grant mode�� ȣȯ�ɶ�,
               grant list�� ������ insert���� �ʰ� ,
               ���� cvs lock node�� lock mode�� ���Ž���
               grant list ������ ���̷��� �ǵ���.

        3.2.2 ������ grant�� lock node�� ���� ���.
            -  table lock request list���� �ִ� Ʈ����ǵ���
               slot id����  waitingTable���� lock�� �䱸��
               Ʈ������� slot id �࿡ list�� �����Ѵ�
               %��α��̴� 1��
            - request list�� tail�� ������ lock node�� �߰�.

        3.2.1, 3.2.2 �������� ������ �������� ������ �����Ѵ�.

        - waiting table����   Ʈ������� slot id �࿡
        grant list����     �ִ�Ʈ����ǵ� ��
        slot id��  table lock waiting ����Ʈ�� ����Ѵ�.
        - dead lock �� �˻��Ѵ�(isCycle)
         -> smxTransMgr::waitForLock���� �˻��ϸ�,
          dead lock�� �߻��ϸ� Ʈ����� abort.
        - waiting�ϰ� �ȴ�(suspend).
         ->smxTransMgr::waitForLock���� �����Ѵ�.
        -  �ٸ� Ʈ������� commit/abort�Ǹ鼭 wakeup�Ǹ�,
          waiting table���� �ڽ��� Ʈ����� slot id��
          �����ϴ� �࿡��  ��� �÷����� clear�Ѵ�.
        -  Ʈ������� lock list�� lock node�� �߰��Ѵ�.
          : lock�� ��� �Ǿ��⶧����.

     3.3 lock conflict �̰� lock��� �ð��� 0�� ���.
        : lock flag�� ID_FALSE�� ����.

  4. Ʈ������� lock slot list��  lock slot  �߰� .


 BUG-28752 lock table ... in row share mode ������ ������ �ʽ��ϴ�.

 implicit/explicit lock�� �����Ͽ� �̴ϴ�.
 implicit is lock�� statement end�� Ǯ���ֱ� �����Դϴ�. 

 �� ��� Upgrding Lock�� ���� ������� �ʾƵ� �˴ϴ�.
  ��, Implicit IS Lock�� �ɸ� ���¿��� Explicit IS Lock�� �ɷ� �ϰų�
 Explicit IS Lock�� �ɸ� ���¿��� Implicit IS Lock�� �ɸ� ��쿡 ����
 ������� �ʾƵ� �˴ϴ�.
  �ֳ��ϸ� Imp�� ���� �ɷ����� ���, ������ Statement End�Ǵ� ��� Ǯ
 ������ ������ ���� Explicit Lock�� �Ŵµ� ���� ����, Exp�� ���� �ɷ�
 ���� ���, ������ IS Lock�� �ɷ� �ִ� �����̱� ������ Imp Lock�� ��
 �� �ʿ�� ���� �����Դϴ�.
***********************************************************/
IDE_RC smlLockMgr::lockTableMutex( SInt          aSlot,
                                   smlLockItem  *aLockItem,
                                   smlLockMode   aLockMode,
                                   ULong         aLockWaitMicroSec,
                                   smlLockMode   *aCurLockMode,
                                   idBool       *aLocked,
                                   smlLockNode **aLockNode,
                                   smlLockSlot **aLockSlot,
                                   idBool        aIsExplicit )
{
    smlLockItemMutex*   sLockItem = (smlLockItemMutex*)aLockItem;
    smlLockNode*        sCurTransLockNode = NULL;
    smlLockNode*        sNewTransLockNode = NULL;
    UInt                sState            = 0;
    idBool              sLocked           = ID_TRUE;
    idvSQL*             sStat;
    idvSession*         sSession;
    UInt                sLockEnable = 1;


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

    sSession = (sStat == NULL ? NULL : sStat->mSess);

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
        if ( (aLockMode != SML_ISLOCK) && (aLockMode !=SML_IXLOCK) )
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

        if ( aLocked != NULL )
        {
            *aLocked = ID_TRUE;
        }

        if ( aLockSlot != NULL )
        {
            *aLockSlot = NULL;
        }

        if ( aCurLockMode != NULL )
        {
            *aCurLockMode = aLockMode;
        }

        return IDE_SUCCESS;
    }

    if ( aLocked != NULL )
    {
        *aLocked = ID_TRUE;
    }

    if ( aLockSlot != NULL )
    {
        *aLockSlot = NULL;
    }

    // Ʈ������� ���� statement�� ���Ͽ�,
    // ���� table A�� ���Ͽ� lock�� ��Ҵ� lock node�� ã�´�.
    sCurTransLockNode = findLockNode(sLockItem,aSlot);
    // case 1: ������ Ʈ�������  table A��  lock�� ��Ұ�,
    // ������ ���� lock mode�� ���� ����� �ϴ� ����� ��ȯ�����
    // ������ �ٷ� return!
    if ( sCurTransLockNode != NULL )
    {
        if ( mConversionTBL[sCurTransLockNode->mLockMode][aLockMode]
             == sCurTransLockNode->mLockMode )
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
                    *aCurLockMode = sCurTransLockNode->mLockMode;
                }

                IDV_SESS_ADD( sSession,
                              IDV_STAT_INDEX_LOCK_ACQUIRED, 
                              1 );

                /* We don't need  this code since delayed unlock thread checks 
                 * if it's ok to unlock every time.
                 * I leave this code just for sure. 
                 *
                 *   if( aLockMode == SML_ISLOCK )
                 *   {
                 *       mDelayedUnlock.clearBitmap(aSlot); 
                 *   }
                 */

                return IDE_SUCCESS;
            }
            else
            {
                /* nothing to do */
            }
        }
        else
        {
            /* nothing to do */
        }
    }
    else
    {
        /* nothing to do */
    }

    IDE_TEST_RAISE( sLockItem->mMutex.lock( NULL /* idvSQL* */)
                    != IDE_SUCCESS, err_mutex_lock );

    sState = 1;

    //---------------------------------------
    // table�� ��ǥ���� ���� ����� �ϴ� ���� ȣȯ�����ϴ��� Ȯ��.
    //---------------------------------------
    if ( mCompatibleTBL[sLockItem->mGrantLockMode][aLockMode] == ID_TRUE )
    {
        if ( sCurTransLockNode != NULL ) /* ���� statement ����� lock�� ���� */
        {   
            decTblLockModeAndTryUpdate( sLockItem,
                                        sCurTransLockNode->mLockMode );
        }
        else                      /* ���� statement ���� �� lock�� ���� ���� */
        {
            if ( sLockItem->mRequestCnt != 0 ) /* ��ٸ��� lock�� �ִ� ��� */
            {
                /* BUGBUG: BUG-16471, BUG-17522
                 * ��ٸ��� lock�� �ִ� ��� ������ lock conflict ó���Ѵ�. */
                IDE_CONT(lock_conflict);
            }
            else
            {
                /* ��ٸ��� lock�� ���� ��� Lock�� grant �� */
            }

            /* allocate lock node and initialize */
            IDE_TEST( allocLockNodeAndInit( aSlot,
                                            aLockMode,
                                            sLockItem,
                                            &sCurTransLockNode,
                                            aIsExplicit )
                      != IDE_SUCCESS );

            /* add node to grant list */
            addLockNodeToTail( sLockItem->mFstLockGrant,
                               sLockItem->mLstLockGrant,
                               sCurTransLockNode );

            sCurTransLockNode->mBeGrant = ID_TRUE;
            sLockItem->mGrantCnt++;
            /* Add Lock Node to a transaction */
            addLockNode( sCurTransLockNode, aSlot );
        }

        /* Lock�� ���ؼ� Conversion�� �����Ѵ�. */
        sCurTransLockNode->mLockMode =
            mConversionTBL[sCurTransLockNode->mLockMode][aLockMode];

        incTblLockModeAndUpdate(sLockItem, sCurTransLockNode->mLockMode);
        sLockItem->mGrantLockMode =
            mConversionTBL[sLockItem->mGrantLockMode][aLockMode];

        sCurTransLockNode->mLockCnt++;
        IDE_CONT(lock_SUCCESS);
    }

    //---------------------------------------
    // Lock Conflict ó��
    //---------------------------------------

    IDE_EXCEPTION_CONT(lock_conflict);

    if ( ( sLockItem->mGrantCnt == 1 ) && ( sCurTransLockNode != NULL ) )
    {
        //---------------------------------------
        // lock conflict������, grant�� lock node�� 1���̰�,
        // �װ��� �ٷ� ��  Ʈ������� ��쿡�� request list�� ���� �ʰ�
        // ���� grant��  lock node�� lock mode�� table�� ��ǥ��,
        // grant lock mode��  �����Ѵ�.
        //---------------------------------------

        decTblLockModeAndTryUpdate( sLockItem,
                                    sCurTransLockNode->mLockMode );

        sCurTransLockNode->mLockMode =
            mConversionTBL[sCurTransLockNode->mLockMode][aLockMode];

        sLockItem->mGrantLockMode =
            mConversionTBL[sLockItem->mGrantLockMode][aLockMode];

        incTblLockModeAndUpdate( sLockItem, sCurTransLockNode->mLockMode );
    }
    else if ( aLockWaitMicroSec != 0 )
    {
        IDE_TEST( allocLockNodeAndInit( aSlot,
                                        aLockMode,
                                        sLockItem,
                                        &sNewTransLockNode,
                                        aIsExplicit ) != IDE_SUCCESS );
        sNewTransLockNode->mBeGrant    = ID_FALSE;
        if ( sCurTransLockNode != NULL )
        {
            sNewTransLockNode->mCvsLockNode = sCurTransLockNode;

            //Lock node�� Lock request ����Ʈ�� ����� �߰��Ѵ�.
            // �ֳ��ϸ� Conversion�̱� �����̴�.
            addLockNodeToHead( sLockItem->mFstLockRequest,
                               sLockItem->mLstLockRequest,
                               sNewTransLockNode );
        }
        else
        {
            sCurTransLockNode = sNewTransLockNode;
            // waiting table����   Ʈ������� slot id �࿡
            // request list����  ����ϰ� �ִ�Ʈ����ǵ� ��
            // slot id��  table lock waiting ����Ʈ�� ����Ѵ�.
            registTblLockWaitListByReq(aSlot,sLockItem->mFstLockRequest);
            //Lock node�� Lock request ����Ʈ�� Tail�� �߰��Ѵ�.
            addLockNodeToTail( sLockItem->mFstLockRequest,
                               sLockItem->mLstLockRequest,
                               sNewTransLockNode );
        }
        // waiting table����   Ʈ������� slot id �࿡
        // grant list����     �ִ�Ʈ����ǵ� ��
        // slot id��  table lock waiting ����Ʈ�� ����Ѵ�.
        registTblLockWaitListByGrant( aSlot,sLockItem->mFstLockGrant );
        sLockItem->mRequestCnt++;

        IDE_TEST_RAISE( smLayerCallback::waitForLock(
                                        smLayerCallback::getTransBySID( aSlot ),
                                        &(sLockItem->mMutex),
                                        aLockWaitMicroSec )
                        != IDE_SUCCESS, err_wait_lock );
        // lock wait�Ŀ� wakeup�Ǿ� lock�� ��Ե�.
        // Add Lock Node to Transaction
        addLockNode( sNewTransLockNode, aSlot );
    }
    else
    {
        sLocked = ID_FALSE;

        if ( aLocked != NULL )
        {
            *aLocked = ID_FALSE;
        }
    }

    IDE_EXCEPTION_CONT(lock_SUCCESS);


    //Ʈ������� Lock node�� �־���,Lock�� ��Ҵٸ�
    // lock slot�� �߰��Ѵ�
    setLockModeAndAddLockSlotMutex( aSlot,
                                    sCurTransLockNode,
                                    aCurLockMode,
                                    aLockMode,
                                    sLocked,
                                    aLockSlot );
    IDE_TEST_RAISE(sLocked == ID_FALSE, err_exceed_wait_time);
    IDE_DASSERT( sCurTransLockNode->mArrLockSlotList[aLockMode].mLockSequence != 0 );

    sState = 0;
    IDE_TEST_RAISE(sLockItem->mMutex.unlock() != IDE_SUCCESS, err_mutex_unlock);

    if ( aLockNode != NULL )
    {
        *aLockNode = sCurTransLockNode;
    }
    else
    {
        /* nothing to do */
    }

    IDV_SESS_ADD(sSession, IDV_STAT_INDEX_LOCK_ACQUIRED, 1);

    return IDE_SUCCESS;

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
    IDE_EXCEPTION(err_mutex_lock);
    {
        IDE_SET(ideSetErrorCode(smERR_FATAL_ThrMutexLock));
    }
    IDE_EXCEPTION(err_mutex_unlock);
    {
        IDE_SET(ideSetErrorCode(smERR_FATAL_ThrMutexUnlock));
    }
    IDE_EXCEPTION(err_wait_lock);
    {
        // fix BUG-10202�ϸ鼭,
        // dead lock , time out�� ƨ�� ���� Ʈ����ǵ���
        // waiting table row�� ���� clear�ϰ�,
        // request list�� transaction�̱���� �Ǵ��� üũ�Ѵ�.

        (void)smlLockMgr::unlockTable( aSlot, 
                                       sNewTransLockNode,
                                       NULL, 
                                       ID_FALSE );
    }
    IDE_EXCEPTION(err_exceed_wait_time);
    {
        IDE_SET(ideSetErrorCode(smERR_ABORT_smcExceedLockTimeWait));
    }
    IDE_EXCEPTION_END;
    //waiting table���� aSlot�� �࿡ ��⿭�� clear�Ѵ�.
    clearWaitItemColsOfTransMutex(ID_TRUE, aSlot);

    if ( sState != 0 )
    {
        IDE_ASSERT( sLockItem->mMutex.unlock() == IDE_SUCCESS );
    }

    return IDE_FAILURE;

}
/*********************************************************
  function description: unlockTable
  �Ʒ��� ���� �پ��� case�� ���� ���� ó���Ѵ�.
  1. lock node�� �̹�table�� grant or wait list����
     �̹� ���� �Ǿ� �ִ� ���.
     - lock node�� Ʈ������� lock list���� �޷�������,
       lock node�� Ʈ������� lock list���� �����Ѵ�.

  2. if (lock node�� grant�� ���� �ϰ��)
     then
      table lock�������� lock node�� lock mode��  1����
      ��Ű�� table ��ǥ�� ���Žõ�.
      aLockNode�� ��ǥ��  &= ~(aLockSlot�� mMask).
      if( aLock node�� ��ǥ��  != 0)
      then
        aLockNode�� lockMode�� �����Ѵ�.
        ���ŵ� lock node�� mLockMode�� table lock�������� 1����.
        lock node���� �϶�� flag�� off��Ų��.
        --> lockTable�� 2.2������ ������ Ʈ������� grant ��
        lock node�� ���Ͽ�, grant lock node�� �߰��ϴ� ��ſ�
        lock mode�� conversion�Ͽ� lock slot��
        add�� ����̴�. �� node�� �ϳ�������, �׾ȿ�
        lock slot�� �ΰ� �̻��� �ִ� ����̴�.
        4��° �ܰ�� �Ѿ��.
      fi
      grant list���� lock node�� ���Ž�Ų��.
      table lock�������� grant count 1 ����.
      ���ο� Grant Lock Mode�� �����Ѵ�.
    else
      // lock node�� request list�� �޷� �ִ� ���.
       request list���� lock node�� ����.
       request count����.
       grant count�� 0�̸� 5���ܰ�� �Ѿ.
    fi
  3. waiting table�� clear�Ѵ�.
     grant lock node�̰ų�, request list�� �־��� lock node
     ��  ������ Free�� ��� �� Transaction�� ��ٸ��� �ִ�
     Transaction��  waiting table���� �����Ͽ� �ش�.
     ������ request�� �־��� lock node�� ���,
     Grant List�� Lock Node��
     ���� �ִ°��� ���� �ʴ´�.


  4. lock�� wait�ϰ� �ִ� Transaction�� �����.
     ���� lock node :=  table lock�������� request list����
                        ��ó�� lock node.
     while( ���� lock node != null)
     begin loop.
      if( ���� table�� grant mode�� ���� lock node�� lock mode�� ȣȯ?)
      then
         request ����Ʈ���� Lock Node�� �����Ѵ�.
         table lock�������� grant lock mode�� �����Ѵ�.
         if(���� lock node�� cvs node�� null�� �ƴϸ�)
         then
            table lock �������� ���� lock node��
            cvs node�� lock mode�� 1���ҽ�Ű��,��ǥ�� ���Žõ�.
            cvs node�� lock mode�� �����Ѵ�.
            table lock �������� cvs node��
            lock mode�� 1���� ��Ű�� ��ǥ���� ����.
         else
            table lock �������� ���� lock node�� lock mode��
            1���� ��Ű�� ��ǥ���� ����.
            Grant����Ʈ�� ���ο� Lock Node�� �߰��Ѵ�.
            ���� lock node�� grant flag�� on ��Ų��.
            grant count�� 1���� ��Ų��.
         fi //���� lock node�� cvs node�� null�� �ƴϸ�
      else
      //���� table�� grant mode�� ���� lock node�� lock mode��
      // ȣȯ���� �ʴ� ���.
        if(table�� grant count == 1)
        then
           if(���� lock node�� cvs node�� �ִ°�?)
           then
             //cvs node�� �ٷ�  grant�� 1�� node�̴�.
              table lock �������� cvs lock node�� lock mode��
              1���� ��Ű��, table ��ǥ�� ���Žõ�.
              table lock �������� grant mode����.
              cvs lock node�� lock mode�� ���� table�� grant mode
              ���� ����.
              table lock �������� ���� grant mode�� 1���� ��Ű��,
              ��ǥ�� ����.
              Request ����Ʈ���� Lock Node�� �����Ѵ�.
           else
              break; // nothint to do
           fi
        else
           break; // nothing to do.
        fi
      fi// lock node�� lock mode�� ȣȯ���� �ʴ� ���.

      table lock�������� request count 1����.
      ���� lock node�� slot id��  waiting�ϰ� �ִ� row����
      waiting table���� clear�Ѵ�.
      waiting�ϰ� �ִ� Ʈ������� resume��Ų��.
     loop end.

  5. lock slot, lock node ���� �õ�.
     - lock slot �� null�� �ƴϸ�,lock slot�� transaction
        �� lock slot list���� �����Ѵ�.
     - 2�� �ܰ迡�� lock node�� �����϶�� flag�� on
     �Ǿ� ������  lock node��  Ʈ������� lock list����
     ���� �ϰ�, lock node�� free�Ѵ�.
***********************************************************/

IDE_RC smlLockMgr::unlockTableMutex( SInt          aSlot,
                                     smlLockNode  *aLockNode,
                                     smlLockSlot  *aLockSlot,
                                     idBool        aDoMutexLock )
{

    smlLockItemMutex*   sLockItem = NULL;
    smlLockNode*        sCurLockNode;
    UInt                sState = 0;
    idBool              sDoFreeLockNode = ID_TRUE;
    idvSQL*             sStat;
    idvSession*         sSession;

    sStat = smLayerCallback::getStatisticsBySID( aSlot );

    sSession = ((sStat == NULL) ? NULL : sStat->mSess);


    if ( aLockNode == NULL )
    {
        IDE_DASSERT( aLockSlot->mLockNode != NULL );

        aLockNode = aLockSlot->mLockNode;
    }

    // lock node�� �̹� request or grant list���� ���ŵ� ����.
    if ( aLockNode->mDoRemove == ID_TRUE )
    {
        IDE_ASSERT( ( aLockNode->mPrvLockNode == NULL ) &&
                    ( aLockNode->mNxtLockNode == NULL ) );
        //Ʈ������� lock list���� ���� ���ŵ��� �ʾҴ���Ȯ��.
        if ( aLockNode->mPrvTransLockNode != NULL )
        {
            // Ʈ����� lock list array����
            // transaction�� slot id�� �ش��ϴ�
            // list���� lock node�� �����Ѵ�.
            removeLockNode(aLockNode);
        }
        else
        {
            /* nothing to do */
        }

        IDE_TEST( freeLockNode(aLockNode) != IDE_SUCCESS );

        IDV_SESS_ADD(sSession, IDV_STAT_INDEX_LOCK_RELEASED, 1);

        return IDE_SUCCESS;
    }
    else
    {
        /* nothing to do */
    }


    sLockItem = (smlLockItemMutex*)aLockNode->mLockItem;

    if ( aDoMutexLock == ID_TRUE )
    {
        IDE_TEST_RAISE( sLockItem->mMutex.lock(NULL /* idvSQL* */) != IDE_SUCCESS,
                        err_mutex_lock );
        sState = 1;
    }
    else
    {
        /* nothing to do */
    }


    while(1)
    {
        if ( aLockNode->mBeGrant == ID_TRUE )
        {
            decTblLockModeAndTryUpdate( sLockItem, aLockNode->mLockMode );
            while ( 1 )
            {
                if ( aLockSlot != NULL )
                {
                    aLockNode->mFlag  &= ~(aLockSlot->mMask);
                    // lockTable�� 2.2������ ������ Ʈ������� grant ��
                    // lock node�� ���Ͽ�, grant lock node�� �߰��ϴ� ��ſ�
                    // lock mode�� conversion�Ͽ� lock slot��
                    // add�� ����̴�. �� node�� �ϳ�������, �׾ȿ�
                    // lock slot�� �ΰ� �̻��� �ִ� ����̴�.
                    // -> ���� lock node�� �����ϸ� �ȵȴ�.
                    //   sDoFreeLockNode = ID_FALSE;
                    if ( aLockNode->mFlag != 0 )
                    {
                        aLockNode->mLockMode = mDecisionTBL[aLockNode->mFlag];
                        incTblLockModeAndUpdate( sLockItem,
                                                 aLockNode->mLockMode );
                        sDoFreeLockNode = ID_FALSE;
                        break;
                    }//if aLockNode
                } // if aLockSlot != NULL
                //Remove lock node from lock Grant list
                removeLockNode( sLockItem->mFstLockGrant,
                                sLockItem->mLstLockGrant,
                                aLockNode );
                sLockItem->mGrantCnt--;
                break;
            } // inner while loop .
            //���ο� Grant Lock Mode�� �����Ѵ�.
            sLockItem->mGrantLockMode = mDecisionTBL[sLockItem->mFlag];
        }//if aLockNode->mBeGrant == ID_TRUE
        else
        {
            // Grant �Ǿ����� �ʴ� ����.
            //remove lock node from lock request list
            removeLockNode( sLockItem->mFstLockRequest,
                            sLockItem->mLstLockRequest, 
                            aLockNode );
            sLockItem->mRequestCnt--;

        }//else aLockNode->mBeGrant == ID_TRUE.

        if ( ( sDoFreeLockNode == ID_TRUE ) && ( aLockNode->mCvsLockNode == NULL ) )
        {
            // grant lock node�̰ų�, request list�� �־��� lock node
            //��  ������ Free�� ��� �� Transaction�� ��ٸ��� �ִ�
            //Transaction��  waiting table���� �����Ͽ� �ش�.
            //������ request�� �־��� lock node�� ���,
            // Grant List�� Lock Node��
            //���� �ִ°��� ���� �ʴ´�.
            clearWaitTableRows( sLockItem->mFstLockRequest,
                                aSlot );
        }

        //lock�� ��ٸ��� Transaction�� �����.
        sCurLockNode = sLockItem->mFstLockRequest;
        while ( sCurLockNode != NULL )
        {
            //Wake up requestors
            //���� table�� grant mode�� ���� lock node�� lock mode�� ȣȯ?
            if ( mCompatibleTBL[sCurLockNode->mLockMode][sLockItem->mGrantLockMode] == ID_TRUE )
            {
                //Request ����Ʈ���� Lock Node�� �����Ѵ�.
                removeLockNode( sLockItem->mFstLockRequest,
                                sLockItem->mLstLockRequest,
                                sCurLockNode );

                sLockItem->mGrantLockMode =
                    mConversionTBL[sLockItem->mGrantLockMode][sCurLockNode->mLockMode];
                if ( sCurLockNode->mCvsLockNode != NULL )
                {
                    //table lock �������� ���� lock node��
                    //cvs node��  lock mode�� 1���ҽ�Ű��,��ǥ�� ���Žõ�.
                    decTblLockModeAndTryUpdate( sLockItem,
                                                sCurLockNode->mCvsLockNode->mLockMode );
                    //cvs node�� lock mode�� �����Ѵ�.
                    sCurLockNode->mCvsLockNode->mLockMode =
                        mConversionTBL[sCurLockNode->mCvsLockNode->mLockMode][sCurLockNode->mLockMode];
                    //table lock �������� cvs node��
                    //lock mode�� 1���� ��Ű�� ��ǥ���� ����.
                    incTblLockModeAndUpdate( sLockItem,
                                             sCurLockNode->mCvsLockNode->mLockMode );
                }// if sCurLockNode->mCvsLockNode != NULL
                else
                {
                    incTblLockModeAndUpdate( sLockItem, sCurLockNode->mLockMode );
                    //Grant����Ʈ�� ���ο� Lock Node�� �߰��Ѵ�.
                    addLockNodeToTail( sLockItem->mFstLockGrant,
                                       sLockItem->mLstLockGrant,
                                       sCurLockNode );
                    sCurLockNode->mBeGrant = ID_TRUE;
                    sLockItem->mGrantCnt++;
                }//else sCurLockNode->mCvsLockNode�� NULL
            }
            // ���� table�� grant lock mode�� lock node�� lockmode
            // �� ȣȯ���� �ʴ� ���.
            else
            {
                if ( sLockItem->mGrantCnt == 1 )
                {
                    if ( sCurLockNode->mCvsLockNode != NULL )
                    {
                        // cvs node�� �ٷ� grant�� 1�� node�̴�.
                        //���� lock�� ���� table�� ���� lock�� Converion �̴�.
                        decTblLockModeAndTryUpdate( sLockItem,
                                                    sCurLockNode->mCvsLockNode->mLockMode );

                        sLockItem->mGrantLockMode =
                            mConversionTBL[sLockItem->mGrantLockMode][sCurLockNode->mLockMode];
                        sCurLockNode->mCvsLockNode->mLockMode = sLockItem->mGrantLockMode;
                        incTblLockModeAndUpdate( sLockItem, sLockItem->mGrantLockMode );
                        //Request ����Ʈ���� Lock Node�� �����Ѵ�.
                        removeLockNode( sLockItem->mFstLockRequest,
                                        sLockItem->mLstLockRequest, 
                                        sCurLockNode );
                    }
                    else
                    {
                        break;
                    } // sCurLockNode->mCvsLockNode�� null
                }//sLockItem->mGrantCnt == 1
                else
                {
                    break;
                }//sLockItem->mGrantCnt != 1
            }//mCompatibleTBL

            sLockItem->mRequestCnt--;
            //waiting table���� aSlot�� �࿡��  ��⿭�� clear�Ѵ�.
            clearWaitItemColsOfTransMutex( ID_FALSE, sCurLockNode->mSlotID );
            // waiting�ϰ� �ִ� Ʈ������� resume��Ų��.
            IDE_TEST( smLayerCallback::resumeTrans( smLayerCallback::getTransBySID( sCurLockNode->mSlotID ) )
                      != IDE_SUCCESS );
            sCurLockNode = sLockItem->mFstLockRequest;
        }/* while */

        break;
    }/* while */

    if ( aLockSlot != NULL )
    {
        removeLockSlot(aLockSlot);
    }
    else
    {
        /* nothing to do */
    }

    if ( sDoFreeLockNode == ID_TRUE )
    {
        if ( aLockNode->mPrvTransLockNode != NULL )
        {
            // Ʈ����� lock list array����
            // transaction�� slot id�� �ش��ϴ�
            // list���� lock node�� �����Ѵ�.
            removeLockNode(aLockNode);
        }
        else
        {
            /* nothing to do */
        }
    }
    else
    {
        /* nothing to do */
    }

    if ( aDoMutexLock == ID_TRUE )
    {
        sState = 0;
        IDE_TEST_RAISE( sLockItem->mMutex.unlock() != IDE_SUCCESS, err_mutex_unlock );
    }
    else
    {
        /* nothing to do */
    }

    if ( sDoFreeLockNode == ID_TRUE )
    {
        IDE_TEST( freeLockNode(aLockNode) != IDE_SUCCESS );
    }
    else
    {
        /* nothing to do */
    }

    IDV_SESS_ADD(sSession, IDV_STAT_INDEX_LOCK_RELEASED, 1);

    return IDE_SUCCESS;

    IDE_EXCEPTION(err_mutex_lock);
    {
        IDE_SET(ideSetErrorCode(smERR_FATAL_ThrMutexLock));
    }
    IDE_EXCEPTION(err_mutex_unlock);
    {
        IDE_SET(ideSetErrorCode(smERR_FATAL_ThrMutexUnlock));
    }
    IDE_EXCEPTION_END;

    if ( ( sState != 0 ) && ( aDoMutexLock == ID_TRUE ) )
    {
        IDE_ASSERT( sLockItem->mMutex.unlock() == IDE_SUCCESS );
    }

    return IDE_FAILURE;

}

/*********************************************************
  function description: detectDeadlockMutex
  SIGMOD RECORD, Vol.17, No,2 ,June,1988
  Bin Jang,
  Dead Lock Detection is Really Cheap paper��
  7 page����� ����.
  �� ����� �ٸ�����,
  waiting table�� chained matrix�̾ �˷θ�����
  ���⵵�� ���߾���,
  ������ loop���� �Ʒ��� ���� ������ �־�
  �ڱ��ڽŰ���ΰ� �߻����ڸ��� ��ü loop�� ���������ٴ�
  ���̴�.
  if(aSlot == j)
  {
   return ID_TRUE;
  }

***********************************************************/
idBool smlLockMgr::detectDeadlockMutex( SInt aSlot )
{
    UShort     i, j;
    // node�� node���� ����� ����.
    UShort sPathLen;
    // node���� ��� ��ΰ� �ִ°� ������ flag
    idBool     sExitPathFlag;
    void       *sTargetTrans;
    SInt       sSlotN;

    sExitPathFlag = ID_TRUE;
    sPathLen= 1;
    IDL_MEM_BARRIER;

    while ( ( sExitPathFlag == ID_TRUE ) &&
            ( mWaitForTable[aSlot][aSlot].mIndex == 0 ) &&
            ( sPathLen < mTransCnt ) )
    {
        sExitPathFlag      = ID_FALSE;

        for ( i  =  mArrOfLockList[aSlot].mFstWaitTblTransItem;
              ( i != SML_END_ITEM ) &&
              ( mWaitForTable[aSlot][aSlot].mNxtWaitTransItem == ID_USHORT_MAX );
              i = mWaitForTable[aSlot][i].mNxtWaitTransItem )
        {
            IDE_ASSERT(i < mTransCnt);

            if ( mWaitForTable[aSlot][i].mIndex == sPathLen )
            {
                sTargetTrans = smLayerCallback::getTransBySID( i );
                sSlotN = smLayerCallback::getTransSlot( sTargetTrans );

                for ( j  = mArrOfLockList[sSlotN].mFstWaitTblTransItem;
                      j < SML_END_ITEM;
                      j  = mWaitForTable[i][j].mNxtWaitTransItem )
                {
                    if ( (mWaitForTable[i][j].mIndex == 1)
                         && (mWaitForTable[aSlot][j].mNxtWaitTransItem == ID_USHORT_MAX) )
                    {
                        IDE_ASSERT(j < mTransCnt);
                        mWaitForTable[aSlot][j].mIndex = sPathLen + 1;
                        mWaitForTable[aSlot][j].mNxtWaitTransItem
                            = mArrOfLockList[aSlot].mFstWaitTblTransItem;

                        mArrOfLockList[aSlot].mFstWaitTblTransItem = j;
                        // aSlot == j�̸�,
                        // dead lock �߻� �̴�..
                        // �ֳ��ϸ�
                        // mWaitForTable[aSlot][aSlot].mIndex !=0
                        // ���°� �Ǿ��� �����̴�.
                        // dead lock dection���� ����,
                        // ������ loop ����. ������ ���� Ʋ����.
                        if ( aSlot == j )
                        {
                            return ID_TRUE;
                        }

                        sExitPathFlag = ID_TRUE;
                    }
                }//For
            }//if
        }//For
        sPathLen++;
    }//While

    return (mWaitForTable[aSlot][aSlot].mIndex == 0) ? ID_FALSE: ID_TRUE;
}

/*********************************************************
  function description:

  aSlot�� �ش��ϴ� Ʈ������� ����ϰ� �־���
  Ʈ����ǵ��� ����� ������ 0���� clear�Ѵ�.
  -> waitTable���� aSlot�࿡ ����ϰ� �ִ� �÷��鿡��
    ��α��̸� 0�� �Ѵ�.

  aDoInit���� ID_TRUE�̸� ��⿬�� ������ ���������.
***********************************************************/
void smlLockMgr::clearWaitItemColsOfTransMutex( idBool aDoInit, SInt aSlot )
{

    UInt    sCurTargetSlot;
    UInt    sNxtTargetSlot;

    sCurTargetSlot = mArrOfLockList[aSlot].mFstWaitTblTransItem;

    while ( sCurTargetSlot != SML_END_ITEM )
    {
        mWaitForTable[aSlot][sCurTargetSlot].mIndex = 0;
        sNxtTargetSlot = mWaitForTable[aSlot][sCurTargetSlot].mNxtWaitTransItem;

        if ( aDoInit == ID_TRUE )
        {
            mWaitForTable[aSlot][sCurTargetSlot].mNxtWaitTransItem = ID_USHORT_MAX;
        }

        sCurTargetSlot = sNxtTargetSlot;
    }

    if ( aDoInit == ID_TRUE )
    {
        mArrOfLockList[aSlot].mFstWaitTblTransItem = SML_END_ITEM;
    }
}

/*********************************************************
  function description:
  : Ʈ����� a(aSlot)�� ���Ͽ� waiting�ϰ� �־��� Ʈ����ǵ���
    ��� ��� ������ clear�Ѵ�.
***********************************************************/
void  smlLockMgr::clearWaitTableRows( smlLockNode * aLockNode,
                                      SInt          aSlot )
{

    UShort sTransSlot;

    while ( aLockNode != NULL )
    {
        sTransSlot = aLockNode->mSlotID;
        mWaitForTable[sTransSlot][aSlot].mIndex = 0;
        aLockNode = aLockNode->mNxtLockNode;
    }
}

/*********************************************************
  function description: registRecordLockWait
  ������ ���� ������  record�� ����
  Ʈ����ǰ��� waiting ���踦 �����Ѵ�.

  1. waiting table���� aSlot �� aWaitSlot�� waiting�ϰ�
     �ֽ��� ǥ���Ѵ�.
  2.  aWaitSlot column��  aSlot��
     record lock list�� �����Ų��.
  3.  aSlot�࿡�� aWaitSlot��  transaction waiting
     list�� �����Ų��.
***********************************************************/
void   smlLockMgr::registRecordLockWaitMutex( SInt aSlot, SInt aWaitSlot )
{
    SInt sLstSlot;

    IDE_ASSERT( mWaitForTable[aSlot][aWaitSlot].mIndex == 0 );
    IDE_ASSERT( mArrOfLockList[aSlot].mFstWaitTblTransItem == SML_END_ITEM );

    /* BUG-24416
     * smlLockMgr::registRecordLockWait() ���� �ݵ��
     * mWaitForTable[aSlot][aWaitSlot].mIndex �� 1�� �����Ǿ�� �մϴ�. */
    mWaitForTable[aSlot][aWaitSlot].mIndex = 1;

    if ( mWaitForTable[aSlot][aWaitSlot].mNxtWaitRecTransItem
         == ID_USHORT_MAX )
    {
        /* BUG-23823: Record Lock�� ���� ��� ����Ʈ�� Waiting������� ����
         * �־�� �Ѵ�.
         *
         * Wait�� Transaction�� Target Wait Transaction List�� �������� ����
         * �Ѵ�. */
        mWaitForTable[aSlot][aWaitSlot].mNxtWaitRecTransItem = SML_END_ITEM;

        if ( mArrOfLockList[aWaitSlot].mFstWaitRecTransItem == SML_END_ITEM )
        {
            IDE_ASSERT( mArrOfLockList[aWaitSlot].mLstWaitRecTransItem ==
                        SML_END_ITEM );

            mArrOfLockList[aWaitSlot].mFstWaitRecTransItem = aSlot;
            mArrOfLockList[aWaitSlot].mLstWaitRecTransItem = aSlot;
        }
        else
        {
            IDE_ASSERT( mArrOfLockList[aWaitSlot].mLstWaitRecTransItem !=
                        SML_END_ITEM );

            sLstSlot = mArrOfLockList[aWaitSlot].mLstWaitRecTransItem;

            mWaitForTable[sLstSlot][aWaitSlot].mNxtWaitRecTransItem = aSlot;
            mArrOfLockList[aWaitSlot].mLstWaitRecTransItem          = aSlot;
        }
    }

    IDE_ASSERT( mArrOfLockList[aSlot].mFstWaitTblTransItem == SML_END_ITEM );

    mWaitForTable[aSlot][aWaitSlot].mNxtWaitTransItem
        = mArrOfLockList[aSlot].mFstWaitTblTransItem;
    mArrOfLockList[aSlot].mFstWaitTblTransItem = aWaitSlot;
}

/*********************************************************
  function description: didLockReleased
  Ʈ����ǿ� �����ϴ�  aSlot��  aWaitSlot Ʈ����ǿ���
  record lock wait�Ǵ� table lock wait�� �ǹ� clear�Ǿ�
  �ִ��� Ȯ���Ѵ�.
  *********************************************************/
idBool  smlLockMgr::didLockReleasedMutex( SInt aSlot, SInt aWaitSlot )
{

    return (mWaitForTable[aSlot][aWaitSlot].mIndex == 0) ?  ID_TRUE: ID_FALSE;
}

/*********************************************************
  function description: freeAllRecordLock
  aSlot�� �ش��ϴ� Ʈ������� record lock���� waiting�ϰ�
  �ִ� Ʈ����ǵ鰣�� ��� ��θ� 0���� clear�Ѵ�.
  record lock�� waiting�ϰ� �ִ� Ʈ������� resume��Ų��.
***********************************************************/
IDE_RC  smlLockMgr::freeAllRecordLockMutex( SInt aSlot )
{

    UShort i;
    UShort  sNxtItem;
    void * sTrans;


    i = mArrOfLockList[aSlot].mFstWaitRecTransItem;

    while ( i != SML_END_ITEM )
    {
        IDE_ASSERT( i != ID_USHORT_MAX );

        sNxtItem = mWaitForTable[i][aSlot].mNxtWaitRecTransItem;

        mWaitForTable[i][aSlot].mNxtWaitRecTransItem = ID_USHORT_MAX;

        if ( mWaitForTable[i][aSlot].mIndex == 1 )
        {
            sTrans = smLayerCallback::getTransBySID( i );
            IDE_TEST( smLayerCallback::resumeTrans( sTrans ) != IDE_SUCCESS );
        }

        mWaitForTable[i][aSlot].mIndex = 0;

        i = sNxtItem;
    }

    /* PROJ-1381 FAC
     * Record Lock�� ������ ���� �ʱ�ȭ�� �Ѵ�. */
    mArrOfLockList[aSlot].mFstWaitRecTransItem = SML_END_ITEM;
    mArrOfLockList[aSlot].mLstWaitRecTransItem = SML_END_ITEM;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/*********************************************************
  function description: registTblLockWaitListByReq
  waiting table����   Ʈ������� slot id ���� ���鿡
  request list����  ����ϰ� �ִ�Ʈ����ǵ� ��
  slot id��  table lock waiting ����Ʈ�� ����Ѵ�.
***********************************************************/
void smlLockMgr::registTblLockWaitListByReq( SInt          aSlot,
                                             smlLockNode * aLockNode )
{

    UShort       sTransSlot;

    while ( aLockNode != NULL )
    {
        sTransSlot = aLockNode->mSlotID;
        if ( aSlot != sTransSlot )
        {
            IDE_ASSERT(mWaitForTable[aSlot][sTransSlot].mNxtWaitTransItem == ID_USHORT_MAX);

            mWaitForTable[aSlot][sTransSlot].mIndex = 1;
            mWaitForTable[aSlot][sTransSlot].mNxtWaitTransItem
                = mArrOfLockList[aSlot].mFstWaitTblTransItem;
            mArrOfLockList[aSlot].mFstWaitTblTransItem = sTransSlot;
        }
        aLockNode = aLockNode->mNxtLockNode;
    }
}

/*********************************************************
  function description: registTblLockWaitListByGrant
  waiting table����   Ʈ������� slot id �࿡
  grant list����     �ִ�Ʈ����ǵ� ��
  slot id��  table lock waiting ����Ʈ�� ����Ѵ�.
***********************************************************/
void smlLockMgr::registTblLockWaitListByGrant( SInt          aSlot,
                                               smlLockNode * aLockNode )
{

    UShort       sTransSlot;

    while ( aLockNode != NULL )
    {
        sTransSlot = aLockNode->mSlotID;
        if ( (aSlot != sTransSlot) &&
             (mWaitForTable[aSlot][sTransSlot].mNxtWaitTransItem == ID_USHORT_MAX) )
        {
            mWaitForTable[aSlot][sTransSlot].mIndex = 1;
            mWaitForTable[aSlot][sTransSlot].mNxtWaitTransItem
                = mArrOfLockList[aSlot].mFstWaitTblTransItem;
            mArrOfLockList[aSlot].mFstWaitTblTransItem = sTransSlot;
        } //if
        aLockNode = aLockNode->mNxtLockNode;
    }//while
}

/*********************************************************
  function description: setLockModeAndAddLockSlot
  Ʈ������� ������ table�� lock�� ���� ��쿡 ���Ͽ�,
  ���� Ʈ������� lock mode�� setting�ϰ�,
  �̹����� lock�� ��Ҵٸ� lock slot�� Ʈ������� lock
  node�� �߰��Ѵ�.
***********************************************************/
void smlLockMgr::setLockModeAndAddLockSlotMutex( SInt           aSlot,
                                                 smlLockNode  * aTxLockNode,
                                                 smlLockMode  * aCurLockMode,
                                                 smlLockMode    aLockMode,
                                                 idBool         aIsLocked,
                                                 smlLockSlot ** aLockSlot )
{
    if ( aTxLockNode != NULL )
    {
        if ( aCurLockMode != NULL )
        {
            *aCurLockMode = aTxLockNode->mLockMode;
        }
        // Ʈ������� lock slot list��  lock slot  �߰� .
        if ( aIsLocked == ID_TRUE )
        {
            aTxLockNode->mFlag |= mLockModeToMask[aLockMode];
            addLockSlot( &(aTxLockNode->mArrLockSlotList[aLockMode]),
                         aSlot );

            if ( aLockSlot != NULL )
            {
                *aLockSlot = &(aTxLockNode->mArrLockSlotList[aLockMode]);
            }//if aLockSlot

        }//if aIsLocked
    }//aTxLockNode != NULL
}
