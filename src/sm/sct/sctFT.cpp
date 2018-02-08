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
 * $Id: sctFT.cpp 17358 2006-07-31 03:12:47Z bskim $
 *
 * Description :
 *
 * ���̺����̽� ���� Dump
 *
 * X$TABLESPACES
 * X$TABLESPACES_HEADER
 *
 **********************************************************************/

#include <smDef.h>
#include <sctDef.h>
#include <smErrorCode.h>
#include <idl.h>
#include <ide.h>
#include <idu.h>
#include <smu.h>
#include <sdd.h>
#include <smm.h>
#include <svm.h>
#include <sctReq.h>
#include <sctTableSpaceMgr.h>
#include <sctFT.h>

/* ------------------------------------------------
 *  Fixed Table Define for TableSpace
 *
 * Disk, Memory, Volatile Tablespace�� ����������
 * �����ϴ� Field�� ���� Fixed Table
 *
 * X$TABLESPACES_HEADER
 * ----------------------------------------------*/

static iduFixedTableColDesc gTBSHdrTableColDesc[] =
{
    {
        (SChar*)"ID",
        offsetof(sctTbsHeaderInfo, mSpaceID),
        IDU_FT_SIZEOF(sctTbsHeaderInfo, mSpaceID),
        IDU_FT_TYPE_UINTEGER | IDU_FT_COLUMN_INDEX,
        NULL,
        0, 0,NULL // for internal use
    },
    {
        (SChar*)"NAME",
        offsetof(sctTbsHeaderInfo, mName),
        40,
        IDU_FT_TYPE_VARCHAR | IDU_FT_TYPE_POINTER,
        NULL,
        0, 0,NULL // for internal use
    },
    {
        (SChar*)"TYPE",
        offsetof(sctTbsHeaderInfo, mType),
        IDU_FT_SIZEOF(sctTbsHeaderInfo, mType),
        IDU_FT_TYPE_UINTEGER | IDU_FT_COLUMN_INDEX,
        NULL,
        0, 0,NULL // for internal use
    },
    {
        (SChar*)"STATE",
        offsetof(sctTbsHeaderInfo, mStateName),
        IDU_FT_SIZEOF(sctTbsHeaderInfo, mStateName),
        IDU_FT_TYPE_VARCHAR,
        NULL,
        0, 0,NULL // for internal use
    },
    {
        (SChar*)"STATE_BITSET",
        offsetof(sctTbsHeaderInfo, mStateBitset),
        IDU_FT_SIZEOF(sctTbsHeaderInfo, mStateBitset),
        IDU_FT_TYPE_UINTEGER,
        NULL,
        0, 0,NULL // for internal use
    },
    {
        (SChar*)"ATTR_LOG_COMPRESS",
        offsetof(sctTbsHeaderInfo, mAttrLogCompress),
        IDU_FT_SIZEOF(sctTbsHeaderInfo, mAttrLogCompress),
        IDU_FT_TYPE_UINTEGER,
        NULL,
        0, 0,NULL // for internal use
    },
    {
        NULL,
        0,
        0,
        IDU_FT_TYPE_CHAR,
        NULL,
        0, 0,NULL // for internal use
    }
};

iduFixedTableDesc gTbsHeaderTableDesc =
{
    (SChar *)"X$TABLESPACES_HEADER",
    sctFT::buildRecordForTableSpaceHeader,
    gTBSHdrTableColDesc,
    IDU_STARTUP_CONTROL,
    0,
    0,
    IDU_FT_DESC_TRANS_NOT_USE,
    NULL
};

#define SCT_TBS_PERFVIEW_LOG_COMPRESS_OFF (0)
#define SCT_TBS_PERFVIEW_LOG_COMPRESS_ON  (1)


/*
    Tablespace Header Performance View����ü��
    Attribute Flag �ʵ���� �����Ѵ�.

    [IN] aSpaceNode - Tablespace��  Node
    [OUT] aSpaceHeader - Tablespace�� Header����ü
 */
IDE_RC sctFT::getSpaceHeaderAttrFlags(
                             sctTableSpaceNode * aSpaceNode,
                             sctTbsHeaderInfo  * aSpaceHeader)
{

    IDE_DASSERT( aSpaceNode   != NULL );
    IDE_DASSERT( aSpaceHeader != NULL );

    UInt * sAttrFlag;

    switch( sctTableSpaceMgr::getTBSLocation( aSpaceNode->mID ) )
    {
        case SMI_TBS_DISK:
            sddTableSpace::getTBSAttrFlagPtr( (sddTableSpaceNode*)aSpaceNode,
                                              &sAttrFlag );
            break;
        case SMI_TBS_MEMORY:
            smmManager::getTBSAttrFlagPtr( (smmTBSNode*)aSpaceNode,
                                           &sAttrFlag);
            break;
        case SMI_TBS_VOLATILE:
            svmManager::getTBSAttrFlagPtr( (svmTBSNode*)aSpaceNode,
                                           &sAttrFlag );
            break;
        default:
            IDE_ASSERT( 0 );
            break;
    }

    if ( ( *sAttrFlag & SMI_TBS_ATTR_LOG_COMPRESS_MASK )
         == SMI_TBS_ATTR_LOG_COMPRESS_TRUE )
    {
        aSpaceHeader->mAttrLogCompress = SCT_TBS_PERFVIEW_LOG_COMPRESS_ON;
    }
    else
    {
        aSpaceHeader->mAttrLogCompress = SCT_TBS_PERFVIEW_LOG_COMPRESS_OFF;
    }

    return IDE_SUCCESS;
}

/******************************************************************************
 * Abstraction : TBS�� state bitset�� �޾Ƽ� Stable state�� ���ڿ���
 *               ��ȯ�Ѵ�. X$TABLESPACES_HEADER�� TBS�� state��
 *               ���ڿ��� ����ϱ� ���� ����Ѵ�.
 *
 *  aTBSStateBitset - [IN]  ���ڿ��� ��ȯ �� State BitSet
 *  aTBSStateName   - [OUT] ���ڿ��� ��ȯ�� ������
 *****************************************************************************/
void sctFT::getTBSStateName( UInt    aTBSStateBitset,
                             UChar*  aTBSStateName )
{
    aTBSStateBitset &= SMI_TBS_STATE_USER_MASK ;

    switch( aTBSStateBitset )
    {
        case SMI_TBS_OFFLINE:
               idlOS::snprintf((char*)aTBSStateName, SM_DUMP_VALUE_LENGTH, "%s",
                               "OFFLINE" );
            break;
        case SMI_TBS_ONLINE:
               idlOS::snprintf((char*)aTBSStateName, SM_DUMP_VALUE_LENGTH, "%s",
                               "ONLINE" );
            break;
        case SMI_TBS_DROPPED:
               idlOS::snprintf((char*)aTBSStateName, SM_DUMP_VALUE_LENGTH, "%s",
                               "DROPPED" );
            break;
        case SMI_TBS_DISCARDED:
               idlOS::snprintf((char*)aTBSStateName, SM_DUMP_VALUE_LENGTH, "%s",
                               "DISCARDED" );
            break;
        default:
               idlOS::snprintf((char*)aTBSStateName, SM_DUMP_VALUE_LENGTH, "%s",
                               "CHANGING" );
            break;
    }
}



// table space header������ �����ֱ�����
IDE_RC sctFT::buildRecordForTableSpaceHeader(idvSQL              * /*aStatistics*/,
                                             void                *aHeader,
                                             void        * /* aDumpObj */,
                                             iduFixedTableMemory *aMemory)
{
    UInt                i;
    UInt                sState = 0;
    sctTbsHeaderInfo    sSpaceHeader;
    sctTableSpaceNode  *sSpaceNode;
    scSpaceID           sNewTableSpaceID;
    void              * sIndexValues[2];

    IDE_ERROR( aHeader != NULL );
    IDE_ERROR( aMemory != NULL );

    IDE_TEST( sctTableSpaceMgr::lock( NULL /* idvSQL* */) != IDE_SUCCESS );
    sState = 1;

    sNewTableSpaceID = sctTableSpaceMgr::getNewTableSpaceID();

    for( i = 0; i < sNewTableSpaceID; i++ )
    {
        sSpaceNode = 
            (sctTableSpaceNode*) sctTableSpaceMgr::getSpaceNodeBySpaceID(i);

        if( sSpaceNode == NULL )
        {
            continue;
        }

        /* BUG-43006 FixedTable Indexing Filter
         * Indexing Filter�� ����ؼ� ��ü Record�� ���������ʰ�
         * �κи� ������ Filtering �Ѵ�.
         * 1. void * �迭�� IDU_FT_COLUMN_INDEX �� ������ �÷���
         * �ش��ϴ� ���� ������� �־��־�� �Ѵ�.
         * 2. IDU_FT_COLUMN_INDEX�� �÷��� �ش��ϴ� ���� ��� ��
         * �� �־���Ѵ�.
         */
        sIndexValues[0] = &sSpaceNode->mID;
        sIndexValues[1] = &sSpaceNode->mType;
        if ( iduFixedTable::checkKeyRange( aMemory,
                                           gTBSHdrTableColDesc,
                                           sIndexValues )
             == ID_FALSE )
        {
            continue;
        }
        else
        {
            /* Nothing to do */
        }
        sSpaceHeader.mSpaceID = sSpaceNode->mID;
        sSpaceHeader.mName    = sSpaceNode->mName;
        sSpaceHeader.mType    = sSpaceNode->mType;

        sSpaceHeader.mStateBitset = sSpaceNode->mState;
        getTBSStateName( sSpaceNode->mState, sSpaceHeader.mStateName );

        IDE_TEST( getSpaceHeaderAttrFlags( sSpaceNode,
                                           & sSpaceHeader )
                  != IDE_SUCCESS );

        IDE_TEST(iduFixedTable::buildRecord( aHeader,
                                             aMemory,
                                             (void *)&sSpaceHeader)
                 != IDE_SUCCESS);
    }

    sState = 0;
    IDE_TEST( sctTableSpaceMgr::unlock() != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    if( sState != 0 )
    {
        // fix BUG-27153 : [codeSonar] Ignored Return Value
        IDE_ASSERT( sctTableSpaceMgr::unlock() == IDE_SUCCESS );
    }

    return IDE_FAILURE;
}

/* ------------------------------------------------
 *  Fixed Table Define for TableSpace
 * ----------------------------------------------*/

static iduFixedTableColDesc gTableSpaceTableColDesc[] =
{
    {
        (SChar*)"ID",
        offsetof(sctTbsInfo, mSpaceID),
        IDU_FT_SIZEOF(sctTbsInfo, mSpaceID),
        IDU_FT_TYPE_UINTEGER | IDU_FT_COLUMN_INDEX,
        NULL,
        0, 0,NULL // for internal use
    },
    {
        (SChar*)"NEXT_FILE_ID",
        offsetof(sctTbsInfo, mNewFileID),
        IDU_FT_SIZEOF(sctTbsInfo, mNewFileID),
        IDU_FT_TYPE_UINTEGER,
        NULL,
        0, 0,NULL // for internal use
    },
    {
        (SChar*)"EXTENT_MANAGEMENT",
        offsetof(sctTbsInfo, mExtMgmtName),
        IDU_FT_SIZEOF(sctTbsInfo, mExtMgmtName),
        IDU_FT_TYPE_VARCHAR,
        NULL,
        0, 0,NULL // for internal use
    },
    {
        (SChar*)"SEGMENT_MANAGEMENT",
        offsetof(sctTbsInfo, mSegMgmtName),
        IDU_FT_SIZEOF(sctTbsInfo, mSegMgmtName),
        IDU_FT_TYPE_VARCHAR,
        NULL,
        0, 0,NULL // for internal use
    },
    {
        (SChar*)"DATAFILE_COUNT",
        offsetof(sctTbsInfo, mDataFileCount),
        IDU_FT_SIZEOF(sctTbsInfo, mDataFileCount),
        IDU_FT_TYPE_UINTEGER,
        NULL,
        0, 0,NULL // for internal use
    },
    {
        (SChar*)"TOTAL_PAGE_COUNT",
        offsetof(sctTbsInfo, mTotalPageCount),
        IDU_FT_SIZEOF(sctTbsInfo, mTotalPageCount),
        IDU_FT_TYPE_UBIGINT,
        NULL,
        0, 0,NULL // for internal use
    },
    {
        (SChar*)"ALLOCATED_PAGE_COUNT",
        offsetof(sctTbsInfo,mAllocPageCount),
        IDU_FT_SIZEOF(sctTbsInfo,mAllocPageCount),
        IDU_FT_TYPE_UBIGINT,
        NULL,
        0, 0,NULL // for internal use
    },
    {
        (SChar*)"EXTENT_PAGE_COUNT",
        offsetof(sctTbsInfo, mExtPageCount ),
        IDU_FT_SIZEOF(sctTbsInfo,mExtPageCount ),
        IDU_FT_TYPE_UINTEGER,
        NULL,
        0, 0,NULL // for internal use
    },

    {
        (SChar*)"PAGE_SIZE",
        offsetof(sctTbsInfo, mPageSize ),
        IDU_FT_SIZEOF(sctTbsInfo,mPageSize ),
        IDU_FT_TYPE_UINTEGER,
        NULL,
        0, 0,NULL // for internal use
    },

    {
        (SChar*)"CACHED_FREE_EXTENT_COUNT",
        offsetof(sctTbsInfo, mCachedFreeExtCount ),
        IDU_FT_SIZEOF(sctTbsInfo,mCachedFreeExtCount ),
        IDU_FT_TYPE_UBIGINT,
        NULL,
        0, 0,NULL // for internal use
    },

    {
        NULL,
        0,
        0,
        IDU_FT_TYPE_CHAR,
        NULL,
        0, 0,NULL // for internal use
    }
};

iduFixedTableDesc gTableSpaceTableDesc =
{
    (SChar *)"X$TABLESPACES",
    sctFT::buildRecordForTABLESPACES,
    gTableSpaceTableColDesc,
    IDU_STARTUP_CONTROL,
    0,
    0,
    IDU_FT_DESC_TRANS_NOT_USE,
    NULL
};

/*
    Disk Tablespace�� ������ Fix Table ����ü�� �����Ѵ�.
 */
IDE_RC sctFT::getDiskTBSInfo( sddTableSpaceNode * aDiskSpaceNode,
                              sctTbsInfo        * aSpaceInfo )
{
    IDE_DASSERT( aDiskSpaceNode != NULL );
    IDE_DASSERT( aSpaceInfo != NULL );
    IDE_ASSERT( aDiskSpaceNode->mExtMgmtType < SMI_EXTENT_MGMT_MAX );

    aSpaceInfo->mNewFileID = aDiskSpaceNode->mNewFileID;

    idlOS::snprintf((char*)aSpaceInfo->mExtMgmtName, 20, "%s",
                    "BITMAP" );

    if( aDiskSpaceNode->mSegMgmtType == SMI_SEGMENT_MGMT_FREELIST_TYPE )
    {
        idlOS::snprintf((char*)aSpaceInfo->mSegMgmtName, 20, "%s",
                        "MANUAL" );
    }
    else
    {
        if( aDiskSpaceNode->mSegMgmtType == SMI_SEGMENT_MGMT_TREELIST_TYPE )
        {
            idlOS::snprintf((char*)aSpaceInfo->mSegMgmtName, 20, "%s",
                            "AUTO" );
        }
        else
        {
            IDE_ASSERT( aDiskSpaceNode->mSegMgmtType ==
                        SMI_SEGMENT_MGMT_CIRCULARLIST_TYPE );
            
            idlOS::snprintf((char*)aSpaceInfo->mSegMgmtName, 20, "%s",
                            "CIRCULAR" );
        }
    }

    aSpaceInfo->mCachedFreeExtCount = smLayerCallback::getCachedFreeExtCount( aSpaceInfo->mSpaceID );

    aSpaceInfo->mTotalPageCount =
        sddTableSpace::getTotalPageCount(aDiskSpaceNode);


    /* BUG-41895 When selecting the v$tablespaces table, 
     * the server doesn`t check the tablespace`s state whether that is discarded or not */
    if ((aDiskSpaceNode->mHeader.mState & SMI_TBS_DISCARDED) != SMI_TBS_DISCARDED)
    {
        IDE_TEST( smLayerCallback::getAllocPageCount( NULL,
                                            aSpaceInfo->mSpaceID,
                                            &(aSpaceInfo->mAllocPageCount) ) 
                  != IDE_SUCCESS );
    }
    else
    {
        /* discard�� tablespace�� ��쿡�� ������������ �������� ������ �ִ�.
         * ���� mAllocPageCount�� �� �� �������� 0���� �����Ѵ� */
        aSpaceInfo->mAllocPageCount = 0;
    }

    aSpaceInfo->mExtPageCount    = aDiskSpaceNode->mExtPageCount;
    aSpaceInfo->mDataFileCount   = aDiskSpaceNode->mDataFileCount;
    aSpaceInfo->mPageSize        = SD_PAGE_SIZE;


    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/*
    Memory Tablespace�� ������ Fix Table ����ü�� �����Ѵ�.
 */
IDE_RC sctFT::getMemTBSInfo( smmTBSNode   * aMemSpaceNode,
                             sctTbsInfo   * aSpaceInfo )
{
    IDE_DASSERT( aMemSpaceNode != NULL );
    IDE_DASSERT( aSpaceInfo != NULL );

    aSpaceInfo->mNewFileID          = 0;
    aSpaceInfo->mExtPageCount       = aMemSpaceNode->mChunkPageCnt;
    aSpaceInfo->mPageSize           = SM_PAGE_SIZE;
    aSpaceInfo->mCachedFreeExtCount = 0;

    if ( aMemSpaceNode->mMemBase != NULL )
    {
        aSpaceInfo->mDataFileCount      =
            aMemSpaceNode->mMemBase->mDBFileCount[ aMemSpaceNode->mTBSAttr.mMemAttr.
                                                   mCurrentDB ];
        aSpaceInfo->mAllocPageCount =
            aSpaceInfo->mTotalPageCount =
            aMemSpaceNode->mMemBase->mAllocPersPageCount;
    }
    else
    {
        // Offline�̰ų� ���� Restore���� ���� Tablespace�� ��� mMemBase�� NULL
        aSpaceInfo->mAllocPageCount = 0 ;
        aSpaceInfo->mDataFileCount  = 0;
        aSpaceInfo->mTotalPageCount = 0;
    }


    return IDE_SUCCESS;
}


/*
    Volatile Tablespace�� ������ Fixed Table ����ü�� �����Ѵ�.
 */
IDE_RC sctFT::getVolTBSInfo( svmTBSNode   * aVolSpaceNode,
                             sctTbsInfo   * aSpaceInfo )
{
    IDE_DASSERT( aVolSpaceNode != NULL );
    IDE_DASSERT( aSpaceInfo != NULL );

    aSpaceInfo->mNewFileID          = 0;
    aSpaceInfo->mExtPageCount       = aVolSpaceNode->mChunkPageCnt;
    aSpaceInfo->mPageSize           = SM_PAGE_SIZE;
    aSpaceInfo->mCachedFreeExtCount = 0;

    // Volatile Tablespace�� ��� Data File�� ����.
    aSpaceInfo->mDataFileCount      = 0;

    aSpaceInfo->mAllocPageCount =
        aSpaceInfo->mTotalPageCount =
        aVolSpaceNode->mMemBase.mAllocPersPageCount;

    return IDE_SUCCESS;
}


/***********************************************************************
 *
 * Description : ��� ��ũ Tablesapce�� Tablespace���� ��� ������ ����Ѵ�.
 *
 * aHeader  - [IN] Fixed Table ���������
 * aDumpObj - [IN] D$�� ���� ���� ����Ʈ ������
 * aMemory  - [IN] Fixed Table�� ���� Memory Manger
 *
 **********************************************************************/
IDE_RC sctFT::buildRecordForTABLESPACES(idvSQL              * /*aStatistics*/,
                                        void                *aHeader,
                                        void        * /* aDumpObj */,
                                        iduFixedTableMemory *aMemory)
{
    UInt               sState = 0;
    sctTbsInfo         sSpaceInfo;
    sctTableSpaceNode *sNextSpaceNode;
    sctTableSpaceNode *sCurrSpaceNode;
    void             * sIndexValues[1];

    IDE_ERROR( aHeader != NULL );
    IDE_ERROR( aMemory != NULL );

    IDE_DASSERT( (ID_SIZEOF(smiTableSpaceType) == ID_SIZEOF(UInt)) &&
                 (ID_SIZEOF(smiTableSpaceState) == ID_SIZEOF(UInt)) );

    IDE_ASSERT( sctTableSpaceMgr::lock( NULL /* idvSQL* */) == IDE_SUCCESS );
    sState = 1;

    //----------------------------------------
    // X$TABLESPACES�� ���� Record ����
    //----------------------------------------

    sctTableSpaceMgr::getFirstSpaceNode( (void**)&sCurrSpaceNode );

    while( sCurrSpaceNode != NULL )
    {
        // First SpaceNode�� System TBS�̱⿡ drop�� �� ����.
        // ���Ŀ��� Drop���� ���� TBS�� �о�´�.
        IDE_ASSERT( (sCurrSpaceNode->mState & SMI_TBS_DROPPED)
                    != SMI_TBS_DROPPED );

        idlOS::memset( &sSpaceInfo,
                       0x00,
                       ID_SIZEOF( sctTbsInfo ) );

        sSpaceInfo.mSpaceID = sCurrSpaceNode->mID;

        sState = 0;
        IDE_ASSERT( sctTableSpaceMgr::unlock() == IDE_SUCCESS );

        /* BUG-43006 FixedTable Indexing Filter
         * Column Index �� ����ؼ� ��ü Record�� ���������ʰ�
         * �κи� ������ Filtering �Ѵ�.
         * 1. void * �迭�� IDU_FT_COLUMN_INDEX �� ������ �÷���
         * �ش��ϴ� ���� ������� �־��־�� �Ѵ�.
         * 2. IDU_FT_COLUMN_INDEX�� �÷��� �ش��ϴ� ���� ��� ��
         * �� �־���Ѵ�.
         */
        sIndexValues[0] = &sSpaceInfo.mSpaceID;
        if ( iduFixedTable::checkKeyRange( aMemory,
                                           gTableSpaceTableColDesc,
                                           sIndexValues )
             == ID_FALSE )
        {
            IDE_ASSERT( sctTableSpaceMgr::lock( NULL ) == IDE_SUCCESS );
            sState = 1;
            /* MEM TBS�� ��쿡�� Drop Pending ���µ� Drop�� ���̱� ������
             * ���ܽ�Ų�� */
            sctTableSpaceMgr::getNextSpaceNodeWithoutDropped ( (void*)sCurrSpaceNode,
                                                               (void**)&sNextSpaceNode );
            sCurrSpaceNode = sNextSpaceNode;
            continue;
        }
        else
        {
            /* Nothing to do */
        }

        switch( sctTableSpaceMgr::getTBSLocation( sCurrSpaceNode->mID ) )
        {
            case SMI_TBS_DISK:

                // smLayerCallback::getAllocPageCount ���߿�
                // sddDiskMgr::read �����ϴٰ� sctTableSpaceMgr::lock�� ��´�
                // ���⿡�� lock�����Ѵ�.
                IDE_ASSERT( sctTableSpaceMgr::lockGlobalPageCountCheckMutex() 
                    == IDE_SUCCESS );
                sState = 2;

                IDE_TEST( getDiskTBSInfo( (sddTableSpaceNode*)sCurrSpaceNode,
                                          & sSpaceInfo )
                          != IDE_SUCCESS );

                sState = 0;
                IDE_ASSERT( sctTableSpaceMgr::unlockGlobalPageCountCheckMutex() 
                    == IDE_SUCCESS );

                break;

            case SMI_TBS_MEMORY:

                /* To Fix BUG-23912 [AT-F5 ART] MemTBS ���� ��½� Membase�� �����Ҷ�
                 * lockGlobalPageCountCheckMutexfmf ȹ���ؾ���
                 * Dirty Read �߿� membase�� ���ڱ� Null�� �ɼ� �ִ�. */

                IDE_ASSERT( smmFPLManager::lockGlobalPageCountCheckMutex()
                            == IDE_SUCCESS );
                sState = 3;

                IDE_TEST( getMemTBSInfo( (smmTBSNode*)sCurrSpaceNode,
                                          & sSpaceInfo )
                          != IDE_SUCCESS );

                sState = 0;
                IDE_ASSERT( smmFPLManager::unlockGlobalPageCountCheckMutex()
                            == IDE_SUCCESS );
                break;

            case SMI_TBS_VOLATILE:

                /* Membase�� Null�� �Ǵ� ��찡 �������� �����Ƿ�,
                 * Dirty Read�ص� �����ϴ� */
                IDE_TEST( getVolTBSInfo( (svmTBSNode*)sCurrSpaceNode,
                                         & sSpaceInfo )
                          != IDE_SUCCESS );
                break;

            default:
                IDE_ASSERT( 0 );
                break;
        }

        IDE_ASSERT( sctTableSpaceMgr::lock( NULL ) == IDE_SUCCESS );
        sState = 1;

        // [2] make one fixed table record
        IDE_TEST( iduFixedTable::buildRecord( aHeader,
                                              aMemory,
                                              &sSpaceInfo )
                  != IDE_SUCCESS );

        /* MEM TBS�� ��쿡�� Drop Pending ���µ� Drop�� ���̱� ������
         * ���ܽ�Ų�� */
        sctTableSpaceMgr::getNextSpaceNodeWithoutDropped ( (void*)sCurrSpaceNode,
                                                           (void**)&sNextSpaceNode );

        sCurrSpaceNode = sNextSpaceNode;
    }

    sState = 0;
    IDE_ASSERT( sctTableSpaceMgr::unlock() == IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    switch( sState )
    {
        case 3:
            IDE_ASSERT( smmFPLManager::unlockGlobalPageCountCheckMutex()
                        == IDE_SUCCESS );
            break;

        case 2:
            IDE_ASSERT( sctTableSpaceMgr::unlockGlobalPageCountCheckMutex() == IDE_SUCCESS );
            break;

        case 1:
            IDE_ASSERT( sctTableSpaceMgr::unlock() == IDE_SUCCESS );
            break;

        default:
            break;
    }

    return IDE_FAILURE;
}

