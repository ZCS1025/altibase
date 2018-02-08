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
 * $Id: qmnUpdate.cpp 55241 2012-08-27 09:13:19Z linkedlist $
 *
 * Description :
 *     UPTE(UPdaTE) Node
 *
 *     ������ �𵨿��� update�� �����ϴ� Plan Node �̴�.
 *
 * ��� ���� :
 *
 * ��� :
 *
 **********************************************************************/

#include <idl.h>
#include <ide.h>
#include <qcg.h>
#include <qmnUpdate.h>
#include <qmoPartition.h>
#include <qdnTrigger.h>
#include <qdnForeignKey.h>
#include <qdbCommon.h>
#include <qdnCheck.h>
#include <qmsDefaultExpr.h>
#include <qmx.h>
#include <qcuTemporaryObj.h>
#include <qdtCommon.h>

IDE_RC
qmnUPTE::init( qcTemplate * aTemplate,
               qmnPlan    * aPlan )
{
/***********************************************************************
 *
 * Description :
 *    UPTE ����� �ʱ�ȭ
 *
 * Implementation :
 *
 ***********************************************************************/

#define IDE_FN "qmnUPTE::init"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY("qmnUPTE::init"));

    qmncUPTE * sCodePlan = (qmncUPTE*) aPlan;
    qmndUPTE * sDataPlan =
        (qmndUPTE*) (aTemplate->tmplate.data + aPlan->offset);

    sDataPlan->flag = & aTemplate->planFlag[sCodePlan->planID];
    sDataPlan->doIt = qmnUPTE::doItDefault;

    //------------------------------------------------
    // ���� �ʱ�ȭ ���� ���� �Ǵ�
    //------------------------------------------------

    if ( ( *sDataPlan->flag & QMND_UPTE_INIT_DONE_MASK )
         == QMND_UPTE_INIT_DONE_FALSE )
    {
        // ���� �ʱ�ȭ ����
        IDE_TEST( firstInit(aTemplate, sCodePlan, sDataPlan) != IDE_SUCCESS );

        //------------------------------------------------
        // Child Plan�� �ʱ�ȭ
        //------------------------------------------------

        IDE_TEST( aPlan->left->init( aTemplate,
                                     aPlan->left ) != IDE_SUCCESS);

        //---------------------------------
        // trigger row�� ����
        //---------------------------------

        // child�� offset�� �̿��ϹǷ� firstInit�� ������ offset�� �̿��� �� �ִ�.
        IDE_TEST( allocTriggerRow(aTemplate, sCodePlan, sDataPlan)
                  != IDE_SUCCESS );

        //---------------------------------
        // returnInto row�� ����
        //---------------------------------

        IDE_TEST( allocReturnRow(aTemplate, sCodePlan, sDataPlan)
                  != IDE_SUCCESS );

        //---------------------------------
        // index table cursor�� ����
        //---------------------------------

        IDE_TEST( allocIndexTableCursor(aTemplate, sCodePlan, sDataPlan)
                  != IDE_SUCCESS );

        //---------------------------------
        // �ʱ�ȭ �ϷḦ ǥ��
        //---------------------------------

        *sDataPlan->flag &= ~QMND_UPTE_INIT_DONE_MASK;
        *sDataPlan->flag |= QMND_UPTE_INIT_DONE_TRUE;
    }
    else
    {
        //------------------------------------------------
        // Child Plan�� �ʱ�ȭ
        //------------------------------------------------

        IDE_TEST( aPlan->left->init( aTemplate,
                                     aPlan->left ) != IDE_SUCCESS);

        //-----------------------------------
        // init lob info
        //-----------------------------------

        if ( sDataPlan->lobInfo != NULL )
        {
            (void) qmx::initLobInfo( sDataPlan->lobInfo );
        }
        else
        {
            // Nothing to do.
        }
    }

    //------------------------------------------------
    // ���� Data �� �ʱ�ȭ
    //------------------------------------------------

    // Limit ���� ������ �ʱ�ȭ
    sDataPlan->limitCurrent = 1;

    // update rowGRID �ʱ�ȭ
    sDataPlan->rowGRID = SC_NULL_GRID;
    
    //------------------------------------------------
    // ���� �Լ� ����
    //------------------------------------------------

    sDataPlan->doIt = qmnUPTE::doItFirst;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;

#undef IDE_FN
}

IDE_RC
qmnUPTE::doIt( qcTemplate * aTemplate,
               qmnPlan    * aPlan,
               qmcRowFlag * aFlag )
{
/***********************************************************************
 *
 * Description :
 *    UPTE �� ���� ����� �����Ѵ�.
 *
 * Implementation :
 *    ������ �Լ� �����͸� �����Ѵ�.
 *
 ***********************************************************************/

#define IDE_FN "qmnUPTE::doIt"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY(""));

    qmndUPTE * sDataPlan =
        (qmndUPTE*) (aTemplate->tmplate.data + aPlan->offset);

    return sDataPlan->doIt( aTemplate, aPlan, aFlag );

#undef IDE_FN
}

IDE_RC
qmnUPTE::padNull( qcTemplate * aTemplate,
                  qmnPlan    * aPlan )
{
/***********************************************************************
 *
 * Description :
 *    UPTE ���� ������ null row�� ������ ������,
 *    Child�� ���Ͽ� padNull()�� ȣ���Ѵ�.
 *
 * Implementation :
 *
 ***********************************************************************/

#define IDE_FN "qmnUPTE::padNull"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY("qmnUPTE::padNull"));

    qmncUPTE * sCodePlan = (qmncUPTE*) aPlan;
    // qmndUPTE * sDataPlan =
    //     (qmndUPTE*) (aTemplate->tmplate.data + aPlan->offset);

    if ( (aTemplate->planFlag[sCodePlan->planID] & QMND_UPTE_INIT_DONE_MASK)
         == QMND_UPTE_INIT_DONE_FALSE )
    {
        // �ʱ�ȭ���� ���� ��� �ʱ�ȭ ����
        IDE_TEST( aPlan->init( aTemplate, aPlan ) != IDE_SUCCESS );
    }
    else
    {
        // Nothing To Do
    }

    // Child Plan�� ���Ͽ� Null Padding����
    IDE_TEST( aPlan->left->padNull( aTemplate, aPlan->left )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;

#undef IDE_FN
}

IDE_RC
qmnUPTE::printPlan( qcTemplate   * aTemplate,
                    qmnPlan      * aPlan,
                    ULong          aDepth,
                    iduVarString * aString,
                    qmnDisplay     aMode )
{
/***********************************************************************
 *
 * Description :
 *    UPTE ����� ���� ������ ����Ѵ�.
 *
 * Implementation :
 *
 ***********************************************************************/

#define IDE_FN "qmnUPTE::printPlan"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY("qmnUPTE::printPlan"));

    qmncUPTE * sCodePlan = (qmncUPTE*) aPlan;
    qmndUPTE * sDataPlan =
        (qmndUPTE*) (aTemplate->tmplate.data + aPlan->offset);

    ULong      i;
    qmmValueNode * sValue;

    sDataPlan->flag = & aTemplate->planFlag[sCodePlan->planID];

    //------------------------------------------------------
    // ���� ������ ���
    //------------------------------------------------------

    for ( i = 0; i < aDepth; i++ )
    {
        iduVarStringAppend( aString,
                            " " );
    }

    //------------------------------------------------------
    // UPTE Target ������ ���
    //------------------------------------------------------

    // UPTE ������ ���
    if ( sCodePlan->tableRef->tableType == QCM_VIEW )
    {
        iduVarStringAppendFormat( aString,
                                  "UPDATE ( VIEW: " );
    }
    else
    {
        iduVarStringAppendFormat( aString,
                                  "UPDATE ( TABLE: " );
    }

    if ( ( sCodePlan->tableOwnerName.name != NULL ) &&
         ( sCodePlan->tableOwnerName.size > 0 ) )
    {
        iduVarStringAppendLength( aString,
                                  sCodePlan->tableOwnerName.name,
                                  sCodePlan->tableOwnerName.size );
        iduVarStringAppend( aString, "." );
    }
    else
    {
        // Nothing to do.
    }

    //----------------------------
    // Table Name ���
    //----------------------------

    if ( ( sCodePlan->tableName.size <= QC_MAX_OBJECT_NAME_LEN ) &&
         ( sCodePlan->tableName.name != NULL ) &&
         ( sCodePlan->tableName.size > 0 ) )
    {
        iduVarStringAppendLength( aString,
                                  sCodePlan->tableName.name,
                                  sCodePlan->tableName.size );
    }
    else
    {
        // Nothing to do.
    }

    //----------------------------
    // Alias Name ���
    //----------------------------

    if ( sCodePlan->aliasName.name != NULL &&
         sCodePlan->aliasName.size > 0  &&
         sCodePlan->aliasName.name != sCodePlan->tableName.name )
    {
        // Table �̸� ������ Alias �̸� ������ �ٸ� ���
        // (alias name)
        iduVarStringAppend( aString, " " );

        if ( sCodePlan->aliasName.size <= QC_MAX_OBJECT_NAME_LEN )
        {
            iduVarStringAppendLength( aString,
                                      sCodePlan->aliasName.name,
                                      sCodePlan->aliasName.size );
        }
        else
        {
            // Nothing to do.
        }
    }
    else
    {
        // Alias �̸� ������ ���ų� Table �̸� ������ ������ ���
        // Nothing To Do
    }

    //----------------------------
    // New line ���
    //----------------------------
    iduVarStringAppend( aString, " )\n" );

    //------------------------------------------------------
    // BUG-38343 Set ������ Subquery ���� ���
    //------------------------------------------------------

    for ( sValue = sCodePlan->values;
          sValue != NULL;
          sValue = sValue->next)
    {
        IDE_TEST( qmn::printSubqueryPlan( aTemplate,
                                          sValue->value,
                                          aDepth,
                                          aString,
                                          aMode ) != IDE_SUCCESS );
    }

    //------------------------------------------------------
    // Child Plan ������ ���
    //------------------------------------------------------

    IDE_TEST( aPlan->left->printPlan( aTemplate,
                                      aPlan->left,
                                      aDepth + 1,
                                      aString,
                                      aMode ) != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;

#undef IDE_FN
}

IDE_RC
qmnUPTE::firstInit( qcTemplate * aTemplate,
                    qmncUPTE   * aCodePlan,
                    qmndUPTE   * aDataPlan )
{
/***********************************************************************
 *
 * Description :
 *    UPTE node�� Data ������ ����� ���� �ʱ�ȭ�� ����
 *
 * Implementation :
 *    - Data ������ �ֿ� ����� ���� �ʱ�ȭ�� ����
 *
 ***********************************************************************/

#define IDE_FN "qmnUPTE::firstInit"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY("qmnUPTE::firstInit"));

    ULong          sCount;
    UShort         sTableID;
    qcmTableInfo * sTableInfo;
    idBool         sIsNeedRebuild = ID_FALSE;

    //--------------------------------
    // ���ռ� �˻�
    //--------------------------------

    //--------------------------------
    // UPTE ���� ������ �ʱ�ȭ
    //--------------------------------

    aDataPlan->insertLobInfo = NULL;
    aDataPlan->insertValues  = NULL;
    aDataPlan->checkValues   = NULL;

    // Tuple Set������ �ʱ�ȭ
    aDataPlan->updateTuple     = & aTemplate->tmplate.rows[aCodePlan->tableRef->table];
    aDataPlan->updateCursor    = NULL;
    aDataPlan->updateTupleID   = ID_USHORT_MAX;

    /* PROJ-2464 hybrid partitioned table ���� */
    aDataPlan->updatePartInfo = NULL;

    // index table cursor �ʱ�ȭ
    aDataPlan->indexUpdateCursor = NULL;
    aDataPlan->indexUpdateTuple = NULL;

    // set, where column list �ʱ�ȭ
    smiInitDMLRetryInfo( &(aDataPlan->retryInfo) );

    /* PROJ-2359 Table/Partition Access Option */
    aDataPlan->accessOption = QCM_ACCESS_OPTION_READ_WRITE;

    //--------------------------------
    // cursorInfo ����
    //--------------------------------

    if ( aCodePlan->insteadOfTrigger == ID_TRUE )
    {
        // instead of trigger�� cursor�� �ʿ����.
        // Nothing to do.
    }
    else
    {
        sTableInfo = aCodePlan->tableRef->tableInfo;

        // PROJ-2219 Row-level before update trigger
        // Invalid �� trigger�� ������ compile�ϰ�, DML�� rebuild �Ѵ�.
        if ( sTableInfo->triggerCount > 0 )
        {
            IDE_TEST( qdnTrigger::verifyTriggers( aTemplate->stmt,
                                                  sTableInfo,
                                                  aCodePlan->updateColumnList,
                                                  &sIsNeedRebuild )
                      != IDE_SUCCESS );

            IDE_TEST_RAISE( sIsNeedRebuild == ID_TRUE,
                            trigger_invalid );
        }
        else
        {
            // Nothing to do.
        }

        IDE_TEST( allocCursorInfo( aTemplate, aCodePlan, aDataPlan )
                  != IDE_SUCCESS );
    }

    //--------------------------------
    // partition ���� ������ �ʱ�ȭ
    //--------------------------------

    if ( aCodePlan->tableRef->tableInfo->lobColumnCount > 0 )
    {
        // PROJ-1362
        IDE_TEST( qmx::initializeLobInfo( aTemplate->stmt,
                                          & aDataPlan->lobInfo,
                                          (UShort)aCodePlan->tableRef->tableInfo->lobColumnCount )
                  != IDE_SUCCESS );
    }
    else
    {
        aDataPlan->lobInfo = NULL;
    }

    switch ( aCodePlan->updateType )
    {
        case QMO_UPDATE_ROWMOVEMENT:
        {
            // insert cursor manager �ʱ�ȭ
            IDE_TEST( aDataPlan->insertCursorMgr.initialize(
                          aTemplate->stmt->qmxMem,
                          aCodePlan->insertTableRef,
                          ID_FALSE )
                      != IDE_SUCCESS );

            // lob info �ʱ�ȭ
            if ( aCodePlan->tableRef->tableInfo->lobColumnCount > 0 )
            {
                // PROJ-1362
                IDE_TEST( qmx::initializeLobInfo(
                              aTemplate->stmt,
                              & aDataPlan->insertLobInfo,
                              (UShort)aCodePlan->tableRef->tableInfo->lobColumnCount )
                          != IDE_SUCCESS );
            }
            else
            {
                aDataPlan->insertLobInfo = NULL;
            }

            // insert smiValues �ʱ�ȭ
            IDE_TEST( aTemplate->stmt->qmxMem->alloc(
                          aCodePlan->tableRef->tableInfo->columnCount
                          * ID_SIZEOF(smiValue),
                          (void**)& aDataPlan->insertValues )
                      != IDE_SUCCESS );

            break;
        }

        case QMO_UPDATE_CHECK_ROWMOVEMENT:
        {
            // check smiValues �ʱ�ȭ
            IDE_TEST( aTemplate->stmt->qmxMem->alloc(
                          aCodePlan->tableRef->tableInfo->columnCount
                          * ID_SIZEOF(smiValue),
                          (void**)& aDataPlan->checkValues )
                      != IDE_SUCCESS );

            break;
        }

        default:
            break;
    }

    //--------------------------------
    // Limitation ���� ������ �ʱ�ȭ
    //--------------------------------

    if( aCodePlan->limit != NULL )
    {
        IDE_TEST( qmsLimitI::getStartValue(
                      aTemplate,
                      aCodePlan->limit,
                      &aDataPlan->limitStart )
                  != IDE_SUCCESS );

        IDE_TEST( qmsLimitI::getCountValue(
                      aTemplate,
                      aCodePlan->limit,
                      &sCount )
                  != IDE_SUCCESS );

        aDataPlan->limitEnd = aDataPlan->limitStart + sCount;
    }
    else
    {
        aDataPlan->limitStart = 1;
        aDataPlan->limitEnd   = 0;
    }

    // ���ռ� �˻�
    if ( aDataPlan->limitEnd > 0 )
    {
        IDE_ASSERT( (aCodePlan->flag & QMNC_UPTE_LIMIT_MASK)
                    == QMNC_UPTE_LIMIT_TRUE );
    }

    //------------------------------------------
    // Default Expr�� Row Buffer ����
    //------------------------------------------

    if ( aCodePlan->defaultExprColumns != NULL )
    {
        sTableID = aCodePlan->defaultExprTableRef->table;

        IDE_TEST( qmc::setRowSize( aTemplate->stmt->qmxMem,
                                   &(aTemplate->tmplate),
                                   sTableID )
                  != IDE_SUCCESS );

        if ( (aTemplate->tmplate.rows[sTableID].lflag & MTC_TUPLE_STORAGE_MASK)
             == MTC_TUPLE_STORAGE_MEMORY )
        {
            IDE_TEST( aTemplate->stmt->qmxMem->alloc(
                          aTemplate->tmplate.rows[sTableID].rowOffset,
                          &(aTemplate->tmplate.rows[sTableID].row) )
                      != IDE_SUCCESS );
        }
        else
        {
            /* Disk Table�� ���, qmc::setRowSize()���� �̹� �Ҵ� */
        }

        aDataPlan->defaultExprRowBuffer = aTemplate->tmplate.rows[sTableID].row;
    }
    else
    {
        aDataPlan->defaultExprRowBuffer = NULL;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( trigger_invalid )
    {
        IDE_SET( ideSetErrorCode( qpERR_REBUILD_TRIGGER_INVALID ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;

#undef IDE_FN
}

IDE_RC
qmnUPTE::allocCursorInfo( qcTemplate * aTemplate,
                          qmncUPTE   * aCodePlan,
                          qmndUPTE   * aDataPlan )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation :
 *
 ***********************************************************************/

#define IDE_FN "qmnUPTE::allocCursorInfo"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY("qmnUPTE::allocCursorInfo"));

    qmnCursorInfo     * sCursorInfo;
    qmsPartitionRef   * sPartitionRef;
    UInt                sPartitionCount;
    UInt                sIndexUpdateColumnCount;
    UInt                i;
    idBool              sIsInplaceUpdate = ID_FALSE;

    //--------------------------------
    // cursorInfo ����
    //--------------------------------

    IDE_TEST( aTemplate->stmt->qmxMem->alloc( ID_SIZEOF(qmnCursorInfo),
                                              (void**)& sCursorInfo )
              != IDE_SUCCESS );

    // cursorInfo �ʱ�ȭ
    sCursorInfo->cursor              = NULL;
    sCursorInfo->selectedIndex       = NULL;
    sCursorInfo->selectedIndexTuple  = NULL;
    sCursorInfo->accessOption        = QCM_ACCESS_OPTION_READ_WRITE; /* PROJ-2359 Table/Partition Access Option */
    sCursorInfo->updateColumnList    = aCodePlan->updateColumnList;
    sCursorInfo->cursorType          = aCodePlan->cursorType;
    sCursorInfo->isRowMovementUpdate = aCodePlan->isRowMovementUpdate;

    /* PROJ-2626 Snapshot Export */
    if ( aTemplate->stmt->mInplaceUpdateDisableFlag == ID_FALSE )
    {
        sIsInplaceUpdate = aCodePlan->inplaceUpdate;
    }
    else
    {
        /* Nothing td do */
    }

    sCursorInfo->inplaceUpdate = sIsInplaceUpdate;

    sCursorInfo->lockMode            = SMI_LOCK_WRITE;

    sCursorInfo->stmtRetryColLst     = aCodePlan->whereColumnList;
    sCursorInfo->rowRetryColLst      = aCodePlan->setColumnList;

    // cursorInfo ����
    aDataPlan->updateTuple->cursorInfo = sCursorInfo;

    //--------------------------------
    // partition cursorInfo ����
    //--------------------------------

    if ( aCodePlan->tableRef->partitionRef != NULL )
    {
        sPartitionCount = 0;
        for ( sPartitionRef = aCodePlan->tableRef->partitionRef;
              sPartitionRef != NULL;
              sPartitionRef = sPartitionRef->next )
        {
            sPartitionCount++;
        }

        // cursorInfo ����
        IDE_TEST( aTemplate->stmt->qmxMem->alloc(
                      sPartitionCount * ID_SIZEOF(qmnCursorInfo),
                      (void**)& sCursorInfo )
                  != IDE_SUCCESS );

        for ( sPartitionRef = aCodePlan->tableRef->partitionRef,
                  i = 0;
              sPartitionRef != NULL;
              sPartitionRef = sPartitionRef->next,
                  i++, sCursorInfo++ )
        {
            // cursorInfo �ʱ�ȭ
            sCursorInfo->cursor              = NULL;
            sCursorInfo->selectedIndex       = NULL;
            sCursorInfo->selectedIndexTuple  = NULL;
            /* PROJ-2359 Table/Partition Access Option */
            sCursorInfo->accessOption        = QCM_ACCESS_OPTION_READ_WRITE;
            sCursorInfo->updateColumnList    = aCodePlan->updatePartColumnList[i];
            sCursorInfo->cursorType          = aCodePlan->cursorType;
            sCursorInfo->isRowMovementUpdate = aCodePlan->isRowMovementUpdate;
            sCursorInfo->inplaceUpdate       = sIsInplaceUpdate;
            sCursorInfo->lockMode            = SMI_LOCK_WRITE;

            /* PROJ-2464 hybrid partitioned table ���� */
            sCursorInfo->stmtRetryColLst     = aCodePlan->wherePartColumnList[i];
            sCursorInfo->rowRetryColLst      = aCodePlan->setPartColumnList[i];

            // cursorInfo ����
            aTemplate->tmplate.rows[sPartitionRef->table].cursorInfo = sCursorInfo;
        }

        // PROJ-1624 non-partitioned index
        // partitioned table�� ��� index table cursor�� update column list�� �����Ѵ�.
        if ( aCodePlan->tableRef->selectedIndexTable != NULL )
        {
            IDE_DASSERT( aCodePlan->tableRef->indexTableRef != NULL );

            sCursorInfo = (qmnCursorInfo*) aDataPlan->updateTuple->cursorInfo;

            IDE_TEST( qmsIndexTable::makeUpdateSmiColumnList(
                          aCodePlan->updateColumnCount,
                          aCodePlan->updateColumnIDs,
                          aCodePlan->tableRef->selectedIndexTable,
                          sCursorInfo->isRowMovementUpdate,
                          & sIndexUpdateColumnCount,
                          aDataPlan->indexUpdateColumnList )
                      != IDE_SUCCESS );

            if ( sIndexUpdateColumnCount > 0 )
            {
                // update�� �÷��� �ִ� ���
                sCursorInfo->updateColumnList = aDataPlan->indexUpdateColumnList;
                // index table�� �׻� update, composite ����
                sCursorInfo->cursorType       = SMI_UPDATE_CURSOR;

                // update�ؾ���
                *aDataPlan->flag &= ~QMND_UPTE_SELECTED_INDEX_CURSOR_MASK;
                *aDataPlan->flag |= QMND_UPTE_SELECTED_INDEX_CURSOR_TRUE;
            }
            else
            {
                // update�� �÷��� ���� ���
                sCursorInfo->updateColumnList = NULL;
                sCursorInfo->cursorType       = SMI_SELECT_CURSOR;
                sCursorInfo->inplaceUpdate    = ID_FALSE;
                sCursorInfo->lockMode         = SMI_LOCK_READ;
            }
        }
        else
        {
            // Nothing to do.
        }
    }
    else
    {
        // Nothing to do.
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;

#undef IDE_FN
}

IDE_RC
qmnUPTE::allocTriggerRow( qcTemplate * aTemplate,
                          qmncUPTE   * aCodePlan,
                          qmndUPTE   * aDataPlan )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation :
 *
 ***********************************************************************/

#define IDE_FN "qmnUPTE::allocTriggerRow"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY("qmnUPTE::allocTriggerRow"));

    UInt sMaxRowOffsetForUpdate = 0;
    UInt sMaxRowOffsetForInsert = 0;

    //---------------------------------
    // Trigger�� ���� ������ ����
    //---------------------------------

    if ( aCodePlan->tableRef->tableInfo->triggerCount > 0 )
    {
        if ( aCodePlan->insteadOfTrigger == ID_TRUE )
        {
            // instead of trigger������ smiValues�� ����Ѵ�.

            // alloc sOldRow
            IDE_TEST( aTemplate->stmt->qmxMem->alloc(
                    ID_SIZEOF(smiValue) *
                    aCodePlan->tableRef->tableInfo->columnCount,
                    (void**) & aDataPlan->oldRow )
                != IDE_SUCCESS);

            // alloc sNewRow
            IDE_TEST( aTemplate->stmt->qmxMem->alloc(
                    ID_SIZEOF(smiValue) *
                    aCodePlan->tableRef->tableInfo->columnCount,
                    (void**) & aDataPlan->newRow )
                != IDE_SUCCESS);
        }
        else
        {
            sMaxRowOffsetForUpdate = qmx::getMaxRowOffset( &(aTemplate->tmplate),
                                                           aCodePlan->tableRef );
            if ( aCodePlan->updateType == QMO_UPDATE_ROWMOVEMENT )
            {
                sMaxRowOffsetForInsert = qmx::getMaxRowOffset( &(aTemplate->tmplate),
                                                               aCodePlan->insertTableRef );
            }
            else
            {
                sMaxRowOffsetForInsert = 0;
            }

            // ���ռ� �˻�
            IDE_DASSERT( sMaxRowOffsetForUpdate > 0 );

            // Old Row Referencing�� ���� ���� �Ҵ�
            IDE_TEST( aTemplate->stmt->qmxMem->alloc(
                    sMaxRowOffsetForUpdate,
                    (void**) & aDataPlan->oldRow )
                != IDE_SUCCESS);

            // New Row Referencing�� ���� ���� �Ҵ�
            IDE_TEST( aTemplate->stmt->qmxMem->cralloc(
                    IDL_MAX( sMaxRowOffsetForUpdate, sMaxRowOffsetForInsert ),
                    (void**) & aDataPlan->newRow )
                != IDE_SUCCESS);
        }

        aDataPlan->columnsForRow = aCodePlan->tableRef->tableInfo->columns;

        aDataPlan->needTriggerRow = ID_FALSE;
        aDataPlan->existTrigger = ID_TRUE;
    }
    else
    {
        // check constraint�� return into������ trigger row�� ����Ѵ�.
        if ( ( aCodePlan->checkConstrList != NULL ) ||
             ( aCodePlan->returnInto != NULL ) )
        {
            sMaxRowOffsetForUpdate = qmx::getMaxRowOffset( &(aTemplate->tmplate),
                                                           aCodePlan->tableRef );
            if ( aCodePlan->updateType == QMO_UPDATE_ROWMOVEMENT )
            {
                sMaxRowOffsetForInsert = qmx::getMaxRowOffset( &(aTemplate->tmplate),
                                                               aCodePlan->insertTableRef );
            }
            else
            {
                sMaxRowOffsetForInsert = 0;
            }

            // ���ռ� �˻�
            IDE_DASSERT( sMaxRowOffsetForUpdate > 0 );

            // Old Row Referencing�� ���� ���� �Ҵ�
            IDE_TEST( aTemplate->stmt->qmxMem->alloc(
                    sMaxRowOffsetForUpdate,
                    (void**) & aDataPlan->oldRow )
                != IDE_SUCCESS);

            // New Row Referencing�� ���� ���� �Ҵ�
            IDE_TEST( aTemplate->stmt->qmxMem->cralloc(
                    IDL_MAX( sMaxRowOffsetForUpdate, sMaxRowOffsetForInsert ),
                    (void**) & aDataPlan->newRow )
                != IDE_SUCCESS);
        }
        else
        {
            aDataPlan->oldRow = NULL;
            aDataPlan->newRow = NULL;
        }

        aDataPlan->needTriggerRow = ID_FALSE;
        aDataPlan->existTrigger = ID_FALSE;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;

#undef IDE_FN
}

IDE_RC
qmnUPTE::allocReturnRow( qcTemplate * aTemplate,
                         qmncUPTE   * aCodePlan,
                         qmndUPTE   * aDataPlan )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation :
 *
 ***********************************************************************/

#define IDE_FN "qmnUPTE::allocReturnRow"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY("qmnUPTE::allocReturnRow"));

    UInt sMaxRowOffset = 0;

    //---------------------------------
    // return into�� ���� ������ ����
    //---------------------------------

    if ( ( aCodePlan->returnInto != NULL ) &&
         ( aCodePlan->insteadOfTrigger == ID_TRUE ) )
    {
        sMaxRowOffset = qmx::getMaxRowOffset( &(aTemplate->tmplate),
                                              aCodePlan->tableRef );

        // New Row Referencing�� ���� ���� �Ҵ�
        IDE_TEST( aTemplate->stmt->qmxMem->cralloc(
                sMaxRowOffset,
                (void**) & aDataPlan->returnRow )
            != IDE_SUCCESS);
    }
    else
    {
        aDataPlan->returnRow = NULL;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;

#undef IDE_FN
}

IDE_RC
qmnUPTE::allocIndexTableCursor( qcTemplate * aTemplate,
                                qmncUPTE   * aCodePlan,
                                qmndUPTE   * aDataPlan )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation :
 *
 ***********************************************************************/

#define IDE_FN "qmnUPTE::allocIndexTableCursor"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY("qmnUPTE::allocIndexTableCursor"));

    //---------------------------------
    // index table ó���� ���� ����
    //---------------------------------

    if ( aCodePlan->tableRef->indexTableRef != NULL )
    {
        IDE_TEST( qmsIndexTable::initializeIndexTableCursors(
                      aTemplate->stmt,
                      aCodePlan->tableRef->indexTableRef,
                      aCodePlan->tableRef->indexTableCount,
                      aCodePlan->tableRef->selectedIndexTable,
                      & (aDataPlan->indexTableCursorInfo) )
                  != IDE_SUCCESS );

        *aDataPlan->flag &= ~QMND_UPTE_INDEX_CURSOR_MASK;
        *aDataPlan->flag |= QMND_UPTE_INDEX_CURSOR_INITED;
    }
    else
    {
        // Nothing to do.
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;

#undef IDE_FN
}

IDE_RC
qmnUPTE::doItDefault( qcTemplate * /* aTemplate */,
                      qmnPlan    * /* aPlan */,
                      qmcRowFlag * /* aFlag */ )
{
/***********************************************************************
 *
 * Description :
 *    �� �Լ��� ����Ǹ� �ȵ�.
 *
 * Implementation :
 *
 ***********************************************************************/

#define IDE_FN "qmnUPTE::doItDefault"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY(""));

    IDE_DASSERT( 0 );

    return IDE_FAILURE;

#undef IDE_FN
}

IDE_RC
qmnUPTE::doItFirst( qcTemplate * aTemplate,
                    qmnPlan    * aPlan,
                    qmcRowFlag * aFlag )
{
/***********************************************************************
 *
 * Description :
 *    UPTE�� ���� ���� �Լ�
 *
 * Implementation :
 *    - Table�� IX Lock�� �Ǵ�.
 *    - Session Event Check (������ ���� Detect)
 *    - Cursor Open
 *    - update one record
 *
 ***********************************************************************/

#define IDE_FN "qmnUPTE::doItFirst"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY(""));

    qmncUPTE * sCodePlan = (qmncUPTE*) aPlan;
    qmndUPTE * sDataPlan =
        (qmndUPTE*) (aTemplate->tmplate.data + aPlan->offset);

    /* PROJ-2359 Table/Partition Access Option */
    idBool     sIsTableCursorChanged;

    //-----------------------------------
    // Child Plan�� ������
    //-----------------------------------

    // To fix PR-3921
    if ( sDataPlan->limitCurrent == sDataPlan->limitEnd )
    {
        // �־��� Limit ���ǿ� �ٴٸ� ���
        *aFlag = QMC_ROW_DATA_NONE;
    }
    else
    {
        // doIt left child
        IDE_TEST( aPlan->left->doIt( aTemplate, aPlan->left, aFlag )
                  != IDE_SUCCESS );
    }

    if ( ( *aFlag & QMC_ROW_DATA_MASK ) == QMC_ROW_DATA_EXIST )
    {
        // Limit Start ó��
        for ( ;
              sDataPlan->limitCurrent < sDataPlan->limitStart;
              sDataPlan->limitCurrent++ )
        {
            // Limitation ������ ���� �ʴ´�.
            // ���� Update���� Child�� �����ϱ⸸ �Ѵ�.
            IDE_TEST( aPlan->left->doIt( aTemplate, aPlan->left, aFlag )
                      != IDE_SUCCESS );

            if ( ( *aFlag & QMC_ROW_DATA_MASK ) == QMC_ROW_DATA_NONE )
            {
                break;
            }
            else
            {
                // Nothing To Do
            }
        }

        if ( sDataPlan->limitStart <= sDataPlan->limitEnd )
        {
            if ( ( sDataPlan->limitCurrent >= sDataPlan->limitStart ) &&
                 ( sDataPlan->limitCurrent < sDataPlan->limitEnd ) )
            {
                // Limit�� ����
                sDataPlan->limitCurrent++;
            }
            else
            {
                // Limitation ������ ��� ���
                *aFlag = QMC_ROW_DATA_NONE;
            }
        }
        else
        {
            // Nothing To Do
        }

        if ( ( *aFlag & QMC_ROW_DATA_MASK ) == QMC_ROW_DATA_EXIST )
        {
            // check trigger
            IDE_TEST( checkTrigger( aTemplate, aPlan ) != IDE_SUCCESS );

            if ( sCodePlan->insteadOfTrigger == ID_TRUE )
            {
                IDE_TEST( fireInsteadOfTrigger( aTemplate, aPlan ) != IDE_SUCCESS );
            }
            else
            {
                // get cursor
                IDE_TEST( getCursor( aTemplate, aPlan, &sIsTableCursorChanged ) != IDE_SUCCESS );

                /* PROJ-2359 Table/Partition Access Option */
                IDE_TEST( qmx::checkAccessOption( sCodePlan->tableRef->tableInfo,
                                                  ID_FALSE /* aIsInsertion */ )
                          != IDE_SUCCESS );

                if ( sCodePlan->tableRef->partitionRef != NULL )
                {
                    IDE_TEST( qmx::checkAccessOptionForExistentRecord(
                                        sDataPlan->accessOption,
                                        sDataPlan->updateTuple->tableHandle )
                              != IDE_SUCCESS );
                }
                else
                {
                    /* Nothing to do */
                }

                switch ( sCodePlan->updateType )
                {
                    case QMO_UPDATE_NORMAL:
                    {
                        // update one record
                        IDE_TEST( updateOneRow( aTemplate, aPlan ) != IDE_SUCCESS );

                        break;
                    }

                    case QMO_UPDATE_ROWMOVEMENT:
                    {
                        // open insert cursor
                        IDE_TEST( openInsertCursor( aTemplate, aPlan ) != IDE_SUCCESS );

                        // update one record
                        IDE_TEST( updateOneRowForRowmovement( aTemplate, aPlan )
                                  != IDE_SUCCESS );

                        break;
                    }

                    case QMO_UPDATE_CHECK_ROWMOVEMENT:
                    {
                        // update one record
                        IDE_TEST( updateOneRowForCheckRowmovement( aTemplate, aPlan )
                                  != IDE_SUCCESS );

                        break;
                    }

                    case QMO_UPDATE_NO_ROWMOVEMENT:
                    {
                        // update one record
                        IDE_TEST( updateOneRow( aTemplate, aPlan ) != IDE_SUCCESS );

                        break;
                    }

                    default:
                        IDE_DASSERT(0);
                        break;
                }
            }

            sDataPlan->doIt = qmnUPTE::doItNext;
        }
        else
        {
            // Nothing To Do
        }
    }
    else
    {
        // nothing to do
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    if ( sDataPlan->lobInfo != NULL )
    {
        (void)qmx::finalizeLobInfo( aTemplate->stmt, sDataPlan->lobInfo );
    }

    return IDE_FAILURE;

#undef IDE_FN
}

IDE_RC
qmnUPTE::doItNext( qcTemplate * aTemplate,
                   qmnPlan    * aPlan,
                   qmcRowFlag * aFlag )
{
/***********************************************************************
 *
 * Description :
 *    UPTE�� ���� ���� �Լ�
 *    ���� Record�� �����Ѵ�.
 *
 * Implementation :
 *    - update one record
 *
 ***********************************************************************/

#define IDE_FN "qmnUPTE::doItNext"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY(""));

    qmncUPTE * sCodePlan = (qmncUPTE*) aPlan;
    qmndUPTE * sDataPlan =
        (qmndUPTE*) (aTemplate->tmplate.data + aPlan->offset);

    /* PROJ-2359 Table/Partition Access Option */
    idBool     sIsTableCursorChanged;
    
    //-----------------------------------
    // Child Plan�� ������
    //-----------------------------------

    // To fix PR-3921
    if ( sDataPlan->limitCurrent == sDataPlan->limitEnd )
    {
        // �־��� Limit ���ǿ� �ٴٸ� ���
        *aFlag = QMC_ROW_DATA_NONE;
    }
    else
    {
        // doIt left child
        IDE_TEST( aPlan->left->doIt( aTemplate, aPlan->left, aFlag )
                  != IDE_SUCCESS );
    }

    if ( ( *aFlag & QMC_ROW_DATA_MASK ) == QMC_ROW_DATA_EXIST )
    {
        if ( sDataPlan->limitStart <= sDataPlan->limitEnd )
        {
            if ( ( sDataPlan->limitCurrent >= sDataPlan->limitStart ) &&
                 ( sDataPlan->limitCurrent < sDataPlan->limitEnd ) )
            {
                // Limit�� ����
                sDataPlan->limitCurrent++;
            }
            else
            {
                // Limitation ������ ��� ���
                *aFlag = QMC_ROW_DATA_NONE;
            }
        }
        else
        {
            // Nothing To Do
        }

        if ( ( *aFlag & QMC_ROW_DATA_MASK ) == QMC_ROW_DATA_EXIST )
        {
            if ( sCodePlan->insteadOfTrigger == ID_TRUE )
            {
                IDE_TEST( fireInsteadOfTrigger( aTemplate, aPlan ) != IDE_SUCCESS );
            }
            else
            {
                switch ( sCodePlan->updateType )
                {
                    case QMO_UPDATE_NORMAL:
                    {
                        // update one record
                        IDE_TEST( updateOneRow( aTemplate, aPlan ) != IDE_SUCCESS );

                        break;
                    }

                    case QMO_UPDATE_ROWMOVEMENT:
                    {
                        // get cursor
                        IDE_TEST( getCursor( aTemplate, aPlan, &sIsTableCursorChanged ) != IDE_SUCCESS );

                        /* PROJ-2359 Table/Partition Access Option */
                        if ( sIsTableCursorChanged == ID_TRUE )
                        {
                            IDE_TEST( qmx::checkAccessOptionForExistentRecord(
                                                sDataPlan->accessOption,
                                                sDataPlan->updateTuple->tableHandle )
                                      != IDE_SUCCESS );
                        }
                        else
                        {
                            /* Nothing to do */
                        }

                        // update one record
                        IDE_TEST( updateOneRowForRowmovement( aTemplate, aPlan )
                                  != IDE_SUCCESS );

                        break;
                    }

                    case QMO_UPDATE_CHECK_ROWMOVEMENT:
                    {
                        // get cursor
                        IDE_TEST( getCursor( aTemplate, aPlan, &sIsTableCursorChanged ) != IDE_SUCCESS );

                        /* PROJ-2359 Table/Partition Access Option */
                        if ( sIsTableCursorChanged == ID_TRUE )
                        {
                            IDE_TEST( qmx::checkAccessOptionForExistentRecord(
                                                sDataPlan->accessOption,
                                                sDataPlan->updateTuple->tableHandle )
                                      != IDE_SUCCESS );
                        }
                        else
                        {
                            /* Nothing to do */
                        }

                        // update one record
                        IDE_TEST( updateOneRowForCheckRowmovement( aTemplate, aPlan )
                                  != IDE_SUCCESS );

                        break;
                    }

                    case QMO_UPDATE_NO_ROWMOVEMENT:
                    {
                        // get cursor
                        IDE_TEST( getCursor( aTemplate, aPlan, &sIsTableCursorChanged ) != IDE_SUCCESS );

                        /* PROJ-2359 Table/Partition Access Option */
                        if ( sIsTableCursorChanged == ID_TRUE )
                        {
                            IDE_TEST( qmx::checkAccessOptionForExistentRecord(
                                                sDataPlan->accessOption,
                                                sDataPlan->updateTuple->tableHandle )
                                      != IDE_SUCCESS );
                        }
                        else
                        {
                            /* Nothing to do */
                        }

                        // update one record
                        IDE_TEST( updateOneRow( aTemplate, aPlan ) != IDE_SUCCESS );

                        break;
                    }

                    default:
                        IDE_DASSERT(0);
                        break;
                }
            }
        }
        else
        {
            // record�� ���� ���
            // ���� ������ ���� ���� ���� �Լ��� ������.
            sDataPlan->doIt = qmnUPTE::doItFirst;
        }
    }
    else
    {
        // record�� ���� ���
        // ���� ������ ���� ���� ���� �Լ��� ������.
        sDataPlan->doIt = qmnUPTE::doItFirst;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    if ( sDataPlan->lobInfo != NULL )
    {
        (void)qmx::finalizeLobInfo( aTemplate->stmt, sDataPlan->lobInfo );
    }

    return IDE_FAILURE;

#undef IDE_FN
}

IDE_RC
qmnUPTE::checkTrigger( qcTemplate * aTemplate,
                       qmnPlan    * aPlan )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation :
 *
 ***********************************************************************/

#define IDE_FN "qmnUPTE::checkTrigger"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY(""));

    qmncUPTE * sCodePlan = (qmncUPTE*) aPlan;
    qmndUPTE * sDataPlan =
        (qmndUPTE*) (aTemplate->tmplate.data + aPlan->offset);
    idBool     sNeedTriggerRow;

    if ( sDataPlan->existTrigger == ID_TRUE )
    {
        if ( sCodePlan->insteadOfTrigger == ID_TRUE )
        {
            IDE_TEST( qdnTrigger::needTriggerRow(
                          aTemplate->stmt,
                          sCodePlan->tableRef->tableInfo,
                          QCM_TRIGGER_INSTEAD_OF,
                          QCM_TRIGGER_EVENT_UPDATE,
                          sCodePlan->updateColumnList,
                          & sNeedTriggerRow )
                      != IDE_SUCCESS );
        }
        else
        {
            // Trigger�� ���� Referencing Row�� �ʿ������� �˻�
            // PROJ-2219 Row-level before update trigger
            IDE_TEST( qdnTrigger::needTriggerRow(
                          aTemplate->stmt,
                          sCodePlan->tableRef->tableInfo,
                          QCM_TRIGGER_BEFORE,
                          QCM_TRIGGER_EVENT_UPDATE,
                          sCodePlan->updateColumnList,
                          & sNeedTriggerRow )
                      != IDE_SUCCESS );

            if ( sNeedTriggerRow == ID_FALSE )
            {
                IDE_TEST( qdnTrigger::needTriggerRow(
                              aTemplate->stmt,
                              sCodePlan->tableRef->tableInfo,
                              QCM_TRIGGER_AFTER,
                              QCM_TRIGGER_EVENT_UPDATE,
                              sCodePlan->updateColumnList,
                              & sNeedTriggerRow )
                          != IDE_SUCCESS );
            }
            else
            {
                // Nothing to do.
            }
        }

        sDataPlan->needTriggerRow = sNeedTriggerRow;
    }
    else
    {
        // Nothing to do.
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;

#undef IDE_FN
}

IDE_RC
qmnUPTE::getCursor( qcTemplate * aTemplate,
                    qmnPlan    * aPlan,
                    idBool     * aIsTableCursorChanged ) /* PROJ-2359 Table/Partition Access Option */
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation :
 *     ���� scan�� open�� cursor�� ��´�.
 *
 ***********************************************************************/

#define IDE_FN "qmnUPTE::getCursor"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY(""));

    qmncUPTE * sCodePlan = (qmncUPTE*) aPlan;
    qmndUPTE * sDataPlan =
        (qmndUPTE*) (aTemplate->tmplate.data + aPlan->offset);

    qmnCursorInfo   * sCursorInfo = NULL;

    /* PROJ-2359 Table/Partition Access Option */
    *aIsTableCursorChanged = ID_FALSE;

    if ( sCodePlan->tableRef->partitionRef == NULL )
    {
        if ( sDataPlan->updateTupleID != sCodePlan->tableRef->table )
        {
            sDataPlan->updateTupleID = sCodePlan->tableRef->table;

            // cursor�� ��´�.
            sCursorInfo = (qmnCursorInfo*)
                aTemplate->tmplate.rows[sDataPlan->updateTupleID].cursorInfo;

            IDE_TEST_RAISE( sCursorInfo == NULL, ERR_NOT_FOUND );

            sDataPlan->updateCursor    = sCursorInfo->cursor;

            /* PROJ-2464 hybrid partitioned table ���� */
            sDataPlan->updatePartInfo = sCodePlan->tableRef->tableInfo;

            sDataPlan->retryInfo.mIsWithoutRetry  = sCodePlan->withoutRetry;
            sDataPlan->retryInfo.mStmtRetryColLst = sCursorInfo->stmtRetryColLst;
            sDataPlan->retryInfo.mRowRetryColLst  = sCursorInfo->rowRetryColLst;

            /* PROJ-2359 Table/Partition Access Option */
            sDataPlan->accessOption = sCursorInfo->accessOption;
            *aIsTableCursorChanged  = ID_TRUE;
        }
        else
        {
            /* Nothing to do */
        }
    }
    else
    {
        if ( sDataPlan->updateTupleID != sDataPlan->updateTuple->partitionTupleID )
        {
            sDataPlan->updateTupleID = sDataPlan->updateTuple->partitionTupleID;

            // partition�� cursor�� ��´�.
            sCursorInfo = (qmnCursorInfo*)
                aTemplate->tmplate.rows[sDataPlan->updateTupleID].cursorInfo;

            /* BUG-42440 BUG-39399 has invalid erorr message */
            if ( ( sDataPlan->updateTuple->lflag & MTC_TUPLE_PARTITIONED_TABLE_MASK )
                 == MTC_TUPLE_PARTITIONED_TABLE_TRUE )
            {
                IDE_TEST_RAISE( sCursorInfo == NULL, ERR_MODIFY_UNABLE_RECORD );
            }
            else
            {
                /* Nothing to do */
            }
            IDE_TEST_RAISE( sCursorInfo == NULL, ERR_NOT_FOUND );

            sDataPlan->updateCursor    = sCursorInfo->cursor;

            /* PROJ-2464 hybrid partitioned table ���� */
            IDE_TEST( smiGetTableTempInfo( sDataPlan->updateTuple->tableHandle,
                                           (void **)&(sDataPlan->updatePartInfo) )
                      != IDE_SUCCESS );

            sDataPlan->retryInfo.mIsWithoutRetry  = sCodePlan->withoutRetry;
            sDataPlan->retryInfo.mStmtRetryColLst = sCursorInfo->stmtRetryColLst;
            sDataPlan->retryInfo.mRowRetryColLst  = sCursorInfo->rowRetryColLst;

            /* PROJ-2359 Table/Partition Access Option */
            sDataPlan->accessOption = sCursorInfo->accessOption;
            *aIsTableCursorChanged  = ID_TRUE;
        }
        else
        {
            /* Nothing to do */
        }

        // index table cursor�� ��´�.
        if ( sDataPlan->indexUpdateTuple == NULL )
        {
            sCursorInfo = (qmnCursorInfo*)
                aTemplate->tmplate.rows[sCodePlan->tableRef->table].cursorInfo;

            IDE_TEST_RAISE( sCursorInfo == NULL, ERR_NOT_FOUND );

            sDataPlan->indexUpdateCursor    = sCursorInfo->cursor;

            sDataPlan->indexUpdateTuple = sCursorInfo->selectedIndexTuple;
        }
        else
        {
            // Nothing to do.
        }
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_NOT_FOUND )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmnUPTE::getCursor",
                                  "cursor not found" ));
    }
    IDE_EXCEPTION( ERR_MODIFY_UNABLE_RECORD );
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMN_MODIFY_UNABLE_RECORD ) ) ;
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;

#undef IDE_FN
}

IDE_RC
qmnUPTE::openInsertCursor( qcTemplate * aTemplate,
                           qmnPlan    * aPlan )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation :
 *
 ***********************************************************************/

#define IDE_FN "qmnUPTE::openInsertCursor"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY(""));

    qmncUPTE * sCodePlan = (qmncUPTE*) aPlan;
    qmndUPTE * sDataPlan =
        (qmndUPTE*) (aTemplate->tmplate.data + aPlan->offset);

    smiCursorProperties   sCursorProperty;
    UShort                sTupleID         = 0;
    idBool                sIsDiskChecked   = ID_FALSE;
    smiFetchColumnList  * sFetchColumnList = NULL;

    if ( ( ( *sDataPlan->flag & QMND_UPTE_CURSOR_MASK )
           == QMND_UPTE_CURSOR_CLOSED )
         &&
         ( sCodePlan->insertTableRef != NULL ) )
    {
        // INSERT �� ���� Cursor ����
        SMI_CURSOR_PROP_INIT_FOR_FULL_SCAN( &sCursorProperty, aTemplate->stmt->mStatistics );

        if ( sCodePlan->insertTableRef->tableInfo->tablePartitionType == QCM_PARTITIONED_TABLE )
        {
            if ( sCodePlan->insertTableRef->partitionSummary->diskPartitionRef != NULL )
            {
                sTupleID = sCodePlan->insertTableRef->partitionSummary->diskPartitionRef->table;
                sIsDiskChecked = ID_TRUE;
            }
            else
            {
                /* Nothing to do */
            }
        }
        else
        {
            if ( QCM_TABLE_TYPE_IS_DISK( sCodePlan->insertTableRef->tableInfo->tableFlag ) == ID_TRUE )
            {
                sTupleID = sCodePlan->insertTableRef->table;
                sIsDiskChecked = ID_TRUE;
            }
            else
            {
                /* Nothing to do */
            }
        }

        if ( sIsDiskChecked == ID_TRUE )
        {
            // PROJ-1705
            // ����Ű üũ�� ���� �о�� �� ��ġ�÷�����Ʈ ����
            IDE_TEST( qdbCommon::makeFetchColumnList4TupleID(
                          aTemplate,
                          sTupleID,
                          sDataPlan->needTriggerRow,  // aIsNeedAllFetchColumn
                          NULL,             // index
                          ID_TRUE,          // allocSmiColumnListEx
                          & sFetchColumnList )
                      != IDE_SUCCESS );

            sCursorProperty.mFetchColumnList = sFetchColumnList;
        }
        else
        {
            /* Nothing to do */
        }

        IDE_TEST( sDataPlan->insertCursorMgr.openCursor(
                      aTemplate->stmt,
                      SMI_LOCK_WRITE | SMI_TRAVERSE_FORWARD | SMI_PREVIOUS_DISABLE,
                      & sCursorProperty )
                  != IDE_SUCCESS );

        *sDataPlan->flag &= ~QMND_UPTE_CURSOR_MASK;
        *sDataPlan->flag |= QMND_UPTE_CURSOR_OPEN;
    }
    else
    {
        // Nothing to do.
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;

#undef IDE_FN
}

IDE_RC
qmnUPTE::closeCursor( qcTemplate * aTemplate,
                      qmnPlan    * aPlan )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation :
 *
 ***********************************************************************/

#define IDE_FN "qmnUPTE::closeCursor"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY(""));

    qmndUPTE * sDataPlan =
        (qmndUPTE*) (aTemplate->tmplate.data + aPlan->offset);

    if ( ( *sDataPlan->flag & QMND_UPTE_CURSOR_MASK )
         == QMND_UPTE_CURSOR_OPEN )
    {
        *sDataPlan->flag &= ~QMND_UPTE_CURSOR_MASK;
        *sDataPlan->flag |= QMND_UPTE_CURSOR_CLOSED;

        IDE_TEST( sDataPlan->insertCursorMgr.closeCursor()
                  != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    if ( ( *sDataPlan->flag & QMND_UPTE_INDEX_CURSOR_MASK )
         == QMND_UPTE_INDEX_CURSOR_INITED )
    {
        IDE_TEST( qmsIndexTable::closeIndexTableCursors(
                      & (sDataPlan->indexTableCursorInfo) )
                  != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    if ( ( *sDataPlan->flag & QMND_UPTE_INDEX_CURSOR_MASK )
         == QMND_UPTE_INDEX_CURSOR_INITED )
    {
        qmsIndexTable::finalizeIndexTableCursors(
            & (sDataPlan->indexTableCursorInfo) );
    }

    return IDE_FAILURE;

#undef IDE_FN
}

IDE_RC
qmnUPTE::updateOneRow( qcTemplate * aTemplate,
                       qmnPlan    * aPlan )
{
/***********************************************************************
 *
 * Description :
 *    UPTE �� ���� ����� �����Ѵ�.
 *
 * Implementation :
 *    - update one record ����
 *    - trigger each row ����
 *
 ***********************************************************************/

#define IDE_FN "qmnUPTE::updateOneRow"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY(""));

    qmncUPTE * sCodePlan = (qmncUPTE*) aPlan;
    qmndUPTE * sDataPlan =
        (qmndUPTE*) (aTemplate->tmplate.data + aPlan->offset);

    void              * sOrgRow;

    smiValue          * sSmiValues;
    smiValue          * sSmiValuesForPartition = NULL;
    smiValue            sValuesForPartition[QC_MAX_COLUMN_COUNT];
    UInt                sSmiValuesValueCount   = 0;
    void              * sNewRow                = NULL;
    void              * sRow;

    // PROJ-1784 DML Without Retry
    smiValue            sWhereSmiValues[QC_MAX_COLUMN_COUNT];
    smiValue            sSetSmiValues[QC_MAX_COLUMN_COUNT];
    idBool              sIsDiskTableOrPartition = ID_FALSE;

    /* PROJ-2464 hybrid partitioned table ����
     * Memory�� ���, newRow�� ���ο� �ּҸ� �Ҵ��Ѵ�. ����, newRow�� ���� �������� �ʴ´�.
     */
    sNewRow = sDataPlan->newRow;

    /* BUG-39399 remove search key preserved table */
    if ( ( sCodePlan->flag & QMNC_UPTE_VIEW_MASK )
         == QMNC_UPTE_VIEW_TRUE )
    {
        IDE_TEST( checkDuplicateUpdate( sCodePlan,
                                        sDataPlan )
                  != IDE_SUCCESS );
    }
    else
    {
        /* Nothing To Do */
    }
        
    //-----------------------------------
    // clear lob
    //-----------------------------------

    // PROJ-1362
    if ( sDataPlan->lobInfo != NULL )
    {
        (void) qmx::clearLobInfo( sDataPlan->lobInfo );
    }
    else
    {
        // Nothing to do.
    }

    //-----------------------------------
    // copy old row
    //-----------------------------------

    if ( sDataPlan->needTriggerRow == ID_TRUE )
    {
        // OLD ROW REFERENCING�� ���� ����
        idlOS::memcpy( sDataPlan->oldRow,
                       sDataPlan->updateTuple->row,
                       sDataPlan->updateTuple->rowOffset );
    }
    else
    {
        // Nothing to do.
    }

    //-----------------------------------
    // set next sequence
    //-----------------------------------

    // Sequence Value ȹ��
    if ( sCodePlan->nextValSeqs != NULL )
    {
        IDE_TEST( qmx::readSequenceNextVals(
                      aTemplate->stmt,
                      sCodePlan->nextValSeqs )
                  != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    //-----------------------------------
    // make update smiValues
    //-----------------------------------

    sSmiValues = aTemplate->insOrUptRow[ sCodePlan->valueIdx ];
    sSmiValuesValueCount = aTemplate->insOrUptRowValueCount[sCodePlan->valueIdx];

    // subquery�� ������ smi value ���� ����
    IDE_TEST( qmx::makeSmiValueForSubquery( aTemplate,
                                            sCodePlan->tableRef->tableInfo,
                                            sCodePlan->columns,
                                            sCodePlan->subqueries,
                                            sCodePlan->canonizedTuple,
                                            sSmiValues,
                                            sCodePlan->isNull,
                                            sDataPlan->lobInfo )
              != IDE_SUCCESS );

    // ������ smi value ���� ����
    IDE_TEST( qmx::makeSmiValueForUpdate( aTemplate,
                                          sCodePlan->tableRef->tableInfo,
                                          sCodePlan->columns,
                                          sCodePlan->values,
                                          sCodePlan->canonizedTuple,
                                          sSmiValues,
                                          sCodePlan->isNull,
                                          sDataPlan->lobInfo )
              != IDE_SUCCESS );

    //-----------------------------------
    // Default Expr
    //-----------------------------------

    if ( sCodePlan->defaultExprColumns != NULL )
    {
        qmsDefaultExpr::setRowBufferFromBaseColumn(
            &(aTemplate->tmplate),
            sCodePlan->tableRef->table,
            sCodePlan->defaultExprTableRef->table,
            sCodePlan->defaultExprBaseColumns,
            sDataPlan->defaultExprRowBuffer );

        IDE_TEST( qmsDefaultExpr::setRowBufferFromSmiValueArray(
                      &(aTemplate->tmplate),
                      sCodePlan->defaultExprTableRef,
                      sCodePlan->columns,
                      sDataPlan->defaultExprRowBuffer,
                      sSmiValues,
                      QCM_TABLE_TYPE_IS_DISK( sCodePlan->tableRef->tableInfo->tableFlag ) )
                  != IDE_SUCCESS );

        IDE_TEST( qmsDefaultExpr::calculateDefaultExpression(
                      aTemplate,
                      sCodePlan->defaultExprTableRef,
                      sCodePlan->columns,
                      sCodePlan->defaultExprColumns,
                      sDataPlan->defaultExprRowBuffer,
                      sSmiValues,
                      sCodePlan->tableRef->tableInfo->columns )
                  != IDE_SUCCESS );
    }
    else
    {
        /* Nothing to do */
    }

    //-----------------------------------
    // PROJ-2334 PMT
    // set update trigger memory variable column info
    //-----------------------------------
    if ( ( sDataPlan->existTrigger == ID_TRUE ) &&
         ( sCodePlan->tableRef->tableInfo->tablePartitionType == QCM_PARTITIONED_TABLE ) )
    {
        sDataPlan->columnsForRow = sDataPlan->updatePartInfo->columns;
    }
    else
    {
        // Nothing To Do
    }

    // PROJ-2219 Row-level before update trigger
    if ( sDataPlan->existTrigger == ID_TRUE )
    {
        // ���� trigger���� lob column�� �ִ� table�� �������� �����Ƿ�
        // Table cursor�� NULL�� �ѱ��.
        IDE_TEST( qdnTrigger::fireTrigger(
                      aTemplate->stmt,
                      aTemplate->stmt->qmxMem,
                      sCodePlan->tableRef->tableInfo,
                      QCM_TRIGGER_ACTION_EACH_ROW,
                      QCM_TRIGGER_BEFORE,
                      QCM_TRIGGER_EVENT_UPDATE,
                      sCodePlan->updateColumnList, // UPDATE Column
                      NULL,                        // Table Cursor
                      SC_NULL_GRID,                // Row GRID
                      sDataPlan->oldRow,           // OLD ROW
                      sDataPlan->columnsForRow,    // OLD ROW Column
                      sSmiValues,                  // NEW ROW(value list)
                      sCodePlan->columns )         // NEW ROW Column
                  != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    //-----------------------------------
    // update one row
    //-----------------------------------

    if ( sDataPlan->retryInfo.mIsWithoutRetry == ID_TRUE )
    {
        if ( sCodePlan->tableRef->tableInfo->tablePartitionType == QCM_PARTITIONED_TABLE )
        {
            sIsDiskTableOrPartition = QCM_TABLE_TYPE_IS_DISK( sDataPlan->updatePartInfo->tableFlag );
        }
        else
        {
            sIsDiskTableOrPartition = QCM_TABLE_TYPE_IS_DISK( sCodePlan->tableRef->tableInfo->tableFlag );
        }

        if ( sIsDiskTableOrPartition == ID_TRUE )
        {
            IDE_TEST( qmx::setChkSmiValueList( sDataPlan->updateTuple->row,
                                               sDataPlan->retryInfo.mStmtRetryColLst,
                                               sWhereSmiValues,
                                               & (sDataPlan->retryInfo.mStmtRetryValLst) )
                      != IDE_SUCCESS );

            IDE_TEST( qmx::setChkSmiValueList( sDataPlan->updateTuple->row,
                                               sDataPlan->retryInfo.mRowRetryColLst,
                                               sSetSmiValues,
                                               & (sDataPlan->retryInfo.mRowRetryValLst) )
                      != IDE_SUCCESS );
        }
        else
        {
            /* Nothing to do */
        }
    }
    else
    {
        // Nothing to do.
    }

    // PROJ-2264 Dictionary table
    if( sCodePlan->compressedTuple != UINT_MAX )
    {
        IDE_TEST( qmx::makeSmiValueForCompress( aTemplate,
                                                sCodePlan->compressedTuple,
                                                sSmiValues )
                  != IDE_SUCCESS );
    }

    sDataPlan->retryInfo.mIsRowRetry = ID_FALSE;

    if ( QCM_TABLE_TYPE_IS_DISK( sCodePlan->tableRef->tableInfo->tableFlag ) !=
         QCM_TABLE_TYPE_IS_DISK( sDataPlan->updatePartInfo->tableFlag ) )
    {
        /* PROJ-2464 hybrid partitioned table ����
         * Partitioned Table�� �������� ���� smiValue Array�� Table Partition�� �°� ��ȯ�Ѵ�.
         */
        IDE_TEST( qmx::makeSmiValueWithSmiValue( sCodePlan->tableRef->tableInfo,
                                                 sDataPlan->updatePartInfo,
                                                 sCodePlan->columns,
                                                 sSmiValuesValueCount,
                                                 sSmiValues,
                                                 sValuesForPartition )
                  != IDE_SUCCESS );

        sSmiValuesForPartition = sValuesForPartition;
    }
    else
    {
        sSmiValuesForPartition = sSmiValues;
    }

    while( sDataPlan->updateCursor->updateRow( sSmiValuesForPartition,
                                               &( sDataPlan->retryInfo ),
                                               & sRow,
                                               & sDataPlan->rowGRID )
           != IDE_SUCCESS )
    {
        IDE_TEST( ideGetErrorCode() != smERR_RETRY_Row_Retry );

        IDE_TEST( sDataPlan->updateCursor->getLastRow(
                      (const void**) &(sDataPlan->updateTuple->row),
                      & sDataPlan->updateTuple->rid )
                  != IDE_SUCCESS );

        if ( sDataPlan->needTriggerRow == ID_TRUE )
        {
            // OLD ROW REFERENCING�� ���� ����
            idlOS::memcpy( sDataPlan->oldRow,
                           sDataPlan->updateTuple->row,
                           sDataPlan->updateTuple->rowOffset );
        }
        else
        {
            // Nothing to do.
        }

        // PROJ-1362
        if ( sDataPlan->lobInfo != NULL )
        {
            (void) qmx::clearLobInfo( sDataPlan->lobInfo );
        }
        else
        {
            // Nothing to do.
        }

        // ������ smi value ���� ����
        IDE_TEST( qmx::makeSmiValueForUpdate( aTemplate,
                                              sCodePlan->tableRef->tableInfo,
                                              sCodePlan->columns,
                                              sCodePlan->values,
                                              sCodePlan->canonizedTuple,
                                              sSmiValues,
                                              sCodePlan->isNull,
                                              sDataPlan->lobInfo )
                  != IDE_SUCCESS );

        //-----------------------------------
        // Default Expr
        //-----------------------------------

        if ( sCodePlan->defaultExprColumns != NULL )
        {
            qmsDefaultExpr::setRowBufferFromBaseColumn(
                &(aTemplate->tmplate),
                sCodePlan->tableRef->table,
                sCodePlan->defaultExprTableRef->table,
                sCodePlan->defaultExprBaseColumns,
                sDataPlan->defaultExprRowBuffer );

            IDE_TEST( qmsDefaultExpr::setRowBufferFromSmiValueArray(
                          &(aTemplate->tmplate),
                          sCodePlan->defaultExprTableRef,
                          sCodePlan->columns,
                          sDataPlan->defaultExprRowBuffer,
                          sSmiValues,
                          QCM_TABLE_TYPE_IS_DISK( sCodePlan->tableRef->tableInfo->tableFlag ) )
                      != IDE_SUCCESS );

            IDE_TEST( qmsDefaultExpr::calculateDefaultExpression(
                          aTemplate,
                          sCodePlan->defaultExprTableRef,
                          sCodePlan->columns,
                          sCodePlan->defaultExprColumns,
                          sDataPlan->defaultExprRowBuffer,
                          sSmiValues,
                          sCodePlan->tableRef->tableInfo->columns )
                      != IDE_SUCCESS );
        }
        else
        {
            /* Nothing to do */
        }

        if ( sIsDiskTableOrPartition == ID_TRUE )
        {
            IDE_TEST( qmx::setChkSmiValueList( sDataPlan->updateTuple->row,
                                               sDataPlan->retryInfo.mRowRetryColLst,
                                               sSetSmiValues,
                                               & (sDataPlan->retryInfo.mRowRetryValLst) )
                      != IDE_SUCCESS );
        }
        else
        {
            // Nothing to do.
        }

        sDataPlan->retryInfo.mIsRowRetry = ID_TRUE;

        if ( QCM_TABLE_TYPE_IS_DISK( sCodePlan->tableRef->tableInfo->tableFlag ) !=
             QCM_TABLE_TYPE_IS_DISK( sDataPlan->updatePartInfo->tableFlag ) )
        {
            /* PROJ-2464 hybrid partitioned table ����
             * Partitioned Table�� �������� ���� smiValue Array�� Table Partition�� �°� ��ȯ�Ѵ�.
             */
            IDE_TEST( qmx::makeSmiValueWithSmiValue( sCodePlan->tableRef->tableInfo,
                                                     sDataPlan->updatePartInfo,
                                                     sCodePlan->columns,
                                                     sSmiValuesValueCount,
                                                     sSmiValues,
                                                     sValuesForPartition )
                      != IDE_SUCCESS );

            sSmiValuesForPartition = sValuesForPartition;
        }
        else
        {
            sSmiValuesForPartition = sSmiValues;
        }
    }

    // update index table
    IDE_TEST( updateIndexTableCursor( aTemplate,
                                      sCodePlan,
                                      sDataPlan,
                                      sSmiValues )
              != IDE_SUCCESS );

    /* PROJ-2464 hybrid partitioned table ����
     *  Disk Partition�� ���, Disk Type�� Lob Column�� �ʿ��ϴ�.
     *  Memory/Volatile Partition�� ���, �ش� Partition�� Lob Column�� �ʿ��ϴ�.
     */
    if ( ( QCM_TABLE_TYPE_IS_DISK( sCodePlan->tableRef->tableInfo->tableFlag ) !=
           QCM_TABLE_TYPE_IS_DISK( sDataPlan->updatePartInfo->tableFlag ) ) ||
         ( ( sCodePlan->tableRef->tableInfo->tablePartitionType == QCM_PARTITIONED_TABLE ) &&
           ( QCM_TABLE_TYPE_IS_DISK( sDataPlan->updatePartInfo->tableFlag ) != ID_TRUE ) ) )
    {
        IDE_TEST( qmx::changeLobColumnInfo( sDataPlan->lobInfo,
                                            sDataPlan->updatePartInfo->columns )
                  != IDE_SUCCESS );
    }
    else
    {
        /* Nothing to do */
    }

    // Lob�÷� ó��
    IDE_TEST( qmx::copyAndOutBindLobInfo( aTemplate->stmt,
                                          sDataPlan->lobInfo,
                                          sDataPlan->updateCursor,
                                          sRow,
                                          sDataPlan->rowGRID )
              != IDE_SUCCESS );

    //-----------------------------------
    // check constraint
    //-----------------------------------

    if ( ( sDataPlan->needTriggerRow == ID_TRUE ) ||
         ( sCodePlan->checkConstrList != NULL ) ||
         ( sCodePlan->returnInto != NULL ) )
    {
        // NEW ROW�� ȹ��
        IDE_TEST( sDataPlan->updateCursor->getLastModifiedRow(
                      & sNewRow,
                      sDataPlan->updateTuple->rowOffset )
                  != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    /* PROJ-1107 Check Constraint ���� */
    if ( sCodePlan->checkConstrList != NULL )
    {
        sOrgRow = sDataPlan->updateTuple->row;
        sDataPlan->updateTuple->row = sNewRow;

        IDE_TEST( qdnCheck::verifyCheckConstraintList(
                      aTemplate,
                      sCodePlan->checkConstrList )
                  != IDE_SUCCESS );

        sDataPlan->updateTuple->row = sOrgRow;
    }
    else
    {
        // Nothing to do.
    }

    //-----------------------------------
    // update after trigger
    //-----------------------------------

    if ( sDataPlan->existTrigger == ID_TRUE )
    {
        // PROJ-1359 Trigger
        // ROW GRANULARITY TRIGGER�� ����
        IDE_TEST( qdnTrigger::fireTrigger( aTemplate->stmt,
                                           aTemplate->stmt->qmxMem,
                                           sCodePlan->tableRef->tableInfo,
                                           QCM_TRIGGER_ACTION_EACH_ROW,
                                           QCM_TRIGGER_AFTER,
                                           QCM_TRIGGER_EVENT_UPDATE,
                                           sCodePlan->updateColumnList,
                                           sDataPlan->updateCursor,         /* Table Cursor */
                                           sDataPlan->updateTuple->rid,     /* Row GRID */
                                           sDataPlan->oldRow,
                                           sDataPlan->columnsForRow,
                                           sNewRow,
                                           sDataPlan->columnsForRow )
                  != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    //-----------------------------------
    // return into
    //-----------------------------------

    /* PROJ-1584 DML Return Clause */
    if ( sCodePlan->returnInto != NULL )
    {
        sOrgRow = sDataPlan->updateTuple->row;
        sDataPlan->updateTuple->row = sNewRow;

        IDE_TEST( qmx::copyReturnToInto( aTemplate,
                                         sCodePlan->returnInto,
                                         aTemplate->numRows )
                  != IDE_SUCCESS );

        sDataPlan->updateTuple->row = sOrgRow;
    }
    else
    {
        // nothing do do
    }

    if ( ( *sDataPlan->flag & QMND_UPTE_UPDATE_MASK )
         == QMND_UPTE_UPDATE_FALSE )
    {
        *sDataPlan->flag &= ~QMND_UPTE_UPDATE_MASK;
        *sDataPlan->flag |= QMND_UPTE_UPDATE_TRUE;
    }
    else
    {
        // Nothing to do.
    }
    
    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;

#undef IDE_FN
}

IDE_RC
qmnUPTE::updateOneRowForRowmovement( qcTemplate * aTemplate,
                                     qmnPlan    * aPlan )
{
/***********************************************************************
 *
 * Description :
 *    UPTE �� ���� ����� �����Ѵ�.
 *
 * Implementation :
 *    - update one record ����
 *    - trigger each row ����
 *
 ***********************************************************************/

#define IDE_FN "qmnUPTE::updateOneRowForRowmovement"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY(""));

    qmncUPTE * sCodePlan = (qmncUPTE*) aPlan;
    qmndUPTE * sDataPlan =
        (qmndUPTE*) (aTemplate->tmplate.data + aPlan->offset);

    smiTableCursor    * sInsertCursor = NULL;
    void              * sOrgRow;

    UShort              sPartitionTupleID       = 0;
    mtcTuple          * sSelectedPartitionTuple = NULL;
    mtcTuple            sCopyTuple;
    idBool              sNeedToRecoverTuple     = ID_FALSE;

    qmsPartitionRef   * sSelectedPartitionRef;
    qcmTableInfo      * sSelectedPartitionInfo = NULL;
    qmnCursorInfo     * sCursorInfo;
    smiTableCursor    * sCursor                = NULL;
    smiValue          * sSmiValues;
    smiValue          * sSmiValuesForPartition = NULL;
    smiValue            sValuesForPartition[QC_MAX_COLUMN_COUNT];
    UInt                sSmiValuesValueCount   = 0;
    void              * sNewRow                = NULL;
    void              * sRow                   = NULL;
    smOID               sPartOID;
    qcmColumn         * sColumnsForNewRow      = NULL;
    
    /* PROJ-2464 hybrid partitioned table ����
     * Memory�� ���, newRow�� ���ο� �ּҸ� �Ҵ��Ѵ�. ����, newRow�� ���� �������� �ʴ´�.
     */
    sNewRow = sDataPlan->newRow;

    /* BUG-39399 remove search key preserved table */
    if ( ( sCodePlan->flag & QMNC_UPTE_VIEW_MASK )
         == QMNC_UPTE_VIEW_TRUE )
    {
        IDE_TEST( checkDuplicateUpdate( sCodePlan,
                                        sDataPlan )
                  != IDE_SUCCESS );
    }
    else
    {
        /* Nothing To Do */
    }
    
    //-----------------------------------
    // clear lob
    //-----------------------------------

    // PROJ-1362
    if ( sDataPlan->lobInfo != NULL )
    {
        (void) qmx::clearLobInfo( sDataPlan->lobInfo );
    }
    else
    {
        // Nothing to do.
    }

    // PROJ-1362
    if ( sDataPlan->insertLobInfo != NULL )
    {
        (void) qmx::initLobInfo( sDataPlan->insertLobInfo );
    }
    else
    {
        // Nothing to do.
    }

    //-----------------------------------
    // copy old row
    //-----------------------------------

    if ( sDataPlan->needTriggerRow == ID_TRUE )
    {
        // OLD ROW REFERENCING�� ���� ����
        idlOS::memcpy( sDataPlan->oldRow,
                       sDataPlan->updateTuple->row,
                       sDataPlan->updateTuple->rowOffset );
    }
    else
    {
        // Nothing to do.
    }

    //-----------------------------------
    // set next sequence
    //-----------------------------------

    // Sequence Value ȹ��
    if ( sCodePlan->nextValSeqs != NULL )
    {
        IDE_TEST( qmx::readSequenceNextVals(
                      aTemplate->stmt,
                      sCodePlan->nextValSeqs )
                  != IDE_SUCCESS );
    }

    //-----------------------------------
    // make update smiValues
    //-----------------------------------

    sSmiValues = aTemplate->insOrUptRow[ sCodePlan->valueIdx ];
    sSmiValuesValueCount = aTemplate->insOrUptRowValueCount[sCodePlan->valueIdx];

    // subquery�� ������ smi value ���� ����
    IDE_TEST( qmx::makeSmiValueForSubquery( aTemplate,
                                            sCodePlan->tableRef->tableInfo,
                                            sCodePlan->columns,
                                            sCodePlan->subqueries,
                                            sCodePlan->canonizedTuple,
                                            sSmiValues,
                                            sCodePlan->isNull,
                                            sDataPlan->lobInfo )
              != IDE_SUCCESS );

    // ������ smi value ���� ����
    IDE_TEST( qmx::makeSmiValueForUpdate( aTemplate,
                                          sCodePlan->tableRef->tableInfo,
                                          sCodePlan->columns,
                                          sCodePlan->values,
                                          sCodePlan->canonizedTuple,
                                          sSmiValues,
                                          sCodePlan->isNull,
                                          sDataPlan->lobInfo )
              != IDE_SUCCESS );

    /* PROJ-1090 Function-based Index */
    if ( sCodePlan->defaultExprColumns != NULL )
    {
        qmsDefaultExpr::setRowBufferFromBaseColumn(
            &(aTemplate->tmplate),
            sCodePlan->tableRef->table,
            sCodePlan->defaultExprTableRef->table,
            sCodePlan->defaultExprBaseColumns,
            sDataPlan->defaultExprRowBuffer );

        IDE_TEST( qmsDefaultExpr::setRowBufferFromSmiValueArray(
                      &(aTemplate->tmplate),
                      sCodePlan->defaultExprTableRef,
                      sCodePlan->columns,
                      sDataPlan->defaultExprRowBuffer,
                      sSmiValues,
                      QCM_TABLE_TYPE_IS_DISK( sCodePlan->tableRef->tableInfo->tableFlag ) )
                  != IDE_SUCCESS );

        IDE_TEST( qmsDefaultExpr::calculateDefaultExpression(
                      aTemplate,
                      sCodePlan->defaultExprTableRef,
                      sCodePlan->columns,
                      sCodePlan->defaultExprColumns,
                      sDataPlan->defaultExprRowBuffer,
                      sSmiValues,
                      sCodePlan->tableRef->tableInfo->columns )
                  != IDE_SUCCESS );
    }
    else
    {
        /* Nothing to do */
    }

    // PROJ-2219 Row-level before update trigger
    if ( sDataPlan->existTrigger == ID_TRUE )
    {
        // ���� trigger���� lob column�� �ִ� table�� �������� �����Ƿ�
        // Table cursor�� NULL�� �ѱ��.
        IDE_TEST( qdnTrigger::fireTrigger(
                      aTemplate->stmt,
                      aTemplate->stmt->qmxMem,
                      sCodePlan->tableRef->tableInfo,
                      QCM_TRIGGER_ACTION_EACH_ROW,
                      QCM_TRIGGER_BEFORE,
                      QCM_TRIGGER_EVENT_UPDATE,
                      sCodePlan->updateColumnList,        // UPDATE Column
                      NULL,                               // Table Cursor
                      SC_NULL_GRID,                       // Row GRID
                      sDataPlan->oldRow,                  // OLD ROW
                      sDataPlan->updatePartInfo->columns, // OLD ROW Column
                      sSmiValues,                         // NEW ROW(value list)
                      sCodePlan->columns )                // NEW ROW Column
                  != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    // row movement�� smi value ���� ����
    IDE_TEST( qmx::makeSmiValueForRowMovement(
                  sCodePlan->tableRef->tableInfo,
                  sCodePlan->updateColumnList,
                  sSmiValues,
                  sDataPlan->updateTuple,
                  sDataPlan->updateCursor,
                  sDataPlan->lobInfo,
                  sDataPlan->insertValues,
                  sDataPlan->insertLobInfo )
              != IDE_SUCCESS );

    //-----------------------------------
    // update one row
    //-----------------------------------

    if ( qmoPartition::partitionFilteringWithRow(
             sCodePlan->tableRef,
             sDataPlan->insertValues,
             &sSelectedPartitionRef )
         != IDE_SUCCESS )
    {
        IDE_CLEAR();

        //-----------------------------------
        // tableRef�� ���� partition�� ���
        // insert row -> update row
        //-----------------------------------

        /* PROJ-1090 Function-based Index */
        if ( sCodePlan->defaultExprColumns != NULL )
        {
            IDE_TEST( qmsDefaultExpr::setRowBufferFromSmiValueArray(
                          &(aTemplate->tmplate),
                          sCodePlan->defaultExprTableRef,
                          sCodePlan->tableRef->tableInfo->columns,
                          sDataPlan->defaultExprRowBuffer,
                          sDataPlan->insertValues,
                          QCM_TABLE_TYPE_IS_DISK( sCodePlan->tableRef->tableInfo->tableFlag ) )
                      != IDE_SUCCESS );

            IDE_TEST( qmsDefaultExpr::calculateDefaultExpression(
                          aTemplate,
                          sCodePlan->defaultExprTableRef,
                          NULL,
                          sCodePlan->defaultExprColumns,
                          sDataPlan->defaultExprRowBuffer,
                          sDataPlan->insertValues,
                          sCodePlan->tableRef->tableInfo->columns )
                      != IDE_SUCCESS );
        }
        else
        {
            /* Nothing to do */
        }

        IDE_TEST( sDataPlan->insertCursorMgr.partitionFilteringWithRow(
                      sDataPlan->insertValues,
                      sDataPlan->insertLobInfo,
                      &sSelectedPartitionInfo )
                  != IDE_SUCCESS );

        /* PROJ-2359 Table/Partition Access Option */
        IDE_TEST( qmx::checkAccessOption( sSelectedPartitionInfo,
                                          ID_TRUE /* aIsInsertion */ )
                  != IDE_SUCCESS );

        // insert row
        IDE_TEST( sDataPlan->insertCursorMgr.getCursor( &sCursor )
                  != IDE_SUCCESS );

        if ( QCM_TABLE_TYPE_IS_DISK( sCodePlan->tableRef->tableInfo->tableFlag ) !=
             QCM_TABLE_TYPE_IS_DISK( sSelectedPartitionInfo->tableFlag ) )
        {
            /* PROJ-2464 hybrid partitioned table ����
             * Partitioned Table�� �������� ���� smiValue Array�� Table Partition�� �°� ��ȯ�Ѵ�.
             */
            IDE_TEST( qmx::makeSmiValueWithSmiValue( sCodePlan->tableRef->tableInfo,
                                                     sSelectedPartitionInfo,
                                                     sCodePlan->tableRef->tableInfo->columns,
                                                     sCodePlan->tableRef->tableInfo->columnCount,
                                                     sDataPlan->insertValues,
                                                     sValuesForPartition )
                      != IDE_SUCCESS );

            sSmiValuesForPartition = sValuesForPartition;
        }
        else
        {
            sSmiValuesForPartition = sDataPlan->insertValues;
        }

        IDE_TEST( sCursor->insertRow( sSmiValuesForPartition,
                                      & sRow,
                                      & sDataPlan->rowGRID )
                  != IDE_SUCCESS );

        IDE_TEST( sDataPlan->insertCursorMgr.getSelectedPartitionOID(
                      & sPartOID )
                  != IDE_SUCCESS );

        // update index table
        IDE_TEST( updateIndexTableCursorForRowMovement( aTemplate,
                                                        sCodePlan,
                                                        sDataPlan,
                                                        sPartOID,
                                                        sDataPlan->rowGRID,
                                                        sSmiValues )
                  != IDE_SUCCESS );

        IDE_TEST( qmx::copyAndOutBindLobInfo( aTemplate->stmt,
                                              sDataPlan->insertLobInfo,
                                              sCursor,
                                              sRow,
                                              sDataPlan->rowGRID )
                  != IDE_SUCCESS );

        // delete row
        IDE_TEST( sDataPlan->updateCursor->deleteRow()
                  != IDE_SUCCESS );

        /* PROJ-2464 hybrid partitioned table ���� */
        IDE_TEST( sDataPlan->insertCursorMgr.getSelectedPartitionTupleID( &sPartitionTupleID )
                  != IDE_SUCCESS );
        sSelectedPartitionTuple = &(aTemplate->tmplate.rows[sPartitionTupleID]);

        if ( ( sDataPlan->needTriggerRow == ID_TRUE ) ||
             ( sCodePlan->checkConstrList != NULL ) ||
             ( sCodePlan->returnInto != NULL ) )
        {
            // NEW ROW�� ȹ��
            IDE_TEST( sCursor->getLastModifiedRow(
                          & sNewRow,
                          sSelectedPartitionTuple->rowOffset )
                      != IDE_SUCCESS );

            IDE_TEST( sDataPlan->insertCursorMgr.setColumnsForNewRow()
                      != IDE_SUCCESS );

            IDE_TEST( sDataPlan->insertCursorMgr.getColumnsForNewRow(
                          &sColumnsForNewRow )
                      != IDE_SUCCESS );
        }
        else
        {
            // Nothing To Do
        }
    }
    else
    {
        /* PROJ-2464 hybrid partitioned table ���� */
        sSelectedPartitionTuple = &(aTemplate->tmplate.rows[sSelectedPartitionRef->table]);

        if ( sSelectedPartitionRef->table == sDataPlan->updateTupleID )
        {
            //-----------------------------------
            // tableRef�� �ְ� select�� partition�� ���
            //-----------------------------------

            if ( QCM_TABLE_TYPE_IS_DISK( sCodePlan->tableRef->tableInfo->tableFlag ) !=
                 QCM_TABLE_TYPE_IS_DISK( sDataPlan->updatePartInfo->tableFlag ) )
            {
                /* PROJ-2464 hybrid partitioned table ����
                 * Partitioned Table�� �������� ���� smiValue Array�� Table Partition�� �°� ��ȯ�Ѵ�.
                 */
                IDE_TEST( qmx::makeSmiValueWithSmiValue( sCodePlan->tableRef->tableInfo,
                                                         sDataPlan->updatePartInfo,
                                                         sCodePlan->columns,
                                                         sSmiValuesValueCount,
                                                         sSmiValues,
                                                         sValuesForPartition )
                          != IDE_SUCCESS );

                sSmiValuesForPartition = sValuesForPartition;
            }
            else
            {
                sSmiValuesForPartition = sSmiValues;
            }

            IDE_TEST( sDataPlan->updateCursor->updateRow( sSmiValuesForPartition,
                                                          NULL,
                                                          & sRow,
                                                          & sDataPlan->rowGRID )
                      != IDE_SUCCESS );

            sPartOID = sSelectedPartitionRef->partitionOID;

            // update index table
            IDE_TEST( updateIndexTableCursorForRowMovement( aTemplate,
                                                            sCodePlan,
                                                            sDataPlan,
                                                            sPartOID,
                                                            sDataPlan->rowGRID,
                                                            sSmiValues )
                      != IDE_SUCCESS );

            /* PROJ-2464 hybrid partitioned table ����
             *  Disk Partition�� ���, Disk Type�� Lob Column�� �ʿ��ϴ�.
             *  Memory/Volatile Partition�� ���, �ش� Partition�� Lob Column�� �ʿ��ϴ�.
             */
            if ( ( QCM_TABLE_TYPE_IS_DISK( sCodePlan->tableRef->tableInfo->tableFlag ) !=
                   QCM_TABLE_TYPE_IS_DISK( sDataPlan->updatePartInfo->tableFlag ) ) ||
                 ( QCM_TABLE_TYPE_IS_DISK( sDataPlan->updatePartInfo->tableFlag ) != ID_TRUE ) )
            {
                IDE_TEST( qmx::changeLobColumnInfo( sDataPlan->lobInfo,
                                                    sDataPlan->updatePartInfo->columns )
                          != IDE_SUCCESS );
            }
            else
            {
                /* Nothing to do */
            }

            // Lob�÷� ó��
            IDE_TEST( qmx::copyAndOutBindLobInfo( aTemplate->stmt,
                                                  sDataPlan->lobInfo,
                                                  sDataPlan->updateCursor,
                                                  sRow,
                                                  sDataPlan->rowGRID )
                      != IDE_SUCCESS );

            if ( ( sDataPlan->needTriggerRow == ID_TRUE ) ||
                 ( sCodePlan->checkConstrList != NULL ) ||
                 ( sCodePlan->returnInto != NULL ) )
            {
                // NEW ROW�� ȹ��
                IDE_TEST( sDataPlan->updateCursor->getLastModifiedRow(
                              & sNewRow,
                              sSelectedPartitionTuple->rowOffset )
                          != IDE_SUCCESS );

                sColumnsForNewRow = sSelectedPartitionRef->partitionInfo->columns;
            }
            else
            {
                // Nothing to do.
            }
        }
        else
        {
            //-----------------------------------
            // tableRef�� �ִ� �ٸ� partition�� ���
            // insert row -> update row
            //-----------------------------------

            /* PROJ-2359 Table/Partition Access Option */
            IDE_TEST( qmx::checkAccessOption( sSelectedPartitionRef->partitionInfo,
                                              ID_TRUE /* aIsInsertion */ )
                      != IDE_SUCCESS );

            /* PROJ-1090 Function-based Index */
            if ( sCodePlan->defaultExprColumns != NULL )
            {
                IDE_TEST( qmsDefaultExpr::setRowBufferFromSmiValueArray(
                              &(aTemplate->tmplate),
                              sCodePlan->defaultExprTableRef,
                              sCodePlan->tableRef->tableInfo->columns,
                              sDataPlan->defaultExprRowBuffer,
                              sDataPlan->insertValues,
                              QCM_TABLE_TYPE_IS_DISK( sCodePlan->tableRef->tableInfo->tableFlag ) )
                          != IDE_SUCCESS );

                IDE_TEST( qmsDefaultExpr::calculateDefaultExpression(
                              aTemplate,
                              sCodePlan->defaultExprTableRef,
                              NULL,
                              sCodePlan->defaultExprColumns,
                              sDataPlan->defaultExprRowBuffer,
                              sDataPlan->insertValues,
                              sCodePlan->tableRef->tableInfo->columns )
                          != IDE_SUCCESS );
            }
            else
            {
                /* Nothing to do */
            }

            // partition�� cursor�� ��´�.
            sCursorInfo = (qmnCursorInfo*)
                aTemplate->tmplate.rows[sSelectedPartitionRef->table].cursorInfo;

            IDE_TEST_RAISE( sCursorInfo == NULL, ERR_NOT_FOUND );

            // insert row
            sInsertCursor = sCursorInfo->cursor;

            if ( QCM_TABLE_TYPE_IS_DISK( sCodePlan->tableRef->tableInfo->tableFlag ) !=
                 QCM_TABLE_TYPE_IS_DISK( sSelectedPartitionRef->partitionInfo->tableFlag ) )
            {
                /* PROJ-2464 hybrid partitioned table ����
                 * Partitioned Table�� �������� ���� smiValue Array�� Table Partition�� �°� ��ȯ�Ѵ�.
                 */
                IDE_TEST( qmx::makeSmiValueWithSmiValue( sCodePlan->tableRef->tableInfo,
                                                         sSelectedPartitionRef->partitionInfo,
                                                         sCodePlan->tableRef->tableInfo->columns,
                                                         sCodePlan->tableRef->tableInfo->columnCount,
                                                         sDataPlan->insertValues,
                                                         sValuesForPartition )
                          != IDE_SUCCESS );

                sSmiValuesForPartition = sValuesForPartition;
            }
            else
            {
                sSmiValuesForPartition = sDataPlan->insertValues;
            }

            IDE_TEST( sInsertCursor->insertRow( sSmiValuesForPartition,
                                                & sRow,
                                                & sDataPlan->rowGRID )
                      != IDE_SUCCESS );

            /* PROJ-2334 PMT
             * ���õ� ��Ƽ���� �÷����� LOB Column���� ���� */
            /* PROJ-2464 hybrid partitioned table ����
             *  Disk Partition�� ���, Disk Type�� Lob Column�� �ʿ��ϴ�.
             *  Memory/Volatile Partition�� ���, �ش� Partition�� Lob Column�� �ʿ��ϴ�.
             */
            if ( ( QCM_TABLE_TYPE_IS_DISK( sCodePlan->tableRef->tableInfo->tableFlag ) !=
                   QCM_TABLE_TYPE_IS_DISK( sSelectedPartitionRef->partitionInfo->tableFlag ) ) ||
                 ( QCM_TABLE_TYPE_IS_DISK( sSelectedPartitionRef->partitionInfo->tableFlag ) != ID_TRUE ) )
            {
                IDE_TEST( qmx::changeLobColumnInfo( sDataPlan->insertLobInfo,
                                                    sSelectedPartitionRef->partitionInfo->columns )
                          != IDE_SUCCESS );
            }
            else
            {
                /* Nothing to do */
            }

            sPartOID = sSelectedPartitionRef->partitionOID;

            // update index table
            IDE_TEST( updateIndexTableCursorForRowMovement( aTemplate,
                                                            sCodePlan,
                                                            sDataPlan,
                                                            sPartOID,
                                                            sDataPlan->rowGRID,
                                                            sSmiValues )
                      != IDE_SUCCESS );

            IDE_TEST( qmx::copyAndOutBindLobInfo( aTemplate->stmt,
                                                  sDataPlan->insertLobInfo,
                                                  sInsertCursor,
                                                  sRow,
                                                  sDataPlan->rowGRID )
                      != IDE_SUCCESS );
            
            // delete row
            IDE_TEST( sDataPlan->updateCursor->deleteRow()
                      != IDE_SUCCESS );

            if ( ( sDataPlan->needTriggerRow == ID_TRUE ) ||
                 ( sCodePlan->checkConstrList != NULL ) ||
                 ( sCodePlan->returnInto != NULL ) )
            {
                // NEW ROW�� ȹ��
                IDE_TEST( sInsertCursor->getModifiedRow(
                              & sNewRow,
                              sSelectedPartitionTuple->rowOffset,
                              sRow,
                              sDataPlan->rowGRID )
                          != IDE_SUCCESS );

                sColumnsForNewRow = sSelectedPartitionRef->partitionInfo->columns;
            }
            else
            {
                // Nothing to do.
            }
        }
    }

    /* PROJ-2464 hybrid partitioned table ���� */
    if ( ( sCodePlan->tableRef->partitionSummary->isHybridPartitionedTable == ID_TRUE ) ||
         ( sCodePlan->insertTableRef->partitionSummary->isHybridPartitionedTable == ID_TRUE ) )
    {
        qmx::copyMtcTupleForPartitionDML( &sCopyTuple, sDataPlan->updateTuple );
        sNeedToRecoverTuple = ID_TRUE;

        qmx::adjustMtcTupleForPartitionDML( sDataPlan->updateTuple, sSelectedPartitionTuple );
    }
    else
    {
        /* Nothing to do */
    }

    //-----------------------------------
    // check constraint
    //-----------------------------------

    /* PROJ-1107 Check Constraint ���� */
    if ( sCodePlan->checkConstrList != NULL )
    {
        sOrgRow = sDataPlan->updateTuple->row;
        sDataPlan->updateTuple->row = sNewRow;

        IDE_TEST( qdnCheck::verifyCheckConstraintList(
                      aTemplate,
                      sCodePlan->checkConstrList )
                  != IDE_SUCCESS );

        sDataPlan->updateTuple->row = sOrgRow;
    }
    else
    {
        // Nothing to do.
    }

    //-----------------------------------
    // update after trigger
    //-----------------------------------

    if ( sDataPlan->existTrigger == ID_TRUE )
    {
        // PROJ-1359 Trigger
        // ROW GRANULARITY TRIGGER�� ����
        IDE_TEST( qdnTrigger::fireTrigger( aTemplate->stmt,
                                           aTemplate->stmt->qmxMem,
                                           sCodePlan->tableRef->tableInfo,
                                           QCM_TRIGGER_ACTION_EACH_ROW,
                                           QCM_TRIGGER_AFTER,
                                           QCM_TRIGGER_EVENT_UPDATE,
                                           sCodePlan->updateColumnList,
                                           sDataPlan->updateCursor,         /* Table Cursor */
                                           sDataPlan->updateTuple->rid,     /* Row GRID */
                                           sDataPlan->oldRow,
                                           sDataPlan->updatePartInfo->columns,
                                           sNewRow,
                                           sColumnsForNewRow )
                  != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    //-----------------------------------
    // return into
    //-----------------------------------

    /* PROJ-1584 DML Return Clause */
    if ( sCodePlan->returnInto != NULL )
    {
        sOrgRow = sDataPlan->updateTuple->row;
        sDataPlan->updateTuple->row = sNewRow;

        IDE_TEST( qmx::copyReturnToInto( aTemplate,
                                         sCodePlan->returnInto,
                                         aTemplate->numRows )
                  != IDE_SUCCESS );

        sDataPlan->updateTuple->row = sOrgRow;
    }
    else
    {
        // nothing do do
    }

    if ( ( *sDataPlan->flag & QMND_UPTE_UPDATE_MASK )
         == QMND_UPTE_UPDATE_FALSE )
    {
        *sDataPlan->flag &= ~QMND_UPTE_UPDATE_MASK;
        *sDataPlan->flag |= QMND_UPTE_UPDATE_TRUE;
    }
    else
    {
        // Nothing to do.
    }

    /* PROJ-2464 hybrid partitioned table ���� */
    if ( sNeedToRecoverTuple == ID_TRUE )
    {
        sNeedToRecoverTuple = ID_FALSE;
        qmx::copyMtcTupleForPartitionDML( sDataPlan->updateTuple, &sCopyTuple );
    }
    else
    {
        /* Nothing to do */
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_NOT_FOUND )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmnUPTE::updateOneRowForRowmovement",
                                  "cursor not found" ));
    }
    IDE_EXCEPTION_END;

    /* PROJ-2464 hybrid partitioned table ���� */
    if ( sNeedToRecoverTuple == ID_TRUE )
    {
        qmx::copyMtcTupleForPartitionDML( sDataPlan->updateTuple, &sCopyTuple );
    }
    else
    {
        /* Nothing to do */
    }

    return IDE_FAILURE;

#undef IDE_FN
}

IDE_RC
qmnUPTE::updateOneRowForCheckRowmovement( qcTemplate * aTemplate,
                                          qmnPlan    * aPlan )
{
/***********************************************************************
 *
 * Description :
 *    UPTE �� ���� ����� �����Ѵ�.
 *
 * Implementation :
 *    - update one record ����
 *    - trigger each row ����
 *
 ***********************************************************************/

#define IDE_FN "qmnUPTE::updateOneRowForCheckRowmovement"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY(""));

    qmncUPTE * sCodePlan = (qmncUPTE*) aPlan;
    qmndUPTE * sDataPlan =
        (qmndUPTE*) (aTemplate->tmplate.data + aPlan->offset);

    void              * sOrgRow;
    qmsPartitionRef   * sSelectedPartitionRef;
    smiValue          * sSmiValues;
    smiValue          * sSmiValuesForPartition = NULL;
    smiValue            sValuesForPartition[QC_MAX_COLUMN_COUNT];
    UInt                sSmiValuesValueCount   = 0;
    void              * sNewRow                = NULL;
    void              * sRow;

    /* PROJ-2464 hybrid partitioned table ����
     * Memory�� ���, newRow�� ���ο� �ּҸ� �Ҵ��Ѵ�. ����, newRow�� ���� �������� �ʴ´�.
     */
    sNewRow = sDataPlan->newRow;

    /* BUG-39399 remove search key preserved table */
    if ( ( sCodePlan->flag & QMNC_UPTE_VIEW_MASK )
         == QMNC_UPTE_VIEW_TRUE )
    {
        IDE_TEST( checkDuplicateUpdate( sCodePlan,
                                        sDataPlan )
                  != IDE_SUCCESS );
    }
    else
    {
        /* Nothing To Do */
    }

    //-----------------------------------
    // clear lob
    //-----------------------------------

    // PROJ-1362
    if ( sDataPlan->lobInfo != NULL )
    {
        (void) qmx::clearLobInfo( sDataPlan->lobInfo );
    }
    else
    {
        // Nothing to do.
    }

    //-----------------------------------
    // copy old row
    //-----------------------------------

    if ( sDataPlan->needTriggerRow == ID_TRUE )
    {
        // OLD ROW REFERENCING�� ���� ����
        idlOS::memcpy( sDataPlan->oldRow,
                       sDataPlan->updateTuple->row,
                       sDataPlan->updateTuple->rowOffset );
    }
    else
    {
        // Nothing to do.
    }

    //-----------------------------------
    // set next sequence
    //-----------------------------------

    // Sequence Value ȹ��
    if ( sCodePlan->nextValSeqs != NULL )
    {
        IDE_TEST( qmx::readSequenceNextVals(
                      aTemplate->stmt,
                      sCodePlan->nextValSeqs )
                  != IDE_SUCCESS );
    }

    //-----------------------------------
    // make update smiValues
    //-----------------------------------

    sSmiValues = aTemplate->insOrUptRow[ sCodePlan->valueIdx ];
    sSmiValuesValueCount = aTemplate->insOrUptRowValueCount[sCodePlan->valueIdx];

    // subquery�� ������ smi value ���� ����
    IDE_TEST( qmx::makeSmiValueForSubquery( aTemplate,
                                            sCodePlan->tableRef->tableInfo,
                                            sCodePlan->columns,
                                            sCodePlan->subqueries,
                                            sCodePlan->canonizedTuple,
                                            sSmiValues,
                                            sCodePlan->isNull,
                                            sDataPlan->lobInfo )
              != IDE_SUCCESS );

    // ������ smi value ���� ����
    IDE_TEST( qmx::makeSmiValueForUpdate( aTemplate,
                                          sCodePlan->tableRef->tableInfo,
                                          sCodePlan->columns,
                                          sCodePlan->values,
                                          sCodePlan->canonizedTuple,
                                          sSmiValues,
                                          sCodePlan->isNull,
                                          sDataPlan->lobInfo )
              != IDE_SUCCESS );

    //-----------------------------------
    // Default Expr
    //-----------------------------------

    if ( sCodePlan->defaultExprColumns != NULL )
    {
        qmsDefaultExpr::setRowBufferFromBaseColumn(
            &(aTemplate->tmplate),
            sCodePlan->tableRef->table,
            sCodePlan->defaultExprTableRef->table,
            sCodePlan->defaultExprBaseColumns,
            sDataPlan->defaultExprRowBuffer );

        IDE_TEST( qmsDefaultExpr::setRowBufferFromSmiValueArray(
                      &(aTemplate->tmplate),
                      sCodePlan->defaultExprTableRef,
                      sCodePlan->columns,
                      sDataPlan->defaultExprRowBuffer,
                      sSmiValues,
                      QCM_TABLE_TYPE_IS_DISK( sCodePlan->tableRef->tableInfo->tableFlag ) )
                  != IDE_SUCCESS );

        IDE_TEST( qmsDefaultExpr::calculateDefaultExpression(
                      aTemplate,
                      sCodePlan->defaultExprTableRef,
                      sCodePlan->columns,
                      sCodePlan->defaultExprColumns,
                      sDataPlan->defaultExprRowBuffer,
                      sSmiValues,
                      sCodePlan->tableRef->tableInfo->columns )
                  != IDE_SUCCESS );
    }
    else
    {
        /* Nothing to do */
    }

    // PROJ-2219 Row-level before update trigger
    if ( sDataPlan->existTrigger == ID_TRUE )
    {
        // ���� trigger���� lob column�� �ִ� table�� �������� �����Ƿ�
        // Table cursor�� NULL�� �ѱ��.
        IDE_TEST( qdnTrigger::fireTrigger(
                      aTemplate->stmt,
                      aTemplate->stmt->qmxMem,
                      sCodePlan->tableRef->tableInfo,
                      QCM_TRIGGER_ACTION_EACH_ROW,
                      QCM_TRIGGER_BEFORE,
                      QCM_TRIGGER_EVENT_UPDATE,
                      sCodePlan->updateColumnList,        // UPDATE Column
                      NULL,                               // Table Cursor
                      SC_NULL_GRID,                       // Row GRID
                      sDataPlan->oldRow,                  // OLD ROW
                      sDataPlan->updatePartInfo->columns, // OLD ROW Column
                      sSmiValues,                         // NEW ROW(value list)
                      sCodePlan->columns )                // NEW ROW Column
                  != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    IDE_TEST( qmx::makeSmiValueForChkRowMovement(
                  sCodePlan->updateColumnList,
                  sSmiValues,
                  sCodePlan->tableRef->tableInfo->partKeyColumns,
                  sDataPlan->updateTuple,
                  sDataPlan->checkValues )
              != IDE_SUCCESS );

    IDE_TEST_RAISE( qmoPartition::partitionFilteringWithRow(
                        sCodePlan->tableRef,
                        sDataPlan->checkValues,
                        &sSelectedPartitionRef )
                    != IDE_SUCCESS,
                    ERR_NO_ROW_MOVEMENT );

    IDE_TEST_RAISE( sSelectedPartitionRef->table != sDataPlan->updateTupleID,
                    ERR_NO_ROW_MOVEMENT );

    //-----------------------------------
    // update one row
    //-----------------------------------

    if ( QCM_TABLE_TYPE_IS_DISK( sCodePlan->tableRef->tableInfo->tableFlag ) !=
         QCM_TABLE_TYPE_IS_DISK( sDataPlan->updatePartInfo->tableFlag ) )
    {
        /* PROJ-2464 hybrid partitioned table ����
         * Partitioned Table�� �������� ���� smiValue Array�� Table Partition�� �°� ��ȯ�Ѵ�.
         */
        IDE_TEST( qmx::makeSmiValueWithSmiValue( sCodePlan->tableRef->tableInfo,
                                                 sDataPlan->updatePartInfo,
                                                 sCodePlan->columns,
                                                 sSmiValuesValueCount,
                                                 sSmiValues,
                                                 sValuesForPartition )
                  != IDE_SUCCESS );

        sSmiValuesForPartition = sValuesForPartition;
    }
    else
    {
        sSmiValuesForPartition = sSmiValues;
    }

    IDE_TEST( sDataPlan->updateCursor->updateRow( sSmiValuesForPartition,
                                                  NULL,
                                                  & sRow,
                                                  & sDataPlan->rowGRID )
              != IDE_SUCCESS );

    // update index table
    IDE_TEST( updateIndexTableCursor( aTemplate,
                                      sCodePlan,
                                      sDataPlan,
                                      sSmiValues )
              != IDE_SUCCESS );

    /* PROJ-2464 hybrid partitioned table ����
     *  Disk Partition�� ���, Disk Type�� Lob Column�� �ʿ��ϴ�.
     *  Memory/Volatile Partition�� ���, �ش� Partition�� Lob Column�� �ʿ��ϴ�.
     */
    if ( ( QCM_TABLE_TYPE_IS_DISK( sCodePlan->tableRef->tableInfo->tableFlag ) !=
           QCM_TABLE_TYPE_IS_DISK( sDataPlan->updatePartInfo->tableFlag ) ) ||
         ( QCM_TABLE_TYPE_IS_DISK( sDataPlan->updatePartInfo->tableFlag ) != ID_TRUE ) )
    {
        IDE_TEST( qmx::changeLobColumnInfo( sDataPlan->lobInfo,
                                            sDataPlan->updatePartInfo->columns )
                  != IDE_SUCCESS );
    }
    else
    {
        /* Nothing to do */
    }

    // Lob�÷� ó��
    IDE_TEST( qmx::copyAndOutBindLobInfo( aTemplate->stmt,
                                          sDataPlan->lobInfo,
                                          sDataPlan->updateCursor,
                                          sRow,
                                          sDataPlan->rowGRID )
              != IDE_SUCCESS );

    if ( ( sDataPlan->needTriggerRow == ID_TRUE ) ||
         ( sCodePlan->checkConstrList != NULL ) ||
         ( sCodePlan->returnInto != NULL ) )
    {
        // NEW ROW�� ȹ��
        IDE_TEST( sDataPlan->updateCursor->getLastModifiedRow(
                      & sNewRow,
                      sDataPlan->updateTuple->rowOffset )
                  != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    //-----------------------------------
    // check constraint
    //-----------------------------------

    /* PROJ-1107 Check Constraint ���� */
    if ( sCodePlan->checkConstrList != NULL )
    {
        sOrgRow = sDataPlan->updateTuple->row;
        sDataPlan->updateTuple->row = sNewRow;

        IDE_TEST( qdnCheck::verifyCheckConstraintList(
                      aTemplate,
                      sCodePlan->checkConstrList )
                  != IDE_SUCCESS );

        sDataPlan->updateTuple->row = sOrgRow;
    }
    else
    {
        // Nothing to do.
    }

    //-----------------------------------
    // update after trigger
    //-----------------------------------

    if ( sDataPlan->existTrigger == ID_TRUE )
    {
        // PROJ-1359 Trigger
        // ROW GRANULARITY TRIGGER�� ����
        IDE_TEST( qdnTrigger::fireTrigger( aTemplate->stmt,
                                           aTemplate->stmt->qmxMem,
                                           sCodePlan->tableRef->tableInfo,
                                           QCM_TRIGGER_ACTION_EACH_ROW,
                                           QCM_TRIGGER_AFTER,
                                           QCM_TRIGGER_EVENT_UPDATE,
                                           sCodePlan->updateColumnList,
                                           sDataPlan->updateCursor,         /* Table Cursor */
                                           sDataPlan->updateTuple->rid,     /* Row GRID */
                                           sDataPlan->oldRow,
                                           sDataPlan->updatePartInfo->columns,
                                           sNewRow,
                                           sDataPlan->updatePartInfo->columns )
                  != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    //-----------------------------------
    // return into
    //-----------------------------------

    /* PROJ-1584 DML Return Clause */
    if ( sCodePlan->returnInto != NULL )
    {
        sOrgRow = sDataPlan->updateTuple->row;
        sDataPlan->updateTuple->row = sNewRow;

        IDE_TEST( qmx::copyReturnToInto( aTemplate,
                                         sCodePlan->returnInto,
                                         aTemplate->numRows )
                  != IDE_SUCCESS );

        sDataPlan->updateTuple->row = sOrgRow;
    }
    else
    {
        // nothing do do
    }

    if ( ( *sDataPlan->flag & QMND_UPTE_UPDATE_MASK )
         == QMND_UPTE_UPDATE_FALSE )
    {
        *sDataPlan->flag &= ~QMND_UPTE_UPDATE_MASK;
        *sDataPlan->flag |= QMND_UPTE_UPDATE_TRUE;
    }
    else
    {
        // Nothing to do.
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_NO_ROW_MOVEMENT )
    {
        IDE_SET( ideSetErrorCode(qpERR_ABORT_QMV_INVALID_PARTITION_KEY_INSERT) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;

#undef IDE_FN
}

IDE_RC
qmnUPTE::fireInsteadOfTrigger( qcTemplate * aTemplate,
                               qmnPlan    * aPlan )
{
/***********************************************************************
 *
 * Description :
 *    UPTE �� ���� ����� �����Ѵ�.
 *
 * Implementation :
 *    - trigger each row ����
 *
 ***********************************************************************/

#define IDE_FN "qmnUPTE::fireInsteadOfTrigger"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY(""));

    qmncUPTE * sCodePlan = (qmncUPTE*) aPlan;
    qmndUPTE * sDataPlan =
        (qmndUPTE*) (aTemplate->tmplate.data + aPlan->offset);

    qcmTableInfo * sTableInfo = NULL;
    qcmColumn    * sQcmColumn = NULL;
    mtcColumn    * sColumn    = NULL;
    smiValue     * sSmiValues = NULL;
    mtcStack     * sStack     = NULL;
    SInt           sRemain    = 0;
    void         * sOrgRow    = NULL;
    UShort         i          = 0;

    sTableInfo = sCodePlan->tableRef->tableInfo;

    if ( ( sDataPlan->needTriggerRow == ID_TRUE ) ||
         ( sCodePlan->returnInto != NULL ) )
    {
        //-----------------------------------
        // get Old Row
        //-----------------------------------

        sStack = aTemplate->tmplate.stack;
        sRemain = aTemplate->tmplate.stackRemain;

        IDE_TEST_RAISE( sRemain < sDataPlan->updateTuple->columnCount,
                        ERR_STACK_OVERFLOW );

        // UPDATE�� VIEW ���̿� FILT ���� �ٸ� ���鿡 ���� stack�� ����Ǿ��� �� �����Ƿ�
        // stack�� view tuple�� �÷����� �缳���Ѵ�.
        for ( i = 0, sColumn = sDataPlan->updateTuple->columns;
              i < sDataPlan->updateTuple->columnCount;
              i++, sColumn++, sStack++ )
        {
            sStack->column = sColumn;
            sStack->value  =
                (void*)((SChar*)sDataPlan->updateTuple->row + sColumn->column.offset);
        }

        /* PROJ-2464 hybrid partitioned table ���� */
        if ( sTableInfo->tablePartitionType == QCM_PARTITIONED_TABLE )
        {
            if ( sDataPlan->updatePartInfo != NULL )
            {
                if ( sDataPlan->updateTuple->tableHandle != sDataPlan->updatePartInfo->tableHandle )
                {
                    IDE_TEST( smiGetTableTempInfo( sDataPlan->updateTuple->tableHandle,
                                                   (void **)&(sDataPlan->updatePartInfo) )
                              != IDE_SUCCESS );
                }
                else
                {
                    /* Nothing to do */
                }
            }
            else
            {
                IDE_TEST( smiGetTableTempInfo( sDataPlan->updateTuple->tableHandle,
                                               (void **)&(sDataPlan->updatePartInfo) )
                          != IDE_SUCCESS );
            }

            // ��� Partition�� ��Ȯ�ϹǷ�, �۾� ������ Partition���� �����Ѵ�.
            sTableInfo = sDataPlan->updatePartInfo;
            sDataPlan->columnsForRow = sDataPlan->updatePartInfo->columns;
        }
        else
        {
            /* Nothing to do */
        }

        IDE_TEST( qmx::makeSmiValueWithStack( sDataPlan->columnsForRow,
                                              aTemplate,
                                              aTemplate->tmplate.stack,
                                              sTableInfo,
                                              (smiValue*) sDataPlan->oldRow,
                                              NULL )
                  != IDE_SUCCESS );

        //-----------------------------------
        // get New Row
        //-----------------------------------

        // Sequence Value ȹ��
        if ( sCodePlan->nextValSeqs != NULL )
        {
            IDE_TEST( qmx::readSequenceNextVals(
                          aTemplate->stmt,
                          sCodePlan->nextValSeqs )
                      != IDE_SUCCESS );
        }

        sSmiValues = aTemplate->insOrUptRow[ sCodePlan->valueIdx ];

        // subquery�� ������ smi value ���� ����
        IDE_TEST( qmx::makeSmiValueForSubquery( aTemplate,
                                                sTableInfo,
                                                sCodePlan->columns,
                                                sCodePlan->subqueries,
                                                sCodePlan->canonizedTuple,
                                                sSmiValues,
                                                sCodePlan->isNull,
                                                sDataPlan->lobInfo )
                  != IDE_SUCCESS );

        // ������ smi value ���� ����
        IDE_TEST( qmx::makeSmiValueForUpdate( aTemplate,
                                              sTableInfo,
                                              sCodePlan->columns,
                                              sCodePlan->values,
                                              sCodePlan->canonizedTuple,
                                              sSmiValues,
                                              sCodePlan->isNull,
                                              sDataPlan->lobInfo )
                  != IDE_SUCCESS );

        // old smiValues�� new smiValues�� ����
        idlOS::memcpy( sDataPlan->newRow,
                       sDataPlan->oldRow,
                       ID_SIZEOF(smiValue) * sDataPlan->updateTuple->columnCount );

        // update smiValues�� ������ ��ġ�� ����
        for ( sQcmColumn = sCodePlan->columns;
              sQcmColumn != NULL;
              sQcmColumn = sQcmColumn->next, sSmiValues++ )
        {
            for ( i = 0; i < sTableInfo->columnCount; i++ )
            {
                if ( sQcmColumn->basicInfo->column.id ==
                     sDataPlan->columnsForRow[i].basicInfo->column.id )
                {
                    *((smiValue*)sDataPlan->newRow + i) = *sSmiValues;
                    break;
                }
                else
                {
                    // Nothing to do.
                }
            }
        }
    }
    else
    {
        // Nothing to do.
    }

    if ( sDataPlan->existTrigger == ID_TRUE )
    {
        // PROJ-1359 Trigger
        // ROW GRANULARITY TRIGGER�� ����
        IDE_TEST( qdnTrigger::fireTrigger( aTemplate->stmt,
                                           aTemplate->stmt->qmxMem,
                                           sCodePlan->tableRef->tableInfo,
                                           QCM_TRIGGER_ACTION_EACH_ROW,
                                           QCM_TRIGGER_INSTEAD_OF,
                                           QCM_TRIGGER_EVENT_UPDATE,
                                           NULL,               // UPDATE Column
                                           NULL,               /* Table Cursor */
                                           SC_NULL_GRID,       /* Row GRID */
                                           sDataPlan->oldRow,
                                           sDataPlan->columnsForRow,
                                           sDataPlan->newRow,
                                           sDataPlan->columnsForRow )
                  != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    /* PROJ-1584 DML Return Clause */
    if ( sCodePlan->returnInto != NULL )
    {
        IDE_TEST( qmx::makeRowWithSmiValue( sDataPlan->updateTuple->columns,
                                            sDataPlan->updateTuple->columnCount,
                                            (smiValue*) sDataPlan->newRow,
                                            sDataPlan->returnRow )
                  != IDE_SUCCESS );

        sOrgRow = sDataPlan->updateTuple->row;
        sDataPlan->updateTuple->row = sDataPlan->returnRow;

        IDE_TEST( qmx::copyReturnToInto( aTemplate,
                                         sCodePlan->returnInto,
                                         aTemplate->numRows )
                  != IDE_SUCCESS );

        sDataPlan->updateTuple->row = sOrgRow;
    }
    else
    {
        // nothing do do
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_STACK_OVERFLOW );
    {
        IDE_SET(ideSetErrorCode(mtERR_ABORT_STACK_OVERFLOW));
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;

#undef IDE_FN
}

IDE_RC
qmnUPTE::checkUpdateParentRef( qcTemplate * aTemplate,
                               qmnPlan    * aPlan )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation :
 *     Foreign Key Referencing�� ����
 *     Master Table�� �����ϴ� �� �˻�
 *
 *     To Fix PR-10592
 *     Cursor�� �ùٸ� ����� ���ؼ��� Master�� ���� �˻縦 ������ �Ŀ�
 *     Child Table�� ���� �˻縦 �����Ͽ��� �Ѵ�.
 *
 ***********************************************************************/

#define IDE_FN "qmnUPTE::checkUpdateParentRef"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY(""));

    qmncUPTE            * sCodePlan;
    qmndUPTE            * sDataPlan;
    iduMemoryStatus       sQmxMemStatus;
    void                * sOrgRow;
    void                * sSearchRow;
    qmsPartitionRef     * sPartitionRef;
    qmcInsertPartCursor * sInsertPartCursor;
    UInt                  i;

    sCodePlan = (qmncUPTE*) aPlan;
    sDataPlan = (qmndUPTE*) ( aTemplate->tmplate.data + aPlan->offset );

    //------------------------------------------
    // UPDATE�� �ο� �˻��� ����,
    // ���ſ����� ����� ù��° row ���� ��ġ�� cursor ��ġ ����
    //------------------------------------------

    if ( ( sCodePlan->parentConstraints != NULL ) &&
         ( sDataPlan->updateCursor != NULL ) )
    {
        if ( sCodePlan->tableRef->partitionRef == NULL )
        {
            IDE_TEST( checkUpdateParentRefOnScan( aTemplate,
                                                  sCodePlan,
                                                  sDataPlan->updateTuple )
                      != IDE_SUCCESS );
        }
        else
        {
            for ( sPartitionRef = sCodePlan->tableRef->partitionRef;
                  sPartitionRef != NULL;
                  sPartitionRef = sPartitionRef->next )
            {
                IDE_TEST( checkUpdateParentRefOnScan(
                              aTemplate,
                              sCodePlan,
                              & aTemplate->tmplate.rows[sPartitionRef->table] )
                      != IDE_SUCCESS );
            }
        }
    }
    else
    {
        // Nothing to do.
    }

    //------------------------------------------
    // INSERT�� �ο� �˻��� ����,
    // ���ſ����� ����� ù��° row ���� ��ġ�� cursor ��ġ ����
    //------------------------------------------

    if ( ( sCodePlan->parentConstraints != NULL ) &&
         ( sCodePlan->updateType == QMO_UPDATE_ROWMOVEMENT ) )
    {
        for ( i = 0; i < sDataPlan->insertCursorMgr.mCursorIndexCount; i++ )
        {
            sInsertPartCursor = sDataPlan->insertCursorMgr.mCursorIndex[i];

            sOrgRow = sSearchRow = sDataPlan->updateTuple->row;

            //------------------------------------------
            // ���� ��ü�� ���� ���� Ȯ�� �� �÷� ���� ����
            //------------------------------------------

            IDE_TEST( sInsertPartCursor->cursor.beforeFirstModified( SMI_FIND_MODIFIED_NEW )
                      != IDE_SUCCESS );
            
            //------------------------------------------
            // ����� Row�� �ݺ������� �о� Referecing �˻縦 ��
            //------------------------------------------
            
            IDE_TEST( sInsertPartCursor->cursor.readNewRow( (const void **) & sSearchRow,
                                                            & sDataPlan->updateTuple->rid )
                      != IDE_SUCCESS);

            sDataPlan->updateTuple->row = ( sSearchRow == NULL ) ? sOrgRow : sSearchRow;

            while ( sSearchRow != NULL )
            {
                // Memory ������ ���Ͽ� ���� ��ġ ���
                IDE_TEST( aTemplate->stmt->qmxMem->getStatus(&sQmxMemStatus)
                          != IDE_SUCCESS);

                //------------------------------------------
                // Master Table�� ���� Referencing �˻�
                //------------------------------------------

                IDE_TEST( qdnForeignKey::checkParentRef(
                              aTemplate->stmt,
                              sCodePlan->updateColumnIDs,
                              sCodePlan->parentConstraints,
                              sDataPlan->updateTuple,
                              sDataPlan->updateTuple->row,
                              sCodePlan->updateColumnCount )
                          != IDE_SUCCESS );

                // Memory ������ ���� Memory �̵�
                IDE_TEST( aTemplate->stmt->qmxMem->setStatus(&sQmxMemStatus)
                          != IDE_SUCCESS);

                sOrgRow = sSearchRow = sDataPlan->updateTuple->row;

                IDE_TEST( sInsertPartCursor->cursor.readNewRow(
                              (const void **) &sSearchRow,
                              & sDataPlan->updateTuple->rid )
                          != IDE_SUCCESS);

                sDataPlan->updateTuple->row =
                    ( sSearchRow == NULL ) ? sOrgRow : sSearchRow;
            }
        }
    }
    else
    {
        // Nothing to do.
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;

#undef IDE_FN
}

IDE_RC
qmnUPTE::checkUpdateParentRefOnScan( qcTemplate   * aTemplate,
                                     qmncUPTE     * aCodePlan,
                                     mtcTuple     * aUpdateTuple )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation :
 *     Foreign Key Referencing�� ����
 *     Master Table�� �����ϴ� �� �˻�
 *
 *     To Fix PR-10592
 *     Cursor�� �ùٸ� ����� ���ؼ��� Master�� ���� �˻縦 ������ �Ŀ�
 *     Child Table�� ���� �˻縦 �����Ͽ��� �Ѵ�.
 *
 ***********************************************************************/

#define IDE_FN "qmnUPTE::checkUpdateParentRefOnScan"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY(""));

    iduMemoryStatus   sQmxMemStatus;
    void            * sOrgRow;
    void            * sSearchRow;
    smiTableCursor  * sUpdateCursor;
    qmnCursorInfo   * sCursorInfo;

    //------------------------------------------
    // UPDATE�� �ο� �˻��� ����,
    // ���ſ����� ����� ù��° row ���� ��ġ�� cursor ��ġ ����
    //------------------------------------------

    sCursorInfo = (qmnCursorInfo*) aUpdateTuple->cursorInfo;

    IDE_TEST_RAISE( sCursorInfo == NULL, ERR_NOT_FOUND );

    sUpdateCursor = sCursorInfo->cursor;

    // BUG-37147 sUpdateCursor �� null �� ��찡 �߻���
    // PROJ-1624 non-partitioned index
    // index table scan���� open���� ���� partition�� �����Ѵ�.
    if ( sUpdateCursor != NULL )
    {
        sOrgRow = sSearchRow = aUpdateTuple->row;

        IDE_TEST( sUpdateCursor->beforeFirstModified( SMI_FIND_MODIFIED_NEW )
                  != IDE_SUCCESS );
        
        IDE_TEST( sUpdateCursor->readNewRow( (const void**) & sSearchRow,
                                             & aUpdateTuple->rid )
                  != IDE_SUCCESS );

        //------------------------------------------
        // Referencing �˻縦 ���� ������ Row���� �˻�
        //------------------------------------------

        aUpdateTuple->row = ( sSearchRow == NULL ) ? sOrgRow : sSearchRow;

        while( sSearchRow != NULL )
        {
            // Memory ������ ���Ͽ� ���� ��ġ ���
            IDE_TEST( aTemplate->stmt->qmxMem->getStatus(&sQmxMemStatus)
                    != IDE_SUCCESS );

            //------------------------------------------
            // Child Table�� ���� Referencing �˻�
            //------------------------------------------

            IDE_TEST( qdnForeignKey::checkParentRef(
                        aTemplate->stmt,
                        aCodePlan->updateColumnIDs,
                        aCodePlan->parentConstraints,
                        aUpdateTuple,
                        aUpdateTuple->row,
                        aCodePlan->updateColumnCount )
                    != IDE_SUCCESS );

            // Memory ������ ���� Memory �̵�
            IDE_TEST( aTemplate->stmt->qmxMem->setStatus(&sQmxMemStatus)
                    != IDE_SUCCESS );

            sOrgRow = sSearchRow = aUpdateTuple->row;

            IDE_TEST( sUpdateCursor->readNewRow( (const void**) & sSearchRow,
                                                 & aUpdateTuple->rid )
                      != IDE_SUCCESS );

            aUpdateTuple->row =
                (sSearchRow == NULL) ? sOrgRow : sSearchRow;
        }
    }
    else
    {
        // Nothing to do.
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_NOT_FOUND )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmnUPTE::checkUpdateParentRefOnScan",
                                  "cursor not found" ));
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;

#undef IDE_FN
}

IDE_RC
qmnUPTE::checkUpdateChildRef( qcTemplate * aTemplate,
                              qmnPlan    * aPlan )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation :
 *
 ***********************************************************************/

#define IDE_FN "qmnUPTE::checkUpdateChildRef"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY(""));

    qmncUPTE        * sCodePlan;
    qmndUPTE        * sDataPlan;
    qmsPartitionRef * sPartitionRef;
    smiStatement      sSmiStmt;
    smiStatement    * sSmiStmtOrg;
    UInt              sStage = 0;

    sCodePlan = (qmncUPTE*) aPlan;
    sDataPlan = (qmndUPTE*) ( aTemplate->tmplate.data + aPlan->offset );

    //------------------------------------------
    // ���ռ� �˻�
    //------------------------------------------

    IDE_DASSERT( aTemplate != NULL );

    //------------------------------------------
    // child constraint �˻�
    //------------------------------------------

    if ( sCodePlan->childConstraints != NULL )
    {
        // BUG-17940 parent key�� �����ϰ� child key�� ã����
        // parent row�� lock�� ���� ���� view�� ��������
        // ���ο� smiStmt�� �̿��Ѵ�.
        // Update cascade �ɼǿ� ����ؼ� normal�� �Ѵ�.
        // child table�� Ÿ���� ���� �� �� ���� ������ ALL CURSOR�� �Ѵ�.
        qcg::getSmiStmt( aTemplate->stmt, & sSmiStmtOrg );

        IDE_TEST( sSmiStmt.begin( aTemplate->stmt->mStatistics,
                                  QC_SMI_STMT( aTemplate->stmt ),
                                  SMI_STATEMENT_NORMAL |
                                  SMI_STATEMENT_SELF_TRUE |
                                  SMI_STATEMENT_ALL_CURSOR )
                  != IDE_SUCCESS );
        qcg::setSmiStmt( aTemplate->stmt, & sSmiStmt );

        sStage = 1;

        if ( sDataPlan->updateCursor != NULL )
        {
            if ( sCodePlan->tableRef->partitionRef == NULL )
            {
                IDE_TEST( checkUpdateChildRefOnScan( aTemplate,
                                                     sCodePlan,
                                                     sCodePlan->tableRef->tableInfo,
                                                     sDataPlan->updateTuple )
                          != IDE_SUCCESS );
            }
            else
            {
                for ( sPartitionRef = sCodePlan->tableRef->partitionRef;
                      sPartitionRef != NULL;
                      sPartitionRef = sPartitionRef->next )
                {
                    IDE_TEST( checkUpdateChildRefOnScan(
                                  aTemplate,
                                  sCodePlan,
                                  sPartitionRef->partitionInfo,
                                  & aTemplate->tmplate.rows[sPartitionRef->table] )
                              != IDE_SUCCESS );
                }
            }
        }
        else
        {
            // Nothing to do.
        }

        sStage = 0;

        qcg::setSmiStmt( aTemplate->stmt, sSmiStmtOrg );
        IDE_TEST( sSmiStmt.end(SMI_STATEMENT_RESULT_SUCCESS) != IDE_SUCCESS);
    }
    else
    {
        // Nothing to do.
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    if( sStage == 1 )
    {
        qcg::setSmiStmt( aTemplate->stmt, sSmiStmtOrg );

        if (sSmiStmt.end(SMI_STATEMENT_RESULT_FAILURE) != IDE_SUCCESS)
        {
            IDE_CALLBACK_FATAL("Check Child Key On Update smiStmt.end() failed");
        }
    }

    return IDE_FAILURE;

#undef IDE_FN
}

IDE_RC
qmnUPTE::checkUpdateChildRefOnScan( qcTemplate     * aTemplate,
                                    qmncUPTE       * aCodePlan,
                                    qcmTableInfo   * aTableInfo,
                                    mtcTuple       * aUpdateTuple )
{
/***********************************************************************
 *
 * Description :
 *    UPDATE ���� ���� �� Child Table�� ���� Referencing ���� ������ �˻�
 *
 * Implementation :
 *
 ***********************************************************************/

#define IDE_FN "qmnUPTE::checkUpdateChildRefOnScan"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY(""));

    iduMemoryStatus   sQmxMemStatus;
    void            * sOrgRow;
    void            * sSearchRow;
    smiTableCursor  * sUpdateCursor;
    qmnCursorInfo   * sCursorInfo;

    //------------------------------------------
    // ���ռ� �˻�
    //------------------------------------------

    IDE_DASSERT( aTemplate != NULL );
    IDE_DASSERT( aCodePlan->childConstraints != NULL );

    //------------------------------------------
    // UPDATE�� �ο� �˻��� ����,
    // ���ſ����� ����� ù��° row ���� ��ġ�� cursor ��ġ ����
    //------------------------------------------

    sCursorInfo = (qmnCursorInfo*) aUpdateTuple->cursorInfo;

    IDE_TEST_RAISE( sCursorInfo == NULL, ERR_NOT_FOUND );

    sUpdateCursor = sCursorInfo->cursor;

    // PROJ-1624 non-partitioned index
    // index table scan���� open���� ���� partition�� �����Ѵ�.
    if ( sUpdateCursor != NULL )
    {
        IDE_TEST( sUpdateCursor->beforeFirstModified( SMI_FIND_MODIFIED_OLD )
                  != IDE_SUCCESS );

        //------------------------------------------
        // Referencing �˻縦 ���� ������ Row���� �˻�
        //------------------------------------------

        sOrgRow = sSearchRow = aUpdateTuple->row;
        IDE_TEST(
            sUpdateCursor->readOldRow( (const void**) & sSearchRow,
                                       & aUpdateTuple->rid )
            != IDE_SUCCESS );

        aUpdateTuple->row = ( sSearchRow == NULL ) ? sOrgRow : sSearchRow;

        while( sSearchRow != NULL )
        {
            // Memory ������ ���Ͽ� ���� ��ġ ���
            IDE_TEST( aTemplate->stmt->qmxMem->getStatus(&sQmxMemStatus)
                      != IDE_SUCCESS );

            //------------------------------------------
            // Child Table�� ���� Referencing �˻�
            //------------------------------------------

            IDE_TEST( qdnForeignKey::checkChildRefOnUpdate(
                          aTemplate->stmt,
                          aCodePlan->tableRef,
                          aTableInfo,
                          aCodePlan->updateColumnIDs,
                          aCodePlan->childConstraints,
                          aTableInfo->tableID,
                          aUpdateTuple,
                          aUpdateTuple->row,
                          aCodePlan->updateColumnCount )
                      != IDE_SUCCESS );

            // Memory ������ ���� Memory �̵�
            IDE_TEST( aTemplate->stmt->qmxMem->setStatus(&sQmxMemStatus)
                      != IDE_SUCCESS );
            sOrgRow = sSearchRow = aUpdateTuple->row;

            IDE_TEST(
                sUpdateCursor->readOldRow( (const void**) & sSearchRow,
                                           & aUpdateTuple->rid )
                != IDE_SUCCESS );

            aUpdateTuple->row =
                (sSearchRow == NULL) ? sOrgRow : sSearchRow;
        }
    }
    else
    {
        // Nothing to do.
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_NOT_FOUND )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmnUPTE::checkUpdateChildRefOnScan",
                                  "cursor not found" ));
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;

#undef IDE_FN
}

IDE_RC
qmnUPTE::updateIndexTableCursor( qcTemplate     * aTemplate,
                                 qmncUPTE       * aCodePlan,
                                 qmndUPTE       * aDataPlan,
                                 smiValue       * aUpdateValue )
{
/***********************************************************************
 *
 * Description :
 *    UPDATE ���� ���� �� index table�� ���� update ����
 *
 * Implementation :
 *
 ***********************************************************************/

#define IDE_FN "qmnUPTE::updateIndexTableCursor"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY(""));

    void      * sRow;
    scGRID      sRowGRID;
    UInt        sIndexUpdateValueCount;

    // update index table
    if ( ( *aDataPlan->flag & QMND_UPTE_INDEX_CURSOR_MASK )
         == QMND_UPTE_INDEX_CURSOR_INITED )
    {
        // selected index table
        if ( ( *aDataPlan->flag & QMND_UPTE_SELECTED_INDEX_CURSOR_MASK )
             == QMND_UPTE_SELECTED_INDEX_CURSOR_TRUE )
        {
            IDE_TEST( qmsIndexTable::makeUpdateSmiValue(
                          aCodePlan->updateColumnCount,
                          aCodePlan->updateColumnIDs,
                          aUpdateValue,
                          aCodePlan->tableRef->selectedIndexTable,
                          ID_FALSE,
                          NULL,
                          NULL,
                          & sIndexUpdateValueCount,
                          aDataPlan->indexUpdateValue )
                      != IDE_SUCCESS );

            // PROJ-2204 join update, delete
            // tuple ������ cursor�� �����ؾ��Ѵ�.
            if ( ( aCodePlan->flag & QMNC_UPTE_VIEW_MASK )
                 == QMNC_UPTE_VIEW_TRUE )
            {
                IDE_TEST( aDataPlan->indexUpdateCursor->setRowPosition(
                              aDataPlan->indexUpdateTuple->row,
                              aDataPlan->indexUpdateTuple->rid )
                          != IDE_SUCCESS );
            }
            else
            {
                // Nothing to do.
            }

            IDE_TEST( aDataPlan->indexUpdateCursor->updateRow(
                          aDataPlan->indexUpdateValue,
                          & sRow,
                          & sRowGRID )
                      != IDE_SUCCESS );
        }
        else
        {
            // Nothing to do.
        }

        // �ٸ� index table�� update
        IDE_TEST( qmsIndexTable::updateIndexTableCursors(
                      aTemplate->stmt,
                      & (aDataPlan->indexTableCursorInfo),
                      aCodePlan->updateColumnCount,
                      aCodePlan->updateColumnIDs,
                      aUpdateValue,
                      ID_FALSE,
                      NULL,
                      NULL,
                      aDataPlan->updateTuple->rid )
                  != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;

#undef IDE_FN
}

IDE_RC
qmnUPTE::updateIndexTableCursorForRowMovement( qcTemplate     * aTemplate,
                                               qmncUPTE       * aCodePlan,
                                               qmndUPTE       * aDataPlan,
                                               smOID            aPartOID,
                                               scGRID           aRowGRID,
                                               smiValue       * aUpdateValue )
{
/***********************************************************************
 *
 * Description :
 *    UPDATE ���� ���� �� index table�� ���� update ����
 *
 * Implementation :
 *
 ***********************************************************************/

#define IDE_FN "qmnUPTE::updateIndexTableCursorForRowMovement"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY(""));

    smOID       sPartOID = aPartOID;
    scGRID      sRowGRID = aRowGRID;
    void      * sRow;
    scGRID      sGRID;
    UInt        sIndexUpdateValueCount;

    // update index table
    if ( ( *aDataPlan->flag & QMND_UPTE_INDEX_CURSOR_MASK )
         == QMND_UPTE_INDEX_CURSOR_INITED )
    {
        // selected index table
        if ( ( *aDataPlan->flag & QMND_UPTE_SELECTED_INDEX_CURSOR_MASK )
             == QMND_UPTE_SELECTED_INDEX_CURSOR_TRUE )
        {
            IDE_TEST( qmsIndexTable::makeUpdateSmiValue(
                          aCodePlan->updateColumnCount,
                          aCodePlan->updateColumnIDs,
                          aUpdateValue,
                          aCodePlan->tableRef->selectedIndexTable,
                          ID_TRUE,
                          & sPartOID,
                          & sRowGRID,
                          & sIndexUpdateValueCount,
                          aDataPlan->indexUpdateValue )
                      != IDE_SUCCESS );

            // PROJ-2204 join update, delete
            // tuple ������ cursor�� �����ؾ��Ѵ�.
            if ( ( aCodePlan->flag & QMNC_UPTE_VIEW_MASK )
                 == QMNC_UPTE_VIEW_TRUE )
            {
                IDE_TEST( aDataPlan->indexUpdateCursor->setRowPosition(
                              aDataPlan->indexUpdateTuple->row,
                              aDataPlan->indexUpdateTuple->rid )
                          != IDE_SUCCESS );
            }
            else
            {
                // Nothing to do.
            }

            IDE_TEST( aDataPlan->indexUpdateCursor->updateRow(
                          aDataPlan->indexUpdateValue,
                          & sRow,
                          & sGRID )
                      != IDE_SUCCESS );
        }
        else
        {
            // Nothing to do.
        }

        // �ٸ� index table�� update
        IDE_TEST( qmsIndexTable::updateIndexTableCursors(
                      aTemplate->stmt,
                      & (aDataPlan->indexTableCursorInfo),
                      aCodePlan->updateColumnCount,
                      aCodePlan->updateColumnIDs,
                      aUpdateValue,
                      ID_TRUE,
                      & sPartOID,
                      & sRowGRID,
                      aDataPlan->updateTuple->rid )
                  != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;

#undef IDE_FN
}

IDE_RC qmnUPTE::getLastUpdatedRowGRID( qcTemplate * aTemplate,
                                       qmnPlan    * aPlan,
                                       scGRID     * aRowGRID )
{
/***********************************************************************
 *
 * Description : BUG-38129
 *     ������ update row�� GRID�� ��ȯ�Ѵ�.
 *
 * Implementation :
 *
 ***********************************************************************/

    qmndUPTE * sDataPlan =
        (qmndUPTE*) (aTemplate->tmplate.data + aPlan->offset);

    *aRowGRID = sDataPlan->rowGRID;

    return IDE_SUCCESS;
}

IDE_RC qmnUPTE::checkDuplicateUpdate( qmncUPTE   * aCodePlan,
                                      qmndUPTE   * aDataPlan )
{
/***********************************************************************
 *
 * Description : BUG-39399 remove search key preserved table 
 *       join view update�� �ߺ� update���� üũ
 * Implementation :
 *    1. join�� ��� null���� üũ.
 *    2. cursor ����
 *    3. update �ߺ� üũ
 ***********************************************************************/
    
    scGRID            sNullRID;
    void            * sNullRow     = NULL;
    UInt              sTableType;
    void            * sTableHandle = NULL;
    idBool            sIsDupUpdate = ID_FALSE;
    
    /* PROJ-2464 hybrid partitioned table ���� */
    if ( aCodePlan->tableRef->partitionRef == NULL )
    {
        sTableType   = aCodePlan->tableRef->tableInfo->tableFlag & SMI_TABLE_TYPE_MASK;
        sTableHandle = aCodePlan->tableRef->tableHandle;
    }
    else
    {
        sTableType   = aDataPlan->updatePartInfo->tableFlag & SMI_TABLE_TYPE_MASK;
        sTableHandle = aDataPlan->updateTuple->tableHandle;
    }

    /* check null */
    if ( sTableType == SMI_TABLE_DISK )
    {
        SMI_MAKE_VIRTUAL_NULL_GRID( sNullRID );
            
        IDE_TEST_RAISE( SC_GRID_IS_EQUAL( sNullRID,
                                          aDataPlan->updateTuple->rid )
                        == ID_TRUE, ERR_MODIFY_UNABLE_RECORD );
    }
    else
    {
        IDE_TEST( smiGetTableNullRow( sTableHandle,
                                      (void **) & sNullRow,
                                      & sNullRID )
                  != IDE_SUCCESS );        

        IDE_TEST_RAISE( sNullRow == aDataPlan->updateTuple->row,
                        ERR_MODIFY_UNABLE_RECORD );
    }

    // PROJ-2204 join update, delete
    // tuple ������ cursor�� �����ؾ��Ѵ�.
    IDE_TEST( aDataPlan->updateCursor->setRowPosition( aDataPlan->updateTuple->row,
                                                       aDataPlan->updateTuple->rid )
              != IDE_SUCCESS );
        
    /* �ߺ� update���� üũ */
    IDE_TEST( aDataPlan->updateCursor->isUpdatedRowBySameStmt( &sIsDupUpdate )
              != IDE_SUCCESS );

    IDE_TEST_RAISE( sIsDupUpdate == ID_TRUE, ERR_MODIFY_UNABLE_RECORD );
    
    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_MODIFY_UNABLE_RECORD );
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMN_MODIFY_UNABLE_RECORD ) ) ;
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}
