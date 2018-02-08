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
 * $Id: sdpTableSpace.h 15787 2006-05-19 02:26:17Z bskim $
 *
 * Description :
 *
 * ���̺����̽� ������
 *
 *
 **********************************************************************/

#ifndef _O_SCT_TABLE_SPACE_MGR_H_
#define _O_SCT_TABLE_SPACE_MGR_H_ 1

#include <sctDef.h>
// BUGBUG-1548 SDD�� SCT���� �� LAyer�̹Ƿ� �̿Ͱ��� Include�ؼ��� �ȵ�
#include <sdd.h>

struct smmTBSNode;
struct svmTBSNode;

class sctTableSpaceMgr
{
public:

    static IDE_RC initialize();

    static IDE_RC destroy();

    static inline idBool isBackupingTBS( scSpaceID  aSpaceID );

    // smiTBSLockValidType�� sctTBSLockValidOpt�� �����Ͽ� ��ȯ�Ѵ�.
    static sctTBSLockValidOpt
              getTBSLvType2Opt( smiTBSLockValidType aTBSLvType )
    {
        IDE_ASSERT( aTBSLvType < SMI_TBSLV_OPER_MAXMAX );

        return mTBSLockValidOpt[ aTBSLvType ];
    }

    // ( Disk/Memory ���� ) Tablespace Node�� �ʱ�ȭ�Ѵ�.
    static IDE_RC initializeTBSNode(sctTableSpaceNode * aSpaceNode,
                                    smiTableSpaceAttr * aSpaceAttr );

    // ( Disk/Memory ���� ) Tablespace Node�� �ı��Ѵ�.
    static IDE_RC destroyTBSNode(sctTableSpaceNode * aSpaceNode );


    // Tablespace�� Sync Mutex�� ȹ���Ѵ�.
    static IDE_RC latchSyncMutex( sctTableSpaceNode * aSpaceNode );

    // Tablespace�� Sync Mutex�� Ǯ���ش�.
    static IDE_RC unlatchSyncMutex( sctTableSpaceNode * aSpaceNode );

    // NEW ���̺����̽� ID�� �Ҵ��Ѵ�.
    static IDE_RC allocNewTableSpaceID( scSpaceID*   aNewID );

    // ���� NEW ���̺����̽� ID�� �����Ѵ�.
    static void   setNewTableSpaceID( scSpaceID aNewID )
                  { mNewTableSpaceID = aNewID; }

    // ���� NEW ���̺����̽� ID�� ��ȯ�Ѵ�.
    static scSpaceID getNewTableSpaceID() { return mNewTableSpaceID; }

    // ù ���̺����̽� ��带 ��ȯ�Ѵ�.
    static void getFirstSpaceNode( void  **aSpaceNode );

    // ���� ���̺����̽��� �������� ���� ���̺����̽� ��带 ��ȯ�Ѵ�.
    static void getNextSpaceNode( void   *aCurrSpaceNode,
                                  void  **aNextSpaceNode );

    static void getNextSpaceNodeIncludingDropped( void  *aCurrSpaceNode,
                                                  void **aNextSpaceNode );

    /* TASK-4007 [SM] PBT�� ���� ��� �߰� *
     * Drop���̰ų� Drop�� ���̰ų� �������� ���̺� �����̽� ��
     * ������ ���̺� �����̽��� �����´�*/
    static void getNextSpaceNodeWithoutDropped( void  *aCurrSpaceNode,
                                                void **aNextSpaceNode );

    // �޸� ���̺����̽��� ���θ� ��ȯ�Ѵ�.
    static inline idBool isMemTableSpace( scSpaceID aSpaceID );

    // Volatile ���̺����̽��� ���θ� ��ȯ�Ѵ�.
    static inline idBool isVolatileTableSpace( scSpaceID aSpaceID );

    // ��ũ ���̺����̽��� ���θ� ��ȯ�Ѵ�.
    static inline idBool isDiskTableSpace( scSpaceID aSpaceID );

    // ��� ���̺����̽��� ���θ� ��ȯ�Ѵ�.
    static idBool isUndoTableSpace( scSpaceID aSpaceID );

    // �ӽ� ���̺����̽��� ���θ� ��ȯ�Ѵ�.
    static idBool isTempTableSpace( scSpaceID aSpaceID );

    // �ý��� �޸� ���̺����̽��� ���θ� ��ȯ�Ѵ�.
    static idBool isSystemMemTableSpace( scSpaceID aSpaceID );

    // BUG-23953
    static inline IDE_RC getTableSpaceType( scSpaceID      aSpaceID,
                                            UInt         * aType );

    // TBS�� ���� ������ ��ȯ�Ѵ�. (Disk, Memory, Volatile)
    static smiTBSLocation getTBSLocation( scSpaceID aSpaceID );

    // PRJ-1548 User Memory Tablespace
    // �ý��� ���̺����̽��� ���θ� ��ȯ�Ѵ�.
    static idBool isSystemTableSpace( scSpaceID aSpaceID );

    static IDE_RC dumpTableSpaceList();

    // TBSID�� �Է¹޾� ���̺����̽��� �Ӽ��� ��ȯ�Ѵ�.
    static IDE_RC getTBSAttrByID(scSpaceID          aID,
                                 smiTableSpaceAttr* aSpaceAttr);

    // Tablespace Attribute Flag�� Pointer�� ��ȯ�Ѵ�.
    static IDE_RC getTBSAttrFlagPtrByID(scSpaceID       aID,
                                        UInt         ** aAttrFlagPtr);

    // Tablespace�� Attribute Flag�κ��� �α� ���࿩�θ� ���´�
    static IDE_RC getSpaceLogCompFlag( scSpaceID aSpaceID, idBool *aDoComp );


    // TBS���� �Է¹޾� ���̺����̽��� �Ӽ��� ��ȯ�Ѵ�.
    static IDE_RC getTBSAttrByName(SChar*              aName,
                                   smiTableSpaceAttr*  aSpaceAttr);


    // TBS���� �Է¹޾� ���̺����̽��� �Ӽ��� ��ȯ�Ѵ�.
    static IDE_RC findSpaceNodeByName(SChar* aName,
                                      void** aSpaceNode);


    // TBS���� �Է¹޾� ���̺����̽��� �����ϴ��� Ȯ���Ѵ�.
    static idBool checkExistSpaceNodeByName( SChar* aTableSpaceName );

    // Tablespace ID�� SpaceNode�� ã�´�.
    // �ش� Tablespace�� DROP�� ��� ������ �߻���Ų��.
    static IDE_RC findSpaceNodeBySpaceID(scSpaceID  aSpaceID,
                                         void**     aSpaceNode);

    // Tablespace ID�� SpaceNode�� ã�´�.
    // ������ TBS Node �����͸� �ش�.
    // BUG-18743: �޸� �������� ������ ��� PBT�� �� �ִ�
    // ������ �ʿ��մϴ�.
    //   smmManager::isValidPageID���� ����ϱ� ���ؼ� �߰�.
    // �ٸ� ���������� �� �Լ� ��뺸�ٴ� findSpaceNodeBySpaceID ��
    // �����մϴ�. �ֳ��ϸ� ���Լ��� Validation�۾��� ���� �����ϴ�.
    static inline void* getSpaceNodeBySpaceID( scSpaceID  aSpaceID )
    {
        return mSpaceNodeArray[aSpaceID];
    }

    // Tablespace ID�� SpaceNode�� ã�´�.
    // �ش� Tablespace�� DROP�� ��� SpaceNode�� NULL�� �����Ѵ�.
    static void findSpaceNodeWithoutException(scSpaceID  aSpaceID,
                                              void**     aSpaceNode,
                                              idBool     aUsingTBSAttr = ID_FALSE );


    // Tablespace ID�� SpaceNode�� ã�´�.
    // �ش� Tablespace�� DROP�� ��쿡�� aSpaceNode�� �ش� TBS�� ����.
    static void findSpaceNodeIncludingDropped(scSpaceID  aSpaceID,
                                              void**     aSpaceNode);

    // Tablespace�� �����ϴ��� üũ�Ѵ�.
    static idBool isExistingTBS( scSpaceID aSpaceID );

    // Tablespace�� ONLINE�������� üũ�Ѵ�.
    static idBool isOnlineTBS( scSpaceID aSpaceID );

    // Tablespace�� ���� State�� �ϳ��� State�� ���ϴ��� üũ�Ѵ�.
    static idBool hasState( scSpaceID   aSpaceID,
                            sctStateSet aStateSet,
                            idBool      aUsingTBSAttr = ID_FALSE );

    static idBool hasState( sctTableSpaceNode * aSpaceNode,
                            sctStateSet         aStateSet );

    // Tablespace�� State�� aStateSet���� State�� �ϳ��� �ִ��� üũ�Ѵ�.
    static idBool isStateInSet( UInt         aTBSState,
                                sctStateSet  aStateSet );

    // Tablespace���� Table/Index�� Open�ϱ� ���� Tablespace��
    // ��� �������� üũ�Ѵ�.
    static IDE_RC validateTBSNode( sctTableSpaceNode * aSpaceNode,
                                   sctTBSLockValidOpt  aTBSLvOpt );

    // TBS�� �ش��ϴ� ���̺����̽� ID�� ��ȯ�Ѵ�.
    static IDE_RC getTableSpaceIDByNameLow(SChar*     aTableSpaceName,
                                           scSpaceID* aTableSpaceID);

    static void getLock( iduMutex **aMutex ) { *aMutex = &mMutex; }

    // ���̺����̽� �������� TBS List�� ���� ���ü� ����
    static IDE_RC lock( idvSQL * aStatistics )
    { return mMutex.lock( aStatistics );   }
    static IDE_RC unlock( ) { return mMutex.unlock(); }

    // ���̺����̽� ������ ����Ÿ���� ������ ���� ���ü� ����
    static IDE_RC lockForCrtTBS( ) { return mMtxCrtTBS.lock( NULL ); }
    static IDE_RC unlockForCrtTBS( ) { return mMtxCrtTBS.unlock(); }

    static IDE_RC lockGlobalPageCountCheckMutex()
    { return mGlobalPageCountCheckMutex.lock( NULL );   }
    static IDE_RC unlockGlobalPageCountCheckMutex( )
    { return mGlobalPageCountCheckMutex.unlock(); }

    // PRJ-1548 User Memory Tablespace
    // ��ݾ��� ���̺����̽��� ���õ� Sync����� Drop ���갣�� ���ü�
    // �����ϱ� ���� Sync Mutex�� ����Ѵ�.

    static void addTableSpaceNode( sctTableSpaceNode *aSpaceNode );
    static void removeTableSpaceNode( sctTableSpaceNode *aSpaceNode );

    // ���������ÿ� ��� �ӽ� ���̺����̽��� Reset �Ѵ�.
    static IDE_RC resetAllTempTBS( void *aTrans );

    //for PRJ-1149 added functions
    static IDE_RC  getDataFileNodeByName(SChar*            aFileName,
                                         sddDataFileNode** aFileNode,
                                         scSpaceID*        aTbsID,
                                         scPageID*         aFstPageID = NULL,
                                         scPageID*         aLstPageID = NULL,
                                         SChar**           aSpaceName = NULL);


    // Ʈ����� Ŀ�� ������ �����ϱ� ���� ������ ���
    static IDE_RC addPendingOperation( void               *aTrans,
                                       scSpaceID           aSpaceID,
                                       idBool              aIsCommit,
                                       sctPendingOpType    aPendingOpType,
                                       sctPendingOp      **aPendingOp = NULL );


    // ���̺����̽� ���� Ʈ����� Commit-Pending ������ �����Ѵ�.
    static IDE_RC executePendingOperation( idvSQL *aStatistics,
                                           void   *aPendingOp,
                                           idBool  aIsCommit);

    // PRJ-1548 User Memory Tablespace
    //
    // # ��ݰ��������� ����
    //
    // �������� table�� tablespace ���� ������ lock hierachy�� ������ ��ݰ����� �ϰ� �ִ�.
    // �̷��� ���������� case by case�� ���� �κ��� ó���ؾ��ϴ� ��찡 ���� �߻��� ���̴�.
    // ������� offline ������ �� ��, ���� table�� ���� X ����� ȹ���ϱ�� �Ͽ��� ��쿡
    // �������� table�̳� drop ���� table�� ���ؼ��� ���ó���� ��� �ϴ��Ķ�� ������ �����.
    // �Ƹ� catalog page�� ���ؼ� ����� �����ؾ� ������ �𸥴�.
    // �̷��� ��츦 ������� ���� ������ tablespace ����� ����� ȹ���ϵ��� �����Ѵٸ�
    // �� �� ����ϰ� �ذ��� �� ���� ���̰�,
    // ���� ������ ��å�� �ƴҷ���..
    //
    // A. TBS NODE -> CATALOG TABLE -> TABLE -> DBF NODE ������
    //    lock hierachy�� �����Ѵ�.
    //
    // B. DML ����� TBS Node�� ���ؼ��� INTENTION LOCK�� ��û�ϵ��� ó���Ѵ�.
    // STATEMENT�� CURSOR ���½� ó���ϰ�, SAVEPOINT�� ó���ؾ��Ѵ�.
    // ���̺����̽� ���� DDL���� ���ü� ��� �����ϴ�.
    //
    // C. SYSTEM TABLESPACE�� drop�� �Ұ����ϹǷ� ����� ȹ������ �ʾƵ� �ȴ�.
    //
    // D. DDL ����� TBS Node �Ǵ� DBF Node�� ���ؼ� ����� ��û�ϵ��� ó���Ѵ�.
    // TABLE/INDEX� ���� DDL�� ���ؼ��� ���� TBS�� ����� ȹ���ϵ��� �Ѵ�.
    // ���̺����̽� ���� DDL���� ���ü� ��� �����ϴ�.
    // ��, PSM/VIEW/SEQUENCE���� SYSTEM_DICTIONARY_TABLESPACE�� �����ǹǷ� �����
    // ȹ���� �ʿ�� ����.
    //
    // E. LOCK ��û ����� ���̱� ���ؼ� TABLE_LOCK_ENABLE�� ����� �ƶ�����
    // TABLESPACE_LOCK_ENABLE ����� �����Ͽ�, TABLESPACE DDL�� ����ϴ� �ʴ� ����������
    // TBS LIST, TBS NODE, DBF NODE �� ���� LOCK ��û�� ���� ���ϵ��� ó���Ѵ�.


    // ���̺��� ���̺����̽��鿡 ���Ͽ� INTENTION ����� ȹ���Ѵ�.
    // smiValidateAndLockTable(), smiTable::lockTable, Ŀ�� open�� ȣ��
    static IDE_RC lockAndValidateTBS(
                      void                * aTrans,      /* [IN]  */
                      scSpaceID             aSpaceID,    /* [IN]  */
                      sctTBSLockValidOpt    aTBSLvOpt,   /* [IN]  */
                      idBool                aIsIntent,   /* [IN]  */
                      idBool                aIsExclusive,/* [IN]  */
                      ULong                 aLockWaitMicroSec ); /* [IN] */

    // ���̺�� ���õ� ���̺����̽��鿡 ���Ͽ� INTENTION ����� ȹ���Ѵ�.
    // smiValidateAndLockTable(), smiTable::lockTable, Ŀ�� open�� ȣ��
    static IDE_RC lockAndValidateRelTBSs(
                    void                 * aTrans,           /* [IN] */
                    void                 * aTable,           /* [IN] */
                    sctTBSLockValidOpt     aTBSLvOpt,        /* [IN] */
                    idBool                 aIsIntent,        /* [IN] */
                    idBool                 aIsExclusive,     /* [IN] */
                    ULong                  aLockWaitMicroSec ); /* [IN] */

    // Tablespace Node�� ��� ȹ�� ( ���� : Tablespace ID )
    static IDE_RC lockTBSNodeByID( void              * aTrans,
                                   scSpaceID           aSpaceID,
                                   idBool              aIsIntent,
                                   idBool              aIsExclusive,
                                   ULong               aLockWaitMicroSec,
                                   sctTBSLockValidOpt  aTBSLvOpt,
                                   idBool            * aLocked,
                                   sctLockHier       * aLockHier );

    // Tablespace Node�� ��� ȹ�� ( ���� : Tablespace Node )
    static IDE_RC lockTBSNode( void              * aTrans,
                               sctTableSpaceNode * aSpaceNode,
                               idBool              aIsIntent,
                               idBool              aIsExclusive,
                               ULong               aLockWaitMicroSec,
                               sctTBSLockValidOpt  aTBSLvOpt,
                               idBool           *  aLocked,
                               sctLockHier      *  aLockHier );



    // Tablespace Node�� ��� ȹ�� ( ���� : Tablespace ID )
    static IDE_RC lockTBSNodeByID( void              * aTrans,
                                   scSpaceID           aSpaceID,
                                   idBool              aIsIntent,
                                   idBool              aIsExclusive,
                                   sctTBSLockValidOpt  aTBSLvOpt );

    // Tablespace Node�� ��� ȹ�� ( ���� : Tablespace Node )
    static IDE_RC lockTBSNode( void              * aTrans,
                               sctTableSpaceNode * aSpaceNode,
                               idBool              aIsIntent,
                               idBool              aIsExclusive,
                               sctTBSLockValidOpt  aTBSLvOpt );

    // ������ Tablespace�� ���� Ư�� Action�� �����Ѵ�.
    static IDE_RC doAction4EachTBS( idvSQL           * aStatistics,
                                    sctAction4TBS      aAction,
                                    void             * aActionArg,
                                    sctActionExecMode  aActionExecMode );

    // DDL_LOCK_TIMEOUT ������Ƽ�� ���� ���ð��� ��ȯ�Ѵ�.
    inline static ULong getDDLLockTimeOut();

    /* BUG-34187 ������ ȯ�濡�� �����ÿ� �������ø� ȥ���ؼ� ��� �Ұ��� �մϴ�. */
#if defined(VC_WIN32)
    static void adjustFileSeparator( SChar * aPath );
#endif

    /* ����� ��ȿ�� �˻� �� ����θ� ������ ��ȯ */
    static IDE_RC makeValidABSPath( idBool         aCheckPerm,
                                    SChar*         aValidName,
                                    UInt*          aNameLength,
                                    smiTBSLocation aTBSLocation );

    /* BUG-38621 */
    static IDE_RC makeRELPath( SChar        * aName,
                               UInt         * aNameLength,
                               smiTBSLocation aTBSLocation );

    // PRJ-1548 User Memory TableSpace ���䵵��

    // ��ũ/�޸� ���̺����̽��� ������¸� �����Ѵ�.
    static IDE_RC startTableSpaceBackup( scSpaceID           aSpaceID,
                                         sctTableSpaceNode** aSpaceNode );

    // ��ũ/�޸� ���̺����̽��� ������¸� �����Ѵ�.
    static IDE_RC endTableSpaceBackup( scSpaceID aSpaceID );

    // PRJ-1149 checkpoint ������ DBF ��忡 �����ϱ� ����
    // �����ϴ� �Լ�.
    static void setRedoLSN4DBFileMetaHdr( smLSN* aDiskRedoLSN,
                                          smLSN* aMemRedoLSN );

    // �ֱ� üũ����Ʈ���� ������ ��ũ Redo LSN�� ��ȯ�Ѵ�.
    static smLSN* getDiskRedoLSN()  { return &mDiskRedoLSN; };
    // �ֱ� üũ����Ʈ���� ������ �޸� Redo LSN �迭�� ��ȯ�Ѵ�.
    static smLSN* getMemRedoLSN() { return &mMemRedoLSN; };

    ////////////////////////////////////////////////////////////////////
    // Alter Tablespace Online/Offline �� ���������� ����ϴ� �Լ�
    ////////////////////////////////////////////////////////////////////

    // Alter Tablespace Online/Offline�� ���� ����ó���� �����Ѵ�.
    static IDE_RC checkError4AlterStatus( sctTableSpaceNode    * aTBSNode,
                                          smiTableSpaceState     aNewTBSState );


    // Tablespace�� ���� �������� Backup�� �Ϸ�Ǳ⸦ ��ٸ� ��,
    // Tablespace�� ��� �Ұ����� ���·� �����Ѵ�.
    static IDE_RC wait4BackupAndBlockBackup(
                      sctTableSpaceNode    * aTBSNode,
                      smiTableSpaceState     aTBSSwitchingState );


    // Tablespace�� DISCARDED���·� �ٲٰ�, Loganchor�� Flush�Ѵ�.
    static IDE_RC alterTBSdiscard( sctTableSpaceNode  * aTBSNode );

    // Tablespace�� mMaxTblDDLCommitSCN�� aCommitSCN���� �����Ѵ�.
    static void updateTblDDLCommitSCN( scSpaceID aSpaceID,
                                       smSCN     aCommitSCN);

    // Tablespace�� ���ؼ� aViewSCN���� Drop Tablespace�� �Ҽ��ִ����� �˻��Ѵ�.
    static IDE_RC canDropByViewSCN( scSpaceID aSpaceID,
                                    smSCN     aViewSCN);

    static IDE_RC setMaxFDCntAllDFileOfAllDiskTBS( UInt aMaxFDCnt4File );

private:
    static void findNextSpaceNode( void   *aCurrSpaceNode,
                                   void  **aNextSpaceNode,
                                   UInt    aSkipStateSet);


    static sctTableSpaceNode **mSpaceNodeArray;
    static scSpaceID           mNewTableSpaceID;

    // ���̺����̽� List�� ���� ���ü� ���� �� ��ũ I/O�� ����
    // �������� ���ü� �����Ѵ�.
    static iduMutex            mMutex;

    // ���̺����̽� ������ ����Ÿ���� ������ ���� ���ü� ����
    static iduMutex            mMtxCrtTBS;

    // Drop DBF�� AllocPageCount ���갣�� ���ü� ����
    static iduMutex            mGlobalPageCountCheckMutex;

    // PRJ-1548 ��ũ/�޸� ��������� ������ ���� �ֱٿ�
    // �߻��� üũ����Ʈ ����
    static smLSN               mDiskRedoLSN;
    static smLSN               mMemRedoLSN;

    // BUG-17285 Disk Tablespace �� OFFLINE/DISCARD �� DROP�� �����߻�
    static sctTBSLockValidOpt mTBSLockValidOpt[ SMI_TBSLV_OPER_MAXMAX ];
};

/*
   PROJ-1548
   DDL_LOCK_TIMEOUT ������Ƽ�� ���� ���ð��� ��ȯ�Ѵ�.
*/
inline ULong sctTableSpaceMgr::getDDLLockTimeOut()
{
    return (((smuProperty::getDDLLockTimeOut() == -1) ?
             ID_ULONG_MAX :
             smuProperty::getDDLLockTimeOut()*1000000) );
}

/***********************************************************************
 *
 * Description : ���̺����̽��� Backup �������� üũ�Ѵ�.
 *
 * aSpaceID  - [IN] ���̺����̽� ID
 *
 **********************************************************************/
inline idBool sctTableSpaceMgr::isBackupingTBS( scSpaceID  aSpaceID )
{
    return (((mSpaceNodeArray[aSpaceID]->mState & SMI_TBS_BACKUP)
             == SMI_TBS_BACKUP) ? ID_TRUE : ID_FALSE );
}

/***********************************************************************
 * Description : ���̺����̽� type�� ���� 3���� Ư���� ��ȯ�Ѵ�.
 * [IN]  aSpaceID : �м��Ϸ��� ���̺� �����̽��� ID
 * [OUT] aTableSpaceType : aSpaceID�� �ش��ϴ� ���̺����̽���
 *              1) �ý��� ���̺����̽� �ΰ�?
 *              2) ���� ���̺����̽� �ΰ�?
 *              3) ����Ǵ� ��ġ(MEM, DISK, VOL)
 *                          �� ���� ������ ��ȯ�Ѵ�.
 **********************************************************************/
inline IDE_RC sctTableSpaceMgr::getTableSpaceType( scSpaceID   aSpaceID,
                                                   UInt      * aType )
{
    sctTableSpaceNode * sSpaceNode      = NULL;
    UInt                sTablespaceType = 0;

    sSpaceNode = mSpaceNodeArray[ aSpaceID ];

    IDE_ASSERT( sSpaceNode != NULL );

    switch ( sSpaceNode->mType )
    {
        case SMI_MEMORY_SYSTEM_DICTIONARY:
            sTablespaceType = SMI_TBS_SYSTEM_YES |
                              SMI_TBS_TEMP_NO    |
                              SMI_TBS_LOCATION_MEMORY;
            break;

        case SMI_MEMORY_SYSTEM_DATA:
            sTablespaceType = SMI_TBS_SYSTEM_YES |
                              SMI_TBS_TEMP_NO    |
                              SMI_TBS_LOCATION_MEMORY;
            break;

        case SMI_MEMORY_USER_DATA:
            sTablespaceType = SMI_TBS_SYSTEM_NO  |
                              SMI_TBS_TEMP_NO    |
                              SMI_TBS_LOCATION_MEMORY;
            break;

        case SMI_DISK_SYSTEM_DATA:
            sTablespaceType = SMI_TBS_SYSTEM_YES |
                              SMI_TBS_TEMP_NO    |
                              SMI_TBS_LOCATION_DISK;
            break;

        case SMI_DISK_USER_DATA:
            sTablespaceType = SMI_TBS_SYSTEM_NO  |
                              SMI_TBS_TEMP_NO    |
                              SMI_TBS_LOCATION_DISK;
            break;

        case SMI_DISK_SYSTEM_TEMP:
            sTablespaceType = SMI_TBS_SYSTEM_YES |
                              SMI_TBS_TEMP_YES   |
                              SMI_TBS_LOCATION_DISK;
            break;

        case SMI_DISK_USER_TEMP:
            sTablespaceType = SMI_TBS_SYSTEM_NO  |
                              SMI_TBS_TEMP_YES   |
                              SMI_TBS_LOCATION_DISK;
            break;

        case SMI_DISK_SYSTEM_UNDO:
            sTablespaceType = SMI_TBS_SYSTEM_YES |
                              SMI_TBS_TEMP_NO    |
                              SMI_TBS_LOCATION_DISK;
            break;

        case SMI_VOLATILE_USER_DATA:
            sTablespaceType = SMI_TBS_SYSTEM_NO  |
                              SMI_TBS_TEMP_NO    |
                              SMI_TBS_LOCATION_VOLATILE;
            break;

        default:
            /* ���ǵ��� ���� Ÿ���� ���� */
            return IDE_FAILURE;
    }

    *aType = sTablespaceType;

    return IDE_SUCCESS;
}

inline idBool sctTableSpaceMgr::isMemTableSpace( scSpaceID aSpaceID )
{
    idBool  sIsMemSpace = ID_FALSE;
    UInt    sType       = 0;

    (void)sctTableSpaceMgr::getTableSpaceType( aSpaceID,
                                               &sType );

    if ( ( sType & SMI_TBS_LOCATION_MASK ) == SMI_TBS_LOCATION_MEMORY )
    {
        sIsMemSpace = ID_TRUE;
    }
    else
    {
        sIsMemSpace = ID_FALSE;
    }

    return sIsMemSpace;
}

inline idBool sctTableSpaceMgr::isVolatileTableSpace( scSpaceID aSpaceID )
{
    UInt    sType   = 0;

    (void)sctTableSpaceMgr::getTableSpaceType( aSpaceID,
                                               &sType );

    if ( ( sType & SMI_TBS_LOCATION_MASK ) == SMI_TBS_LOCATION_VOLATILE )
    {
        return ID_TRUE;
    }
    else
    {
        return ID_FALSE;
    }
}

inline idBool sctTableSpaceMgr::isDiskTableSpace( scSpaceID aSpaceID )
{
    idBool  sIsDiskSpace    = ID_FALSE;
    UInt    sType           = 0;

    (void)sctTableSpaceMgr::getTableSpaceType( aSpaceID,
                                               &sType );

    if ( ( sType & SMI_TBS_LOCATION_MASK ) == SMI_TBS_LOCATION_DISK )
    {
        sIsDiskSpace = ID_TRUE;
    }
    else
    {
        sIsDiskSpace = ID_FALSE;
    }

    return sIsDiskSpace;
}

#endif // _O_SCT_TABLE_SPACE_H_


