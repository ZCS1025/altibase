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
 * $Id
 *
 * Description :
 *
 * TempPage
 * Persistent Page�� sdpPhyPage�� �����Ǵ� ���� �� ����ü�̴�.
 * TempPage�� LSN, SmoNo, PageState, Consistent �� sdpPhyPageHdr�� �ִ� ���
 * ���� ����� �����ϴ�.
 * ���� ����ȿ���� ���� ���� �̸� ��� �����Ѵ�.
 *
 *  # ���� �ڷᱸ��
 *
 *    - sdpPageHdr ����ü
 *
 **********************************************************************/

#ifndef _O_SDT_PAGE_H_
#define _O_SDT_PAGE_H_ 1

#include <smDef.h>
#include <sdr.h>
#include <smu.h>
#include <sdtDef.h>
#include <smrCompareLSN.h>

class sdtTempPage
{
public:
    static IDE_RC init( UChar    * aPagePtr,
                        UInt       aType,
                        scPageID   aPrev,
                        scPageID   aSelf,
                        scPageID   aNext);

    inline static void allocSlotDir( UChar * aPagePtr, scSlotNum * aSlotNo );

    inline static void allocSlot( UChar     * aPagePtr,
                                  scSlotNum   aSlotNo,
                                  UInt        aNeedSize,
                                  UInt        aMinSize,
                                  UInt      * aRetSize,
                                  UChar    ** aRetPtr );

    inline static IDE_RC getSlotPtr( UChar      * aPagePtr,
                                     scSlotNum    aSlotNo,
                                     UChar     ** aSlotPtr );

    inline static UInt getSlotDirOffset( scSlotNum    aSlotNo );

    inline static scOffset getSlotOffset( UChar      * aPagePtr,
                                          scSlotNum    aSlotNo );

    inline static void setSlot( UChar     * aPagePtr,
                                scSlotNum   aSlotNo,
                                UInt        aOffset);
    inline static IDE_RC getSlotCount( UChar * aPagePtr, UInt *aSlotCount );
    inline static void   spendFreeSpace( UChar * aPagePtr,  UInt aSize );

    /********************** GetSet ************************/
    static scPageID getPrevPID( UChar * aPagePtr )
    {
        return ( (sdtTempPageHdr*)aPagePtr )->mPrevPID;
    }
    static scPageID getPID( UChar * aPagePtr )
    {
        return ( (sdtTempPageHdr*)aPagePtr )->mSelfPID;
    }
    static scPageID getNextPID( UChar * aPagePtr )
    {
        return ( (sdtTempPageHdr*)aPagePtr )->mNextPID;
    }
/*  not used
    static void setPrevPID( UChar * aPagePtr, scPageID aPrevPID )
    {
    sdtTempPageHdr *sHdr = (sdtTempPageHdr*)aPagePtr;

    sHdr->mPrevPID = aPrevPID;
    }
*/
/*  not used
    static void setPID( UChar * aPagePtr, scPageID aPID )
    {
    sdtTempPageHdr *sHdr = (sdtTempPageHdr*)aPagePtr;

    sHdr->mSelfPID = aPID;
    }
*/
    static void setNextPID( UChar * aPagePtr, scPageID aNextPID )
    {
        sdtTempPageHdr *sHdr = (sdtTempPageHdr*)aPagePtr;

        sHdr->mNextPID = aNextPID;
    }

    static UInt getType( UChar *aPagePtr )
    {
        sdtTempPageHdr *sHdr = (sdtTempPageHdr*)aPagePtr;

        return sHdr->mTypeAndFreeOffset >> SDT_TEMP_TYPE_SHIFT;
    }

    /* FreeSpace�� ���۵Ǵ� Offset */
    static UInt getBeginFreeOffset( UChar *aPagePtr )
    {
        sdtTempPageHdr *sHdr = (sdtTempPageHdr*)aPagePtr;

        return ID_SIZEOF( sdtTempPageHdr )
            + sHdr->mSlotCount * ID_SIZEOF( scSlotNum );
    }

    /* FreeSpace�� ������ Offset */
    static UInt getFreeOffset( UChar *aPagePtr )
    {
        sdtTempPageHdr *sHdr = (sdtTempPageHdr*)aPagePtr;

        return sHdr->mTypeAndFreeOffset & SDT_TEMP_FREEOFFSET_BITMASK;
    }

    /* �� ���������� �����Ͱ� �־�������, �������� �������� ���Ѵ�. */
    static UChar* getPageStartPtr( void   *aPagePtr )
    {
        IDE_DASSERT( aPagePtr != NULL );

        return (UChar *)idlOS::alignDown( (void*)aPagePtr,
                                          (UInt)SD_PAGE_SIZE );
    }

    /* FreeOffset�� �����Ѵ�. */
    static void setFreeOffset( UChar *aPagePtr, UInt aFreeOffset )
    {
        sdtTempPageHdr *sHdr = (sdtTempPageHdr*)aPagePtr;

        sHdr->mTypeAndFreeOffset = ( getType( aPagePtr )
                                     << SDT_TEMP_TYPE_SHIFT ) |
            ( aFreeOffset );
    }

public:
    static void dumpTempPage( void  * aPagePtr,
                              SChar * aOutBuf,
                              UInt    aOutSize );
    static SChar mPageName[ SDT_TEMP_PAGETYPE_MAX ][ SMI_TT_STR_SIZE ];
};

/**************************************************************************
 * Description :
 * SlotDirectory�κ��� Slot�� Offset��ġ�� �����´�.
 *
 * <IN>
 * aPagePtr       - �ʱ�ȭ�� ��� Page
 * aSlotNo        - Slot ��ȣ
 ***************************************************************************/
UInt       sdtTempPage::getSlotDirOffset( scSlotNum    aSlotNo )
{
    return ID_SIZEOF( sdtTempPageHdr ) + ( aSlotNo * ID_SIZEOF( scSlotNum ) );
}


/**************************************************************************
 * Description :
 * SlotDirectory�κ��� aSlotNo�� �̿��ؼ� Slot�� Offset�� �����´�.
 *
 * <IN>
 * aPagePtr       - �ʱ�ȭ�� ��� Page
 * aSlotNo        - Slot ��ȣ
 ***************************************************************************/
scOffset sdtTempPage::getSlotOffset( UChar      * aPagePtr,
                                     scSlotNum    aSlotNo )
{
    return *( (scOffset*)( aPagePtr + getSlotDirOffset( aSlotNo ) ) );
}


/**************************************************************************
 * Description :
 * ������ Pointer�� �����´�.
 *
 * <IN>
 * aPagePtr       - �ʱ�ȭ�� ��� Page
 * aSlotNo        - �Ҵ�޾Ҵ� Slot ��ȣ
 * <OUT>
 * aSlotPtr       - Slot�� ��ġ
 ***************************************************************************/
IDE_RC sdtTempPage::getSlotPtr( UChar      * aPagePtr,
                                scSlotNum    aSlotNo,
                                UChar     ** aSlotPtr )
{
    sdtTempPageHdr * sPageHdr = ( sdtTempPageHdr * ) aPagePtr;

    IDE_ERROR( sPageHdr->mSlotCount > aSlotNo );

    *aSlotPtr = aPagePtr + getSlotOffset( aPagePtr, aSlotNo );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * Slot�� ������ �����´�. Header���� �������� ������, ������ Slot��
 * UnusedSlot�̶��, ������� ���� ������ ġ�⿡ �߰� ó���� �ؾ� �Ѵ�.
 *
 * <IN>
 * aPagePtr       - �ʱ�ȭ�� ��� Page
 * <OUT>
 * aSlotCount     - Slot ����
 ***************************************************************************/
IDE_RC sdtTempPage::getSlotCount( UChar * aPagePtr, UInt *aSlotCount )
{
    sdtTempPageHdr * sHdr = (sdtTempPageHdr*)aPagePtr;
    UInt             sSlotValue;

    if( sHdr->mSlotCount > 1 )
    {
        /* Slot�� �ϳ��� �ִٸ�, �� Slot�� Unused���� üũ�Ѵ�. */
        sSlotValue = getSlotOffset( aPagePtr, sHdr->mSlotCount - 1 );
        if( sSlotValue == SDT_TEMP_SLOT_UNUSED )
        {
            (*aSlotCount) = sHdr->mSlotCount - 1;
        }
        else
        {
            (*aSlotCount) = sHdr->mSlotCount;
        }
    }
    else
    {
        (*aSlotCount) = sHdr->mSlotCount;
    }


    return IDE_SUCCESS;
}

/**************************************************************************
 * Description :
 * ������ ���� �Ҵ�޴´�. �� ������ �ƹ��͵� ����ġ�� ����ü,
 * UNUSED(ID_USHORT_MAX)���� �ʱ�ȭ �Ǿ��ִ�.
 *
 * <IN>
 * aPagePtr       - �ʱ�ȭ�� ��� Page
 * <OUT>
 * aSlotNo        - �Ҵ��� Slot ��ȣ
 ***************************************************************************/
void sdtTempPage::allocSlotDir( UChar * aPagePtr, scSlotNum * aSlotNo )
{
    UInt            sCandidateSlotDirOffset;
    sdtTempPageHdr *sHdr = (sdtTempPageHdr*)aPagePtr;

    sCandidateSlotDirOffset = getSlotDirOffset( sHdr->mSlotCount + 1 );

    if( sCandidateSlotDirOffset < getFreeOffset( aPagePtr ) )
    {
        /* �Ҵ� ���� */
        (*aSlotNo) = sHdr->mSlotCount;
        setSlot( aPagePtr, (*aSlotNo), SDT_TEMP_SLOT_UNUSED );

        sHdr->mSlotCount ++;
    }
    else
    {
        (*aSlotNo) = SDT_TEMP_SLOT_UNUSED;
    }
}

/**************************************************************************
 * Description :
 * ������ ������ �Ҵ�޴´�. ��û�� ũ�⸸ŭ �Ҵ�Ǿ� ��ȯ�ȴ�.
 *
 * <IN>
 * aPagePtr       - �ʱ�ȭ�� ��� Page
 * aSlotNo        - �Ҵ�޾Ҵ� Slot ��ȣ
 * aNeedSize      - �䱸�ϴ� Byte����
 * aMinSize       - aNeedSize�� �ȵǸ� �ּ� aMinSize��ŭ�� �Ҵ�޾߾� �Ѵ�
 * <OUT>
 * aRetSize       - ������ �Ҵ���� ũ��
 * aRetPtr        - �Ҵ���� Slot�� Begin����
 ***************************************************************************/
void sdtTempPage::allocSlot( UChar     * aPagePtr,
                             scSlotNum   aSlotNo,
                             UInt        aNeedSize,
                             UInt        aMinSize,
                             UInt      * aRetSize,
                             UChar    ** aRetPtr )
{
    UInt sEndFreeOffset   = getFreeOffset( aPagePtr );
    UInt sBeginFreeOffset = getBeginFreeOffset( aPagePtr );
    UInt sAlignedBeginFreeOffset = idlOS::align8( sBeginFreeOffset );
    UInt sFreeSize = ( sEndFreeOffset - sAlignedBeginFreeOffset );
    UInt sRetOffset;

    if( sFreeSize < aMinSize )
    {
        /* ���� �������� �Ҵ����� ������ */
        *aRetPtr  = NULL;
        *aRetSize = 0;
    }
    else
    {
        if( sFreeSize < aNeedSize )
        {
            /* �ּҺ��� ������, �˳����� ����.
             * Align�� BegeinFreeOffset���� ���θ� ��ȯ���� */
            sRetOffset = sAlignedBeginFreeOffset;
        }
        else
        {
            /* �Ҵ� ������.
             * �ʿ��� ������ŭ �� BeginOffset�� 8BteAling���� ����� */
            sRetOffset = sEndFreeOffset - aNeedSize;
            sRetOffset = sRetOffset - ( sRetOffset & 7 );
        }

        *aRetPtr  = aPagePtr + sRetOffset;
        /* Align�� ���� Padding ������ sRetOffset�� ���� �˳��ϰ� �� ��
         * ����. ���� Min���� �ٿ��� */
        *aRetSize = IDL_MIN( sEndFreeOffset - sRetOffset,
                             aNeedSize );
        spendFreeSpace( aPagePtr, sEndFreeOffset - sRetOffset );
        setSlot( aPagePtr, aSlotNo, sRetOffset );
    }
}


/**************************************************************************
 * Description :
 * �ش� SlotDir�� offset�� �����Ѵ�
 *
 * <IN>
 * aPagePtr       - �ʱ�ȭ�� ��� Page
 * aSlotNo        - �Ҵ�޾Ҵ� Slot ��ȣ
 * aOffset        - ������ Offset
 ***************************************************************************/
void sdtTempPage::setSlot( UChar     * aPagePtr,
                           scSlotNum   aSlotNo,
                           UInt        aOffset)
{
    scSlotNum  * sSlotOffset;

    sSlotOffset  = (scSlotNum*)( aPagePtr + getSlotDirOffset( aSlotNo ) );
    *sSlotOffset = aOffset;
}

/**************************************************************************
 * Description :
 * FreeSpace�� �����κ� �׳� �Ҹ��Ų��. ( Align �뵵 )
 *
 * <IN>
 * aPagePtr       - �ʱ�ȭ�� ��� Page
 * aSize          - �Ҹ�� ũ��
 ***************************************************************************/
void sdtTempPage::spendFreeSpace( UChar * aPagePtr, UInt aSize )
{
    SInt             sBeginFreeOffset;

    sBeginFreeOffset = getFreeOffset( aPagePtr ) - aSize;

    IDE_DASSERT( sBeginFreeOffset >= 0 );
    setFreeOffset( aPagePtr, sBeginFreeOffset );
}

#endif // _SDT_PAGE_H_
