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
 * $Id: smxTrans.h 82075 2018-01-17 06:39:52Z jina.kim $
 **********************************************************************/

#ifndef _O_SMX_TRANS_H_
#define _O_SMX_TRANS_H_ 1

#include <ide.h>
#include <idu.h>
#include <idv.h>
#include <iduCompression.h>
#include <idl.h>
#include <iduMemoryHandle.h>
#include <iduReusedMemoryHandle.h>
#include <smErrorCode.h>
#include <smuProperty.h>
#include <sdc.h>
#include <smxOIDList.h>
#include <smxTouchPageList.h>
#include <smxSavepointMgr.h>
#include <smxTableInfoMgr.h>
#include <smxLCL.h>
#include <svrLogMgr.h>
#include <smr.h>
#include <smiTrans.h>

class  smxTransFreeList;
class  smrLogFile;
struct sdrMtxStartInfo;
class  smrLogMgr;

/* BUG-24151: [SC] Update Retry, Delete Retry, Statement Rebuild Count��
 *            AWI�� �߰��ؾ� �մϴ�.
 *
 * Transaction�� Statistics�� aValue�� �ش��ϴ� State������ ���� ������
 * Ų��. */
#define SMX_INC_SESSION_STATISTIC( aTrans, aSessStatistic, aValue ) \
    if( aTrans != NULL )                                            \
    {                                                               \
        if( ((smxTrans*)aTrans)->mStatistics != NULL )              \
        {                                                           \
             IDV_SESS_ADD( ((smxTrans*)aTrans)->mStatistics->mSess, \
                           aSessStatistic,                          \
                           aValue );                                \
        }                                                           \
    }

/*
 * BUG-39825
 * Infinite SCN �� Lock Row SCN �Ѵ� �ֻ��� bit �� 1 �Դϴ�.
 * �׷��� SM_SCN_IS_INFINITE üũ�����δ� �� ���̸� ���� �� �� �����ϴ�.
 * Transaction SCN �� �������� ����, Lock Row �� ���� bit setting ���� �ƴ���
 * üũ�� ��� �մϴ�.
 * */
#define SMX_GET_SCN_AND_TID( aSrcSCN, aDestSCN, aDestTID ) {             \
    SM_GET_SCN( &(aDestSCN), &(aSrcSCN) );                               \
    if ( SM_SCN_IS_INFINITE( aDestSCN ) )                                \
    {                                                                    \
        aDestTID = SMP_GET_TID( (aDestSCN) );                            \
        if ( ! SM_SCN_IS_LOCK_ROW( aDestSCN ) )                          \
        {                                                                \
            smxTrans::getTransCommitSCN( (aDestTID),                     \
                                         &(aSrcSCN),                     \
                                         &(aDestSCN) );                  \
        }                                                                \
    }                                                                    \
    else                                                                 \
    {                                                                    \
        aDestTID = SM_NULL_TID;                                          \
    }                                                                    \
}

class smxTrans
{
// For Operation
public:

    static IDE_RC initializeStatic();
    static IDE_RC destroyStatic();

    IDE_RC initialize(smTID aTransID, UInt aSlotMask);
    IDE_RC destroy();


    //Transaction Management
    inline IDE_RC lock();
    inline IDE_RC unlock();

    inline IDE_RC lockCursorList();
    inline IDE_RC unlockCursorList();

    // For Local & Global Transaction
    inline void initXID();
    //Replication Mode(default/eager/lazy/acked/none)PROJ-1541: Eager Replication

    IDE_RC begin(idvSQL * aStatistics,
                 UInt     aFlag,
                 UInt     aReplID );

    IDE_RC abort();
    //PROJ-1677 DEQ.
    IDE_RC commit( smSCN*  aCommitSCN,
                   idBool  aIsLegacyTrans = ID_FALSE,
                   void**  aLegacyTrans   = NULL );

    // For Layer callback.
    static IDE_RC begin4LayerCall( void* aTrans, UInt aFlag, idvSQL* aStatistics );
    static IDE_RC abort4LayerCall( void* aTrans );
    static IDE_RC commit4LayerCall( void* aTrans );
    static void  updateSkipCheckSCN( void* aTrans, idBool aDoSkipCheckSCN );

    // To know Log Flush Is Need
    static idBool isNeedLogFlushAtCommitAPrepare(void *aTrans);
    inline idBool isNeedLogFlushAtCommitAPrepareInternal();

    // For Global Transaction
    /* BUG-18981 */
    IDE_RC prepare(ID_XID *aXID);
    //IDE_RC forget(XID *a_pXID, idBool a_bIsRecovery = ID_FALSE);
    IDE_RC getXID(ID_XID *aXID);

    inline idBool isActive();
    inline idBool isReadOnly();
    inline idBool isVolatileTBSTouched();

    // callback���� ���� isReadOnly �Լ�
    static idBool isReadOnly4Callback(void *aTrans);

    inline idBool isPrepared();
    inline idBool isValidXID();

    inline IDE_RC suspend(smxTrans*, smTID, iduMutex*, ULong);

    /* PROJ-2620 suspend in lock manager spin mode */
    IDE_RC suspendMutex(smxTrans *  aWaitTrans,
                        iduMutex *  aMutex,
                        ULong       aWaitMicroSec);
    IDE_RC suspendSpin(smxTrans *   aWaitTrans,
                       smTID        aWaitTransID,
                       ULong        aWaitMicroSec);



    IDE_RC resume();

    inline IDE_RC init();
    inline void   init(smTID aTransID);

    inline smSCN getInfiniteSCN();

    IDE_RC incInfiniteSCNAndGet(smSCN *aSCN);

    inline smSCN getMinMemViewSCN() { return mMinMemViewSCN; }

    inline smSCN getMinDskViewSCN() { return mMinDskViewSCN; }

    inline void  setMinViewSCN( UInt   aCursorFlag,
                                smSCN* aMemViewSCN,
                                smSCN* aDskViewSCN );

    inline void  initMinViewSCN( UInt   aCursorFlag );

    // Lock Slot
    inline SInt getSlotID() { return mSlotN; }

    //Set Last LSN
    IDE_RC setLstUndoNxtLSN( smLSN aLSN );
    smLSN  getLstUndoNxtLSN();

    //For post commit operation
    inline IDE_RC addOID( smOID            aTableOID,
                          smOID            aRecordOID,
                          scSpaceID        aSpaceID,
                          UInt             aFlag );

    // freeSlot�ϴ� OID�� �߰��Ѵ�.
    inline IDE_RC addFreeSlotOID( smOID            aTableOID,
                                  smOID            aRecordOID,
                                  scSpaceID        aSpaceID,
                                  UInt             aFlag );

    IDE_RC freeOIDList();
    void   showOIDList();

    smxOIDNode* getCurNodeOfNVL();

    //For save point
    IDE_RC setImpSavepoint(smxSavepoint **aSavepoint,
                           UInt           aStmtDepth);

    IDE_RC unsetImpSavepoint(smxSavepoint *aSavepoint);

    inline IDE_RC abortToImpSavepoint(smxSavepoint *aSavepoint);

    IDE_RC setExpSavepoint(const SChar *aExpSVPName);

    inline IDE_RC abortToExpSavepoint(const SChar *aExpSVPName);

    /* BUG-32292 [sm-util] Self deadlock occur since fixed-table building
     * operation uses another transaction. 
     * LayerCallback�� ���� Wrapping �Լ�*/
    static IDE_RC setImpSavepoint4LayerCall( void          * aTrans,
                                             void         ** aSavepoint,
                                             UInt            aStmtDepth)
    {
        return ((smxTrans*)aTrans)->setImpSavepoint( 
                    (smxSavepoint**) aSavepoint, 
                    aStmtDepth );
    }
    static IDE_RC setImpSavepoint4Retry( void          * aTrans,
                                         void         ** aSavepoint )
    {
        return ((smxTrans*)aTrans)->setImpSavepoint(
                    (smxSavepoint**) aSavepoint,
                    ((smxTrans*)aTrans)->mSvpMgr.getStmtDepthFromImpSvpList() + 1 );
    }

    static IDE_RC unsetImpSavepoint4LayerCall( void         * aTrans,
                                               void         * aSavepoint)
    {
        return ((smxTrans*)aTrans)->unsetImpSavepoint( 
                (smxSavepoint*) aSavepoint );
    }

    static IDE_RC abortToImpSavepoint4LayerCall( void         * aTrans,
                                                 void         * aSavepoint)
    {
        return ((smxTrans*)aTrans)->abortToImpSavepoint( 
                (smxSavepoint*) aSavepoint );
    }

    IDE_RC addOIDList2LogicalAger(smLSN        *aLSN,
                                  SInt          aAgingState,
                                  void        **aAgerNormalList);
    IDE_RC end();

    IDE_RC removeAllAndReleaseMutex();


    //For Session Management
    UInt   getFstUpdateTime();

    inline SChar* getLogBuffer(){return mLogBuffer;};

    void   initLogBuffer();

    IDE_RC writeLogToBufferUsingDynArr( smuDynArrayBase  * aLogBuffer,
                                        UInt               aLogSize );

    IDE_RC writeLogToBufferUsingDynArr( smuDynArrayBase  * aLogBuffer,
                                        UInt               aOffset,
                                        UInt               aLogSize );

    IDE_RC writeLogToBuffer(const void *aLog,
                            UInt        aLogSize);

    IDE_RC writeLogToBuffer( const void   * aLog,
                             UInt           aOffset,
                             UInt           aLogSize );
    inline smLSN getBeginLSN(){return mBeginLogLSN;};
    inline smLSN getCommitLSN(){return mCommitLogLSN;};

    static IDE_RC allocTXSegEntry( idvSQL          * aStatistics,
                                   sdrMtxStartInfo * aStartInfo );

    static inline sdcUndoSegment * getUDSegPtr( smxTrans * aTrans );

    static inline UInt getLegacyTransCnt(void * aTrans)
                             { return ((smxTrans*)aTrans)->mLegacyTransCnt; };

    inline sdcTXSegEntry * getTXSegEntry();
    inline void setTXSegEntry( sdcTXSegEntry * aTXSegEntry );

    inline static sdSID getTSSlotSID( void * aTrans );

    inline void  setTSSAllocPos( sdRID aLstExtRID,
                                 sdSID aTSSlotSID );
    inline void setUndoCurPos( sdSID    aUndoRecSID,
                               sdRID    aUndoExtRID );
    inline void getUndoCurPos( sdSID  * aUndoRecSID,
                               sdRID  * aUndoExtRID );


    // Ư�� Transaction�� �α� ������ ������ �α����Ͽ� ����Ѵ�.
    static IDE_RC writeTransLog(void *aTrans );

    inline static smSCN  getFstDskViewSCN( void * aTrans );
    static void   setFstDskViewSCN( void  * aTrans,
                                    smSCN * aFstDskViewSCN );

    // BUG-26881 �߸��� CTS stamping���� acces�� �� ���� row�� ������
    static smSCN  getOldestFstViewSCN( void * aTrans );

    // api member function list
    static void     getTransInfo( void    * aTrans, 
                                  SChar  ** aTransLogBuffer,
                                  smTID   * aTID, 
                                  UInt    * aTransLogType );
    static smLSN    getTransLstUndoNxtLSN( void  * aTrans );
    /* Transaction�� ���� UndoNxtLSN�� return */
    static smLSN    getTransCurUndoNxtLSN( void  * aTrans );
    /* Transaction�� ���� UndoNxtLSN�� Set */
     static void    setTransCurUndoNxtLSN( void  * aTrans, smLSN  * aLSN );
    /* Transaction�� ������ UndoNxtLSN�� return */
    static smLSN*   getTransLstUndoNxtLSNPtr( void  * aTrans );
    /* Transaction�� ������ UndoNxtLSN�� set */
    static IDE_RC   setTransLstUndoNxtLSN( void  * aTrans,
                                           smLSN   aLSN );
    static void     getTxIDAnLogType( void  * aTrans, 
                                      smTID * aTID,
                                      UInt  * aLogType );

#ifdef PROJ_2181_DBG
    ULong    getTxDebugID() { return    mTransIDDBG;} 

    ULong    mTransIDDBG;   
#endif


    static idBool   getTransAbleToRollback(void* aTrans);

    static idBool   isTxBeginStatus(void * aTrans);

    static IDE_RC   addOIDToVerify( void    * aTrans,
                                    smOID     aTableOID,
                                    smOID     aIndexOID,
                                    scSpaceID aSpaceID );
    
    static inline IDE_RC addOID2Trans( void    * aTrans,
                                       smOID     aTblOID,
                                       smOID     aRecOID,
                                       scSpaceID aSpaceID,
                                       UInt      aFlag );

    static IDE_RC   addOIDByTID( smTID     aTID,
                                 smOID     aTblOID,
                                 smOID     aRecordOID,
                                 scSpaceID aSpaceID,
                                 UInt      aFlag );

    static idBool   isXAPreparedCommitState(void* aTrans);

    static inline smTID getTransID( const void * aTrans );

    static void *   getTransLogFilePtr(void* aTrans);

    static void     setTransLogFilePtr(void* aTrans, void* aLogFile);

    static void     addMutexToTrans (void * aTrans, void * aMutex);

    static inline IDE_RC writeLogToBufferOfTx( void         * aTrans,
                                               const void   * aLog,
                                               UInt           aOffset,
                                               UInt           aLogSize );

    static IDE_RC   writeLogBufferWithOutOffset(void *aTrans,
                                                const void *aLog,
                                                UInt aLogSize);

    static inline void initTransLogBuffer( void * aTrans );

    static SChar *  getTransLogBuffer (void *aTrans);

    static UInt     getLogTypeFlagOfTrans(void * aTrans);

    static void     addToUpdateSizeOfTrans (void *aTrans, UInt aSize);


    static idvSQL * getStatistics( void * aTrans );
    static inline SInt     getTransSlot( void* aTrans );
    static IDE_RC   resumeTrans(void * aTrans);

    static void     getTransCommitSCN( smTID         aRecTID,
                                       const smSCN * aRecSCN,
                                       smSCN       * aOutSCN );

    static void     setTransCommitSCN(void      *aTrans,
                                      smSCN      aSCN,
                                      void      *aStatus);

    static void trySetupMinMemViewSCN( void  * aTrans,
                                       smSCN * aViewSCN );

    static void trySetupMinDskViewSCN( void  * aTrans,
                                       smSCN * aViewSCN );

    static void trySetupMinAllViewSCN( void  * aTrans,
                                       smSCN * aViewSCN );

    IDE_RC allocViewSCN( UInt  aStmtFlag, smSCN * aSCN );

    // �α��� ���������� Sync�Ѵ�.
    static IDE_RC syncToEnd();


    IDE_RC executePendingList(idBool aIsCommit );
    inline idBool hasPendingOp();
    static void addPendingOperation( void  * aTrans, smuList  * aRemoveDBF );

    static idBool  getIsFirstLog( void* aTrans )
        {
            return ((smxTrans*)aTrans)->mIsFirstLog;
        };
    static void setIsFirstLog( void* aTrans, idBool aFlag )
        {
            ((smxTrans*)aTrans)->mIsFirstLog = aFlag;
        };
    static idBool checkAndSetImplSVPStmtDepth4Repl(void* aTrans);

    static void setIsTransWaitRepl( void* aTrans, idBool aIsWaitRepl )
    {
        ((smxTrans*)aTrans)->mIsTransWaitRepl = aIsWaitRepl;
    };
    idBool isReplTrans()
    {
          if ( mReplID != SMX_NOT_REPL_TX_ID )
          {
              return ID_TRUE;
          }

          return ID_FALSE; 
    }
    /*
     * BUG-33539
     * receiver���� lock escalation�� �߻��ϸ� receiver�� self deadlock ���°� �˴ϴ�
     */
    ULong getLockTimeoutByUSec( ULong aLockWaitMicroSec );
    ULong getLockTimeoutByUSec( );

    IDE_RC setReplLockTimeout( UInt aReplLockTimeout );
    UInt getReplLockTimeout();

    // For BUG-12512
    static idBool isPsmSvpReserved(void* aTrans)
        {
            return ((smxTrans*)aTrans)->mSvpMgr.isPsmSvpReserved();
        };
    void reservePsmSvp( );
    static IDE_RC writePsmSvp(void* aTrans)
        {
            return ((smxTrans*)aTrans)->mSvpMgr.writePsmSvp((smxTrans*)aTrans);
        };
    void clearPsmSvp( )
        {
            mSvpMgr.clearPsmSvp();
        };
    IDE_RC abortToPsmSvp( )
        {
            return mSvpMgr.abortToPsmSvp(this);
        };

    // PRJ-1496
    inline IDE_RC getTableInfo(smOID          aTableOID,
                               smxTableInfo** aTableInfo,
                               idBool         aIsSearch = ID_FALSE);

    inline IDE_RC undoDelete(smOID aTableOID);
    inline IDE_RC undoInsert(smOID aTableOID);
    inline IDE_RC getRecordCountFromTableInfo(smOID aOIDTable,
                                              SLong* aRecordCnt);
    inline static IDE_RC incRecordCountOfTableInfo( void  * aTrans,
                                                    smOID   aTableOID,
                                                    SLong   aRecordCnt );

    static IDE_RC undoDeleteOfTableInfo(void* aTrans, smOID aTableOID);
    static IDE_RC undoInsertOfTableInfo(void* aTrans, smOID aTableOID);

    inline static IDE_RC setExistDPathIns( void    * aTrans,
                                           smOID     aTableOID,
                                           idBool    aExistDPathIns );

    /* TableInfo�� �˻��Ͽ� HintDataPID�� �����Ѵ�.. */
    static void setHintDataPIDofTableInfo( void       *aTableInfo,
                                           scPageID    aHintDataPID );

    /* TableInfo�� �˻��Ͽ� HintDataPID�� ��ȯ�Ѵ�. */
    static void  getHintDataPIDofTableInfo( void       *aTableInfo,
                                            scPageID  * aHintDataPID );

    // Ư�� Transaction�� RSID�� �ο��Ѵ�.
    static void allocRSGroupID(void             *aTrans,
                               UInt             *aPageListIdx );

    // Ư�� Transaction�� RSID�� �����´�.
    static UInt getRSGroupID(void* aTrans);

    // Ư�� Transaction�� RSID��  aIdx�� �ٲ۴�.
    static void setRSGroupID(void* aTrans, UInt aIdx);

    // Tx's PrivatePageList�� HashTable�� Hash Function
    inline static UInt hash(void* aKeyPtr);

    // Tx's PrivatePageList�� HashTable�� Ű�� �� �Լ�
    inline static SInt isEQ( void *aLhs, void *aRhs );

    // Tx's PrivatePageList�� ��ȯ�Ѵ�.
    static IDE_RC findPrivatePageList(
        void* aTrans,
        smOID aTableOID,
        smpPrivatePageListEntry** aPrivatePageList);

    // PrivatePageList�� �߰��Ѵ�.
    static IDE_RC addPrivatePageList(
        void* aTrans,
        smOID aTableOID,
        smpPrivatePageListEntry* aPrivatePageList);

    // PrivatePageList�� �����Ѵ�.
    static IDE_RC createPrivatePageList(
        void*                     aTrans,
        smOID                     aTableOID,
        smpPrivatePageListEntry** aPrivatePageList );

    // Tx's PrivatePageList ����
    IDE_RC finAndInitPrivatePageList();

    //==========================================================
    // PROJ-1594 Volatile TBS�� ���� �߰��� �Լ���
    // Tx's PrivatePageList�� ��ȯ�Ѵ�.
    static IDE_RC findVolPrivatePageList(
        void* aTrans,
        smOID aTableOID,
        smpPrivatePageListEntry** aPrivatePageList);

    // PrivatePageList�� �߰��Ѵ�.
    static IDE_RC addVolPrivatePageList(
        void* aTrans,
        smOID aTableOID,
        smpPrivatePageListEntry* aPrivatePageList);

    // PrivatePageList�� �����Ѵ�.
    static IDE_RC createVolPrivatePageList(
        void*                     aTrans,
        smOID                     aTableOID,
        smpPrivatePageListEntry** aPrivatePageList );

    // Tx's PrivatePageList ����
    IDE_RC finAndInitVolPrivatePageList();
    //==========================================================

    /* BUG-30871 When excuting ALTER TABLE in MRDB, the Private Page Lists of
     * new and old table are registered twice. */
    /* PrivatePageList�� ������ϴ�.
     * �� �������� �̹� �ش� Page���� TableSpace�� ��ȯ�� �����̱� ������
     * Page�� ���� �ʽ��ϴ�. */
    static IDE_RC dropMemAndVolPrivatePageList( void           * aTrans,
                                                smcTableHeader * aSrcHeader );

    // Commit�̳� Abort�Ŀ� FreeSlot�� ���� FreeSlotList�� �Ŵܴ�.
    IDE_RC addFreeSlotPending();

    // PROJ-1362 QP Large Record & Internal LOB
    // memory lob cursor-open
    IDE_RC  openLobCursor(idvSQL*          aStatistics,
                          void*            aTable,
                          smiLobCursorMode aOpenMode,
                          smSCN            aLobViewSCN,
                          smSCN            aInfinite,
                          void*            aRow,
                          const smiColumn* aColumn,
                          UInt             aInfo,
                          smLobLocator*    aLobLocator);

    // disk lob cursor-open
    IDE_RC  openLobCursor(idvSQL*            aStatistics,
                          void*              aTable,
                          smiLobCursorMode   aOpenMode,
                          smSCN              aLobViewSCN,
                          smSCN              aInfinite4Disk,
                          scGRID             aRowGRID,
                          smiColumn*         aColumn,
                          UInt               aInfo,
                          smLobLocator*      aLobLocator);

    // close lob cursor
    IDE_RC closeLobCursor(smLobCursorID aLobCursorID);
    IDE_RC closeLobCursorInternal(smLobCursor * aLobCursor);

    IDE_RC getLobCursor(smLobCursorID aLobCursorID,
                        smLobCursor** aLobCursor );
    static void updateDiskLobCursors(void* aTrans, smLobViewEnv* aLobViewEnv);

    // PROJ-2068 Direct-Path INSERT ���� �� �ʿ��� DPathEntry�� �Ҵ�
    inline static IDE_RC allocDPathEntry( smxTrans* aTrans );
    inline static void* getDPathEntry( void* aTrans );

    // BUG-24821 V$TRANSACTION�� LOB���� MinSCN�� ��µǾ�� �մϴ�.
    inline void getMinMemViewSCN4LOB( smSCN* aSCN );
    inline void getMinDskViewSCN4LOB( smSCN* aSCN );

    static UInt mAllocRSIdx;

    static UInt getMemLobCursorCnt(void   *aTrans, UInt aColumnID, void *aRow);

    /* Implicit Savepoint���� IS Lock���� �����Ѵ�.*/
    inline IDE_RC unlockSeveralLock(ULong aLockSequence);

    /* PROJ-1594 Volatile TBS */
    /* Volatile logging�� �ϱ� ���� �ʿ��� environment�� ��´�. */
    inline svrLogEnv *getVolatileLogEnv();

    /* BUG-15396
       Commit ��, log�� disk�� ����Ҷ� ����Ҷ����� ��ٷ��� ����
       ������ ���� ������ �����´�.
    */
    inline idBool isCommitWriteWait();

    static inline UInt getLstReplStmtDepth( void * aTrans );

    // PROJ-1566
    static void setExistAppendInsertInTrans( void * aTrans );

    static void setFreeInsUndoSegFlag( void * aTrans, idBool aFlag );

    /* TASK-2398 �α׾���
       Ʈ������� �α� ����/���������� ����� ���ҽ��� �����Ѵ� */
    IDE_RC getCompRes(smrCompRes ** aCompRes);

    // Ʈ������� �α� ����/���������� ����� ���ҽ��� �����Ѵ�
    // (callback��)
    static IDE_RC getCompRes4Callback( void *aTrans, smrCompRes ** aCompRes );

    /* TASK-2401 MMAP Loggingȯ�濡�� Disk/Memory Log�� �и�
       �ش� Ʈ������� Disk, Memory Tablespace�� ������ ��� ȣ���
     */
    void setDiskTBSAccessed();
    void setMemoryTBSAccessed();
    // �ش� Ʈ������� Meta Table�� ������ ��� ȣ���
    void setMetaTableModified();

    static void setMemoryTBSAccessed4Callback(void * aTrans);

    static void setXaSegsInfo( void    * aTrans,
                               UInt      aTXSegEntryIdx,
                               sdRID     aExtRID4TSS,
                               scPageID  aFstPIDOfLstExt4TSS,
                               sdRID     aFstExtRID4UDS,
                               sdRID     aLstExtRID4UDS,
                               scPageID  aFstPIDOfLstExt4UDS,
                               scPageID  aFstUndoPID,
                               scPageID  aLstUndoPID );

    // PROJ-1665 Parallel Direct-Path INSERT
    static IDE_RC setTransLogBufferSize(void * aTrans,
                                        UInt   aNeedSize);

    static inline smrRTOI *getRTOI4UndoFailure(void * aTrans );

    // DDL Transaction�� ǥ���ϴ� Log Record�� ����Ѵ�.
    IDE_RC writeDDLLog();

    IDE_RC addTouchedPage( scSpaceID aSpaceID,
                           scPageID  aPageID,
                           SShort    aTSSlotNum );

    void setStatistics( idvSQL * aStatistics );

    // BUG-22576
    IDE_RC addPrivatePageListToTableOnPartialAbort();

    inline void initCommitLog( smrTransCommitLog *aCommitLog,
                               smrLogType         aCommitLogType  );

    // BUG-29262 TSS �Ҵ翡 ������ Ʈ������� COMMIT �α׸� ����ؾ� �մϴ�.
    inline idBool isLogWritten();

    inline UInt   getLogSize() { return mLogOffset; };
    
    static inline UInt genHashValueFunc( void* aData );
    static inline SInt compareFunc( void* aLhs, void* aRhs );

    /* BUG-40427 [sm_resource] Closing cost of a LOB cursor 
     * which is used internally is too much expensive */
    IDE_RC closeAllLobCursors();
    IDE_RC closeAllLobCursorsWithRPLog();

    IDE_RC closeAllLobCursors( UInt aInfo );

    inline static void set2PCCallback( smGetDtxGlobalTxId aGetDtxGlobalTxId );

    void dumpTransInfo();

private:

    IDE_RC findORAddESVP(const SChar    *aExpSVPName,
                         idBool          aDoAdd,
                         smLSN          *aLSN);

    inline void initAbortLog( smrTransAbortLog *aAbortLog, 
                              smrLogType        aAbortLogType );
    inline void initPreAbortLog( smrTransPreAbortLog *aAbortLog );

    IDE_RC addOIDList2AgingList( SInt       aAgingState,
                                 smxStatus  aStatus,
                                 smLSN*     aEndLSN,
                                 smSCN*     aCommitSCN,
                                 idBool     aIsLegacyTrans );

    static SInt getLogBufferSize(void* aTrans);

    // PROJ-1665
    IDE_RC setLogBufferSize( UInt   aNeedSize );

    /* Commit Log��� �Լ� */
    IDE_RC writeCommitLog( smLSN* aEndLSN );
    /* Abort Log����� Undo Transaction */
    IDE_RC writeAbortLogAndUndoTrans( smLSN* aEndLSN );

    IDE_RC writeCommitLog4Memory( smLSN* aEndLSN );
    IDE_RC writeCommitLog4Disk( smLSN* aEndLSN );

    // Hybrid Transaction�� ���� Log Flush�ǽ�
    IDE_RC flushLog( smLSN *aLSN, idBool aIsCommit );

    /* LobCursor���� ID�� �ο����ֱ� ���� Sequence ��. 
     * LobCursor�� ������ 0�̴�. */
    smLobCursorID    mCurLobCursorID;
    smuHashBase      mLobCursorHash;

    static iduMemPool   mLobCursorPool;
    static iduMemPool   mLobColBufPool;

// For Member
public:

    void           *   mSmiTransPtr;

    // Transaction�� mFlag ����
    // - Transactional Replication Mode Set ( PROJ-1541 )
    // - Commit Write Wait Mode ( BUG-15396 )
    UInt               mFlag;

    smTID              mTransID;
    //fix BUG-23656 session,xid ,transaction�� ������ performance view�� �����ϰ�,
    //�׵鰣�� ���踦 ��Ȯ�� �����ؾ� ��.
    //transaction���� session id�� �߰�.
    UInt               mSessionID;

    smSCN              mMinMemViewSCN;      // Minimum Memory ViewSCN
    smSCN              mMinDskViewSCN;      // Minimum Disk ViewSCN
    smSCN              mFstDskViewSCN;      // Ʈ������� ��ũ ���� ViewSCN

    // BUG-26881 �߸��� CTS stamping���� acces�� �� ���� row�� ������
    // Ʈ����� ���۽������� active transaction �� oldestFstViewSCN�� ����
    smSCN              mOldestFstViewSCN;      

    smSCN              mCommitSCN;
    smSCN              mInfinite;

    smxStatus          mStatus;
    smxStatus          mStatus4FT;      /* FixedTable ��ȸ�� Status */
    UInt               mLSLockFlag;

    idBool             mIsUpdate; // durable writing�� ����
    idBool             mIsTransWaitRepl; /* BUG-39143 */
    /* BUG-19245: Transaction�� �ι� Free�Ǵ� ���� Detect�ϱ� ���� �߰��� */
    idBool             mIsFree;

    // PROJ-1553 Replication self-deadlock
    // transaction�� ������Ų replication�� ID
    // ���� replication�� transaction�� �ƴ϶��
    // SMX_NOT_REPL_TX�� �Ҵ�ȴ�.
    UInt               mReplID;

    idBool             mIsWriteImpLog;
    UInt               mLogTypeFlag;

    // For XA
    /* BUG-18981 */
    ID_XID            mXaTransID;
    smxCommitState    mCommitState;
    timeval           mPreparedTime;

    iduCond           mCondV;
    iduMutex          mMutex;
    smxTrans         *mNxtFreeTrans;
    smxTransFreeList *mTransFreeList;
    smLSN             mFstUndoNxtLSN;
    smLSN             mLstUndoNxtLSN;
    SInt              mSlotN;

    /* BUG-33895 [sm_recovery] add the estimate function 
     * the time of transaction undoing. */
    UInt              mTotalLogCount;
    UInt              mProcessedUndoLogCount;
    UInt              mUndoBeginTime;

    // PROJ-1705 Disk MVCC Renewal
    smxTouchPageList  mTouchPageList;

    // For disk manager pending List
    smuList           mPendingOp;

    smxOIDList       *mOIDList;
    // BUG-14093 FreeSlot���� addFreeSlotPending�� OIDList
    smxOIDList        mOIDFreeSlotList;

    //For Lock
    ULong             mUpdateSize;
    //For save point
    smxSavepointMgr   mSvpMgr;
    //For xa
    smxTrans         *mPrvPT;
    smxTrans         *mNxtPT;

    // For Recovery
    smxTrans         *mPrvAT;
    smxTrans         *mNxtAT;
    idBool            mAbleToRollback;

    /* BUG-27122 Restart Recovery �� UTRANS�� �����ϴ� ��ũ �ε�����
     * Verify ���( __SM_CHECK_DISK_INDEX_INTEGRITY =2 ) �߰� */
    smxOIDList       *mOIDToVerify;

    //For Recovery
    static iduMemPool mMutexListNodePool;
    smuList           mMutexList;
    //For Session Management
    UInt              mFstUpdateTime;
    idvSQL           *mStatistics;

    SChar            *mLogBuffer;
    UInt              mLogBufferSize;

    // TASK-2398 Log Compress
    // Ʈ����� rollback�� �α� ���� ������ ���� ������ �ڵ��
    // �αױ�Ͻ� �α� ������ ���� ���ҽ�
    smrCompRes       * mCompRes;
    // �α� ���� ���ҽ� Ǯ
    static smrCompResPool mCompResPool;

    UInt              mLogOffset;
    idBool            mDoSkipCheck;
    // unpin, alter table add column�� disk�� ������
    // old version  table�� ���� Ʈ�������
    // LogicalAger���� ������Ű�� ���Ͽ� ���.
    idBool            mDoSkipCheckSCN;
    idBool            mIsDDL;
    idBool            mIsFirstLog;

    /* PROJ-1381 */
    UInt              mLegacyTransCnt;

    UInt              mTXSegEntryIdx;  // Ʈ����� ���׸�Ʈ ��Ʈ�� ����
    sdcTXSegEntry   * mTXSegEntry;     // Ʈ����� ���׸�Ʈ ��Ʈ�� Pointer

    smxOIDNode *      mCacheOIDNode4Insert;

    // PRJ-1496
    smxTableInfoMgr   mTableInfoMgr;
    smxTableInfo*     mTableInfoPtr;

    /* Transaction Rollback�� Undo log�� ��ġ�� ����Ų��. */
    smLSN              mCurUndoNxtLSN;
    //for Eager Replication PROJ-1541
    smLSN              mLastWritedLSN;
    //PROJ-1608 recovery from replication
    smLSN              mBeginLogLSN;
    smLSN              mCommitLogLSN;

    // For PROJ-1490
    // Ʈ������� ���� mRSGroupID
    UInt              mRSGroupID;

    // For PROJ-1464
    // TX's Private Free Page List Entry
    smpPrivatePageListEntry* mPrivatePageListCachePtr;
                             // PrivatePageList�� Cache ������
    smuHashBase              mPrivatePageListHashTable;
                             // PrivatePageList�� HashTable
    iduMemPool               mPrivatePageListMemPool;
                             // PrivatePageList�� MemPool

    // PROJ-1594 Volatile TBS
    smpPrivatePageListEntry* mVolPrivatePageListCachePtr;
    smuHashBase              mVolPrivatePageListHashTable;
    iduMemPool               mVolPrivatePageListMemPool;

    //PROJ-1362.
    /* PROJ-2174 Supporting LOB in the volatile tablespace
     * volatile���� ���� ������ �ʰ�, mMemLCL�� ���� ����Ѵ�. */
    smxLCL           mMemLCL;
    smxLCL           mDiskLCL;

    /* PROJ-1594 Volatile TBS */
    /* Volatile logging�� ���� environment */
    svrLogEnv        mVolatileLogEnv;

    // PROJ-2068 Direct-Path INSERT ���� ����
    void*            mDPathEntry;

    idBool           mFreeInsUndoSegFlag;

    /* PROJ-2162 RestartRiskReduction
     * Undo ���п� ���� ��Ȳ�� ����� ���� */
    smrRTOI          mRTOI4UndoFailure;

    static smGetDtxGlobalTxId  mGetDtxGlobalTxIdFunc;

private:
    /* TASK-2401 MMAP Loggingȯ�濡�� Disk/Memory Log�� �и�
       �ش� Ʈ������� Disk, Memory Tablespace�� �����ߴ��� ����
     */
    idBool           mDiskTBSAccessed;
    idBool           mMemoryTBSAccessed;

    // �ش� Ʈ������� Meta Table�� �����Ͽ����� ����
    idBool           mMetaTableModified;
    UInt              mReplLockTimeout;

};

inline void smxTrans::setTSSAllocPos( sdRID   aCurExtRID,
                                      sdSID   aTSSlotSID )
{
    IDE_ASSERT( mTXSegEntry != NULL );

    mTXSegEntry->mTSSlotSID  = aTSSlotSID;
    mTXSegEntry->mExtRID4TSS = aCurExtRID;
}

inline void smxTrans::setDiskTBSAccessed()
{
    mDiskTBSAccessed = ID_TRUE;
}

inline void smxTrans::setMemoryTBSAccessed()
{
    mMemoryTBSAccessed = ID_TRUE;
}

// �ش� Ʈ������� Meta Table�� ������ ��� ȣ���
inline void smxTrans::setMetaTableModified()
{
    mMetaTableModified = ID_TRUE;
}

inline idBool smxTrans::isReadOnly()
{
    return (mIsUpdate == ID_TRUE ? ID_FALSE : ID_TRUE);
}

inline idBool smxTrans::isVolatileTBSTouched()
{
    return svrLogMgr::isOnceUpdated( &mVolatileLogEnv );
}

inline idBool smxTrans::isPrepared()
{
    return (mCommitState==SMX_XA_PREPARED ? ID_TRUE : ID_FALSE);
}

inline idBool smxTrans::isActive()
{
    if(mStatus != SMX_TX_END && mCommitState != SMX_XA_PREPARED)
    {
        return ID_TRUE;
    }
    else
    {
        return ID_FALSE;
    }
}

inline IDE_RC smxTrans::lock()
{
    return mMutex.lock( NULL );
}

inline IDE_RC smxTrans::unlock()
{
    return mMutex.unlock();
}

inline void smxTrans::initXID()
{
    mXaTransID.gtrid_length = (vULong)-1;
    mXaTransID.bqual_length = (vULong)-1;
}

inline idBool smxTrans::isValidXID()
{
    if ( mXaTransID.gtrid_length != (vSLong)-1 &&
         mXaTransID.bqual_length != (vSLong)-1)
    {
        return ID_TRUE;
    }
    else
    {
        return ID_FALSE;
    }
}

inline void smxTrans::init(smTID aTransID)
{
    mTransID = aTransID;
}

inline smxOIDNode* smxTrans::getCurNodeOfNVL()
{
    smxOIDNode *sHeadNode = &(mOIDList->mOIDNodeListHead);

    if(sHeadNode == sHeadNode->mPrvNode)
    {
        return NULL;
    }
    else
    {
        return sHeadNode->mPrvNode;
    }
}

inline IDE_RC smxTrans::abortToImpSavepoint(smxSavepoint *aSavepoint)
{
    IDE_TEST(removeAllAndReleaseMutex() != IDE_SUCCESS);

    IDE_TEST(mSvpMgr.abortToImpSavepoint(this,
                                         aSavepoint) != IDE_SUCCESS);

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

inline IDE_RC smxTrans::abortToExpSavepoint(const SChar *aExpSVPName)
{
    IDE_TEST(removeAllAndReleaseMutex() != IDE_SUCCESS);

    IDE_TEST(mSvpMgr.abortToExpSavepoint(this,aExpSVPName)
             != IDE_SUCCESS);

    /* PROJ-2462 ResultCache */
    mTableInfoMgr.addTablesModifyCount();

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;

}

inline IDE_RC smxTrans::addOID(smOID            aTableOID,
                               smOID            aRecordOID,
                               scSpaceID        aSpaceID,
                               UInt             aFlag)
{
    IDE_ASSERT(mStatus == SMX_TX_BEGIN);
    return mOIDList->add(aTableOID, aRecordOID, aSpaceID, aFlag);
}


/******************************************************
  Description:
          freeSlot�ϴ� OID�� �߰��Ѵ�.

  aTableOID  [IN]  freeSlot�ϴ� ���̺�OID
  aRecordOID [IN]  freeSlot�ϴ� ���ڵ�OID
  aFlag      [IN]  freeSlot�ϴ� ����(Fixed or Var)
********************************************************/
inline IDE_RC smxTrans::addFreeSlotOID(smOID            aTableOID,
                                       smOID            aRecordOID,
                                       scSpaceID        aSpaceID,
                                       UInt             aFlag)
{
    IDE_DASSERT(aTableOID != SM_NULL_OID);
    IDE_DASSERT(aRecordOID != SM_NULL_OID);
    IDE_DASSERT((aFlag == SM_OID_TYPE_FREE_FIXED_SLOT) ||
                (aFlag == SM_OID_TYPE_FREE_VAR_SLOT));
    IDE_ASSERT(mStatus == SMX_TX_BEGIN);

    return mOIDFreeSlotList.add(aTableOID, aRecordOID, aSpaceID, aFlag);
}

inline UInt smxTrans::getFstUpdateTime()
{
    return mFstUpdateTime;
}

inline smLSN smxTrans::getLstUndoNxtLSN()
{
    return mLstUndoNxtLSN;
}


inline IDE_RC smxTrans::getTableInfo(smOID          aOIDTable,
                                     smxTableInfo** aTableInfo,
                                     idBool         aIsSearch)
{
    return mTableInfoMgr.getTableInfo(aOIDTable,
                                      aTableInfo,
                                      aIsSearch);
}

inline IDE_RC smxTrans::undoDelete(smOID aOIDTable)
{
    smxTableInfo     *sTableInfoPtr = NULL;

    while(1)
    {
        if(mTableInfoPtr != NULL)
        {
            if(mTableInfoPtr->mTableOID == aOIDTable)
            {
                break;
            }
        }

        IDE_TEST(getTableInfo(aOIDTable, &sTableInfoPtr, ID_TRUE)
                 != IDE_SUCCESS);

        if(sTableInfoPtr != NULL)
        {
            mTableInfoPtr = sTableInfoPtr;
        }

        break;
    }

    if(mTableInfoPtr != NULL)
    {
        mTableInfoPtr->mRecordCnt++;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

inline IDE_RC smxTrans::undoInsert(smOID aOIDTable)
{
    smxTableInfo     *sTableInfoPtr = NULL;

    while(1)
    {
        if(mTableInfoPtr != NULL)
        {
            if(mTableInfoPtr->mTableOID == aOIDTable)
            {
                break;
            }
        }

        IDE_TEST(getTableInfo(aOIDTable, &sTableInfoPtr, ID_TRUE)
                 != IDE_SUCCESS);

        if(sTableInfoPtr != NULL)
        {
            mTableInfoPtr = sTableInfoPtr;
        }

        break;
    }

    if(mTableInfoPtr != NULL)
    {
        mTableInfoPtr->mRecordCnt--;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

inline IDE_RC smxTrans::getRecordCountFromTableInfo(smOID aOIDTable,
                                                    SLong* aRecordCnt)
{
    smxTableInfo     *sTableInfoPtr;

    sTableInfoPtr = NULL;
    *aRecordCnt = 0;

    while(1)
    {
        if(mTableInfoPtr != NULL)
        {
            if(mTableInfoPtr->mTableOID == aOIDTable)
            {
                break;
            }
        }

        IDE_TEST(getTableInfo(aOIDTable, &sTableInfoPtr, ID_FALSE)
                 != IDE_SUCCESS);

        if(sTableInfoPtr != NULL)
        {
            mTableInfoPtr = sTableInfoPtr;
        }

        break;
    }

    if(mTableInfoPtr != NULL)
    {
        *aRecordCnt = mTableInfoPtr->mRecordCnt;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/*******************************************************************************
 * Description : aTrans�� �޸� TableInfo���� aTableOID�� �ش��ϴ� ���̺���
 *          Record Count�� aRecordCnt��ŭ ������Ų��.
 ******************************************************************************/
inline IDE_RC smxTrans::incRecordCountOfTableInfo( void  * aTrans,
                                                   smOID   aTableOID,
                                                   SLong   aRecordCnt )
{
    smxTrans      * sTrans = (smxTrans*)aTrans;
    smxTableInfo  * sTableInfoPtr;

    sTableInfoPtr = NULL;

    //--------------------------------------------------------------
    // mTableInfoPtr ĳ�ÿ� �̸� aTableOID�� �ش��ϴ� Table Info��
    // ����Ǿ� ������ �ٷ� ���
    //--------------------------------------------------------------
    if( sTrans->mTableInfoPtr != NULL )
    {
        if( sTrans->mTableInfoPtr->mTableOID == aTableOID )
        {
            IDE_CONT( found_table_info );
        }
    }

    //-------------------------------------------------------------
    // Table Info�� ã�� �� ������ TableInfoMgr���� ã�ƺ���.
    // 3��° ���ڰ� ID_TRUE�̹Ƿ�, TableInfoMgr�� Hash�� ������ �ȵȴ�.
    // ���� sTableInfoPtr�� NULL�̸� ����ó���Ѵ�.
    //-------------------------------------------------------------
    IDE_TEST( sTrans->getTableInfo(aTableOID, &sTableInfoPtr, ID_TRUE)
              != IDE_SUCCESS );

    IDE_ASSERT( sTableInfoPtr != NULL );

    sTrans->mTableInfoPtr = sTableInfoPtr;

    IDE_EXCEPTION_CONT( found_table_info );

    if( sTrans->mTableInfoPtr != NULL )
    {
        sTrans->mTableInfoPtr->mRecordCnt += aRecordCnt;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/*******************************************************************************
 * Description: ��� TX�� ������ �ִ� table info�� DPath INSERT ���� ���θ�
 *      �����Ѵ�.
 *
 * Parameters:
 *  aTrans          - [IN] ��� TX�� smxTrans
 *  aTableOID       - [IN] DPath INSERT ���� ���θ� ǥ���� ��� table�� OID
 *  aExistDPathIns  - [IN] DPath INSERT ���� ����
 ******************************************************************************/
inline IDE_RC smxTrans::setExistDPathIns( void    * aTrans,
                                          smOID     aTableOID,
                                          idBool    aExistDPathIns )
{
    smxTrans      * sTrans = (smxTrans*)aTrans;
    smxTableInfo  * sTableInfoPtr = NULL;

    //--------------------------------------------------------------
    // mTableInfoPtr ĳ�ÿ� �̸� aTableOID�� �ش��ϴ� Table Info��
    // ����Ǿ� ������ �ٷ� ���
    //--------------------------------------------------------------
    if( sTrans->mTableInfoPtr != NULL )
    {
        if( sTrans->mTableInfoPtr->mTableOID == aTableOID )
        {
            sTableInfoPtr = sTrans->mTableInfoPtr;
            IDE_CONT( found_table_info );
        }
    }

    //-------------------------------------------------------------
    // Table Info�� ã�� �� ������ TableInfoMgr���� ã�ƺ���.
    // 3��° ���ڰ� ID_TRUE�̹Ƿ�, TableInfoMgr�� Hash�� ������ �ȵȴ�.
    // ���� sTableInfoPtr�� NULL�̸� ����ó���Ѵ�.
    //-------------------------------------------------------------
    IDE_TEST( sTrans->mTableInfoMgr.getTableInfo( aTableOID,
                                                  &sTableInfoPtr,
                                                  ID_TRUE ) /* aIsSearch */
              != IDE_SUCCESS );

    IDE_EXCEPTION_CONT( found_table_info );

    IDE_ERROR( sTableInfoPtr != NULL );

    smxTableInfoMgr::setExistDPathIns( sTableInfoPtr,
                                       aExistDPathIns );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

inline IDE_RC smxTrans::syncToEnd()
{
    return smrLogMgr::syncToLstLSN( SMR_LOG_SYNC_BY_TRX );
}

/*
 * Tx's PrivatePageList�� HashTable�� Hash Function
 *
 * aKeyPtr [IN]  Key���� ���� ������
 */
inline UInt smxTrans::hash(void* aKeyPtr)
{
    vULong sKey;

    IDE_DASSERT(aKeyPtr != NULL);

    sKey = *(vULong*)aKeyPtr;

    return sKey & (SMX_PRIVATE_BUCKETCOUNT - 1);
};

/*
 * Tx's PrivatePageList�� HashTable�� Ű�� �� �Լ�
 *
 * aLhs [IN]  �񱳴��1�� ���� ������
 * aRhs [IN]  �񱳴��2�� ���� ������
 */
inline SInt smxTrans::isEQ( void *aLhs, void *aRhs )
{
    vULong sLhs;
    vULong sRhs;

    IDE_DASSERT( aLhs != NULL );
    IDE_DASSERT( aRhs != NULL );

    sLhs = *(vULong*)aLhs;
    sRhs = *(vULong*)aRhs;

    return (sLhs == sRhs) ? 0 : -1;
};

/***********************************************************************
 *
 * Description : Hash�� ���� Hash Key�� ��ȯ�ϴ� �Լ�
 *
 * smuHash ���� Hash Key�� 32��Ʈ ������ ó���ϱ� ������ Hash �Լ��� ��ȯ����
 * 32��Ʈ�̴�. ���� �Է�����(aData)�� 8����Ʈ������ ������ �ȴٸ� ����ȿ� ����
 * Hash Key�� �ߺ��Ǵ� ��� ���� �߻��� ���� �ִ�.
 *
 * aData [IN] - Hash�� ����Ÿ
 *
 **********************************************************************/
inline UInt smxTrans::genHashValueFunc( void* aData )
{
    return *(UInt*)(aData);
}

/***********************************************************************
 *
 * Description : Hash Value�� ����  �� �Լ�
 *
 **********************************************************************/
inline SInt smxTrans::compareFunc( void* aLhs, void* aRhs )
{

    IDE_ASSERT( aLhs != NULL );
    IDE_ASSERT( aRhs != NULL );

    if (*((UInt*)(aLhs)) == *((UInt*)(aRhs)) )
    {
        return (0);
    }

    return (-1);
}

/*************************************************************************
 * Description: Implicit Savepoint���� IS Lock���� �����Ѵ�.
 * ***********************************************************************/
inline IDE_RC smxTrans::unlockSeveralLock(ULong aLockSequence)
{
    return mSvpMgr.unlockSeveralLock(this, aLockSequence);
}

/*************************************************************************
 * Description: Volatile logging�� ���� �ʿ��� svrLogEnv �����͸� ��´�.
 *************************************************************************/
inline svrLogEnv* smxTrans::getVolatileLogEnv()
{
    return &mVolatileLogEnv;
}

/****************************************************************************
 * Description : Ʈ����� Abort �α׸� �ʱ�ȭ�Ѵ�.
 * 
 * aAbortLog     - [IN] AbortLog ������
 * aAbortLogType - [IN] AbortLog Ÿ��
 ****************************************************************************/
inline void smxTrans::initAbortLog( smrTransAbortLog *aAbortLog,
                                    smrLogType        aAbortLogType )
{
    smrLogHeadI::setType( &( aAbortLog->mHead ), aAbortLogType );
    smrLogHeadI::setSize( &( aAbortLog->mHead ),
                          SMR_LOGREC_SIZE(smrTransAbortLog) +
                          ID_SIZEOF(smrLogTail));
    smrLogHeadI::setTransID( &( aAbortLog->mHead ), mTransID);
    smrLogHeadI::setFlag( &( aAbortLog->mHead ), mLogTypeFlag);
    aAbortLog->mDskRedoSize = 0;
    aAbortLog->mGlobalTxId = mGetDtxGlobalTxIdFunc( mTransID );
}

inline void smxTrans::initPreAbortLog( smrTransPreAbortLog *aAbortLog )
{
    smrLogHeadI::setType( &( aAbortLog->mHead ), SMR_LT_TRANS_PREABORT );
    smrLogHeadI::setSize( &( aAbortLog->mHead ),
                          SMR_LOGREC_SIZE( smrTransPreAbortLog ) );
    smrLogHeadI::setTransID( &( aAbortLog->mHead ), mTransID);
    smrLogHeadI::setFlag( &( aAbortLog->mHead ), mLogTypeFlag);

    aAbortLog->mTail = SMR_LT_TRANS_PREABORT;
}

/****************************************************************************
 * Description : Ʈ����� Commit �α׸� �ʱ�ȭ�Ѵ�.
 * 
 * aCommitLog     - [IN] CommitLog ������
 * aCommitLogType - [IN] CommitLog Ÿ��
 ****************************************************************************/
inline void smxTrans::initCommitLog(  smrTransCommitLog *aCommitLog,
                                      smrLogType         aCommitLogType )
{
    SChar *sCurLogPtr;

    smrLogHeadI::setType( &aCommitLog->mHead, aCommitLogType );
    smrLogHeadI::setSize( &aCommitLog->mHead,
                          SMR_LOGREC_SIZE(smrTransCommitLog) + ID_SIZEOF(smrLogTail) );
    smrLogHeadI::setTransID (&aCommitLog->mHead, mTransID );
    smrLogHeadI::setFlag( &aCommitLog->mHead, mLogTypeFlag );

    aCommitLog->mDskRedoSize = 0;
    aCommitLog->mGlobalTxId = mGetDtxGlobalTxIdFunc( mTransID );

    sCurLogPtr = (SChar*)aCommitLog + SMR_LOGREC_SIZE(smrTransCommitLog);
    smrLogHeadI::copyTail(sCurLogPtr, &aCommitLog->mHead);
}

inline idBool smxTrans::isLogWritten()
{
    return ((mLogOffset > 0)? ID_TRUE : ID_FALSE);
}

/*
   fix BUG-15480

   Ʈ������� Commit �̳� Abort Pending Operation��
   ������ �ִ��� ���θ� ��ȯ�Ѵ�.

   [ ���� ]

   [IN] aTrans  - Ʈ����� ��ü
*/
inline idBool smxTrans::hasPendingOp()
{
    idBool sHasPendingOp;

    if( SMU_LIST_IS_EMPTY( &mPendingOp ) )
    {
        sHasPendingOp = ID_FALSE;
    }
    else
    {
        sHasPendingOp = ID_TRUE;
    }

    return sHasPendingOp;
}

/*************************************************************************
  BUG-15396
  Description: Commit ��, log�� disk�� ������������ ��ٸ� ���ο� ����
                ���� ��ȯ
 *************************************************************************/
inline idBool smxTrans::isCommitWriteWait()
{
    idBool sIsWait;

    if ( (mFlag & SMX_COMMIT_WRITE_MASK )
         == SMX_COMMIT_WRITE_WAIT )
    {
        sIsWait = ID_TRUE;
    }
    else
    {
        sIsWait = ID_FALSE;
    }

    return sIsWait;
}


/**************************************************************
 * Descrition: Log Flush�� �ʿ������� ����
 *
 * aTrans - [IN] Transaction Pointer
 *
**************************************************************/
inline idBool smxTrans::isNeedLogFlushAtCommitAPrepareInternal()
{
    idBool sNeedLogFlush = ID_FALSE;

    // log�� disk�� ��ϵɶ����� ��ٸ����� ���� ���� ȹ��
    if ( ( hasPendingOp() == ID_TRUE ) ||
         ( isCommitWriteWait() == ID_TRUE ) )
    {
        sNeedLogFlush = ID_TRUE;
    }
    else
    {
        /* nothing to do ...  */
    }

    return sNeedLogFlush;
}

inline sdcUndoSegment* smxTrans::getUDSegPtr( smxTrans * aTrans )
{
    IDE_ASSERT( aTrans->mTXSegEntry != NULL );

    return &(aTrans->mTXSegEntry->mUDSegmt);
}

inline sdcTXSegEntry* smxTrans::getTXSegEntry()
{
    return mTXSegEntry;
}

inline void smxTrans::setTXSegEntry( sdcTXSegEntry * aTXSegEntry )
{
    mTXSegEntry    = aTXSegEntry;
    if ( aTXSegEntry != NULL )
    {
        mTXSegEntryIdx = aTXSegEntry->mEntryIdx;
    }
    else
    {
        mTXSegEntryIdx = ID_UINT_MAX;
    }
}

inline void smxTrans::setUndoCurPos( sdSID    aUndoRecSID,
                                     sdRID    aUndoExtRID )
{
    IDE_ASSERT( mTXSegEntry != NULL );

    if ( mTXSegEntry->mFstUndoPID == SD_NULL_PID )
    {
        IDE_ASSERT( mTXSegEntry->mFstUndoSlotNum == SC_NULL_SLOTNUM );
        IDE_ASSERT( mTXSegEntry->mFstExtRID4UDS  == SD_NULL_RID );
        IDE_ASSERT( mTXSegEntry->mLstUndoPID     == SD_NULL_PID );
        IDE_ASSERT( mTXSegEntry->mLstUndoSlotNum == SC_NULL_SLOTNUM );
        IDE_ASSERT( mTXSegEntry->mLstExtRID4UDS  == SD_NULL_RID );

        mTXSegEntry->mFstUndoPID     = SD_MAKE_PID( aUndoRecSID );
        mTXSegEntry->mFstUndoSlotNum = SD_MAKE_SLOTNUM( aUndoRecSID );
        mTXSegEntry->mFstExtRID4UDS  = aUndoExtRID;
    }

    mTXSegEntry->mLstUndoPID     = SD_MAKE_PID( aUndoRecSID );
    mTXSegEntry->mLstUndoSlotNum = SD_MAKE_SLOTNUM( aUndoRecSID ) + 1;
    mTXSegEntry->mLstExtRID4UDS = aUndoExtRID;
}

inline void smxTrans::getUndoCurPos( sdSID  * aUndoRecSID,
                                     sdRID  * aUndoExtRID )
{
    if ( mTXSegEntry != NULL )
    {
       if ( mTXSegEntry->mLstUndoPID == SD_NULL_PID )
       {
           *aUndoRecSID = SD_NULL_SID;
           *aUndoExtRID = SD_NULL_RID;
       }
       else
       {
           *aUndoRecSID = SD_MAKE_SID( mTXSegEntry->mLstUndoPID,
                                       mTXSegEntry->mLstUndoSlotNum );
           *aUndoExtRID = mTXSegEntry->mLstExtRID4UDS;
       }
    }
    else
    {
        *aUndoRecSID = SD_NULL_SID;
        *aUndoExtRID = SD_NULL_RID;
    }
}

/***********************************************************************
 *
 * Description :
 *  infinite scn ���� ��ȯ�Ѵ�.
 *
 **********************************************************************/
inline smSCN smxTrans::getInfiniteSCN()
{
    return mInfinite;
}

/***************************************************************************
 *
 * Description: Ʈ������� MemViewSCN Ȥ�� DskViewSCN Ȥ�� ��� �����Ѵ�.
 *
 * End�ϴ� statement�� �����ϰ� ������ statement���� SCN ���� ���� ���� ������ �����Ѵ�.
 * ������ ���� ���� ���� ���� transaction�� viewSCN�� ���� ��쿡��  ������ skip�Ѵ�.
 * End�Ҷ� �ٸ� Stmt�� ������, Ʈ������� ViewSCN�� infinite�� �Ѵ�.
 *
 * getViewSCNforTransMinSCN���� Ʈ������� MinViewSCN�� infinite�� ��쿡
 * �־��� SCN������ �����Ѵ�.
 *
 * aCursorFlag - [IN] Stmt�� Cursor Ÿ�� Flag (MASK ���� ��)
 * aMemViewSCN - [IN] ������ Ʈ������� MemViewSCN
 * aDskViewSCN - [IN] ������ Ʈ������� DskViewSCN
 *
 ****************************************************************************/
inline void smxTrans::setMinViewSCN( UInt    aCursorFlag,
                                     smSCN * aMemViewSCN,
                                     smSCN * aDskViewSCN )
{
    IDE_ASSERT( (aCursorFlag == SMI_STATEMENT_ALL_CURSOR)    ||
                (aCursorFlag == SMI_STATEMENT_MEMORY_CURSOR) ||
                (aCursorFlag == SMI_STATEMENT_DISK_CURSOR) );

    mLSLockFlag = SMX_TRANS_LOCKED;

    IDL_MEM_BARRIER;

    if ( (aCursorFlag & SMI_STATEMENT_MEMORY_CURSOR) != 0 )
    {
        IDE_ASSERT( aMemViewSCN != NULL );
        IDE_ASSERT( SM_SCN_IS_GE(aMemViewSCN, &mMinMemViewSCN) );

        SM_SET_SCN( &mMinMemViewSCN, aMemViewSCN );
    }

    if ( (aCursorFlag & SMI_STATEMENT_DISK_CURSOR) != 0 )
    {
        IDE_ASSERT( aDskViewSCN != NULL );
        IDE_ASSERT( SM_SCN_IS_GE(aDskViewSCN, &mMinDskViewSCN) );

        SM_SET_SCN( &mMinDskViewSCN, aDskViewSCN );
    }

    IDL_MEM_BARRIER;

    mLSLockFlag = SMX_TRANS_UNLOCKED;
}

/***************************************************************************
 *
 * Description: Ʈ������� MemViewSCN Ȥ�� DskViewSCN�� �ʱ�ȭ�Ѵ�.
 *
 * aCursorFlag - [IN] Stmt�� Cursor Ÿ�� Flag (MASK ���� ��)
 *
 ****************************************************************************/
inline void  smxTrans::initMinViewSCN( UInt   aCursorFlag )
{
    IDE_ASSERT( (aCursorFlag == SMI_STATEMENT_ALL_CURSOR)    ||
                (aCursorFlag == SMI_STATEMENT_MEMORY_CURSOR) ||
                (aCursorFlag == SMI_STATEMENT_DISK_CURSOR) );

    mLSLockFlag = SMX_TRANS_LOCKED;

    IDL_MEM_BARRIER;

    if ( (aCursorFlag & SMI_STATEMENT_MEMORY_CURSOR) != 0 )
    {
        SM_SET_SCN_INFINITE( &mMinMemViewSCN );
    }

    if ( (aCursorFlag & SMI_STATEMENT_DISK_CURSOR) != 0 )
    {
        SM_SET_SCN_INFINITE( &mMinDskViewSCN );
    }

    IDL_MEM_BARRIER;

    mLSLockFlag = SMX_TRANS_UNLOCKED;
}

/***********************************************************************
 *
 * Description : Lob�� ����Ͽ� MemViewSCN�� ��ȯ�Ѵ�.
 *
 * aSCN - [OUT] Ʈ������� MinMemViewSCN
 *
 **********************************************************************/
inline void smxTrans::getMinMemViewSCN4LOB( smSCN*  aSCN )
{
    mMemLCL.getOldestSCN(aSCN);

    if ( SM_SCN_IS_GT(aSCN,&mMinMemViewSCN) )
    {
        SM_GET_SCN(aSCN,&mMinMemViewSCN);
    }
}

/***********************************************************************
 *
 * Description : Lob�� ����Ͽ� DskViewSCN�� ��ȯ�Ѵ�.
 *
 * aSCN - [OUT] Ʈ������� MinDskViewSCN
 *
 **********************************************************************/
inline void smxTrans::getMinDskViewSCN4LOB( smSCN* aSCN )
{
    mDiskLCL.getOldestSCN(aSCN);

    if( SM_SCN_IS_GT(aSCN,&mMinDskViewSCN) )
    {
        SM_GET_SCN(aSCN,&mMinDskViewSCN);
    }
}

/*******************************************************************************
 * Description : [PROJ-2068] Direct-Path INSERT�� ���� Entry Point��
 *          DPathEntry�� �Ҵ��Ѵ�.
 *
 * Implementation : sdcDPathInsertMgr�� ��û�Ͽ� DPathEntry�� �Ҵ� �޴´�.
 * 
 * Parameters :
 *      aTrans  - [IN] DPathEntry�� �Ҵ� ���� Transaction�� ������
 ******************************************************************************/
inline IDE_RC smxTrans::allocDPathEntry( smxTrans* aTrans )
{
    IDE_ASSERT( aTrans != NULL );

    if( aTrans->mDPathEntry == NULL )
    {
        IDE_TEST( sdcDPathInsertMgr::allocDPathEntry( &aTrans->mDPathEntry )
                  != IDE_SUCCESS );
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/*******************************************************************************
 * Description : [PROJ-2068] aTrans�� �޷��ִ� DPathEntry�� ��ȯ�Ѵ�.
 *
 * Parameters :
 *      aTrans  - [IN] smxTrans�� ������
 ******************************************************************************/
inline void* smxTrans::getDPathEntry( void* aTrans )
{
    return ((smxTrans*)aTrans)->mDPathEntry;
}

/*****************************************************************************
 * Description : [PROJ-2162] aTrans�� �޷��ִ� mRTOI4UndoFailure�� ��ȯ�Ѵ�.
 *
 * Parameters :
 *      aTrans  - [IN] smxTrans�� ������
 ****************************************************************************/
inline smrRTOI * smxTrans::getRTOI4UndoFailure(void * aTrans )
{
    return &((smxTrans*)aTrans)->mRTOI4UndoFailure;
}

inline void smxTrans::set2PCCallback( smGetDtxGlobalTxId aGetDtxGlobalTxId )
{
    mGetDtxGlobalTxIdFunc = aGetDtxGlobalTxId;
}

inline IDE_RC smxTrans::suspend(smxTrans*   aWaitTrans,
                                smTID       aWaitTransID,
                                iduMutex*   aMutex,
                                ULong       aWaitMicroSec)
{
    switch ( smuProperty::getLockMgrType() )
    {
    case 0:
        return suspendMutex(aWaitTrans, aMutex, aWaitMicroSec);
    case 1:
        return suspendSpin(aWaitTrans, aWaitTransID, aWaitMicroSec);
    default:
        IDE_ASSERT(0);
        return IDE_FAILURE;
    }
}

inline IDE_RC smxTrans::writeLogToBufferOfTx( void       * aTrans, 
                                              const void * aLog,
                                              UInt         aOffset, 
                                              UInt         aLogSize )
{
    return ((smxTrans*)aTrans)->writeLogToBuffer( aLog, aOffset, aLogSize );
}

inline smTID smxTrans::getTransID( const void * aTrans )
{
    return ((smxTrans*)aTrans)->mTransID;
}

inline void  smxTrans::initTransLogBuffer( void * aTrans )
{
    ((smxTrans*)aTrans)->initLogBuffer();
}

inline IDE_RC smxTrans::addOID2Trans( void    * aTrans,
                                      smOID     aTblOID,
                                      smOID     aRecordOID,
                                      scSpaceID aSpaceID,
                                      UInt      aFlag )
{
    IDE_RC sResult = IDE_SUCCESS;

    /* BUG-14558:OID List�� ���� Add�� Transaction Begin�Ǿ��� ����
       ����Ǿ�� �Ѵ�.*/
    if ( ((smxTrans*)aTrans)->mStatus == SMX_TX_BEGIN )
    {
        if ( ( aFlag == SM_OID_TYPE_FREE_FIXED_SLOT ) ||
             ( aFlag == SM_OID_TYPE_FREE_VAR_SLOT ) )
        {
            // BUG-14093 freeSlot�� �͵��� addFreeSlotPending�� ����Ʈ�� �Ŵܴ�.
            sResult = ((smxTrans*)aTrans)->addFreeSlotOID( aTblOID,
                                                           aRecordOID,
                                                           aSpaceID,
                                                           aFlag );
        }
        else
        {
            sResult = ((smxTrans*)aTrans)->addOID( aTblOID,
                                                   aRecordOID,
                                                   aSpaceID,
                                                   aFlag );
        }
    }

    return sResult;
}

/***********************************************************************
 * Description : ���������� ������ Statement�� Depth
 *
 * aTrans - [IN]  Transaction Pointer
 ***********************************************************************/
UInt smxTrans::getLstReplStmtDepth(void* aTrans)
{
    smxTrans * sTrans = (smxTrans*)aTrans;

    return sTrans->mSvpMgr.getLstReplStmtDepth();
}

inline SInt  smxTrans::getTransSlot( void * aTrans )
{
    return ((smxTrans *)aTrans)->mSlotN;
}

/***********************************************************************
 *
 * Description : Ʈ������� TSS�� SID�� ��ȯ�Ѵ�.
 *
 **********************************************************************/
inline sdSID smxTrans::getTSSlotSID( void * aTrans )
{
    sdcTXSegEntry * sTXSegEntry;

    IDE_ASSERT( aTrans != NULL );

    sTXSegEntry = ((smxTrans *)aTrans)->mTXSegEntry;

    if ( sTXSegEntry != NULL )
    {
        return sTXSegEntry->mTSSlotSID;
    }

    return SD_NULL_SID;
}

/**********************************************************************
 *
 * Description : ù��° Disk Stmt�� ViewSCN�� ��ȯ�Ѵ�.
 *
 * aTrans - [IN] Ʈ����� ������
 *
 **********************************************************************/
inline smSCN smxTrans::getFstDskViewSCN( void * aTrans )
{
    smSCN   sFstDskViewSCN;

    SM_GET_SCN( &sFstDskViewSCN, &((smxTrans *)aTrans)->mFstDskViewSCN );

    return sFstDskViewSCN;
}

inline UInt smxTrans::getReplLockTimeout()
{
    return mReplLockTimeout;
}

#endif
