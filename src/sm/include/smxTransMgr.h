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
 * $Id: smxTransMgr.h 82186 2018-02-05 05:17:56Z lswhh $
 **********************************************************************/

#ifndef _O_SMX_TRANS_MGR_H_
#define _O_SMX_TRANS_MGR_H_ 1

#include <idu.h>
#include <smxTrans.h>
#include <smxTransFreeList.h>
#include <smxMinSCNBuild.h>

class smxTransFreeList;
class smrUTransQueue;

class smxTransMgr
{
//For Operation
public:
    // for idp::update
    enum {CONV_BUFF_SIZE=8};

    static IDE_RC   calibrateTransCount(UInt *aTransCnt);
    static IDE_RC initialize();
    static IDE_RC destroy();

    //For Transaction Management
    static IDE_RC alloc(smxTrans **aTrans,
                        idvSQL    *aStatistics = NULL,
                        idBool     aIgnoreRetry = ID_FALSE);
    inline static IDE_RC freeTrans(smxTrans  *aTrans);
    // for layer callback
    static IDE_RC alloc4LayerCall(void** aTrans);
    static IDE_RC freeTrans4LayerCall(void  *aTrans);
    //fix BUG-13175
    static IDE_RC allocNestedTrans(smxTrans  **aTrans);
    static IDE_RC allocNestedTrans4LayerCall(void  **aTrans);


    static inline smxTrans* getTransByTID(smTID aTransID);
    static inline smxTrans* getTransBySID(SInt aSlotN);
    static inline void*     getTransBySID2Void(SInt aSlotN);
    static inline idvSQL*   getStatisticsBySID(SInt aSlotN);
    static inline idBool    isActiveBySID(SInt aSlotN);
    static inline smTID     getTIDBySID(SInt aSlotN);
    static inline void      setTransBlockedBySID(const SInt);
    static inline void      setTransBegunBySID(const SInt);
    static inline smxStatus getTransStatus(const SInt);

    static inline IDE_RC resetBuilderInterval();
    static inline IDE_RC rebuildMinViewSCN( idvSQL * aStatistics );
    static inline void getSysMinDskViewSCN( smSCN* aMinSCN );

    static inline void getSysMinDskFstViewSCN( smSCN* aMinDskFstViewSCN );

    // BUG-26881 �߸��� CTS stamping���� acces�� �� ���� row�� ������
    static inline void getSysAgableSCN( smSCN* aSysAagableSCN );

    static void  getMinMemViewSCNofAll( smSCN* aMinSCN, smTID* aTID );

    // BUG-26881 �߸��� CTS stamping���� acces�� �� ���� row�� ������
    static void  getDskSCNsofAll( smSCN   * aMinViewSCN,
                                  smSCN   * aMinDskFstViewSCN,
                                  smSCN   * aMinOldestFstViewSCN );

    static IDE_RC getMinLSNOfAllActiveTrans( smLSN  * aLSN );
    static SInt getSlotID( smTID aTransID ){return aTransID & mSlotMask;}

    // for global transaction
    /* BUG-20727 */
    static IDE_RC existPreparedTrans (idBool * aExistFlag );
    static IDE_RC recover( SInt           * aSlotID,
                           /* BUG-18981 */
                           ID_XID         * aXID,
                           timeval        * aPreparedTime,
                           smxCommitState * aState );
    /* BUG-18981 */
    static IDE_RC getXID( smTID aTID, ID_XID  * aXID );
    static IDE_RC rebuildTransFreeList();

    static void dump();
    static void dumpActiveTrans();

    /* BUG-20862 ���� hash resize�� �ϱ� ���ؼ� �ٸ� Ʈ����ǵ��� ��� ��������
     * ���ϰ� �ؾ� �մϴ�.*/
    static void block( void * aTrans, UInt   aTryMicroSec, idBool *aSuccess );
    static void unblock(void);
    
    static void enableTransBegin();
    static void disableTransBegin();
    static idBool  existActiveTrans(smxTrans* aTrans);

    // active transaction list releated function.
    static inline void initATL();
    static inline void addAT( smxTrans *aTrans );
    static inline void removeAT( smxTrans  * aTrans );
    static inline void initATLAnPrepareLst();
    static  void addActiveTrans( void   * aTrans,
                                 smTID    aTID,
                                 UInt     aReplFlag,
                                 idBool   aIsBeginLog,
                                 smLSN  * aCurLSN );
    static  idBool isEmptyActiveTrans();

    // global transaction recovery releated function.
    static inline void initPreparedList();
    static inline void addPreparedList( smxTrans  * aTrans );
    static inline void removePreparedList( smxTrans  *aTrans );
    static IDE_RC setXAInfoAnAddPrepareLst( void    * aTrans,
                                            timeval   aTimeVal,
                                            ID_XID    aXID, /* BUG-18981 */
                                            smSCN   * aFstDskViewSCN );
    static void rebuildPrepareTransOldestSCN();

    static IDE_RC incRecCnt4InDoubtTrans( smTID aTransID,
                                          smOID aTableOID );
    static IDE_RC decRecCnt4InDoubtTrans( smTID aTransID,
                                          smOID aTableOID );
    static  UInt   getPrepareTransCount();
    static  void*  getFirstPreparedTrx();
    static  void*  getNxtPreparedTrx(void* aTrans);

    static IDE_RC  makeTransEnd( void*    aTrans,
                                 idBool   aIsCommit );

    static IDE_RC  insertUndoLSNs( smrUTransQueue  * aTransQueue );
    static IDE_RC  abortAllActiveTrans();
    static IDE_RC  setRowSCNForInDoubtTrans();

    static IDE_RC verifyIndex4ActiveTrans( idvSQL * aStatistics );

    static inline IDE_RC lock() {return mMutex.lock( NULL /* idvSQL* */ ); }
    static inline IDE_RC unlock() {return mMutex.unlock(); }


    //move from smlLockMgr by recfactoring.
    static IDE_RC waitForLock( void             * aTrans,
                               iduMutex         * aMutex,
                               ULong              aLockWaitMicroSec );

    static idBool isWaitForTransCase( void   * aTrans,
                                      smTID    aWaitTransID );

    static IDE_RC waitForTrans( void      * aTrans,
                                smTID       aWaitTransID,
                                ULong       aLockWaitTime );

    static SInt   getCurTransCnt();
    static void*  getTransByTID2Void( smTID aTID );

    static void   getDummySmmViewSCN( smSCN  * aSCN );
    static IDE_RC getDummySmmCommitSCN( void   * aTrans,
                                        idBool   aIsLegacyTrans,
                                        void   * aStatus );
    static void   setSmmCallbacks( );
    static void   unsetSmmCallbacks( );

    static UInt   getActiveTransCount();
    static UInt   getPreparedTransCnt() { return mPreparedTransCnt; }

    /* PROJ-1594 Volatile TBS */
    /* Volatile logging�� �ϱ� ���� �ʿ��� �Լ� */
    static svrLogEnv *getVolatileLogEnv( void *aTrans );

    /* BUG-19245: Transaction�� �ι� Free�Ǵ� ���� Detect�ϱ� ���� �߰��� */
    static void checkFreeTransList();

    /* PROJ-1704 MVCC renwal */
    static IDE_RC addTouchedPage( void      * aTrans,
                                  scSpaceID   aSpaceID,
                                  scPageID    aPageID,
                                  SShort      aCTSlotNum );

    static IDE_RC initializeMinSCNBuilder();
    static IDE_RC startupMinSCNBuilder();
    static IDE_RC shutdownMinSCNBuilder();

    static idBool existActiveTrans();
    /* BUG-42724 */
    static IDE_RC setOIDFlagForInDoubtTrans();

    static idBool isReplTrans( void * aTrans );
    static idBool isSameReplID( void * aTrans, smTID aWaitTransID );
    static idBool isTransForReplConflictResolution( smTID aWaitTransID );

private:
    //For Versioning
    static iduMutex           mMutex;

//For Member
public:
    
    static smxTransFreeList  *mArrTransFreeList;
    static smxTrans          *mArrTrans;
    static UInt               mTransCnt;
    static UInt               mTransFreeListCnt;
    static UInt               mCurAllocTransFreeList;
    static idBool             mEnabledTransBegin;
    static UInt               mSlotMask;
    //Transaction Active List
    static UInt               mActiveTransCnt;
    static smxTrans           mActiveTrans;
    //Prepared Transaction List for global transaction
    static UInt               mPreparedTransCnt;
    static smxTrans           mPreparedTrans;
    static smxMinSCNBuild     mMinSCNBuilder;

    static smxGetSmmViewSCNFunc   mGetSmmViewSCN;
    static smxGetSmmCommitSCNFunc mGetSmmCommitSCN;

    /* BUG-33873 TRANSACTION_TABLE_SIZE �� �����߾����� trc �α׿� ����� */
    static UInt               mTransTableFullCount;
};

inline smxTrans* smxTransMgr::getTransByTID(smTID aTransID)
{
    return mArrTrans + getSlotID(aTransID);
}

inline void* smxTransMgr::getTransByTID2Void(smTID aTransID)
{
    return (void*)(mArrTrans + getSlotID(aTransID));
}

inline smxTrans* smxTransMgr::getTransBySID(SInt aSlotN)
{
    return mArrTrans + aSlotN;
}

inline void* smxTransMgr::getTransBySID2Void(SInt aSlotN)
{
    return (void*)(mArrTrans + aSlotN);
}

inline idvSQL* smxTransMgr::getStatisticsBySID(SInt aSlotN)
{
    return mArrTrans[aSlotN].mStatistics;
}

IDE_RC smxTransMgr::freeTrans(smxTrans *aTrans)
{
    IDE_ASSERT(aTrans->mStatus == SMX_TX_END);

    IDE_TEST( aTrans->mTransFreeList->freeTrans( aTrans )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}
// active transaction list���� �Լ� ����.
// active transaction list�� circular double link list.
inline void smxTransMgr::initATL()
{
    mActiveTrans.mPrvAT = &mActiveTrans;
    mActiveTrans.mNxtAT = &mActiveTrans;
}
// active transaction list�� add.
inline void smxTransMgr::addAT(smxTrans *aTrans)
{
    aTrans->mPrvAT = &mActiveTrans;
    aTrans->mNxtAT = mActiveTrans.mNxtAT;

    mActiveTrans.mNxtAT->mPrvAT = aTrans;
    mActiveTrans.mNxtAT = aTrans;
    mActiveTransCnt++;
}
// active transaction list���� remove
inline void smxTransMgr::removeAT(smxTrans *aTrans)
{
    aTrans->mPrvAT->mNxtAT = aTrans->mNxtAT;
    aTrans->mNxtAT->mPrvAT = aTrans->mPrvAT;
    mActiveTransCnt--;
}

inline idBool smxTransMgr::isEmptyActiveTrans()
{
    if(mActiveTrans.mNxtAT == &mActiveTrans)
    {
        return ID_TRUE;
    }
    else
    {
        return ID_FALSE;
    }
}

// prepared transaction list���� �Լ� ����.
inline void smxTransMgr::initPreparedList()
{
    mPreparedTrans.mPrvPT = &mPreparedTrans;
    mPreparedTrans.mNxtPT = &mPreparedTrans;
}

inline void smxTransMgr::addPreparedList(smxTrans *aTrans)
{
    aTrans->mPrvPT = &mPreparedTrans;
    aTrans->mNxtPT = mPreparedTrans.mNxtPT;
    mPreparedTrans.mNxtPT->mPrvPT = aTrans;
    mPreparedTrans.mNxtPT = aTrans;
    mPreparedTransCnt++;
}

inline void smxTransMgr::removePreparedList(smxTrans *aTrans)
{
    aTrans->mPrvPT->mNxtPT = aTrans->mNxtPT;
    aTrans->mNxtPT->mPrvPT = aTrans->mPrvPT;
    mPreparedTransCnt--;
}

inline void* smxTransMgr::getFirstPreparedTrx()
{
    if (mPreparedTrans.mNxtPT == &mPreparedTrans )
    {
        return NULL;
    }
    else
    {
        return mPreparedTrans.mNxtPT;
    }
}

inline void smxTransMgr::initATLAnPrepareLst()
{
    initATL();
    initPreparedList();
}

inline SInt smxTransMgr::getCurTransCnt()
{
   return mTransCnt;
}

inline UInt smxTransMgr::getActiveTransCount()
{
    return mActiveTransCnt;
}

inline IDE_RC smxTransMgr::allocNestedTrans4LayerCall(void** aTrans)
{
    return smxTransMgr::allocNestedTrans((smxTrans**)aTrans);

}

inline idBool  smxTransMgr::isActiveBySID(SInt aSlotN)
{
    smxTrans *sCurTx;

    sCurTx = getTransBySID(aSlotN);

    return (sCurTx->mStatus != SMX_TX_END ? ID_TRUE : ID_FALSE);
}

inline smTID  smxTransMgr::getTIDBySID(SInt aSlotN)
{
    smxTrans *sCurTx;

    sCurTx = getTransBySID(aSlotN);

    return sCurTx->mTransID;
}

inline void smxTransMgr::setTransBlockedBySID(const SInt aSlot)
{
    smxTrans* sTrans    = getTransBySID(aSlot);
    sTrans->mStatus     = SMX_TX_BLOCKED;
    sTrans->mStatus4FT  = SMX_TX_BLOCKED;
}

inline void smxTransMgr::setTransBegunBySID(const SInt aSlot)
{
    smxTrans* sTrans    = getTransBySID(aSlot);
    sTrans->mStatus     = SMX_TX_BEGIN;
    sTrans->mStatus4FT  = SMX_TX_BEGIN;
}

inline smxStatus smxTransMgr::getTransStatus(const SInt aSlot)
{
    return getTransBySID(aSlot)->mStatus;
}

inline svrLogEnv *smxTransMgr::getVolatileLogEnv(void *aTrans)
{
    return ((smxTrans*)aTrans)->getVolatileLogEnv();
}

/* Description : Get MinViewSCN From MinSCNBuilder  */
inline void smxTransMgr::getSysMinDskViewSCN( smSCN* aSysMinDskViewSCN )
{
    mMinSCNBuilder.getMinDskViewSCN( aSysMinDskViewSCN );
}

/* Description : reset Interval of MinSCNBuilder  */
inline IDE_RC smxTransMgr::resetBuilderInterval()
{
    return mMinSCNBuilder.resetInterval();
}

// BUG-24885 wrong delayed stamping
// get the minimum fstDskViewSCN of all active transactions
inline void smxTransMgr::getSysMinDskFstViewSCN( smSCN* aSysMinDskFstViewSCN )
{
    mMinSCNBuilder.getMinDskFstViewSCN( aSysMinDskFstViewSCN );
}

// BUG-26881 �߸��� CTS stamping���� acces�� �� ���� row�� ������
// system�� active transaction�� �� �ּ��� oldestViewSCN(AgableSCN)�� ����
inline void smxTransMgr::getSysAgableSCN( smSCN* aSysAgableSCN )
{
    mMinSCNBuilder.getMinOldestFstViewSCN( aSysAgableSCN );
}

/* Description : update MinViewSCN of MinSCNBuilder right now*/
inline IDE_RC smxTransMgr::rebuildMinViewSCN( idvSQL  * aStatistics )
{
    return mMinSCNBuilder.resumeAndWait( aStatistics );
}

inline idBool smxTransMgr::isReplTrans( void * aTrans )
{
    return ((smxTrans*)aTrans)->isReplTrans();
}

inline idBool smxTransMgr::isSameReplID( void   * aTrans,
                                         smTID    aWaitTransID )
{
    smxTrans * sTrans = NULL;
    smxTrans * sWaitTrans = NULL;

    sTrans = (smxTrans *)aTrans;
    sWaitTrans = getTransByTID(aWaitTransID);

    if ( sTrans->mReplID == sWaitTrans->mReplID )
    {
        return ID_TRUE;
    }
    else
    {
        return ID_FALSE;
    }
}

inline idBool smxTransMgr::isTransForReplConflictResolution( smTID aWaitTransID )
{
    smxTrans * sWaitTrans = NULL;

    sWaitTrans = getTransByTID( aWaitTransID );

    if ( ( sWaitTrans->mFlag & SMI_TRANSACTION_REPL_CONFLICT_RESOLUTION )
         == SMI_TRANSACTION_REPL_CONFLICT_RESOLUTION )
    {
        return ID_TRUE;
    }
    else
    {
        return ID_FALSE;
    }
}

#endif
