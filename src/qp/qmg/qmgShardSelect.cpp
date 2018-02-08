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
 * $Id: qmgShardSELT.cpp 77650 2016-10-17 02:19:47Z timmy.kim $
 *
 * Description : Shard Graph�� ���� ���� �Լ�
 *
 * ��� ���� :
 *
 * ��� :
 *
 **********************************************************************/

#include <ide.h>
#include <qcg.h>
#include <qmo.h>
#include <qcgPlan.h>
#include <qmgShardSelect.h>
#include <qmgShardDML.h>
#include <qmgSelection.h>
#include <qmoOneNonPlan.h>

IDE_RC qmgShardSelect::init( qcStatement  * aStatement,
                             qmsQuerySet  * aQuerySet,
                             qmsFrom      * aFrom,
                             qmgGraph    ** aGraph )
{
/***********************************************************************
 *
 * Description : qmgShardSELT Graph�� �ʱ�ȭ
 *
 * Implementation :
 *
 *            - alloc qmgShardSELT
 *            - init qmgShardSELT, qmgGraph
 *
 ***********************************************************************/

    qmgShardSELT * sMyGraph = NULL;

    IDU_FIT_POINT_FATAL( "qmgShardSelect::init::__FT__" );

    //---------------------------------------------------
    // ���ռ� �˻�
    //---------------------------------------------------

    IDE_FT_ASSERT( aStatement != NULL );
    IDE_FT_ASSERT( aQuerySet != NULL );
    IDE_FT_ASSERT( aFrom != NULL );

    //---------------------------------------------------
    // Shard Graph�� ���� �⺻ �ʱ�ȭ
    //---------------------------------------------------

    // qmgShardSELT�� ���� ���� �Ҵ�
    IDE_TEST( QC_QMP_MEM(aStatement)->alloc( ID_SIZEOF( qmgShardSELT ),
                                             (void**) &sMyGraph )
              != IDE_SUCCESS );

    // Graph ���� ������ �ʱ�ȭ
    IDE_TEST( qmg::initGraph( & sMyGraph->graph ) != IDE_SUCCESS );

    sMyGraph->graph.type = QMG_SHARD_SELECT;
    qtc::dependencySetWithDep( & sMyGraph->graph.depInfo,
                               & aFrom->depInfo );
    sMyGraph->graph.myFrom = aFrom;
    sMyGraph->graph.myQuerySet = aQuerySet;

    sMyGraph->graph.optimize = qmgShardSelect::optimize;
    sMyGraph->graph.makePlan = qmgShardSelect::makePlan;
    sMyGraph->graph.printGraph = qmgShardSelect::printGraph;

    // BUGBUG
    // SDSE ��ü�� disk �� �����ϳ�
    // SDSE �� ���� plan �� interResultType �� memory temp �� ����Ǿ�� �Ѵ�.
    sMyGraph->graph.flag &= ~QMG_GRAPH_TYPE_MASK;
    sMyGraph->graph.flag |=  QMG_GRAPH_TYPE_DISK;

    //---------------------------------------------------
    // Shard ���� ������ �ʱ�ȭ
    //---------------------------------------------------

    SET_POSITION( sMyGraph->shardQuery,
                  aFrom->tableRef->view->myPlan->parseTree->stmtPos );
    sMyGraph->shardAnalysis = aFrom->tableRef->view->myPlan->mShardAnalysis;
    sMyGraph->shardParamOffset = aFrom->tableRef->view->myPlan->mShardParamOffset;
    sMyGraph->shardParamCount = aFrom->tableRef->view->myPlan->mShardParamCount;

    sMyGraph->limit = NULL;
    sMyGraph->accessMethod = NULL;
    sMyGraph->accessMethodCnt = 0;
    sMyGraph->flag = QMG_SHARD_FLAG_CLEAR;

    // out ����
    *aGraph = (qmgGraph *)sMyGraph;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmgShardSelect::optimize( qcStatement * aStatement,
                                 qmgGraph    * aGraph )
{
/***********************************************************************
 *
 * Description : qmgShardSELT �� ����ȭ
 *
 * Implementation :
 *
 *      - ������� ����
 *      - Subquery Graph ����
 *      - ���� ��� ���� ���� (recordSize, inputRecordCnt)
 *      - Predicate ���ġ �� ���� Predicate�� selectivity ���
 *      - ��ü selectivity ��� �� ���� ��� ������ selectivity�� ����
 *      - Access Method ����
 *      - Preserved Order ����
 *      - ���� ��� ���� ���� (outputRecordCnt, myCost, totalCost)
 *
 ***********************************************************************/

    qmgShardSELT   * sMyGraph  = NULL;
    qmsTarget      * sTarget   = NULL;
    qmsTableRef    * sTableRef = NULL;
    qmsQuerySet    * sQuerySet = NULL;
    qmsParseTree   * sParseTree = NULL;
    mtcColumn      * sTargetColumn = NULL;
    SDouble          sOutputRecordCnt = 0;
    SDouble          sRecordSize = 0;

    IDU_FIT_POINT_FATAL( "qmgShardSelect::optimize::__FT__" );

    //---------------------------------------------------
    // ���ռ� �˻�
    //---------------------------------------------------

    IDE_FT_ASSERT( aStatement != NULL );
    IDE_FT_ASSERT( aGraph != NULL );

    //---------------------------------------------------
    // �⺻ �ʱ�ȭ
    //---------------------------------------------------

    sMyGraph = (qmgShardSELT *)aGraph;
    sTableRef = sMyGraph->graph.myFrom->tableRef;

    IDE_FT_ASSERT( sTableRef != NULL );
    IDE_FT_ASSERT( sTableRef->view != NULL );

    //---------------------------------------------------
    // ��� ���� ����
    //---------------------------------------------------

    IDE_TEST( qmoStat::getStatInfo4RemoteTable( aStatement,
                                                sTableRef->tableInfo,
                                                &(sTableRef->statInfo) )
              != IDE_SUCCESS );

    //---------------------------------------------------
    // Subquery�� Graph ����
    //---------------------------------------------------

    if ( sMyGraph->graph.myPredicate != NULL )
    {
        IDE_TEST( qmoPred::optimizeSubqueries( aStatement,
                                               sMyGraph->graph.myPredicate,
                                               ID_FALSE ) // No KeyRange Tip
                  != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    //---------------------------------------------------
    // ���� ��� ���� ���� (recordSize, inputRecordCnt)
    //---------------------------------------------------

    sParseTree = (qmsParseTree *)sTableRef->view->myPlan->parseTree;

    IDE_FT_ASSERT( sParseTree != NULL );

    for ( sQuerySet = sParseTree->querySet;
          sQuerySet != NULL;
          sQuerySet = sQuerySet->left )
    {
        if ( sQuerySet->setOp == QMS_NONE )
        {
            break;
        }
        else
        {
            // Nothing to do.
        }
    }

    // recordSize ����
    // To Fix BUG-8241
    IDE_FT_ASSERT( sQuerySet != NULL );
    IDE_FT_ASSERT( sQuerySet->SFWGH != NULL );

    for ( sTarget = sQuerySet->SFWGH->target;
          sTarget != NULL;
          sTarget = sTarget->next )
    {
        sTargetColumn = QTC_STMT_COLUMN( aStatement, sTarget->targetColumn );

        IDE_FT_ASSERT( sTargetColumn != NULL );

        IDE_TEST_RAISE(
            ( sTargetColumn->module->id != MTD_BIGINT_ID   ) &&
            ( sTargetColumn->module->id != MTD_SMALLINT_ID ) &&
            ( sTargetColumn->module->id != MTD_INTEGER_ID  ) &&
            ( sTargetColumn->module->id != MTD_DOUBLE_ID   ) &&
            ( sTargetColumn->module->id != MTD_FLOAT_ID    ) &&
            ( sTargetColumn->module->id != MTD_REAL_ID     ) &&
            ( sTargetColumn->module->id != MTD_NUMBER_ID   ) &&
            ( sTargetColumn->module->id != MTD_NUMERIC_ID  ) &&
            ( sTargetColumn->module->id != MTD_BINARY_ID   ) &&
            ( sTargetColumn->module->id != MTD_BIT_ID      ) &&
            ( sTargetColumn->module->id != MTD_VARBIT_ID   ) &&
            ( sTargetColumn->module->id != MTD_BYTE_ID     ) &&
            ( sTargetColumn->module->id != MTD_VARBYTE_ID  ) &&
            ( sTargetColumn->module->id != MTD_NIBBLE_ID   ) &&
            ( sTargetColumn->module->id != MTD_DATE_ID     ) &&
            ( sTargetColumn->module->id != MTD_INTERVAL_ID ) &&
            ( sTargetColumn->module->id != MTD_CHAR_ID     ) &&
            ( sTargetColumn->module->id != MTD_VARCHAR_ID  ) &&
            ( sTargetColumn->module->id != MTD_NCHAR_ID    ) &&
            ( sTargetColumn->module->id != MTD_NVARCHAR_ID ) &&
            ( sTargetColumn->module->id != MTD_NULL_ID     ) &&
            ( sTargetColumn->module->id != MTD_GEOMETRY_ID ),
            ERR_INVALID_SHARD_QUERY );

        sRecordSize += sTargetColumn->column.size;
    }

    // BUG-36463 sRecordSize �� 0�� �Ǿ�� �ȵȴ�.
    sRecordSize = IDL_MAX( sRecordSize, 1 );
    sMyGraph->graph.costInfo.recordSize = sRecordSize;

    // inputRecordCnt ����
    sMyGraph->graph.costInfo.inputRecordCnt = sTableRef->statInfo->totalRecordCnt;

    //---------------------------------------------------
    // Predicate�� ���ġ �� ���� Predicate�� Selectivity ���
    //---------------------------------------------------

    if ( sMyGraph->graph.myPredicate != NULL )
    {
        IDE_TEST( qmoPred::relocatePredicate( aStatement,
                                              sMyGraph->graph.myPredicate,
                                              & sMyGraph->graph.depInfo,
                                              & sMyGraph->graph.myQuerySet->outerDepInfo,
                                              sMyGraph->graph.myFrom->tableRef->statInfo,
                                              & sMyGraph->graph.myPredicate )
                  != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    //---------------------------------------------------
    // accessMethod (full scan)
    //---------------------------------------------------

    IDE_TEST( QC_QMP_MEM(aStatement)->alloc( ID_SIZEOF(qmoAccessMethod),
                                             (void**)&sMyGraph->accessMethod)
              != IDE_SUCCESS );

    qmgSelection::setFullScanMethod( aStatement,
                                     sTableRef->statInfo,
                                     sMyGraph->graph.myPredicate,
                                     sMyGraph->accessMethod,
                                     1,           // parallel degree
                                     ID_FALSE );  // execute time

    sMyGraph->accessMethodCnt = 1;

    //---------------------------------------------------
    // Preserved Order ����
    //---------------------------------------------------

    sMyGraph->graph.flag &= ~QMG_PRESERVED_ORDER_MASK;
    sMyGraph->graph.flag |=  QMG_PRESERVED_ORDER_NEVER;

    //---------------------------------------------------
    // ���� ��� ���� ���� (outputRecordCnt, myCost, totalCost)
    //---------------------------------------------------

    sMyGraph->graph.costInfo.selectivity = sMyGraph->accessMethod->methodSelectivity;

    // output record count ����
    sOutputRecordCnt = sMyGraph->graph.costInfo.selectivity *
        sMyGraph->graph.costInfo.inputRecordCnt;

    sMyGraph->graph.costInfo.outputRecordCnt =
        ( sOutputRecordCnt < 1 ) ? 1 : sOutputRecordCnt;

    // my cost
    sMyGraph->graph.costInfo.myAccessCost = sMyGraph->accessMethod->accessCost;
    sMyGraph->graph.costInfo.myDiskCost = sMyGraph->accessMethod->diskCost;
    sMyGraph->graph.costInfo.myAllCost = sMyGraph->accessMethod->totalCost;

    // total cost
    sMyGraph->graph.costInfo.totalAccessCost = sMyGraph->accessMethod->accessCost;
    sMyGraph->graph.costInfo.totalDiskCost = sMyGraph->accessMethod->diskCost;
    sMyGraph->graph.costInfo.totalAllCost = sMyGraph->accessMethod->totalCost;

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_INVALID_SHARD_QUERY )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMV_NOT_APPLICABLE_TYPE_IN_TARGET,
                                  sTargetColumn->module->names->string ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmgShardSelect::makePlan( qcStatement    * aStatement,
                                 const qmgGraph * aParent,
                                 qmgGraph       * aGraph )
{
/***********************************************************************
 *
 *  Description : qmgShardSELT �� ���� Plan�� �����Ѵ�.
 *
 *  Implementation : SHARD�� ���� ��� �Ʒ��� ���� plan�� �����Ѵ�.
 *
 *          [PARENT]
 *            |
 *          [SDSE] (SharD SElect)
 *        ----|------------------------------------------
 *          [...]    [...]    [...] : shard data node 
 *    
 ************************************************************************/

    qmgShardSELT * sMyGraph = NULL;
    qmnPlan      * sSDSE    = NULL;

    IDU_FIT_POINT_FATAL( "qmgShardSelect::makePlan::__FT__" );

    //---------------------------------------------------
    // ���ռ� �˻�
    //---------------------------------------------------

    IDE_FT_ASSERT( aStatement != NULL );
    IDE_FT_ASSERT( aParent    != NULL );
    IDE_FT_ASSERT( aGraph     != NULL );

    sMyGraph = (qmgShardSELT*)aGraph;

    //---------------------------------------------------
    // Current CNF�� ���
    //---------------------------------------------------

    if ( sMyGraph->graph.myCNF != NULL )
    {
        sMyGraph->graph.myQuerySet->SFWGH->crtPath->currentCNF = sMyGraph->graph.myCNF;
    }
    else
    {
        // Nothing to do.
    }

    sMyGraph->graph.myPlan = aParent->myPlan;

    // PROJ-1071 Parallel Query
    sMyGraph->graph.flag &= ~QMG_PARALLEL_IMPOSSIBLE_MASK;
    sMyGraph->graph.flag |=  QMG_PARALLEL_IMPOSSIBLE_TRUE;

    //-----------------------
    // init SDSE
    //-----------------------
    IDE_TEST( qmoOneNonPlan::initSDSE( aStatement,
                                       aParent->myPlan,
                                       &sSDSE )
              != IDE_SUCCESS );

    //-----------------------
    // make SDSE
    //-----------------------
    IDE_TEST( qmoOneNonPlan::makeSDSE( aStatement,
                                       aParent->myPlan,
                                       &sMyGraph->shardQuery,
                                       sMyGraph->shardAnalysis,
                                       sMyGraph->shardParamOffset,
                                       sMyGraph->shardParamCount,
                                       &sMyGraph->graph,
                                       sSDSE )
              != IDE_SUCCESS );

    qmg::setPlanInfo( aStatement, sSDSE, &(sMyGraph->graph) );

    sMyGraph->graph.myPlan = sSDSE;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmgShardSelect::printGraph( qcStatement  * aStatement,
                                   qmgGraph     * aGraph,
                                   ULong          aDepth,
                                   iduVarString * aString )
{
/***********************************************************************
 *
 * Description : Graph�� �����ϴ� ���� ������ ����Ѵ�.
 *
 * Implementation :
 *
 ***********************************************************************/

    qmoPredicate * sPredicate = NULL;
    qmoPredicate * sMorePred  = NULL;
    qmgShardSELT * sMyGraph = NULL;
    UInt  i;

    IDU_FIT_POINT_FATAL( "qmgShardSelect::printGraph::__FT__" );

    //-----------------------------------
    // ���ռ� �˻�
    //-----------------------------------

    IDE_FT_ASSERT( aStatement != NULL );
    IDE_FT_ASSERT( aGraph     != NULL );
    IDE_FT_ASSERT( aString    != NULL );

    sMyGraph = (qmgShardSELT *)aGraph;

    //-----------------------------------
    // Graph�� ���� ���
    //-----------------------------------

    if (aDepth == 0)
    {
        QMG_PRINT_LINE_FEED(i, aDepth, aString);
        iduVarStringAppend(aString,
                           "----------------------------------------------------------");
    }
    else
    {
        // Nothing to do.
    }

    //-----------------------------------
    // Graph ���� ������ ���
    //-----------------------------------

    IDE_TEST( qmg::printGraph( aStatement,
                               aGraph,
                               aDepth,
                               aString )
              != IDE_SUCCESS );

    //-----------------------------------
    // Graph ���� ������ ���
    //-----------------------------------

    IDE_TEST( qmoStat::printStat( aGraph->myFrom,
                                  aDepth,
                                  aString )
              != IDE_SUCCESS );

    //-----------------------------------
    // Access method ���� ���
    //-----------------------------------

    QMG_PRINT_LINE_FEED( i, aDepth, aString );
    iduVarStringAppend( aString,
                        "== Access Method Information ==" );

    IDE_TEST( printAccessMethod( aStatement,
                                 sMyGraph->accessMethod,
                                 aDepth,
                                 aString )
              != IDE_SUCCESS );

    //-----------------------------------
    // Subquery Graph ������ ���
    //-----------------------------------

    for ( sPredicate = aGraph->myPredicate;
          sPredicate != NULL;
          sPredicate = sPredicate->next )
    {
        for ( sMorePred = sPredicate;
              sMorePred != NULL;
              sMorePred = sMorePred->more )
        {
            IDE_TEST( qmg::printSubquery( aStatement,
                                          sMorePred->node,
                                          aDepth,
                                          aString )
                      != IDE_SUCCESS );
        }
    }

    for ( sPredicate = aGraph->constantPredicate;
          sPredicate != NULL;
          sPredicate = sPredicate->next )
    {
        for ( sMorePred = sPredicate;
              sMorePred != NULL;
              sMorePred = sMorePred->more )
        {
            IDE_TEST( qmg::printSubquery( aStatement,
                                          sMorePred->node,
                                          aDepth,
                                          aString )
                      != IDE_SUCCESS );
        }
    }

    //-----------------------------------
    // shard ���� ���
    //-----------------------------------

    IDE_TEST( qmgShardDML::printShardInfo( aStatement,
                                           sMyGraph->shardAnalysis,
                                           &(sMyGraph->shardQuery),
                                           aDepth,
                                           aString )
              != IDE_SUCCESS );

    //-----------------------------------
    // Graph�� ������ ���
    //-----------------------------------

    if (aDepth == 0)
    {
        QMG_PRINT_LINE_FEED(i, aDepth, aString);
        iduVarStringAppend(aString,
                           "----------------------------------------------------------\n\n");
    }
    else
    {
        // Nothing to do.
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmgShardSelect::printAccessMethod( qcStatement     * /* aStatement */ ,
                                          qmoAccessMethod * aMethod,
                                          ULong             aDepth,
                                          iduVarString    * aString )
{
    UInt i;

    IDU_FIT_POINT_FATAL( "qmgShardSelect::printAccessMethod::__FT__" );

    IDE_FT_ASSERT( aMethod->method == NULL );

    // FULL SCAN
    QMG_PRINT_LINE_FEED( i, aDepth, aString );
    iduVarStringAppend( aString,
                        "FULL SCAN" );

    QMG_PRINT_LINE_FEED( i, aDepth, aString );
    iduVarStringAppendFormat( aString,
                              "  ACCESS_COST : %"ID_PRINT_G_FMT,
                              aMethod->accessCost );

    QMG_PRINT_LINE_FEED( i, aDepth, aString );
    iduVarStringAppendFormat( aString,
                              "  DISK_COST   : %"ID_PRINT_G_FMT,
                              aMethod->diskCost );

    QMG_PRINT_LINE_FEED( i, aDepth, aString );
    iduVarStringAppendFormat( aString,
                              "  TOTAL_COST  : %"ID_PRINT_G_FMT,
                              aMethod->totalCost );

    return IDE_SUCCESS;
    
}
