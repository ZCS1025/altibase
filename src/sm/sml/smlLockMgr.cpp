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
 * $Id: smlLockMgr.cpp 82075 2018-01-17 06:39:52Z jina.kim $
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
#include <iduList.h>
#include <ideErrorMgr.h>

#include <smErrorCode.h>
//#include <smlDef.h>
#include <smDef.h>
#include <smr.h>
#include <smc.h>
#include <sml.h>
#include <smlReq.h>
#include <smu.h>
//#include <smrLogHeadI.h>
#include <sct.h>
//#include <smxTrans.h>
//#include <smxTransMgr.h>
#include <smx.h>    


/*
   �� ȣȯ�� ���
   ���� - ���� �ɷ��ִ� ��Ÿ��
   ���� - ���ο� ��Ÿ��
*/
idBool smlLockMgr::mCompatibleTBL[SML_NUMLOCKTYPES][SML_NUMLOCKTYPES] = {
/*                   SML_NLOCK SML_SLOCK SML_XLOCK SML_ISLOCK SML_IXLOCK SML_SIXLOCK */
/* for SML_NLOCK  */{ID_TRUE,  ID_TRUE,  ID_TRUE,  ID_TRUE,   ID_TRUE,   ID_TRUE},
/* for SML_SLOCK  */{ID_TRUE,  ID_TRUE,  ID_FALSE, ID_TRUE,   ID_FALSE,  ID_FALSE},
/* for SML_XLOCK  */{ID_TRUE,  ID_FALSE, ID_FALSE, ID_FALSE,  ID_FALSE,  ID_FALSE},
/* for SML_ISLOCK */{ID_TRUE,  ID_TRUE,  ID_FALSE, ID_TRUE,   ID_TRUE,   ID_TRUE},
/* for SML_IXLOCK */{ID_TRUE,  ID_FALSE, ID_FALSE, ID_TRUE,   ID_TRUE,   ID_FALSE},
/* for SML_SIXLOCK*/{ID_TRUE,  ID_FALSE, ID_FALSE, ID_TRUE,   ID_FALSE,  ID_FALSE}
};
/*
   �� ��ȯ ���
   ���� - ���� �ɷ��ִ� ��Ÿ��
   ���� - ���ο� ��Ÿ��
*/
smlLockMode smlLockMgr::mConversionTBL[SML_NUMLOCKTYPES][SML_NUMLOCKTYPES] = {
/*                   SML_NLOCK    SML_SLOCK    SML_XLOCK  SML_ISLOCK   SML_IXLOCK   SML_SIXLOCK */
/* for SML_NLOCK  */{SML_NLOCK,   SML_SLOCK,   SML_XLOCK, SML_ISLOCK,  SML_IXLOCK,  SML_SIXLOCK},
/* for SML_SLOCK  */{SML_SLOCK,   SML_SLOCK,   SML_XLOCK, SML_SLOCK,   SML_SIXLOCK, SML_SIXLOCK},
/* for SML_XLOCK  */{SML_XLOCK,   SML_XLOCK,   SML_XLOCK, SML_XLOCK,   SML_XLOCK,   SML_XLOCK},
/* for SML_ISLOCK */{SML_ISLOCK,  SML_SLOCK,   SML_XLOCK, SML_ISLOCK,  SML_IXLOCK,  SML_SIXLOCK},
/* for SML_IXLOCK */{SML_IXLOCK,  SML_SIXLOCK, SML_XLOCK, SML_IXLOCK,  SML_IXLOCK,  SML_SIXLOCK},
/* for SML_SIXLOCK*/{SML_SIXLOCK, SML_SIXLOCK, SML_XLOCK, SML_SIXLOCK, SML_SIXLOCK, SML_SIXLOCK}
};

/*
   �� Mode���� ���̺�
*/
smlLockMode smlLockMgr::mDecisionTBL[64] = {
    SML_NLOCK,   SML_SLOCK,   SML_XLOCK, SML_XLOCK,
    SML_ISLOCK,  SML_SLOCK,   SML_XLOCK, SML_XLOCK,
    SML_IXLOCK,  SML_SIXLOCK, SML_XLOCK, SML_XLOCK,
    SML_IXLOCK,  SML_SIXLOCK, SML_XLOCK, SML_XLOCK,
    SML_SIXLOCK, SML_SIXLOCK, SML_XLOCK, SML_XLOCK,
    SML_SIXLOCK, SML_SIXLOCK, SML_XLOCK, SML_XLOCK,
    SML_SIXLOCK, SML_SIXLOCK, SML_XLOCK, SML_XLOCK,
    SML_SIXLOCK, SML_SIXLOCK, SML_XLOCK, SML_XLOCK,
    SML_NLOCK,   SML_SLOCK,   SML_XLOCK, SML_XLOCK,
    SML_ISLOCK,  SML_SLOCK,   SML_XLOCK, SML_XLOCK,
    SML_IXLOCK,  SML_SIXLOCK, SML_XLOCK, SML_XLOCK,
    SML_IXLOCK,  SML_SIXLOCK, SML_XLOCK, SML_XLOCK,
    SML_SIXLOCK, SML_SIXLOCK, SML_XLOCK, SML_XLOCK,
    SML_SIXLOCK, SML_SIXLOCK, SML_XLOCK, SML_XLOCK,
    SML_SIXLOCK, SML_SIXLOCK, SML_XLOCK, SML_XLOCK,
    SML_SIXLOCK, SML_SIXLOCK, SML_XLOCK, SML_XLOCK
};

/*
   �� Mode�� ���� Lock Mask���� ���̺�
*/
SInt smlLockMgr::mLockModeToMask[SML_NUMLOCKTYPES] = {
    /* for SML_NLOCK  */ 0x00000000,
    /* for SML_SLOCK  */ 0x00000001,
    /* for SML_XLOCK  */ 0x00000002,
    /* for SML_ISLOCK */ 0x00000004,
    /* for SML_IXLOCK */ 0x00000008,
    /* for SML_SIXLOCK*/ 0x00000010
};

smlLockMode2StrTBL smlLockMgr::mLockMode2StrTBL[SML_NUMLOCKTYPES] ={
    {SML_NLOCK,"NO_LOCK"},
    {SML_SLOCK,"S_LOCK"},
    {SML_XLOCK,"X_LOCK"},
    {SML_ISLOCK,"IS_LOCK"},
    {SML_IXLOCK,"IX_LOCK"},
    {SML_SIXLOCK,"SIX_LOCK"}
};

const SInt
smlLockMgr::mLockBit[SML_NUMLOCKTYPES] =
{
    0,      /* NLOCK */
    0,      /* SLOCK */
    61,     /* XLOCK */
    20,     /* ISLOCK */
    40,     /* IXLOCK */
    60      /* SIXLOCK */
};

const SLong
smlLockMgr::mLockDelta[SML_NUMLOCKTYPES] =
{
    0,      /* NLOCK */
    ID_LONG(1) << smlLockMgr::mLockBit[SML_SLOCK],   /* SLOCK */
    ID_LONG(1) << smlLockMgr::mLockBit[SML_XLOCK],   /* XLOCK */
    ID_LONG(1) << smlLockMgr::mLockBit[SML_ISLOCK],  /* ISLOCK */
    ID_LONG(1) << smlLockMgr::mLockBit[SML_IXLOCK],  /* IXLOCK */
    ID_LONG(1) << smlLockMgr::mLockBit[SML_SIXLOCK]  /* SIXLOCK */
};

const SLong
smlLockMgr::mLockMax[SML_NUMLOCKTYPES] =
{
    0,          /* NLOCK */
    ID_LONG(0xFFFFF),   /* SLOCK */
    ID_LONG(1),         /* XLOCK */
    ID_LONG(0xFFFFF),   /* ISLOCK */
    ID_LONG(0xFFFFF),   /* IXLOCK */
    ID_LONG(1)          /* SIXLOCK */
};

const SLong
smlLockMgr::mLockMask[SML_NUMLOCKTYPES] =
{
    0,
    smlLockMgr::mLockMax[SML_SLOCK]    << smlLockMgr::mLockBit[SML_SLOCK],
    smlLockMgr::mLockMax[SML_XLOCK]    << smlLockMgr::mLockBit[SML_XLOCK],
    smlLockMgr::mLockMax[SML_ISLOCK]   << smlLockMgr::mLockBit[SML_ISLOCK],
    smlLockMgr::mLockMax[SML_IXLOCK]   << smlLockMgr::mLockBit[SML_IXLOCK],
    smlLockMgr::mLockMax[SML_SIXLOCK]  << smlLockMgr::mLockBit[SML_SIXLOCK]
};

const smlLockMode
smlLockMgr::mPriority[SML_NUMLOCKTYPES] = 
{
    SML_XLOCK,
    SML_SLOCK,
    SML_SIXLOCK,
    SML_IXLOCK,
    SML_ISLOCK,
    SML_NLOCK
};



SInt                    smlLockMgr::mTransCnt;
SInt                    smlLockMgr::mSpinSlotCnt;
iduMemPool              smlLockMgr::mLockPool;
smlLockMatrixItem**     smlLockMgr::mWaitForTable;
smiLockWaitFunc         smlLockMgr::mLockWaitFunc;
smiLockWakeupFunc       smlLockMgr::mLockWakeupFunc;
smlTransLockList*       smlLockMgr::mArrOfLockList;

smlLockTableFunc        smlLockMgr::mLockTableFunc;
smlUnlockTableFunc      smlLockMgr::mUnlockTableFunc;

smlAllocLockItemFunc            smlLockMgr::mAllocLockItemFunc;
smlInitLockItemFunc             smlLockMgr::mInitLockItemFunc;
smlRegistRecordLockWaitFunc     smlLockMgr::mRegistRecordLockWaitFunc;
smlDidLockReleasedFunc          smlLockMgr::mDidLockReleasedFunc;
smlFreeAllRecordLockFunc        smlLockMgr::mFreeAllRecordLockFunc;
smlClearWaitItemColsOfTransFunc smlLockMgr::mClearWaitItemColsOfTransFunc;
smlAllocLockNodeFunc            smlLockMgr::mAllocLockNodeFunc;
smlFreeLockNodeFunc             smlLockMgr::mFreeLockNodeFunc;

SInt**                  smlLockMgr::mPendingMatrix;
SInt*                   smlLockMgr::mPendingArray;
SInt*                   smlLockMgr::mPendingCount;
SInt**                  smlLockMgr::mIndicesMatrix;
SInt*                   smlLockMgr::mIndicesArray;
idBool*                 smlLockMgr::mIsCycle;
idBool*                 smlLockMgr::mIsChecked;
SInt*                   smlLockMgr::mDetectQueue;
ULong*                  smlLockMgr::mSerialArray;
ULong                   smlLockMgr::mPendSerial;
ULong                   smlLockMgr::mTXPendCount;

smlLockNode**           smlLockMgr::mNodeCache;
smlLockNode*            smlLockMgr::mNodeCacheArray;
ULong*                  smlLockMgr::mNodeAllocMap;

SInt                    smlLockMgr::mStopDetect;
idtThreadRunner         smlLockMgr::mDetectDeadlockThread;

static IDE_RC smlLockWaitNAFunction( ULong, idBool * )
{

    return IDE_SUCCESS;

}

static IDE_RC smlLockWakeupNAFunction()
{

    return IDE_SUCCESS;

}


IDE_RC smlLockMgr::initialize( UInt              aTransCnt,
                               smiLockWaitFunc   aLockWaitFunc,
                               smiLockWakeupFunc aLockWakeupFunc )
{

    SInt i;
    smlTransLockList * sTransLockList; /* BUG-43408 */

    mTransCnt = aTransCnt;
    mSpinSlotCnt = (mTransCnt + 63) / 64;

    IDE_ASSERT(mTransCnt > 0);
    IDE_ASSERT(mSpinSlotCnt > 0);

    /* TC/FIT/Limit/sm/sml/smlLockMgr_initialize_calloc1.sql */
    IDU_FIT_POINT_RAISE( "smlLockMgr::initialize::calloc1",
                          insufficient_memory );

    IDE_TEST( mLockPool.initialize( IDU_MEM_SM_SML,
                                    (SChar*)"LOCK_MEMORY_POOL",
                                    ID_SCALABILITY_SYS,
                                    sizeof(smlLockNode),
                                    SML_LOCK_POOL_SIZE,
                                    IDU_AUTOFREE_CHUNK_LIMIT,			/* ChunkLimit */
                                    ID_TRUE,							/* UseMutex */
                                    IDU_MEM_POOL_DEFAULT_ALIGN_SIZE,	/* AlignByte */
                                    ID_FALSE,							/* ForcePooling */
                                    ID_TRUE,							/* GarbageCollection */
                                    ID_TRUE ) != IDE_SUCCESS );			/* HWCacheLine */

    // allocate transLock List array.
    mArrOfLockList = NULL;

    /* TC/FIT/Limit/sm/sml/smlLockMgr_initialize_malloc.sql */
    IDU_FIT_POINT_RAISE( "smlLockMgr::initialize::malloc",
                          insufficient_memory );

    IDE_TEST_RAISE( iduMemMgr::malloc( IDU_MEM_SM_SML,
                                       (ULong)sizeof(smlTransLockList) * mTransCnt,
                                       (void**)&mArrOfLockList ) != IDE_SUCCESS,
                    insufficient_memory );
    mLockWaitFunc = ( (aLockWaitFunc == NULL) ?
                      smlLockWaitNAFunction : aLockWaitFunc );

    mLockWakeupFunc = ( (aLockWakeupFunc == NULL) ?
                        smlLockWakeupNAFunction : aLockWakeupFunc );

    for ( i = 0 ; i < mTransCnt ; i++ )
    {
        initTransLockList(i);
    }

    switch( smuProperty::getLockMgrCacheNode() )
    {
    case 0:
        mAllocLockNodeFunc  = allocLockNodeNormal;
        mFreeLockNodeFunc   = freeLockNodeNormal;
        break;

    case 1:
        for ( i = 0 ; i < mTransCnt ; i++ )
        {
            sTransLockList = (mArrOfLockList+i);
            IDE_TEST_RAISE( initTransLockNodeCache( i, 
                                                    &( sTransLockList->mLockNodeCache ) )
                            != IDE_SUCCESS,
                            insufficient_memory );
        }

        mAllocLockNodeFunc  = allocLockNodeList;
        mFreeLockNodeFunc   = freeLockNodeList;
        break;

    case 2:
        IDE_TEST_RAISE( iduMemMgr::malloc( IDU_MEM_SM_SML,
                                           sizeof(smlLockNode) * mTransCnt * 64,
                                           (void**)&mNodeCacheArray ) != IDE_SUCCESS,
                        insufficient_memory );
        IDE_TEST_RAISE( iduMemMgr::malloc( IDU_MEM_SM_SML,
                                           sizeof(smlLockNode*) * mTransCnt,
                                           (void**)&mNodeCache ) != IDE_SUCCESS,
                        insufficient_memory );
        IDE_TEST_RAISE( iduMemMgr::calloc( IDU_MEM_SM_SML,
                                           mTransCnt, 
                                           sizeof(ULong),
                                           (void**)&mNodeAllocMap ) != IDE_SUCCESS,
                        insufficient_memory );

        for ( i = 0 ; i < mTransCnt ; i++ )
        {
            mNodeCache[i] = &(mNodeCacheArray[i * 64]);
        }
        
        mAllocLockNodeFunc  = allocLockNodeBitmap;
        mFreeLockNodeFunc   = freeLockNodeBitmap;

        break;

    }

    switch( smuProperty::getLockMgrType() )
    {
    case 0:
        //Alloc wait table
        IDE_TEST( initializeMutex(aTransCnt) != IDE_SUCCESS );
        break;

    case 1:
        IDE_TEST( initializeSpin(aTransCnt) != IDE_SUCCESS );
        break;

    default:
        IDE_ASSERT(0);
        break;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( insufficient_memory );
    {
        IDE_SET(ideSetErrorCode(idERR_ABORT_InsufficientMemory));
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/* BUG-43408 */
IDE_RC smlLockMgr::initTransLockNodeCache( SInt      aSlotID, 
                                           iduList * aTxSlotLockNodeBitmap )
{
    smlLockNode * sLockNode;
    UInt          i;
    UInt          sLockNodeCacheCnt;

    /* BUG-43408 Property�� �ʱ� �Ҵ� ������ ���� */
    sLockNodeCacheCnt = smuProperty::getLockNodeCacheCount();
    
    IDU_LIST_INIT( aTxSlotLockNodeBitmap );

    for ( i = 0 ; i < sLockNodeCacheCnt ; i++ )
    {
        IDE_TEST( mLockPool.alloc( (void**)&sLockNode )
                  != IDE_SUCCESS );
        sLockNode->mSlotID = aSlotID;
        IDU_LIST_INIT_OBJ( &(sLockNode->mNode4LockNodeCache), sLockNode );
        IDU_LIST_ADD_AFTER( aTxSlotLockNodeBitmap, &(sLockNode->mNode4LockNodeCache) );
    }
    
    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smlLockMgr::destroy()
{
    iduListNode*        sIterator = NULL;
    iduListNode*        sNodeNext = NULL;
    iduList*            sLockNodeCache;
    smlLockNode*        sLockNode;
    smlTransLockList*   sTransLockList;
    SInt                i;

    switch(smuProperty::getLockMgrType())
    {
    case 0:
        IDE_TEST( destroyMutex() != IDE_SUCCESS );
        break;

    case 1:
        IDE_TEST( destroySpin() != IDE_SUCCESS );
        break;

    default:
        IDE_ASSERT(0);
        break;
    }

    switch(smuProperty::getLockMgrCacheNode())
    {
    case 0:
        /* do nothing */
        break;

    case 1:
        for ( i = 0 ; i < mTransCnt ; i++ )
        {
            sTransLockList = (mArrOfLockList+i);
            sLockNodeCache = &(sTransLockList->mLockNodeCache);
            IDU_LIST_ITERATE_SAFE( sLockNodeCache, sIterator, sNodeNext )
            {
                sLockNode = (smlLockNode*) (sIterator->mObj);
                IDE_TEST( mLockPool.memfree(sLockNode) != IDE_SUCCESS );
            }
        }
        break;

    case 2:
        IDE_TEST( iduMemMgr::free(mNodeCacheArray) != IDE_SUCCESS );
        IDE_TEST( iduMemMgr::free(mNodeCache) != IDE_SUCCESS );
        IDE_TEST( iduMemMgr::free(mNodeAllocMap) != IDE_SUCCESS );
        break;
    }

    IDE_TEST( mLockPool.destroy() != IDE_SUCCESS );

    mLockWaitFunc = NULL;

    IDE_TEST( iduMemMgr::free(mArrOfLockList) != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/*********************************************************
  function description: initTransLockList
  Transaction lock list array���� aSlot�� �ش��ϴ�
  smlTransLockList�� �ʱ�ȭ�� �Ѵ�.
  - Tx�� ��ó�� table lock ��� item
  - Tx�� ��ó�� record lock��� item.
  - Tx�� lock node�� list �ʱ�ȭ.
  - Tx�� lock slot list�ʱ�ȭ.
***********************************************************/
void  smlLockMgr::initTransLockList( SInt aSlot )
{
    smlTransLockList * sTransLockList = (mArrOfLockList+aSlot);

    sTransLockList->mFstWaitTblTransItem  = SML_END_ITEM;
    sTransLockList->mFstWaitRecTransItem  = SML_END_ITEM;
    sTransLockList->mLstWaitRecTransItem  = SML_END_ITEM;

    sTransLockList->mLockSlotHeader.mPrvLockSlot  = &(sTransLockList->mLockSlotHeader);
    sTransLockList->mLockSlotHeader.mNxtLockSlot  = &(sTransLockList->mLockSlotHeader);

    sTransLockList->mLockSlotHeader.mLockNode     = NULL;
    sTransLockList->mLockSlotHeader.mLockSequence = 0;

    sTransLockList->mLockNodeHeader.mPrvTransLockNode = &(sTransLockList->mLockNodeHeader);
    sTransLockList->mLockNodeHeader.mNxtTransLockNode = &(sTransLockList->mLockNodeHeader);
}

IDE_RC smlLockMgr::freeLockItem( void * aLockItem )
{
    IDE_TEST( iduMemMgr::free(aLockItem) != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smlLockMgr::initLockItemCore( scSpaceID          aSpaceID,
                                     ULong              aItemID,
                                     smiLockItemType    aLockItemType,
                                     void*              aLockItem )
{
    smlLockItem * sLockItem;
    SChar         sBuffer[128];

    sLockItem = (smlLockItem*) aLockItem;

    /* LockItem Type�� ���� mSpaceID�� mItemID�� �����Ѵ�.
     *                   mSpaceID    mItemID
     * TableSpace Lock :  SpaceID     N/A
     * Table Lock      :  SpaceID     TableOID
     * DataFile Lock   :  SpaceID     FileID */
    sLockItem->mLockItemType    = aLockItemType;
    sLockItem->mSpaceID         = aSpaceID;
    sLockItem->mItemID          = aItemID;

    idlOS::sprintf( sBuffer,
                    "LOCKITEM_MUTEX_%"ID_UINT32_FMT"_%"ID_UINT64_FMT,
                    aSpaceID,
                    (ULong)aItemID );

    IDE_TEST_RAISE( sLockItem->mMutex.initialize( sBuffer,
                                                  IDU_MUTEX_KIND_NATIVE,
                                                  IDV_WAIT_INDEX_NULL ) 
                    != IDE_SUCCESS,
                    mutex_init_error );

    return IDE_SUCCESS;

    IDE_EXCEPTION(mutex_init_error);
    {
        IDE_SET(ideSetErrorCode(smERR_FATAL_ThrMutexInit));
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smlLockMgr::destroyLockItem( void *aLockItem )
{
    IDE_TEST( ((smlLockItem*)(aLockItem))->mMutex.destroy() != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    IDE_SET(ideSetErrorCode(smERR_FATAL_ThrMutexDestroy));

    return IDE_FAILURE;
}

void smlLockMgr::getTxLockInfo( SInt aSlot, smTID *aOwnerList, UInt *aOwnerCount )
{

    UInt      i;
    void*   sTrans;

    if ( aSlot >= 0 )
    {

        for ( i = mArrOfLockList[aSlot].mFstWaitTblTransItem;
              (i != SML_END_ITEM) && (i != ID_USHORT_MAX);
              i = mWaitForTable[aSlot][i].mNxtWaitTransItem )
        {
            if ( mWaitForTable[aSlot][i].mIndex == 1 )
            {
                sTrans = smLayerCallback::getTransBySID( i );
                aOwnerList[*aOwnerCount] = smLayerCallback::getTransID( sTrans );
                (*aOwnerCount)++;
            }
            else
            {
                /* nothing to do */
            }            
        }
    }
}

/*********************************************************
  function description: addLockNode
  Ʈ������� table�� ���Ͽ� lock�� ��� �Ǵ� ���,
  �� table�� grant list  lock node��  �ڽ��� Ʈ�����
  lock list�� �߰��Ѵ�.

  lock�� ��� �Ǵ� ���� �Ʒ��� �ΰ��� ����̴�.
  1. Ʈ������� table�� ���Ͽ� lock�� grant�Ǿ�
     lock�� ��� �Ǵ� ���.
  2. lock waiting�ϰ� �ִٰ� lock�� ��� �־���  �ٸ� Ʈ�����
     �� commit or rollback�� �����Ͽ�,  wakeup�Ǵ� ���.
***********************************************************/
void smlLockMgr::addLockNode( smlLockNode *aLockNode, SInt aSlot )
{

    smlLockNode*  sTransLockNodeHdr = &(mArrOfLockList[aSlot].mLockNodeHeader);

    aLockNode->mNxtTransLockNode = sTransLockNodeHdr;
    aLockNode->mPrvTransLockNode = sTransLockNodeHdr->mPrvTransLockNode;

    sTransLockNodeHdr->mPrvTransLockNode->mNxtTransLockNode = aLockNode;
    sTransLockNodeHdr->mPrvTransLockNode  = aLockNode;
}

/*********************************************************
  function description: removeLockNode
  Ʈ����� lock list array����  transaction�� slotid��
  �ش��ϴ� list���� lock node�� �����Ѵ�.
***********************************************************/
void smlLockMgr::removeLockNode(smlLockNode *aLockNode)
{

    aLockNode->mNxtTransLockNode->mPrvTransLockNode = aLockNode->mPrvTransLockNode;
    aLockNode->mPrvTransLockNode->mNxtTransLockNode = aLockNode->mNxtTransLockNode;

    aLockNode->mPrvTransLockNode = NULL;
    aLockNode->mNxtTransLockNode = NULL;

}

/*********************************************************
  PROJ-1381 Fetch Across Commits
  function description: freeAllItemLockExceptIS
  aSlot id�� �ش��ϴ� Ʈ������� grant�Ǿ� lock�� ���
  �ִ� lock�� IS lock�� �����ϰ� �����Ѵ�.

  ���� �������� ��Ҵ� lock���� lock�� �����Ѵ�.
***********************************************************/
IDE_RC smlLockMgr::freeAllItemLockExceptIS( SInt aSlot )
{
    smlLockSlot *sCurLockSlot;
    smlLockSlot *sPrvLockSlot;
    smlLockSlot *sHeadLockSlot;
    static SInt  sISLockMask = smlLockMgr::mLockModeToMask[SML_ISLOCK];

    sHeadLockSlot = &(mArrOfLockList[aSlot].mLockSlotHeader);

    sCurLockSlot = (smlLockSlot*)getLastLockSlotPtr(aSlot);

    while ( sCurLockSlot != sHeadLockSlot )
    {
        sPrvLockSlot = sCurLockSlot->mPrvLockSlot;

        if ( sISLockMask != sCurLockSlot->mMask ) /* IS Lock�� �ƴϸ� */
        {
            IDE_TEST( smlLockMgr::unlockTable(aSlot, NULL, sCurLockSlot)
                      != IDE_SUCCESS );
        }
        else
        {
            /* IS Lock�� ��� ��� fetch�� �����ϱ� ���ؼ� ���ܵд�. */
        }

        sCurLockSlot = sPrvLockSlot;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/*********************************************************
  function description: freeAllItemLock
  aSlot id�� �ش��ϴ� Ʈ������� grant�Ǿ� lock�� ���
  �ִ� lock�� �� �����Ѵ�.

  lock ���� ������ ���� �������� ��Ҵ� lock���� Ǯ��
  �����Ѵ�.
*******************m***************************************/
IDE_RC smlLockMgr::freeAllItemLock( SInt aSlot )
{

    smlLockNode *sCurLockNode;
    smlLockNode *sPrvLockNode;

    smlLockNode * sTransLockNodeHdr = &(mArrOfLockList[aSlot].mLockNodeHeader);

    sCurLockNode = sTransLockNodeHdr->mPrvTransLockNode;

    while( sCurLockNode != sTransLockNodeHdr )
    {
        sPrvLockNode = sCurLockNode->mPrvTransLockNode;


        IDE_TEST( smlLockMgr::unlockTable( aSlot, sCurLockNode, NULL )
                  != IDE_SUCCESS );
        sCurLockNode = sPrvLockNode;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/*********************************************************
  function description: partialTableUnlock
  Ʈ����� aSlot�� �ش��ϴ� lockSlot list����
  ���� ������ lock slot����  aLockSlot���� ���ٷ� �ö󰡸鼭
  aLockSlot�� �����ִ� lock node�� �̿��Ͽ� table unlock��
  �����Ѵ�. �̶� Lock Slot�� Sequence �� aLockSequence����
  Ŭ�������� Unlock�� �����Ѵ�.

  aSlot          - [IN] aSlot�� �ش��ϴ� Transaction����
  aLockSequence  - [IN] LockSlot�� mSequence�� ���� aLockSequence
                        ���� ����������  Unlock�� �����Ѵ�.
  aIsSeveralLock  - [IN] ID_FALSE:��� Lock�� ����.
                         ID_TRUE :Implicit Savepoint���� ��� ���̺� ����
                                  IS���� �����ϰ�,temp tbs�ϰ��� IX��������.

***********************************************************/
IDE_RC smlLockMgr::partialItemUnlock( SInt   aSlot,
                                      ULong  aLockSequence,
                                      idBool aIsSeveralLock )
{
    smlLockSlot *sCurLockSlot;
    smlLockSlot *sPrvLockNode;
    static SInt  sISLockMask = smlLockMgr::mLockModeToMask[SML_ISLOCK];
    static SInt  sIXLockMask = smlLockMgr::mLockModeToMask[SML_IXLOCK];
    scSpaceID    sSpaceID;
    IDE_RC       sRet = IDE_SUCCESS;

    sCurLockSlot = (smlLockSlot*)getLastLockSlotPtr(aSlot);

    while ( sCurLockSlot->mLockSequence > aLockSequence )
    {
        sPrvLockNode = sCurLockSlot->mPrvLockSlot;

        if ( aIsSeveralLock == ID_FALSE )
        {
            // Abort Savepoint���� �Ϲ����� ��� Partial Unlock
            // Lock�� ������ ������� LockSequence�� ������ �����Ͽ� ��� unlock
            IDE_TEST( smlLockMgr::unlockTable(aSlot, NULL, sCurLockSlot)
                      != IDE_SUCCESS );
        }
        else
        {
            // Statement End�� ȣ��Ǵ� ���
            // Implicit IS Lock��, TableTable�� IX Lock�� Unlock

            if ( sISLockMask == sCurLockSlot->mMask ) // IS Lock�� ���
            {
                /* BUG-15906: non-autocommit��忡�� select�Ϸ��� IS_LOCK�� �����Ǹ�
                   ���ڽ��ϴ�.
                   aPartialLock�� ID_TRUE�̸� IS_LOCK���� �����ϵ��� ��. */
                // BUG-28752 lock table ... in row share mode ������ ������ �ʽ��ϴ�. 
                // Implicit IS lock�� Ǯ���ݴϴ�.

                if ( sCurLockSlot->mLockNode->mIsExplicitLock != ID_TRUE )
                {
                    IDE_TEST( smlLockMgr::unlockTable(aSlot, NULL, sCurLockSlot)
                              != IDE_SUCCESS );
                }
            }
            else if ( sIXLockMask == sCurLockSlot->mMask ) //IX Lock�� ���
            {
                /* BUG-21743    
                 * Select ���꿡�� User Temp TBS ���� TBS�� Lock�� ��Ǯ���� ���� */
                sSpaceID = sCurLockSlot->mLockNode->mSpaceID;

                if ( sctTableSpaceMgr::isTempTableSpace( sSpaceID ) == ID_TRUE )
                {
                    IDE_TEST( smlLockMgr::unlockTable(aSlot, NULL, sCurLockSlot)
                              != IDE_SUCCESS );
                }
            }
        }

        sCurLockSlot = sPrvLockNode;
    }

    return sRet;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;

}

IDE_RC smlLockMgr::unlockItem( void *aTrans,
                               void *aLockSlot )
{
    IDE_DASSERT( aLockSlot != NULL );


    IDE_TEST( unlockTable( smLayerCallback::getTransSlot( aTrans ),
                           NULL,
                           (smlLockSlot*)aLockSlot )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/* BUG-18981 */
IDE_RC  smlLockMgr::logLocks( void*   aTrans,
                              smTID   aTID,
                              UInt    aFlag,
                              ID_XID* aXID,
                              SChar*  aLogBuffer,
                              SInt    aSlot,
                              smSCN*  aFstDskViewSCN )
{
    smrXaPrepareLog  *sPrepareLog;
    smlTableLockInfo  sLockInfo;
    SChar            *sLogBuffer;
    SChar            *sLog;
    UInt              sLockCount = 0;
    idBool            sIsLogged = ID_FALSE;
    smlLockNode      *sCurLockNode;
    smlLockNode      *sPrvLockNode;
    PDL_Time_Value    sTmv;
    smLSN             sEndLSN;

    /* -------------------------------------------------------------------
       xid�� �α׵���Ÿ�� ���
       ------------------------------------------------------------------- */
    sLogBuffer = aLogBuffer;
    sPrepareLog = (smrXaPrepareLog*)sLogBuffer;
    smrLogHeadI::setTransID(&sPrepareLog->mHead, aTID);
    smrLogHeadI::setFlag(&sPrepareLog->mHead, aFlag);
    smrLogHeadI::setType(&sPrepareLog->mHead, SMR_LT_XA_PREPARE);

    // BUG-27024 XA���� Prepare �� Commit���� ���� Disk Row��
    //           Server Restart �� ����Ῡ�� �մϴ�.
    // XA Trans�� FstDskViewSCN�� Log�� ����Ͽ�,
    // Restart�� XA Prepare Trans �籸�࿡ ���
    SM_SET_SCN( &sPrepareLog->mFstDskViewSCN, aFstDskViewSCN );

    if ( (smrLogHeadI::getFlag(&sPrepareLog->mHead) & SMR_LOG_SAVEPOINT_MASK)
         == SMR_LOG_SAVEPOINT_OK)
    {
        smrLogHeadI::setReplStmtDepth( &sPrepareLog->mHead,
                                       smLayerCallback::getLstReplStmtDepth( aTrans ) );
    }
    else
    {
        smrLogHeadI::setReplStmtDepth( &sPrepareLog->mHead,
                                       SMI_STATEMENT_DEPTH_NULL );
    }

    // prepared�� ������ �α���
    // �ֳ��ϸ� heuristic commit/rollback �������� timeout ����� ����ϴµ�
    // system failure ���Ŀ��� prepared�� ��Ȯ�� ������ �����ϱ� ������.
    /* BUG-18981 */
    idlOS::memcpy(&(sPrepareLog->mXaTransID), aXID, sizeof(ID_XID));
    sTmv                       = idlOS::gettimeofday();
    sPrepareLog->mPreparedTime = (timeval)sTmv;

    /* -------------------------------------------------------------------
       table lock�� prepare log�� ����Ÿ�� �α�
       record lock�� OID ������ ����� ȸ���� ����� �ܰ迡�� �����ؾ� ��
       ------------------------------------------------------------------- */
    sLog         = sLogBuffer + SMR_LOGREC_SIZE(smrXaPrepareLog);
    sCurLockNode = mArrOfLockList[aSlot].mLockNodeHeader.mPrvTransLockNode;

    while ( sCurLockNode != &(mArrOfLockList[aSlot].mLockNodeHeader) )
    {
        sPrvLockNode = sCurLockNode->mPrvTransLockNode;

        // ���̺� �����̽� ���� DDL�� XA�� �������� �ʴ´�.
        if ( sCurLockNode->mLockItemType != SMI_LOCK_ITEM_TABLE )
        {
            sCurLockNode = sPrvLockNode;
            continue;
        }

        sLockInfo.mOidTable = (smOID)sCurLockNode->mLockItem->mItemID;
        sLockInfo.mLockMode = getLockMode(sCurLockNode);

        idlOS::memcpy(sLog, &sLockInfo, sizeof(smlTableLockInfo));
        sLog        += sizeof( smlTableLockInfo );
        sCurLockNode = sPrvLockNode;
        sLockCount++;

        if ( sLockCount >= SML_MAX_LOCK_INFO )
        {
            smrLogHeadI::setSize( &sPrepareLog->mHead,
                                  SMR_LOGREC_SIZE(smrXaPrepareLog) +
                                   + (sLockCount * sizeof(smlTableLockInfo))
                                   + sizeof(smrLogTail));

            sPrepareLog->mLockCount = sLockCount;

            smrLogHeadI::copyTail( (SChar*)sLogBuffer +
                                   smrLogHeadI::getSize(&sPrepareLog->mHead) -
                                   sizeof(smrLogTail),
                                   &(sPrepareLog->mHead) );

            IDE_TEST( smrLogMgr::writeLog( NULL, /* idvSQL* */
                                           aTrans,
                                           (SChar*)sLogBuffer,
                                           NULL,  // Previous LSN Ptr
                                           NULL,  // Log LSN Ptr
                                           NULL ) // End LSN Ptr
                     != IDE_SUCCESS );

            sLockCount = 0;
            sIsLogged  = ID_TRUE;
            sLog       = sLogBuffer + SMR_LOGREC_SIZE(smrXaPrepareLog);
        }
    }

    if ( !( (sIsLogged == ID_TRUE) && (sLockCount == 0) ) )
    {
        smrLogHeadI::setSize(&sPrepareLog->mHead,
                             SMR_LOGREC_SIZE(smrXaPrepareLog)
                                + sLockCount * sizeof(smlTableLockInfo)
                                + sizeof(smrLogTail));

        sPrepareLog->mLockCount = sLockCount;

        smrLogHeadI::copyTail( (SChar*)sLogBuffer +
                               smrLogHeadI::getSize(&sPrepareLog->mHead) -
                               sizeof(smrLogTail),
                               &(sPrepareLog->mHead) );

        IDE_TEST( smrLogMgr::writeLog( NULL, /* idvSQL* */
                                       aTrans,
                                       (SChar*)sLogBuffer,
                                       NULL,  // Previous LSN Ptr
                                       NULL,  // Log LSN Ptr
                                       &sEndLSN ) // End LSN Ptr
                  != IDE_SUCCESS );

        if ( smLayerCallback::isNeedLogFlushAtCommitAPrepare( aTrans )
             == ID_TRUE )
        {
            IDE_TEST( smrLogMgr::syncLFThread( SMR_LOG_SYNC_BY_TRX,
                                               &sEndLSN )
                      != IDE_SUCCESS );
        }
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smlLockMgr::lockItem( void        *aTrans,
                             void        *aLockItem,
                             idBool       aIsIntent,
                             idBool       aIsExclusive,
                             ULong        aLockWaitMicroSec,
                             idBool     * aLocked,
                             void      ** aLockSlot )
{
    smlLockMode sLockMode;

    if ( aIsIntent == ID_TRUE )
    {
        if ( aIsExclusive == ID_TRUE )
        {
            sLockMode = SML_IXLOCK;
        }
        else
        {
            sLockMode = SML_ISLOCK;
        }
    }
    else
    {
        if ( aIsExclusive == ID_TRUE )
        {
            sLockMode = SML_XLOCK;
        }
        else
        {
            sLockMode = SML_SLOCK;
        }
    }

    IDE_TEST( lockTable( smLayerCallback::getTransSlot( aTrans ),
                         (smlLockItem *)aLockItem,
                         sLockMode,
                         aLockWaitMicroSec, /* wait micro second */
                         NULL,      /* current lock mode */
                         aLocked,   /* is locked */
                         NULL,      /* get locked node */
                         (smlLockSlot**)aLockSlot ) != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smlLockMgr::lockTableModeX( void     *aTrans,
                                   void     *aLockItem )
{

    return lockTable( smLayerCallback::getTransSlot( aTrans ),
                      (smlLockItem *)aLockItem,
                      SML_XLOCK );
}
/*********************************************************
  function description: lockTableModeIX
  IX lock mode���� table lock�� �Ǵ�.
 ***********************************************************/
IDE_RC smlLockMgr::lockTableModeIX( void    *aTrans,
                                    void    *aLockItem )
{


    return lockTable( smLayerCallback::getTransSlot( aTrans ),
                      (smlLockItem *)aLockItem,
                      SML_IXLOCK );
}
/*********************************************************
  function description: lockTableModeIS
  IS lock mode���� table lock�� �Ǵ�.
 ***********************************************************/
IDE_RC smlLockMgr::lockTableModeIS(void    *aTrans,
                                   void    *aLockItem )
{


    return lockTable( smLayerCallback::getTransSlot( aTrans ),
                      (smlLockItem *)aLockItem,
                      SML_ISLOCK );
}

/*********************************************************
  function description: lockTableModeXAndCheckLocked
  X lock mode���� table lock�� �Ǵ�.
 ***********************************************************/
IDE_RC smlLockMgr::lockTableModeXAndCheckLocked( void   *aTrans,
                                                 void   *aLockItem,
                                                 idBool *aIsLock )
{

    smlLockMode      sLockMode;

    return lockTable( smLayerCallback::getTransSlot( aTrans ),
                      (smlLockItem *)aLockItem,
                      SML_XLOCK,
                      sctTableSpaceMgr::getDDLLockTimeOut(),
                      &sLockMode,
                      aIsLock );
}
/*********************************************************
  function description: getMutexOfLockItem
  table lock������ aLockItem�� mutex�� pointer��
  return�Ѵ�.
***********************************************************/
/* BUG-33048 [sm_transaction] The Mutex of LockItem can not be the Native
 * mutex.
 * LockItem���� NativeMutex�� ����� �� �ֵ��� ������ */
iduMutex * smlLockMgr::getMutexOfLockItem( void *aLockItem )
{

    return &( ((smlLockItem *)aLockItem)->mMutex );

}

void  smlLockMgr::lockTableByPreparedLog( void  * aTrans, 
                                          SChar * aLogPtr,
                                          UInt    aLockCnt,
                                          UInt  * aOffset )
{

    UInt i;
    smlTableLockInfo  sLockInfo;
    smOID sTableOID;
    smcTableHeader   *sTableHeader;
    smlLockItem      *sLockItem    = NULL;

    for ( i = 0 ; i < aLockCnt ; i++ )
    {

        idlOS::memcpy( &sLockInfo, 
                       (SChar *)((aLogPtr) + *aOffset),
                       sizeof(smlTableLockInfo) );
        sTableOID = sLockInfo.mOidTable;
        IDE_ASSERT( smcTable::getTableHeaderFromOID( sTableOID,
                                                     (void**)&sTableHeader )
                    == IDE_SUCCESS );

        sLockItem = (smlLockItem *)SMC_TABLE_LOCK( sTableHeader );
        IDE_ASSERT( sLockItem->mLockItemType == SMI_LOCK_ITEM_TABLE );

        smlLockMgr::lockTable( smLayerCallback::getTransSlot( aTrans ),
                               sLockItem,
                               sLockInfo.mLockMode );

        *aOffset += sizeof(smlTableLockInfo);
    }
}

void smlLockMgr::initLockNode( smlLockNode *aLockNode )
{
    SInt          i;
    smlLockSlot  *sLockSlotList;

    aLockNode->mLockMode          = SML_NLOCK;
    aLockNode->mPrvLockNode       = NULL;
    aLockNode->mNxtLockNode       = NULL;
    aLockNode->mCvsLockNode       = NULL;
        
    aLockNode->mPrvTransLockNode  = NULL;
    aLockNode->mNxtTransLockNode  = NULL;
    
    aLockNode->mDoRemove          = ID_TRUE;

    sLockSlotList = aLockNode->mArrLockSlotList;
    
    for ( i = 0 ; i < SML_NUMLOCKTYPES ; i++ )
    {
        sLockSlotList[i].mLockNode      = aLockNode;
        sLockSlotList[i].mMask          = mLockModeToMask[i];
        sLockSlotList[i].mPrvLockSlot   = NULL;
        sLockSlotList[i].mNxtLockSlot   = NULL;
        sLockSlotList[i].mLockSequence  = 0;
        sLockSlotList[i].mOldMode       = SML_NLOCK;
        sLockSlotList[i].mNewMode       = SML_NLOCK;
    }
}

IDE_RC smlLockMgr::allocLockNodeNormal(SInt /*aSlot*/, smlLockNode** aNewNode)
{
    IDE_TEST( mLockPool.alloc((void**)aNewNode) != IDE_SUCCESS );
    (*aNewNode)->mIndex = -1;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;
    return IDE_FAILURE;
}

IDE_RC smlLockMgr::allocLockNodeList( SInt aSlot, smlLockNode** aNewNode )
{
    /* BUG-43408 */
    smlLockNode*        sLockNode;
    smlTransLockList*   sTransLockList;
    iduList*            sLockNodeCache;
    iduListNode*        sNode;
    
    sTransLockList = (mArrOfLockList+aSlot);
    sLockNodeCache = &(sTransLockList->mLockNodeCache);

    if ( IDU_LIST_IS_EMPTY( sLockNodeCache ) )
    {
        IDE_TEST( mLockPool.alloc((void**)aNewNode)
                  != IDE_SUCCESS );
        sLockNode = *aNewNode;
        IDU_LIST_INIT_OBJ( &(sLockNode->mNode4LockNodeCache), sLockNode );
    }
    else
    {
        sNode = IDU_LIST_GET_FIRST( sLockNodeCache );
        sLockNode = (smlLockNode*)sNode->mObj;
        IDU_LIST_REMOVE( sNode );
        *aNewNode = sLockNode;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;
    return IDE_FAILURE;
}

IDE_RC smlLockMgr::allocLockNodeBitmap( SInt aSlot, smlLockNode** aNewNode )
{
    SInt            sIndex;
    smlLockNode*    sLockNode = NULL;

    IDE_DASSERT( smuProperty::getLockMgrCacheNode() == 1 );

    sIndex = acpBitFfs64(~(mNodeAllocMap[aSlot]));

    if ( sIndex != -1 )
    {
        mNodeAllocMap[aSlot]   |= ID_ULONG(1) << sIndex;
        sLockNode               = &(mNodeCache[aSlot][sIndex]);
        sLockNode->mIndex       = sIndex;

        *aNewNode               = sLockNode;
    }
    else
    {
        IDE_TEST( allocLockNodeNormal(aSlot, aNewNode) != IDE_SUCCESS );
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;
    return IDE_FAILURE;
}

/*********************************************************
  function description: allocLockNodeAndInit
  lock node�� �Ҵ��ϰ�, �ʱ�ȭ�� �Ѵ�.
***********************************************************/
IDE_RC  smlLockMgr::allocLockNodeAndInit( SInt           aSlot,
                                          smlLockMode    aLockMode,
                                          smlLockItem  * aLockItem,
                                          smlLockNode ** aNewLockNode,
                                          idBool         aIsExplicitLock ) 
{
    smlLockNode*    sLockNode = NULL;

    IDE_TEST( mAllocLockNodeFunc(aSlot, &sLockNode) != IDE_SUCCESS );
    IDE_DASSERT( sLockNode != NULL );
    initLockNode( sLockNode );

    // table oid added for perfv
    sLockNode->mLockItemType    = aLockItem->mLockItemType;
    sLockNode->mSpaceID         = aLockItem->mSpaceID;
    sLockNode->mItemID          = aLockItem->mItemID;
    sLockNode->mSlotID          = aSlot;
    sLockNode->mLockCnt         = 0;
    sLockNode->mLockItem        = aLockItem;
    sLockNode->mLockMode        = aLockMode;
    sLockNode->mFlag            = mLockModeToMask[aLockMode];
    // BUG-28752 implicit/explicit �����մϴ�.
    sLockNode->mIsExplicitLock  = aIsExplicitLock; 

    sLockNode->mTransID = smLayerCallback::getTransID( smLayerCallback::getTransBySID( aSlot ) );

    *aNewLockNode = sLockNode;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC smlLockMgr::freeLockNodeNormal( smlLockNode* aLockNode )
{
    return mLockPool.memfree(aLockNode);
}

IDE_RC smlLockMgr::freeLockNodeList( smlLockNode* aLockNode )
{
    SInt                sSlot;
    smlTransLockList*   sTransLockList;
    iduList*            sLockNodeCache;

    sSlot           = aLockNode->mSlotID;
    sTransLockList  = (mArrOfLockList+sSlot);
    sLockNodeCache  = &(sTransLockList->mLockNodeCache);
    IDU_LIST_ADD_AFTER( sLockNodeCache, &(aLockNode->mNode4LockNodeCache) );

    return IDE_SUCCESS;
}

IDE_RC smlLockMgr::freeLockNodeBitmap( smlLockNode* aLockNode )
{
    SInt            sSlot;
    SInt            sIndex;
    ULong           sDelta;

    if ( aLockNode->mIndex != -1 )
    {
        IDE_DASSERT( smuProperty::getLockMgrCacheNode() == 1 );

        sSlot   = aLockNode->mSlotID;
        sIndex  = aLockNode->mIndex;
        sDelta  = ID_ULONG(1) << sIndex;

        IDE_DASSERT( (mNodeAllocMap[sSlot] & sDelta) != 0 );
        mNodeAllocMap[sSlot] &= ~sDelta;
    }
    else
    {
        IDE_TEST( mLockPool.memfree(aLockNode) != IDE_SUCCESS );
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;
    return IDE_FAILURE;
}

