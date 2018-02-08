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
 * - WorkAreaMap
 *     SortTemp�� extractNSort&insertNSort���� SortGroup�� �����ϴ� KeyMap��
 *     Merge���� Heap, UniquehashTemp�� HashGroup���� ���� �ΰ��� ��������
 *     �ִ�.
 *          WAPage�󿡸� �����Ѵ�. �� NPage�� �������� �ʴ´�.
 *          �� Pointing�ϱ� ���� �������� Array/Map������ �����Ѵ�.
 *     �� ������ ��� ���ڱ����� �Ϸ� �Ͽ�����, �󼼼��� ���� �ߺ��ڵ尡 �ɰ�
 *     ���Ƽ� sdtWAMap���� �ϳ��� ��ģ��.
 *
 * - Ư¡
 *     NPage�� Assign ���� �ʴ´�.
 *     Array�� �����ϴ� Slot�� ũ��� �����̴�.
 *     Address�� �������� �ش� Array�� Random Access �ϴ� ���̴�.
 **********************************************************************/

#ifndef _O_SDT_WA_MAP_H_
#define _O_SDT_WA_MAP_H_ 1

#include <idl.h>
#include <smDef.h>
#include <smiDef.h>
#include <sdtDef.h>
#include <sdtWorkArea.h>
#include <iduLatch.h>

class sdtWAMap
{
public:
    static IDE_RC create( sdtWASegment * aWASegment,
                          sdtWAGroupID   aWAGID,
                          sdtWMType      aWMType,
                          UInt           aSlotCount,
                          UInt           aVersionCount,
                          void         * aRet );
    static IDE_RC disable( void         * aMapHdr );

    static IDE_RC resetPtrAddr( void         * aWAMapHdr,
                                sdtWASegment * aWASegment );
    inline static IDE_RC set( void * aWAMapHdr, UInt aIdx, void * aValue );
    inline static IDE_RC setNextVersion( void * aWAMapHdr,
                                         UInt   aIdx,
                                         void * aValue );
    inline static IDE_RC setvULong( void   * aWAMapHdr,
                                    UInt     aIdx,
                                    vULong * aValue );

    inline static IDE_RC get( void * aWAMapHdr, UInt aIdx, void * aValue );
    inline static IDE_RC getvULong( void   * aWAMapHdr,
                                    UInt     aIdx,
                                    vULong * aValue );

    inline static IDE_RC getSlotPtr( void *  aWAMapHdr,
                                     UInt    aIdx,
                                     UInt    aSlotSize,
                                     void ** aSlotPtr );
    inline static IDE_RC getSlotPtrWithCheckState( void  * aWAMapHdr,
                                                   UInt    aIdx,
                                                   idBool  aResetPage,
                                                   void ** aSlotPtr );
    inline static IDE_RC getSlotPtrWithVersion( void *  aWAMapHdr,
                                                UInt    aVersionIdx,
                                                UInt    aIdx,
                                                UInt    aSlotSize,
                                                void ** aSlotPtr );
    inline static UChar * getPtrByOffset( void *  aWAMapHdr,
                                          UInt    aOffset );

    static UInt getOffset( UInt   aSlotIdx,
                           UInt   aSlotSize,
                           UInt   aVersionIdx,
                           UInt   aVersionCount )
    {
        return ( ( aSlotIdx * aVersionCount ) + aVersionIdx ) * aSlotSize;
    }

    inline static IDE_RC swap( void * aWAMapHdr, UInt aIdx1, UInt aIdx2 );
    inline static IDE_RC swapvULong( void * aWAMapHdr,
                                     UInt   aIdx1,
                                     UInt   aIdx2 );
    inline static IDE_RC incVersionIdx( void * aWAMapHdr );

    static IDE_RC expand( void     * aWAMapHdr,
                          scPageID   aLimitPID,
                          UInt     * aIdx );

    /* Map�� ũ�� (Map������ Slot����)�� ��ȯ�� */
    static UInt getSlotCount( void * aWAMapHdr )
    {
        sdtWAMapHdr * sWAMap = (sdtWAMapHdr*)aWAMapHdr;

        return sWAMap->mSlotCount;
    }

    /* Slot�ϳ��� ũ�⸦ ��ȯ��. */
    static UInt getSlotSize( void * aWAMapHdr )
    {
        sdtWAMapHdr * sWAMap = (sdtWAMapHdr*)aWAMapHdr;

        return mWMTypeDesc[ sWAMap->mWMType ].mSlotSize;
    }

    /* WAMap�� ����ϴ� �������� ������ ��ȯ*/
    static UInt getWPageCount( void * aWAMapHdr )
    {
        sdtWAMapHdr * sWAMap = (sdtWAMapHdr*)aWAMapHdr;

        if ( sWAMap == NULL )
        {
            return 0;
        }
        else
        {
            return ( sWAMap->mSlotCount * sWAMap->mSlotSize
                     * sWAMap->mVersionCount
                     / SD_PAGE_SIZE ) + 1;
        }
    }
    /* WAMap�� ����ϴ� ������ �������� ��ȯ�� */
    static scPageID getEndWPID( void * aWAMapHdr )
    {
        sdtWAMapHdr * sWAMap = (sdtWAMapHdr*)aWAMapHdr;

        return sWAMap->mBeginWPID + getWPageCount( aWAMapHdr );
    }


    static void dumpWAMap( void        * aMapHdr,
                           SChar       * aOutBuf,
                           UInt          aOutSize );
private:
    static sdtWMTypeDesc mWMTypeDesc[ SDT_WM_TYPE_MAX ];
};

/**************************************************************************
 * Description :
 * �ش� WAMap�� Index�� Slot�� Value�� �����Ѵ�.
 *
 * <IN>
 * aWAMap         - ��� WAMap
 * aIdx           - ��� Index
 * aValue         - ������ Value
 ***************************************************************************/
IDE_RC sdtWAMap::set( void * aWAMapHdr, UInt aIdx, void * aValue )
{
    sdtWAMapHdr * sWAMapHdr = (sdtWAMapHdr*)aWAMapHdr;
    void        * sSlotPtr;
    UInt          sSlotSize;

    sSlotSize = sWAMapHdr->mSlotSize;

    IDE_TEST( getSlotPtr( sWAMapHdr, aIdx, sSlotSize, &sSlotPtr )
              != IDE_SUCCESS );

    idlOS::memcpy( sSlotPtr, aValue, sSlotSize );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * �ش� WAMap�� Index�� Slot�� Value�� �����ϵ�, '���� ����'�� �����Ѵ�.
 * sdtSortModule::mergeSort ����
 *
 * <IN>
 * aWAMap         - ��� WAMap
 * aIdx           - ��� Index
 * aValue         - ������ Value
 ***************************************************************************/
IDE_RC sdtWAMap::setNextVersion( void * aWAMapHdr,
                                 UInt   aIdx,
                                 void * aValue )
{
    sdtWAMapHdr * sWAMapHdr = (sdtWAMapHdr*)aWAMapHdr;
    void        * sSlotPtr;
    UInt          sSlotSize;
    UInt          sVersionIdx;

    sSlotSize = sWAMapHdr->mSlotSize;
    sVersionIdx = ( sWAMapHdr->mVersionIdx + 1 ) % sWAMapHdr->mVersionCount;
    IDE_TEST( getSlotPtrWithVersion( sWAMapHdr,
                                     sVersionIdx,
                                     aIdx,
                                     sSlotSize,
                                     &sSlotPtr )
              != IDE_SUCCESS );

    idlOS::memcpy( sSlotPtr, aValue, sSlotSize );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}


/**************************************************************************
 * Description :
 * �ش� WAMap�� Index�� Slot���� vULongValue�� �����Ѵ�.
 *
 * <IN>
 * aWAMap         - ��� WAMap
 * aIdx           - ��� Index
 * aValue         - ������ Value
 ***************************************************************************/
IDE_RC sdtWAMap::setvULong( void * aWAMapHdr, UInt aIdx, vULong * aValue )
{
    sdtWAMapHdr * sWAMapHdr = (sdtWAMapHdr*)aWAMapHdr;
    void        * sSlotPtr;


    IDE_DASSERT( sWAMapHdr->mWMType == SDT_WM_TYPE_POINTER );
    IDE_DASSERT( ID_SIZEOF( vULong ) == getSlotSize( aWAMapHdr ) );
    IDE_TEST( getSlotPtr( sWAMapHdr, aIdx, ID_SIZEOF( vULong ), &sSlotPtr )
              != IDE_SUCCESS );

    *(vULong*)sSlotPtr = *aValue;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * �ش� WAMap�� Index�� Slot���� Value�� �����´�.
 *
 * <IN>
 * aWAMap         - ��� WAMap
 * aIdx           - ��� Index
 * aValue         - ������ Value
 ***************************************************************************/
IDE_RC sdtWAMap::get( void * aWAMapHdr, UInt aIdx, void * aValue )
{
    sdtWAMapHdr * sWAMapHdr = (sdtWAMapHdr*)aWAMapHdr;
    void        * sSlotPtr;
    UInt          sSlotSize;

    sSlotSize = sWAMapHdr->mSlotSize;
    IDE_TEST( getSlotPtr( sWAMapHdr, aIdx, sSlotSize, &sSlotPtr )
              != IDE_SUCCESS );

    idlOS::memcpy( aValue, sSlotPtr, sSlotSize );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * �ش� WAMap�� Index�� Slot���� vULongValue�� �����´�.
 *
 * <IN>
 * aWAMap         - ��� WAMap
 * aIdx           - ��� Index
 * aValue         - ������ Value
 ***************************************************************************/
IDE_RC sdtWAMap::getvULong( void * aWAMapHdr, UInt aIdx, vULong * aValue )
{
    sdtWAMapHdr * sWAMapHdr = (sdtWAMapHdr*)aWAMapHdr;
    void        * sSlotPtr;

    IDE_DASSERT( sWAMapHdr->mWMType == SDT_WM_TYPE_POINTER );
    IDE_DASSERT( ID_SIZEOF( vULong ) == getSlotSize( aWAMapHdr ) );
    IDE_TEST( getSlotPtr( sWAMapHdr, aIdx, ID_SIZEOF( vULong ), &sSlotPtr )
              != IDE_SUCCESS );

    *aValue = *(vULong*)sSlotPtr;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * WAMap�� �ش� Slot�� �ּҰ��� ��ȯ�Ѵ�.
 *
 * <IN>
 * aWAMap         - ��� WAMap
 * aIdx           - ��� Index
 * aSlotSize      - ��� WAMap���� �� Slot�� ũ��
 * <OUT>
 * aSlotPtr       - ã�� Slot�� ��ġ
 ***************************************************************************/
IDE_RC sdtWAMap::getSlotPtr( void *  aWAMapHdr,
                             UInt    aIdx,
                             UInt    aSlotSize,
                             void ** aSlotPtr )
{
    sdtWAMapHdr * sWAMapHdr = (sdtWAMapHdr*)aWAMapHdr;

    return getSlotPtrWithVersion( aWAMapHdr,
                                  sWAMapHdr->mVersionIdx,
                                  aIdx,
                                  aSlotSize,
                                  aSlotPtr );
}

/**************************************************************************
 * Description :
 * getSlotPtr�� ����, WAMap Slot�� ������ Page�� ���¸� Ȯ���ϰ� �����´�.
 * PageState�� none�̸� ���� �����̶� �ʱ�ȭ�Ǿ����� �ʾ� DirtyValue��
 * �б� �����̴�.
 * �׷��� �ʱ�ȭ ���ְ� PageState�� Init���� �����Ѵ�.
 * sdtUniqueHashModule::insert ����
 *
 * <IN>
 * aWAMap         - ��� WAMap
 * aIdx           - ��� Index
 * <OUT>
 * aSlotPtr       - ã�� Slot�� ��ġ
 ***************************************************************************/
IDE_RC sdtWAMap::getSlotPtrWithCheckState( void    * aWAMapHdr,
                                           UInt      aIdx,
                                           idBool    aResetPage,
                                           void   ** aSlotPtr )
{
    sdtWAMapHdr    * sWAMapHdr = (sdtWAMapHdr*)aWAMapHdr;
    sdtWASegment   * sWASeg = (sdtWASegment*)sWAMapHdr->mWASegment;
    UInt             sSlotSize;
    UInt             sOffset;
    void           * sSlotPtr;
    scPageID         sWPID;
    sdtWAPageState   sWAPageState;
    sdtWCB         * sWCBPtr;

    sSlotSize   = sWAMapHdr->mSlotSize;
    sOffset     = getOffset( aIdx,
                             sSlotSize,
                             sWAMapHdr->mVersionIdx,
                             sWAMapHdr->mVersionCount );
    sWPID       = sWAMapHdr->mBeginWPID + ( sOffset / SD_PAGE_SIZE );

    sWCBPtr      = sdtWASegment::getWCB( sWASeg, sWPID );
    sWAPageState = sdtWASegment::getWAPageState( sWCBPtr );
    sSlotPtr     = sWCBPtr->mWAPagePtr + ( sOffset % SD_PAGE_SIZE );

    if ( sWAPageState == SDT_WA_PAGESTATE_NONE )
    {
        if ( aResetPage == ID_TRUE )
        {
            idlOS::memset( sWCBPtr->mWAPagePtr, 0, SD_PAGE_SIZE );
            IDE_TEST( sdtWASegment::setWAPageState( sWASeg,
                                                    sWPID,
                                                    SDT_WA_PAGESTATE_INIT )
                      != IDE_SUCCESS );
        }
        else
        {
            idlOS::memset( sSlotPtr, 0, sSlotSize );
        }
    }

    *aSlotPtr = sSlotPtr;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}


/**************************************************************************
 * Description :
 * WAMap�� �ش� Slot�� �ּҰ��� ��ȯ�Ѵ�. Version�� ����Ͽ� �����´�.
 *
 * <IN>
 * aWAMap         - ��� WAMap
 * aIdx           - ��� Index
 * aSlotSize      - ��� WAMap���� �� Slot�� ũ��
 * <OUT>
 * aSlotPtr       - ã�� Slot�� ��ġ
 ***************************************************************************/
IDE_RC sdtWAMap::getSlotPtrWithVersion( void *  aWAMapHdr,
                                        UInt    aVersionIdx,
                                        UInt    aIdx,
                                        UInt    aSlotSize,
                                        void ** aSlotPtr )
{
    sdtWAMapHdr * sWAMapHdr = (sdtWAMapHdr*)aWAMapHdr;

    IDE_DASSERT( aIdx < sWAMapHdr->mSlotCount );

    *aSlotPtr = (void*)
        getPtrByOffset( aWAMapHdr,
                        getOffset( aIdx,
                                   aSlotSize,
                                   aVersionIdx,
                                   sWAMapHdr->mVersionCount ) );

    return IDE_SUCCESS;
}

/**************************************************************************
 * Description :
 * WAMap�� �ش� Slot�� �ּҰ��� Offset�� �������� ã�´�.
 *
 * <IN>
 * aWAMap         - ��� WAMap
 * aOffset        - ��� Offset
 * <OUT>
 * aSlotPtr       - ã�� Slot�� ��ġ
 ***************************************************************************/
UChar * sdtWAMap::getPtrByOffset( void *  aWAMapHdr,
                                  UInt    aOffset )
{
    sdtWAMapHdr * sWAMapHdr = (sdtWAMapHdr*)aWAMapHdr;

    return sdtWASegment::getWAPagePtr( (sdtWASegment*)sWAMapHdr->mWASegment,
                                        SDT_WAGROUPID_NONE,
                                        sWAMapHdr->mBeginWPID
                                        + ( aOffset / SD_PAGE_SIZE ) )
                                        + ( aOffset % SD_PAGE_SIZE );
}

/**************************************************************************
 * Description :
 * �� Slot�� ���� ��ȯ�Ѵ�.
 * sdtTempTable::quickSort ����
 *
 * <IN>
 * aWAMap         - ��� WAMap
 * aIdx1,aIdx2    - ġȯ�� �� Slot
 ***************************************************************************/
IDE_RC sdtWAMap::swap( void * aWAMapHdr, UInt aIdx1, UInt aIdx2 )
{
    sdtWAMapHdr * sWAMapHdr = (sdtWAMapHdr*)aWAMapHdr;
    UChar         sBuffer[ SDT_WAMAP_SLOT_MAX_SIZE ];
    UInt          sSlotSize;
    void        * sSlotPtr1;
    void        * sSlotPtr2;

    if ( aIdx1 == aIdx2 )
    {
        /* ������ ��ȯ�� �ʿ� ���� */
    }
    else
    {
        sSlotSize = sWAMapHdr->mSlotSize;

        IDE_TEST( getSlotPtr( sWAMapHdr, aIdx1, sSlotSize, &sSlotPtr1 )
                  != IDE_SUCCESS );
        IDE_TEST( getSlotPtr( sWAMapHdr, aIdx2, sSlotSize, &sSlotPtr2 )
                  != IDE_SUCCESS );

#if defined(DEBUG)
        /* �ٸ� �� ���� ���ؾ� �Ѵ�. */
        IDE_ERROR( sSlotPtr1 != sSlotPtr2 );
        /* �� Slot���� ���̰� Slot�� ũ�⺸�� ���ų� Ŀ��,
         * ��ġ�� �ʵ��� �ؾ� �Ѵ�. */
        if ( sSlotPtr1 < sSlotPtr2 )
        {
            IDE_ERROR( ((UChar*)sSlotPtr2) - ((UChar*)sSlotPtr1) >= sSlotSize );
        }
        else
        {
            IDE_ERROR( ((UChar*)sSlotPtr1) - ((UChar*)sSlotPtr2) >= sSlotSize );
        }
#endif

        idlOS::memcpy( sBuffer, sSlotPtr1, sSlotSize );
        idlOS::memcpy( sSlotPtr1, sSlotPtr2, sSlotSize );
        idlOS::memcpy( sSlotPtr2, sBuffer, sSlotSize );
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * �� vULongSlot�� ���� ��ȯ�Ѵ�.
 * sdtTempTable::quickSort ����
 *
 * <IN>
 * aWAMap         - ��� WAMap
 * aIdx1,aIdx2    - ġȯ�� �� Slot
 ***************************************************************************/
IDE_RC sdtWAMap::swapvULong( void * aWAMapHdr, UInt aIdx1, UInt aIdx2 )
{
    sdtWAMapHdr * sWAMapHdr = (sdtWAMapHdr*)aWAMapHdr;
    UInt          sSlotSize;
    vULong      * sSlotPtr1;
    vULong      * sSlotPtr2;
    vULong        sValue;

    IDE_DASSERT( sWAMapHdr->mWMType == SDT_WM_TYPE_POINTER );
    IDE_DASSERT( ID_SIZEOF( vULong ) == getSlotSize( aWAMapHdr ) );

    if ( aIdx1 == aIdx2 )
    {
        /* ������ ��ȯ�� �ʿ� ���� */
    }
    else
    {
        sSlotSize = ID_SIZEOF( vULong );

        IDE_TEST( getSlotPtr( sWAMapHdr, aIdx1, sSlotSize, (void**)&sSlotPtr1)
                  != IDE_SUCCESS );
        IDE_TEST( getSlotPtr( sWAMapHdr, aIdx2, sSlotSize, (void**)&sSlotPtr2)
                  != IDE_SUCCESS );

#if defined(DEBUG)
        /* �ٸ� �� ���� ���ؾ� �Ѵ�. */
        IDE_ERROR( sSlotPtr1 != sSlotPtr2 );
        /* �� Slot���� ���̰� Slot�� ũ�⺸�� ���ų� Ŀ��,
         * ��ġ�� �ʵ��� �ؾ� �Ѵ�. */
        if ( sSlotPtr1 < sSlotPtr2 )
        {
            IDE_ERROR( ((UChar*)sSlotPtr2) - ((UChar*)sSlotPtr1) >= sSlotSize );
        }
        else
        {
            IDE_ERROR( ((UChar*)sSlotPtr1) - ((UChar*)sSlotPtr2) >= sSlotSize );
        }
#endif

        sValue     = *sSlotPtr1;
        *sSlotPtr1 = *sSlotPtr2;
        *sSlotPtr2 = sValue;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * Map�� ������ ������Ų��.
 *
 * <IN>
 * aWAMap         - ��� WAMap
 ***************************************************************************/
IDE_RC sdtWAMap::incVersionIdx( void * aWAMapHdr )
{
    sdtWAMapHdr * sWAMap = (sdtWAMapHdr*)aWAMapHdr;

    sWAMap->mVersionIdx = ( sWAMap->mVersionIdx + 1 ) % sWAMap->mVersionCount;

    return IDE_SUCCESS;
}

#endif //_O_SDT_WA_MAP_H_
