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
 * $Id: sdpTableSpace.h 82075 2018-01-17 06:39:52Z jina.kim $
 *
 * ���̺����̽� ������
 *
 * # ����
 *
 * �ϳ� �̻��� �������� ����Ÿ ���Ϸ� ������ ���̺����̽���
 * ������ level�� ������ �ڷᱸ���� ������
 *
 **********************************************************************/

#ifndef _O_SDP_TABLE_SPACE_H_
#define _O_SDP_TABLE_SPACE_H_ 1

#include <sdpDef.h>
#include <sddDef.h>
#include <sddDiskMgr.h>
#include <sdpModule.h>

class sdpTableSpace
{
public:

    /* ��� ���̺����̽� ��� �ʱ�ȭ */
    static IDE_RC initialize();
    /* ��� ���̺����̽� ��� ���� */
    static IDE_RC destroy();

    static IDE_RC createTBS( idvSQL            * aStatistics,
                             smiTableSpaceAttr * aTableSpaceAttr,
                             smiDataFileAttr  ** aDataFileAttr,
                             UInt                aDataFileAttrCount,
                             void*               aTrans );

    /* Tablespace ���� */
    static IDE_RC dropTBS( idvSQL       *aStatistics,
                           void*         aTrans,
                           scSpaceID     aSpaceID,
                           smiTouchMode  aTouchMode );

    /* Tablespace ���� */
    static IDE_RC resetTBS( idvSQL           *aStatistics,
                            scSpaceID         aSpaceID,
                            void             *aTrans );

    /* Tablespace ��ȿȭ */
    static IDE_RC alterTBSdiscard( sddTableSpaceNode * aTBSNode );


    // PRJ-1548 User Memory TableSpace
    /* Disk Tablespace�� ���� Alter Tablespace Online/Offline�� ���� */
    static IDE_RC alterTBSStatus( idvSQL*             aStatistics,
                                  void              * aTrans,
                                  sddTableSpaceNode * aSpaceNode,
                                  UInt                aState );

    /* ����Ÿ���� ���� */
    static IDE_RC createDataFiles( idvSQL            *aStatistics,
                                   void*              aTrans,
                                   scSpaceID          aSpaceID,
                                   smiDataFileAttr  **aDataFileAttr,
                                   UInt               aDataFileAttrCount );

    /* ����Ÿ���� ���� */
    static IDE_RC removeDataFile( idvSQL         *aStatistics,
                                  void*           aTrans,
                                  scSpaceID       aSpaceID,
                                  SChar          *aFileName,
                                  SChar          *aValidDataFileName );

    /* ����Ÿ���� �ڵ�Ȯ�� ��� ���� */
    static IDE_RC alterDataFileAutoExtend( idvSQL     *aStatistics,
                                           void*       aTrans,
                                           scSpaceID   aSpaceID,
                                           SChar      *aFileName,
                                           idBool      aAutoExtend,
                                           ULong       aNextSize,
                                           ULong       aMaxSize,
                                           SChar      *aValidDataFileName );

    /* ����Ÿ���� ��� ���� */
    static IDE_RC alterDataFileName( idvSQL*      aStatistics,
                                     scSpaceID    aSpaceID,
                                     SChar       *aOldName,
                                     SChar       *aNewName );

    /* ����Ÿ���� ũ�� ���� */
    static IDE_RC alterDataFileReSize( idvSQL       *aStatistics,
                                       void         *aTrans,
                                       scSpaceID     aSpaceID,
                                       SChar        *aFileName,
                                       ULong         aSizeWanted,
                                       ULong        *aSizeChanged,
                                       SChar        *aValidDataFileName );


    /*
     * ================================================================
     * Request Function
     * ================================================================
     */
    // callback ���� ����ϱ� ���� public�� ����
    // Tablespace�� OFFLINE��Ų Tx�� Commit�Ǿ��� �� �Ҹ��� Pending�Լ�
    static IDE_RC alterOfflineCommitPending(
                      idvSQL            * aStatistics,
                      sctTableSpaceNode * aTBSNode,
                      sctPendingOp      * aPendingOp );

    // Tablespace�� ONLINE��Ų Tx�� Commit�Ǿ��� �� �Ҹ��� Pending�Լ�
    static IDE_RC alterOnlineCommitPending(
                      idvSQL            * aStatistics,
                      sctTableSpaceNode * aTBSNode,
                      sctPendingOp      * aPendingOp );

    /* BUG-15564 ���̺����̽��� �� �������� ������ ���� ��ȯ */
    static IDE_RC getTotalPageCount( idvSQL*      aStatistics,
                                     scSpaceID    aSpaceID,
                                     ULong       *aTotalPageCount );

    // usedPageLimit���� allocPageCount�� �Լ��� ���� �̸��� �ٲ۴�.
    static IDE_RC getAllocPageCount( idvSQL*      aStatistics,
                                     scSpaceID    aSpaceID,
                                     ULong       *aAllocPageCount );

    static ULong getCachedFreeExtCount( scSpaceID aSpaceID );

    static IDE_RC allocExts( idvSQL          * aStatistics,
                             sdrMtxStartInfo * aStartInfo,
                             scSpaceID         aSpaceID,
                             UInt              aOrgNrExts,
                             sdpExtDesc      * aExtSlot );

    static IDE_RC freeExt( idvSQL       * aStatistics,
                           sdrMtx       * aMtx,
                           scSpaceID      aSpaceID,
                           scPageID       aExtFstPID,
                           UInt         * aNrDone );

    static UInt  getExtPageCount( UChar * aPagePtr );

    static IDE_RC verify( idvSQL*    aStatistics,
                          scSpaceID  aSpaceID,
                          UInt       aFlag );

    static IDE_RC dump( scSpaceID  aSpaceID,
                        UInt       aDumpFlag );

    /* Space Cache �Ҵ� �� �ʱ�ȭ */
    static IDE_RC  doActAllocSpaceCache( idvSQL            * aStatistics,
                                         sctTableSpaceNode * aSpaceNode,
                                         void              * /*aActionArg*/ );
    /* Space Cache ���� */
    static IDE_RC  doActFreeSpaceCache( idvSQL            * aStatistics,
                                        sctTableSpaceNode * aSpaceNode,
                                        void              * /*aActionArg*/ );

    static IDE_RC  freeSpaceCacheCommitPending(
                                        idvSQL               * /*aStatistics*/,
                                        sctTableSpaceNode    * aSpaceNode,
                                        sctPendingOp         * /*aPendingOp*/ );

    static IDE_RC refineDRDBSpaceCache(void);
    static IDE_RC doRefineSpaceCache( idvSQL            * /* aStatistics*/,
                                      sctTableSpaceNode * aSpaceNode,
                                      void              * /*aActionArg*/ );

    //  Tablespace���� ����ϴ� extent ���� ���� ��� ��ȯ
    static smiExtMgmtType getExtMgmtType( scSpaceID aSpaceID );

    /* Tablespace �������� ��Ŀ� ���� Segment �������� ��� ��ȯ */
    static smiSegMgmtType getSegMgmtType( scSpaceID aSpaceID );

    // Tablespace�� extent�� page���� ��ȯ�Ѵ�.
    static UInt getPagesPerExt( scSpaceID     aSpaceID );

    inline static sdpExtMgmtOp* getTBSMgmtOP( scSpaceID aSpaceID );
    inline static sdpExtMgmtOp* getTBSMgmtOP( sddTableSpaceNode * aSpaceNode );
    inline static sdpExtMgmtOp* getTBSMgmtOP( smiExtMgmtType aExtMgmtType );

    static IDE_RC checkPureFileSize(
                           smiDataFileAttr   ** aDataFileAttr,
                           UInt                 aDataFileAttrCount,
                           UInt                 aValidSmallSize );

};

inline sdpExtMgmtOp* sdpTableSpace::getTBSMgmtOP( scSpaceID aSpaceID )
{
    sdpSpaceCacheCommon *sSpaceCache;

    sSpaceCache = (sdpSpaceCacheCommon*) sddDiskMgr::getSpaceCache( aSpaceID );
    IDE_ASSERT( sSpaceCache != NULL );
    IDE_ASSERT( sSpaceCache->mExtMgmtOp != NULL );
    
    return sSpaceCache->mExtMgmtOp;
}

inline sdpExtMgmtOp* sdpTableSpace::getTBSMgmtOP( sddTableSpaceNode * aSpaceNode )
{
    return getTBSMgmtOP( aSpaceNode->mExtMgmtType );
}

inline sdpExtMgmtOp* sdpTableSpace::getTBSMgmtOP( smiExtMgmtType aExtMgmtType )
{
    IDE_ASSERT( aExtMgmtType == SMI_EXTENT_MGMT_BITMAP_TYPE );

    return &gSdptbOp;
}

#endif // _O_SDP_TABLE_SPACE_H_
