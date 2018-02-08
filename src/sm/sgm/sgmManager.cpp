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
 * SGM Layer
 *
 *    SGM(Storage Global Memory)�� SMM�� SVM�� ���� ���μ�,
 *    SMM�� SVM�� �����ϴ� ������ �Ѵ�.
 *    SC �迭�� SM, SV�� ���� ����̰�,
 *    SG �迭�� SM, SV�� �����ϴ� ���� ����̴�.
 *
 *    +-----------------------------------------------+
 *    |               SGM, SGP(?), SGC(?)             |
 *    +-----------------------------------------------+
 *    | SMM, SMP, SMC | SVM, SVP, SVC | SDD, SDP, SDC |
 *    +-----------------------------------------------+
 *    |                 SCR, SCM, SCT                 |
 *    +-----------------------------------------------+
 *
 *    ** SGP, SGC�� ���� ����
 ***********************************************************************/
//fix BUG-18251.
#include <idl.h>
#include <smp.h>
#include <sct.h> 
#include <smmManager.h>
#include <svmManager.h>
#include <svcRecord.h>
#include <sgmManager.h>

SChar* sgmManager::getVarColumn( SChar           * aRow,
                                 const smiColumn * aColumn,
                                 UInt            * aLength )
{
    scSpaceID   sSpaceID    = aColumn->colSpace;
    SChar     * sBufferPtr  = NULL;
    SChar     * sRet        = NULL;


    if ( aColumn->value != NULL )
    {
        // BUG-27649 CodeSonar::Null Pointer Dereference (9)
        sBufferPtr = (SChar*)(aColumn->value);
    }

    if ( sctTableSpaceMgr::isMemTableSpace( sSpaceID )  == ID_TRUE )
    {
        sRet = smcRecord::getVarRow( aRow,
                                     aColumn,
                                     0,
                                     aLength,
                                     sBufferPtr,
                                     ID_TRUE );

    }
    else
    {
        IDE_ASSERT( sctTableSpaceMgr::isVolatileTableSpace( sSpaceID )
                    == ID_TRUE );

        sRet = svcRecord::getVarRow( aRow,
                                     aColumn,
                                     0,
                                     aLength,
                                     sBufferPtr,
                                     ID_TRUE );
    }

    return sRet;
}

SChar* sgmManager::getVarColumn( SChar           * aRow,
                                 const smiColumn * aColumn,
                                 SChar           * aDestBuffer  )
{
    scSpaceID  sSpaceID = aColumn->colSpace;
    SChar    * sRet = NULL;
    UInt       sLen = 0;


    if ( sctTableSpaceMgr::isMemTableSpace( sSpaceID ) == ID_TRUE )
    {
        sRet = smcRecord::getVarRow( aRow,
                                     aColumn,
                                     0,
                                     &sLen,
                                     aDestBuffer,
                                     ID_TRUE );
    }
    else
    {
        IDE_ASSERT( sctTableSpaceMgr::isVolatileTableSpace( sSpaceID ) == ID_TRUE );

        sRet = svcRecord::getVarRow( aRow,
                                     aColumn,
                                     0,
                                     &sLen,
                                     aDestBuffer,
                                     ID_TRUE );
    }

    if ( sLen <= SMP_VC_PIECE_MAX_SIZE )
    {
        idlOS::memcpy( aDestBuffer, sRet, sLen );
    }
    else
    {
        /* Nothing to do */
    }

    return sRet;
}

// PROJ-2264
SChar* sgmManager::getCompressionVarColumn( SChar           * aRow,
                                            const smiColumn * aColumn,
                                            UInt*             aLength )
{
    smVCDesc  * sVCDesc;
    void      * sFstVarPiecePtr = NULL;
    idBool      sIsSameColumn = ID_FALSE;
    scSpaceID   sSpaceID;
    SChar     * sRet          = NULL;
    SChar     * sRow          = (SChar*)aRow + SMP_SLOT_HEADER_SIZE;
    smiColumn   sCompColumn;

    // PROJ-2429 Dictionary based data compress for on-disk DB
    if ( sctTableSpaceMgr::isDiskTableSpace( aColumn->colSpace ) == ID_TRUE )
    {
        sSpaceID = SMI_ID_TABLESPACE_SYSTEM_MEMORY_DATA;
    }
    else
    {
        sSpaceID = aColumn->colSpace;
    }

    if ( ( aColumn->flag & SMI_COLUMN_TYPE_MASK ) == SMI_COLUMN_TYPE_VARIABLE_LARGE )
    {
        sVCDesc = (smVCDesc*)sRow;
        *aLength = sVCDesc->length;

        if ( aColumn->value != NULL )
        {
            if ( ( sVCDesc->flag & SM_VCDESC_MODE_MASK )
                   == SM_VCDESC_MODE_OUT )
            {
                /* Out Mode�� ����� Row�� �о��� ����Ÿ�� ���ۿ� ����� ���
                 * ù 8byte���� ���� ���ۿ� �ִ� ����Ÿ�� DB�� ��ġ�� Row��
                 * ���� Pointer�� �ִ�.*/
                IDE_ASSERT( sVCDesc->length != 0 );
                IDE_ASSERT( getOIDPtr( sSpaceID,
                                       sVCDesc->fstPieceOID,
                                       (void**)&sFstVarPiecePtr )
                        == IDE_SUCCESS );

                /* ���ۿ��� ������ ���� ����Ÿ�� ����Ǿ� �ִ�. */
                if ( *((void**)( aColumn->value )) == sFstVarPiecePtr )
                {
                    sIsSameColumn = ID_TRUE;
                }
                else
                {
                    /* Nothing to do */
                }
            }
            else
            {
                /* Nothing to do */
            }
        }
        else
        {
            /* Nothing to do */
        }
    }
    else
    {
        /* Nothing to do */
    }


    if ( sIsSameColumn == ID_FALSE )
    {
        if ( sctTableSpaceMgr::isMemTableSpace( sSpaceID ) == ID_TRUE )
        {
            IDE_ASSERT_MSG( ( ( aColumn->flag & SMI_COLUMN_TYPE_MASK )
                              == SMI_COLUMN_TYPE_VARIABLE ) ||
                            ( ( aColumn->flag & SMI_COLUMN_TYPE_MASK )
                              == SMI_COLUMN_TYPE_VARIABLE_LARGE ),
                            "flag : %"ID_UINT32_FMT"\n",
                            aColumn->flag );
            IDE_ASSERT_MSG( ( aColumn->flag & SMI_COLUMN_STORAGE_MASK )
                            == SMI_COLUMN_STORAGE_MEMORY,
                            "flag : %"ID_UINT32_FMT"\n",
                            aColumn->flag );

            idlOS::memset( &sCompColumn,
                           0x00,
                           sizeof(sCompColumn) );
            sCompColumn.flag     = aColumn->flag;
            sCompColumn.size     = aColumn->size;
            sCompColumn.colSpace = aColumn->colSpace;
            sCompColumn.varOrder = 0; /* dictionary table ���� �ϳ��� column�� �ִ� */
            sCompColumn.value    = NULL;

            sRet = getVarColumn( aRow,
                                 &sCompColumn,
                                 aLength );
        }
        else
        {
            // ���� �������� �ʴ´�.
            IDE_ASSERT(0);
        }

        if ( aColumn->value != NULL )
        {
            *((void**)(aColumn->value)) = sRet;
        }
    }

    return sRet;
}
