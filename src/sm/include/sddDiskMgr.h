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
 * $Id: sddDiskMgr.h 82075 2018-01-17 06:39:52Z jina.kim $
 *
 * Description :
 *
 * �� ������ disk �����ڿ� ���� ��� �����̴�.
 *
 * # ����
 *
 * tablespace�� ����Ʈ�� ���µ� datafile �� I/O�� �ʿ��� ��ü ������ ����
 * �� Ŭ������ �ý��� ������ �ϳ� �����Ѵ�.
 *
 **********************************************************************/

#ifndef _O_SDD_DISK_MGR_H_
#define _O_SDD_DISK_MGR_H_ 1

#include <idu.h>
#include <smu.h>
#include <smDef.h>
#include <sdpDef.h>
#include <sddDef.h>
#include <sctDef.h>
#include <smriDef.h>
#include <idCore.h>

class sddDiskMgr
{

public:

    /* ��ũ������ �ʱ�ȭ  */
    static IDE_RC initialize( UInt aMaxFilePageCnt );
    /* ��ũ������ ���� */
    static IDE_RC destroy();

    /* Space Cache ���� */
    static void  setSpaceCache( scSpaceID  aSpaceID,
                                void     * aSpaceCache );

    /* Space Cache ��ȯ */
    static void * getSpaceCache( scSpaceID  aSpaceID );

    /* �α׾�Ŀ�� ���� tablespace ��� ���� �� �ʱ�ȭ */
    static IDE_RC loadTableSpaceNode(
                           idvSQL*            aStatistics,
                           smiTableSpaceAttr* aTableSpaceAttr,
                           UInt               aAnchorOffset );

    /* �α׾�Ŀ�� ���� datafile ������ */
    static IDE_RC loadDataFileNode( idvSQL*           aStatistics,
                                    smiDataFileAttr*  aDataFileAttr,
                                    UInt              aAnchorOffset );

    /* tablespace ��� ���� �� �ʱ�ȭ, datafile ��� ���� */
    static IDE_RC createTableSpace(
                        idvSQL             * aStatistics,
                        void               * aTrans,
                        smiTableSpaceAttr  * aTableSpaceAttr,
                        smiDataFileAttr   ** aDataFileAttr,
                        UInt                 aDataFileAttrCount,
                        smiTouchMode         aTouchMode );

    /* PROJ-1923
     * tablespace ��� ���� �� �ʱ�ȭ, redo ���������� ��� �� */
    static IDE_RC createTableSpace4Redo( void               * aTrans,
                                         smiTableSpaceAttr  * aTableSpaceAttr );

    /* PROJ-1923
     * DBF ��� ���� �� �ʱ�ȭ, redo ���������� ��� �� */
    static IDE_RC createDataFile4Redo( void               * aTrans,
                                       smLSN                aCurLSN,
                                       scSpaceID            aSpaceID,
                                       smiDataFileAttr    * aDataFileAttr );

    /* tablespace ���� (��� ���Ÿ� Ȥ�� ��� �� ���� ����) */
    static IDE_RC removeTableSpace( idvSQL*       aStatistics,
                                    void *        aTrans,
                                    scSpaceID     aTableSpaceID,
                                    smiTouchMode  aTouchMode );

    // ��� ��ũ ���̺����̽��� DBFile����
    // �̵�� ���� �ʿ� ���θ� üũ�Ѵ�.
    static IDE_RC identifyDBFilesOfAllTBS( idvSQL * aStatistics,
                                           idBool   aIsOnCheckPoint );

    // SyncŸ�Կ� ���� ��� ���̺����̽��� Sync�Ѵ�.
    static IDE_RC syncAllTBS( idvSQL    * aStatistics,
                              sddSyncType aSyncType );

    // ���̺����̽��� Dirty�� ����Ÿ������ Sync�Ѵ�. (WRAPPER)
    static IDE_RC syncTBSInNormal( idvSQL    * aStatistics,
                                   scSpaceID   aSpaceID );

    // üũ����Ʈ�� ��� ����Ÿ���Ͽ� üũ����Ʈ ������ �����Ѵ�.
    static IDE_RC doActUpdateAllDBFHdrInChkpt(
                       idvSQL*             aStatistics,
                       sctTableSpaceNode * aSpaceNode,
                       void              * aActionArg );

    // ��� ����Ÿ������ ��Ÿ����� �ǵ��Ͽ�
    // �̵�� �������θ� Ȯ���Ѵ�.
    static IDE_RC doActIdentifyAllDBFiles(
                       idvSQL*              aStatistics,
                       sctTableSpaceNode  * aSpaceNode,
                       void               * aActionArg );

    /* datafile ���� (����������) */
    static IDE_RC createDataFiles( idvSQL          * aStatistics,
                                   void*             aTrans,
                                   scSpaceID         aTableSpaceID,
                                   smiDataFileAttr** aDataFileAttr,
                                   UInt              aDataFileAttrCount,
                                   smiTouchMode      aTouchMode);

    static inline IDE_RC validateDataFileName(
                                     sddTableSpaceNode *  aSpaceNode,
                                     smiDataFileAttr   ** aDataFileAttr,
                                     UInt                 aDataFileAttrCount,
                                     SChar             ** sExistFileName,
                                     idBool            *  aNameExist);

    /* datafile ���� (�����������) �Ǵ� datafile ��常 ���� */
    static IDE_RC removeDataFileFEBT( idvSQL*             aStatistics,
                                      void*               aTrans,
                                      sddTableSpaceNode * sSpaceNode,
                                      sddDataFileNode   * aFileNode,
                                      smiTouchMode        aTouchMode);
    /*
       PROJ-1548
       DROP DBF�� ���� Pending ���� �Լ�
    */
    static IDE_RC removeFilePending( idvSQL            * aStatistics,
                                     sctTableSpaceNode * aSpaceNode,
                                     sctPendingOp      * aPendingOp  );

    /* tablespace�� datafile ũ�� Ȯ�� */
    //PROJ-1671 Bitmap-based Tablespace And Segment Space Management
    static IDE_RC extendDataFileFEBT(
                       idvSQL      *         aStatistics,
                       void        *         aTrans,
                       scSpaceID             aTableSpaceID,
                       ULong                 aSizeWanted,   // page ������ ǥ��
                       sddDataFileNode     * aFileNode );

    /* datafile ����� autoextend �Ӽ� ���� */
    static IDE_RC alterAutoExtendFEBT(
        idvSQL*    aStatistics,
        void*            aTrans,
        scSpaceID        aTableSpaceID,
        SChar           *aDataFileName,
        sddDataFileNode *aFileNode,
        idBool           aAutoExtendMode,
        ULong            aNextSize,
        ULong            aMaxSize);

    /* tablespace�� datafile ũ�� resize */
    static IDE_RC alterResizeFEBT( idvSQL          * aStatistics,
                                   void             * aTrans,
                                   scSpaceID          aTableSpaceID,
                                   SChar            * aDataFileName,
                                   scPageID           aHWM,
                                   ULong              aSizeWanted,
                                   sddDataFileNode  * aFileNode);

    /* datafile ����� datafile �� �Ӽ� ���� */
    static IDE_RC alterDataFileName( idvSQL    * aStatistics,
                                     scSpaceID   aTableSpaceID,
                                     SChar*      aOldFileName,
                                     SChar*      aNewFileName );

    // ���̺����̽��� ����Ÿ������
    // ������ ���� �������� ��ȿ�� ���θ� ��ȯ�Ѵ�.
    static IDE_RC isValidPageID( idvSQL*    aStatistics,
                                 scSpaceID  aTableSpaceID,
                                 scPageID   aPageID,
                                 idBool*    aIsValid );

    static IDE_RC existDataFile( idvSQL *  aStatistics,
                                 scSpaceID aID,
                                 SChar*    aName,
                                 idBool*   aExist);

    static IDE_RC existDataFile( SChar*    aName,
                                 idBool*   aExist);

    /* page �ǵ� #1 */
    static IDE_RC read( idvSQL      * aStatistics,
                        scSpaceID     aTableSpaceID,
                        scPageID      aPageID,
                        UChar       * aBuffer,
                        UInt        * aDataFileID,
                        smLSN       * aOnlineTBSLSN4Idx );
    // PRJ-1149.
    static IDE_RC readPageFromDataFile( idvSQL*           aStatistics,
                                        sddDataFileNode*  aFileNode,
                                        scPageID          aPageID,
                                        ULong             aPageCnt,
                                        UChar*            aBuffer,
                                        UInt*             aState );

    /* page �ǵ� #2 */
    static IDE_RC read( idvSQL*       aStatistics,
                        scSpaceID     aTableSpaceID,
                        scPageID      aPageID,
                        ULong         aPageCount,
                        UChar*        aBuffer );

    /* page ��� #1 */
    static IDE_RC write( idvSQL*       aStatistics,
                         scSpaceID     aTableSpaceID,
                         scPageID      aPageID,
                         UChar*        aBuffer );

    /* page ��� #2 */
    static IDE_RC write4DPath( idvSQL*       aStatistics,
                               scSpaceID     aTableSpaceID,
                               scPageID      aPageID,
                               ULong         aPageCount,
                               UChar*        aBuffer );

    // PR-15004
    static IDE_RC readPageNA( idvSQL*          /* aStatistics */,
                              sddDataFileNode* /* aFileNode */,
                              scSpaceID        /* aSpaceID */,
                              scPageID         /* aPageID */,
                              ULong            /* aPageCnt */,
                              UChar*           /* aBuffer */,
                              UInt*            /* aState */ );

    static IDE_RC writePageNA( idvSQL          * aStatistics,
                               sddDataFileNode * aFileNode,
                               scPageID          aFstPID,
                               ULong             aPageCnt,
                               UChar           * aBuffer,
                               UInt            * aState );

    static IDE_RC writePage2DataFile( idvSQL          * aStatistics,
                                      sddDataFileNode * aFileNode,
                                      scPageID          aFstPID,
                                      ULong             aPageCnt,
                                      UChar           * aBuffer,
                                      UInt            * aState );


    static IDE_RC writePageInBackup(
                         idvSQL*           aStatistics,
                         sddDataFileNode*  aFileNode,
                         scSpaceID         aSpaceID,
                         scPageID          aPageID,
                         ULong             aPageOffset,
                         UChar*            aBuffer,
                         UInt*             aState);

    static IDE_RC writeOfflineDataFile( idvSQL          * aStatistics,
                                        sddDataFileNode * aFileNode,
                                        scPageID          aFstPID,
                                        ULong             aPageCnt,
                                        UChar           * aBuffer,
                                        UInt            * aState );

    static IDE_RC readOfflineDataFile(
                         idvSQL*           aStatistics,
                         sddDataFileNode* /*aFileNode*/,
                         scPageID         /*aPageID*/,
                         ULong            /*aPageCnt*/,
                         UChar*           /*aBuffer*/,
                         UInt*            /*aState*/);

    /* �ش� tablespace ����� �� page ���� ��ȯ */
    static IDE_RC getTotalPageCountOfTBS(
                     idvSQL*          aStatistics,
                     scSpaceID        aTableSpaceID,
                     ULong*           aTotalPageCount );

    static IDE_RC getExtentAnTotalPageCnt(
                     idvSQL*    aStatistics,
                     scSpaceID  aTableSpaceID,
                     UInt*      aExtentPageCout,
                     ULong*     aTotalPageCount );

    /* tablespace ��带 ���*/
    static IDE_RC dumpTableSpaceNode( scSpaceID aTableSpaceID );

    /* ��ũ�������� ���� datafile LRU ����Ʈ�� ��� */
    static IDE_RC dumpOpenDataFileLRUList();

    static UInt     getMaxDataFileSize()
                    { return (mMaxDataFilePageCount); }

    //for PRJ-1149 added functions
    static IDE_RC  getDataFileIDByPageID(idvSQL*      aStatistics,
                                         scSpaceID    aSpaceID,
                                         scPageID     aFstPageID,
                                         sdFileID*    aDataFileID);

    // ����Ÿ������ ������ ���� ��ȯ
    static IDE_RC  getPageRangeInFileByID(idvSQL*            aStatistics,
                                          scSpaceID          aSpaceID,
                                          UInt               aFileID,
                                          scPageID         * aFstPageID,
                                          scPageID         * aLstPageID );

    static IDE_RC completeFileBackup( idvSQL*            aStatistics,
                                      sddDataFileNode*   aDataFileNode );

    // update tablespace info to loganchor
    static IDE_RC updateTBSInfoForChkpt();

    static IDE_RC updateDataFileState(idvSQL*          aStatistics,
                                      sddDataFileNode* aDataFileNode,
                                      smiDataFileState aDataFileState);

    /* ------------------------------------------------
     * PRJ-1149 �̵�������
     * ----------------------------------------------*/
    static IDE_RC checkValidationDBFHdr(
                       idvSQL*           aStatistics,
                       sddDataFileNode*  aFileNode,
                       sddDataFileHdr*   aDBFileHdr,
                       idBool*           aIsMediaFailure );

    static IDE_RC readDBFHdr(
                       idvSQL*           aStatistics,
                       sddDataFileNode*  aFileNode,
                       sddDataFileHdr*   aDBFileHdr );

    static IDE_RC getMustRedoToLSN(
                       idvSQL            * aStatistics,
                       sctTableSpaceNode * aSpaceNode,
                       smLSN             * aMustRedoToLSN,
                       SChar            ** aDBFileName );

    /* ������������� ���Ͽ� ��� */
    static IDE_RC writeDBFHdr( idvSQL*          aStatistics,
                               sddDataFileNode* aDataFile );

    /* call by recovery manager */
    static IDE_RC abortBackupAllTableSpace( idvSQL*  aStatistics );

    /* begin backup */
    static IDE_RC abortBackupTableSpace(
                       idvSQL*            aStatistics,
                       sddTableSpaceNode* aSpaceNode );

    static void unsetTBSBackupState(
                       idvSQL*            aStatistics,
                       sddTableSpaceNode* aSpaceNode );

    static IDE_RC copyDataFile( idvSQL*          aStatistics,
                                sddDataFileNode* aDataFileNode,
                                SChar*           aBackupFilePath );

    //PROJ-2133 incremental backup
    static IDE_RC incrementalBackup(idvSQL                 * aStatistics,                 
                                    smriCTDataFileDescSlot * aDataFileDescSlot,
                                    sddDataFileNode        * aDataFileNode,
                                    smriBISlot             * sBackupInfo );

    static void wait4BackupFileEnd();

    // PRJ-1548 User Memory Tablespace
    // ���������� �������Ŀ� ��� ���̺����̽���
    // DataFileCount�� TotalPageCount�� ����Ͽ� �����Ѵ�.
    static IDE_RC calculateFileSizeOfAllTBS( idvSQL * aStatistics );

    // PRJ-1548 User Memory Tablespace
    // ��ũ ���̺����̽��� ����Ѵ�.
    static IDE_RC backupAllDiskTBS( idvSQL  * aStatistics,
                                    void    * aTrans,
                                    SChar   * aBackupDir );

    //PROJ-2133 incremental backup
    //��ũ ���̺����̽��� incremental ����Ѵ�.
    static IDE_RC incrementalBackupAllDiskTBS( idvSQL     * aStatistics,
                                               void       * aTrans,
                                               smriBISlot * aCommonBackupInfo,
                                               SChar      * aBackupDir );

    // ���̺����̽��� DBF ������ ����Ϸ��۾��� �����Ѵ�.
    static IDE_RC endBackupDiskTBS( idvSQL*   aStatistics,
                                    scSpaceID aSpaceID );

    // ���̺����̽��� �̵������� �ִ� ����Ÿ���� ����� �����.
    static IDE_RC makeMediaRecoveryDBFList(
                           idvSQL            * aStatistics,
                           sctTableSpaceNode * sSpaceNode,
                           smiRecoverType      aRecoveryType,
                           UInt              * aDiskDBFCount,
                           smLSN             * aFromRedoLSN,
                           smLSN             * aToRedoLSN );

    /* TableSapce�� ��� DataFile�� Max Open Count�� �����Ѵ�. */
    static IDE_RC  setMaxFDCnt4AllDFileOfTBS( sctTableSpaceNode* aSpaceNode,
                                              UInt               aMaxFDCnt4File );

    // Restart �ܰ迡�� Offline TBS�� ���� Runtime ��ü���� Free
    static IDE_RC finiOfflineTBSAction(
                      idvSQL*             aStatistics,
                      sctTableSpaceNode * aSpaceNode,
                      void              * /* aActionArg */ );

    // TBS�� �� Extent�� ������ ������ �����Ѵ�.
    static inline UInt getPageCntInExt( scSpaceID aSpaceID );

    //PROJ-2133 incremental backup
    static inline UInt getCurrChangeTrackingThreadCnt()
            { return idCore::acpAtomicGet32( &mCurrChangeTrackingThreadCnt ); }

    /* datafile ���� �� LRU ����Ʈ�� datafile ��� �߰� */
    static IDE_RC openDataFile( sddDataFileNode* aDataFileNode );

    /* datafile �ݱ� �� LRU ����Ʈ�κ��� datafile ��� ���� */
    static IDE_RC closeDataFile( sddDataFileNode* aDataFileNode );

public: // for unit test-driver

    static IDE_RC openDataFile( idvSQL*   aStatistics,
                                scSpaceID aTableSpaceID,
                                scPageID  aPageID );

    static IDE_RC closeDataFile( scSpaceID  aTableSpaceID,
                                 scPageID   aPageID );

    static IDE_RC completeIO( idvSQL*   aStatistics,
                              scSpaceID aTableSpaceID,
                              scPageID  aPageID );

    /* datafile�� ���� I/O�� �ϱ� ���� datafile ��带 ���� */
    static IDE_RC prepareIO( sddDataFileNode*  aDataFileNode );

    /* datafile�� ���� I/O �Ϸ��� datafile ��� ���� */
    static IDE_RC completeIO( sddDataFileNode*  aDataFileNode,
                              sddIOMode         aIOMode );

    // Offline DBF�� ���� Write ���� ���ɿ��� ����
    static void   setEnableWriteToOfflineDBF( idBool aOnOff )
                  { mEnableWriteToOfflineDBF = aOnOff; }

     //PROJ-1671 Bitmap-based Tablespace And Segment Space Management
    static void getExtendableSmallestFileNode( sddTableSpaceNode *sSpaceNode,
                                               sddDataFileNode  **sFileNode );

    static IDE_RC tracePageInFile( UInt            aChkFlag,
                                   ideLogModule    aModule,
                                   UInt            aLevel,
                                   scSpaceID       aSpaceID,
                                   scPageID        aPageID,
                                   const SChar   * aTitle );

    /* PROJ-1923
     * private -> public ��ȯ */
    static IDE_RC shrinkFilePending( idvSQL            * aStatistics,
                                     sctTableSpaceNode * aSpaceNode,
                                     sctPendingOp      * aPendingOp  );

private:

    // BUG-17158
    // Offline DBF�� ���� Write ���� ���ɿ��� ��ȯ
    static idBool isEnableWriteToOfflineDBF()
                  { return mEnableWriteToOfflineDBF; }


    /* ���µ� datafile���� ������� �ʴ� datafile�� �˻� */
    static sddDataFileNode*  findVictim();

public:

    static UInt   mInitialCheckSum;  // ���� page write�ÿ� ����Ǵ� checksum

private:

    // ���µ� datafile ����� LRU ����Ʈ
    static smuList     mOpenFileLRUList;
    // ���µ� datafile ��� ����
    static UInt        mOpenFileLRUCount;

    /* ------------------------------------------------
     * ��� datafile�� ũ��� maxsize(����������)�ʰ��� �� ����.
     * �� ���� �������Ͽ� ���ǵǾ� ������,
     * ��ũ�������� �ʱ�ȭ�ÿ� �Ҵ�ȴ�.
     * ----------------------------------------------*/

    // datafile�� ������ �ִ� max page ����
    static UInt        mMaxDataFilePageCount;

    /* ------------------------------------------------
     * ���µ� ������ �ִ밡 �Ǿ��� ��, ������ ������ ��
     * ���������� ����ϱ����� CV�̴�.
     * ----------------------------------------------*/
    static iduCond           mOpenFileCV;

    //PRJ-1149.
    static iduCond           mBackupCV;
    /* ------------------------------------------------
     * ���� ���� �� �� �ִ� ȭ���� ����� ��ٸ��� �����尡 �ִٴ� ǥ��
     * ----------------------------------------------*/
    static UInt              mWaitThr4Open;
    static PDL_Time_Value    mTimeValue;   // ���ð�����

    // BUG-17158
    // Offline DBF�� ���� Write�� �ؾ��ϴ� ���
    static idBool            mEnableWriteToOfflineDBF;

    // i/o function vector
    static sddReadPageFunc  sddReadPageFuncs[SMI_ONLINE_OFFLINE_MAX];
    static sddWritePageFunc sddWritePageFuncs[SMI_ONLINE_OFFLINE_MAX];

    //PROJ-2133 incremental backup
    static UInt             mCurrChangeTrackingThreadCnt;

};

inline UInt sddDiskMgr::getPageCntInExt( scSpaceID aSpaceID )
{
    sdpSpaceCacheCommon *sTbsCache =
        (sdpSpaceCacheCommon*)getSpaceCache( aSpaceID );

    return sTbsCache->mPagesPerExt;
}
#endif // _O_SDD_DISK_MGR_H_
