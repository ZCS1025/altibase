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
 * $Id $
 **********************************************************************/

#include <sdtWAMap.h>
#include <sdtWorkArea.h>
#include <sdtTempDef.h>
#include <smErrorCode.h>



sdtWMTypeDesc sdtWAMap::mWMTypeDesc[ SDT_WM_TYPE_MAX ] =
{
    {
        SDT_WM_TYPE_UINT,
        "UINT",
        ID_SIZEOF(UInt),
        smuUtility::dumpUInt,
    },
    {
        SDT_WM_TYPE_GRID,
        "GRID",
        ID_SIZEOF(scGRID),
        smuUtility::dumpGRID,
    },
    {
        SDT_WM_TYPE_EXTDESC,
        "EXTDESC",
        ID_SIZEOF(sdpExtDesc),
        smuUtility::dumpExtDesc,
    },
    {
        SDT_WM_TYPE_RUNINFO,
        "RUNINFO",
        ID_SIZEOF(sdtTempMergeRunInfo),
        smuUtility::dumpRunInfo,
    },
    {
        SDT_WM_TYPE_POINTER,
        "POINTER",
        ID_SIZEOF(vULong),
        smuUtility::dumpPointer,
    }
};

/**************************************************************************
 * Description :
 * WAMap�� �����. ������ Group�� Count��ŭ �����.
 * ���� Count�� 0�� ���, ���� Ȯ���Ѵ�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWAGroupID     - ��� Group ID
 * aWMType        - WAMap�� Type
 * aSlotCount     - WAMap�� ��� Slot�� ����
 * aVersionCount  - Versioning�ϱ� ���� ����
 * <OUT>
 * aRet           - ������� WAMap
 ***************************************************************************/
IDE_RC sdtWAMap::create( sdtWASegment * aWASegment,
                         sdtWAGroupID   aWAGID,
                         sdtWMType      aWMType,
                         UInt           aSlotCount,
                         UInt           aVersionCount,
                         void         * aRet )
{
    sdtWAMapHdr    * sWAMapHdr  = (sdtWAMapHdr*)aRet;
    sdtWAGroup     * sWAGrpInfo = sdtWASegment::getWAGroupInfo(
        aWASegment, aWAGID );
    UInt             sSlotSize;

    sWAMapHdr->mWASegment    = (void*)aWASegment;
    sWAMapHdr->mWAGID        = aWAGID;
    sWAMapHdr->mBeginWPID    = sdtWASegment::getFirstWPageInWAGroup( aWASegment,
                                                                     aWAGID );
    sWAMapHdr->mWMType       = aWMType;
    sWAMapHdr->mSlotCount    = aSlotCount;
    sWAMapHdr->mVersionCount = aVersionCount;
    sWAMapHdr->mVersionIdx   = 0;
    sSlotSize                = getSlotSize( (void*)sWAMapHdr );
    sWAMapHdr->mSlotSize     = sSlotSize;

    IDE_DASSERT( sSlotSize < SD_PAGE_SIZE );
    IDE_DASSERT( sSlotSize <= SDT_WAMAP_SLOT_MAX_SIZE );

    IDE_ERROR( sWAGrpInfo->mPolicy == SDT_WA_REUSE_INMEMORY );

    IDE_TEST_RAISE( (ULong)sSlotSize * aSlotCount * aVersionCount >
                    (ULong)sdtWASegment::getWAGroupPageCount( aWASegment, aWAGID )
                    * SD_PAGE_SIZE,
                    ERROR_MANY_SLOT );

    sWAGrpInfo->mMapHdr = aRet;

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERROR_MANY_SLOT );
    {
        IDE_SET( ideSetErrorCode( smERR_ABORT_TooManySlotIndiskTempTable ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * �ش� WAmap�� ��ȿȭ��Ų��.
 *
 * <IN>
 * aWAMap         - ��� WAMap
 ***************************************************************************/
IDE_RC sdtWAMap::disable( void     * aWAMapHdr )
{
    sdtWAMapHdr    * sWAMapHdr  = (sdtWAMapHdr*)aWAMapHdr;

    sWAMapHdr->mWAGID = SDT_WAGROUPID_NONE;

    return IDE_SUCCESS;
}


/**************************************************************************
 * Description :
 * WAMap�� Pointer�� ��� �缳���Ѵ�. Dump���̴�.
 *
 * <IN>
 * aWAMap         - ��� WAMap
 * aWASegment     - ��� WASegment
 ***************************************************************************/
IDE_RC sdtWAMap::resetPtrAddr( void * aWAMapHdr, sdtWASegment * aWASegment )
{
    sdtWAMapHdr    * sWAMapHdr = (sdtWAMapHdr*)aWAMapHdr;
    sdtWAGroup     * sGrpInfo;

    sWAMapHdr->mWASegment = aWASegment;

    if( sWAMapHdr->mWAGID != SDT_WAGROUPID_NONE )
    {
        sGrpInfo = sdtWASegment::getWAGroupInfo( aWASegment,
                                                 sWAMapHdr->mWAGID );
        sGrpInfo->mMapHdr = aWAMapHdr;
    }

    return IDE_SUCCESS;
}

/**************************************************************************
 * Description :
 * WAMap�� ���� ������ �� Slot�� �߰��Ͽ� Ȯ���Ѵ�.
 * ���� �� Ȯ���� ���������� ���� Ȯ���ϴµ�,  aLimitPID ��������
 * Ȯ�� �����ϴ�. Ȯ���� �����ϸ�, idx�� UINT_MAX�� ��ȯ�Ѵ�.
 *
 * <IN>
 * aWAMap         - ��� WAMap
 * aLimitPID      - Ȯ���� �� �ִ� �Ѱ� PID
 * <OUT>
 * aIdx           - Ȯ���� Slot�� ��ġ
 ***************************************************************************/
IDE_RC sdtWAMap::expand( void * aWAMapHdr, scPageID   aLimitPID, UInt * aIdx )
{
    sdtWAMapHdr * sWAMapHdr = (sdtWAMapHdr*)aWAMapHdr;
    scPageID      sPID;
    UInt          sSlotSize;

    IDE_ERROR( sWAMapHdr->mWAGID != SDT_WAGROUPID_NONE );

    sSlotSize = sWAMapHdr->mSlotSize;
    /* SlotCount�� �����Ͽ����� �߰� �������� �ʿ�� �ϴ� ��� */
    if( ( sWAMapHdr->mSlotCount * sSlotSize * sWAMapHdr->mVersionCount )
        / SD_PAGE_SIZE !=
        ( ( sWAMapHdr->mSlotCount + 1 ) * sSlotSize * sWAMapHdr->mVersionCount
          / SD_PAGE_SIZE ) )
    {
        sPID = sWAMapHdr->mBeginWPID
            + ( ( sWAMapHdr->mSlotCount + 1 )
                * sSlotSize * sWAMapHdr->mVersionCount
                / SD_PAGE_SIZE );
        if( sPID >= aLimitPID )
        {
            (*aIdx) = SDT_WASLOT_UNUSED;
        }
        else
        {
            (*aIdx) = sWAMapHdr->mSlotCount;
            sWAMapHdr->mSlotCount ++;
        }
    }
    else
    {
        (*aIdx) = sWAMapHdr->mSlotCount;
        sWAMapHdr->mSlotCount ++;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

void sdtWAMap::dumpWAMap( void        * aMapHdr,
                          SChar       * aOutBuf,
                          UInt          aOutSize )
{
    sdtWAMapHdr * sMapHdr = (sdtWAMapHdr*)aMapHdr;
    UChar       * sSlotPtr;
    idBool        sSkip = ID_TRUE;
    UInt          i;
    UInt          j;

    if( aMapHdr != NULL )
    {
        idlVA::appendFormat( aOutBuf,
                             aOutSize,
                             "DUMP WAMAP:%s\n"
                             "       SegPtr   :0x%"ID_xINT64_FMT"\n"
                             "       GroupID  :%"ID_UINT32_FMT"\n"
                             "       BeginWPID:%"ID_UINT32_FMT"\n"
                             "       SlotCount:%"ID_UINT32_FMT"\n",
                             mWMTypeDesc[ sMapHdr->mWMType ].mName,
                             sMapHdr->mWASegment,
                             sMapHdr->mWAGID,
                             sMapHdr->mBeginWPID,
                             sMapHdr->mSlotCount );

        if( sMapHdr->mWAGID != SDT_WAGROUPID_NONE )
        {
            for( i = 0 ; i< sMapHdr->mSlotCount ; i ++ )
            {
                IDE_TEST( getSlotPtr( sMapHdr,
                                      i,
                                      getSlotSize( aMapHdr ),
                                      (void**)& sSlotPtr )
                          != IDE_SUCCESS );

                sSkip = ID_TRUE;
                for( j = 0 ; j < getSlotSize( aMapHdr ) ; j ++ )
                {
                    if( sSlotPtr[ j ] )
                    {
                        sSkip = ID_FALSE;
                        break;
                    }
                }

                if( sSkip == ID_FALSE)
                {
                    idlVA::appendFormat( aOutBuf,
                                         aOutSize,
                                         "%6"ID_UINT32_FMT" : ",
                                         i );

                    mWMTypeDesc[ sMapHdr->mWMType ].mDumpFunc( sSlotPtr,
                                                               aOutBuf,
                                                               aOutSize );

                    idlVA::appendFormat( aOutBuf, aOutSize, "\n" );
                }
            }
        }
    }
    idlVA::appendFormat( aOutBuf, aOutSize, "\n" );

    return;

    IDE_EXCEPTION_END;

    return;
}
