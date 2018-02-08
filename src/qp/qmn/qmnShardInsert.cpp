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
 * $Id$
 *
 * Description :
 *     SDIN(SharD INsert) Node
 *
 *     ������ �𵨿��� insert�� �����ϴ� Plan Node �̴�.
 *
 * ��� ���� :
 *
 * ��� :
 *
 **********************************************************************/

#include <cm.h>
#include <idl.h>
#include <ide.h>
#include <qcg.h>
#include <qmnShardInsert.h>
#include <qdbCommon.h>
#include <qmx.h>

IDE_RC qmnSDIN::init( qcTemplate * aTemplate,
                      qmnPlan    * aPlan )
{
/***********************************************************************
 *
 * Description :
 *    SDIN ����� �ʱ�ȭ
 *
 * Implementation :
 *
 ***********************************************************************/

    qmncSDIN * sCodePlan = (qmncSDIN*) aPlan;
    qmndSDIN * sDataPlan =
        (qmndSDIN*) (aTemplate->tmplate.data + aPlan->offset);

    sDataPlan->flag = & aTemplate->planFlag[sCodePlan->planID];
    sDataPlan->doIt = qmnSDIN::doItDefault;

    //------------------------------------------------
    // ���� �ʱ�ȭ ���� ���� �Ǵ�
    //------------------------------------------------

    if ( ( *sDataPlan->flag & QMND_SDIN_INIT_DONE_MASK )
         == QMND_SDIN_INIT_DONE_FALSE )
    {
        // ���� �ʱ�ȭ ����
        IDE_TEST( firstInit(aTemplate, sCodePlan, sDataPlan) != IDE_SUCCESS );

        //---------------------------------
        // �ʱ�ȭ �ϷḦ ǥ��
        //---------------------------------

        *sDataPlan->flag &= ~QMND_SDIN_INIT_DONE_MASK;
        *sDataPlan->flag |= QMND_SDIN_INIT_DONE_TRUE;
    }
    else
    {
        // Nothing to do.
    }

    //------------------------------------------------
    // Child Plan�� �ʱ�ȭ
    //------------------------------------------------

    if ( aPlan->left != NULL )
    {
        IDE_TEST( aPlan->left->init( aTemplate,
                                     aPlan->left ) != IDE_SUCCESS);
    }
    else
    {
        // Nothing to do.
    }

    //------------------------------------------------
    // ���� �Լ� ����
    //------------------------------------------------

    if ( sCodePlan->isInsertSelect == ID_TRUE )
    {
        sDataPlan->doIt = qmnSDIN::doItFirst;
    }
    else
    {
        sDataPlan->doIt = qmnSDIN::doItFirstMultiRows;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmnSDIN::doIt( qcTemplate * aTemplate,
                      qmnPlan    * aPlan,
                      qmcRowFlag * aFlag )
{
/***********************************************************************
 *
 * Description :
 *    SDIN �� ���� ����� �����Ѵ�.
 *
 * Implementation :
 *    ������ �Լ� �����͸� �����Ѵ�.
 *
 ***********************************************************************/

    qmndSDIN * sDataPlan =
        (qmndSDIN*) (aTemplate->tmplate.data + aPlan->offset);

    return sDataPlan->doIt( aTemplate, aPlan, aFlag );
}

IDE_RC qmnSDIN::padNull( qcTemplate * aTemplate,
                         qmnPlan    * aPlan )
{
/***********************************************************************
 *
 * Description :
 *    SDIN ���� ������ null row�� ������ ������,
 *    Child�� ���Ͽ� padNull()�� ȣ���Ѵ�.
 *
 * Implementation :
 *
 ***********************************************************************/

    qmncSDIN * sCodePlan = (qmncSDIN*) aPlan;
    // qmndSDIN * sDataPlan = 
    //     (qmndSDIN*) (aTemplate->tmplate.data + aPlan->offset);

    if ( (aTemplate->planFlag[sCodePlan->planID] & QMND_SDIN_INIT_DONE_MASK)
         == QMND_SDIN_INIT_DONE_FALSE )
    {
        // �ʱ�ȭ���� ���� ��� �ʱ�ȭ ����
        IDE_TEST( aPlan->init( aTemplate, aPlan ) != IDE_SUCCESS );
    }
    else
    {
        // Nothing To Do
    }

    // Child Plan�� ���Ͽ� Null Padding����
    if ( aPlan->left != NULL )
    {
        IDE_TEST( aPlan->left->padNull( aTemplate, aPlan->left )
                  != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmnSDIN::printPlan( qcTemplate   * aTemplate,
                           qmnPlan      * aPlan,
                           ULong          aDepth,
                           iduVarString * aString,
                           qmnDisplay     aMode )
{
/***********************************************************************
 *
 * Description :
 *    SDIN ����� ���� ������ ����Ѵ�.
 *
 * Implementation :
 *
 ***********************************************************************/

    sdiClientInfo * sClientInfo = aTemplate->stmt->session->mQPSpecific.mClientInfo;
    qmncSDIN * sCodePlan = (qmncSDIN*) aPlan;
    qmndSDIN * sDataPlan =
        (qmndSDIN*) (aTemplate->tmplate.data + aPlan->offset);

    qmmValueNode * sValue;
    qmmMultiRows * sMultiRows;

    sDataPlan->flag = & aTemplate->planFlag[sCodePlan->planID];

    //------------------------------------------------------
    // SDIN Target ������ ���
    //------------------------------------------------------

    // SDIN ������ ���
    qmn::printSpaceDepth( aString, aDepth );
    iduVarStringAppendFormat( aString,
                              "SHARD-INSERT ( TABLE: " );

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
    // BUG-38343 VALUES ������ Subquery ���� ���
    //------------------------------------------------------

    for ( sMultiRows = sCodePlan->rows;
          sMultiRows != NULL;
          sMultiRows = sMultiRows->next )
    {
        for ( sValue = sMultiRows->values;
              sValue != NULL;
              sValue = sValue->next)
        {
            IDE_TEST( qmn::printSubqueryPlan( aTemplate,
                                              sValue->value,
                                              aDepth,
                                              aString,
                                              aMode ) != IDE_SUCCESS );
        }
    }

    //----------------------------
    // ���� ������ �� ���
    //----------------------------

    if ( ( QCG_GET_SESSION_TRCLOG_DETAIL_PREDICATE(aTemplate->stmt) == 1 ) &&
         ( sClientInfo != NULL ) )
    {
        //---------------------------------------------
        // shard execution
        //---------------------------------------------

        IDE_TEST( qmnSDEX::printDataInfo( aTemplate,
                                          sClientInfo,
                                          sDataPlan->mDataInfo,
                                          aDepth + 1,
                                          aString,
                                          aMode,
                                          sDataPlan->flag )
                  != IDE_SUCCESS );
    }
    else
    {
        /* Nothing to do. */
    }

    //------------------------------------------------------
    // Child Plan ������ ���
    //------------------------------------------------------

    if ( aPlan->left != NULL )
    {
        IDE_TEST( aPlan->left->printPlan( aTemplate,
                                          aPlan->left,
                                          aDepth + 1,
                                          aString,
                                          aMode ) != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmnSDIN::firstInit( qcTemplate * aTemplate,
                           qmncSDIN   * aCodePlan,
                           qmndSDIN   * aDataPlan )
{
/***********************************************************************
 *
 * Description :
 *    SDIN node�� Data ������ ����� ���� �ʱ�ȭ�� ����
 *
 * Implementation :
 *    - Data ������ �ֿ� ����� ���� �ʱ�ȭ�� ����
 *
 ***********************************************************************/

    sdiClientInfo  * sClientInfo = NULL;
    sdiBindParam   * sBindParams = NULL;
    sdiDataNode      sDataNodeArg;
    smiValue       * sInsertedRow;

    //---------------------------------
    // �⺻ ����
    //---------------------------------

    aDataPlan->rows = aCodePlan->rows;

    //------------------------------------------
    // INSERT�� ���� Default ROW ����
    //------------------------------------------

    if ( aCodePlan->isInsertSelect == ID_TRUE )
    {
        sInsertedRow = aTemplate->insOrUptRow[aCodePlan->valueIdx];

        if ( aDataPlan->rows != NULL )
        {
            // set DEFAULT value
            IDE_TEST( qmx::makeSmiValueWithValue( aCodePlan->columnsForValues,
                                                  aDataPlan->rows->values,
                                                  aTemplate,
                                                  aCodePlan->tableRef->tableInfo,
                                                  sInsertedRow,
                                                  NULL )
                      != IDE_SUCCESS);
        }
        else
        {
            // Nothing To Do
        }
    }
    else
    {
        // Nothing To Do
    }

    //-------------------------------
    // ������ �ʱ�ȭ
    //-------------------------------

    // shard linker �˻� & �ʱ�ȭ
    IDE_TEST( sdi::checkShardLinker( aTemplate->stmt ) != IDE_SUCCESS );

    IDE_TEST_RAISE( aTemplate->shardExecData.execInfo == NULL,
                    ERR_NO_SHARD_INFO );

    aDataPlan->mDataInfo = ((sdiDataNodes*)aTemplate->shardExecData.execInfo)
        + aCodePlan->shardDataIndex;

    //-------------------------------
    // shard ������ ���� �غ�
    //-------------------------------

    sClientInfo = aTemplate->stmt->session->mQPSpecific.mClientInfo;

    if ( aDataPlan->mDataInfo->mInitialized == ID_FALSE )
    {
        idlOS::memset( &sDataNodeArg, 0x00, ID_SIZEOF(sdiDataNode) );

        sDataNodeArg.mBindParamCount = aCodePlan->shardParamCount;
        sDataNodeArg.mBindParams = (sdiBindParam*)
            ( aTemplate->shardExecData.data + aCodePlan->bindParam );

        // �ʱ�ȭ
        IDE_TEST( setParamInfo( aTemplate,
                                aCodePlan,
                                sDataNodeArg.mBindParams )
                  != IDE_SUCCESS );

        IDE_TEST( sdi::initShardDataInfo( aTemplate,
                                          aCodePlan->shardAnalysis,
                                          sClientInfo,
                                          aDataPlan->mDataInfo,
                                          & sDataNodeArg )
                  != IDE_SUCCESS );
    }
    else
    {
        if ( aCodePlan->shardParamCount > 0 )
        {
            IDE_TEST( aTemplate->stmt->qmxMem->alloc(
                          ID_SIZEOF(sdiBindParam) * aCodePlan->shardParamCount,
                          (void**) & sBindParams )
                      != IDE_SUCCESS );

            IDE_TEST( setParamInfo( aTemplate,
                                    aCodePlan,
                                    sBindParams )
                      != IDE_SUCCESS );

            IDE_TEST( sdi::reuseShardDataInfo( aTemplate,
                                               sClientInfo,
                                               aDataPlan->mDataInfo,
                                               sBindParams,
                                               aCodePlan->shardParamCount )
                      != IDE_SUCCESS );
        }
        else
        {
            // Nothing to do.
        }
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_NO_SHARD_INFO )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmnSDIN::firstInit",
                                  "Shard Info is not found" ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmnSDIN::setParamInfo( qcTemplate   * aTemplate,
                              qmncSDIN     * aCodePlan,
                              sdiBindParam * aBindParams )
{
    qcmTableInfo   * sTableForInsert;
    qcmColumn      * sColumn;
    mtcTuple       * sCanonizedTuple;
    mtcColumn      * sMtcColumn;
    UInt             i;
    UInt             j;

    sTableForInsert = aCodePlan->tableRef->tableInfo;
    sCanonizedTuple = &(aTemplate->tmplate.rows[aCodePlan->canonizedTuple]);

    for ( i = 0, j = 0; i < sTableForInsert->columnCount; i++ )
    {
        sColumn = &(sTableForInsert->columns[i]);

        if ( (sColumn->flag & QCM_COLUMN_HIDDEN_COLUMN_MASK)
             == QCM_COLUMN_HIDDEN_COLUMN_FALSE )
        {
            sMtcColumn = &(sCanonizedTuple->columns[i]);

            aBindParams[j].mId        = j + 1;
            aBindParams[j].mInoutType = CMP_DB_PARAM_INPUT;
            aBindParams[j].mType      = sColumn->basicInfo->module->id;
            aBindParams[j].mData      =
                (UChar*)sCanonizedTuple->row + sMtcColumn->column.offset;
            aBindParams[j].mDataSize  = sMtcColumn->column.size;
            aBindParams[j].mPrecision = sMtcColumn->precision;
            aBindParams[j].mScale     = sMtcColumn->scale;

            j++;
            IDE_DASSERT( j <= aCodePlan->shardParamCount );
        }
        else
        {
            // Nothing to do.
        }
    }

    return IDE_SUCCESS;
}

IDE_RC qmnSDIN::doItDefault( qcTemplate * /* aTemplate */,
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

    IDE_DASSERT( 0 );

    return IDE_FAILURE;
}

IDE_RC qmnSDIN::doItFirst( qcTemplate * aTemplate,
                           qmnPlan    * aPlan,
                           qmcRowFlag * aFlag )
{
/***********************************************************************
 *
 * Description :
 *    SDIN�� ���� ���� �Լ�
 *
 * Implementation :
 *    - Table�� IX Lock�� �Ǵ�.
 *    - Session Event Check (������ ���� Detect)
 *    - Cursor Open
 *    - insert one record
 *
 ***********************************************************************/

    //qmncSDIN * sCodePlan = (qmncSDIN*) aPlan;
    qmndSDIN * sDataPlan =
        (qmndSDIN*) (aTemplate->tmplate.data + aPlan->offset);

    //-----------------------------------
    // Child Plan�� ������
    //-----------------------------------

    // doIt left child
    IDE_TEST( aPlan->left->doIt( aTemplate, aPlan->left, aFlag )
              != IDE_SUCCESS );

    //-----------------------------------
    // Insert�� ������
    //-----------------------------------

    if ( ( *aFlag & QMC_ROW_DATA_MASK ) == QMC_ROW_DATA_EXIST )
    {
        // insert one record
        IDE_TEST( insertOneRow( aTemplate, aPlan ) != IDE_SUCCESS );

        sDataPlan->doIt = qmnSDIN::doItNext;
    }
    else
    {
        // nothing to do
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmnSDIN::doItNext( qcTemplate * aTemplate,
                          qmnPlan    * aPlan,
                          qmcRowFlag * aFlag )
{
/***********************************************************************
 *
 * Description :
 *    SDIN�� ���� ���� �Լ�
 *    ���� Record�� �����Ѵ�.
 *
 * Implementation :
 *    - insert one record
 *
 ***********************************************************************/

    //qmncSDIN * sCodePlan = (qmncSDIN*) aPlan;
    qmndSDIN * sDataPlan =
        (qmndSDIN*) (aTemplate->tmplate.data + aPlan->offset);

    //-----------------------------------
    // Child Plan�� ������
    //-----------------------------------

    // doIt left child
    IDE_TEST( aPlan->left->doIt( aTemplate, aPlan->left, aFlag )
              != IDE_SUCCESS );

    //-----------------------------------
    // Insert�� ������
    //-----------------------------------

    if ( ( *aFlag & QMC_ROW_DATA_MASK ) == QMC_ROW_DATA_EXIST )
    {
        // insert one record
        IDE_TEST( insertOneRow( aTemplate, aPlan ) != IDE_SUCCESS );
    }
    else
    {
        // record�� ���� ���
        // ���� ������ ���� ���� ���� �Լ��� ������.
        sDataPlan->doIt = qmnSDIN::doItFirst;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmnSDIN::doItFirstMultiRows( qcTemplate * aTemplate,
                                    qmnPlan    * aPlan,
                                    qmcRowFlag * aFlag )
{
    //qmncSDIN * sCodePlan = (qmncSDIN*) aPlan;
    qmndSDIN * sDataPlan =
        (qmndSDIN*) (aTemplate->tmplate.data + aPlan->offset);

    //-----------------------------------
    // Insert�� ������
    //-----------------------------------

    // insert one record
    IDE_TEST( insertOnce( aTemplate, aPlan ) != IDE_SUCCESS );

    if ( sDataPlan->rows->next != NULL )
    {
        *aFlag &= ~QMC_ROW_DATA_MASK;
        *aFlag |= QMC_ROW_DATA_EXIST;

        sDataPlan->doIt = qmnSDIN::doItNextMultiRows;
    }
    else
    {
        *aFlag &= ~QMC_ROW_DATA_MASK;
        *aFlag |= QMC_ROW_DATA_NONE;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmnSDIN::doItNextMultiRows( qcTemplate * aTemplate,
                                   qmnPlan    * aPlan,
                                   qmcRowFlag * aFlag )
{
    //qmncSDIN * sCodePlan = (qmncSDIN*) aPlan;
    qmndSDIN * sDataPlan =
        (qmndSDIN*) (aTemplate->tmplate.data + aPlan->offset);

    IDE_TEST_RAISE( sDataPlan->rows->next == NULL, ERR_UNEXPECTED );

    //-----------------------------------
    // �ٸ� Row�� ����
    //-----------------------------------
    sDataPlan->rows = sDataPlan->rows->next;

    //-----------------------------------
    // Insert�� ������
    //-----------------------------------

    // insert one record
    IDE_TEST( insertOnce( aTemplate, aPlan ) != IDE_SUCCESS );

    if ( sDataPlan->rows->next == NULL )
    {
        *aFlag &= ~QMC_ROW_DATA_MASK;
        *aFlag |= QMC_ROW_DATA_NONE;

        // ���� ������ ���� ���� ���� �Լ��� ������.
        sDataPlan->doIt = qmnSDIN::doItFirstMultiRows;
    }
    else
    {
        *aFlag &= ~QMC_ROW_DATA_MASK;
        *aFlag |= QMC_ROW_DATA_EXIST;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_UNEXPECTED )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmnInsert::doItNextMultiRows",
                                  "Invalid Next rows" ));
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmnSDIN::insertOneRow( qcTemplate * aTemplate,
                              qmnPlan    * aPlan )
{
/***********************************************************************
 *
 * Description :
 *    SDIN �� ���� ����� �����Ѵ�.
 *
 * Implementation :
 *    - insert one record ����
 *
 ***********************************************************************/

    qmncSDIN         * sCodePlan = (qmncSDIN*) aPlan;
    qmndSDIN         * sDataPlan =
        (qmndSDIN*) (aTemplate->tmplate.data + aPlan->offset);

    iduMemoryStatus    sQmxMemStatus;
    qcmTableInfo     * sTableForInsert;
    mtcTuple         * sCanonizedTuple;
    smiValue         * sInsertedRow;

    sdiClientInfo    * sClientInfo = aTemplate->stmt->session->mQPSpecific.mClientInfo;
    vSLong             sNumRows = 0;

    //---------------------------------
    // �⺻ ����
    //---------------------------------

    sCanonizedTuple = &(aTemplate->tmplate.rows[sCodePlan->canonizedTuple]);
    sTableForInsert = sCodePlan->tableRef->tableInfo;
    sInsertedRow = aTemplate->insOrUptRow[sCodePlan->valueIdx];

    // Memory ������ ���Ͽ� ���� ��ġ ���
    IDE_TEST( aTemplate->stmt->qmxMem->getStatus( &sQmxMemStatus )
              != IDE_SUCCESS );

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
    // make insert value
    //-----------------------------------

    // stack�� ���� �̿�
    IDE_TEST( qmx::makeSmiValueWithResult( sCodePlan->columns,
                                           aTemplate,
                                           sTableForInsert,
                                           sInsertedRow,
                                           NULL )
              != IDE_SUCCESS );

    //-----------------------------------
    // check null
    //-----------------------------------

    IDE_TEST( qmx::checkNotNullColumnForInsert(
                  sTableForInsert->columns,
                  sInsertedRow,
                  NULL,
                  ID_TRUE )
              != IDE_SUCCESS );

    //------------------------------------------
    // Shard Insert�� ���� Default ROW ����
    //------------------------------------------

    if ( ( sCodePlan->columnsForValues != NULL ) &&
         ( sDataPlan->rows != NULL ) &&
         ( sCodePlan->nextValSeqs != NULL ) )
    {
        sInsertedRow = aTemplate->insOrUptRow[sCodePlan->valueIdx];

        // set DEFAULT value
        IDE_TEST( qmx::makeSmiValueWithValue( sCodePlan->columnsForValues,
                                              sDataPlan->rows->values,
                                              aTemplate,
                                              sTableForInsert,
                                              sInsertedRow,
                                              NULL )
                  != IDE_SUCCESS );
    }
    else
    {
        // Nothing To Do
    }

    //-----------------------------------
    // canonized tuple�� ���� ����
    //-----------------------------------

    IDE_TEST( copySmiValueToTuple( sTableForInsert,
                                   sInsertedRow,
                                   sCanonizedTuple )
              != IDE_SUCCESS );

    //-------------------------------
    // ������ ����
    //-------------------------------

    IDE_TEST( sdi::decideShardDataInfo(
                  aTemplate,
                  &(aTemplate->tmplate.rows[sCodePlan->canonizedTuple]),
                  sCodePlan->shardAnalysis,
                  sClientInfo,
                  sDataPlan->mDataInfo,
                  &(sCodePlan->shardQuery) )
              != IDE_SUCCESS );

    //-------------------------------
    // ����
    //-------------------------------

    IDE_TEST( sdi::executeInsert( aTemplate->stmt,
                                  sClientInfo,
                                  sDataPlan->mDataInfo,
                                  & sNumRows )
              != IDE_SUCCESS );

    // result row count
    aTemplate->numRows += sNumRows;

    //-----------------------------------
    // �Ϸ�
    //-----------------------------------

    // Memory ������ ���� Memory �̵�
    IDE_TEST( aTemplate->stmt->qmxMem->setStatus( &sQmxMemStatus )
              != IDE_SUCCESS);

    if ( ( *sDataPlan->flag & QMND_SDIN_INSERT_MASK )
         == QMND_SDIN_INSERT_FALSE )
    {
        *sDataPlan->flag &= ~QMND_SDIN_INSERT_MASK;
        *sDataPlan->flag |= QMND_SDIN_INSERT_TRUE;
    }
    else
    {
        // Nothing to do.
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmnSDIN::insertOnce( qcTemplate * aTemplate,
                            qmnPlan    * aPlan )
{
/***********************************************************************
 *
 * Description :
 *    insertOnce�� insertOneRow�� cursor�� open�ϴ� ������ �ٸ���.
 *    insertOnce������ makeSmiValue���Ŀ� cursor�� open�Ѵ�.
 *    ���� �������� t1 insert cursor�� ���� ���� subquery�� ����� �� ����.
 *
 *    ex)
 *    insert into t1 values ( select max(i1) from t1 );
 *
 * Implementation :
 *    - cursor open
 *    - insert one record ����
 *    - trigger each row ����
 *
 ***********************************************************************/

    qmncSDIN       * sCodePlan = (qmncSDIN*) aPlan;
    qmndSDIN       * sDataPlan =
        (qmndSDIN*) (aTemplate->tmplate.data + aPlan->offset);

    qcmTableInfo   * sTableForInsert;
    mtcTuple       * sCanonizedTuple;
    smiValue       * sInsertedRow;

    sdiClientInfo  * sClientInfo = aTemplate->stmt->session->mQPSpecific.mClientInfo;
    vSLong           sNumRows = 0;

    sCanonizedTuple = &(aTemplate->tmplate.rows[sCodePlan->canonizedTuple]);
    sTableForInsert = sCodePlan->tableRef->tableInfo;
    sInsertedRow = aTemplate->insOrUptRow[sCodePlan->valueIdx];

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
    // make insert value
    //-----------------------------------

    // values�� ���� �̿�
    IDE_TEST( qmx::makeSmiValueWithValue( aTemplate,
                                          sTableForInsert,
                                          sCodePlan->canonizedTuple,
                                          sDataPlan->rows->values,
                                          sCodePlan->queueMsgIDSeq,
                                          sInsertedRow,
                                          NULL )
              != IDE_SUCCESS );

    //-----------------------------------
    // canonized tuple�� ���� ����
    //-----------------------------------

    IDE_TEST( copySmiValueToTuple( sTableForInsert,
                                   sInsertedRow,
                                   sCanonizedTuple )
              != IDE_SUCCESS );

    //-----------------------------------
    // check null
    //-----------------------------------

    IDE_TEST( qmx::checkNotNullColumnForInsert(
                  sTableForInsert->columns,
                  sInsertedRow,
                  NULL,
                  ID_TRUE )
              != IDE_SUCCESS );

    //-------------------------------
    // ������ ����
    //-------------------------------

    IDE_TEST( sdi::decideShardDataInfo(
                  aTemplate,
                  &(aTemplate->tmplate.rows[sCodePlan->canonizedTuple]),
                  sCodePlan->shardAnalysis,
                  sClientInfo,
                  sDataPlan->mDataInfo,
                  &(sCodePlan->shardQuery) )
              != IDE_SUCCESS );

    //-------------------------------
    // ����
    //-------------------------------

    IDE_TEST( sdi::executeInsert( aTemplate->stmt,
                                  sClientInfo,
                                  sDataPlan->mDataInfo,
                                  & sNumRows )
              != IDE_SUCCESS );

    // result row count
    aTemplate->numRows += sNumRows;

    //-----------------------------------
    // �Ϸ�
    //-----------------------------------

    if ( ( *sDataPlan->flag & QMND_SDIN_INSERT_MASK )
         == QMND_SDIN_INSERT_FALSE )
    {
        *sDataPlan->flag &= ~QMND_SDIN_INSERT_MASK;
        *sDataPlan->flag |= QMND_SDIN_INSERT_TRUE;
    }
    else
    {
        // Nothing to do.
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmnSDIN::copySmiValueToTuple( qcmTableInfo * aTableInfo,
                                     smiValue     * aInsertedRow,
                                     mtcTuple     * aTuple )
{
    qcmColumn  * sColumn = NULL;
    SInt         sColumnOrder;
    mtcColumn  * sCanonizedColumn = NULL;
    void       * sCanonizedValue = NULL;
    void       * sValue = NULL;
    UInt         sActualSize;

    IDE_DASSERT( aTableInfo != NULL );
    IDE_DASSERT( aTuple != NULL );

    for ( sColumn = aTableInfo->columns;
          sColumn != NULL;
          sColumn = sColumn->next )
    {
        if ( ( sColumn->flag & QCM_COLUMN_HIDDEN_COLUMN_MASK )
             == QCM_COLUMN_HIDDEN_COLUMN_FALSE )
        {
            IDE_DASSERT( sColumn->basicInfo != NULL );
            sColumnOrder = sColumn->basicInfo->column.id & SMI_COLUMN_ID_MASK;

            sCanonizedColumn = &(aTuple->columns[sColumnOrder]);

            IDE_DASSERT( sCanonizedColumn != NULL );
            sCanonizedValue = (void*)
                ((UChar*)aTuple->row + sCanonizedColumn->column.offset);

            IDE_DASSERT( sColumn->basicInfo->column.id == sCanonizedColumn->column.id );

            IDE_TEST( qdbCommon::storingValue2MtdValue(
                          sCanonizedColumn,
                          (void*) aInsertedRow[sColumnOrder].value,
                          & sValue )
                      != IDE_SUCCESS );

            // BUG-45751
            if ( sValue == NULL )
            {
                sValue = sCanonizedColumn->module->staticNull;
            }
            else
            {
                // Nothing to do.
            }

            if ( sCanonizedValue != sValue )
            {
                sActualSize = sCanonizedColumn->module->actualSize(
                    sCanonizedColumn,
                    sValue );

                idlOS::memcpy( sCanonizedValue,
                               sValue,
                               sActualSize );
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
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}
