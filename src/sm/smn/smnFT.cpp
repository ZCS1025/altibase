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
 * $Id: smnFT.cpp 37720 2010-01-11 08:53:19Z cgkim $
 **********************************************************************/

#include <ide.h>
#include <idu.h>
#include <smErrorCode.h>

#include <smDef.h>
#include <smp.h>
#include <sdp.h>
#include <smc.h>
#include <smm.h>
#include <smn.h>
#include <smr.h>
#include <smnFT.h>
#include <smnReq.h>
#include <smxTrans.h>


//======================================================================
//  X$INDEX
//  index�� general information�� �����ִ� peformance view
//======================================================================

IDE_RC smnFT::buildRecordForIndexInfo(idvSQL              * /*aStatistics*/,
                                      void        *aHeader,
                                      void        * /* aDumpObj */,
                                      iduFixedTableMemory *aMemory)
{
    smcTableHeader     *sCatTblHdr;
    smcTableHeader     *sTableHeader;
    smpSlotHeader      *sPtr;
    SChar              *sCurPtr;
    SChar              *sNxtPtr;
    smnIndexHeader     *sIndexCursor;
    smnIndexInfo4PerfV  sIndexInfo;
    void               *sTrans;
    UInt                sIndexCnt;
    UInt                i;
    void              * sISavepoint = NULL;
    UInt                sDummy = 0;

    IDE_ERROR( aHeader != NULL );
    IDE_ERROR( aMemory != NULL );

    sCatTblHdr = (smcTableHeader*)SMC_CAT_TABLE;
    sCurPtr = NULL;

    if ( aMemory->useExternalMemory() == ID_TRUE )
    {
        /* BUG-43006 FixedTable Indexing Filter */
        sTrans = ((smiFixedTableProperties *)aMemory->getContext())->mTrans;
    }
    else
    {
        /* BUG-32292 [sm-util] Self deadlock occur since fixed-table building
         * operation uses another transaction. 
         * NestedTransaction�� ����ϸ� Self-deadlock ����� �ִ�.
         * ���� id Memory �������κ��� Iterator�� ��� Transaction�� ����. */
        sTrans = ((smiIterator*)aMemory->getContext())->trans;
    }

    while(1)
    {
        IDE_TEST(smcRecord::nextOIDall(sCatTblHdr,sCurPtr,&sNxtPtr)
                  != IDE_SUCCESS);
        if( sNxtPtr == NULL )
        {
            break;
        }//if sNxtPtr
        sPtr = (smpSlotHeader *)sNxtPtr;
        // To fix BUG-14681
        if( SM_SCN_IS_INFINITE(sPtr->mCreateSCN) == ID_TRUE )
        {
            /* BUG-14974: ���� Loop�߻�.*/
            sCurPtr = sNxtPtr;
            continue;
        }
        sTableHeader = (smcTableHeader *)( sPtr + 1 );

        if( smcTable::isDropedTable(sTableHeader) == ID_TRUE )
        {
            sCurPtr = sNxtPtr;
            continue;
        }//if

        sIndexCnt =  smcTable::getIndexCount(sTableHeader);

        if( sIndexCnt != 0  )
        {
            //DDL �� ����.
            IDE_TEST( smLayerCallback::setImpSavepoint( sTrans, 
                                                        &sISavepoint,
                                                        sDummy )
                      != IDE_SUCCESS );
            IDE_TEST( smLayerCallback::lockTableModeIS( sTrans,
                                                        SMC_TABLE_LOCK( sTableHeader ) )
                      != IDE_SUCCESS );

            //lock�� ������� table�� drop�� ��쿡�� skip;
            if(smcTable::isDropedTable(sTableHeader) == ID_TRUE)
            {
                IDE_TEST( smLayerCallback::abortToImpSavepoint( sTrans, 
                                                                sISavepoint )
                          != IDE_SUCCESS );
                IDE_TEST( smLayerCallback::unsetImpSavepoint( sTrans, 
                                                              sISavepoint )
                          != IDE_SUCCESS );
                sCurPtr = sNxtPtr;
                continue;
            }//if

            // lock�� ����ϴ� ���� index�� drop�Ǿ��ų�, ���ο� index��
            // �����Ǿ��� �� �����Ƿ� ��Ȯ�� index ���� �ٽ� ���Ѵ�.
            // �Ӹ� �ƴ϶�, index cnt�� ������Ų �� index�� �����ϹǷ�
            // index�� �Ϸ���� ���ϸ� index cnt�� �����ϹǷ� �ٽ� ���ؾ� ��.
            sIndexCnt = smcTable::getIndexCount(sTableHeader);

            for(i=0;i < sIndexCnt; i++ )
            {
                sIndexCursor=(smnIndexHeader*)smcTable::getTableIndex(sTableHeader,i);

                idlOS::memset(&sIndexInfo,0x00,ID_SIZEOF(smnIndexInfo4PerfV));
                sIndexInfo.mTableOID     = sIndexCursor->mTableOID;
                sIndexInfo.mIndexSegPID  = SC_MAKE_PID(sIndexCursor->mIndexSegDesc);
                sIndexInfo.mIndexID      = sIndexCursor->mId;
                // primary key�� �Ϲ� �ε��� ��?
                sIndexInfo.mIndexType  =   sIndexCursor->mFlag &  SMI_INDEX_TYPE_MASK;

                IDE_TEST(iduFixedTable::buildRecord(aHeader,
                                                    aMemory,
                                                    (void *)&sIndexInfo)
                     != IDE_SUCCESS);


            }//for
            IDE_TEST( smLayerCallback::abortToImpSavepoint( sTrans, 
                                                            sISavepoint )
                      != IDE_SUCCESS );
            IDE_TEST( smLayerCallback::unsetImpSavepoint( sTrans, 
                                                          sISavepoint )
                      != IDE_SUCCESS );
        }// if �ε����� ������
        sCurPtr = sNxtPtr;
    }// while

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

static iduFixedTableColDesc  gIndexInfoColDesc[]=
{

    {
        (SChar*)"TABLE_OID",
        offsetof(smnIndexInfo4PerfV,mTableOID),
        IDU_FT_SIZEOF(smnIndexInfo4PerfV,mTableOID),
        IDU_FT_TYPE_UBIGINT,
        NULL,
        0, 0,NULL // for internal use
    },

    {
        (SChar*)"INDEX_SEG_PID",
        offsetof(smnIndexInfo4PerfV,mIndexSegPID),
        IDU_FT_SIZEOF(smnIndexInfo4PerfV,mIndexSegPID),
        IDU_FT_TYPE_UINTEGER,
        NULL,
        0, 0,NULL // for internal use
    },

    {
        (SChar*)"INDEX_ID",
        offsetof(smnIndexInfo4PerfV,mIndexID),
        IDU_FT_SIZEOF(smnIndexInfo4PerfV,mIndexID),
        IDU_FT_TYPE_UINTEGER,
        NULL,
        0, 0,NULL // for internal use
    },

    {
        (SChar*)"INDEX_TYPE",
        offsetof(smnIndexInfo4PerfV,mIndexType ),
        IDU_FT_SIZEOF(smnIndexInfo4PerfV,mIndexType ),
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

// X$INDEX
iduFixedTableDesc  gIndexDesc=
{
    (SChar *)"X$INDEX",
    smnFT::buildRecordForIndexInfo,
    gIndexInfoColDesc,
    IDU_STARTUP_META,
    0,
    0,
    IDU_FT_DESC_TRANS_USE,
    NULL
};
