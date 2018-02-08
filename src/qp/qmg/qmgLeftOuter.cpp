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
 * $Id: qmgLeftOuter.cpp 82075 2018-01-17 06:39:52Z jina.kim $
 *
 * Description :
 *     LeftOuter Graph�� ���� ���� �Լ�
 *
 * ��� ���� :
 *
 * ��� :
 *
 **********************************************************************/

#include <ide.h>
#include <qmgLeftOuter.h>
#include <qmoOneNonPlan.h>
#include <qmoOneMtrPlan.h>
#include <qmoTwoNonPlan.h>
#include <qmoSelectivity.h>
#include <qmoParallelPlan.h>
#include <qmgJoin.h>
#include <qmoCostDef.h>
#include <qmv.h>

IDE_RC
qmgLeftOuter::init( qcStatement * aStatement,
                    qmsQuerySet * aQuerySet,
                    qmsFrom     * aFrom,
                    qmgGraph   ** aGraph )
{
/***********************************************************************
 *
 * Description : qmgLeftOuter Graph�� �ʱ�ȭ
 *
 * Implementation :
 *    (1)  qmgLeftOuter�� ���� ���� �Ҵ�
 *    (2)  graph( ��� Graph�� ���� ���� �ڷ� ���� ) �ʱ�ȭ
 *    (3)  graph.type ����
 *    (4)  graph.myQuerySet�� aQuerySet���� ����
 *    (5)  graph.myFrom�� aFrom���� ����
 *    (6)  graph.dependencies ����
 *    (7)  qmgJoin�� onConditonCNF ó��
 *    (8)  ����graph�� ���� �� �ʱ�ȭ
 *    (9)  graph.optimize�� graph.makePlan ����
 *    (10) out ����
 *
 ***********************************************************************/

    qmgLOJN   * sMyGraph;
    qtcNode   * sNormalCNF;
    qmoNormalType sNormalType;

    IDU_FIT_POINT_FATAL( "qmgLeftOuter::init::__FT__" );

    //------------------------------------------
    // ���ռ� �˻�
    //------------------------------------------

    IDE_DASSERT( aStatement != NULL );
    IDE_DASSERT( aQuerySet != NULL );
    IDE_DASSERT( aFrom != NULL );

    //---------------------------------------------------
    // Left Outer Join Graph�� ���� �⺻ �ʱ�ȭ
    //---------------------------------------------------

    // qmgLeftOuter�� ���� ���� �Ҵ�
    IDE_TEST( QC_QMP_MEM(aStatement)->alloc( ID_SIZEOF(qmgLOJN),
                                             (void**) & sMyGraph )
              != IDE_SUCCESS );


    // Graph ���� ������ �ʱ�ȭ
    IDE_TEST( qmg::initGraph( & sMyGraph->graph ) != IDE_SUCCESS );

    // Graph�� ���� ǥ��
    sMyGraph->graph.type = QMG_LEFT_OUTER_JOIN;
    qtc::dependencySetWithDep( & sMyGraph->graph.depInfo,
                               & aFrom->depInfo );

    sMyGraph->graph.myFrom = aFrom;
    sMyGraph->graph.myQuerySet = aQuerySet;

    // Graph�� �Լ� �����͸� ����
    sMyGraph->graph.optimize = qmgLeftOuter::optimize;
    sMyGraph->graph.makePlan = qmgLeftOuter::makePlan;
    sMyGraph->graph.printGraph = qmgLeftOuter::printGraph;

    //---------------------------------------------------
    // Left Outer Join Graph ���� ���� �ڷ� ���� �ʱ�ȭ
    //---------------------------------------------------

    // on condition CNF�� ���� ���� �Ҵ� �� �ڷ� ���� �ʱ�ȭ
    IDE_TEST( QC_QMP_MEM(aStatement)->alloc( ID_SIZEOF( qmoCNF ),
                                             (void**) & sMyGraph->onConditionCNF )
              != IDE_SUCCESS );

    // To Fix PR-12743 NNF Filter����
    IDE_TEST( qmoCrtPathMgr::decideNormalType( aStatement,
                                               aFrom,
                                               aFrom->onCondition,
                                               aQuerySet->SFWGH->hints,
                                               ID_TRUE, // CNF Only��
                                               & sNormalType )
              != IDE_SUCCESS );

    switch ( sNormalType )
    {
        case QMO_NORMAL_TYPE_NNF:

            if ( aFrom->onCondition != NULL )
            {
                aFrom->onCondition->lflag &= ~QTC_NODE_NNF_FILTER_MASK;
                aFrom->onCondition->lflag |= QTC_NODE_NNF_FILTER_TRUE;
            }
            else
            {
                // Nothing To Do
            }

            IDE_TEST( qmoCnfMgr::init( aStatement,
                                       sMyGraph->onConditionCNF,
                                       aQuerySet,
                                       aFrom,
                                       NULL,
                                       aFrom->onCondition )
                      != IDE_SUCCESS );
            break;

        case QMO_NORMAL_TYPE_CNF:
            IDE_TEST( qmoNormalForm::normalizeCNF( aStatement,
                                                   aFrom->onCondition,
                                                   & sNormalCNF )
                      != IDE_SUCCESS );

            IDE_TEST( qmoCnfMgr::init( aStatement,
                                       sMyGraph->onConditionCNF,
                                       aQuerySet,
                                       aFrom,
                                       sNormalCNF,
                                       NULL )
                      != IDE_SUCCESS );
            break;

        case QMO_NORMAL_TYPE_DNF:
        case QMO_NORMAL_TYPE_NOT_DEFINED:
        default:
            IDE_FT_ASSERT( 0 );
            break;
    }

    sMyGraph->graph.left = sMyGraph->onConditionCNF->baseGraph[0];
    sMyGraph->graph.right = sMyGraph->onConditionCNF->baseGraph[1];

    // BUG-45296 rownum Pred �� left outer �� ���������� ������ �ȵ˴ϴ�.
    sMyGraph->graph.left->flag &= ~QMG_ROWNUM_PUSHED_MASK;
    sMyGraph->graph.left->flag |= QMG_ROWNUM_PUSHED_TRUE;
    
    sMyGraph->graph.right->flag &= ~QMG_ROWNUM_PUSHED_MASK;
    sMyGraph->graph.right->flag |= QMG_ROWNUM_PUSHED_FALSE;

    // join method �ʱ�ȭ
    sMyGraph->nestedLoopJoinMethod = NULL;
    sMyGraph->sortBasedJoinMethod = NULL;
    sMyGraph->hashBasedJoinMethod = NULL;

    // joinable predicate / non joinable predicate �ʱ�ȭ
    sMyGraph->joinablePredicate = NULL;
    sMyGraph->nonJoinablePredicate = NULL;
    sMyGraph->pushedDownPredicate = NULL;

    // bucket count, hash temp table count �ʱ�ȭ
    sMyGraph->hashBucketCnt = 0;
    sMyGraph->hashTmpTblCnt = 0;

    // Disk/Memory ���� ����
    switch(  aQuerySet->SFWGH->hints->interResultType )
    {
        case QMO_INTER_RESULT_TYPE_NOT_DEFINED :
            // �߰� ��� Type Hint�� ���� ���,
            // left �Ǵ� right�� disk�̸� disk
            if ( ( ( sMyGraph->graph.left->flag & QMG_GRAPH_TYPE_MASK )
                   == QMG_GRAPH_TYPE_DISK ) ||
                 ( ( sMyGraph->graph.right->flag & QMG_GRAPH_TYPE_MASK )
                   == QMG_GRAPH_TYPE_DISK ) )
            {
                sMyGraph->graph.flag &= ~QMG_GRAPH_TYPE_MASK;
                sMyGraph->graph.flag |= QMG_GRAPH_TYPE_DISK;
            }
            else
            {
                sMyGraph->graph.flag &= ~QMG_GRAPH_TYPE_MASK;
                sMyGraph->graph.flag |= QMG_GRAPH_TYPE_MEMORY;
            }
            break;
        case QMO_INTER_RESULT_TYPE_DISK :
            sMyGraph->graph.flag &= ~QMG_GRAPH_TYPE_MASK;
            sMyGraph->graph.flag |= QMG_GRAPH_TYPE_DISK;
            break;
        case QMO_INTER_RESULT_TYPE_MEMORY :
            sMyGraph->graph.flag &= ~QMG_GRAPH_TYPE_MASK;
            sMyGraph->graph.flag |= QMG_GRAPH_TYPE_MEMORY;
            break;
        default :
            IDE_DASSERT( 0 );
            break;
    }

    sMyGraph->joinOrderFactor = 0;
    sMyGraph->joinSize = 0;
    sMyGraph->firstRowsFactor = QMO_COST_FIRST_ROWS_FACTOR_DEFAULT;

    // out ����
    *aGraph = (qmgGraph*)sMyGraph;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;
    
    return IDE_FAILURE;
}

IDE_RC
qmgLeftOuter::optimize( qcStatement * aStatement, qmgGraph * aGraph )
{
/***********************************************************************
 *
 * Description : qmgLeftOuter Graph�� ����ȭ
 *
 * Implementation :
 *    (0) on Condition���� Transitive Predicate ����
 *    (1) on Condition Predicate�� �з�
 *    (2) left graph�� ����ȭ ����
 *    (3) right graph�� ����ȭ ����
 *    (4) subquery�� ó��
 *    (5) Join Method�� �ʱ�ȭ
 *    (6) Join Method�� ����
 *    (7) Join Method ���� �� ó��
 *    (8) ���� ��� ������ ����
 *    (9) Preserved Order, DISK/MEMORY ����
 *
 ***********************************************************************/

    qmgLOJN       * sMyGraph;
    qmoJoinMethod * sJoinMethods;
    qmoPredicate  * sLowerPredicate;
    qmoPredicate  * sPredicate;

    IDU_FIT_POINT_FATAL( "qmgLeftOuter::optimize::__FT__" );

    //------------------------------------------
    // ���ռ� �˻�
    //------------------------------------------

    IDE_DASSERT( aStatement != NULL );
    IDE_DASSERT( aGraph != NULL );

    //------------------------------------------
    // �⺻ �ʱ�ȭ
    //------------------------------------------

    sMyGraph = (qmgLOJN*) aGraph;

    //------------------------------------------
    // CHECK NOT ALLOWED JOIN
    //------------------------------------------
    
    // PROJ-2582 recursive with
    IDE_TEST_RAISE( ( ( sMyGraph->graph.right->myQuerySet->flag &
                        QMV_QUERYSET_RECURSIVE_VIEW_MASK )
                      == QMV_QUERYSET_RECURSIVE_VIEW_RIGHT ) &&
                    ( ( sMyGraph->graph.right->myFrom->tableRef->flag &
                        QMS_TABLE_REF_RECURSIVE_VIEW_MASK )
                      == QMS_TABLE_REF_RECURSIVE_VIEW_TRUE ),
                    ERR_NOT_ALLOWED_JOIN_RECURSIVE_VIEW );

    //------------------------------------------
    // PROJ-1404 Transitive Predicate ����
    //------------------------------------------
    
    IDE_TEST( qmoCnfMgr::generateTransitivePred4OnCondition(
                  aStatement,
                  sMyGraph->onConditionCNF,
                  sMyGraph->graph.myPredicate,
                  & sLowerPredicate )
              != IDE_SUCCESS );
    
    //------------------------------------------
    // on Condition Predicate�� �з�
    //------------------------------------------

    IDE_TEST( qmoCnfMgr::classifyPred4OnCondition(
                  aStatement,
                  sMyGraph->onConditionCNF,
                  & sMyGraph->graph.myPredicate,
                  sLowerPredicate,
                  sMyGraph->graph.myFrom->joinType )
              != IDE_SUCCESS );
    
    // left graph�� ����ȭ ����
    IDE_TEST( sMyGraph->graph.left->optimize( aStatement,
                                              sMyGraph->graph.left )
              != IDE_SUCCESS );

    // right graph�� ����ȭ ����
    IDE_TEST( sMyGraph->graph.right->optimize( aStatement,
                                               sMyGraph->graph.right )
              != IDE_SUCCESS );

    // BUG-32703
    // ����ȭ �ÿ� view�� graph�� �����ȴ�
    // left outer join�� ���� graph�� view�� �ִٸ�
    // ����ȭ ���Ŀ� ( ��, view graph�� ����ȭ �� ���Ŀ� )
    // left outer join�� ���� type ( disk or memory )�� ���� �����ؾ� �Ѵ�.

    // BUG-40191 __OPTIMIZER_DEFAULT_TEMP_TBS_TYPE ��Ʈ�� ����ؾ� �Ѵ�.
    if ( aGraph->myQuerySet->SFWGH->hints->interResultType == QMO_INTER_RESULT_TYPE_NOT_DEFINED )
    {
        if ( ( ( sMyGraph->graph.left->flag & QMG_GRAPH_TYPE_MASK )
               == QMG_GRAPH_TYPE_DISK ) ||
             ( ( sMyGraph->graph.right->flag & QMG_GRAPH_TYPE_MASK )
               == QMG_GRAPH_TYPE_DISK ) )
        {
            sMyGraph->graph.flag &= ~QMG_GRAPH_TYPE_MASK;
            sMyGraph->graph.flag |= QMG_GRAPH_TYPE_DISK;
        }
        else
        {
            sMyGraph->graph.flag &= ~QMG_GRAPH_TYPE_MASK;
            sMyGraph->graph.flag |= QMG_GRAPH_TYPE_MEMORY;
        }
    }
    else
    {
        // nothing to do
    }

    // fix BUG-19203
    IDE_TEST( qmoCnfMgr::optimizeSubQ4OnCondition( aStatement,
                                                   sMyGraph->onConditionCNF )
              != IDE_SUCCESS );

    //------------------------------------------
    // Subquery�� Graph ����
    // - To Fix BUG-10577
    //   Left, Right ����ȭ �Ŀ� subquery graph�� �����ؾ� ������ view�϶�
    //   view ��� ���� �̱������� ������ ����ϴ� ������ �߻����� �ʴ´�.
    //   ( = BUG-9736 )
    //   Predicate �з��� dependencies�� �����ϱ� ������ predicate��
    //   Subquery graph ���� ���� �����ص� �ȴ�.
    //------------------------------------------

    if ( sMyGraph->graph.myPredicate != NULL )
    {
        IDE_TEST( qmoPred::optimizeSubqueries( aStatement,
                                               sMyGraph->graph.myPredicate,
                                               ID_FALSE )  // No KeyRange Tip
                  != IDE_SUCCESS );
    }
    else
    {
        // nothing to do
    }

    //------------------------------------------
    // on Condition CNF�� Join Predicate�� �з�
    //------------------------------------------

    if ( sMyGraph->onConditionCNF->joinPredicate != NULL )
    {
        IDE_TEST(
            qmoPred::classifyJoinPredicate(
                aStatement,
                sMyGraph->onConditionCNF->joinPredicate,
                sMyGraph->graph.left,
                sMyGraph->graph.right,
                & sMyGraph->graph.depInfo )
            != IDE_SUCCESS );

        // To fix BUG-26885
        // DNF�� join�� �Ǵ� ��� materialize�� ���� predicate
        // �� ����Ʃ�� ��ġ�� �ٲ� �� �����Ƿ�
        // join predicate�� �����Ѵ�.
        if( aGraph->myQuerySet->SFWGH->crtPath->currentNormalType
            == QMO_NORMAL_TYPE_DNF )
        {   
            IDE_TEST(qmoPred::copyPredicate4Partition(
                         aStatement,
                         sMyGraph->onConditionCNF->joinPredicate,
                         &sPredicate,
                         0,
                         0,
                         ID_TRUE )
                     != IDE_SUCCESS );
        
            sMyGraph->onConditionCNF->joinPredicate = sPredicate;
        }
        else
        {
            // CNF�̰ų� NNF�� ��� ������ �ʿ䰡 ����.
            // Nothing To Do
        }
    }
    else
    {
        // nothing to do
    }

    //------------------------------------------
    // selectivity ��� 
    //------------------------------------------

    IDE_TEST( qmoSelectivity::setOuterJoinSelectivity(
                  aStatement,
                  aGraph,
                  sMyGraph->graph.myPredicate,
                  sMyGraph->onConditionCNF->joinPredicate,
                  sMyGraph->onConditionCNF->oneTablePredicate,
                  & sMyGraph->graph.costInfo.selectivity )
              != IDE_SUCCESS );

    //------------------------------------------
    // Join Method�� �ʱ�ȭ
    //------------------------------------------

    // Join Method ���� �Ҵ�
    IDE_TEST( QC_QMP_MEM(aStatement)->alloc( ID_SIZEOF( qmoJoinMethod ) * 3,
                                             (void **) &sJoinMethods )
              != IDE_SUCCESS );
    sMyGraph->nestedLoopJoinMethod = & sJoinMethods[0];
    // To Fix BUG-9156
    sMyGraph->hashBasedJoinMethod  = & sJoinMethods[1];
    sMyGraph->sortBasedJoinMethod  = & sJoinMethods[2];

    // nested loop join method�� �ʱ�ȭ
    IDE_TEST( qmoJoinMethodMgr::init( aStatement,
                                      & sMyGraph->graph,
                                      sMyGraph->firstRowsFactor,
                                      sMyGraph->onConditionCNF->joinPredicate,
                                      QMO_JOIN_METHOD_NL,
                                      sMyGraph->nestedLoopJoinMethod )
              != IDE_SUCCESS );

    // hash based join method�� �ʱ�ȭ
    IDE_TEST( qmoJoinMethodMgr::init( aStatement,
                                      & sMyGraph->graph,
                                      sMyGraph->firstRowsFactor,
                                      sMyGraph->onConditionCNF->joinPredicate,
                                      QMO_JOIN_METHOD_HASH,
                                      sMyGraph->hashBasedJoinMethod )
              != IDE_SUCCESS );

    // sort based join method�� �ʱ�ȭ
    IDE_TEST( qmoJoinMethodMgr::init( aStatement,
                                      & sMyGraph->graph,
                                      sMyGraph->firstRowsFactor,
                                      sMyGraph->onConditionCNF->joinPredicate,
                                      QMO_JOIN_METHOD_SORT,
                                      sMyGraph->sortBasedJoinMethod )
              != IDE_SUCCESS );

    //------------------------------------------
    // �� Join Method �� ���� ���� cost�� Join Method�� ����
    //------------------------------------------

    IDE_TEST(
        qmgJoin::selectJoinMethodCost( aStatement,
                                       & sMyGraph->graph,
                                       sMyGraph->onConditionCNF->joinPredicate,
                                       sJoinMethods,
                                       & sMyGraph->selectedJoinMethod )
        != IDE_SUCCESS );

    //------------------------------------------
    // ���� ��� ������ ����
    //------------------------------------------

    // record size ����
    sMyGraph->graph.costInfo.recordSize =
        sMyGraph->graph.left->costInfo.recordSize +
        sMyGraph->graph.right->costInfo.recordSize;

    // �� qmgJoin �� joinOrderFactor, joinSize ���
    IDE_TEST( qmoSelectivity::setJoinOrderFactor(
                  aStatement,
                  & sMyGraph->graph,
                  sMyGraph->onConditionCNF->joinPredicate,
                  &sMyGraph->joinOrderFactor,
                  &sMyGraph->joinSize )
              != IDE_SUCCESS );

    // input record count ����
    sMyGraph->graph.costInfo.inputRecordCnt =
        sMyGraph->graph.left->costInfo.outputRecordCnt *
        sMyGraph->graph.right->costInfo.outputRecordCnt;

    // output record count ����
    IDE_TEST( qmoSelectivity::setLeftOuterOutputCnt(
                  aStatement,
                  aGraph,
                  & sMyGraph->graph.left->depInfo,
                  sMyGraph->graph.myPredicate,
                  sMyGraph->onConditionCNF->joinPredicate,
                  sMyGraph->onConditionCNF->oneTablePredicate,
                  sMyGraph->graph.left->costInfo.outputRecordCnt,
                  sMyGraph->graph.right->costInfo.outputRecordCnt,
                  & sMyGraph->graph.costInfo.outputRecordCnt )
              != IDE_SUCCESS );

    // My Cost ���
    sMyGraph->graph.costInfo.myAccessCost =
        sMyGraph->selectedJoinMethod->accessCost;
    sMyGraph->graph.costInfo.myDiskCost =
        sMyGraph->selectedJoinMethod->diskCost;
    sMyGraph->graph.costInfo.myAllCost =
        sMyGraph->selectedJoinMethod->totalCost;

    // Total Cost ���
    // Join Graph ��ü�� Cost�� �̹� Child�� Cost�� ��� �����ϰ� �ִ�.
    sMyGraph->graph.costInfo.totalAccessCost =
        sMyGraph->graph.costInfo.myAccessCost;
    sMyGraph->graph.costInfo.totalDiskCost =
        sMyGraph->graph.costInfo.myDiskCost;
    sMyGraph->graph.costInfo.totalAllCost =
        sMyGraph->graph.costInfo.myAllCost;

    //------------------------------------------
    // Join Method�� ���� �� ó��
    //------------------------------------------

    IDE_TEST(
        qmgJoin::afterJoinMethodDecision(
            aStatement,
            & sMyGraph->graph,
            sMyGraph->selectedJoinMethod,
            & sMyGraph->onConditionCNF->joinPredicate,
            & sMyGraph->joinablePredicate,
            & sMyGraph->nonJoinablePredicate)
        != IDE_SUCCESS );
    
    // PROJ-1404
    // ������ transitive predicate�� one table predicate�� ���
    // �׻� filter�� ó���ǹǷ� bad transitive predicate�̴�.
    IDE_TEST( qmoPred::removeTransitivePredicate(
                  & sMyGraph->onConditionCNF->oneTablePredicate,
                  ID_FALSE )
              != IDE_SUCCESS );

    //------------------------------------------
    // Preserved Order
    //------------------------------------------

    IDE_TEST( qmgJoin::makePreservedOrder( aStatement,
                                           & sMyGraph->graph,
                                           sMyGraph->selectedJoinMethod,
                                           sMyGraph->joinablePredicate )
              != IDE_SUCCESS );

    // BUG-38279 Applying parallel query
    aGraph->flag &= ~QMG_PARALLEL_EXEC_MASK;
    aGraph->flag |= ((aGraph->left->flag & QMG_PARALLEL_EXEC_MASK) |
                     (aGraph->right->flag & QMG_PARALLEL_EXEC_MASK));

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_NOT_ALLOWED_JOIN_RECURSIVE_VIEW )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMV_NOT_ALLOWED_JOIN_RECURSIVE_VIEW ) );
    }
    IDE_EXCEPTION_END;
    
    return IDE_FAILURE;
}


IDE_RC
qmgLeftOuter::makePlan( qcStatement * aStatement, const qmgGraph * aParent, qmgGraph * aGraph )
{
/***********************************************************************
 *
 * Description : qmgLeftOuter�� ���� Plan�� �����Ѵ�.
 *
 * Implementation :
 *     - qmgLeftOuter�� ���� ���������� Plan
 *
 *     1.  Nested Loop �迭
 *         1.1  Full Nested Loop Join�� ���
 *
 *                 ( [FILT] )  : WHERE ���κ��� ����� Predicate
 *                     |
 *                   [LOJN]
 *                   |    |
 *                        [SCAN]  : SCAN�� ���Ե�
 *
 *         1.2  Full Store Nested Loop Join�� ���
 *
 *                 ( [FILT] )  : WHERE ���κ��� ����� Predicate
 *                     |
 *                   [LOJN]
 *                   |    |
 *                        [SORT]
 *
 *         1.3  Index Nested Loop Join�� ���
 *
 *                 ( [FILT] )  : WHERE ���κ��� ����� Predicate
 *                     |
 *                   [LOJN]
 *                   |    |
 *                        [SCAN]
 *
 *         1.4  Anti-Outer Nested Loop Join�� ���
 *              - qmgFullOuter�κ��͸� ������.
 *
 *     2.  Sort-based �迭
 *
 *                      |
 *                  ( [FILT] )  : WHERE ���κ��� ����� Predicate
 *                      |
 *                    [LOJN]
 *                    |    |
 *              ([SORT])   [SORT]
 *
 *         - Two-Pass Sort Join�� ���, Left�� SORT�� �����ǳ�
 *           Preserved Order�� �ִٸ� Two-Pass Sort Join�̶� �ϴ���
 *           Left�� SORT�� �������� �ʴ´�.
 *
 *     3.  Hash-based �迭
 *
 *                      |
 *                  ( [FILT] )  : WHERE ���κ��� ����� Predicate
 *                      |
 *                    [LOJN]
 *                    |    |
 *              ([HASH])   [HASH]
 *
 *         - Two-Pass Hash Join�� ���, Left�� HASH�� ������
 *
 ***********************************************************************/

    qmgLOJN         * sMyGraph;
    
    IDU_FIT_POINT_FATAL( "qmgLeftOuter::makePlan::__FT__" );

    //------------------------------------------
    // ���ռ� �˻�
    //------------------------------------------

    IDE_DASSERT( aStatement != NULL );
    IDE_DASSERT( aGraph != NULL );

    sMyGraph = (qmgLOJN*) aGraph;

    //---------------------------------------------------
    // Current CNF�� ���
    //---------------------------------------------------
    if ( sMyGraph->graph.myCNF != NULL )
    {
        sMyGraph->graph.myQuerySet->SFWGH->crtPath->currentCNF =
            sMyGraph->graph.myCNF;
    }
    else
    {
        // Nothing To Do
    }

    /* PROJ-1071 Parallel Query */
    aGraph->flag &= ~QMG_PARALLEL_IMPOSSIBLE_MASK;
    aGraph->flag |= (aParent->flag & QMG_PARALLEL_IMPOSSIBLE_MASK);

    // BUG-38410
    // SCAN parallel flag �� �ڽ� ���� �����ش�.
    aGraph->left->flag  |= (aGraph->flag & QMG_PLAN_EXEC_REPEATED_MASK);
    aGraph->right->flag |= (aGraph->flag & QMG_PLAN_EXEC_REPEATED_MASK);

    sMyGraph->graph.myPlan = aParent->myPlan;

    switch( sMyGraph->selectedJoinMethod->flag & QMO_JOIN_METHOD_MASK )
    {
        case QMO_JOIN_METHOD_ONE_PASS_HASH:
            IDE_TEST( makeHashJoin( aStatement,
                                    sMyGraph,
                                    ID_FALSE,   // TwoPass?
                                    ID_FALSE )  // Inverse?
                      != IDE_SUCCESS );
            break;
        case QMO_JOIN_METHOD_INVERSE_HASH:
            IDE_TEST( makeHashJoin( aStatement,
                                    sMyGraph,
                                    ID_FALSE,   // TwoPass?
                                    ID_TRUE )   // Inverse?
                      != IDE_SUCCESS );
            break;
        case QMO_JOIN_METHOD_TWO_PASS_HASH:
            IDE_TEST( makeHashJoin( aStatement,
                                    sMyGraph,
                                    ID_TRUE,    // TwoPass?
                                    ID_FALSE )  // Inverse?
                      != IDE_SUCCESS );
            break;
        case QMO_JOIN_METHOD_ONE_PASS_SORT:
            IDE_TEST( makeSortJoin( aStatement,
                                    sMyGraph,
                                    ID_FALSE )   // TwoPass?
                      != IDE_SUCCESS );
            break;
        case QMO_JOIN_METHOD_TWO_PASS_SORT:
            IDE_TEST( makeSortJoin( aStatement,
                                    sMyGraph,
                                    ID_TRUE )    // TwoPass?
                      != IDE_SUCCESS );
            break;
        case QMO_JOIN_METHOD_FULL_STORE_NL:
            IDE_TEST( makeNestedLoopJoin(
                          aStatement,
                          sMyGraph,
                          (sMyGraph->selectedJoinMethod->flag & 
                           QMO_JOIN_METHOD_MASK) )
                      != IDE_SUCCESS );
            break;
        case QMO_JOIN_METHOD_INDEX:
        case QMO_JOIN_METHOD_FULL_NL:
            IDE_TEST( makeNestedLoopJoin(
                          aStatement,
                          sMyGraph,
                          sMyGraph->selectedJoinMethod->flag & QMO_JOIN_METHOD_MASK )
                      != IDE_SUCCESS );
            break;
        case QMO_JOIN_METHOD_ANTI:
            //����
        default:
            IDE_FT_ASSERT( 0 );
            break;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;
    
    return IDE_FAILURE;
}

IDE_RC
qmgLeftOuter::makeSortRange( qcStatement  * aStatement,
                             qmgLOJN      * aMyGraph,
                             qtcNode     ** aRange )
{
    qtcNode         * sCNFRange;

    IDU_FIT_POINT_FATAL( "qmgLeftOuter::makeSortRange::__FT__" );

    //---------------------------------------------------
    // SORT�� ���� range , HASH�� ���� filter ����
    //---------------------------------------------------
    if( aMyGraph->joinablePredicate != NULL )
    {
        IDE_TEST( qmoPred::linkPredicate( aStatement ,
                                          aMyGraph->joinablePredicate ,
                                          &sCNFRange ) != IDE_SUCCESS );

        // To Fix PR-8019
        // Key Range ������ ���ؼ��� DNF���·� ��ȯ�Ͽ��� �Ѵ�.
        IDE_TEST( qmoNormalForm::normalizeDNF( aStatement ,
                                               sCNFRange ,
                                               aRange ) != IDE_SUCCESS );

        IDE_TEST( qmoPred::setPriorNodeID( aStatement ,
                                           aMyGraph->graph.myQuerySet ,
                                           *aRange ) != IDE_SUCCESS );
    }
    else
    {
        *aRange = NULL;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;
    
    return IDE_FAILURE;
}

IDE_RC
qmgLeftOuter::makeHashFilter( qcStatement  * aStatement,
                              qmgLOJN      * aMyGraph,
                              qtcNode     ** aFilter )
{
    IDU_FIT_POINT_FATAL( "qmgLeftOuter::makeHashFilter::__FT__" );

    IDE_TEST( makeSortRange( aStatement, aMyGraph, aFilter ) != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC
qmgLeftOuter::makeChildPlan( qcStatement     * aStatement,
                             qmgLOJN         * aMyGraph,
                             qmnPlan         * aLeftPlan,
                             qmnPlan         * aRightPlan )
{
    //---------------------------------------------------
    // ���� Plan�� ���� , Join�̹Ƿ� ���� ��� ����
    //---------------------------------------------------

    IDU_FIT_POINT_FATAL( "qmgLeftOuter::makeChildPlan::__FT__" );

    aMyGraph->graph.myPlan = aLeftPlan;

    IDE_TEST( aMyGraph->graph.left->makePlan( aStatement ,
                                              &aMyGraph->graph ,
                                              aMyGraph->graph.left )
              != IDE_SUCCESS);

    aMyGraph->graph.myPlan = aRightPlan;

    IDE_TEST( aMyGraph->graph.right->makePlan( aStatement ,
                                               &aMyGraph->graph ,
                                               aMyGraph->graph.right )
              != IDE_SUCCESS);

    //---------------------------------------------------
    // Process ���� ���� 
    //---------------------------------------------------
    aMyGraph->graph.myQuerySet->processPhase = QMS_MAKEPLAN_JOIN;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;
    
    return IDE_FAILURE;
}

IDE_RC
qmgLeftOuter::makeNestedLoopJoin( qcStatement * aStatement,
                                  qmgLOJN     * aMyGraph,
                                  UInt          aJoinType )
{
    UInt              sMtrFlag;
    UInt              sJoinFlag;
    qmnPlan         * sFILT = NULL;
    qmnPlan         * sLOJN = NULL;
    qmnPlan         * sSORT = NULL;
    qmoPredicate    * sPredicate[3];
    qtcNode         * sFilter = NULL;
    qtcNode         * sJoinFilter= NULL;

    IDU_FIT_POINT_FATAL( "qmgLeftOuter::makeNestedLoopJoin::__FT__" );
    
    // ������ 3���� nested loop join�� �����Ѵ�.
    // 1. Full nested loop join
    // 2. Full store nested loop join
    // 3. Index nested loop join

    //---------------------------------------------------
    //       [*FILT]
    //          |
    //       [LOJN]
    //         /`.
    //   [LEFT]  [*SORT]
    //              |
    //           [RIGHT]
    // * Option
    //---------------------------------------------------

    //-----------------------
    // Top-down �ʱ�ȭ
    //-----------------------

    //-----------------------
    // init FILT
    //-----------------------

    IDE_TEST( initFILT( aStatement,
                        aMyGraph,
                        &sFilter,
                        &sFILT )
              != IDE_SUCCESS );
    aMyGraph->graph.myPlan = sFILT;

    //-----------------------
    // init LOJN
    //-----------------------

    sPredicate[0] = aMyGraph->onConditionCNF->constantPredicate;
    sPredicate[1] = aMyGraph->onConditionCNF->oneTablePredicate;
    if( aJoinType == QMO_JOIN_METHOD_INDEX )
    {
        // Index nested loop join�� ���
        sPredicate[2] = aMyGraph->nonJoinablePredicate;
    }
    else
    {
        // �� �� full nested/full stored nested loop join�� ���
        sPredicate[2] = aMyGraph->onConditionCNF->joinPredicate;
    }

    IDE_TEST(
        qmg::makeOuterJoinFilter( aStatement ,
                                  aMyGraph->graph.myQuerySet ,
                                  sPredicate ,
                                  aMyGraph->onConditionCNF->nnfFilter,
                                  &sJoinFilter ) != IDE_SUCCESS );

    IDE_TEST( qmoTwoNonPlan::initLOJN(
                  aStatement ,
                  aMyGraph->graph.myQuerySet ,
                  NULL,
                  sJoinFilter,
                  aMyGraph->pushedDownPredicate,
                  aMyGraph->graph.myPlan,
                  &sLOJN ) != IDE_SUCCESS);

    aMyGraph->graph.myPlan = sLOJN;

    if( aJoinType == QMO_JOIN_METHOD_FULL_STORE_NL )
    {
        //---------------------------------------------------
        // Full Store Nested Loop Join�� ���
        //---------------------------------------------------

        //-----------------------
        // init SORT
        //-----------------------

        sJoinFlag = 0;

        sJoinFlag &= ~QMO_JOIN_METHOD_RIGHT_STORAGE_MASK;
        sJoinFlag |= (aMyGraph->selectedJoinMethod->flag &
                      QMO_JOIN_METHOD_RIGHT_STORAGE_MASK);

        IDE_TEST( qmg::initRightSORT4Join( aStatement,
                                           &aMyGraph->graph,
                                           sJoinFlag,
                                           ID_FALSE,
                                           NULL,
                                           ID_FALSE,
                                           aMyGraph->graph.myPlan,
                                           &sSORT )
                  != IDE_SUCCESS );

        // BUG-38410
        // Sort �ǹǷ� right �� �ѹ��� ����ǰ� sort �� ������ �����Ѵ�.
        // ���� right �� SCAN �� ���� parallel �� ����Ѵ�.
        aMyGraph->graph.right->flag &= ~QMG_PLAN_EXEC_REPEATED_MASK;
        aMyGraph->graph.right->flag |= QMG_PLAN_EXEC_REPEATED_FALSE;
    }
    else
    {
        sSORT = aMyGraph->graph.myPlan;

        // BUG-38410
        // �ݺ� ���� �� ��� SCAN Parallel �� �����Ѵ�.
        aMyGraph->graph.right->flag &= ~QMG_PLAN_EXEC_REPEATED_MASK;
        aMyGraph->graph.right->flag |= QMG_PLAN_EXEC_REPEATED_TRUE;
    }

    //-----------------------
    // ���� plan ����
    //-----------------------
    IDE_TEST( makeChildPlan( aStatement,
                             aMyGraph,
                             sLOJN,
                             sSORT )
              != IDE_SUCCESS );

    //-----------------------
    // Bottom-up ����
    //-----------------------

    if( aJoinType == QMO_JOIN_METHOD_FULL_STORE_NL )
    {
        //---------------------------------------------------
        // Full Store Nested Loop Join�� ���
        //---------------------------------------------------

        //----------------------------
        // make SORT
        //----------------------------

        sMtrFlag = 0;
        sMtrFlag &= ~QMO_MAKESORT_METHOD_MASK;
        sMtrFlag |= QMO_MAKESORT_STORE;

        sMtrFlag &= ~QMO_MAKESORT_PRESERVED_ORDER_MASK;
        sMtrFlag |= QMO_MAKESORT_PRESERVED_FALSE;

        //���� ��ü�� ����
        sJoinFlag = 0;

        sJoinFlag &= ~QMO_JOIN_METHOD_RIGHT_STORAGE_MASK;
        sJoinFlag |= (aMyGraph->selectedJoinMethod->flag &
                      QMO_JOIN_METHOD_RIGHT_STORAGE_MASK);

        IDE_TEST( qmg::makeRightSORT4Join( aStatement,
                                           &aMyGraph->graph,
                                           sMtrFlag,
                                           sJoinFlag,
                                           ID_FALSE,
                                           aMyGraph->graph.right->myPlan,
                                           sSORT )
                  != IDE_SUCCESS );
    }
    else
    {
        sSORT = aMyGraph->graph.right->myPlan;
    }

    //-----------------------
    // make LOJN
    //-----------------------

    IDE_TEST( qmoTwoNonPlan::makeLOJN(
                  aStatement ,
                  aMyGraph->graph.myQuerySet ,
                  sJoinFilter ,
                  aMyGraph->graph.left->myPlan,
                  sSORT,
                  sLOJN ) != IDE_SUCCESS);
    aMyGraph->graph.myPlan = sLOJN;

    qmg::setPlanInfo( aStatement, sLOJN, &(aMyGraph->graph) );

    switch( aJoinType )
    {
        case QMO_JOIN_METHOD_FULL_NL :
            sLOJN->flag       &= ~QMN_PLAN_JOIN_METHOD_TYPE_MASK;
            sLOJN->flag       |= QMN_PLAN_JOIN_METHOD_FULL_NL;
            break;

        case QMO_JOIN_METHOD_INDEX :
            sLOJN->flag       &= ~QMN_PLAN_JOIN_METHOD_TYPE_MASK;
            sLOJN->flag       |= QMN_PLAN_JOIN_METHOD_INDEX;
            break;

        case QMO_JOIN_METHOD_FULL_STORE_NL :
            sLOJN->flag       &= ~QMN_PLAN_JOIN_METHOD_TYPE_MASK;
            sLOJN->flag       |= QMN_PLAN_JOIN_METHOD_FULL_STORE_NL;
            break;

        default :
            IDE_DASSERT(0);
            break;
    }

    //-----------------------
    // make FILT
    //-----------------------

    IDE_TEST( makeFILT( aStatement,
                        aMyGraph,
                        sFilter,
                        sFILT )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;
    
    return IDE_FAILURE;
}

IDE_RC
qmgLeftOuter::makeHashJoin( qcStatement    * aStatement,
                            qmgLOJN        * aMyGraph,
                            idBool           aIsTwoPass,
                            idBool           aIsInverse )
{
    UInt              sMtrFlag;
    UInt              sJoinFlag;
    qmnPlan         * sFILT = NULL;
    qmnPlan         * sLOJN = NULL;
    qmnPlan         * sLeftHASH = NULL;
    qmnPlan         * sRightHASH = NULL;
    qmoPredicate    * sPredicate[3];
    qtcNode         * sFilter = NULL;
    qtcNode         * sJoinFilter = NULL;
    qtcNode         * sHashFilter = NULL;

    IDU_FIT_POINT_FATAL( "qmgLeftOuter::makeHashJoin::__FT__" );

    //---------------------------------------------------
    //       [*FILT]
    //          |
    //       [LOJN]
    //        /  `.
    //   [*HASH]  [HASH]
    //      |        |
    //   [LEFT]   [RIGHT]
    // * Option
    //---------------------------------------------------

    IDE_TEST( makeHashFilter( aStatement,
                              aMyGraph,
                              &sHashFilter )
              != IDE_SUCCESS );

    //-----------------------
    // Top-down �ʱ�ȭ
    //-----------------------

    //-----------------------
    // init FILT
    //-----------------------

    IDE_TEST( initFILT( aStatement,
                        aMyGraph,
                        &sFilter,
                        &sFILT )
              != IDE_SUCCESS );
    aMyGraph->graph.myPlan = sFILT;

    //-----------------------
    // make LOJN filter
    //-----------------------
    sPredicate[0] = aMyGraph->onConditionCNF->constantPredicate;
    sPredicate[1] = aMyGraph->onConditionCNF->oneTablePredicate;
    sPredicate[2] = aMyGraph->nonJoinablePredicate;
    IDE_TEST(
        qmg::makeOuterJoinFilter( aStatement ,
                                  aMyGraph->graph.myQuerySet ,
                                  sPredicate,
                                  aMyGraph->onConditionCNF->nnfFilter,
                                  &sJoinFilter ) != IDE_SUCCESS);

    //-----------------------
    // init LOJN
    //-----------------------

    IDE_TEST( qmoTwoNonPlan::initLOJN( aStatement ,
                                       aMyGraph->graph.myQuerySet ,
                                       sHashFilter,
                                       sJoinFilter,
                                       aMyGraph->graph.myPredicate,
                                       aMyGraph->graph.myPlan,
                                       &sLOJN ) != IDE_SUCCESS);
    aMyGraph->graph.myPlan = sLOJN;

    //-----------------------
    // init right HASH
    //-----------------------

    IDE_TEST( qmoOneMtrPlan::initHASH(
                  aStatement ,
                  aMyGraph->graph.myQuerySet ,
                  sHashFilter,
                  ID_FALSE, // isLeftHash
                  ID_FALSE, // isDistinct
                  aMyGraph->graph.myPlan,
                  &sRightHASH ) != IDE_SUCCESS);

    // BUG-38410
    // Hash �ǹǷ� right �� �ѹ��� ����ǰ� sort �� ���븸 �����Ѵ�.
    // ���� right �� SCAN �� ���� parallel �� ����Ѵ�.
    aMyGraph->graph.right->flag &= ~QMG_PLAN_EXEC_REPEATED_MASK;
    aMyGraph->graph.right->flag |= QMG_PLAN_EXEC_REPEATED_FALSE;

    if( aIsTwoPass == ID_TRUE )
    {
        // Two pass hash join�� ���

        //-----------------------
        // init left HASH
        //-----------------------

        IDE_TEST( qmoOneMtrPlan::initHASH(
                      aStatement,
                      aMyGraph->graph.myQuerySet,
                      sHashFilter,
                      ID_TRUE,  // isLeftHash
                      ID_FALSE, // isDistinct
                      aMyGraph->graph.myPlan,
                      &sLeftHASH )
                  != IDE_SUCCESS );

        // BUG-38410
        // Hash �ǹǷ� left �� �ѹ��� ����ǰ� hash �� ���븸 �����Ѵ�.
        // ���� left �� SCAN �� ���� parallel �� ����Ѵ�.
        aMyGraph->graph.left->flag &= ~QMG_PLAN_EXEC_REPEATED_MASK;
        aMyGraph->graph.left->flag |= QMG_PLAN_EXEC_REPEATED_FALSE;
    }
    else
    {
        // One pass hash join�� ���
        // left child�� parent�� join�� �����Ѵ�.
        sLeftHASH = aMyGraph->graph.myPlan;
    }

    //-----------------------
    // ���� plan ����
    //-----------------------

    IDE_TEST( makeChildPlan( aStatement,
                             aMyGraph,
                             sLeftHASH,
                             sRightHASH )
              != IDE_SUCCESS );

    //-----------------------
    // Bottom-up ����
    //-----------------------

    if( aIsTwoPass == ID_TRUE )
    {
        //-----------------------
        // make left HASH
        //-----------------------

        sMtrFlag = 0;
        sMtrFlag &= ~QMO_MAKEHASH_METHOD_MASK;
        sMtrFlag |= QMO_MAKEHASH_HASH_BASED_JOIN;

        // To Fix PR-8032
        // HASH�� ���Ǵ� ��ġ�� ǥ���ؾ���.
        sMtrFlag &= ~QMO_MAKEHASH_POSITION_MASK;
        sMtrFlag |= QMO_MAKEHASH_POSITION_LEFT;

        //���� ��ü�� ����
        sJoinFlag = 0;

        sJoinFlag &= ~QMO_JOIN_METHOD_LEFT_STORAGE_MASK;
        sJoinFlag |= (aMyGraph->selectedJoinMethod->flag &
                      QMO_JOIN_METHOD_LEFT_STORAGE_MASK);

        IDE_TEST( qmg::makeLeftHASH4Join( aStatement,
                                          &aMyGraph->graph,
                                          sMtrFlag,
                                          sJoinFlag,
                                          sHashFilter,
                                          aMyGraph->graph.left->myPlan,
                                          sLeftHASH )
                  != IDE_SUCCESS);
    }
    else
    {
        // One pass hash join�� ���
        // Join�� left child�� left graph�� plan�� �����Ѵ�.
        sLeftHASH = aMyGraph->graph.left->myPlan;
    }

    //-----------------------
    // make right HASH
    //-----------------------

    sMtrFlag = 0;
    sMtrFlag &= ~QMO_MAKEHASH_METHOD_MASK;
    sMtrFlag |= QMO_MAKEHASH_HASH_BASED_JOIN;

    // To Fix PR-8032
    // HASH�� ���Ǵ� ��ġ�� ǥ���ؾ���.
    sMtrFlag &= ~QMO_MAKEHASH_POSITION_MASK;
    sMtrFlag |= QMO_MAKEHASH_POSITION_RIGHT;

    //���� ��ü�� ����
    sJoinFlag = 0;

    sJoinFlag &= ~QMO_JOIN_METHOD_RIGHT_STORAGE_MASK;
    sJoinFlag |= (aMyGraph->selectedJoinMethod->flag &
                  QMO_JOIN_METHOD_RIGHT_STORAGE_MASK);

    IDE_TEST( qmg::makeRightHASH4Join( aStatement,
                                       &aMyGraph->graph,
                                       sMtrFlag,
                                       sJoinFlag,
                                       sHashFilter,
                                       aMyGraph->graph.right->myPlan,
                                       sRightHASH )
              != IDE_SUCCESS);

    //-----------------------
    // make LOJN
    //-----------------------

    IDE_TEST( qmoTwoNonPlan::makeLOJN( aStatement ,
                                       aMyGraph->graph.myQuerySet ,
                                       sJoinFilter ,
                                       sLeftHASH,
                                       sRightHASH,
                                       sLOJN ) != IDE_SUCCESS);
    aMyGraph->graph.myPlan = sLOJN;

    qmg::setPlanInfo( aStatement, sLOJN, &(aMyGraph->graph) );

    if( aIsTwoPass == ID_TRUE )
    {
        sLOJN->flag       &= ~QMN_PLAN_JOIN_METHOD_TYPE_MASK;
        sLOJN->flag       |= QMN_PLAN_JOIN_METHOD_TWO_PASS_HASH;
    }
    else
    {
        /* PROJ-2339 */
        if ( aIsInverse == ID_TRUE )
        {
            sLOJN->flag       &= ~QMN_PLAN_JOIN_METHOD_TYPE_MASK;
            sLOJN->flag       |= QMN_PLAN_JOIN_METHOD_INVERSE_HASH;
        }
        else
        {
            sLOJN->flag       &= ~QMN_PLAN_JOIN_METHOD_TYPE_MASK;
            sLOJN->flag       |= QMN_PLAN_JOIN_METHOD_ONE_PASS_HASH;
        }
    }

    //-----------------------
    // make FILT
    //-----------------------

    IDE_TEST( makeFILT( aStatement,
                        aMyGraph,
                        sFilter,
                        sFILT )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;
    
    return IDE_FAILURE;
}

IDE_RC
qmgLeftOuter::makeSortJoin( qcStatement    * aStatement,
                            qmgLOJN        * aMyGraph,
                            idBool           aIsTwoPass )
{
    UInt              sMtrFlag;
    UInt              sJoinFlag;
    qmnPlan         * sFILT = NULL;
    qmnPlan         * sLOJN = NULL;
    qmnPlan         * sLeftSORT = NULL;
    qmnPlan         * sRightSORT = NULL;
    qmoPredicate    * sPredicate[3];
    qtcNode         * sFilter = NULL;
    qtcNode         * sJoinFilter = NULL;
    qtcNode         * sSortRange = NULL;

    IDU_FIT_POINT_FATAL( "qmgLeftOuter::makeSortJoin::__FT__" );

    //---------------------------------------------------
    //       [*FILT]
    //          |
    //       [LOJN]
    //        /  `.
    //   [*SORT]  [SORT]
    //      |        |
    //   [LEFT]   [RIGHT]
    // * Option
    //---------------------------------------------------

    IDE_TEST( makeSortRange( aStatement,
                             aMyGraph,
                             &sSortRange )
              != IDE_SUCCESS );

    //-----------------------
    // Top-down �ʱ�ȭ
    //-----------------------

    //-----------------------
    // init FILT
    //-----------------------

    IDE_TEST( initFILT( aStatement,
                        aMyGraph,
                        &sFilter,
                        &sFILT )
              != IDE_SUCCESS );
    aMyGraph->graph.myPlan = sFILT;

    //-----------------------
    // make LOJN filter
    //-----------------------
    sPredicate[0] = aMyGraph->onConditionCNF->constantPredicate;
    sPredicate[1] = aMyGraph->onConditionCNF->oneTablePredicate;
    sPredicate[2] = aMyGraph->nonJoinablePredicate;
    IDE_TEST(
        qmg::makeOuterJoinFilter( aStatement ,
                                  aMyGraph->graph.myQuerySet ,
                                  sPredicate,
                                  aMyGraph->onConditionCNF->nnfFilter,
                                  &sJoinFilter ) != IDE_SUCCESS);

    //-----------------------
    // init LOJN
    //-----------------------

    IDE_TEST( qmoTwoNonPlan::initLOJN( aStatement ,
                                       aMyGraph->graph.myQuerySet ,
                                       sSortRange,
                                       sJoinFilter,
                                       aMyGraph->graph.myPredicate,
                                       aMyGraph->graph.myPlan,
                                       &sLOJN ) != IDE_SUCCESS);
    aMyGraph->graph.myPlan = sLOJN;

    //-----------------------
    // init right SORT
    //-----------------------

    sJoinFlag = 0;

    sJoinFlag &= ~QMO_JOIN_METHOD_RIGHT_STORAGE_MASK;
    sJoinFlag |= (aMyGraph->selectedJoinMethod->flag &
                  QMO_JOIN_METHOD_RIGHT_STORAGE_MASK);

    sJoinFlag &= ~QMO_JOIN_RIGHT_NODE_MASK;
    sJoinFlag |= (aMyGraph->selectedJoinMethod->flag &
                  QMO_JOIN_RIGHT_NODE_MASK);

    IDE_TEST( qmg::initRightSORT4Join( aStatement,
                                       &aMyGraph->graph,
                                       sJoinFlag,
                                       ID_TRUE,
                                       sSortRange,
                                       ID_TRUE,
                                       aMyGraph->graph.myPlan,
                                       &sRightSORT )
              != IDE_SUCCESS);

    if( aIsTwoPass == ID_TRUE )
    {
        // Two pass sort join�� ���

        //-----------------------
        // init left SORT
        //-----------------------
        sJoinFlag = 0;

        sJoinFlag &= ~QMO_JOIN_METHOD_LEFT_STORAGE_MASK;
        sJoinFlag |= (aMyGraph->selectedJoinMethod->flag &
                      QMO_JOIN_METHOD_LEFT_STORAGE_MASK);

        sJoinFlag &= ~QMO_JOIN_LEFT_NODE_MASK;
        sJoinFlag |= (aMyGraph->selectedJoinMethod->flag &
                      QMO_JOIN_LEFT_NODE_MASK);

        IDE_TEST( qmg::initLeftSORT4Join( aStatement,
                                          &aMyGraph->graph,
                                          sJoinFlag,
                                          sSortRange,
                                          aMyGraph->graph.myPlan,
                                          &sLeftSORT )
                  != IDE_SUCCESS);

        // BUG-38410
        // Sort �ǹǷ� left �� �ѹ��� ����ǰ� sort �� ���븸 �����Ѵ�.
        // ���� left �� SCAN �� ���� parallel �� ����Ѵ�.
        aMyGraph->graph.left->flag &= ~QMG_PLAN_EXEC_REPEATED_MASK;
        aMyGraph->graph.left->flag |= QMG_PLAN_EXEC_REPEATED_FALSE;
    }
    else
    {
        // One pass sort join�� ���
        // left child�� parent�� join�� �����Ѵ�.
        sLeftSORT = aMyGraph->graph.myPlan;
    }

    // BUG-38410
    // Sort �ǹǷ� right �� �ѹ��� ����ǰ� sort �� ���븸 �����Ѵ�.
    // ���� right �� SCAN �� ���� parallel �� ����Ѵ�.
    aMyGraph->graph.right->flag &= ~QMG_PLAN_EXEC_REPEATED_MASK;
    aMyGraph->graph.right->flag |= QMG_PLAN_EXEC_REPEATED_FALSE;

    //-----------------------
    // ���� plan ����
    //-----------------------

    IDE_TEST( makeChildPlan( aStatement,
                             aMyGraph,
                             sLeftSORT,
                             sRightSORT )
              != IDE_SUCCESS );

    //-----------------------
    // Bottom-up ����
    //-----------------------

    if( aIsTwoPass == ID_TRUE )
    {
        //-----------------------
        // make left SORT
        //-----------------------

        sMtrFlag = 0;

        sMtrFlag &= ~QMO_MAKESORT_METHOD_MASK;
        sMtrFlag |= QMO_MAKESORT_SORT_BASED_JOIN;

        sMtrFlag &= ~QMO_MAKESORT_POSITION_MASK;
        sMtrFlag |= QMO_MAKESORT_POSITION_LEFT;

        //���� ��ü�� ����
        sJoinFlag = 0;

        sJoinFlag &= ~QMO_JOIN_METHOD_LEFT_STORAGE_MASK;
        sJoinFlag |= (aMyGraph->selectedJoinMethod->flag &
                      QMO_JOIN_METHOD_LEFT_STORAGE_MASK);

        sJoinFlag &= ~QMO_JOIN_LEFT_NODE_MASK;
        sJoinFlag |= (aMyGraph->selectedJoinMethod->flag &
                      QMO_JOIN_LEFT_NODE_MASK);

        IDE_TEST( qmg::makeLeftSORT4Join( aStatement,
                                          &aMyGraph->graph,
                                          sMtrFlag,
                                          sJoinFlag,
                                          aMyGraph->graph.left->myPlan,
                                          sLeftSORT )
                  != IDE_SUCCESS);
    }
    else
    {
        // One pass sort join�� ���
        // Join�� left child�� left graph�� plan�� �����Ѵ�.
        sLeftSORT = aMyGraph->graph.left->myPlan;
    }

    //-----------------------
    // make right SORT
    //-----------------------

    sMtrFlag = 0;

    sMtrFlag &= ~QMO_MAKESORT_METHOD_MASK;
    sMtrFlag |= QMO_MAKESORT_SORT_BASED_JOIN;

    sMtrFlag &= ~QMO_MAKESORT_POSITION_MASK;
    sMtrFlag |= QMO_MAKESORT_POSITION_RIGHT;

    //���� ��ü�� ����
    sJoinFlag = 0;

    sJoinFlag &= ~QMO_JOIN_METHOD_RIGHT_STORAGE_MASK;
    sJoinFlag |= (aMyGraph->selectedJoinMethod->flag &
                  QMO_JOIN_METHOD_RIGHT_STORAGE_MASK);

    sJoinFlag &= ~QMO_JOIN_RIGHT_NODE_MASK;
    sJoinFlag |= (aMyGraph->selectedJoinMethod->flag &
                  QMO_JOIN_RIGHT_NODE_MASK);

    IDE_TEST( qmg::makeRightSORT4Join( aStatement,
                                       &aMyGraph->graph,
                                       sMtrFlag,
                                       sJoinFlag,
                                       ID_TRUE,
                                       aMyGraph->graph.right->myPlan,
                                       sRightSORT )
              != IDE_SUCCESS);

    //-----------------------
    // make LOJN
    //-----------------------

    IDE_TEST( qmoTwoNonPlan::makeLOJN( aStatement ,
                                       aMyGraph->graph.myQuerySet ,
                                       sJoinFilter ,
                                       sLeftSORT,
                                       sRightSORT,
                                       sLOJN ) != IDE_SUCCESS);
    aMyGraph->graph.myPlan = sLOJN;

    qmg::setPlanInfo( aStatement, sLOJN, &(aMyGraph->graph) );

    if( aIsTwoPass == ID_TRUE )
    {
        sLOJN->flag       &= ~QMN_PLAN_JOIN_METHOD_TYPE_MASK;
        sLOJN->flag       |= QMN_PLAN_JOIN_METHOD_TWO_PASS_SORT;
    }
    else
    {
        sLOJN->flag       &= ~QMN_PLAN_JOIN_METHOD_TYPE_MASK;
        sLOJN->flag       |= QMN_PLAN_JOIN_METHOD_ONE_PASS_SORT;
    }

    //-----------------------
    // make FILT
    //-----------------------

    IDE_TEST( makeFILT( aStatement,
                        aMyGraph,
                        sFilter,
                        sFILT )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;
    
    return IDE_FAILURE;

}

IDE_RC
qmgLeftOuter::initFILT( qcStatement  * aStatement,
                        qmgLOJN      * aMyGraph,
                        qtcNode     ** aFilter,
                        qmnPlan     ** aFILT )
{
    IDU_FIT_POINT_FATAL( "qmgLeftOuter::initFILT::__FT__" );

    // fix BUG-9791 BUG-10419
    // constant filter�� �����ϰų�
    // myPredicate�� �����ϴ� ���, ��� FILT node�� �����Ǿ�� �Ѵ�.
    if( ( aMyGraph->graph.myPredicate != NULL ) ||
        ( aMyGraph->graph.constantPredicate != NULL ) ||
        ( aMyGraph->graph.nnfFilter != NULL ) )
    {
        if( aMyGraph->graph.myPredicate != NULL )
        {
            IDE_TEST( qmoPred::linkFilterPredicate(
                          aStatement ,
                          aMyGraph->graph.myPredicate ,
                          aFilter ) != IDE_SUCCESS );

            // BUG-35155 Partial CNF
            if ( aMyGraph->graph.nnfFilter != NULL )
            {
                IDE_TEST( qmoPred::addNNFFilter4linkedFilter(
                              aStatement,
                              aMyGraph->graph.nnfFilter,
                              aFilter )
                          != IDE_SUCCESS );
            }
            else
            {
                // Nothing to do.
            }
        }
        else
        {
            if ( aMyGraph->graph.nnfFilter != NULL )
            {
                *aFilter = aMyGraph->graph.nnfFilter;
            }
            else
            {
                *aFilter = NULL;
            }
        }

        if ( *aFilter != NULL )
        {
            IDE_TEST( qmoPred::setPriorNodeID(
                          aStatement ,
                          aMyGraph->graph.myQuerySet ,
                          *aFilter ) != IDE_SUCCESS );
        }
        else
        {
            *aFilter = NULL;
        }

        //make FILT
        IDE_TEST( qmoOneNonPlan::initFILT(
                      aStatement ,
                      aMyGraph->graph.myQuerySet ,
                      *aFilter ,
                      aMyGraph->graph.myPlan,
                      aFILT) != IDE_SUCCESS);
    }
    else
    {
        *aFILT = aMyGraph->graph.myPlan;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;
    
    return IDE_FAILURE;
}

IDE_RC
qmgLeftOuter::makeFILT( qcStatement * aStatement,
                        qmgLOJN     * aMyGraph,
                        qtcNode     * aFilter,
                        qmnPlan     * aFILT )
{
    IDU_FIT_POINT_FATAL( "qmgLeftOuter::makeFILT::__FT__" );

    if( ( aMyGraph->graph.myPredicate != NULL ) ||
        ( aMyGraph->graph.constantPredicate != NULL ) ||
        ( aMyGraph->graph.nnfFilter != NULL ) )
    {

        IDE_TEST( qmoOneNonPlan::makeFILT(
                      aStatement,
                      aMyGraph->graph.myQuerySet,
                      aFilter,
                      aMyGraph->graph.constantPredicate,
                      aMyGraph->graph.myPlan,
                      aFILT) != IDE_SUCCESS );
        aMyGraph->graph.myPlan = aFILT;

        qmg::setPlanInfo( aStatement, aFILT, &(aMyGraph->graph) );
    }
    else
    {
        // Nothing to do.
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;
    
    return IDE_FAILURE;
}

IDE_RC
qmgLeftOuter::printGraph( qcStatement  * aStatement,
                          qmgGraph     * aGraph,
                          ULong          aDepth,
                          iduVarString * aString )
{
/***********************************************************************
 *
 * Description :
 *    Graph�� �����ϴ� ���� ������ ����Ѵ�.
 *
 *
 * Implementation :
 *
 ***********************************************************************/

    UInt  i;

    qmgLOJN * sMyGraph;

    qmoPredicate * sPredicate;
    qmoPredicate * sMorePred;

    IDU_FIT_POINT_FATAL( "qmgLeftOuter::printGraph::__FT__" );

    //-----------------------------------
    // ���ռ� �˻�
    //-----------------------------------

    IDE_DASSERT( aStatement != NULL );
    IDE_DASSERT( aGraph != NULL );
    IDE_DASSERT( aString != NULL );

    sMyGraph = (qmgLOJN*) aGraph;

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

    //-----------------------------------
    // Join Method ������ ���
    //-----------------------------------

    QMG_PRINT_LINE_FEED( i, aDepth, aString );
    iduVarStringAppend( aString,
                        "== Join Method Information ==" );

    IDE_TEST(
        qmoJoinMethodMgr::printJoinMethod( sMyGraph->nestedLoopJoinMethod,
                                           aDepth,
                                           aString )
        != IDE_SUCCESS );

    IDE_TEST(
        qmoJoinMethodMgr::printJoinMethod( sMyGraph->sortBasedJoinMethod,
                                           aDepth,
                                           aString)
        != IDE_SUCCESS );

    IDE_TEST(
        qmoJoinMethodMgr::printJoinMethod( sMyGraph->hashBasedJoinMethod,
                                           aDepth,
                                           aString )
        != IDE_SUCCESS );

    //-----------------------------------
    // Subquery Graph ������ ���
    //-----------------------------------

    for ( sPredicate = sMyGraph->joinablePredicate;
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

    for ( sPredicate = sMyGraph->joinablePredicate;
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

    for ( sPredicate = sMyGraph->graph.myPredicate;
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
    // Child Graph ���� ������ ���
    //-----------------------------------

    IDE_TEST( aGraph->left->printGraph( aStatement,
                                        aGraph->left,
                                        aDepth + 1,
                                        aString )
              != IDE_SUCCESS );

    IDE_TEST( aGraph->right->printGraph( aStatement,
                                         aGraph->right,
                                         aDepth + 1,
                                         aString )
              != IDE_SUCCESS );

    return IDE_SUCCESS;
    
    IDE_EXCEPTION_END;
        
    return IDE_FAILURE;
}
