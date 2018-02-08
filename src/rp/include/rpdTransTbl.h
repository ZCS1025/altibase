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
 * $Id: rpdTransTbl.h 82075 2018-01-17 06:39:52Z jina.kim $
 **********************************************************************/

#ifndef _O_RPD_TRANSTBL_H_
#define _O_RPD_TRANSTBL_H_ 1

#include <smiTrans.h>
#include <smDef.h>
#include <smrDef.h>
#include <rpDef.h>
#include <rpdLogAnalyzer.h>
#include <rprSNMapMgr.h>

#define RPD_TRANSTBL_USE_MASK      0x00000001
#define RPD_TRANSTBL_USE_RECEIVER  0x00000000
#define RPD_TRANSTBL_USE_SENDER    0x00000001

typedef struct rpdTrans
{
    smiTrans     mSmiTrans;
    iduListNode  mNode;
} rpdTrans;

class rpdTransEntry
{
public:
    rpdTrans      * mRpdTrans;
    rpdTrans      * mTransForConflictResolution;
    UInt            mStatus;
    idBool          mSetPSMSavepoint;
    rpdTransEntry * mNext;

    smTID  getTransID() { return mRpdTrans->mSmiTrans.getTransID(); }
    IDE_RC commit( smTID aTID );
    IDE_RC rollback( smTID aTID );
    IDE_RC setSavepoint( smTID aTID, rpSavepointType aType, SChar * aSavepointName );
    IDE_RC abortSavepoint( rpSavepointType aType, SChar * aSavepointName );
};

typedef struct rpdLOBLocEntry
{
    smLobLocator            mRemoteLL;
    smLobLocator            mLocalLL;
    struct rpdLOBLocEntry  *mNext;
} rpdLOBLocEntry;

typedef struct rpdItemMetaEntry
{
    smiTableMeta  mItemMeta;
    void         *mLogBody;

    iduListNode   mNode;
} rpdItemMetaEntry;

typedef struct rpdSavepointEntry
{
    smSN            mSN;
    rpSavepointType mType;
    SChar           mName[RP_SAVEPOINT_NAME_LEN + 1];

    iduListNode     mNode;
} rpdSavepointEntry;

typedef struct rpdTransTblNode
{
    SChar             *mRepName;        // Performace View ����
    RP_SENDER_TYPE     mCurrentType;    // START_FLAG
    smTID              mRemoteTID;
    smTID              mMyTID;          // Performace View ����
    idBool             mBeginFlag;
    idBool             mAbortFlag;
    idBool             mSkipLobLogFlag; /* BUG-21858 DML + LOB ó�� */
    idBool             mSendLobLogFlag; /* BUG-24398 LOB Cursor Open�� Table OID�� Ȯ�� */
    smSN               mBeginSN;
    smSN               mAbortSN;        /* Abort ���·� ���� XLog�� SN */
    UInt               mTxListIdx;      /* Abort/Clear Transaction List ���� ��ġ */
    rpdLogAnalyzer    *mLogAnlz;
    rpdTransEntry      mTrans;
    rpdLOBLocEntry     mLocHead;
    rprSNMapEntry     *mSNMapEntry;     //proj-1608 recovery from replication

    // PROJ-1442 Replication Online �� DDL ���
    idBool             mIsDDLTrans;
    iduList            mItemMetaList;

    // BUG-28206 ���ʿ��� Transaction Begin�� ����
    iduList            mSvpList;
    rpdSavepointEntry *mPSMSvp;
    idBool             mIsSvpListSent;

    SInt               mParallelID;
    idBool             mIsConflict;
} rpdTransTblNode;

class rpdTransTbl
{
/* Member Function */
public:
    rpdTransTbl();
    ~rpdTransTbl();

    /* �ʱ�ȭ ��ƾ */
    IDE_RC         initialize( SInt aFlag, UInt aTransactionPoolSize );
    /* �Ҹ��� */
    void           destroy();

    /* ���ο� Transaction�� ���� */
    IDE_RC insertTrans( iduMemAllocator * aAllocator,
                        smTID             aRemoteTID,
                        smSN              aBeginSN,
                        iduMemPool      * aChainedValuePool );

    /* Transaction�� ���� */
    void           removeTrans(smTID aRemoteTID);

    /* ��� Active Transaction�� Rollback��Ų��.*/
    void           rollbackAllATrans();

    /* Active Transaction�� �ִ°� */
    inline idBool  isThereATrans() 
    { 
        idBool      sRC = ID_FALSE;

        if ( idCore::acpAtomicGet32( &mATransCnt ) == 0 )
        {
            sRC = ID_FALSE;
        }
        else
        {
            sRC = ID_TRUE;
        }

        return sRC;
    }

    /* Begin�� Transaction�� SN�߿��� ���� ���� SN����
       return */
    void           getMinTransFirstSN( smSN * aSN );

    inline idBool  isReceiver() { return ((mFlag & RPD_TRANSTBL_USE_MASK) == RPD_TRANSTBL_USE_RECEIVER) ?
                                      ID_TRUE : ID_FALSE; }

    inline idBool    isATrans(smTID aRemoteTID);
    inline smiTrans* getSMITrans(smTID aRemoteTID);
    inline void      setBeginFlag(smTID aRemoteTID, idBool aIsBegin);
    inline idBool    getBeginFlag(smTID aRemoteTID);
    inline rpdLogAnalyzer* getLogAnalyzer(smTID aRemoteTID);
    inline rpdTransTblNode* getTransTblNodeArray() { return mTransTbl; }
    inline UInt      getTransTblSize() { return mTblSize; }
    inline idBool    isATransNode(rpdTransTblNode *aHashNode);
    inline rpdTransTblNode *getTrNode(smTID aTID) { return &mTransTbl[getTransSlotID(aTID)]; }
    /*PROJ-1541*/
    inline idBool    getAbortFlag(smTID aRemoteTID);
    inline smSN      getAbortSN(smTID aRemoteTID);
    inline void      setAbortInfo(smTID aRemoteTID, idBool aIsAbort, smSN aAbortSN);
    inline UInt      getTxListIdx(smTID aRemoteTID);
    inline void      setTxListIdx(smTID aRemoteTID, UInt aTxListIdx);
    /* BUG-21858 DML + LOB ó�� */
    inline idBool    getSkipLobLogFlag(smTID aRemoteTID);
    inline void      setSkipLobLogFlag(smTID aRemoteTID, idBool aSet);
    inline smSN      getBeginSN(smTID aRemoteTID);
    //BUG-24398
    inline idBool    getSendLobLogFlag(smTID aRemoteTID);
    inline void      setSendLobLogFlag(smTID aRemoteTID, idBool aSet);

    IDE_RC           insertLocator(smTID aTID, smLobLocator aRemoteLL, smLobLocator aLocalLL);
    void             removeLocator(smTID aTID, smLobLocator aRemoteLL);
    IDE_RC           searchLocator(smTID aTID, smLobLocator aRemoteLL, smLobLocator *aLocalLL, idBool *aIsFound);
    IDE_RC           getFirstLocator(smTID aTID, smLobLocator *aRemoteLL, smLobLocator *aLocalLL, idBool *aIsExist);
    void             setSNMapEntry(smTID aRemoteTID, rprSNMapEntry  *aSNMapEntry);
    rprSNMapEntry*   getSNMapEntry(smTID aRemoteTID);

    // PROJ-1442 Replication Online �� DDL ���
    inline void      setDDLTrans(smTID aTID) { mTransTbl[getTransSlotID(aTID)].mIsDDLTrans = ID_TRUE; }
    inline idBool    isDDLTrans(smTID aTID) { return mTransTbl[getTransSlotID(aTID)].mIsDDLTrans; }
    IDE_RC           addItemMetaEntry(smTID          aTID,
                                      smiTableMeta * aItemMeta,
                                      const void   * aItemMetaLogBody,
                                      UInt           aItemMetaLogBodySize);
    void             getFirstItemMetaEntry(smTID               aTID,
                                           rpdItemMetaEntry ** aItemMetaEntry);
    void             removeFirstItemMetaEntry(smTID aTID);
    idBool           existItemMeta(smTID aTID);

    // BUG-28206 ���ʿ��� Transaction Begin�� ����
    IDE_RC           addLastSvpEntry(smTID            aTID,
                                     smSN             aSN,
                                     rpSavepointType  aType,
                                     SChar           *aSvpName,
                                     UInt             aImplicitSvpDepth);
    void             applySvpAbort(smTID  aTID,
                                   SChar *aSvpName,
                                   smSN  *aSN);
    void             getFirstSvpEntry(smTID               aTID,
                                      rpdSavepointEntry **aSavepointEntry);
    void             removeSvpEntry(rpdSavepointEntry *aSavepointEntry);
    inline void      setSvpListSent(smTID aRemoteTID);
    inline idBool    isSvpListSent(smTID aRemoteTID);
    inline void      setIsConflictFlag( smTID aTID, idBool aFlag );
    inline idBool    getIsConflictFlag( smTID aTID );

    inline SInt      getATransCnt()
    {
        return idCore::acpAtomicGet32( &mATransCnt );
    }

    IDE_RC buildRecordForReplReceiverTransTbl( void                    * aHeader,
                                               void                    * /* aDumpObj */,
                                               iduFixedTableMemory     * aMemory,
                                               SChar                   * aRepName,
                                               UInt                      aParallelID,
                                               SInt                      aParallelApplyIndex );

    inline rpdTrans * getTransForConflictResolution( smTID aTID );
    inline smiTrans * getSmiTransForConflictResolution( smTID aTID );
    inline idBool isNullTransForConflictResolution( smTID aTID );
    IDE_RC allocConflictResolutionTransNode( smTID aTID );
    void removeTransNode( rpdTrans * aRpdTrans );
    idBool isSetPSMSavepoint( smTID    aTID );

/* Member Function */
private:
    inline void   initTransNode(rpdTransTblNode *aHashNode);
    inline UInt   getTransSlotID(smTID aRemoteTID) { return aRemoteTID % mTblSize; }

    // BUG-28206 ���ʿ��� Transaction Begin�� ����
    void          removeLastImplicitSvpEntries(iduList *aSvpList);

    /* BUG-35153 replication receiver performance enhancement 
     * by sm transaction pre-initialize.
     */
    IDE_RC        allocTransNode();
    void          destroyTransPool();
    IDE_RC        initTransPool();
    IDE_RC        getTransNode(rpdTrans** aRpdTrans);

/* Member Variable */
private:
    /* Replication Transaction������ ����
       Transaction Table */
    rpdTransTblNode     * mTransTbl;
    /* rpdTransTbl�� Replication�� Receiver����
       ����ϸ�
       RPD_TRANSTBL_USE_RECEIVER
       �ƴϸ�
       RPD_TRANSTBL_USE_SENDER
    */
    SInt                  mFlag;
    /* Active Transaction���� */
    volatile SInt         mATransCnt;
    /* Transactin Table Size */
    UInt                  mTblSize;
    /* LFG�� ���� */
    UInt                  mLFGCount;
    /* Last Commit SN */
    smSN                  mEndSN;

    // BUG-28206 ���ʿ��� Transaction Begin�� ����
    iduMemPool            mSvpPool;
    /* BUG-35153 replication receiver performance enhancement 
     * by sm transaction pre-initialize.
     */
    iduMemPool            mTransPool;
    iduList               mFreeTransList;
    UInt                  mOverAllocTransCount;

public:
    iduMutex              mAbortTxMutex;

};

/***********************************************************************
 * Description : aHashNode�� Active Transaction�� ����̸� ID_TRUE, �ƴϸ�
 *               ID_FALSE.
 *               BeginSN�� sender�� set�ϰ�
 *               �� �Լ��� sender thread�� ȣ���ϹǷ� mutex�� ���� ����
 *               receiver�� �ٸ� thread�� ���ÿ� BeginSN�� �������� ����
 * aHashNode  - [IN] �׽�Ʈ�� ������ rpdTransTblNode
 *
 **********************************************************************/
idBool rpdTransTbl::isATransNode(rpdTransTblNode *aHashNode)
{
    return (aHashNode->mBeginSN == SM_SN_NULL) ? ID_FALSE : ID_TRUE;
}

/***********************************************************************
 * Description : aRemoteTID�� ����Ű�� Transaction Slot�� Active Transaction��
 *               ����̸� ID_TRUE, �ƴϸ� ID_FALSE.
 *               BeginSN�� sender�� set�ϰ�
 *               �� �Լ��� sender thread�� ȣ���ϹǷ� mutex�� ���� ����
 *               receiver�� �ٸ� thread�� ���ÿ� BeginSN�� �������� ����
 * aRemoteTID  - [IN] �׽�Ʈ�� ������ Transaction ID
 *
 **********************************************************************/
inline idBool rpdTransTbl::isATrans(smTID aRemoteTID)
{
    SInt sIndex = getTransSlotID(aRemoteTID);
    return isATransNode( &(mTransTbl[sIndex]) );
}

/***********************************************************************
 * Description : aRemoteTID�� �ش��ϴ� Transaction Slot�� ã�Ƽ� smiTrans��
 *               return�Ѵ�.
 *
 * aRemoteTID  - [IN] ã�ƾ� �� Transaction ID
 *
 **********************************************************************/
inline smiTrans* rpdTransTbl::getSMITrans(smTID aRemoteTID)
{
    SInt sIndex = getTransSlotID(aRemoteTID);
    return &(mTransTbl[sIndex].mTrans.mRpdTrans->mSmiTrans);
}
/***********************************************************************
 * Description : aRemoteTID�� �ش��ϴ� Transaction Slot�� ã�Ƽ�
 *               mBeginFlag�� aIsBegin���� �Ѵ�.
 * aRemoteTID  - [IN] ã�ƾ� �� Transaction ID
 * aIsBegin    - [IN] Begin Flag Value.
 *
 **********************************************************************/
inline void rpdTransTbl::setBeginFlag(smTID aRemoteTID, idBool aIsBegin)
{
    SInt sIndex = getTransSlotID(aRemoteTID);
    mTransTbl[sIndex].mBeginFlag = aIsBegin;
}
/***********************************************************************
 * Description : aRemoteTID�� �ش��ϴ� Transaction Slot�� ã�Ƽ�
 *               Abort Transaction ������ �����Ѵ�.
 *               Sender Apply�� Service thread�� Sender Info�� ���� �����Ѵ�.
 * aRemoteTID  - [IN] ã�ƾ� �� Transaction ID
 * aIsAbort    - [IN] Abort Flag Value
 * aAbortSN    - [IN] Abort SN Value
 **********************************************************************/
inline void rpdTransTbl::setAbortInfo(smTID  aRemoteTID,
                                      idBool aIsAbort,
                                      smSN   aAbortSN)
{
    SInt sIndex = getTransSlotID(aRemoteTID);
    mTransTbl[sIndex].mAbortFlag = aIsAbort;
    mTransTbl[sIndex].mAbortSN   = aAbortSN;
}
/***********************************************************************
 * Description : aRemoteTID�� �ش��ϴ� Transaction Slot�� ã�Ƽ�
 *               mAbortFlag�� Set�Ǿ� �ִ��� ��ȯ�Ѵ�.
 * aRemoteTID  - [IN] ã�ƾ� �� Transaction ID
 **********************************************************************/
inline idBool rpdTransTbl::getAbortFlag(smTID aRemoteTID)
{
    SInt sIndex = getTransSlotID(aRemoteTID);
    return mTransTbl[sIndex].mAbortFlag;
}
/***********************************************************************
 * Description : aRemoteTID�� �ش��ϴ� Transaction Slot�� ã�Ƽ�
 *               mAbortSN�� ��ȯ�Ѵ�.
 * aRemoteTID  - [IN] ã�ƾ� �� Transaction ID
 *
 **********************************************************************/
inline smSN rpdTransTbl::getAbortSN(smTID aRemoteTID)
{
    SInt sIndex = getTransSlotID(aRemoteTID);
    return mTransTbl[sIndex].mAbortSN;
}
/***********************************************************************
 * Description : aRemoteTID�� �ش��ϴ� Transaction Slot�� ã�Ƽ�
 *               Abort/Clear Tx List ���� ��ġ�� �����Ѵ�.
 * aRemoteTID  - [IN] ã�ƾ� �� Transaction ID
 * aTxListIdx  - [IN] Abort/Clear Tx List ���� ��ġ
 **********************************************************************/
inline void rpdTransTbl::setTxListIdx(smTID aRemoteTID, UInt aTxListIdx)
{
    SInt sIndex = getTransSlotID(aRemoteTID);
    mTransTbl[sIndex].mTxListIdx = aTxListIdx;
}
/***********************************************************************
 * Description : aRemoteTID�� �ش��ϴ� Transaction Slot�� ã�Ƽ�
 *               Abort/Clear Tx List ���� ��ġ�� ��ȯ�Ѵ�.
 * aRemoteTID  - [IN] ã�ƾ� �� Transaction ID
 **********************************************************************/
inline UInt rpdTransTbl::getTxListIdx(smTID aRemoteTID)
{
    SInt sIndex = getTransSlotID(aRemoteTID);
    return mTransTbl[sIndex].mTxListIdx;
}

/* BUG-21858 DML + LOB ó�� */
inline void rpdTransTbl::setSkipLobLogFlag(smTID aRemoteTID, idBool aSet)
{
    SInt sIndex = getTransSlotID(aRemoteTID);
    mTransTbl[sIndex].mSkipLobLogFlag = aSet;
}
inline idBool rpdTransTbl::getSkipLobLogFlag(smTID aRemoteTID)
{
    SInt sIndex = getTransSlotID(aRemoteTID);
    return mTransTbl[sIndex].mSkipLobLogFlag;
}

/***********************************************************************
 * Description : aRemoteTID�� �ش��ϴ� Transaction Slot�� ã�Ƽ�
 *               mBeginFlag�� return �Ѵ�.
 * aRemoteTID  - [IN] ã�ƾ� �� Transaction ID
 *
 **********************************************************************/
inline idBool rpdTransTbl::getBeginFlag(smTID aRemoteTID)
{
    SInt sIndex = getTransSlotID(aRemoteTID);
    return mTransTbl[sIndex].mBeginFlag;
}

/***********************************************************************
 * Description : aRemoteTID�� �ش��ϴ� Transaction Slot�� ã�Ƽ�
 *               mLogAnlz�� return �Ѵ�.
 * aRemoteTID  - [IN] ã�ƾ� �� Transaction ID
 *
 **********************************************************************/
inline rpdLogAnalyzer* rpdTransTbl::getLogAnalyzer(smTID aRemoteTID)
{
    SInt sIndex = getTransSlotID(aRemoteTID);
    return mTransTbl[sIndex].mLogAnlz;
}

/***********************************************************************
 * Description : aRemoteTID�� �ش��ϴ� Transaction Slot�� ã�Ƽ�
 *               mBeginSN�� return �Ѵ�.
 * aRemoteTID  - [IN] ã�ƾ� �� Transaction ID
 *
 **********************************************************************/
inline smSN rpdTransTbl::getBeginSN(smTID aRemoteTID)
{
    SInt sIndex = getTransSlotID(aRemoteTID);
    return mTransTbl[sIndex].mBeginSN;
}
/***********************************************************************
 * Description : aRemoteTID�� �ش��ϴ� Transaction Slot�� ã�Ƽ�
 *               mSNMapEntry�� Set �Ѵ�.
 * aRemoteTID  - [IN] ã�ƾ� �� Transaction ID
 * aIsAbort    - [IN] aSNMapEntry.
 **********************************************************************/
inline void rpdTransTbl::setSNMapEntry(smTID aRemoteTID, rprSNMapEntry  *aSNMapEntry)
{
    SInt sIndex = getTransSlotID(aRemoteTID);
    mTransTbl[sIndex].mSNMapEntry = aSNMapEntry;
}
inline rprSNMapEntry* rpdTransTbl::getSNMapEntry(smTID aRemoteTID)
{
    SInt sIndex = getTransSlotID(aRemoteTID);
    return mTransTbl[sIndex].mSNMapEntry;
}

//BUG-24398
inline void rpdTransTbl::setSendLobLogFlag(smTID aRemoteTID, idBool aSet)
{
    SInt sIndex = getTransSlotID(aRemoteTID);
    mTransTbl[sIndex].mSendLobLogFlag = aSet;
}
inline idBool rpdTransTbl::getSendLobLogFlag(smTID aRemoteTID)
{
    SInt sIndex = getTransSlotID(aRemoteTID);
    return mTransTbl[sIndex].mSendLobLogFlag;
}

/***********************************************************************
 * Description : aRemoteTID�� �ش��ϴ� Transaction Slot�� ã�Ƽ�
 *               mIsSvpListSent�� ID_TRUE�� �����Ѵ�.
 * aRemoteTID  - [IN] ã�ƾ� �� Transaction ID
 *
 **********************************************************************/
inline void rpdTransTbl::setSvpListSent(smTID aRemoteTID)
{
    SInt sIndex = getTransSlotID(aRemoteTID);
    mTransTbl[sIndex].mIsSvpListSent = ID_TRUE;
}

/***********************************************************************
 * Description : aRemoteTID�� �ش��ϴ� Transaction Slot�� ã�Ƽ�
 *               mIsSvpListSent�� ��ȯ�Ѵ�.
 * aRemoteTID  - [IN] ã�ƾ� �� Transaction ID
 *
 **********************************************************************/
inline idBool rpdTransTbl::isSvpListSent(smTID aRemoteTID)
{
    SInt sIndex = getTransSlotID(aRemoteTID);
    return mTransTbl[sIndex].mIsSvpListSent;
}
inline void rpdTransTbl::setIsConflictFlag( smTID aTID, idBool aFlag )
{
    SInt sIndex = getTransSlotID( aTID );
    mTransTbl[sIndex].mIsConflict = aFlag;
}
inline idBool rpdTransTbl::getIsConflictFlag( smTID aTID )
{
    SInt sIndex = getTransSlotID( aTID );
    return mTransTbl[sIndex].mIsConflict;
}

inline rpdTrans * rpdTransTbl::getTransForConflictResolution( smTID aTID )
{
    SInt sIndex = getTransSlotID( aTID );
    return mTransTbl[sIndex].mTrans.mTransForConflictResolution;
}

inline smiTrans * rpdTransTbl::getSmiTransForConflictResolution( smTID aTID )
{
    rpdTrans * sTrans = NULL;

    sTrans =  getTransForConflictResolution( aTID );

    return ( sTrans == NULL ) ? NULL : &(sTrans->mSmiTrans);
}

inline idBool rpdTransTbl::isNullTransForConflictResolution( smTID aTID )
{
    rpdTrans * sTrans = getTransForConflictResolution( aTID );

    return ( sTrans == NULL ) ? ID_TRUE : ID_FALSE;
}

inline idBool rpdTransTbl::isSetPSMSavepoint( smTID    aTID )
{
    SInt    sIndex = getTransSlotID( aTID );

    return mTransTbl[sIndex].mTrans.mSetPSMSavepoint;
}

#endif /* _O_RPD_TRANSTBL_H_ */

