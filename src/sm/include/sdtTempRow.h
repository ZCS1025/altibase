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
 

#ifndef O_SDT_ROW_H_
#define O_SDT_ROW_H_ 1

#include <smDef.h>
#include <smpDef.h>
#include <smcDef.h>
#include <sdtDef.h>
#include <sdr.h>
#include <smiDef.h>
#include <sdpDef.h>
#include <sdtTempPage.h>
#include <sdtWorkArea.h>

class sdtTempRow
{
public:
    /* ������ �������� RowInfo ������ ������ */
    inline static void makeTRPInfo( UChar               aFlag,
                                    ULong               aHitSequence,
                                    UInt                aHashValue,
                                    scGRID              aChildGRID,
                                    scGRID              aNextGRID,
                                    UInt                aValueLength,
                                    UInt                aColumnCount,
                                    smiTempColumn     * aColumns,
                                    smiValue          * aValueList,
                                    sdtTRPInfo4Insert * aTRPInfo );

    inline static IDE_RC append( sdtWASegment      * aWASegment,
                                 sdtWAGroupID        aWAGroupID,
                                 sdtTempPageType     aPageType,
                                 UInt                aCuttingOffset,
                                 sdtTRPInfo4Insert * aTRPInfo,
                                 sdtTRInsertResult * aTRInsertResult );

    static IDE_RC allocNewPage( sdtWASegment     * aWASegment,
                                sdtWAGroupID       aWAGroupID,
                                scPageID           aPrevWPID,
                                sdtTempPageType    aPageType,
                                UInt             * aRowPageCount,
                                scPageID         * aNewWPID,
                                UChar           ** aWAPagePtr );

    static void   initTRInsertResult( sdtTRInsertResult * aTRInsertResult );

    inline static IDE_RC appendRowPiece(sdtWASegment      * aWASegment,
                                        scPageID            aWPID,
                                        UChar             * aWAPagePtr,
                                        UInt                aSlotNo,
                                        UInt                aCuttingOffset,
                                        sdtTRPInfo4Insert * aRowInfo,
                                        sdtTRInsertResult * aTRInsertResult );

    inline static IDE_RC fetch( sdtWASegment      * aWASegment,
                                sdtWAGroupID        aGroupID,
                                UChar             * aRowPtr,
                                UInt                aValueLength,
                                UChar             * aRowBuffer,
                                sdtTRPInfo4Select * aTRPInfo );

    static IDE_RC fetchChainedRowPiece( sdtWASegment      * aWASegment,
                                        sdtWAGroupID        aGroupID,
                                        UChar             * aRowPtr,
                                        UInt                aValueLen,
                                        UChar             * aRowBuffer,
                                        sdtTRPInfo4Select * aTRPInfo );

    static IDE_RC fetchByGRID( sdtWASegment      * aWASegment,
                               sdtWAGroupID        aGroupID,
                               scGRID              aGRID,
                               UInt                aValueLength,
                               UChar             * aRowBuffer,
                               sdtTRPInfo4Select * aTRPInfo );

    inline static IDE_RC filteringAndFetchByGRID( smiTempCursor * aTempCursor,
                                                  scGRID          aTargetGRID,
                                                  UChar        ** aRow,
                                                  scGRID        * aRowGSID,
                                                  idBool        * aResult);

    inline static IDE_RC filteringAndFetch( smiTempCursor  * aTempCursor,
                                            UChar          * aTargetPtr,
                                            UChar         ** aRow,
                                            scGRID         * aRowGSID,
                                            idBool         * aResult);

private:
    inline static void   copyColumn( UChar      * aSlotPtr,
                                     UInt         aBeginOffset,
                                     UInt         aEndOffset,
                                     smiColumn  * aColumn,
                                     smiValue   * aValue );

public:
    static IDE_RC update( smiTempCursor * aTempCursor,
                          smiValue      * aCurColumn );
    static IDE_RC setHitFlag( smiTempCursor * aTempCursor,
                              ULong           aHitSeq );
    static idBool isHitFlagged( smiTempCursor * aTempCursor,
                                ULong           aHitSeq );

public:
    /* Dump�Լ��� */
    static void dumpTempTRPHeader( void       * aTRPHeader,
                                   SChar      * aOutBuf,
                                   UInt         aOutSize );
    static void dumpTempTRPInfo4Insert( void       * aTRPInfo,
                                        SChar      * aOutBuf,
                                        UInt         aOutSize );
    static void dumpTempTRPInfo4Select( void       * aTRPInfo,
                                        SChar      * aOutBuf,
                                        UInt         aOutSize );
    static void dumpTempRow( void  * aRowPtr,
                             SChar * aOutBuf,
                             UInt    aOutSize );
    static void dumpTempPageByRow( void  * aPagePtr,
                                   SChar * aOutBuf,
                                   UInt    aOutSize );
    static void dumpRowWithCursor( void   * aTempCursor,
                                   SChar  * aOutBuf,
                                   UInt     aOutSize );

};

/**************************************************************************
 * Description :
 * ������ �������� RowInfo ������ ������
 *
 * <IN>
 * aFlag        - ������ Flag
 * aHitSequence - ������ HitSequence
 * aHashValue   - ������ HashValue
 * aChildGRID   - ������ ChildGRID
 * aNextGRID    - ������ NextGRID
 * aValueLength - ������ ValueLength
 * aColumnCount - ������ ColumnCount
 * aColumns     - ������ Columns
 * aValueList   - ������ ValueList
 * <OUT>
 * aTRPInfo     - ������ �����
 ***************************************************************************/
void sdtTempRow::makeTRPInfo( UChar               aFlag,
                              ULong               aHitSequence,
                              UInt                aHashValue,
                              scGRID              aChildGRID,
                              scGRID              aNextGRID,
                              UInt                aValueLength,
                              UInt                aColumnCount,
                              smiTempColumn     * aColumns,
                              smiValue          * aValueList,
                              sdtTRPInfo4Insert * aTRPInfo )
{
    aTRPInfo->mTRPHeader.mTRFlag      = aFlag;
    aTRPInfo->mTRPHeader.mHitSequence = aHitSequence;
    aTRPInfo->mTRPHeader.mValueLength = 0; /* append�ϸ鼭 ������ */
    aTRPInfo->mTRPHeader.mHashValue   = aHashValue;
    aTRPInfo->mTRPHeader.mNextGRID    = aNextGRID;
    aTRPInfo->mTRPHeader.mChildGRID   = aChildGRID;
    aTRPInfo->mColumnCount            = aColumnCount;
    aTRPInfo->mColumns                = aColumns;
    aTRPInfo->mValueLength            = aValueLength;
    aTRPInfo->mValueList              = aValueList;
}

/**************************************************************************
 * Description :
 * �ش� WAPage�� Row�� �����Ѵ�.
 * ���� ������ ������ Row�� ���Ҵٸ� ���ʺ��� Row�� �����ϰ� ���� RowValue����
 * ���� ������ ��ġ�� RID�� ����ü�� ���� ��ȯ�Ѵ� . �̶� rowPiece ����ü��
 * rowValue�� smiValue���·� ������ �ִµ�, �� value�� ���� smiValue��
 * pointing�ϴ� �����̴�. ���� �ش� Slot�� �̹� Value�� �ִٸ�, ���� Row��
 * �Ǵ��ϰ� �׳� �о�ִ´�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWAGroupID     - ��� Group ID
 * aPageType      - ���� Page�� Type
 * aCuttingOffset - �� �� ������ Row�鸸 �����Ѵ�. ������ ������ �����.
 *                  sdtSortModule::copyExtraRow ����
 * aTRPInfo       - ������ Row
 * <OUT>
 * aTRInsertResult- ������ ���
 ***************************************************************************/
IDE_RC sdtTempRow::append( sdtWASegment      * aWASegment,
                           sdtWAGroupID        aWAGroupID,
                           sdtTempPageType     aPageType,
                           UInt                aCuttingOffset,
                           sdtTRPInfo4Insert * aTRPInfo,
                           sdtTRInsertResult * aTRInsertResult )
{
    UChar         * sWAPagePtr;
    scPageID        sHintWPID;
    scSlotNum       sSlotNo;
    idBool          sAllocNewPage;
    UInt            sRowPageCount = 1;

    IDE_DASSERT( SDT_TR_HEADER_SIZE_BASE ==
                 ( IDU_FT_SIZEOF( sdtTRPHeader, mTRFlag )
                   + IDU_FT_SIZEOF( sdtTRPHeader, mDummy )
                   + IDU_FT_SIZEOF( sdtTRPHeader, mHitSequence )
                   + IDU_FT_SIZEOF( sdtTRPHeader, mValueLength )
                   + IDU_FT_SIZEOF( sdtTRPHeader, mHashValue ) ) );

    IDE_DASSERT( SDT_TR_HEADER_SIZE_FULL ==
                 ( SDT_TR_HEADER_SIZE_BASE +
                   + IDU_FT_SIZEOF( sdtTRPHeader, mNextGRID ) +
                   + IDU_FT_SIZEOF( sdtTRPHeader, mChildGRID ) ) );

    idlOS::memset( aTRInsertResult, 0, ID_SIZEOF( sdtTRInsertResult ) );

    sHintWPID = sdtWASegment::getHintWPID( aWASegment, aWAGroupID );
    if( sHintWPID == SD_NULL_PID )
    {
        sAllocNewPage = ID_TRUE;    /* ���ʻ����� ��� */
    }
    else
    {
        sAllocNewPage = ID_FALSE;
        sWAPagePtr = sdtWASegment::getWAPagePtr( aWASegment,
                                                 aWAGroupID,
                                                 sHintWPID );
    }

    while( 1 )
    {
        if( sAllocNewPage == ID_TRUE )
        {
            /* ������ �����̰ų�, ������ �����ϴ� �������� �ٴڳ��� ��� */
            IDE_TEST( allocNewPage( aWASegment,
                                    aWAGroupID,
                                    sHintWPID,
                                    aPageType,
                                    &sRowPageCount,
                                    &sHintWPID,
                                    &sWAPagePtr )
                      != IDE_SUCCESS );
            if( sWAPagePtr == NULL )
            {
                /* FreePage ���� */
                break;
            }
            else
            {
                aTRInsertResult->mAllocNewPage = ID_TRUE;
            }
        }

        sdtTempPage::allocSlotDir( sWAPagePtr, &sSlotNo );

        IDE_TEST( sdtTempRow::appendRowPiece( aWASegment,
                                              sHintWPID,
                                              sWAPagePtr,
                                              sSlotNo,
                                              aCuttingOffset,
                                              aTRPInfo,
                                              aTRInsertResult )
                  != IDE_SUCCESS );

        sAllocNewPage = ID_FALSE;
        if( aTRInsertResult->mComplete == ID_TRUE )
        {
            break;
        }
        else
        {
            /* �������� ���� �Ҵ��ߴµ��� ������ �����Ͽ�
             * Slot�� Unused�� ���̽��� �־�� �ȵ� */
            IDE_ERROR( !( ( sAllocNewPage == ID_TRUE ) &&
                          ( sSlotNo == SDT_TEMP_SLOT_UNUSED ) ) );

            sAllocNewPage = ID_TRUE;
        }
    }

    IDE_TEST( sdtWASegment::setHintWPID( aWASegment, aWAGroupID, sHintWPID )
              != IDE_SUCCESS );
    aTRInsertResult->mRowPageCount = sRowPageCount;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    smuUtility::dumpFuncWithBuffer( IDE_DUMP_0,
                                    dumpTempTRPInfo4Insert,
                                    aTRPInfo );

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * RowPiece�ϳ��� �� �������� �����Ѵ�.
 * Append���� ����Ѵ�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWPID          - ������ ��� WPID
 * aWPagePtr      - ������ ��� Page�� Pointer ��ġ
 * aSlotNo        - ������ ��� Slot
 * aCuttingOffset - �� �� ������ Row�鸸 �����Ѵ�. ������ ������ �����.
 *                  sdtSortModule::copyExtraRow ����
 * aTRPInfo       - ������ Row
 * <OUT>
 * aTRInsertResult- ������ ���
 ***************************************************************************/
IDE_RC sdtTempRow::appendRowPiece( sdtWASegment      * aWASegment,
                                   scPageID            aWPID,
                                   UChar             * aWAPagePtr,
                                   UInt                aSlotNo,
                                   UInt                aCuttingOffset,
                                   sdtTRPInfo4Insert * aTRPInfo,
                                   sdtTRInsertResult * aTRInsertResult )
{
    UInt           sRowPieceSize;
    UInt           sRowPieceHeaderSize;
    UInt           sMinSize;
    UInt           sSlotSize;
    UChar        * sSlotPtr;
    UChar        * sRowPtr;
    UInt           sBeginOffset;
    UInt           sEndOffset;
    sdtTRPHeader * sTRPHeader;

    IDE_ERROR( aWPID != SC_NULL_PID );

    /* Slot�Ҵ��� ������ ��� */
    IDE_TEST_CONT( aSlotNo == SDT_TEMP_SLOT_UNUSED, SKIP );

    /* FreeSpace��� */
    sRowPieceHeaderSize = SDT_TR_HEADER_SIZE( aTRPInfo->mTRPHeader.mTRFlag );
    sRowPieceSize       = sRowPieceHeaderSize + aTRPInfo->mValueLength;

    IDE_ERROR_MSG( aCuttingOffset < sRowPieceSize,
                   "CuttingOffset : %"ID_UINT32_FMT"\n"
                   "RowPieceSize  : %"ID_UINT32_FMT"\n",
                   aCuttingOffset,
                   sRowPieceHeaderSize );

    if ( SM_IS_FLAG_ON( aTRPInfo->mTRPHeader.mTRFlag, SDT_TRFLAG_UNSPLIT ) )
    {
        sMinSize = sRowPieceSize - aCuttingOffset;
    }
    else
    {
        sMinSize = IDL_MIN( smuProperty::getTempRowSplitThreshold(),
                            sRowPieceSize - aCuttingOffset);
    }

    /* Slot�Ҵ��� �õ��� */
    sdtTempPage::allocSlot( aWAPagePtr,
                            aSlotNo,
                            sRowPieceSize - aCuttingOffset,
                            sMinSize,
                            &sSlotSize,
                            &sSlotPtr );
    /* �Ҵ� ���� */
    IDE_TEST_CONT( sSlotSize == 0, SKIP );

    /* �Ҵ��� Slot ������ */
    aTRInsertResult->mHeadRowpiecePtr = sSlotPtr;

    if( sSlotSize == sRowPieceSize )
    {
        /* Cutting�� �͵� ����, �׳� ���� Copy�ϸ� �� */
        aTRInsertResult->mComplete = ID_TRUE;

        /*Header ��� */
        sTRPHeader = (sdtTRPHeader*)sSlotPtr;
        idlOS::memcpy( sTRPHeader,
                       &aTRPInfo->mTRPHeader,
                       sRowPieceHeaderSize );
        sTRPHeader->mValueLength = aTRPInfo->mValueLength;

        /* ���� Row�� ���� ��ġ */
        sRowPtr = ((UChar*)aTRPInfo->mValueList[ 0 ].value)
            - aTRPInfo->mColumns[ 0 ].mColumn.offset;

        idlOS::memcpy( sSlotPtr + sRowPieceHeaderSize,
                       sRowPtr,
                       aTRPInfo->mValueLength );

        /* ���� �Ϸ��� */
        aTRPInfo->mValueLength = 0;

        SC_MAKE_GRID( aTRInsertResult->mHeadRowpieceGRID,
                      SDT_SPACEID_WORKAREA,
                      aWPID,
                      aSlotNo );

        sdtWASegment::convertFromWGRIDToNGRID(
            aWASegment,
            aTRInsertResult->mHeadRowpieceGRID,
            &aTRInsertResult->mHeadRowpieceGRID );

        aTRInsertResult->mTailRowpieceGRID =
            aTRInsertResult->mHeadRowpieceGRID;

        aTRPInfo->mTRPHeader.mNextGRID = SC_NULL_GRID;
    }
    else
    {
        /* Row�� �ɰ��� �� ���� */
        /*********************** Range ��� ****************************/
        /* ����ϴ� OffsetRange�� ����� */
        sBeginOffset = sRowPieceSize - sSlotSize; /* �䱸���� ������ ��ŭ */
        sEndOffset   = aTRPInfo->mValueLength;
        IDE_ERROR( sEndOffset > sBeginOffset );

        /*********************** Header ��� **************************/
        aTRInsertResult->mHeadRowpiecePtr = sSlotPtr;
        sTRPHeader = (sdtTRPHeader*)sSlotPtr;

        idlOS::memcpy( sTRPHeader,
                       &aTRPInfo->mTRPHeader,
                       sRowPieceHeaderSize );
        sTRPHeader->mValueLength = sEndOffset - sBeginOffset;

        if( sBeginOffset == aCuttingOffset )
        {
            /* ��û�� Write �Ϸ� */
            aTRInsertResult->mComplete = ID_TRUE;
        }
        else
        {
            aTRInsertResult->mComplete = ID_FALSE;
        }

        if( sBeginOffset > 0 )
        {
            /* ���� ���� �κ��� ���ƴ� ������,
             * ������ TRPHeader���� NextGRID�� ���̰� (������ �����϶��)
             * ����� TRPHeader���� Head�� ����. ( Head�� �ƴϴϱ�. ) */
            SM_SET_FLAG_ON( aTRPInfo->mTRPHeader.mTRFlag,
                            SDT_TRFLAG_NEXTGRID );
            SM_SET_FLAG_OFF( sTRPHeader->mTRFlag,
                             SDT_TRFLAG_HEAD );
        }

        sSlotPtr += sRowPieceHeaderSize;

        /* ���� Row�� ���� ��ġ */
        sRowPtr = ((UChar*)aTRPInfo->mValueList[ 0 ].value)
            - aTRPInfo->mColumns[ 0 ].mColumn.offset;
        idlOS::memcpy( sSlotPtr,
                       sRowPtr + sBeginOffset,
                       sEndOffset - sBeginOffset );

        aTRPInfo->mValueLength -= sTRPHeader->mValueLength;

        SC_MAKE_GRID( aTRInsertResult->mHeadRowpieceGRID,
                      SDT_SPACEID_WORKAREA,
                      aWPID,
                      aSlotNo );

        sdtWASegment::convertFromWGRIDToNGRID(
            aWASegment,
            aTRInsertResult->mHeadRowpieceGRID,
            &aTRInsertResult->mHeadRowpieceGRID );

        /* Tail�� ���� �����ϰ� Head�� �ʰ� �����Ѵ�. ���� Tail�� NULL��
         * �������� �ʾ�����, ù �����̱� ������ �� GRID�� �����ϸ� �ȴ�. */
        if( SC_GRID_IS_NULL( aTRInsertResult->mTailRowpieceGRID ) )
        {
            aTRInsertResult->mTailRowpieceGRID =
                aTRInsertResult->mHeadRowpieceGRID;
        }

        if ( SM_IS_FLAG_ON( aTRPInfo->mTRPHeader.mTRFlag, SDT_TRFLAG_NEXTGRID ) )
        {
            aTRPInfo->mTRPHeader.mNextGRID =  aTRInsertResult->mHeadRowpieceGRID;
        }
        else
        {
            aTRPInfo->mTRPHeader.mNextGRID = SC_NULL_GRID;
        }
    }

    IDE_TEST( sdtWASegment::setDirtyWAPage( aWASegment, aWPID )
              != IDE_SUCCESS );

    IDE_EXCEPTION_CONT( SKIP );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 *   column �ϳ��� Page�� ������
 *   Value ��ü�󿡼� Begin/EndOffset�� �������� �߶� ������
 *
 *
 * 0    4    8   12   16
 * +----+----+----+----+
 * | A  | B  | C  | D  |
 * +----+----+----+----+
 *        <---->
 *        ^    ^
 * BeginOff(6) ^
 *             EndOffset(10)
 * ���� ���� ���,
 * +--+--+
 * |B | C|
 * +--+--+
 * �Ʒ��� ���� ��ϵǾ�� �Ѵ�.
 *
 * <IN>
 * aSloPtr           - ������ ����� ������ ��ġ
 * aBeginOffset      - ������ Value�� ���� Offset
 * aEndOffset        - ������ Value�� �� Offset
 * aColumn           - ������ Column�� ����
 * aValue            - ������ Value
 ***************************************************************************/
void   sdtTempRow::copyColumn( UChar      * aSlotPtr,
                               UInt         aBeginOffset,
                               UInt         aEndOffset,
                               smiColumn  * aColumn,
                               smiValue   * aValue )
{
    SInt    sColumnBeginOffset;
    SInt    sColumnEndOffset;
    SInt    sSlotEndOffset = aEndOffset - aBeginOffset;

    /* ������ Slot�󿡼� �� Column�� ��� ��ġ�ϴ°�, �� �ǹ�.*/
    sColumnBeginOffset = aColumn->offset                  - aBeginOffset;
    sColumnEndOffset   = aColumn->offset + aValue->length - aBeginOffset;

    if( sColumnEndOffset < 0 )
    {
        /* ������ A Column.
         * Column�� Slot�� ���ʿ��� ���
         *
         *         +------+
         *         |RowHdr|
         *      +..+------+--+--+
         *      :  A   :  |B | C|
         *      +..... +..+--+--+
         *     -6     -2  0
         *
         * sColumnBeginOffset = -6
         * sColumnEndOffset   = -2
         * sSlotEndOffset     = 4  */
        return;
    }
    if( sSlotEndOffset <= sColumnBeginOffset )
    {
        /* ������ D Column.
         * Column�� Slot�� �������ʿ��� ���
         *
         * +------+
         * |RowHdr|
         * +------+--+--+..+....+
         *        |B | C|  : D  :
         *        +--+--+..+....+
         *                 6    10
         *
         * sColumnBeginOffset = 6
         * sColumnEndOffset   = 10
         * sSlotEndOffset     = 4 */
        return;
    }

    if( sColumnBeginOffset < 0 ) /* ������ �߷ȴ°�? */
    {

        if( sColumnEndOffset > sSlotEndOffset ) /* �������� �߷ȴ°�*/
        {
            /* ������ B�� C�� ��ģ Column���� ���.
             * ���� ������ �Ѵ� �߸�
             *
             * +------+
             * |RowHdr|
             * +------+-----+..+
             *     :  :  Z  :  :
             *     +..+-----+..+
             *    -2           6
             *
             * sColumnBeginOffset = -2
             * sColumnEndOffset   =  6
             * sSlotEndOffset     =  4 */
            idlOS::memcpy( aSlotPtr,
                           ((UChar*)aValue->value)
                           - sColumnBeginOffset,
                           sSlotEndOffset );
        }
        else /* ������ ���߸� */
        {
            /* ������ B Column.
             * ���� �߸� �͸� �Ű澸
             *
             * +------+
             * |RowHdr|
             * +------+--+--+
             *     :  |B | C|
             *     +..+--+--+
             *    -2     2
             *
             * sColumnBeginOffset = -2
             * sColumnEndOffset   =  2
             * sSlotEndOffset     =  4 */
            idlOS::memcpy( aSlotPtr,
                           ((UChar*)aValue->value)
                           - sColumnBeginOffset,
                           sColumnEndOffset );
        }
    }
    else /* ���� ���߸�*/
    {
        if( sColumnEndOffset > sSlotEndOffset ) /* �������� �߷ȴ°�*/
        {
            /* C�� ���
             * ������ �߸��� �Ű澸
             *
             * +------+
             * |RowHdr|
             * +------+--+--+..+
             *        |B | C|  :
             *        +--+--+..+
             *           2     6
             *
             * sColumnBeginOffset =  2
             * sColumnEndOffset   =  6
             * sSlotEndOffset     =  4 */
            idlOS::memcpy( aSlotPtr + sColumnBeginOffset,
                           aValue->value,
                           sSlotEndOffset - sColumnBeginOffset );
        }
        else /* ������ ���߸� */
        {
            /* Offset���࿡ ��� ���� ���
             *
             * +------+
             * |RowHdr|
             * +------+--+--+--+--+..+
             *     :  |B |  Z  | C|  :
             *     +..+--+-----+--+..+
             *           2     6
             *
             * sColumnBeginOffset =  2
             * sColumnEndOffset   =  6
             * sSlotEndOffset     =  8 */
            idlOS::memcpy( aSlotPtr + sColumnBeginOffset,
                           aValue->value,
                           aValue->length );
        }
    }
}

/**************************************************************************
 * Description :
 * page�� �����ϴ� Row ��ü�� rowInfo���·� �д´�.
 *
 * <IN>
 * aWASegment     - ��� WASegment
 * aWAGroupID     - ��� Group ID
 * aRowPtr        - Row�� ��ġ Pointer
 * aValueLength   - Row�� Value�κ��� �� ����
 * aRowBuffer     - �ɰ��� Column�� �����ϱ� ���� Buffer
 * <OUT>
 * aTRPInfo       - Fetch�� ���
 ***************************************************************************/
IDE_RC sdtTempRow::fetch( sdtWASegment         * aWASegment,
                          sdtWAGroupID           aGroupID,
                          UChar                * aRowPtr,
                          UInt                   aValueLength,
                          UChar                * aRowBuffer,
                          sdtTRPInfo4Select    * aTRPInfo )
{
    UChar           sFlag;

    aTRPInfo->mSrcRowPtr = (sdtTRPHeader*)aRowPtr;
    aTRPInfo->mTRPHeader = (sdtTRPHeader*)aRowPtr;
    sFlag                = aTRPInfo->mTRPHeader->mTRFlag;

    IDE_DASSERT( SM_IS_FLAG_ON( ( (sdtTRPHeader*)aRowPtr)->mTRFlag, SDT_TRFLAG_HEAD ) );

    if ( SM_IS_FLAG_ON( sFlag, SDT_TRFLAG_NEXTGRID ) )
    {
        IDE_TEST( fetchChainedRowPiece( aWASegment,
                                        aGroupID,
                                        aRowPtr,
                                        aValueLength,
                                        aRowBuffer,
                                        aTRPInfo )
                  != IDE_SUCCESS );
    }
    else
    {
        aTRPInfo->mValuePtr    = aRowPtr + SDT_TR_HEADER_SIZE( sFlag );
        aTRPInfo->mValueLength = aTRPInfo->mTRPHeader->mValueLength;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * Filtering��ġ�� fetch��.
 *
 * <IN>
 * aTempCursor    - ��� Ŀ��
 * aTargetGRID    - ��� Row�� GRID
 * <OUT>
 * aRow           - ��� Row�� ���� Bufer
 * aRowGRID       - ��� Row�� ���� GRID
 * aResult        - Filtering ��� ����
 ***************************************************************************/
IDE_RC sdtTempRow::filteringAndFetchByGRID( smiTempCursor  * aTempCursor,
                                            scGRID           aTargetGRID,
                                            UChar         ** aRow,
                                            scGRID         * aRowGRID,
                                            idBool         * aResult)
{
    smiTempTableHeader * sHeader = aTempCursor->mTTHeader;
    sdtWASegment       * sWASeg  = (sdtWASegment*)sHeader->mWASegment;
    UChar              * sCursor    = NULL;
    idBool               sIsValidSlot;

    /* ���� Fetch�ϱ� ���� ���� PieceHeader�� ���� First���� Ȯ�� */
    IDE_TEST( sdtWASegment::getPagePtrByGRID( sWASeg,
                                              aTempCursor->mWAGroupID,
                                              aTargetGRID,
                                              &sCursor,
                                              &sIsValidSlot )
              != IDE_SUCCESS );
    IDE_ERROR( sIsValidSlot == ID_TRUE );
    IDE_DASSERT( SM_IS_FLAG_ON( ((sdtTRPHeader*)sCursor)->mTRFlag, SDT_TRFLAG_HEAD ) );

    IDE_TEST( filteringAndFetch( aTempCursor,
                                 sCursor,
                                 aRow,
                                 aRowGRID,
                                 aResult )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * Filtering��ġ�� fetch��.
 *
 * <IN>
 * aTempCursor    - ��� Ŀ��
 * aTargetPtr     - ��� Row�� Ptr
 * <OUT>
 * aRow           - ��� Row�� ���� Bufer
 * aRowGRID       - ��� Row�� ���� GRID
 * aResult        - Filtering ��� ����
 ***************************************************************************/
IDE_RC sdtTempRow::filteringAndFetch( smiTempCursor  * aTempCursor,
                                      UChar          * aTargetPtr,
                                      UChar         ** aRow,
                                      scGRID         * aRowGRID,
                                      idBool         * aResult)
{
    smiTempTableHeader * sHeader;
    sdtWASegment       * sWASeg;
    sdtTRPInfo4Select    sTRPInfo;
    UInt                 sFlag;
    idBool               sHit;
    idBool               sNeedHit;

    *aResult = ID_FALSE;

    sHeader = aTempCursor->mTTHeader;
    sWASeg  = (sdtWASegment*)sHeader->mWASegment;
    IDE_DASSERT( SM_IS_FLAG_ON( ((sdtTRPHeader*)aTargetPtr)->mTRFlag, SDT_TRFLAG_HEAD ) );

    IDE_TEST( fetch( sWASeg,
                     aTempCursor->mWAGroupID,
                     aTargetPtr,
                     sHeader->mRowSize,
                     sHeader->mRowBuffer4Fetch,
                     &sTRPInfo )
              != IDE_SUCCESS );
    IDE_DASSERT( sTRPInfo.mValueLength <= sHeader->mRowSize );

    sFlag = aTempCursor->mTCFlag;

    /* HashScan�ε� HashValue�� �ٸ��� �ȵ� */
    if ( sFlag & SMI_TCFLAG_HASHSCAN )
    {
        IDE_TEST_CONT( sTRPInfo.mTRPHeader->mHashValue !=
                       aTempCursor->mHashValue,
                       SKIP );
    }
    else
    {
        /* nothing to do */
    }

    /* HItFlagüũ�� �ʿ��Ѱ�? */
    if ( sFlag & SMI_TCFLAG_HIT_MASK )
    {
        /* Hit������ ũ�� �� Flag�̴�.
         * IgnoreHit -> Hit��ü�� ��� ����
         * Hit       -> Hit�� �͸� ������
         * NoHit     -> Hit �ȵ� �͸� ������
         *
         * �� if���� IgnoreHit�� �ƴ����� �����ϴ� ���̰�.
         * �Ʒ� IF���� Hit���� NoHit���� �����ϴ� ���̴�. */
        sNeedHit = ( sFlag & SMI_TCFLAG_HIT ) ?ID_TRUE : ID_FALSE;
        sHit = ( sTRPInfo.mTRPHeader->mHitSequence ==
                 aTempCursor->mTTHeader->mHitSequence ) ?
            ID_TRUE : ID_FALSE;

        IDE_TEST_CONT( sHit != sNeedHit, SKIP );
    }
    else
    {
        /* nothing to do */
    }

    if ( sFlag & SMI_TCFLAG_FILTER_KEY )
    {
        IDE_TEST( aTempCursor->mKeyFilter->minimum.callback(
                      aResult,
                      sTRPInfo.mValuePtr,
                      NULL,
                      0,
                      SC_NULL_GRID,
                      aTempCursor->mKeyFilter->minimum.data )
                  != IDE_SUCCESS );
        IDE_TEST_CONT( *aResult == ID_FALSE, SKIP );

        IDE_TEST( aTempCursor->mKeyFilter->maximum.callback(
                      aResult,
                      sTRPInfo.mValuePtr,
                      NULL,
                      0,
                      SC_NULL_GRID,
                      aTempCursor->mKeyFilter->maximum.data )
                  != IDE_SUCCESS );
        IDE_TEST_CONT( *aResult == ID_FALSE, SKIP );
    }
    else
    {
        /* nothing to do */
    }

    if ( sFlag & SMI_TCFLAG_FILTER_ROW )
    {
        IDE_TEST( aTempCursor->mRowFilter->callback(
                      aResult,
                      sTRPInfo.mValuePtr,
                      NULL,
                      0,
                      SC_NULL_GRID,
                      aTempCursor->mRowFilter->data )
                  != IDE_SUCCESS );
        IDE_TEST_CONT( *aResult == ID_FALSE, SKIP );
    }
    else
    {
        /* nothing to do */
    }

    *aResult             = ID_TRUE;
    aTempCursor->mRowPtr = aTargetPtr;
    *aRowGRID            = aTempCursor->mGRID;

    sdtWASegment::convertFromWGRIDToNGRID( sWASeg,
                                           *aRowGRID,
                                           aRowGRID );

    idlOS::memcpy( *aRow,
                   sTRPInfo.mValuePtr,
                   sTRPInfo.mValueLength );

    IDE_EXCEPTION_CONT( SKIP );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

#endif
