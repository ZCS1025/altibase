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
 * $Id: smmManager.h 82075 2018-01-17 06:39:52Z jina.kim $
 **********************************************************************/

#ifndef _O_SMM_MANAGER_H_
#define _O_SMM_MANAGER_H_ 1

#include <idu.h>
#include <iduMemPool.h>
#include <idp.h>
#include <smDef.h>
#include <smmDef.h>
#include <smmDatabaseFile.h>
#include <smmFixedMemoryMgr.h>
#include <sctTableSpaceMgr.h>
#include <smu.h>
#include <smmPLoadMgr.h>
/*
    ���� SMM�ȿ����� File���� Layer�� ������ ������ ����.
    ���� Layer�� �ڵ忡���� ���� Layer�� �ڵ带 ����� �� ����.

    ----------------
    smmTableSpace    ; Tablespace���� Operation���� (create/alter/drop)
    ----------------
    smmTBSMultiPhase ; Tablespace�� �ٴܰ� �ʱ�ȭ
    ----------------
    smmManager       ; Tablespace�� ���� ����
    smmFPLManager    ; Tablespace Free Page List�� ���� ����
    smmExpandChunk   ; Chunk�� ���α��� ����
    ----------------
 */

class  smmFixedMemoryMgr;

// smmManager�� �ʱ�ȭ �ϴ� ���
typedef enum
{
    // �Ϲ� STARTUP - ��ũ�� �����ͺ��̽� ���� ����
    SMM_MGR_INIT_MODE_NORMAL_STARTUP = 1,
    // CREATEDB���� - ���� ��ũ�� �����ͺ��̽� ������ �������� ����
    SMM_MGR_INIT_MODE_CREATEDB = 2
} smmMgrInitMode ;

// fillPCHEntry �Լ����� ������ �޸��� ���翩�θ� ����
typedef enum smmFillPCHOption {
    SMM_FILL_PCH_OP_NONE = 0 ,
    /** ������ �޸��� ������ PCH ���� Page�� �����Ѵ� */
    SMM_FILL_PCH_OP_COPY_PAGE = 1,
    /** ������ �޸��� �����͸� PCH ���� Page�� �����Ѵ� */
    SMM_FILL_PCH_OP_SET_PAGE = 2
} smmFillPCHOption ;


// allocNewExpandChunk �Լ��� �ɼ�
typedef enum smmAllocChunkOption
{
    SMM_ALLOC_CHUNK_OP_NONE = 0,              // �ɼǾ���
    SMM_ALLOC_CHUNK_OP_CREATEDB         = 1   // createdb �������� �˸�
} smmAllocChunkOption ;



// getDBFile �Լ��� �ɼ�
typedef enum smmGetDBFileOption
{
    SMM_GETDBFILEOP_NONE = 0,        // �ɼ� ����
    SMM_GETDBFILEOP_SEARCH_FILE = 1  // ��� MEM_DB_DIR���� DB������ ã�Ƴ���
} smmGetDBFileOption ;

/*********************************************************************
  �޸� �����ͺ��̽��� ������ ���
 *********************************************************************
  0��° Page���� SMM_DATABASE_META_PAGE_CNT����ŭ�� Page����
  �����ͺ��̽��� �����ϱ� ���� ��Ÿ������ ��ϵȴ�.

  �׸��� �� ���� Page���� Expand Chunk�� ���� �����Ѵ�.

  SMM_DATABASE_META_PAGE_CNT �� 1�̰�, Expand Chunk�� Page���� 10�� �����
  Page ��� ���� �����ϸ� ������ ����.
  --------------------------------------------------
   Page#0 : Base Page
            Membase�� Catalog Table���� ����
  --------------------------------------------------
   Page#1  ~ Page#10  Expand Chunk #0
  --------------------------------------------------
   Page#11 ~ Page#20  Expand Chunk #1
  --------------------------------------------------
 *********************************************************************/

class smmManager
{
private:

    static iduMemPool              mIndexMemPool;

    // �� �����ͺ��̽� Page�� non-durable�� �����͸� ���ϴ� PCH �迭
    static smmPCH                **mPCHArray[SC_MAX_SPACE_COUNT];

    static smmGetPersPagePtrFunc   mGetPersPagePtrFunc;
private :
    // Check Database Dir Exist
    static IDE_RC checkDatabaseDirExist(smmTBSNode * aTBSNode);

    // �����ͺ��̽� Ȯ�忡 ���� ���ο� Expand Chunk�� �Ҵ�ʿ� ����
    // Membase�� �̿� ���� ������ �����Ѵ�.
    static IDE_RC updateMembase4AllocChunk( void     * aTrans,
                                            scPageID   aChunkFirstPID,
                                            scPageID   aChunkLastPID );

    // �����޸� Ǯ�� �ʱ�ȭ�Ѵ�.
    static IDE_RC initializeShmMemPool( smmTBSNode *  aTBSNode );


    // �Ϲ� �޸� Page Pool�� �ʱ�ȭ�Ѵ�.
    static IDE_RC initializeDynMemPool(smmTBSNode * aTBSNode);

    // Tablespace�� �Ҵ�� Page Pool�� �ı��Ѵ�.
    static IDE_RC destroyPagePool( smmTBSNode * aTBSNode );


    // �� �����ͺ��̽� ũ�Ⱑ Ư�� ũ�� �̻�����
    // Ȯ��Ǳ� ���� Trap�� ������.
    static IDE_RC sendDatabaseSizeTrap( UInt aNewPageSize );


    // �����ͺ��̽� Ÿ�Կ� ����
    // �����޸𸮳� �Ϲ� �޸𸮸� ������ �޸𸮷� �Ҵ��Ѵ�
    static IDE_RC allocDynOrShm( smmTBSNode   * aTBSNode,
                                 void        ** aPageHandle,
                                 smmTempPage ** aPage );

    // �����ͺ��̽�Ÿ�Կ�����
    // �ϳ��� Page�޸𸮸� �����޸𸮳� �Ϲݸ޸𸮷� �����Ѵ�.
    static IDE_RC freeDynOrShm( smmTBSNode  * aTBSNode,
                                void        * aPageHandle,
                                smmTempPage * aPage );


    // ���� �Ҵ�� Expand Chunk�ȿ� ���ϴ� Page���� PCH Entry�� �Ҵ��Ѵ�.
    // Chunk���� Free List Info Page�� Page Memory�� �Ҵ��Ѵ�.
    static IDE_RC fillPCHEntry4AllocChunk(smmTBSNode * aTBSNode,
                                          scPageID     aNewChunkFirstPID,
                                          scPageID     aNewChunkLastPID );


    // Tablespace�� Meta Page ( 0�� Page)�� �ʱ�ȭ�Ѵ�.
    static IDE_RC createTBSMetaPage( smmTBSNode   * aTBSNode,
                                     void         * aTrans,
                                     SChar        * aDBName,
                                     scPageID       aDBFilePageCount,
                                     SChar        * aDBCharSet,
                                     SChar        * aNationalCharSet,
                                     idBool         aIsNeedLogging = ID_TRUE );

public:
    // �Ϲ� ���̺��� ������ īŻ�α� ���̺��� ���
    static void               *m_catTableHeader;
    // Temp ���̺��� ������ īŻ�α� ���̺��� ���
    static void               *m_catTempTableHeader;

    /*****************************************************************
       Tablespace�� �ܰ躰 �ʱ�ȭ ��ƾ
     */

    // Memory Tablespace Node�� �ʵ带 �ʱ�ȭ�Ѵ�.
    static IDE_RC initMemTBSNode( smmTBSNode * aTBSNode,
                                  smiTableSpaceAttr * aTBSAttr );

    // Tablespace�� ���� ��ü(DB File) ���� �ý��� �ʱ�ȭ
    static IDE_RC initMediaSystem( smmTBSNode * aTBSNode );


    // Tablespace�� ���� Page���� �ý��� �ʱ�ȭ
    static IDE_RC initPageSystem( smmTBSNode  * aTBSNode );

    /*****************************************************************
       Tablespace�� �ܰ躰 ���� ��ƾ
     */
    // Tablespace Node�� �ı��Ѵ�.
    static IDE_RC finiMemTBSNode(smmTBSNode * aTBSNode);


    // Tablespace�� ���� ��ü(DB File) ���� �ý��� ����
    static IDE_RC finiMediaSystem(smmTBSNode * aTBSNode);

    // Tablespace�� ���� Page���� �ý��� ����
    static IDE_RC finiPageSystem(smmTBSNode * aTBSNode);

    /*****************************************************************
     */
    // DB���� ũ�Ⱑ OS�� ����ũ�� ���ѿ� �ɸ����� �ʴ��� üũ�Ѵ�.
    static IDE_RC checkOSFileSize( vULong aDBFileSize );

    // DB File ��ü��� ���� ������ �������� �ʱ�ȭ�Ѵ�.
    static IDE_RC initDBFileObjects(smmTBSNode *       aTBSNode,
                                    scPageID           aDBFilePageCount);

    // DB File ��ü�� �����Ѵ�.
    static IDE_RC createDBFileObject( smmTBSNode       * aTBSNode,
                                      UInt               aPingPongNum,
                                      UInt               aFileNum,
                                      scPageID           aFstPageID,
                                      scPageID           aLstPageID,
                                      smmDatabaseFile  * aDBFileObj );

    // DB File��ü�� ���� �ڷᱸ���� �����Ѵ�.
    static IDE_RC finiDBFileObjects( smmTBSNode * aTBSNode );


    // Tablespace�� ��� Checkpoint Image File����
    // Close�ϰ� (����������) �����ش�.
    static IDE_RC closeAndRemoveChkptImages(smmTBSNode * aTBSNode,
                                            idBool       aRemoveImageFiles );


    static IDE_RC removeAllChkptImages(smmTBSNode *   aTBSNode);

    // Membase�� ����ִ� Meta Page(0��)�� Flush�Ѵ�.
    static IDE_RC flushTBSMetaPage(smmTBSNode *   aTBSNode,
                            UInt           aWhichDB);

    // 0�� Page�� ��ũ�κ��� �о
    // MemBase�κ��� �����ͺ��̽����� ������ �о�´�.
    static IDE_RC readMemBaseInfo(smmTBSNode * aTBSNode,
                                  scPageID *   aDbFilePageCount,
                                  scPageID *   aChunkPageCount );

    // 0�� Page�� ��ũ�κ��� �о MemBase�� �����Ѵ�.
    static IDE_RC readMemBaseFromFile(smmTBSNode *   aTBSNode,
                                      smmMemBase *   aMemBase );

    // PROJ 2281 storing bufferpool stat persistantly
    static IDE_RC setSystemStatToMemBase( smiSystemStat      * aSystemStat );
    static IDE_RC getSystemStatFromMemBase( smiSystemStat     * aSystemStat );

    // �����ͺ��̽� ũ�⸦ �̿��Ͽ� �����ͺ��̽� Page���� ���Ѵ�.
    static ULong calculateDbPageCount( ULong aDbSize, ULong aChunkPageCount );

    // �ϳ��� Page�� �����Ͱ� ����Ǵ� �޸� ������ PCH Entry���� �����´�.
    static IDE_RC getPersPagePtr(scSpaceID    aSpaceID, 
                                 scPageID     aPID, 
                                 void      ** aPagePtr);
    static UInt   getPageNoInFile(smmTBSNode * aTBSNode, scPageID aPageID );

    // Space ID�� Valid�� ������ üũ�Ѵ�.
    static idBool isValidSpaceID( scSpaceID aSpaceID );
    // Page ID�� Valid�� ������ üũ�Ѵ�.
    static idBool isValidPageID( scSpaceID aSpaceID, scPageID aPageID );
    // PCH �� �޸𸮿� �ִ��� üũ�Ѵ�.
    static idBool isExistPCH( scSpaceID aSpaceID, scPageID aPageID );

    // ���̺� �Ҵ�� �������̸鼭 �޸𸮰� ���� ��� �ִ��� üũ
    // Free Page�̸鼭 ������ �޸� ������ ��� �ִ��� üũ
    static idBool isAllPageMemoryValid(smmTBSNode * aTBSNode);

    /***********************************************************
     * smmTableSpace ���� ȣ���ϴ� �Լ���
     ***********************************************************/
    // Tablespace���� ����� Page �޸� Ǯ�� �ʱ�ȭ�Ѵ�.
    static IDE_RC initializePagePool( smmTBSNode * aTBSNode );

    //CreateDB�� �����ͺ��̽��� ������ �����Ѵ�.
    static IDE_RC createDBFile( void          * aTrans,
                                smmTBSNode    * aTBSNode,
                                SChar         * aTBSName,
                                vULong          aDBPageCnt,
                                idBool          aIsNeedLogging );

    // PROJ-1923 ALTIBASE HDB Disaster Recovery
    // Tablespace�� Meta Page�� �ʱ�ȭ�ϰ� Free Page���� �����Ѵ�.
    static IDE_RC createTBSPages4Redo( smmTBSNode * aTBSNode,
                                       void       * aTrans,
                                       SChar      * aDBName,
                                       scPageID     aDBFilePageCount,
                                       scPageID     aCreatePageCount,
                                       SChar      * aDBCharSet,
                                       SChar      * aNationalCharSet );

    // Tablespace�� Meta Page�� �ʱ�ȭ�ϰ� Free Page���� �����Ѵ�.
    static IDE_RC createTBSPages( smmTBSNode      * aTBSNode,
                                  void            * aTrans,
                                  SChar *           aDBName,
                                  scPageID          aDBFilePageCount,
                                  scPageID          aCreatePageCount,
                                  SChar *           aDBCharSet,
                                  SChar *           aNationalCharSet );

    // ������ �޸𸮸� �Ҵ��ϰ�, �ش� Page�� �ʱ�ȭ�Ѵ�.
    // �ʿ��� ���, ������ �ʱ�ȭ�� ���� �α��� �ǽ��Ѵ�
    static IDE_RC allocAndLinkPageMemory( smmTBSNode * aTBSNode,
                                          void     *   aTrans,
                                          scPageID     aPID,
                                          scPageID     aPrevPID,
                                          scPageID     aNextPID );

    // Dirty Page�� PCH�� ��ϵ� Dirty Flag�� ��� �ʱ�ȭ�Ѵ�.
    static IDE_RC clearDirtyFlag4AllPages(smmTBSNode * aTBSNode );

    /////////////////////////////////////////////////////////////////////
    // fix BUG-17343 loganchor�� Stable/Unstable Chkpt Image�� ���� ����
    //               ������ ����

    static void initCrtDBFileInfo( smmTBSNode * aTBSNode );

    // �־��� ���Ϲ�ȣ�� �ش��ϴ� DBF�� �ϳ��� ������ �Ǿ����� ���ι�ȯ
    static idBool isCreateDBFileAtLeastOne( idBool     * aCreateDBFileOnDisk );

    // create dbfile �÷��׸� �����Ѵ�.
    static void   setCreateDBFileOnDisk( smmTBSNode    * aTBSNode,
                                         UInt            aPingPongNum,
                                         UInt            aFileNum,
                                         idBool          aFlag )
    { (aTBSNode->mCrtDBFileInfo[aFileNum]).mCreateDBFileOnDisk[aPingPongNum]
                 = aFlag; }

    // create dbfile �÷��׸� ��ȯ�Ѵ�.
    static idBool getCreateDBFileOnDisk( smmTBSNode    * aTBSNode,
                                         UInt            aPingPongNum,
                                         UInt            aFileNum )
   { return
     (aTBSNode->mCrtDBFileInfo[aFileNum]).mCreateDBFileOnDisk[aPingPongNum ]; }

    // ChkptImg Attribute �� Loganchor �������� �����Ѵ�.
    static void setAnchorOffsetToCrtDBFileInfo(
                            smmTBSNode * aTBSNode,
                            UInt         aFileNum,
                            UInt         aAnchorOffset )
    { (aTBSNode->mCrtDBFileInfo[aFileNum]).mAnchorOffset = aAnchorOffset; }

private:

    // Ư�� Page�� PCH���� Page Memory�� �Ҵ��Ѵ�.
    static IDE_RC allocPageMemory( smmTBSNode * aTBSNode,
                                   scPageID     aPID );

    // Ư�� Page�� PCH���� Page Memory�� �����Ѵ�.
    static IDE_RC freePageMemory( smmTBSNode * aTBSNode, scPageID aPID );


    // FLI Page�� Next Free Page ID�� ��ũ�� �������鿡 ����
    // PCH���� Page �޸𸮸� �Ҵ��ϰ� Page Header�� Prev/Next�����͸� �����Ѵ�.
    static IDE_RC allocFreePageMemoryList( smmTBSNode * aTBSNode,
                                           void       * aTrans,
                                           scPageID     aHeadPID,
                                           scPageID     aTailPID,
                                           vULong     * aPageCount );

    // PCH�� Page���� Page Header�� Prev/Next�����͸� �������
    // PCH �� Page �޸𸮸� �ݳ��Ѵ�.
    static IDE_RC freeFreePageMemoryList( smmTBSNode * aTBSNode,
                                          void       * aHeadPage,
                                          void       * aTailPage,
                                          vULong     * aPageCount );

    // PCH�� Page���� Page Header�� Prev/Next�����͸� �������
    // FLI Page�� Next Free Page ID�� �����Ѵ�.
    static IDE_RC linkFreePageList( smmTBSNode * aTBSNode,
                                    void       * aTrans,
                                    void       * aHeadPage,
                                    void       * aTailPage,
                                    vULong     * aPageCount );


    static IDE_RC reverseMap(); // bugbug : implement this.

    /*
     * Page�� PCH(Page Control Header)������ �����Ѵ�.
     */
    // PCH �Ҵ�� �ʱ�ȭ
    static IDE_RC allocPCHEntry(smmTBSNode *  aTBSNode,
                                scPageID      a_pid);

    // PCH ����
    static IDE_RC freePCHEntry(smmTBSNode *  aTBSNode,
                                scPageID     a_pid,
                               idBool       aPageFree = ID_TRUE);

    // Page�� PCH(Page Control Header)������ �����Ѵ�.
    // �ʿ信 ����, Page Memory�� ������ �����ϱ⵵ �Ѵ�.
    static IDE_RC fillPCHEntry(smmTBSNode *        aTBSNode,
                               scPageID            aPID,
                               smmFillPCHOption    aFillOption
                                                   = SMM_FILL_PCH_OP_NONE,
                               void              * aPage = NULL);


    // Tablespace���� ��� Page Memory/PCH Entry�� �����Ѵ�.
    static IDE_RC freeAllPageMemAndPCH(smmTBSNode * aTBSNode );

    // ��� PCH �޸𸮸� �����Ѵ�.
    // aPageFree == ID_TRUE�̸� ������ �޸𸮵� �����Ѵ�.
    static IDE_RC freeAll(smmTBSNode * aTBSNode, idBool aPageFree);

    static IDE_RC setupNewPageRange(scPageID aNeedCount,
                                    scPageID *aBegin,
                                    scPageID *aEnd,
                                    scPageID *aCount);

    // ������ ���̽��� Base Page (Page#0)�� ����Ű�� �����͵��� �����Ѵ�.
    static IDE_RC setupCatalogPointers( smmTBSNode * aTBSNode,
                                        UChar *      aBasePage );
    static IDE_RC setupMemBasePointer( smmTBSNode * aTBSNode,
                                       UChar *      aBasePage );

    // ������ ���̽��� ��Ÿ������ �����Ѵ�.
    // ���� ��Ƽ���̽��� ��Ÿ�����δ� MemBase�� Catalog Table������ �ִ�.
    static IDE_RC setupDatabaseMetaInfo(
                      UChar *a_base_page,
                      idBool a_check_version = ID_TRUE);

    // �Ϲ� �޸𸮷� ����Ÿ���� �̹����� ������ �о���δ�.
    static IDE_RC restoreDynamicDBFile( smmTBSNode      * aTBSNode,
                                        smmDatabaseFile * aDatabaseFile );

    // �Ϲ� �޸𸮷� �����ͺ��̽� �̹����� ������ �о���δ�.
    static IDE_RC restoreDynamicDB(smmTBSNode *     aTBSNode);

    // ���� �޸𸮷� �����ͺ��̽� �̹����� ������ �о���δ�.
    static IDE_RC restoreCreateSharedDB( smmTBSNode * aTBSNode,
                                         smmRestoreOption aOp );

    // �ý����� �����޸𸮿� �����ϴ� �����ͺ��̽� ���������� ATTACH�Ѵ�.
    static IDE_RC restoreAttachSharedDB(smmTBSNode *        aTBSNode,
                                        smmShmHeader      * aShmHeader,
                                        smmPrepareOption    aOp );

    // calc need table page count
    static IDE_RC getDBPageCount(smmTBSNode * aTBSNode,
                                 scPageID *   aPageCount); // ��� ȯ�� �����.
    static vULong  getTablPageCount(void *aTable);

    // DB File Loading Message���
    static void printLoadingMessage( smmDBRestoreType aRestoreType );

public:

    /* --------------------
     * [1] Class Control
     * -------------------*/
    smmManager();
    // smmManager�� �ʱ�ȭ�Ѵ�.
    static IDE_RC initializeStatic(  );
    static IDE_RC destroyStatic();

    // Base Page ( 0�� Page ) �� Latch�� �Ǵ�
    static IDE_RC lockBasePage(smmTBSNode * aTBSNode);
    // Base Page ( 0�� Page ) ���� Latch�� Ǭ��.
    static IDE_RC unlockBasePage(smmTBSNode * aTBSNode);

    static IDE_RC initSCN();

    static IDE_RC getDBFileCountFromDisk(UInt  a_stableDB,
                                         UInt *a_cDBFile);

    /* -----------------------
     * [2] Create & Restore DB
     * ----------------------*/

    // Tablespace Node�� �ʱ�ȭ�Ѵ�.
    static IDE_RC initializeTBSNode( smmTBSNode        * aTBSNode );

    static IDE_RC writeDB(smmTBSNode * aTBSNode,
                          SInt     aDbNum,
                          scPageID aStart,
                          scPageID aCount,
                          idBool   aParallel = ID_TRUE);

    // �̵������� ������ �ʿ��� ���̺����̽��� ���θ� ��ȯ
    static idBool isMediaFailureTBS( smmTBSNode * aTBSNode );

    // �����ͺ��̽� restore(Disk -> �޸𸮷� �ε� )�� ���� �غ��Ѵ�.
    static IDE_RC prepareDB (smmPrepareOption  aOp);

    static IDE_RC prepareTBS (smmTBSNode *      aTBSNode,
                              smmPrepareOption  aOp );

    // prepareDB�� ���� Action�Լ�
    static IDE_RC prepareTBSAction( idvSQL            * aStatistics,
                                    sctTableSpaceNode * aTBSNode,
                                    void              * aActionArg );

    // Alter TBS Online�� ���� Tablespace�� Prepare / Restore �Ѵ�.
    static IDE_RC prepareAndRestore( smmTBSNode * aTBSNode );

    // ��ũ �̹����κ��� �����ͺ��̽� �������� �ε��Ѵ�.
    static IDE_RC restoreDB ( smmRestoreOption aOp = SMM_RESTORE_OP_NONE );

    // loganchor�� checkpoint image attribute���� ���� ����
    //  dbfile ���� ��ȯ => restore db�ÿ� ����
    static UInt   getRestoreDBFileCount( smmTBSNode      * aTBSNode);

    static IDE_RC restoreTBS ( smmTBSNode * aTBSNode, smmRestoreOption aOp );

    // restoreDB�� ���� Action�Լ�
    static IDE_RC restoreTBSAction( idvSQL            * aStatistics,
                                    sctTableSpaceNode * aTBSNode,
                                    void              * aActionArg );

    // ù��° ����Ÿ������ �����ϰ� Membase�� �����Ѵ�.
    static IDE_RC openFstDBFilesAndSetupMembase( smmTBSNode * aTBSNode,
                                                 smmRestoreOption aOp,
                                                 UChar      * aReadBuffer );

    /* ------------------------------------------------
     *              *** DB Loading ��� ���� ***
     *
     *  1. loadParallel ()
     *     ==> Thread�� �̿��� �� Table ������ ���ÿ� �ε�
     *         system call�� Page�� 1ȸ�� �Ҹ��� ������
     *         HP, AIX, DEC�� ��쿡�� �����尡 1���� �ε�
     *         pread�� thread safe���� �ʱ� ������.
     *
     *  2. loadSerial ()
     *     ==> �ϳ��� �����尡 DB ȭ���� Chunk( ~M) ������
     *         �ε��Ѵ�. system call Ƚ���� ����.
     *         HP, AIX, DEC�� ��쿡 ������ ����.
     * ----------------------------------------------*/
    // moved to smc
    static IDE_RC loadParallel(smmTBSNode *     aTBSNode);

    // ��ũ���� �����ͺ��̽� �̹����� �޸𸮷� �ε��Ѵ�
    // ( 1���� Thread�� ��� )
    static IDE_RC loadSerial2( smmTBSNode *     aTBSNode);

    // DB File�κ��� ���̺� �Ҵ�� Page���� �޸𸮷� �ε��Ѵ�.
    static IDE_RC loadDbFile(smmTBSNode *     aTBSNode,
                             UInt             aFileNumber,
                             scPageID         aFileMinPID,
                             scPageID         aFileMaxPID,
                             scPageID         aLoadPageCount );

    // �ϳ��� DB���Ͽ� ���� ��� Page�� �޸� Page�� �ε��Ѵ�.
    static IDE_RC loadDbPagesFromFile( smmTBSNode *     aTBSNode,
                                       UInt             aFileNumber,
                                       scPageID         aFileMinPID,
                                       ULong            aLoadPageCount );

    // �����ͺ��̽� ������ �Ϻ� ����(Chunk) �ϳ���
    // �޸� �������� �ε��Ѵ�.
    static IDE_RC loadDbFileChunk( smmTBSNode *     aTBSNode,
                                   smmDatabaseFile * aDbFile,
                                   void            * aAlignedBuffer,
                                   scPageID          aFileMinPID,
                                   scPageID          aChunkStartPID,
                                   ULong             aChunkPageCount );

    static void setLstCreatedDBFileToAllTBS ( );

    static IDE_RC syncDB( sctStateSet aSkipStateSet,
                          idBool aSyncLatch );
    static IDE_RC syncTBS(smmTBSNode * aTBSNode);

    static void setStartupPID(smmTBSNode *     aTBSNode,
                              scPageID         aPid)
    {
        aTBSNode->mStartupPID = aPid;
    }

    static IDE_RC checkExpandChunkProps(smmMemBase * aMemBase);



    // �����ͺ��̽� ���� Page�� Free Page�� �Ҵ�� �޸𸮸� �����Ѵ�
    static IDE_RC freeAllFreePageMemory(void);
    static IDE_RC freeTBSFreePageMemory(smmTBSNode * aTBSNode);

    /* -----------------------
     * [3] persistent  memory
     *     manipulations
     * ----------------------*/

    // �������� Expand Chunk�� �߰��Ͽ� �����ͺ��̽��� Ȯ���Ѵ�.
    static IDE_RC allocNewExpandChunks( smmTBSNode *  aTBSNode,
                                        void       *  aTrans,
                                        UInt          aExpandChunkCount );


    // Ư�� ������ ������ŭ �����ͺ��̽��� Ȯ���Ѵ�.
    static IDE_RC allocNewExpandChunk( smmTBSNode  * aTBSNode,
                                       void        * aTrans,
                                       scPageID      aNewChunkFirstPID,
                                       scPageID      aNewChunkLastPID );

    // �̵�� ������ AllocNewExpandChunk�� ���� Logical Redo�� �����Ѵ�.
    static IDE_RC allocNewExpandChunk4MR( smmTBSNode *  aTBSNode,
                                          scPageID      aNewChunkFirstPID,
                                          scPageID      aNewChunkLastPID,
                                          idBool        aSetFreeListOfMembase,
                                          idBool        aSetNextFreePageOfFPL );

    // DB�κ��� �ϳ��� Page�� �Ҵ�޴´�.
    static IDE_RC allocatePersPage (void       *  aTrans,
                                    scSpaceID     aSpaceID,
                                    void      **  aAllocatedPage);


    // DB�κ��� Page�� ������ �Ҵ�޾� Ʈ����ǿ��� Free Page�� �����Ѵ�.
    // aHeadPage���� aTailPage����
    // Page Header�� Prev/Next�����ͷ� �������ش�.
    static IDE_RC allocatePersPageList (void        *aTrans,
                                        scSpaceID    aSpaceID,
                                        UInt         aPageCount,
                                        void       **aHeadPage,
                                        void       **aTailPage);

    // �ϳ��� Page�� �����ͺ��̽��� �ݳ��Ѵ�
    static IDE_RC freePersPage (void     *   aTrans,
                                scSpaceID    aSpaceID,
                                void     *   aToBeFreePage );

    // �������� Page�� �Ѳ����� �����ͺ��̽��� �ݳ��Ѵ�.
    // aHeadPage���� aTailPage����
    // Page Header�� Prev/Next�����ͷ� ����Ǿ� �־�� �Ѵ�.
    static IDE_RC freePersPageList (void        *aTrans,
                                    scSpaceID    aSpaceID,
                                    void        *aHeadPage,
                                    void        *aTailPage,
                                    smLSN       *aNTALSN );

    /* -----------------------
     * [*] temporary  memory
     *     manipulations
     * ----------------------*/
    static IDE_RC allocateTempPage ( smmTBSNode * aTBSNode,
                                    smmTempPage **a_allocated);
    static IDE_RC freeTempPage ( smmTBSNode *  aTBSNode,
                                 smmTempPage  *a_head,
                                 smmTempPage  *a_tail = NULL);

    //allocate  memory index node
    static IDE_RC allocateIndexPage(smmTempPage **aAllocated);
    static IDE_RC freeIndexPage (smmTempPage  *aHead,
                                 smmTempPage  *aTail=NULL);

    /* -----------------------
     * [*] selective loading..
     * ----------------------*/
    // Runtime �ÿ� Selective Loading�� �����ϴ� ����ΰ�?
    static idBool isRuntimeSelectiveLoadingSupport();


    /* -----------------------
     * [*] retrieval of
     *     the memory manager
     *     informations
     * ----------------------*/
    // Ư�� Page�� PCH�� �����´�.
    static smmPCH          * getPCH(scSpaceID aSpaceID, scPageID aPID );
    // OID�� �޸� �ּҰ����� ��ȯ�Ѵ�.
    static inline IDE_RC getOIDPtr( scSpaceID      aSpaceID,
                                    smOID          aOID,
                                    void        ** aPtr );
    // GRID�� �޸� �ּҰ����� ��ȯ�Ѵ�.
    static IDE_RC getGRIDPtr( scGRID aGRID, void ** aPtr );
    // Ư�� Page�� PCH���� Dirty Flag�� �����´�.
    static idBool            getDirtyPageFlag(scSpaceID aSpaceID,
                                              scPageID  aPageID);

    // Ư�� Page�� S��ġ�� ȹ���Ѵ�. ( ����� X��ġ�� �����Ǿ� �ִ� )
    static IDE_RC            holdPageSLatch(scSpaceID aSpaceID,
                                            scPageID  aPageID);
    // Ư�� Page�� X��ġ�� ȹ���Ѵ�.
    static IDE_RC            holdPageXLatch(scSpaceID aSpaceID,
                                            scPageID  aPageID);
    // Ư�� Page���� ��ġ�� Ǯ���ش�.
    static IDE_RC            releasePageLatch(scSpaceID aSpaceID,
                                              scPageID  aPageID);
    // Membase�� ������ DB File���� �����´�.
    static UInt              getDbFileCount(smmTBSNode * aTBSNode,
                                            SInt         aCurrentDB);

    // �����ͺ��̽� Ȯ�忡 ���� ���ο� Expand Chunk�� �Ҵ�ʿ� ����
    // ���� ���ܳ��� �Ǵ� DB������ ���� ����Ѵ�.
    static IDE_RC            calcNewDBFileCount( smmTBSNode * aTBSNode,
                                                 scPageID     aChunkFirstPID,
                                                 scPageID     aChunkLastPID,
                                                 UInt     *   aNewDBFileCount);

    // Ư�� Page�� ���ϴ� �����ͺ��̽� ���� ��ȣ�� �˾Ƴ���.
    static SInt              getDbFileNo(smmTBSNode * aTBSNode,
                                         scPageID     aPageID);

    // Ư�� DB������ ��� Page�� �����Ͽ��� �ϴ����� �����Ѵ�.
    static scPageID      getPageCountPerFile(smmTBSNode * aTBSNode,
                                             UInt         aDBFileNo );

    static IDE_RC        openOrCreateDBFileinRecovery( smmTBSNode * aTBSNode,
                                                       SInt         aDBFileNo, 
                                                       idBool     * aIsCreated );

    // Disk�� �����ϴ� �����ͺ��̽� Page�� ���� ����Ѵ�
    static IDE_RC        calculatePageCountInDisk(smmTBSNode *  aTBSNode);
    static SInt          getCurrentDB( smmTBSNode * aTBSNode );
    static SInt          getNxtStableDB( smmTBSNode * aTBSNode );

    static vULong        getAllocTempPageCount(smmTBSNode * aTBSNode);
    static vULong        getUsedTempPageCount(smmTBSNode * aTBSNode);

    // �����ͺ��̽� ���� ��ü�� �����Ѵ�.
    // �ʿ��ϴٸ� ��� DB ���丮���� DB������ ã�´� )
    static IDE_RC getDBFile( smmTBSNode *          aTBSNode,
                             UInt                  aStableDB,
                             UInt                  aDBFileNo,
                             smmGetDBFileOption    aOp,
                             smmDatabaseFile    ** aDBFile );


    // �����ͺ��̽� File�� Open�ϰ�, �����ͺ��̽� ���� ��ü�� �����Ѵ�
    static IDE_RC openAndGetDBFile( smmTBSNode *      aTBSNode,
                                    SInt              aStableDB,
                                    UInt              aDBFileNo,
                                    smmDatabaseFile **aDBFile );

    // BUG-34530 
    // SYS_TBS_MEM_DIC���̺����̽� �޸𸮰� �����Ǵ���
    // DicMemBase�����Ͱ� NULL�� �ʱ�ȭ ���� �ʽ��ϴ�.
    static void clearCatalogPointers();

    /* ------------------------------------------------
     * [*] Database Validation for Shared Memory
     * ----------------------------------------------*/
    static void validate(smmTBSNode * aTBSNode)
    {
        if (aTBSNode->mRestoreType != SMM_DB_RESTORE_TYPE_DYNAMIC)
        {
            smmFixedMemoryMgr::validate(aTBSNode);
        }
    }
    static void invalidate(smmTBSNode * aTBSNode)
    {
        if (aTBSNode->mRestoreType != SMM_DB_RESTORE_TYPE_DYNAMIC)
        {
            smmFixedMemoryMgr::invalidate(aTBSNode);
        }
    }

    static smmDBRestoreType    getRestoreType(smmTBSNode * aTBSNode)
    {
        return aTBSNode->mRestoreType;
    }

    /* -------------------------
     * [10] Dump of Memory Mgr
     * -----------------------*/
    static void dumpPage(FILE        *a_fp,
                         smmMemBase * aMemBase,
                         scPageID     a_no,
                         scPageID     a_size = 0);
    static IDE_RC allocPageAlignedPtr(UInt aSize, void**, void**);

    //To fix BUG-5181
    static inline smmDatabaseFile **getDbf(smmTBSNode * aTBSNode)
    {
        return (smmDatabaseFile**)aTBSNode->mDBFile;
    };

    /* id4 - get property */

    static const SChar* getDBDirPath(UInt aWhich)
        { return smuProperty::getDBDir(aWhich); }


    static UInt   getDBMaxPageCount(smmTBSNode * aTBSNode)
    {
        return (UInt)aTBSNode->mDBMaxPageCount;
    }

    static void   getTableSpaceAttr(smmTBSNode * aTBSNode,
                                    smiTableSpaceAttr * aTBSAttr)
    {
        IDE_DASSERT( aTBSNode != NULL );
        IDE_DASSERT( aTBSAttr != NULL );

        idlOS::memcpy(aTBSAttr,
                      &aTBSNode->mTBSAttr,
                      ID_SIZEOF(smiTableSpaceAttr));
        aTBSAttr->mType = aTBSNode->mHeader.mType;
        aTBSAttr->mTBSStateOnLA = aTBSNode->mHeader.mState;
    }

    static void   getTBSAttrFlagPtr(smmTBSNode         * aTBSNode,
                                    UInt              ** aAttrFlagPtr)
    {
        IDE_DASSERT( aTBSNode != NULL );
        IDE_DASSERT( aAttrFlagPtr != NULL );

        *aAttrFlagPtr = &aTBSNode->mTBSAttr.mAttrFlag;
    }

    static void switchCurrentDB(smmTBSNode * aTBSNode)
    {
        aTBSNode->mTBSAttr.mMemAttr.mCurrentDB =
            (aTBSNode->mTBSAttr.mMemAttr.mCurrentDB + 1) % SM_PINGPONG_COUNT;
    }

    // ������ ���̽��� Base Page (Page#0) �� ������ ������ �����Ѵ�.
    // �̿� ���õ� �����δ�  MemBase�� Catalog Table������ �ִ�.
    static IDE_RC setupBasePageInfo( smmTBSNode * aTBSNode,
                                     UChar *      aBasePage );

    // ChkptImageNode�� ���� AnchorOffset�� ���Ѵ�.
    static inline UInt getAnchorOffsetFromCrtDBFileInfo( smmTBSNode* aSpaceNode,
                                                         UInt        aFileNum )
    {
        IDE_ASSERT( aSpaceNode != NULL );
        IDE_ASSERT( aFileNum < aSpaceNode->mHighLimitFile );
        return aSpaceNode->mCrtDBFileInfo[ aFileNum ].mAnchorOffset;
    }

    /* in order to dumpci utility uses server modules */
    static void setCallback4Util( smmGetPersPagePtrFunc aFunc )
    {
        mGetPersPagePtrFunc = aFunc;
    }
private:
    static void dump(FILE     *a_fp,
                     scPageID  a_no);
};

/*
 * Ư�� Page�� PCH���� Dirty Flag�� �����´�.
 *
 * aPageID [IN] Dirty Flag�� ������ Page�� ID
 */
inline idBool
smmManager::getDirtyPageFlag(scSpaceID aSpaceID, scPageID aPageID)
{
    smmPCH * sPCH;

    sPCH = getPCH(aSpaceID, aPageID);

    IDE_DASSERT( sPCH != NULL );

    return sPCH->m_dirty;
}

/*
 * Ư�� Page�� PCH�� �����´�
 *
 * aPID [IN] PCH�� ������ �������� ID
 */
inline smmPCH *
smmManager::getPCH(scSpaceID aSpaceID, scPageID aPID )
{
    IDE_DASSERT( isValidSpaceID( aSpaceID ) == ID_TRUE );
    IDE_DASSERT( isValidPageID( aSpaceID, aPID ) == ID_TRUE );

    return mPCHArray[aSpaceID][ aPID ];
}

/*
 * OID�� �޸� �ּҰ����� ��ȯ�Ѵ�.
 *
 * aOID [IN] �޸� �ּҰ����� ��ȯ�� Object ID
 */
inline IDE_RC smmManager::getOIDPtr( scSpaceID     aSpaceID,
                                     smOID         aOID,
                                     void       ** aPtr )
{
    scOffset sOffset;
    scPageID sPageID;

    sPageID = (scPageID)(aOID >> SM_OFFSET_BIT_SIZE);
    sOffset = (scOffset)(aOID & SM_OFFSET_MASK);

    IDE_TEST( getPersPagePtr( aSpaceID, sPageID, aPtr ) != IDE_SUCCESS );

    (*aPtr) = (void *)( ((UChar *)(*aPtr)) + sOffset  );

    return IDE_SUCCESS ;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/*
 * GRID�� �޸� �ּҰ����� ��ȯ�Ѵ�.
 *
 * aGRID [IN] �޸� �ּҰ����� ��ȯ�� GRID
 */
inline IDE_RC smmManager::getGRIDPtr( scGRID aGRID, void ** aPtr )
{
    IDE_TEST( getPersPagePtr( SC_MAKE_SPACE( aGRID ),
                              SC_MAKE_PID( aGRID ),
                              aPtr )
              != IDE_SUCCESS );

    (*aPtr) = (void *)( ((UChar *)(*aPtr)) + SC_MAKE_OFFSET( aGRID ) );

    return IDE_SUCCESS ;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/*
 * Membase�� ������ DB File���� �����´�.
 *
 * aCurrentDB [IN] Ping/Pong �����ͺ��̽� ��ȣ ( 0 Ȥ�� 1 )
 */

inline UInt
smmManager::getDbFileCount(smmTBSNode * aTBSNode,
                           SInt         aCurrentDB)
{
    IDE_DASSERT( (aCurrentDB == 0) || (aCurrentDB == 1) );
    IDE_DASSERT( aTBSNode->mRestoreType !=
                 SMM_DB_RESTORE_TYPE_NOT_RESTORED_YET );



    return smmDatabase::getDBFileCount(aTBSNode->mMemBase, aCurrentDB);
}

/* Page ID�� �ٰŷ�, �ϳ��� ���������� �������� Page ��ȣ�� ���Ѵ�.
 * Page��ȣ�� 0���� �����Ͽ� 1�� ���������� �����ϴ� ���̴�.
 *
 * �ϳ��� ������������ �������� Expand Chunk�� ������,
 * �ϳ��� Expand Chunk�� �������� ������������ ���� ���� �� �����Ƿ�,
 * ������������ Expand Chunk Page���� ��Ȯ�� ������ �������� ������
 * ������ ���� ������ �ȴ�.
 *
 * �ٷ� �� �ϳ��� ������������ ������ Page���� �ٷ� mPageCountPerFile�̴�.
 *
 * ���������� ù��° ������������ ��� Membase�� Catalog Table�� ���ϴ�
 * Page�� Database metapage�� ���ϰ� �Ǿ� SMM_DATABASE_META_PAGES ��ŭ
 * �� ���� �������� ������ �ȴ� ( ���� 1 ���� Page�� �� ����. )
 *
 */
inline UInt smmManager::getPageNoInFile( smmTBSNode * aTBSNode,
                                         scPageID     aPageID )
{
    UInt          sPageNoInFile;
    // Primitive Operation�̾ aPageID�� ���� Validation�� �� �� ����

    // �����ͺ��̽� Meta Page�� �о�� ���� m_membase���� �о���� ���� ��Ȳ.
    // �� �� getPageCountPerFile���� mPageCountPerFile �� �ʱ�ȭ��
    // �ȵ� �����̴�. ( mPageCountPerFile�� m_membase�� ExpandChunk�� Page��
    // �� �̿��Ͽ� ���Ǳ� ����.
    if ( aPageID < SMM_DATABASE_META_PAGE_CNT )
    {
        sPageNoInFile = aPageID;
    }
    else
    {
        IDE_ASSERT( smmDatabase::getDBFilePageCount(aTBSNode->mMemBase) != 0 );

        sPageNoInFile = (UInt)( ( aPageID - SMM_DATABASE_META_PAGE_CNT )
                                % smmDatabase::getDBFilePageCount(aTBSNode->mMemBase) );

        // ���� ù��° ���������Ͽ� ���� Page���
        if ( aPageID < (smmDatabase::getDBFilePageCount(aTBSNode->mMemBase)
                        + SMM_DATABASE_META_PAGE_CNT) )
        {
            // �� �տ� Meta Page�� �ֱ� ������ �̸�ŭ�� �����Ѵ�.
            sPageNoInFile += SMM_DATABASE_META_PAGE_CNT;
        }
    }

    return sPageNoInFile ;
}

/*
 * Ư�� Page�� ���ϴ� �����ͺ��̽� ���� ��ȣ�� �˾Ƴ���.
 * ( �����ͺ��̽� ������ ��ȣ�� 0���� �����Ѵ� )
 *
 * aPageID [IN]  ��� �����ͺ��̽� ���Ͽ� ���� ������ �˰���� �������� ID
 * return  [OUT] aPageID�� ���� �����ͺ��̽� ������ ��ȣ
 */
inline SInt
smmManager::getDbFileNo(smmTBSNode * aTBSNode, scPageID aPageID)
{
    vULong   sDbFilePageCount;

    IDE_DASSERT( isValidPageID(aTBSNode->mTBSAttr.mID, aPageID) == ID_TRUE );

    // �����ͺ��̽� ��Ÿ �������� �׻� 0�� ���Ͽ� �����Ѵ�.
    // �̷��� ������ 0�� ������ ũ�Ⱑ �ٸ� ���Ϻ���
    // SMM_DATABASE_META_PAGE_CNT��ŭ ũ��.
    if ( aPageID < SMM_DATABASE_META_PAGE_CNT )
    {
        return 0;
    }
    else
    {
        /*
        IDE_ASSERT( smmDatabase::getDBFilePageCount(aTBSNode->mMemBase) != 0 );

        return (SInt)( ( aPageID - SMM_DATABASE_META_PAGE_CNT )
                       / smmDatabase::getDBFilePageCount(aTBSNode->mMemBase) );
        */
        sDbFilePageCount = aTBSNode->mTBSAttr.mMemAttr.mSplitFilePageCount;
        IDE_ASSERT( sDbFilePageCount != 0 );

        return (SInt)( ( aPageID - SMM_DATABASE_META_PAGE_CNT )
                       / sDbFilePageCount  );
    }
}

/*
 * Ư�� DB������ ��� Page�� �����Ͽ��� �ϴ����� �����Ѵ�.
 *
 * PROJ-1490 ������ ����Ʈ ����ȭ�� �޸� �ݳ�
 *
 * DB���Ͼ��� Free Page�� Disk�� ���������� �ʰ�
 * �޸𸮷� �ö����� �ʴ´�.
 * �׷��Ƿ�, DB������ ũ��� DB���Ͽ� ����Ǿ�� �� Page���ʹ�
 * �ƹ��� ���谡 ����.
 *
 * �ϳ��� DB���Ͽ� ��ϵǴ� Page�� ���� ����Ѵ�.
 *
 */
inline scPageID
smmManager::getPageCountPerFile(smmTBSNode * aTBSNode, UInt aDBFileNo )
{
    scPageID sDBFilePageCount;

    sDBFilePageCount = aTBSNode->mTBSAttr.mMemAttr.mSplitFilePageCount;
    IDE_DASSERT( sDBFilePageCount != 0 );

#ifdef DEBUG
    // aTBSNode->mMembase == NULL�϶����� �� �Լ��� ȣ��� �� �ִ�.
    if (aTBSNode->mMemBase != NULL)
    {
        IDE_DASSERT( sDBFilePageCount ==
                     smmDatabase::getDBFilePageCount(aTBSNode->mMemBase) );
    }
#endif

    // �����ͺ��̽� ��Ÿ �������� �׻� 0�� ���Ͽ� �����Ѵ�.
    // �̷��� ������ 0�� ������ ũ�Ⱑ �ٸ� ���Ϻ���
    // SMM_DATABASE_META_PAGE_CNT��ŭ ũ��.
    if ( aDBFileNo == 0 )
    {
         sDBFilePageCount += SMM_DATABASE_META_PAGE_CNT ;
    }

    return sDBFilePageCount ;
}

#if 0
/* TRUE�� �����ϰ� �־ �Լ�������. */

// Page ���� Valid�� ������ üũ�Ѵ�.
inline idBool
smmManager::isValidPageCount( smmTBSNode * /* aTBSNode*/,
                              vULong       /*aPageCount*/ )
{
/*
    IDE_ASSERT( mHighLimitPAGE > 0 );
    return ( aPageCount <= mHighLimitPAGE ) ?
             ID_TRUE : ID_FALSE ;
*/
    return ID_TRUE;
}
#endif

// Page ID�� Valid�� ������ üũ�Ѵ�.
inline idBool
smmManager::isValidSpaceID( scSpaceID aSpaceID )
{
    return (mPCHArray[aSpaceID] == NULL) ? ID_FALSE : ID_TRUE;
}

// Page ID�� Valid�� ������ üũ�Ѵ�.
inline idBool
smmManager::isValidPageID( scSpaceID  aSpaceID ,
                           scPageID   aPageID )
{
    smmTBSNode *sTBSNode;

    sTBSNode = (smmTBSNode*)sctTableSpaceMgr::getSpaceNodeBySpaceID(
        aSpaceID );

    if( sTBSNode != NULL )
    {
        IDE_DASSERT( sTBSNode->mDBMaxPageCount > 0 );

        return ( aPageID <= sTBSNode->mDBMaxPageCount ) ?
            ID_TRUE : ID_FALSE;

    }

    return ID_TRUE;
}

/***********************************************************************
 * Description : ã�� PCH�� �޸𸮿� �����ϰ� �ִ��� Ȯ���Ѵ�.
 *
 * [IN] aSpaceID    - SpaceID
 * [IN] aPageID     - PageID
 **********************************************************************/
inline idBool
smmManager::isExistPCH( scSpaceID aSpaceID, scPageID aPageID )
{

    if ( isValidPageID(aSpaceID, aPageID) == ID_TRUE )
    {
        if ( ( mPCHArray[aSpaceID][aPageID] != NULL ) &&
                ( mPCHArray[aSpaceID][aPageID]->m_page != NULL ) )
        {
            return ID_TRUE;
        }
    }

    return ID_FALSE;
}

#endif // _O_SMM_MANAGER_H_

