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
 * $Id: qtcSubquery.cpp 82186 2018-02-05 05:17:56Z lswhh $
 *
 * Description :
 *
 *     Subquery ������ �����ϴ� Node
 *     One Column �� Subquery �� List(Multi-Column)�� Subquery��
 *     �����Ͽ� ó���Ͽ��� �Ѵ�.
 *
 * ��� ���� :
 *
 * ��� :
 *
 **********************************************************************/

#include <idl.h>
#include <mte.h>
#include <qtc.h>
#include <qmv.h>
#include <qmvQuerySet.h>
#include <qmvQTC.h>
#include <qmn.h>
#include <qcuSqlSourceInfo.h>
#include <qcuProperty.h>
#include <qtcCache.h>
#include <qcgPlan.h>

extern mtdModule mtdList;

static IDE_RC qtcSubqueryEstimate( mtcNode*     aNode,
                                   mtcTemplate* aTemplate,
                                   mtcStack*    aStack,
                                   SInt         aRemain,
                                   mtcCallBack* aCallBack );

/* one column, single-row unknown */
static IDE_RC qtcSubqueryCalculateUnknown( mtcNode*     aNode,
                                           mtcStack*    aStack,
                                           SInt         aRemain,
                                           void*        aInfo,
                                           mtcTemplate* aTemplate );

/* one column, single-row sure */
static IDE_RC qtcSubqueryCalculateSure( mtcNode*     aNode,
                                        mtcStack*    aStack,
                                        SInt         aRemain,
                                        void*        aInfo,
                                        mtcTemplate* aTemplate );

/* list subquery, single-row sure */
static IDE_RC qtcSubqueryCalculateListSure( mtcNode*     aNode,
                                            mtcStack*    aStack,
                                            SInt         aRemain,
                                            void*        aInfo,
                                            mtcTemplate* aTemplate );

/* list subquery, single-row unknown, too many targets */
static IDE_RC qtcSubqueryCalculateListTwice( mtcNode*     aNode,
                                             mtcStack*    aStack,
                                             SInt         aRemain,
                                             void*        aInfo,
                                             mtcTemplate* aTemplate );

static idBool qtcSubqueryIs1RowSure(qcStatement* aStatement);

/* Subquery �������� �̸��� ���� ���� */
static mtcName mtfFunctionName[1] = {
    { NULL, 8, (void*)"SUBQUERY" }
};

/* Subquery �������� Module �� ���� ���� */
mtfModule qtc::subqueryModule = {
    1|                          // �ϳ��� Column ����
    MTC_NODE_OPERATOR_SUBQUERY | MTC_NODE_HAVE_SUBQUERY_TRUE, // Subquery ������
    ~(MTC_NODE_INDEX_MASK),     // Indexable Mask : Column ������ �ƴ�.
    1.0,                        // default selectivity (�� ������ �ƴ�)
    mtfFunctionName,            // �̸� ����
    NULL,                       // Counter ������ ����
    mtf::initializeDefault,     // ���� ������ �ʱ�ȭ �Լ�, ����
    mtf::finalizeDefault,       // ���� ����� ���� �Լ�, ����
    qtcSubqueryEstimate         // Estimate �� �Լ�
};

static IDE_RC qtcSubqueryEstimate( mtcNode     * aNode,
                                   mtcTemplate * aTemplate,
                                   mtcStack    * aStack,
                                   SInt          aRemain,
                                   mtcCallBack * aCallBack )
{
/***********************************************************************
 *
 * Description :
 *    Subquery �����ڿ� ���Ͽ� Estimate �� ������.
 *    Subquery Node�� ���� Column ���� �� Execute ������ Setting�Ѵ�.
 *
 * Implementation :
 *
 *    Subquery�� Target Column�� ������ ����,
 *    One-Column / List Subquery �� ���θ� �Ǵ��ϰ�, �̿� ����
 *    ó���� �����Ѵ�.
 *
 ***********************************************************************/

    UInt                 sCount;
    UInt                 sFence;
    SInt                 sRemain;
    idBool               sIs1RowSure;

    mtcStack*            sStack;
    mtcStack*            sStack2;
    mtcNode*             sNode;
    mtcNode*             sConvertedNode;

    qcTemplate         * sTemplate;
    qcStatement        * sStatement;
    qtcCallBackInfo    * sCallBackInfo;
    qtcNode            * sQtcNode;
    qmsTarget*           sTarget;
    qcuSqlSourceInfo     sqlInfo;
    qmsQuerySet        * sQuerySet = NULL;
    idBool               sIsShardView = ID_FALSE;

    sCallBackInfo    = (qtcCallBackInfo*)((mtcCallBack*)aCallBack)->info;
    sTemplate        = sCallBackInfo->tmplate;
    sStack           = sTemplate->tmplate.stack;
    sRemain          = sTemplate->tmplate.stackRemain;
    sStatement       = ((qtcNode*)aNode)->subquery;

    sTemplate->tmplate.stack       = aStack + 1;
    sTemplate->tmplate.stackRemain = aRemain - 1;

    IDE_TEST_RAISE(sTemplate->tmplate.stackRemain < 1, ERR_STACK_OVERFLOW);

    // set member of qcStatement
    sStatement->myPlan->parseTree->stmtKind = QCI_STMT_SELECT;

    // BUG-41104 statement �� null �϶��� skip �ص� �ȴ�.
    // validate �ܰ迡�� �̹� �����Ǿ� �ִ�.
    if ( sCallBackInfo->statement != NULL )
    {
        sStatement->myPlan->planEnv = sCallBackInfo->statement->myPlan->planEnv;
        sStatement->spvEnv          = sCallBackInfo->statement->spvEnv;
        sStatement->mRandomPlanInfo = sCallBackInfo->statement->mRandomPlanInfo;

        /* PROJ-2197 PSM Renewal */
        sStatement->calledByPSMFlag = sCallBackInfo->statement->calledByPSMFlag;
    }
    else
    {
        // nothing todo
    }

    if( sCallBackInfo->SFWGH != NULL )
    {
        ((qmsParseTree*)sStatement->myPlan->parseTree)->querySet->flag =
            sCallBackInfo->SFWGH->flag;
    }
    else
    {
        ((qmsParseTree*)sStatement->myPlan->parseTree)->querySet->flag = 0;
    }

    // BUG-20272
    ((qmsParseTree*)sStatement->myPlan->parseTree)->querySet->flag &=
        ~QMV_QUERYSET_SUBQUERY_MASK;
    ((qmsParseTree*)sStatement->myPlan->parseTree)->querySet->flag |=
        QMV_QUERYSET_SUBQUERY_TRUE;

    // BUG-41311
    IDE_TEST( qmv::validateLoop( sStatement ) != IDE_SUCCESS );

    // shard view�� ���� statement������ shard table�� �� �� �ִ�.
    if ( sCallBackInfo->statement != NULL )
    {
        if ( ( ( sTemplate->flag & QC_TMP_SHARD_TRANSFORM_MASK )
               == QC_TMP_SHARD_TRANSFORM_ENABLE ) &&
             ( sCallBackInfo->statement->myPlan->parseTree->stmtShard
               != QC_STMT_SHARD_NONE ) )
        {
            sTemplate->flag &= ~QC_TMP_SHARD_TRANSFORM_MASK;
            sTemplate->flag |= QC_TMP_SHARD_TRANSFORM_DISABLE;

            sIsShardView = ID_TRUE;
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

    // Query Set�� ���� Validation ����
    IDE_TEST( qmvQuerySet::validate(
                  sStatement,
                  ((qmsParseTree*)sStatement->myPlan->parseTree)->querySet,
                  sCallBackInfo->SFWGH, sCallBackInfo->from,
                  0)
              != IDE_SUCCESS );

    sStatement->calledByPSMFlag = ID_FALSE;

    if ( sIsShardView == ID_TRUE )
    {
        sTemplate->flag &= ~QC_TMP_SHARD_TRANSFORM_MASK;
        sTemplate->flag |= QC_TMP_SHARD_TRANSFORM_ENABLE;
    }
    else
    {
        // Nothing to do.
    }

    // PROJ-2462 Result Cache
    // SubQuery�� Reulst Cache���ʾ��������� ���� QuerySet�� ǥ���Ѵ�.
    if ( sCallBackInfo->querySet != NULL )
    {
        sCallBackInfo->querySet->flag |= ((qmsParseTree*)sStatement->myPlan->parseTree)->querySet->flag
                                         & QMV_QUERYSET_RESULT_CACHE_INVALID_MASK;
    }
    else
    {
        /* Nothing to do */
    }

    // BUG-33225
    if( ((qmsParseTree*)sStatement->myPlan->parseTree)->limit != NULL )
    {
        IDE_TEST( qmv::validateLimit( sStatement,
                                      ((qmsParseTree*)sStatement->myPlan->parseTree)->limit )
                  != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    // Subquery���� Sequence�� ����� �� ����
    if (sStatement->myPlan->parseTree->currValSeqs != NULL)
    {
        sqlInfo.setSourceInfo(
            sStatement,
            & sStatement->myPlan->parseTree->
            currValSeqs->sequenceNode->position );
        IDE_RAISE(ERR_USE_SEQUENCE_IN_SUBQUERY);
    }

    if (sStatement->myPlan->parseTree->nextValSeqs != NULL)
    {
        sqlInfo.setSourceInfo(
            sStatement,
            & sStatement->myPlan->parseTree->
            nextValSeqs->sequenceNode->position );
        IDE_RAISE(ERR_USE_SEQUENCE_IN_SUBQUERY);
    }

    // Subquery Node �� ���� dependencies�� ������
    // set dependencies
    qtc::dependencySetWithDep(
        & ((qtcNode *)aNode)->depInfo,
        & ((qmsParseTree*)sStatement->myPlan->parseTree)->querySet->outerDepInfo );

    // Subquery Node�� ���� Argument�� ������
    // ��, Subquery Target�� Subquery Node�� argument�� �ȴ�.
    sTarget = ((qmsParseTree*)sStatement->myPlan->parseTree)->querySet->target;

    aNode->arguments = (mtcNode*)sTarget->targetColumn;

    // Target Column�� ������ ����
    // one-column subquery����, List Subquery������ �Ǵ��Ѵ�.
    for ( sNode   = aNode->arguments, sFence  = 0;
          sNode  != NULL;
          sNode   = sNode->next, sFence++ )
    {
        // BUG-36258
        if ((((qtcNode*)sNode)->lflag & QTC_NODE_COLUMN_RID_MASK) ==
            QTC_NODE_COLUMN_RID_EXIST)
        {
            IDE_RAISE(ERR_PROWID_NOT_SUPPORTED);
        }
    }

    IDE_TEST_RAISE( sFence > MTC_NODE_ARGUMENT_COUNT_MAXIMUM,
                    ERR_INVALID_FUNCTION_ARGUMENT );

    // Subquery Node�� Argument�� ������ ������
    aNode->lflag &= ~(MTC_NODE_ARGUMENT_COUNT_MASK);
    aNode->lflag |= sFence;

    // To fix PR-7904
    // Subquery�� �������� ������
    sQtcNode = (qtcNode*) aNode;
    sQtcNode->lflag &= ~QTC_NODE_SUBQUERY_MASK;
    sQtcNode->lflag |= QTC_NODE_SUBQUERY_EXIST;

    /* PROJ-2283 Single-Row Subquery ���� */
    sIs1RowSure = qtcSubqueryIs1RowSure(sStatement);

    //---------------------------------------------------------
    // Argument�� ������ ���� ó�� ����� �޸��Ѵ�.
    // ��, One-Column Subquery (sFence == 1)��
    // List Subquery (sFence > 1)�� ��쿡 ���� ó�� ����� �ٸ���.
    // One-Column Subquery�� ���,
    //    - Indirection ������� ó��
    // List Subquery�� ���,
    //    - List ������� ó��
    //---------------------------------------------------------

    if( sFence == 1 )
    {
        //---------------------------------------------------------
        // One Column Subquery �� ���� ó���� ������.
        //---------------------------------------------------------

        // Subquery Node�� Indirection�� �����ϵ��� ��.
        aNode->lflag |= MTC_NODE_INDIRECT_TRUE;

        // Column ������ skipModule�� �����.
        IDE_TEST( mtc::initializeColumn( aTemplate->rows[aNode->table].columns
                                         + aNode->column,
                                         & qtc::skipModule,
                                         0,
                                         0,
                                         0 )
                  != IDE_SUCCESS );

        // Argument�� Column ������ Stack�� �����Ͽ�
        // ���� Node������ estimate �� �����ϵ��� ��.
        sConvertedNode = aNode->arguments;
        sConvertedNode = mtf::convertedNode( sConvertedNode,
                                             &sTemplate->tmplate );

        aStack[0].column = aTemplate->rows[sConvertedNode->table].columns
            + sConvertedNode->column;

        // One Column�� Subquery�� ���� �Լ��� ������.
        aTemplate->rows[aNode->table].execute[aNode->column].initialize    = mtf::calculateNA;
        aTemplate->rows[aNode->table].execute[aNode->column].aggregate     = mtf::calculateNA;
        aTemplate->rows[aNode->table].execute[aNode->column].finalize      = mtf::calculateNA;
        aTemplate->rows[aNode->table].execute[aNode->column].calculateInfo = NULL;
        aTemplate->rows[aNode->table].execute[aNode->column].estimateRange = mtk::estimateRangeNA;
        aTemplate->rows[aNode->table].execute[aNode->column].extractRange  = mtk::extractRangeNA;

        if (sIs1RowSure == ID_TRUE)
        {
            aTemplate->rows[aNode->table].execute[aNode->column].calculate =
                qtcSubqueryCalculateSure;
        }
        else
        {
            aTemplate->rows[aNode->table].execute[aNode->column].calculate =
                qtcSubqueryCalculateUnknown;
        }
    }
    else
    {
        //---------------------------------------------------------
        // List Subquery �� ���� ó���� ������.
        //---------------------------------------------------------

        aStack[0].column =
            aTemplate->rows[aNode->table].columns + aNode->column;

        //---------------------------------------------------------
        // Subquery Node�� Column ������ list�� Data Type���� ������.
        // List�� Ÿ���� estimation�� ���� ���� ������ ������ ���� �����ȴ�.
        //
        // Ex) (I1, I2) IN ( (1,1), (2,1), (3,2) )
        //    Argument ���� : 3
        //    Arguemnt�� Column ���� : 2
        //
        // Subquery�� List ����
        //    (I1, I2) IN ( SELECT A1, A2 FROM ... )
        //    Argument ���� : 1
        //    Arguemnt�� Column ���� : sFence
        //---------------------------------------------------------
        //IDE_TEST( mtdList.estimate( aStack[0].column, 1, sFence, 0 )
        //          != IDE_SUCCESS );

        IDE_TEST( mtc::initializeColumn( aStack[0].column,
                                         &mtdList,
                                         1,
                                         sFence,
                                         0 )
                  != IDE_SUCCESS );

        //-------------------------------------------------------------
        // ���������� estimate �� ���� Target ������ŭ�� ó���� �����Ѵ�.
        //-------------------------------------------------------------

        // ���� Column�� ó���� ���� ������ Stack ���� �Ҵ�
        IDU_FIT_POINT( "qtcSubquery::qtcSubqueryEstimate::alloc::Stack" );
        IDE_TEST(aCallBack->alloc( aCallBack->info,
                                   aStack[0].column->column.size,
                                   (void**)&(aStack[0].value))
                 != IDE_SUCCESS);

        // ������ �Ҵ�� ������ ���� Column�� ������ �����Ѵ�.
        sStack2 = (mtcStack*)aStack[0].value;

        for( sCount = 0, sNode = aNode->arguments;
             sCount < sFence;
             sCount++,   sNode = sNode->next )
        {
            sConvertedNode = sNode;
            sConvertedNode = mtf::convertedNode( sConvertedNode,
                                                 &sTemplate->tmplate );

            sStack2[sCount].column =
                aTemplate->rows[sConvertedNode->table].columns
                + sConvertedNode->column;
        }

        // List �� Subquery�� ���� �Լ��� ������.
        aTemplate->rows[aNode->table].execute[aNode->column].initialize    = mtf::calculateNA;
        aTemplate->rows[aNode->table].execute[aNode->column].aggregate     = mtf::calculateNA;
        aTemplate->rows[aNode->table].execute[aNode->column].finalize      = mtf::calculateNA;
        aTemplate->rows[aNode->table].execute[aNode->column].calculateInfo = NULL;
        aTemplate->rows[aNode->table].execute[aNode->column].estimateRange = mtk::estimateRangeNA;
        aTemplate->rows[aNode->table].execute[aNode->column].extractRange  = mtk::extractRangeNA;

        if (sIs1RowSure == ID_TRUE)
        {
            aTemplate->rows[aNode->table].execute[aNode->column].calculate =
                qtcSubqueryCalculateListSure;
        }
        else
        {
            aTemplate->rows[aNode->table].execute[aNode->column].calculate =
                qtcSubqueryCalculateListTwice;
        }
    }

    sTemplate->tmplate.stack       = sStack;
    sTemplate->tmplate.stackRemain = sRemain;

    /* PROJ-2448 Subquery caching */
    IDE_TEST( qtcCache::validateSubqueryCache( (qcTemplate *)aTemplate, sQtcNode )
              != IDE_SUCCESS );

    // BUG-43059 Target subquery unnest/removal disable
    if ( ( sCallBackInfo->querySet != NULL ) &&
         ( sCallBackInfo->querySet->processPhase == QMS_VALIDATE_TARGET ) )
    {
        sQuerySet = ((qmsParseTree*)sStatement->myPlan->parseTree)->querySet;

        if ( QCU_OPTIMIZER_TARGET_SUBQUERY_UNNEST_DISABLE == 1 )
        {
            sQuerySet->flag &= ~QMV_QUERYSET_TARGET_SUBQUERY_UNNEST_MASK;
            sQuerySet->flag |=  QMV_QUERYSET_TARGET_SUBQUERY_UNNEST_FALSE;
        }
        else
        {
            // Nothing to do.
        }

        if ( QCU_OPTIMIZER_TARGET_SUBQUERY_REMOVAL_DISABLE == 1 )
        {
            sQuerySet->flag &= ~QMV_QUERYSET_TARGET_SUBQUERY_REMOVAL_MASK;
            sQuerySet->flag |=  QMV_QUERYSET_TARGET_SUBQUERY_REMOVAL_FALSE;
        }
        else
        {
            // Nothing to do.
        }

        IDE_DASSERT( sCallBackInfo->statement != NULL );
        qcgPlan::registerPlanProperty( sCallBackInfo->statement,
                                       PLAN_PROPERTY_OPTIMIZER_TARGET_SUBQUERY_UNNEST_DISABLE );
        qcgPlan::registerPlanProperty( sCallBackInfo->statement,
                                       PLAN_PROPERTY_OPTIMIZER_TARGET_SUBQUERY_REMOVAL_DISABLE );
    }
    else
    {
        // Nothing to do.
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_INVALID_FUNCTION_ARGUMENT )
    {
        IDE_SET(ideSetErrorCode(mtERR_ABORT_INVALID_FUNCTION_ARGUMENT));
    }
    IDE_EXCEPTION( ERR_STACK_OVERFLOW );
    {
        IDE_SET(ideSetErrorCode(mtERR_ABORT_STACK_OVERFLOW));
    }
    IDE_EXCEPTION( ERR_USE_SEQUENCE_IN_SUBQUERY );
    {
        (void)sqlInfo.init(sStatement->qmeMem);
        IDE_SET(
            ideSetErrorCode(qpERR_ABORT_QMV_USE_SEQUENCE_IN_SUBQUERY,
                            sqlInfo.getErrMessage() ));
        (void)sqlInfo.fini();
    }
    IDE_EXCEPTION(ERR_PROWID_NOT_SUPPORTED)
    {    
        IDE_SET(ideSetErrorCode(qpERR_ABORT_QMV_PROWID_NOT_SUPPORTED));
    }  
    IDE_EXCEPTION_END;

    sTemplate->tmplate.stack       = sStack;
    sTemplate->tmplate.stackRemain = sRemain;

    return IDE_FAILURE;
}

static IDE_RC qtcSubqueryCalculateSure( mtcNode     * aNode,
                                        mtcStack    * aStack,
                                        SInt          aRemain,
                                        void        * /* aInfo */,
                                        mtcTemplate * aTemplate )
{
/***********************************************************************
 *
 * Description :
 *
 *    One Column Subquery�� ������ �����Ѵ�.
 *    �� �Լ��� One Row�� �����ϴ� Subquery�� ���࿡���� ȣ��ȴ�.
 *    ��, ������ ���� Subquery�� ���࿡���� ȣ��ȴ�.
 *       - ex1) INSERT INTO T1 VALUES ( one_row_subquery );
 *       - ex2) SELECT * FROM T1 WHERE i1 = one_row_subquery;
 *    �ݴ�Ǵ� ���δ� ������ ���� �͵��� �ִ�.
 *       - ex1) INSERT INTO T1 multi_row_subquery;
 *       - ex2) SELECT * FROM T1 WHERE I1 in ( multi_row_subquery );
 *
 *    <PROJ-2283 Single-Row Subquery ����>
 *    single-row sure subquery �� ��츸 ȣ��ȴ�.
 *    subquery�� ����� �׻� 1�� �����̴�.
 *    ���� �ش� subquery�� �ѹ��� �����Ѵ�.
 *
 ***********************************************************************/

    qcStatement* sStatement;
    qmnPlan*     sPlan;
    mtcStack*    sStack;
    SInt         sRemain;
    qmcRowFlag   sFlag = QMC_ROW_INITIALIZE;

    qtcCacheObj   * sCacheObj = NULL;
    qtcCacheState   sState    = QTC_CACHE_STATE_BEGIN;
    UInt sParamCnt = 0;

    sStack     = aTemplate->stack;
    sRemain    = aTemplate->stackRemain;
    sStatement = ((qtcNode*)aNode)->subquery;
    sPlan      = sStatement->myPlan->plan;

    /* PROJ-2448 Subquery caching */

    // BUG-43696 outerColumn �� ������ ���� stack ����
    aTemplate->stack       = aStack  + 1;
    aTemplate->stackRemain = aRemain - 1;
    IDE_TEST_RAISE(aTemplate->stackRemain < 1, ERR_STACK_OVERFLOW);

    // �ش� Subquery�� plan�� �ʱ�ȭ�Ѵ�.
    IDE_TEST( sPlan->init( (qcTemplate*)aTemplate, sPlan ) != IDE_SUCCESS );

    IDE_TEST( qtcCache::searchCache( (qcTemplate *)aTemplate,
                                     (qtcNode*)aNode,
                                     aStack,
                                     QTC_CACHE_TYPE_SCALAR_SUBQUERY,
                                     &sCacheObj,
                                     &sParamCnt,
                                     &sState )
              != IDE_SUCCESS );

    // ������� ������ ���� stack ������
    aTemplate->stack       = aStack  + 1 + sParamCnt;
    aTemplate->stackRemain = aRemain - 1 - sParamCnt;
    IDE_TEST_RAISE( aTemplate->stackRemain < 1, ERR_STACK_OVERFLOW );

    switch ( sState )
    {
        case QTC_CACHE_STATE_INVOKE_MAKE_RECORD:
        case QTC_CACHE_STATE_RETURN_INVOKE:

            /* subquery ���� */
            IDE_TEST( sPlan->doIt( (qcTemplate*)aTemplate, sPlan, &sFlag )
                      != IDE_SUCCESS );

            if ( ( sFlag & QMC_ROW_DATA_MASK ) == QMC_ROW_DATA_EXIST )
            {
                // Nothing to do.
            }
            else
            {
                /* ����� ���� ��� null padding */
                IDE_TEST( sPlan->padNull( (qcTemplate*)aTemplate, sPlan )
                          != IDE_SUCCESS );
            }

            // Make cacheObj->currRecord and cache
            IDE_TEST( qtcCache::executeCache( QC_QXC_MEM( ((qcTemplate *)aTemplate)->stmt ),
                                              aNode,
                                              aStack,
                                              sCacheObj,
                                              ((qcTemplate *)aTemplate)->cacheBucketCnt,
                                              sState )
                      != IDE_SUCCESS );

            // Subquery�� ����� ���� Stack�� �װ�, Subquery�� Plan�� �����Ѵ�.
            aStack[0] = aStack[1+sParamCnt];
            break;

        case QTC_CACHE_STATE_RETURN_CURR_RECORD:

            IDE_TEST( qtcCache::getCacheValue( aNode,
                                               aStack,
                                               sCacheObj )
                      != IDE_SUCCESS );
            break;

        default:
            IDE_ERROR_RAISE( 0, ERR_UNEXPECTED_CACHE_ERROR );
            break;
    }

    sState = QTC_CACHE_STATE_END;

    aTemplate->stack       = sStack;
    aTemplate->stackRemain = sRemain;

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_STACK_OVERFLOW )
    {
        IDE_SET(ideSetErrorCode(mtERR_ABORT_STACK_OVERFLOW));
    }
    IDE_EXCEPTION( ERR_UNEXPECTED_CACHE_ERROR )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qtcSubquery::qtcSubqueryCalculateSure",
                                  "Unexpected execution cache error" ) );
    }
    IDE_EXCEPTION_END;

    aTemplate->stack       = sStack;
    aTemplate->stackRemain = sRemain;

    return IDE_FAILURE;
}

static IDE_RC qtcSubqueryCalculateUnknown( mtcNode     * aNode,
                                           mtcStack    * aStack,
                                           SInt          aRemain,
                                           void        * /*aInfo*/,
                                           mtcTemplate * aTemplate )
{
/***********************************************************************
 *
 * Description :
 *
 *    One Column Subquery�� ������ �����Ѵ�.
 *    �� �Լ��� One Row�� �����ϴ� Subquery�� ���࿡���� ȣ��ȴ�.
 *    ��, ������ ���� Subquery�� ���࿡���� ȣ��ȴ�.
 *       - ex1) INSERT INTO T1 VALUES ( one_row_subquery );
 *       - ex2) SELECT * FROM T1 WHERE i1 = one_row_subquery;
 *    �ݴ�Ǵ� ���δ� ������ ���� �͵��� �ִ�.
 *       - ex1) INSERT INTO T1 multi_row_subquery;
 *       - ex2) SELECT * FROM T1 WHERE I1 in ( multi_row_subquery );
 *
 * Implementation :
 *
 *    Subquery�� ����� one row������ Ȯ���Ͽ��� �Ѵ�.
 *    ����, �ش� Subquery�� �� �� �����ϰ� �ȴ�.
 *
 ***********************************************************************/

    qcStatement* sStatement;
    qmnPlan*     sPlan;
    mtcStack*    sStack;
    SInt         sRemain;
    qmcRowFlag   sFlag = QMC_ROW_INITIALIZE;

    qtcCacheObj   * sCacheObj = NULL;
    qtcCacheState   sState    = QTC_CACHE_STATE_BEGIN;
    UInt sParamCnt = 0;

    sStack     = aTemplate->stack;
    sRemain    = aTemplate->stackRemain;
    sStatement = ((qtcNode*)aNode)->subquery;
    sPlan      = sStatement->myPlan->plan;

    /* PROJ-2448 Subquery caching */

    // BUG-43696 outerColumn �� ������ ���� stack ����
    aTemplate->stack       = aStack  + 1;
    aTemplate->stackRemain = aRemain - 1;
    IDE_TEST_RAISE(aTemplate->stackRemain < 1, ERR_STACK_OVERFLOW);

    // �ش� Subquery�� plan�� �ʱ�ȭ�Ѵ�.
    IDE_TEST( sPlan->init( (qcTemplate*)aTemplate, sPlan ) != IDE_SUCCESS );

    IDE_TEST( qtcCache::searchCache( (qcTemplate *)aTemplate,
                                     (qtcNode*)aNode,
                                     aStack,
                                     QTC_CACHE_TYPE_SCALAR_SUBQUERY,
                                     &sCacheObj,
                                     &sParamCnt,
                                     &sState )
              != IDE_SUCCESS );

    // ������� ������ ���� stack ������
    aTemplate->stack       = aStack  + 1 + sParamCnt;
    aTemplate->stackRemain = aRemain - 1 - sParamCnt;
    IDE_TEST_RAISE(aTemplate->stackRemain < 1, ERR_STACK_OVERFLOW);

    switch ( sState )
    {
        case QTC_CACHE_STATE_INVOKE_MAKE_RECORD:
        case QTC_CACHE_STATE_RETURN_INVOKE:

            // �ش� Subquery�� plan�� �����Ѵ�.
            IDE_TEST( sPlan->doIt( (qcTemplate*)aTemplate, sPlan, &sFlag )
                      != IDE_SUCCESS );

            // ����� ���� ���, One Row Subquery���� Ȯ���Ͽ��� �ϸ�,
            // ����� ���� ���, Null Padding�Ѵ�.
            if ( ( sFlag & QMC_ROW_DATA_MASK ) == QMC_ROW_DATA_EXIST )
            {
                /*
                 * BUG-41784 subquery �������� PROJ-2283 �������� ����
                 */

                // ����� One Row������ Ȯ���Ѵ�.
                // ��, �� ��° �����ؼ� ����� ���� ���� Ȯ���Ѵ�.
                IDE_TEST( sPlan->doIt( (qcTemplate*)aTemplate, sPlan, &sFlag )
                          != IDE_SUCCESS );
                IDE_TEST_RAISE( ( sFlag & QMC_ROW_DATA_MASK ) == QMC_ROW_DATA_EXIST,
                                ERR_MULTIPLE_ROWS );

                IDE_TEST( sPlan->init( (qcTemplate*)aTemplate, sPlan )
                          != IDE_SUCCESS );
                IDE_TEST( sPlan->doIt( (qcTemplate*)aTemplate, sPlan, &sFlag )
                          != IDE_SUCCESS );

                IDE_ERROR_RAISE( ( sFlag & QMC_ROW_DATA_MASK ) == QMC_ROW_DATA_EXIST,
                                 ERR_SUBQUERY_RETURN_VALUE_CHANGED );

            }
            else
            {
                IDE_TEST( sPlan->padNull( (qcTemplate*)aTemplate, sPlan )
                          != IDE_SUCCESS );
            }

            // Make cacheObj->currRecord and cache
            IDE_TEST( qtcCache::executeCache( QC_QXC_MEM( ((qcTemplate *)aTemplate)->stmt ),
                                              aNode,
                                              aStack,
                                              sCacheObj,
                                              ((qcTemplate *)aTemplate)->cacheBucketCnt,
                                              sState )
                      != IDE_SUCCESS );

            // Subquery�� ����� ���� Stack�� �װ�, Subquery�� Plan�� �����Ѵ�.
            aStack[0] = aStack[1+sParamCnt];
            break;

        case QTC_CACHE_STATE_RETURN_CURR_RECORD:

            IDE_TEST( qtcCache::getCacheValue( aNode,
                                               aStack,
                                               sCacheObj )
                      != IDE_SUCCESS );
            break;

        default:
            IDE_ERROR_RAISE( 0, ERR_UNEXPECTED_CACHE_ERROR );
            break;
    }

    sState = QTC_CACHE_STATE_END;

    aTemplate->stack       = sStack;
    aTemplate->stackRemain = sRemain;

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_MULTIPLE_ROWS )
    {
        IDE_SET(ideSetErrorCode(qpERR_ABORT_QTC_MULTIPLE_ROWS));
    }
    IDE_EXCEPTION( ERR_STACK_OVERFLOW )
    {
        IDE_SET(ideSetErrorCode(mtERR_ABORT_STACK_OVERFLOW));
    }
    IDE_EXCEPTION( ERR_UNEXPECTED_CACHE_ERROR )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qtcSubquery::qtcSubqueryCalculateUnknown",
                                  "Unexpected execution cache error" ) );
    }
    IDE_EXCEPTION( ERR_SUBQUERY_RETURN_VALUE_CHANGED )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QTC_SUBQUERY_RETURN_VALUE_CHANGED ) );
    }
    IDE_EXCEPTION_END;

    aTemplate->stack       = sStack;
    aTemplate->stackRemain = sRemain;

    return IDE_FAILURE;
}

static IDE_RC qtcSubqueryCalculateListSure( mtcNode     * aNode,
                                            mtcStack    * aStack,
                                            SInt          aRemain,
                                            void        * /* aInfo */,
                                            mtcTemplate * aTemplate )
{
/***********************************************************************
 *
 * Description :
 *
 *    List Subquery�� ������ �����Ѵ�.
 *    �� �Լ��� One Row�� �����ϴ� Subquery�� ���࿡���� ȣ��ȴ�.
 *
 *    <PROJ-2283 Single-Row Subquery ����>
 *    single-row sure subquery �� ��츸 ȣ��ȴ�.
 *    subquery�� ����� �׻� 1�� �����̴�.
 *    ���� �ش� subquery�� �ѹ��� �����Ѵ�.
 *
 ***********************************************************************/

    qcStatement* sStatement;
    qmnPlan*     sPlan;
    mtcStack*    sStack;
    SInt         sRemain;
    qmcRowFlag   sFlag = QMC_ROW_INITIALIZE;

    qtcCacheObj  * sCacheObj = NULL;
    qtcCacheState  sState = QTC_CACHE_STATE_BEGIN;
    UInt sParamCnt = 0;

    sStack     = aTemplate->stack;
    sRemain    = aTemplate->stackRemain;
    sStatement = ((qtcNode*)aNode)->subquery;
    sPlan      = sStatement->myPlan->plan;

    /* PROJ-2448 Subquery caching */

    // BUG-43696 outerColumn �� ������ ���� stack ����
    aTemplate->stack       = aStack  + 1;
    aTemplate->stackRemain = aRemain - 1;
    IDE_TEST_RAISE(aTemplate->stackRemain < 1, ERR_STACK_OVERFLOW);

    // �ش� Subquery�� plan�� �ʱ�ȭ�Ѵ�.
    IDE_TEST( sPlan->init( (qcTemplate*)aTemplate, sPlan ) != IDE_SUCCESS );

    IDE_TEST( qtcCache::searchCache( (qcTemplate *)aTemplate,
                                     (qtcNode*)aNode,
                                     aStack,
                                     QTC_CACHE_TYPE_LIST_SUBQUERY,
                                     &sCacheObj,
                                     &sParamCnt,
                                     &sState )
              != IDE_SUCCESS );

    // ������� ������ ���� stack ������
    aTemplate->stack       = aStack  + 1 + sParamCnt;
    aTemplate->stackRemain = aRemain - 1 - sParamCnt;
    IDE_TEST_RAISE( aTemplate->stackRemain < 1, ERR_STACK_OVERFLOW );

    // ���� Column�� �����͸� ��� ����
    // estimate ������ �Ҵ�� ������ Stack�� �����Ѵ�.
    aStack[0].column = aTemplate->rows[aNode->table].columns + aNode->column;
    aStack[0].value  = (void*)((UChar*) aTemplate->rows[aNode->table].row
                               + aStack[0].column->column.offset);

    switch ( sState )
    {
        case QTC_CACHE_STATE_INVOKE_MAKE_RECORD:
        case QTC_CACHE_STATE_RETURN_INVOKE:

            // �ش� Subquery�� plan�� �����Ѵ�.
            IDE_TEST( sPlan->doIt( (qcTemplate*)aTemplate, sPlan, &sFlag )
                      != IDE_SUCCESS );

            if ( ( sFlag & QMC_ROW_DATA_MASK ) == QMC_ROW_DATA_EXIST )
            {
            }
            else
            {
                IDE_TEST( sPlan->padNull( (qcTemplate*)aTemplate, sPlan )
                          != IDE_SUCCESS );
            }

            // Make cacheObj->currRecord and cache
            IDE_TEST( qtcCache::executeCache( QC_QXC_MEM( ((qcTemplate *)aTemplate)->stmt ),
                                              aNode,
                                              aStack,
                                              sCacheObj,
                                              ((qcTemplate *)aTemplate)->cacheBucketCnt,
                                              sState )
                      != IDE_SUCCESS );

            // Subquery�� Target ������ ������ Stack�� ��°�� ������ ������ �����Ѵ�.
            idlOS::memcpy( aStack[0].value,
                           aStack + 1 + sParamCnt,
                           aStack[0].column->column.size );
            break;

        case QTC_CACHE_STATE_RETURN_CURR_RECORD:

            IDE_TEST( qtcCache::getCacheValue( aNode,
                                               aStack,
                                               sCacheObj )
                      != IDE_SUCCESS );
            break;

        default:
            IDE_ERROR_RAISE( 0, ERR_UNEXPECTED_CACHE_ERROR );
            break;
    }

    sState = QTC_CACHE_STATE_END;

    aTemplate->stack       = sStack;
    aTemplate->stackRemain = sRemain;

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_STACK_OVERFLOW )
    {
        IDE_SET(ideSetErrorCode(mtERR_ABORT_STACK_OVERFLOW));
    }
    IDE_EXCEPTION( ERR_UNEXPECTED_CACHE_ERROR )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qtcSubquery::qtcSubqueryCalculateListSure",
                                  "Unexpected execution cache error" ) );
    }
    IDE_EXCEPTION_END;

    aTemplate->stack       = sStack;
    aTemplate->stackRemain = sRemain;

    return IDE_FAILURE;
}

/*
 * subquery �� target ������ �ʹ� ���Ƽ�
 * single-row tuple �� �Ҵ���� �� ���� ���
 * ��¿�� ���� ���� ��Ĵ�� subquery �� �ι� �����Ѵ�.
 */
IDE_RC qtcSubqueryCalculateListTwice( mtcNode     * aNode,
                                      mtcStack    * aStack,
                                      SInt          aRemain,
                                      void        * /*aInfo*/,
                                      mtcTemplate * aTemplate )
{
    qcStatement* sStatement;
    qmnPlan*     sPlan;
    mtcStack*    sStack;
    SInt         sRemain;
    qmcRowFlag   sFlag = QMC_ROW_INITIALIZE;

    qtcCacheObj   * sCacheObj = NULL;
    qtcCacheState   sState = QTC_CACHE_STATE_BEGIN;
    UInt sParamCnt = 0;

    sStack     = aTemplate->stack;
    sRemain    = aTemplate->stackRemain;
    sStatement = ((qtcNode*)aNode)->subquery;
    sPlan      = sStatement->myPlan->plan;

    /* PROJ-2448 Subquery caching */

    // BUG-43696 outerColumn �� ������ ���� stack ����
    aTemplate->stack       = aStack  + 1;
    aTemplate->stackRemain = aRemain - 1;
    IDE_TEST_RAISE(aTemplate->stackRemain < 1, ERR_STACK_OVERFLOW);

    // �ش� Subquery�� plan�� �ʱ�ȭ�Ѵ�.
    IDE_TEST( sPlan->init( (qcTemplate*)aTemplate, sPlan ) != IDE_SUCCESS );

    IDE_TEST( qtcCache::searchCache( (qcTemplate *)aTemplate,
                                     (qtcNode*)aNode,
                                     aStack,
                                     QTC_CACHE_TYPE_LIST_SUBQUERY,
                                     &sCacheObj,
                                     &sParamCnt,
                                     &sState )
              != IDE_SUCCESS );

    // ������� ������ ���� stack ������
    aTemplate->stack       = aStack  + 1 + sParamCnt;
    aTemplate->stackRemain = aRemain - 1 - sParamCnt;
    IDE_TEST_RAISE( aTemplate->stackRemain < 1, ERR_STACK_OVERFLOW );

    // ���� Column�� �����͸� ��� ����
    // Unbound ������ �Ҵ�� ������ Stack�� �����Ѵ�.
    aStack[0].column = aTemplate->rows[aNode->table].columns + aNode->column;
    aStack[0].value  = (void*)((UChar*) aTemplate->rows[aNode->table].row
                               + aStack[0].column->column.offset);

    switch ( sState )
    {
        case QTC_CACHE_STATE_INVOKE_MAKE_RECORD:
        case QTC_CACHE_STATE_RETURN_INVOKE:

            // �ش� Subquery�� plan�� �����Ѵ�.
            IDE_TEST( sPlan->doIt( (qcTemplate*)aTemplate, sPlan, &sFlag )
                      != IDE_SUCCESS );

            if ( ( sFlag & QMC_ROW_DATA_MASK ) == QMC_ROW_DATA_EXIST )
            {
                // ����� One Row������ Ȯ���Ѵ�.
                // ��, �� ��° �����ؼ� ����� ���� ���� Ȯ���Ѵ�.
                IDE_TEST( sPlan->doIt( (qcTemplate*)aTemplate, sPlan, &sFlag )
                          != IDE_SUCCESS );
                IDE_TEST_RAISE( ( sFlag & QMC_ROW_DATA_MASK ) == QMC_ROW_DATA_EXIST,
                                ERR_MULTIPLE_ROWS );

                // �� ��° �������� Tuple Set�� ����ǹǷ�,
                // �ٽ� �� �� �����Ͽ��� �Ѵ�.

                IDE_TEST( sPlan->init( (qcTemplate*)aTemplate, sPlan )
                          != IDE_SUCCESS );

                IDE_TEST( sPlan->doIt( (qcTemplate*)aTemplate, sPlan, &sFlag )
                          != IDE_SUCCESS );
                IDE_TEST_RAISE( ( sFlag & QMC_ROW_DATA_MASK ) == QMC_ROW_DATA_NONE,
                                ERR_SUBQUERY_RETURN_VALUE_CHANGED );
            }
            else
            {
                IDE_TEST( sPlan->padNull( (qcTemplate*)aTemplate, sPlan )
                          != IDE_SUCCESS );
            }

            // Make cacheObj->currRecord and cache
            IDE_TEST( qtcCache::executeCache( QC_QXC_MEM( ((qcTemplate *)aTemplate)->stmt ),
                                              aNode,
                                              aStack,
                                              sCacheObj,
                                              ((qcTemplate *)aTemplate)->cacheBucketCnt,
                                              sState )
                      != IDE_SUCCESS );

            // Subquery�� Target ������ ������ Stack�� ��°�� ������ ������ �����Ѵ�.
            idlOS::memcpy( aStack[0].value,
                           aStack + 1 + sParamCnt,
                           aStack[0].column->column.size );
            break;

        case QTC_CACHE_STATE_RETURN_CURR_RECORD:

            IDE_TEST( qtcCache::getCacheValue( aNode,
                                               aStack,
                                               sCacheObj )
                      != IDE_SUCCESS );
            break;

        default:
            IDE_ERROR_RAISE( 0, ERR_UNEXPECTED_CACHE_ERROR );
            break;
    }

    sState = QTC_CACHE_STATE_END;

    aTemplate->stack       = sStack;
    aTemplate->stackRemain = sRemain;

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_MULTIPLE_ROWS )
    {
        IDE_SET(ideSetErrorCode(qpERR_ABORT_QTC_MULTIPLE_ROWS));
    }
    IDE_EXCEPTION( ERR_SUBQUERY_RETURN_VALUE_CHANGED )
    {
        IDE_SET(ideSetErrorCode(qpERR_ABORT_QTC_SUBQUERY_RETURN_VALUE_CHANGED));
    }
    IDE_EXCEPTION( ERR_STACK_OVERFLOW )
    {
        IDE_SET(ideSetErrorCode(mtERR_ABORT_STACK_OVERFLOW));
    }
    IDE_EXCEPTION( ERR_UNEXPECTED_CACHE_ERROR )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qtcSubquery::qtcSubqueryCalculateTwice",
                                  "Unexpected execution cache error" ) );
    }
    IDE_EXCEPTION_END;

    aTemplate->stack       = sStack;
    aTemplate->stackRemain = sRemain;

    return IDE_FAILURE;
}

static idBool qtcSubqueryIs1RowSure(qcStatement* aStatement)
{
    qmsQuerySet* sQuerySet;

    IDE_DASSERT(aStatement != NULL);
    IDE_DASSERT(aStatement->myPlan != NULL);
    IDE_DASSERT(aStatement->myPlan->parseTree != NULL);

    sQuerySet = ((qmsParseTree*)aStatement->myPlan->parseTree)->querySet;

    IDE_DASSERT(sQuerySet != NULL);

    if (sQuerySet->analyticFuncList != NULL)
    {
        /* analytic function �����ϸ� �׻� false */
        return ID_FALSE;
    }

    if (sQuerySet->SFWGH == NULL)
    {
        /* subquery �� set operation ���� ����� ��� �׻� false */
        return ID_FALSE;
    }

    if (sQuerySet->SFWGH->group == NULL)
    {
        if (sQuerySet->SFWGH->aggsDepth1 != NULL)
        {
            /* group by �� ���� aggregation �� �ִ� ��� */
            return ID_TRUE;
        }
        else
        {
            return ID_FALSE;
        }
    }
    else
    {
        if (sQuerySet->SFWGH->aggsDepth2 != NULL)
        {
            /* group by �� �ְ� nested aggregation �� �ִ� ��� */
            return ID_TRUE;
        }
        else
        {
            return ID_FALSE;
        }
    }
}

IDE_RC qtcSubqueryCalculateExists( mtcNode     * aNode,
                                   mtcStack    * aStack,
                                   SInt          aRemain,
                                   void        * /* aInfo */,
                                   mtcTemplate * aTemplate )
{
/***********************************************************************
 *
 * Description : PROJ-2448 Subquery caching
 *
 *               EXISTS �� ���ڷ� ���� subquery �� calculation
 *
 * Implementation :
 *
 *               aStack[0] �� EXISTS �� stack ���� true/false �� ��´�.
 *
 ***********************************************************************/

    qcStatement * sStatement = NULL;
    qmnPlan     * sPlan = NULL;
    mtcStack    * sStack = NULL;
    SInt          sRemain = 0;
    qmcRowFlag    sFlag = QMC_ROW_INITIALIZE;

    qtcCacheObj   * sCacheObj = NULL;
    qtcCacheState   sState    = QTC_CACHE_STATE_BEGIN;
    UInt sParamCnt = 0;

    sStack  = aTemplate->stack;
    sRemain = aTemplate->stackRemain;

    sStatement = ((qtcNode*)aNode)->subquery;
    sPlan      = sStatement->myPlan->plan;

    /* PROJ-2448 Subquery caching */

    // BUG-43696 outerColumn �� ������ ���� stack ����
    aTemplate->stack       = aStack  + 1;
    aTemplate->stackRemain = aRemain - 1;
    IDE_TEST_RAISE(aTemplate->stackRemain < 1, ERR_STACK_OVERFLOW);

    // �ش� Subquery�� plan�� �ʱ�ȭ�Ѵ�.
    IDE_TEST( sPlan->init( (qcTemplate*)aTemplate, sPlan ) != IDE_SUCCESS );

    IDE_TEST( qtcCache::searchCache( (qcTemplate *)aTemplate,
                                     (qtcNode*)aNode,
                                     aStack,
                                     QTC_CACHE_TYPE_EXISTS_SUBQUERY,
                                     &sCacheObj,
                                     &sParamCnt,
                                     &sState )
              != IDE_SUCCESS );

    // ������� ������ ���� stack ������
    aTemplate->stack       = aStack  + 1 + sParamCnt;
    aTemplate->stackRemain = aRemain - 1 - sParamCnt;
    IDE_TEST_RAISE( aTemplate->stackRemain < 1, ERR_STACK_OVERFLOW );

    switch ( sState )
    {
        case QTC_CACHE_STATE_INVOKE_MAKE_RECORD:
        case QTC_CACHE_STATE_RETURN_INVOKE:

            IDE_TEST( sPlan->doIt( (qcTemplate*)aTemplate, sPlan, &sFlag ) != IDE_SUCCESS );

            if ( ( sFlag & QMC_ROW_DATA_MASK ) == QMC_ROW_DATA_EXIST )
            {
                *(mtdBooleanType*)aStack[0].value = MTD_BOOLEAN_TRUE;
            }
            else
            {
                *(mtdBooleanType*)aStack[0].value = MTD_BOOLEAN_FALSE;
            }

            // Make cacheObj->currRecord and cache
            IDE_TEST( qtcCache::executeCache( QC_QXC_MEM( ((qcTemplate *)aTemplate)->stmt ),
                                              aNode,
                                              aStack,
                                              sCacheObj,
                                              ((qcTemplate *)aTemplate)->cacheBucketCnt,
                                              sState )
                      != IDE_SUCCESS );
            break;

        case QTC_CACHE_STATE_RETURN_CURR_RECORD:

            IDE_TEST( qtcCache::getCacheValue( aNode,
                                               aStack,
                                               sCacheObj )
                      != IDE_SUCCESS );
            break;

        default:

            IDE_ERROR_RAISE( 0, ERR_UNEXPECTED_CACHE_ERROR );
            break;
    }

    sState = QTC_CACHE_STATE_END;

    aTemplate->stack       = sStack;
    aTemplate->stackRemain = sRemain;

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_STACK_OVERFLOW )
    {
        IDE_SET(ideSetErrorCode(mtERR_ABORT_STACK_OVERFLOW));
    }
    IDE_EXCEPTION( ERR_UNEXPECTED_CACHE_ERROR )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qtcSubquery::qtcSubqueryCalculateExists",
                                  "Unexpected execution cache error" ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qtcSubqueryCalculateNotExists( mtcNode     * aNode,
                                      mtcStack    * aStack,
                                      SInt          aRemain,
                                      void        * /* aInfo */,
                                      mtcTemplate * aTemplate )
{
/***********************************************************************
 *
 * Description : PROJ-2448 Subquery caching
 *
 *               NOT EXISTS �� ���ڷ� ���� subquery �� calculation
 *
 * Implementation :
 *
 *               aStack[0] �� NOT EXISTS �� stack ���� true/false �� ��´�.
 *
 ***********************************************************************/

    qcStatement * sStatement = NULL;
    qmnPlan     * sPlan = NULL;
    mtcStack    * sStack = NULL;
    SInt          sRemain = 0;
    qmcRowFlag    sFlag = QMC_ROW_INITIALIZE;

    qtcCacheObj   * sCacheObj = NULL;
    qtcCacheState   sState    = QTC_CACHE_STATE_BEGIN;
    UInt sParamCnt = 0;

    sStack  = aTemplate->stack;
    sRemain = aTemplate->stackRemain;

    sStatement = ((qtcNode*)aNode)->subquery;
    sPlan      = sStatement->myPlan->plan;

    /* PROJ-2448 Subquery caching */

    // BUG-43696 outerColumn �� ������ ���� stack ����
    aTemplate->stack       = aStack  + 1;
    aTemplate->stackRemain = aRemain - 1;
    IDE_TEST_RAISE(aTemplate->stackRemain < 1, ERR_STACK_OVERFLOW);

    // �ش� Subquery�� plan�� �ʱ�ȭ�Ѵ�.
    IDE_TEST( sPlan->init( (qcTemplate*)aTemplate, sPlan ) != IDE_SUCCESS );

    IDE_TEST( qtcCache::searchCache( (qcTemplate *)aTemplate,
                                     (qtcNode*)aNode,
                                     aStack,
                                     QTC_CACHE_TYPE_NOT_EXISTS_SUBQUERY,
                                     &sCacheObj,
                                     &sParamCnt,
                                     &sState )
              != IDE_SUCCESS );

    // ������� ������ ���� stack ������
    aTemplate->stack       = aStack  + 1 + sParamCnt;
    aTemplate->stackRemain = aRemain - 1 - sParamCnt;
    IDE_TEST_RAISE( aTemplate->stackRemain < 1, ERR_STACK_OVERFLOW );

    switch ( sState )
    {
        case QTC_CACHE_STATE_INVOKE_MAKE_RECORD:
        case QTC_CACHE_STATE_RETURN_INVOKE:

            IDE_TEST( sPlan->doIt( (qcTemplate*)aTemplate, sPlan, &sFlag ) != IDE_SUCCESS );

            if ( ( sFlag & QMC_ROW_DATA_MASK ) == QMC_ROW_DATA_EXIST )
            {
                *(mtdBooleanType*)aStack[0].value = MTD_BOOLEAN_FALSE;
            }
            else
            {
                *(mtdBooleanType*)aStack[0].value = MTD_BOOLEAN_TRUE;
            }

            // Make cacheObj->currRecord and cache
            IDE_TEST( qtcCache::executeCache( QC_QXC_MEM( ((qcTemplate *)aTemplate)->stmt ),
                                              aNode,
                                              aStack,
                                              sCacheObj,
                                              ((qcTemplate *)aTemplate)->cacheBucketCnt,
                                              sState )
                      != IDE_SUCCESS );
            break;

        case QTC_CACHE_STATE_RETURN_CURR_RECORD:

            IDE_TEST( qtcCache::getCacheValue( aNode,
                                               aStack,
                                               sCacheObj )
                      != IDE_SUCCESS );
            break;

        default:

            IDE_ERROR_RAISE( 0, ERR_UNEXPECTED_CACHE_ERROR );
            break;
    }

    sState = QTC_CACHE_STATE_END;

    aTemplate->stack       = sStack;
    aTemplate->stackRemain = sRemain;

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_STACK_OVERFLOW )
    {
        IDE_SET(ideSetErrorCode(mtERR_ABORT_STACK_OVERFLOW));
    }
    IDE_EXCEPTION( ERR_UNEXPECTED_CACHE_ERROR )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qtcSubquery::qtcSubqueryCalculateNotExists",
                                  "Unexpected execution cache error" ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}
