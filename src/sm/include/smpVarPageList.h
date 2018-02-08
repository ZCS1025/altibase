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
 * $Id: smpVarPageList.h 82075 2018-01-17 06:39:52Z jina.kim $
 **********************************************************************/

#ifndef _O_SMP_VAR_PAGELIST_H_
#define _O_SMP_VAR_PAGELIST_H_ 1

#include <smDef.h>

struct smiValue;

class smpVarPageList
{
public:
    // Runtime Item�� NULL�� �����Ѵ�.
    static IDE_RC setRuntimeNull( UInt              aVarEntryCount,
                                  smpPageListEntry* aVarEntryArray );
    
    /* runtime ���� �� mutex �ʱ�ȭ */
    static IDE_RC initEntryAtRuntime( smOID             aTableOID,
                                      smpPageListEntry* aVarEntry,
                                      smpAllocPageListEntry* aAllocPageList );
    
    /* runtime ���� �� mutex ���� */
    static IDE_RC finEntryAtRuntime( smpPageListEntry* aVarEntry );
    
    // PageList�� �ʱ�ȭ�Ѵ�.
    static void   initializePageListEntry( smpPageListEntry* aVarEntry,
                                           smOID             aTableOID );
        
    // PageList�� refine�Ѵ�.
    static IDE_RC refinePageList( void*             aTrans,
                                  scSpaceID         aSpaceID,
                                  smpPageListEntry* aVarEntry );

    // aVarEntry�� �����ϰ� DB�� PageList �ݳ�
    static IDE_RC freePageListToDB( void*             aTrans,
                                    scSpaceID         aSpaceID,
                                    smOID             aTableOID,
                                    smpPageListEntry* aVarEntry );
    
    // aPage�� �ʱ�ȭ�Ѵ�.
    static void   initializePage( vULong        aIdx,
                                  UInt          aPageListID,
                                  vULong        aSlotSize,
                                  vULong        aSlotCount,
                                  smOID         aTableOID,
                                  smpPersPage*  aPageID );
    
    // aOID�� �ش��ϴ� Slot�� aValue�� �����Ѵ�.
    static IDE_RC setValue( scSpaceID       aSpaceID,
                            smOID           aOID,
                            const void*     aValue,
                            UInt            aLength);

    /* aFstPieceOID�� �ش��ϴ� Row�� �о�´�.*/
    static SChar* getValue( scSpaceID       aSpaceID,
                            UInt            aBeginPos,
                            UInt            aReadLen,
                            smOID           aFstPieceOID,
                            SChar          *aBuffer );

    /* temporary table header�� column, index ��������
       �����ϱ� ���� variable slot�� nologging���� slot�� �Ҵ��ϴ� ���ڸ� �߰� */
    static IDE_RC allocSlotForTempTableHdr( scSpaceID          aSpaceID,
                                            smOID              aTableOID,
                                            smpPageListEntry*  aVarEntry,
                                            UInt               aPieceSize,
                                            smOID              aNextOID,                                            
                                            smOID*             aPieceOID,
                                            SChar**            aPiecePtr,
                                            UInt               aPieceType );

    // Variable Slot�� �Ҵ��Ѵ�.
    static IDE_RC allocSlot( void*              aTrans,
                             scSpaceID          aSpaceID,
                             smOID              aTableOID,
                             smpPageListEntry*  aVarEntry,
                             UInt               aPieceSize,
                             smOID              aNextOID,
                             smOID*             aPieceOID,
                             SChar**            aPiecePtr,
                             UInt               aPieceType = SM_VCPIECE_TYPE_OTHER );
    
    // FreeSlotList���� Slot�� �����´�.
    static void   removeSlotFromFreeSlotList(
        scSpaceID          aSpaceID,
        smpFreePageHeader* aFreePageHeader,
        smOID*             aPieceOID,
        SChar**            aPiecePtr);

    // slot�� free�Ѵ�.
    static IDE_RC freeSlot( void*             aTrans,
                            scSpaceID         aSpaceID,
                            smpPageListEntry* aVarEntry,
                            smOID             aPieceOID,
                            SChar*            aPiecePtr,
                            smLSN*            aLsnNTA,
                            smpTableType      aTableType );

    // ���� FreeSlot�� FreeSlotList�� �߰��Ѵ�.
    static IDE_RC addFreeSlotPending( void*             aTrans,
                                      scSpaceID         aSpaceID,
                                      smpPageListEntry* aVarEntry,
                                      smOID             aPieceOID,
                                      SChar*            aPiecePtr );

    // aCurRow���� ��ȿ�� aNxtRow�� �����Ѵ�.
    static IDE_RC nextOIDallForRefineDB( scSpaceID          aSpaceID,
                                         smpPageListEntry * aVarEntry,
                                         smOID              aCurPieceOID,
                                         SChar            * aCurRow,
                                         smOID            * aNxtPieceOID,
                                         SChar           ** aNxtPiecePtr,
                                         UInt             * aIdx);

    // PageList�� Record ������ �����Ѵ�.
    static IDE_RC getRecordCount( smpPageListEntry* aVarEntry,
                                  ULong*            aRecordCount );

    // VarPage�� VarIdx���� ���Ѵ�.
    static UInt getVarIdx( void* aPagePtr );

    static inline SChar* getPieceValuePtr( void* aPiecePtr, UInt aPos )
    {
        return (SChar*)((smVCPieceHeader*)aPiecePtr + 1) + aPos;
    }


    /* BUG-31206    improve usability of DUMPCI and DUMPDDF */
    /* VCPiece�� altibase_sm.log�� �����Ѵ� */
    static void dumpVCPieceHeader( smVCPieceHeader     * aSlotHeader );


    /* BUG-31206    improve usability of DUMPCI and DUMPDDF */
    /* VCPiec(VariableColumnPiece)�� ����Ѵ�. */
    static void dumpVCPieceHeaderByBuffer( smVCPieceHeader * aVCPHeader,
                                           idBool            aDisplayTable,
                                           SChar           * aOutBuf,
                                           UInt              aOutSize );

    /* BUG-31206    improve usability of DUMPCI and DUMPDDF */
    /* dumpVarPageByBuffer�� �̿��� VarPage�� altibase_sm.log�� �����Ѵ� */
    static void dumpVarPage( scSpaceID         aSpaceID,
                             scPageID          aPageID );

    /* BUG-31206    improve usability of DUMPCI and DUMPDDF */
    /* VarPage�� �����Ѵ� */
    static void dumpVarPageByBuffer( UChar            * aPagePtr,
                                     SChar            * aOutBuf,
                                     UInt               aOutSize );

    // calcVarIdx �� ������ �ϱ� ���� AllocArray�� �����Ѵ�.
    static void initAllocArray();

private:
    
    /* FOR A4 :  system���κ��� persistent page�� �Ҵ�޴� �Լ� */
    static IDE_RC allocPersPages( void*             aTrans,
                                  scSpaceID         aSpaceID,
                                  smpPageListEntry* aVarEntry,
                                  UInt              aIdx );
    
    // nextOIDall�� ���� aRow���� �ش� Page�� ã���ش�.
    static IDE_RC initForScan( scSpaceID          aSpaceID,
                               smpPageListEntry * aVarEntry,
                               smOID              aPieceOID,
                               SChar            * aPiecePtr,
                               smpPersPage     ** aPage,
                               smOID            * aNxtPieceOID,
                               SChar           ** aNxtPiecePtr );

    // allocSlot�ϱ����� PrivatePageList�� �˻��Ͽ� �õ�
    static IDE_RC tryForAllocSlotFromPrivatePageList( void*       aTrans,
                                                      scSpaceID   aSpaceID,
                                                      smOID       aTableOID,
                                                      UInt        aIdx,
                                                      smOID*      aPieceOID,
                                                      SChar**     aPiecePtr );

    // allocSlot�ϱ����� FreePageList�� FreePagePool�� �˻��Ͽ� �õ�
    static IDE_RC tryForAllocSlotFromFreePageList(
        void*             aTrans,
        scSpaceID         aSpaceID,
        smpPageListEntry* aVarEntry,
        UInt              aPageListID,
        smOID*            aPieceOID,
        SChar**           aPiecePtr );

    // FreeSlotList ����
    static IDE_RC buildFreeSlotList( scSpaceID         aSpaceID,
                                     smpPageListEntry* aVarEntry );

    // FreeSlot ������ ����Ѵ�.
    static IDE_RC setFreeSlot( void*        aTrans,
                               scSpaceID    aSpaceID,
                               scPageID     aPageID,
                               smOID        aVCPieceOID,
                               SChar*       aVCPiecePtr,
                               smLSN*       aNTA,
                               smpTableType aTableType );

    // FreePageHeader�� FreeSlotList�� FreeSlot�� �߰��Ѵ�.
    static void   addFreeSlotToFreeSlotList( scSpaceID aSpaceID,
                                             scPageID  aPageID,
                                             SChar*    aPiecePtr );

    // aValue�� �ش��ϴ� VarIdx���� ã�´�.
    static inline IDE_RC calcVarIdx( UInt   aLength,
                                     UInt*  aVarIdx );

    // Page���� FreeSlotList�� ������ �ùٸ��� �˻��Ѵ�.
    static idBool isValidFreeSlotList(smpFreePageHeader* aFreePageHeader );
};

#endif /* _O_SMP_VAR_PAGELIST_H_ */
