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
 

#ifndef _O_SDT_UNIQUE_HASH_H_
#define _O_SDT_UNIQUE_HASH_H_ 1

#include <idu.h>
#include <smDef.h>
#include <smnDef.h>
#include <sdtWAMap.h>
#include <sdtDef.h>
#include <sdtTempRow.h>

extern smnIndexModule sdtUniqueHashModule;

class sdtUniqueHashModule
{
public:
    static IDE_RC init( void * aHeader );
    static IDE_RC destroy( void * aHeader);

    /************************ Interface ***********************/
    static IDE_RC insert(void     * aTempTableHeader,
                         smiValue * aValue,
                         UInt       aHashValue,
                         scGRID   * aGRID,
                         idBool   * aResult );
    static IDE_RC openCursor(void * aTempTableHeader,
                             void * aCursor );
    static IDE_RC fetchHashScan(void    * aTempCursor,
                                UChar  ** aRow,
                                scGRID  * aRowGRID );
    static IDE_RC fetchFullScan(void    * aTempCursor,
                                UChar  ** aRow,
                                scGRID  * aRowGRID );
    static IDE_RC closeCursor(void * aTempCursor);

    /*********************** Internal **************************/
    static IDE_RC compareRowAndValue( idBool        * aResult,
                                      const void    * aRow,
                                      void          *, /* aDirectKey */
                                      UInt           , /* aDirectKeyPartialSize */
                                      const scGRID    aGRID,
                                      void          * aData );

    inline static IDE_RC findExactRow(smiTempTableHeader  * aHeader,
                                      sdtWASegment        * aWASegment,
                                      idBool                aResetPage,
                                      const smiCallBack   * aFilter,
                                      UInt                  aHashValue,
                                      UInt                * aMapIdx,
                                      UChar              ** aSrcRowPtr,
                                      scGRID             ** aBucketGRID,
                                      scGRID              * aTargetGRID );
    inline static IDE_RC checkNextRow( smiTempTableHeader  * aHeader,
                                       sdtWASegment        * aWASegment,
                                       const smiCallBack   * aFilter,
                                       UInt                  aHashValue,
                                       scGRID              * aGRID,
                                       UChar              ** aSrcRowPtr,
                                       idBool              * aResult );

    static IDE_RC getNextNPage( sdtWASegment   * aWASegment,
                                smiTempCursor  * aCursor );
};

/**************************************************************************
 * Description :
 * hashValue�� �������� �ڽŰ� ���� hashValue �� value�� ���� Row��
 * ã���ϴ�.
 * ã���� rowPointer �� RID�� �����ϰ�, ������
 * NULL, NULL_RID�� ��ȯ�մϴ�.
 *
 * <IN>
 * aHeader        - ��� Table
 * aWASegment     - ��� WASegment
 * aResetPage     - reset page
 * aFilter        - ������ Row�� ���� Filter
 * aHashValue     - Ž���� ��� Hash
 * aMapIndx       - Ž���� ������ HashMap�� Index��ȣ
 * <OUT>
 * aRowPtr        - Row�� Ž���� ���
 * aBucketGRID    - Hash���� �ش��ϴ� BucketGRID�� ������
 * aTargetGRID    - ã�� Row
 ***************************************************************************/
IDE_RC sdtUniqueHashModule::findExactRow( smiTempTableHeader  * aHeader,
                                          sdtWASegment        * aWASeg,
                                          idBool                aResetPage,
                                          const smiCallBack   * aFilter,
                                          UInt                  aHashValue,
                                          UInt                * aMapIdx,
                                          UChar              ** aSrcRowPtr,
                                          scGRID             ** aBucketGRID,
                                          scGRID              * aTargetGRID )
{
    idBool              sResult;
    UInt                sLoop = 0;

    /*MapSize�� ��������, Map�� SlotIndex�� ã�� */
    *aMapIdx = aHashValue % sdtWAMap::getSlotCount( &aWASeg->mSortHashMapHdr );

    IDE_TEST( sdtWAMap::getSlotPtrWithCheckState( &aWASeg->mSortHashMapHdr,
                                                  *aMapIdx,
                                                  aResetPage,
                                                  (void**)aBucketGRID )
              != IDE_SUCCESS );
    *aTargetGRID = **aBucketGRID;

    while( !SC_GRID_IS_NULL( *aTargetGRID ) )
    {
        sLoop ++;
        IDE_TEST( checkNextRow( aHeader,
                                aWASeg,
                                aFilter,
                                aHashValue,
                                aTargetGRID,
                                aSrcRowPtr,
                                &sResult )
                  != IDE_SUCCESS );
        if( sResult == ID_TRUE )
        {
            /* ã�Ҵ�! */
            break;
        }
    }
    aHeader->mStatsPtr->mExtraStat2 =
        IDL_MAX( aHeader->mStatsPtr->mExtraStat2, sLoop );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/**************************************************************************
 * Description :
 * Row�� ���� Row�� ���� HaahValue�� ������, ������ ������� üũ��
 *
 * <IN>
 * aHeader        - ��� Table
 * aWASegment     - ��� WASegment
 * aFilter        - ������ Row�� ���� Filter
 * aHashValue     - Ž���� ��� Hash
 * <OUT>
 * aGRID          - ��� Row�� ��ġ
 * aSrcRowPtr     - Row�� Ž���� ���
 * aResult        - Ž�� ���� ����
 ***************************************************************************/
IDE_RC sdtUniqueHashModule::checkNextRow( smiTempTableHeader  * aHeader,
                                          sdtWASegment        * aWASegment,
                                          const smiCallBack   * aFilter,
                                          UInt                  aHashValue,
                                          scGRID              * aGRID,
                                          UChar              ** aSrcRowPtr,
                                          idBool              * aResult )
{
    sdtTRPHeader      * sTRPHeader = NULL;
    sdtTRPInfo4Select   sTRPInfo;
    idBool              sIsValidSlot = ID_FALSE;
    scGRID              sChildGRID;

    IDE_TEST( sdtWASegment::getPagePtrByGRID( aWASegment,
                                              SDT_WAGROUPID_SUB,
                                              *aGRID,
                                              (UChar**)&sTRPHeader,
                                              &sIsValidSlot )
              != IDE_SUCCESS );
    IDE_ERROR( sIsValidSlot == ID_TRUE );
    IDE_DASSERT( SM_IS_FLAG_ON( sTRPHeader->mTRFlag, SDT_TRFLAG_HEAD ) );

    *aSrcRowPtr =(UChar*) sTRPHeader;

    /*BUG-45474 fetch �߿� replace �� �߻��Ҽ� �����Ƿ� GRID�� ������ �ΰ� ����Ѵ�. */
    sChildGRID = sTRPHeader->mChildGRID;
#ifdef DEBUG
    if ( !SC_GRID_IS_NULL( sChildGRID ) )
    {
        IDE_DASSERT( sctTableSpaceMgr::isTempTableSpace( SC_MAKE_SPACE( sChildGRID ) ) == ID_TRUE );
    }
#endif

    if( sTRPHeader->mHashValue == aHashValue )
    {
        if( aFilter != NULL )
        {
            IDE_TEST( sdtTempRow::fetch( aWASegment,
                                         SDT_WAGROUPID_SUB,
                                         (UChar*)sTRPHeader,
                                         aHeader->mRowSize,
                                         aHeader->mRowBuffer4Fetch,
                                         &sTRPInfo )
                      != IDE_SUCCESS );

            IDE_TEST( aFilter->callback( aResult,
                                         sTRPInfo.mValuePtr,
                                         NULL,
                                         0,
                                         SC_NULL_GRID,
                                         aFilter->data)
                      != IDE_SUCCESS );

            if( *aResult == ID_FALSE )
            {
                *aGRID = sChildGRID;
            }
        }
        else
        {
            *aResult = ID_TRUE;
        }
    }
    else
    {
        /* HashValue���� �ٸ� */
        *aGRID = sChildGRID;
        *aResult = ID_FALSE;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;

}


#endif /* _O_SDT_UNIQUE_HASH_H_ */
