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

#include <sdtHashModule.h>
#include <sdtWorkArea.h>
#include <sdtWAMap.h>
#include <sdtDef.h>
#include <sdtTempRow.h>
#include <sdtTempPage.h>

/**************************************************************************
 * Description :
 * WorkArea�� �Ҵ�ް� temptableHeader���� �ʱ�ȭ ���ش�.
 * HashGroup, SubGroup�� ������ش�.
 *
 * <IN>
 * aHeader        - ��� Table
 ***************************************************************************/
IDE_RC sdtHashModule::init( void * aHeader )
{
    smiTempTableHeader * sHeader = (smiTempTableHeader*)aHeader;
    sdtWASegment       * sWASegment;
    UInt                 sInitGroupPageCount;
    UInt                 sHashGroupPageCount;    /* HashPartition�� */
    UInt                 sSubGroupPageCount;     /* Flush����ϴ� Group */
    UInt                 sPartmapGroupPageCount; /* HashPartition�� Map */

    sWASegment = (sdtWASegment*)sHeader->mWASegment;

    /* Clustered Hash�� unique üũ ���� */
    IDE_ERROR( SM_IS_FLAG_OFF( sHeader->mTTFlag, SMI_TTFLAG_UNIQUE ) );

    sInitGroupPageCount = sdtWASegment::getAllocableWAGroupPageCount(
        sWASegment,
        SDT_WAGROUPID_INIT );

    sHashGroupPageCount = sInitGroupPageCount
        * sHeader->mWorkGroupRatio / 100;
    sPartmapGroupPageCount = ( (sHashGroupPageCount * ID_SIZEOF( scPageID ) )
                               / SD_PAGE_SIZE ) + 1 ;
    sSubGroupPageCount =
        sInitGroupPageCount - sHashGroupPageCount - sPartmapGroupPageCount;

    IDE_TEST( sdtWASegment::createWAGroup( sWASegment,
                                           SDT_WAGROUPID_HASH,
                                           sHashGroupPageCount,
                                           SDT_WA_REUSE_INMEMORY )
              != IDE_SUCCESS );
    IDE_TEST( sdtWASegment::createWAGroup( sWASegment,
                                           SDT_WAGROUPID_SUB,
                                           sSubGroupPageCount,
                                           SDT_WA_REUSE_FIFO )
              != IDE_SUCCESS );
    IDE_TEST( sdtWASegment::createWAGroup( sWASegment,
                                           SDT_WAGROUPID_PARTMAP,
                                           sPartmapGroupPageCount,
                                           SDT_WA_REUSE_INMEMORY )
              != IDE_SUCCESS );

    IDE_TEST( sdtWAMap::create( sWASegment,
                                SDT_WAGROUPID_PARTMAP,
                                SDT_WM_TYPE_UINT, /* (== scPageID) */
                                sHashGroupPageCount,
                                1, /* aVersionCount */
                                (void*)&sWASegment->mSortHashMapHdr )
              != IDE_SUCCESS );

    sHeader->mModule.mInsert        = insert;
    sHeader->mModule.mSort          = NULL;
    sHeader->mModule.mOpenCursor    = openCursor;
    sHeader->mModule.mCloseCursor   = closeCursor;
    sHeader->mTTState               = SMI_TTSTATE_CLUSTERHASH_PARTITIONING;

    sHeader->mStatsPtr->mExtraStat1 = getPartitionPageCount( sHeader );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * �����Ѵ�. WorkArea�� Cursor���� smiTemp���� �˾Ƽ� �Ѵ�.
 *
 * <IN>
 * aHeader        - ��� Table
 ***************************************************************************/
IDE_RC sdtHashModule::destroy( void * aHeader )
{
    smiTempTableHeader * sHeader = (smiTempTableHeader*)aHeader;

    /* ����Ǹ鼭 ���� ���ġ�� ����Ѵ�. */
    /* Optimal(InMemory)�� ��� �����Ͱ� SortArea�� ��� ũ��� �ȴ�. */
    sHeader->mStatsPtr->mEstimatedOptimalHashSize =
        (ULong)(( SDT_TR_HEADER_SIZE_FULL + sHeader->mRowSize )
                * sHeader->mRowCount
                * 100 / sHeader->mWorkGroupRatio * 1.2);

    return IDE_SUCCESS;
}


/**************************************************************************
 * Description :
 * �����͸� Partition�� ã�� �����Ѵ�
 *
 * <IN>
 * aTable           - ��� Table
 * aValue           - ������ Value
 * aHashValue       - ������ HashValue (HashTemp�� ��ȿ )
 * <OUT>
 * aGRID            - ������ ��ġ
 * aResult          - ������ �����Ͽ��°�?(UniqueViolation Check�� )
 ***************************************************************************/
IDE_RC sdtHashModule::insert(void     * aHeader,
                             smiValue * aValue,
                             UInt       aHashValue,
                             scGRID   * aGRID,
                             idBool   * aResult )
{
    smiTempTableHeader * sHeader = (smiTempTableHeader *)aHeader;
    sdtWASegment       * sWASeg = (sdtWASegment*)sHeader->mWASegment;
    UInt                 sIdx;
    sdtTRPInfo4Insert    sTRPInfo4Insert;
    sdtTRInsertResult    sTRInsertResult;
    scSlotNum            sSlotNo;
    scPageID             sTargetWPID;
    scPageID             sDestNPID;
    UChar              * sWAPagePtr;

    IDE_ERROR( sHeader->mTTState == SMI_TTSTATE_CLUSTERHASH_PARTITIONING );

    /* Unique Hashtemp����. ����  ������ True�� ������ */
    *aResult = ID_TRUE;

    /*************************** ������ ��ġ Ž�� *************************/
    sIdx        = getPartitionIdx( sHeader, aHashValue );
    sTargetWPID = getPartitionWPID( sWASeg, sIdx );

    if( sdtWASegment::getWAPageState( sWASeg, sTargetWPID )
        == SDT_WA_PAGESTATE_NONE )
    {
        /* ������(Partition)�� ���� ���� ����̸� */
        sWAPagePtr = sdtWASegment::getWAPagePtr( sWASeg,
                                                 SDT_WAGROUPID_HASH,
                                                 sTargetWPID );
        IDE_TEST( sdtTempPage::init( sWAPagePtr,
                                     SDT_TEMP_PAGETYPE_HASHPARTITION,
                                     SC_NULL_PID,  /* prev */
                                     SC_NULL_PID,  /* self */
                                     SC_NULL_PID ) /* next */
                  != IDE_SUCCESS );
        IDE_TEST( sdtWASegment::setWAPageState( sWASeg,
                                                sTargetWPID,
                                                SDT_WA_PAGESTATE_INIT )
                  != IDE_SUCCESS );
    }

    /******************************** ���� ******************************/
    sdtTempRow::makeTRPInfo( SDT_TRFLAG_HEAD,
                             0, /*HitSequence */
                             aHashValue,
                             SC_NULL_GRID, /* aChildRID */
                             SC_NULL_GRID, /* aNextRID */
                             sHeader->mRowSize,
                             sHeader->mColumnCount,
                             sHeader->mColumns,
                             aValue,
                             &sTRPInfo4Insert );
    while( 1 )
    {
        sWAPagePtr = sdtWASegment::getWAPagePtr( sWASeg,
                                                 SDT_WAGROUPID_HASH,
                                                 sTargetWPID );

        sdtTempPage::allocSlotDir( sWAPagePtr, &sSlotNo );

        idlOS::memset( &sTRInsertResult, 0, ID_SIZEOF( sdtTRInsertResult ) );
        IDE_TEST( sdtTempRow::appendRowPiece( sWASeg,
                                              sTargetWPID,
                                              sWAPagePtr,
                                              sSlotNo,
                                              0, /*CuttingOffset*/
                                              &sTRPInfo4Insert,
                                              &sTRInsertResult )
                  != IDE_SUCCESS );

        if( sTRInsertResult.mComplete == ID_TRUE )
        {
            break;
        }
        else
        {
            /* �����Ұ� ���Ҵٴ� ���� �� �������� ���� á�ٴ� �� */
            IDE_TEST( movePartition( sHeader,
                                     sTargetWPID,
                                     sWAPagePtr,
                                     &sDestNPID )
                      != IDE_SUCCESS );

            sTRPInfo4Insert.mTRPHeader.mNextGRID.mSpaceID = sWASeg->mSpaceID;
            sTRPInfo4Insert.mTRPHeader.mNextGRID.mPageID  = sDestNPID;

            sTRInsertResult.mTailRowpieceGRID =
                sTRInsertResult.mHeadRowpieceGRID =
                sTRPInfo4Insert.mTRPHeader.mNextGRID;
        }
    }

    *aGRID = sTRPInfo4Insert.mTRPHeader.mNextGRID;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/***************************************************************************
 * Description :
 * Ŀ���� ���ϴ�.
 * �̶� ������ Partitioning ���¿�����, Scan���·� �����Ѵ�.
 *
 * <IN>
 * aHeader        - ��� Table
 * <OUT>
 * aCursor        - ��ȯ��
 ***************************************************************************/
IDE_RC sdtHashModule::openCursor(void * aHeader,
                                 void * aCursor )
{
    smiTempTableHeader * sHeader = (smiTempTableHeader *)aHeader;
    smiTempCursor      * sCursor = (smiTempCursor *)aCursor;
    UInt                 sSlotIdx;

    IDE_ERROR( SM_IS_FLAG_ON( sCursor->mTCFlag, SMI_TCFLAG_FORWARD ) );
    /* Hash���� OrderScan�� �Ұ����� */
    IDE_ERROR( SM_IS_FLAG_OFF( sCursor->mTCFlag, SMI_TCFLAG_ORDEREDSCAN ) );

    /************************* prepare ***********************************
     * Partition���¿��ٰ�, ó������ OpenCursor�� �� ����
     * ���¸� ��ȯ��Ų��. */
    if( sHeader->mTTState == SMI_TTSTATE_CLUSTERHASH_PARTITIONING )
    {
        IDE_TEST( prepareScan( sHeader ) != IDE_SUCCESS );
        sHeader->mStatsPtr->mIOPassNo ++;

        sHeader->mTTState = SMI_TTSTATE_CLUSTERHASH_SCAN;
    }
    else
    {
        IDE_ERROR( sHeader->mTTState == SMI_TTSTATE_CLUSTERHASH_SCAN );
    }

    /************************* Initialize ***********************************/
    sCursor->mWAGroupID     = SDT_WAGROUPID_HASH;
    sCursor->mStoreCursor   = NULL;
    sCursor->mRestoreCursor = NULL;

    if ( SM_IS_FLAG_ON( sCursor->mTCFlag, SMI_TCFLAG_HASHSCAN ) )
    {
        sCursor->mFetch = fetchHashScan;
        sSlotIdx        = getPartitionIdx( sHeader, sCursor->mHashValue );
    }
    else
    {
        sCursor->mFetch = fetchFullScan;
        IDE_ERROR( SM_IS_FLAG_ON( sCursor->mTCFlag, SMI_TCFLAG_FULLSCAN )
                   == ID_TRUE );

        sCursor->mSeq = 0;
        IDE_ERROR( sCursor->mSeq < getPartitionPageCount( sHeader ) );

        sSlotIdx = sCursor->mSeq;
    }

    IDE_TEST( initPartition( sCursor, sSlotIdx ) != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/***************************************************************************
 * Description :
 * �ش� Partition�� ��ȸ�ϱ� ���� �ʱ�ȭ�� �����մϴ�.
 *
 * <IN>
 * aCursor        - ��� Ŀ��
 * aSlotIdx       - Partition�� Index
 ***************************************************************************/
IDE_RC sdtHashModule::initPartition( smiTempCursor * aCursor,
                                     UInt            aSlotIdx )
{
    smiTempTableHeader * sHeader = (smiTempTableHeader *)aCursor->mTTHeader;
    sdtWASegment       * sWASeg = (sdtWASegment*)sHeader->mWASegment;
    scPageID             sNPID;

    IDE_ERROR( getPartitionPageCount( sHeader ) > aSlotIdx );

    IDE_TEST( sdtWAMap::get( &sWASeg->mSortHashMapHdr,
                             aSlotIdx,
                             &sNPID )
              != IDE_SUCCESS );

    if( sNPID == SC_NULL_PID )
    {
        aCursor->mGRID = SC_NULL_GRID;
    }
    else
    {
        SC_MAKE_GRID( aCursor->mGRID,
                      sHeader->mSpaceID,
                      sNPID,
                      -1 );

        IDE_TEST( sdtWASegment::getPageWithFix( sWASeg,
                                                SDT_WAGROUPID_HASH,
                                                aCursor->mGRID,
                                                &aCursor->mWPID,
                                                &aCursor->mWAPagePtr,
                                                &aCursor->mSlotCount )
                  != IDE_SUCCESS );
        IDE_DASSERT( sdtTempPage::getType( aCursor->mWAPagePtr )
                     == SDT_TEMP_PAGETYPE_HASHPARTITION );
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * Ŀ���κ��� HashValue�� �������� ���� Row�� �����ɴϴ�.
 *
 * <IN>
 * aCursor        - ��� Cursor
 * <OUT>
 * aRow           - ��� Row
 * aGRID          - ������ Row�� GRID
 ***************************************************************************/
IDE_RC sdtHashModule::fetchHashScan(void    * aCursor,
                                    UChar  ** aRow,
                                    scGRID  * aRowGRID )
{
    idBool               sResult = ID_FALSE;

    IDE_TEST( fetchAtPartition( (smiTempCursor *)aCursor,
                                aRow,
                                aRowGRID,
                                &sResult )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * Ŀ���κ��� ��� Row�� ������� FetchNext�� �����ɴϴ�.
 * �ٸ� Row�� Partition���� ������ �������� �˴ϴ�.
 *
 * <IN>
 * aCursor        - ��� Cursor
 * <OUT>
 * aRow           - ��� Row
 * aGRID          - ������ Row�� GRID
 ***************************************************************************/
IDE_RC sdtHashModule::fetchFullScan(void    * aCursor,
                                    UChar  ** aRow,
                                    scGRID  * aRowGRID )
{
    smiTempCursor      * sCursor = (smiTempCursor *)aCursor;
    smiTempTableHeader * sHeader = sCursor->mTTHeader;
    idBool               sResult = ID_FALSE;

    while( 1 )
    {
        IDE_TEST( fetchAtPartition( sCursor,
                                    aRow,
                                    aRowGRID,
                                    &sResult )
                  != IDE_SUCCESS );
        if ( sResult == ID_FALSE )
        {
            /* �� Partition�� ���� ��ȸ�� ��� ��������, ���� Partition���� */
            sCursor->mSeq ++;
            if ( sCursor->mSeq < getPartitionPageCount( sHeader ) )
            {
                IDE_TEST( initPartition( sCursor, sCursor->mSeq )
                          != IDE_SUCCESS );
            }
            else
            {
                /* ��� Partition�� ��ȸ���� */
                break;
            }
        }
        else
        {
            /* ��ȿ�� ����� ã���� */
            break;
        }
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * Partition���� Row�� �ϳ� Fetch�ؼ� �ø��ϴ�.
 * fetchHashScan, fetchFullScan ��� �� �Լ��� �̿��մϴ�.
 *
 * <IN>
 * aCursor        - ��� Cursor
 * <OUT>
 * aRow           - ��� Row
 * aGRID          - ������ Row�� GRID
 * aResult        - ���� ����
 ***************************************************************************/
IDE_RC sdtHashModule::fetchAtPartition( smiTempCursor * aCursor,
                                        UChar        ** aRow,
                                        scGRID        * aRowGRID,
                                        idBool        * aResult )
{
    smiTempTableHeader * sHeader = aCursor->mTTHeader;
    sdtWASegment       * sWASeg = (sdtWASegment*)sHeader->mWASegment;
    UChar              * sPtr;
    scPageID             sNPID;
    idBool               sResult = ID_FALSE;
    idBool               sIsValidSlot;
    UInt                 sLoop = 0;

    if ( !SC_GRID_IS_NULL( aCursor->mGRID ) )
    {
        while( sResult == ID_FALSE )
        {
            sLoop ++;
            aCursor->mGRID.mOffset ++;
            sdtWASegment::getSlot( aCursor->mWAPagePtr,
                                   aCursor->mSlotCount,
                                   SC_MAKE_OFFSET( aCursor->mGRID ),
                                   &sPtr,
                                   &sIsValidSlot );

            if( sIsValidSlot == ID_FALSE )
            {
                /* Page�� Slot�� ���� ��ȸ�� */
                sNPID = sdtTempPage::getNextPID( sPtr );
                if ( sNPID == SD_NULL_PID )
                {
                    /*��� Page�� ��ȸ�� */
                    break;
                }
                else
                {
                    SC_MAKE_GRID( aCursor->mGRID,
                                  sHeader->mSpaceID,
                                  sNPID,
                                  -1 );

                    IDE_TEST( sdtWASegment::getPageWithFix( sWASeg,
                                                            SDT_WAGROUPID_HASH,
                                                            aCursor->mGRID,
                                                            &aCursor->mWPID,
                                                            &aCursor->mWAPagePtr,
                                                            &aCursor->mSlotCount )
                              != IDE_SUCCESS );

                    IDE_DASSERT( sdtTempPage::getType( aCursor->mWAPagePtr )
                                 == SDT_TEMP_PAGETYPE_HASHPARTITION );
                }
            }
            else
            {
                /* Head TRP���߸� Fetch ����̴�. */
                if ( SM_IS_FLAG_ON( ( (sdtTRPHeader*)sPtr )->mTRFlag,
                                    SDT_TRFLAG_HEAD ) )
                {
                    IDE_TEST( sdtTempRow::filteringAndFetch( aCursor,
                                                             sPtr,
                                                             aRow,
                                                             aRowGRID,
                                                             &sResult )
                              != IDE_SUCCESS );
                }
                else
                {
                    /* nothing to do */
                }
            }
        }// while( sResult == ID_FALSE )
    }
    else
    {
        /* nothing to do */
    }

    sHeader->mStatsPtr->mExtraStat2 =
        IDL_MAX( sHeader->mStatsPtr->mExtraStat2, sLoop );

    *aResult = sResult;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;

}

/**************************************************************************
 * Description :
 * Ŀ���� �ݽ��ϴ�.
 *
 * <IN>
 * aCursor        - ��� Cursor
 ***************************************************************************/
IDE_RC sdtHashModule::closeCursor(void * /*aTempCursor*/)
{
    /* �Ұ� ���� */
    return IDE_SUCCESS;
}


/**************************************************************************
 * Description :
 * �ش� partition�� FlushGroup���� �ű�ϴ�. �׷��� async write ��,
 * partition�� �ұ����� ��ȭ�� �� �ֽ��ϴ�.
 *
 * <IN>
 * aHeader        - ��� Table
 * aSrcWPID       - ������ WPID
 * aSrcPagePtr    - ������ PagePtr
 * <OUT>
 * aTargetNPID    - ����� ���NPID
 ***************************************************************************/
IDE_RC sdtHashModule::movePartition( smiTempTableHeader  * aHeader,
                                     scPageID              aSrcWPID,
                                     UChar               * aSrcPagePtr,
                                     scPageID            * aTargetNPID )
{
    scPageID       sFlushWPID;
    sdtWASegment * sWASeg = (sdtWASegment*)aHeader->mWASegment;

    /* SubGroup���� ������ ���� �ش� ��ġ�� ������ */
    IDE_TEST( sdtWASegment::getFreeWAPage( sWASeg,
                                           SDT_WAGROUPID_SUB,
                                           &sFlushWPID )
              != IDE_SUCCESS );
    IDE_TEST( sdtWASegment::movePage( sWASeg,
                                      SDT_WAGROUPID_HASH,
                                      aSrcWPID,
                                      SDT_WAGROUPID_SUB,
                                      sFlushWPID )
              != IDE_SUCCESS );
    IDE_TEST( sdtWASegment::allocAndAssignNPage( sWASeg,
                                                 sFlushWPID )
              != IDE_SUCCESS );
    *aTargetNPID = sdtWASegment::getNPID( sWASeg, sFlushWPID );
    IDE_TEST( sdtWASegment::writeNPage( sWASeg,
                                        sFlushWPID )
              != IDE_SUCCESS );

    IDE_TEST( sdtTempPage::init( aSrcPagePtr,
                                 SDT_TEMP_PAGETYPE_HASHPARTITION,
                                 SC_NULL_PID,   /* Prev */
                                 SC_NULL_PID,   /* Self */
                                 *aTargetNPID ) /* Next */
              != IDE_SUCCESS );
    aHeader->mStatsPtr->mExtraStat2 ++;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * Scan�ϱ� ���� �غ� ��
 *
 * <IN>
 * aHeader        - ��� Table
 ***************************************************************************/
IDE_RC sdtHashModule::prepareScan( smiTempTableHeader * aHeader )
{
    scPageID         sTargetWPID;
    UInt             sPartitionCount = getPartitionPageCount( aHeader );
    scPageID         sNPID;
    sdtWASegment   * sWASeg = (sdtWASegment*)aHeader->mWASegment;
    UInt             i;

    /* Partition�� HeadPage���� Assign(NPID�Ҵ�)�ϰ� Map�� NPID�� �����Ѵ�.*/
    for( i = 0 ; i < sPartitionCount ; i ++ )
    {
        sTargetWPID = getPartitionWPID( sWASeg, i );

        if( sdtWASegment::getWAPageState( sWASeg, sTargetWPID )
            == SDT_WA_PAGESTATE_INIT )
        {
            /* �����Ͱ� ������ �Ǿ��µ�, �Ҵ��� �ȵ� �������� ��� */
            IDE_TEST( sdtWASegment::allocAndAssignNPage( sWASeg, sTargetWPID )
                      != IDE_SUCCESS );
            sNPID = sdtWASegment::getNPID( sWASeg, sTargetWPID );
            IDE_TEST( sdtWASegment::setDirtyWAPage( sWASeg, sTargetWPID )
                      != IDE_SUCCESS );
        }
        else
        {
            sNPID = SC_NULL_PID;
        }
        IDE_TEST( sdtWAMap::set( &sWASeg->mSortHashMapHdr,
                                 i,
                                 (void*)&sNPID )
                  != IDE_SUCCESS );
    }

    /* Hash�� Sub�� Insert�� ���� �� ������, Insert�� ����Ǿ�����
     * ���ܵ� �ʿ� ����.
     * Hash, SubGroup�� Hash �ϳ��� ��ģ��. */
    IDE_TEST( sdtWASegment::mergeWAGroup( sWASeg,
                                          SDT_WAGROUPID_HASH,
                                          SDT_WAGROUPID_SUB,
                                          SDT_WA_REUSE_LRU )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}
