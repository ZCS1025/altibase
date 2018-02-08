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
 * $Id: smaDeleteThread.h 82075 2018-01-17 06:39:52Z jina.kim $
 **********************************************************************/

#ifndef _O_SMA_DELETE_THREAD_H_
#define _O_SMA_DELETE_THREAD_H_ 1

#include <idl.h>
#include <idu.h>
#include <idu.h>
#include <idtBaseThread.h>
#include <smxTrans.h>
#include <smaLogicalAger.h>
#include <smaDef.h>

class smaDeleteThread : public idtBaseThread
{
//For Operation
public:
    static IDE_RC initializeStatic();
    static IDE_RC destroyStatic();
    static IDE_RC shutdownAll();

    // Ư�� Table���� OID�� Ư�� Tablespace���� OID�� ���ؼ���
    // ��� Aging�� �ǽ��Ѵ�.
    static IDE_RC deleteInstantly( smaInstantAgingFilter * aAgingFilter );

    static IDE_RC processJob( smxTrans    * aTransPtr,
                              smxOIDInfo  * aOIDInfoPtr,
                              idBool        aDeleteAll,
                              idBool      * aIsProcessed );

    IDE_RC initialize();
    IDE_RC destroy();

    virtual void run();
    IDE_RC  shutdown();
    IDE_RC  realDelete(idBool aDeleteAll);

    static IDE_RC  lock() { return mMutex.lock( NULL ); }
    static IDE_RC  unlock() { return mMutex.unlock(); }

    static IDE_RC  lockCheckMtx() { return mCheckMutex.lock( NULL ); }
    static IDE_RC  unlockCheckMtx() { return mCheckMutex.unlock(); }
    
    static void waitForNoAccessAftDropTbl();
    static IDE_RC processFreeSlotPending(
        smxTrans   *aTrans,
        smxOIDList *aOIDList);

    smaDeleteThread();

private:
    static inline idBool isAgingTarget( smaOidList  *aOIDHead,
                                        idBool       aDeleteAll,
                                        smSCN       *aMinViewSCN );

    IDE_RC processAgingOIDNodeList( smaOidList *aOIDHead,
                                    idBool      aDeleteAll );

    static inline void beginATrans();
    static inline void commitATrans();
    static inline void commitAndBeginATransIfNeed();
    static inline IDE_RC freeOIDNodeListHead( smaOidList *aOIDHead );

//For Member
public:
    static ULong            mHandledCnt;
    static UInt             mThreadCnt;
    static smaDeleteThread *mDeleteThreadList;
    static smxTrans        *mTrans;
    static ULong            mAgingCntAfterATransBegin;
    
    /* BUG-17417 V$Ager������ Add OID������ ���� Ager��
     *           �ؾ��� �۾��� ������ �ƴϴ�.
     *
     * mAgingProcessedOIDCnt �߰���.  */

    /* Ager�� OID List�� OID�� �ϳ��� ó���ϴµ� �̶� 1������ */
    static ULong  mAgingProcessedOIDCnt;

    UInt                mTimeOut;
    idBool              mFinished;
    idBool              mHandled;

    //BUG-17371 [MMDB] Aging�� �и���� System�� ������ �� Aging��
    //                  �и��� ������ �� ��ȭ��
    // getMinSCN������, MinSCN������ �۾����� ���� Ƚ��
    static ULong        mSleepCountOnAgingCondition;
private:
    static iduMutex         mMutex;
    static iduMutex         mCheckMutex;
};

/*
 * aOIDHead�� Aging������� �����Ѵ�. 
 *
 * aOIDHead     - [IN] Aging��� OID List�� Head
 * aDeleteAll   - [IN] ID_TRUE�̸� ������ Aging ������� ó���Ѵ�.
 * aMinViewSCN  - [IN] Transaction Minimum SCN
 */
idBool smaDeleteThread::isAgingTarget( smaOidList  *aOIDHead,
                                       idBool       aDeleteAll,
                                       smSCN       *aMinViewSCN )
{
    if( aOIDHead != NULL )
    {
        if( aDeleteAll == ID_FALSE )
        {
            if( smaLogicalAger::mTailDeleteThread->mErasable
                == ID_FALSE )
            {
                return ID_FALSE;
            }

            /* Aging Node List�� �ִ� Aging��� Row�� �ٸ� Transaction��
               ���� �ִ��� �����Ѵ�. Logical Ager�� Index���� �ش� Row��
               ����µ�, �ش� Row�� Index���� �����ٰ� �ؼ� �ٷ� Row�� Table����
               ������� ����. �ֳ��ϸ� Index Ž���� Latch�� ���� �ʰ� �ϱ� ������
               Row�� Index Node�� ���ؼ� �����Ҽ� �ֱ⶧���̴�.
               �̶����� Node Header�� Index���� �����۾��� �Ϸ��Ŀ� mKeyFreeSCN��
               �����ϰ� Transaction�� Minimum View SCN�� ���ؼ� ���� ��������
               �ʴ´ٴ� ���� �����ϱ� ���� �ΰ��� ������ �˻��Ѵ�.

               1. mKeyFreeSCN < MinViewSCN

               �� ������ �����Ѵٸ� Aging�� ������ ���� �ִ�.

            */
            /* BUG-18343 �а� �ִ� Row�� Ager�� ���ؼ� �����ǰ� �ֽ��ϴ�.
             *
             * mKeyFreeSCN: OID List Header�� Key Free SCN
             *
             * smxTransMgr::getMemoryMinSCN���� ������
             * MinViewSCN:  Minimum View SCN
             * MinViewTID:  MinViewSCN�� ������ Transaction ID.
             *
             * Delete Thread������ OID List Header�� Key Free SCN�� MIN SCN����
             * ������ Aging�� �����ϵ��� ���ƴ�. ��������
             *    1. mKeyFreeSCN < MinViewSCN
             *    2. mKeyFreeSCN = MinViewSCN, MinViewTID = 0
             * �ΰ��� ������ �ϳ��� �����ϸ� Aging�� �����ϵ��� �Ͽ���. �׷���
             * MinViewTID�� 0�϶� ������ Ʈ����ǵ� MinViewSCN�� �ڽ��� ViewSCN�� ������ �ִ�.
             * �ֳĸ� smxTransMgr::getMemoryMinSCN���� ���� system scn�� Transaction��
             * min scn�� ���� ���� �����ϴµ� ���� ������ MinViewTID�� 0�� �ȴ�. �̶� Aging��
             * �����ϰ� �Ǹ� Transaction�� check�ϰ� �ִ� Row�� ���ؼ� Aging�� �߻��ϰ� �ȴ�.
             * ������ �� ������
             *    1. mKeyFreeSCN < MinViewSCN
             * �� ���θ� �����Ѵ�.
             */
            if( SM_SCN_IS_LT( &( aOIDHead->mKeyFreeSCN ), aMinViewSCN ) )
            {
                return ID_TRUE;
            }
            else
            {
                /* BUG-17371  [MMDB] Aging�� �и���� System�� ������ �� Aging��
                   �и��� ������ �� ��ȭ��.getMinSCN������, MinSCN������ �۾�����
                   ���� Ƚ�� */
                mSleepCountOnAgingCondition++;
                return ID_FALSE;
            }
        }

        return ID_TRUE;
    }

    return ID_FALSE;
}

/*
 * Aging Transaction�� �����Ѵ�.
 *
 */
void smaDeleteThread::beginATrans()
{
    IDE_ASSERT(
        mTrans->begin( NULL,
                       ( SMI_TRANSACTION_REPL_NONE |
                         SMI_COMMIT_WRITE_NOWAIT ),
                       SMX_NOT_REPL_TX_ID )
        == IDE_SUCCESS);

    smxTrans::setRSGroupID((void*)mTrans, 0);

    mAgingCntAfterATransBegin = 0;

}

/*
 * Aging Transaction�� Commit�Ѵ�.
 *
 */
void smaDeleteThread::commitATrans()
{
    smSCN sDummySCN;
    
    IDE_ASSERT( mTrans->commit(&sDummySCN) == IDE_SUCCESS );
}

/*
 * Aging�� ó���� Ƚ���� smuProperty::getAgerCommitInterval
 * ���� ũ�� Aging Transaction�� Commit�ϰ� ���� �����Ѵ�.
 *
 */
void smaDeleteThread::commitAndBeginATransIfNeed()
{
    if( mAgingCntAfterATransBegin >
        smuProperty::getAgerCommitInterval() )
    {
        commitATrans();
        beginATrans();
    }
}

/*
 * aOIDHead�� ����Ű�� OID List Head�� Free�Ѵ�.
 *
 * aOIDHead - [IN] OID List Header
 */
IDE_RC smaDeleteThread::freeOIDNodeListHead( smaOidList *aOIDHead )
{
    smmSlot *sSlot;

    sSlot = (smmSlot*)aOIDHead;
    sSlot->next = sSlot;
    sSlot->prev = sSlot;

    IDE_TEST( smaLogicalAger::mSlotList[2]->releaseSlots(
                  1,
                  sSlot,
                  SMM_SLOT_LIST_MUTEX_NEEDLESS )
              != IDE_SUCCESS);

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

#endif
