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
 **********************************************************************/

#include <idl.h>
#include <cm.h>
#include <qcg.h>
#include <qcgPlan.h>
#include <qcpManager.h>
#include <qtc.h>
#include <qmvShardTransform.h>
#include <qcuSqlSourceInfo.h>
#include <qcuProperty.h>
#include <qmv.h>
#include <qmo.h>
#include <qmx.h>
#include <qsvEnv.h>
#include <qcpUtil.h>
#include <qciStmtType.h>
#include <sdi.h>

extern mtfModule mtfAvg;
extern mtfModule mtfCount;
extern mtfModule mtfSum;
extern mtfModule mtfMin;
extern mtfModule mtfMax;
extern mtfModule mtfDivide;

IDE_RC qmvShardTransform::doTransform( qcStatement  * aStatement )
{
/***********************************************************************
 *
 * Description : Shard View Transform
 *     shard table�� ���Ե� �������� ������� shard view�� �ƴϰų�,
 *     shard Ű���尡 ���� ������ shard view�� shard ������
 *     ��ȯ�Ѵ�.
 *
 *     ��1) top query, query ��ü�� shard query�� ���
 *          select * from t1 where i1=1 order by i1;
 *          --> select * from shard(select * from t1 where i1=1 order by i1);
 *
 *     ��2) view�� shard query�� ���
 *          select * from (select * from t1 where i1=1);
 *          --> select * from shard(select * from t1 where i1=1);
 *
 *     ��3) querySet�� shard query�� ���
 *          select * from t1 where i1=1 order by i2 loop 2;
 *          --> select * from shard(select * from t1 where i1=1) order by i2 loop 2;
 *
 *          select * from t1 where i1=1
 *          union all
 *          select * from t2 where i2=1;
 *          --> select * from shard(select * from t1 where i1=1)
 *              union all
 *              select * from t2 where i2=1;
 *
 *     ��4) from-where�� shard query�� ��� (�̱���)
 *          select func1(i1) from t1 where i1=1;
 *          --> select func1(i1) from (select * from t1 where i1=1);
 *
 *          select * from t1, t2 where t1.i1=t2.i1 and t1.i1=1;
 *          --> select * from (select * from t1 where t1.i1=1) v1, t2 where v1.i1=t2.i1;
 *
 *     ��5) from�� shard table�� ��� (�̱���)
 *          select * from t1, t2 where t1.i1=t2.i1 and t1.i1=1;
 *          --> select * from (select * from t1) v1, t2 where v1.i1=t2.i1 and v1.i1=1;
 *
 *     ��6) DML, query ��ü�� shard query�� ���
 *          insert into t1 values (1, 2);
 *          --> shard insert into t1 values (1, 2);
 *
 *          update t1 set i2=1 where i1=1;
 *          --> shard update t1 set i2=1 where i1=1;
 *
 *          delete from t1 where i1=1;
 *          --> shard delete from t1 where i1=1;
 *
 * Implementation :
 *
 ***********************************************************************/

    IDU_FIT_POINT_FATAL( "qmvShardTransform::doTransform::__FT__" );

    //------------------------------------------
    // ���ռ� �˻�
    //------------------------------------------

    IDE_FT_ASSERT( aStatement != NULL );

    //------------------------------------------
    // Shard View Transform ����
    //------------------------------------------

    // shard_meta�� ��ȯ���� �ʴ´�.
    if ( ( ( QC_SHARED_TMPLATE(aStatement)->flag & QC_TMP_SHARD_TRANSFORM_MASK )
           == QC_TMP_SHARD_TRANSFORM_ENABLE ) &&
         ( ( ( aStatement->mFlag & QC_STMT_SHARD_OBJ_MASK ) == QC_STMT_SHARD_OBJ_EXIST ) ||
           ( aStatement->myPlan->parseTree->stmtShard != QC_STMT_SHARD_NONE ) ) &&
         ( aStatement->myPlan->parseTree->stmtShard != QC_STMT_SHARD_META ) &&
         ( aStatement->spvEnv->createPkg == NULL ) &&
         ( aStatement->spvEnv->createProc == NULL ) &&
         ( qcg::isShardCoordinator( aStatement ) == ID_TRUE ) )
    {
        switch ( aStatement->myPlan->parseTree->stmtKind )
        {
            case QCI_STMT_SELECT:
            case QCI_STMT_SELECT_FOR_UPDATE:
                IDE_TEST( processTransform( aStatement ) != IDE_SUCCESS );
                break;

            case QCI_STMT_INSERT:
            case QCI_STMT_UPDATE:
            case QCI_STMT_DELETE:
            case QCI_STMT_EXEC_PROC:
                IDE_TEST( processTransformForDML( aStatement ) != IDE_SUCCESS );
                break;

            default:
                break;
        }
    }
    else
    {
        // Nothing to do.
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmvShardTransform::processTransform( qcStatement  * aStatement )
{
/***********************************************************************
 *
 * Description : Shard View Transform
 *
 * Implementation :
 *     top query block�̳� subquery�� ��� inline view�� �ѹ� �� ������ �Ѵ�.
 *
 ***********************************************************************/

    qmsParseTree     * sParseTree;
    qmsSortColumns   * sCurrSort;
    idBool             sIsShardQuery  = ID_FALSE;
    sdiAnalyzeInfo   * sShardAnalysis = NULL;
    UShort             sShardParamOffset = ID_USHORT_MAX;
    UShort             sShardParamCount = 0;
    qcuSqlSourceInfo   sqlInfo;

    IDU_FIT_POINT_FATAL( "qmvShardTransform::processTransform::__FT__" );

    //------------------------------------------
    // ���ռ� �˻�
    //------------------------------------------

    IDE_FT_ASSERT( aStatement != NULL );

    //------------------------------------------
    // �ʱ�ȭ
    //------------------------------------------

    sParseTree = (qmsParseTree *) aStatement->myPlan->parseTree;

    //------------------------------------------
    // Shard View Transform�� ����
    //------------------------------------------

    switch ( sParseTree->common.stmtShard )
    {
        case QC_STMT_SHARD_NONE:
        {
            if ( QC_IS_NULL_NAME( sParseTree->common.stmtPos ) == ID_FALSE )
            {
                // shard query���� �˻��Ѵ�.
                IDE_TEST( isShardQuery( aStatement,
                                        & sParseTree->common.stmtPos,
                                        & sIsShardQuery,
                                        & sShardAnalysis,
                                        & sShardParamOffset,
                                        & sShardParamCount )
                          != IDE_SUCCESS );

                if ( sIsShardQuery == ID_TRUE )
                {
                    if ( sParseTree->isView == ID_TRUE )
                    {
                        // view�� ��� shard view�� �����Ѵ�.
                        sParseTree->common.stmtShard = QC_STMT_SHARD_ANALYZE;

                        // �м������ ����Ѵ�.
                        aStatement->myPlan->mShardAnalysis = sShardAnalysis;
                        aStatement->myPlan->mShardParamOffset = sShardParamOffset;
                        aStatement->myPlan->mShardParamCount = sShardParamCount;
                    }
                    else
                    {
                        // top query�̰ų� subquery�� ��� shard view�� �����Ѵ�.
                        IDE_TEST( makeShardStatement( aStatement,
                                                      & sParseTree->common.stmtPos,
                                                      QC_STMT_SHARD_ANALYZE,
                                                      sShardAnalysis,
                                                      sShardParamOffset,
                                                      sShardParamCount )
                                  != IDE_SUCCESS );
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

            if ( sIsShardQuery == ID_FALSE )
            {
                // querySet
                IDE_TEST( processTransformForQuerySet( aStatement,
                                                       sParseTree->querySet )
                          != IDE_SUCCESS );

                // order by
                for ( sCurrSort = sParseTree->orderBy;
                      sCurrSort != NULL;
                      sCurrSort = sCurrSort->next )
                {
                    if ( sCurrSort->targetPosition <= QMV_EMPTY_TARGET_POSITION )
                    {
                        IDE_TEST( processTransformForExpr( aStatement,
                                                           sCurrSort->sortColumn )
                                  != IDE_SUCCESS );
                    }
                    else
                    {
                        // Nothing to do.
                    }
                }

                // loop (subquery�� �������� ������ ������ �߸��߻��Ѵ�.)
                if ( sParseTree->loopNode != NULL)
                {
                    IDE_TEST( processTransformForExpr( aStatement,
                                                       sParseTree->loopNode )
                              != IDE_SUCCESS );
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

            break;
        }

        case QC_STMT_SHARD_ANALYZE:
        {
            // select���� ��������� ���� shard view��
            // shard query�� �ƴϴ��� ����Ѵ�.
            if ( aStatement->myPlan->mShardAnalysis == NULL )
            {
                IDE_FT_ASSERT( sParseTree->common.stmtPos.size > 0 );

                IDE_TEST( isShardQuery( aStatement,
                                        & sParseTree->common.stmtPos,
                                        & sIsShardQuery,
                                        & sShardAnalysis,
                                        & sShardParamOffset,
                                        & sShardParamCount )
                          != IDE_SUCCESS );

                // ������� shard query�̳� analysis�� �������� ���� ���, ����ó���Ѵ�.
                if ( sShardAnalysis == NULL )
                {
                    sqlInfo.setSourceInfo( aStatement,
                                           & sParseTree->common.stmtPos );
                    IDE_RAISE( ERR_INVALID_SHARD_QUERY );
                }
                else
                {
                    if ( sShardAnalysis->mSplitMethod == SDI_SPLIT_NONE )
                    {
                        sqlInfo.setSourceInfo( aStatement,
                                               & sParseTree->common.stmtPos );
                        IDE_RAISE( ERR_INVALID_SHARD_QUERY );
                    }
                    else
                    {
                        // Nothing to do.
                    }
                }

                if ( sParseTree->isView == ID_TRUE )
                {
                    aStatement->myPlan->mShardAnalysis = sShardAnalysis;
                    aStatement->myPlan->mShardParamOffset = sShardParamOffset;
                    aStatement->myPlan->mShardParamCount = sShardParamCount;
                }
                else
                {
                    // top query�� ��� shard view�� �����Ѵ�.
                    IDE_TEST( makeShardStatement( aStatement,
                                                  & sParseTree->common.stmtPos,
                                                  QC_STMT_SHARD_ANALYZE,
                                                  sShardAnalysis,
                                                  sShardParamOffset,
                                                  sShardParamCount )
                              != IDE_SUCCESS );
                }
            }
            else
            {
                // Nothing to do.
            }

            break;
        }

        case QC_STMT_SHARD_DATA:
        {
            // select���� ��������� ���� shard view��
            // shard query�� �ƴϴ��� ����Ѵ�.
            if ( aStatement->myPlan->mShardAnalysis == NULL )
            {
                IDE_FT_ASSERT( sParseTree->common.stmtPos.size > 0 );

                IDE_TEST( isShardQuery( aStatement,
                                        & sParseTree->common.stmtPos,
                                        & sIsShardQuery,
                                        & sShardAnalysis,
                                        & sShardParamOffset,
                                        & sShardParamCount )
                          != IDE_SUCCESS );

                if ( sParseTree->common.nodes == NULL )
                {
                    // �м������ ������� ����� �м������ ��ü�Ѵ�.
                    sShardAnalysis = sdi::getAnalysisResultForAllNodes();
                }
                else
                {
                    IDE_TEST( sdi::validateNodeNames( aStatement,
                                                      sParseTree->common.nodes )
                              != IDE_SUCCESS );

                    if ( sShardAnalysis == NULL )
                    {
                        IDE_TEST( STRUCT_ALLOC( QC_QMP_MEM(aStatement),
                                                sdiAnalyzeInfo,
                                                &sShardAnalysis )
                                  != IDE_SUCCESS );
                    }
                    else
                    {
                        // Nothing to do
                    }

                    // BUG-45359
                    // Ư�� ������ ���� �м������ �����Ѵ�.
                    SDI_INIT_ANALYZE_INFO( sShardAnalysis );

                    sShardAnalysis->mSplitMethod = SDI_SPLIT_NODES;
                    sShardAnalysis->mNodeNames = sParseTree->common.nodes;
                }

                if ( sParseTree->isView == ID_TRUE )
                {
                    aStatement->myPlan->mShardAnalysis = sShardAnalysis;
                    aStatement->myPlan->mShardParamOffset = sShardParamOffset;
                    aStatement->myPlan->mShardParamCount = sShardParamCount;
                }
                else
                {
                    // top query�� ��� shard view�� �����Ѵ�.
                    IDE_TEST( makeShardStatement( aStatement,
                                                  & sParseTree->common.stmtPos,
                                                  QC_STMT_SHARD_DATA,
                                                  sShardAnalysis,
                                                  sShardParamOffset,
                                                  sShardParamCount )
                              != IDE_SUCCESS );
                }
            }
            else
            {
                // Nothing to do.
            }

            break;
        }

        case QC_STMT_SHARD_META:
            break;

        default:
            IDE_DASSERT(0);
            break;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_INVALID_SHARD_QUERY )
    {
        (void)sqlInfo.initWithBeforeMessage(aStatement->qmeMem);
        IDE_SET( ideSetErrorCode( sdERR_ABORT_INVALID_SHARD_QUERY,
                                  sqlInfo.getBeforeErrMessage(),
                                  sqlInfo.getErrMessage() ) );
        (void)sqlInfo.fini();
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmvShardTransform::processTransformForQuerySet( qcStatement  * aStatement,
                                                       qmsQuerySet  * aQuerySet )
{
/***********************************************************************
 *
 * Description : Shard View Transform
 *     query set�� ���Ͽ� top-up���� ��ȸ�ϸ� Shard View Transform�� �����Ѵ�.
 *
 * Implementation :
 *
 ***********************************************************************/

    qmsQuerySet       * sQuerySet;
    qmsFrom           * sFrom;
    qmsTarget         * sTarget;
    qmsConcatElement  * sConcatElement;
    qcNamePosition      sParsePosition;
    idBool              sIsShardQuery  = ID_FALSE;
    sdiAnalyzeInfo    * sShardAnalysis = NULL;
    UShort              sShardParamOffset = ID_USHORT_MAX;
    UShort              sShardParamCount = 0;
    idBool              sIsTransformAble = ID_FALSE;
    idBool              sIsTransformed = ID_FALSE;

    IDU_FIT_POINT_FATAL( "qmvShardTransform::processTransformForQuerySet::__FT__" );

    //------------------------------------------
    // ���ռ� �˻�
    //------------------------------------------

    IDE_FT_ASSERT( aStatement != NULL );
    IDE_FT_ASSERT( aQuerySet  != NULL );

    //------------------------------------------
    // �ʱ�ȭ
    //------------------------------------------

    sQuerySet = aQuerySet;

    //------------------------------------------
    // Shard View Transform�� ����
    //------------------------------------------

    // shard querySet���� �˻��Ѵ�.
    if ( ( QC_IS_NULL_NAME( sQuerySet->startPos ) == ID_FALSE ) &&
         ( QC_IS_NULL_NAME( sQuerySet->endPos ) == ID_FALSE ) )
    {
        // startPos�� ù��° token
        // endPos�� ������ token
        sParsePosition.stmtText = sQuerySet->startPos.stmtText;
        sParsePosition.offset   = sQuerySet->startPos.offset;
        sParsePosition.size     =
            sQuerySet->endPos.offset + sQuerySet->endPos.size -
            sQuerySet->startPos.offset;

        IDE_TEST( isShardQuery( aStatement,
                                & sParsePosition,
                                & sIsShardQuery,
                                & sShardAnalysis,
                                & sShardParamOffset,
                                & sShardParamCount )
                  != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    if ( sIsShardQuery == ID_TRUE )
    {
        // transform parse tree
        IDE_TEST( makeShardQuerySet( aStatement,
                                     sQuerySet,
                                     & sParsePosition,
                                     sShardAnalysis,
                                     sShardParamOffset,
                                     sShardParamCount )
                  != IDE_SUCCESS );
    }
    else
    {
        if ( sQuerySet->setOp == QMS_NONE )
        {
            IDE_DASSERT( sQuerySet->SFWGH != NULL );

            /* PROJ-2687 Shard aggregation transform */
            if ( sShardAnalysis != NULL )
            {
                if ( ( QCU_SHARD_AGGREGATION_TRANSFORM_DISABLE == 0 ) &&
                     ( sShardAnalysis->mIsTransformAble == 1 ) )
                {
                    IDE_TEST( isTransformAbleQuery( aStatement,
                                                    &sParsePosition,
                                                    &sIsTransformAble )
                              != IDE_SUCCESS );

                    if ( sIsTransformAble == ID_TRUE )
                    {
                        IDE_TEST( processAggrTransform( aStatement,
                                                        sQuerySet,
                                                        sShardAnalysis,
                                                        &sIsTransformed )
                                  != IDE_SUCCESS );
                    }
                    else
                    {
                        // sIsTransformed is false
                        // Nothing to do.
                    }
                }
                else
                {
                    // sIsTransformed is false
                    // Nothing to do.
                }
            }
            else
            {
                // sIsTransformed is false
                // Nothing to do.
            }

            if ( sIsTransformed == ID_FALSE )
            {
                // select target
                for ( sTarget = sQuerySet->SFWGH->target;
                      sTarget != NULL;
                      sTarget = sTarget->next )
                {
                    if ( ( sTarget->flag & QMS_TARGET_ASTERISK_MASK )
                         != QMS_TARGET_ASTERISK_TRUE )
                    {
                        IDE_TEST( processTransformForExpr(
                                      aStatement,
                                      sTarget->targetColumn )
                                  != IDE_SUCCESS );
                    }
                    else
                    {
                        // Nothing to do.
                    }
                }

                // from
                for ( sFrom = sQuerySet->SFWGH->from;
                      sFrom != NULL;
                      sFrom = sFrom->next )
                {
                    IDE_TEST( processTransformForFrom( aStatement,
                                                       sFrom )
                              != IDE_SUCCESS );
                }

                // where
                if ( sQuerySet->SFWGH->where != NULL )
                {
                    IDE_TEST( processTransformForExpr(
                                  aStatement,
                                  sQuerySet->SFWGH->where )
                              != IDE_SUCCESS );
                }
                else
                {
                    // Nothing to do.
                }

                // hierarchy
                if ( sQuerySet->SFWGH->hierarchy != NULL )
                {
                    if ( sQuerySet->SFWGH->hierarchy->startWith != NULL )
                    {
                        IDE_TEST( processTransformForExpr(
                                      aStatement,
                                      sQuerySet->SFWGH->hierarchy->startWith )
                                  != IDE_SUCCESS );
                    }
                    else
                    {
                        // Nothing to do.
                    }

                    if ( sQuerySet->SFWGH->hierarchy->connectBy != NULL )
                    {
                        IDE_TEST( processTransformForExpr(
                                      aStatement,
                                      sQuerySet->SFWGH->hierarchy->connectBy )
                                  != IDE_SUCCESS );
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

                // group by
                for ( sConcatElement = sQuerySet->SFWGH->group;
                      sConcatElement != NULL;
                      sConcatElement = sConcatElement->next )
                {
                    if ( sConcatElement->arithmeticOrList != NULL )
                    {
                        IDE_TEST( processTransformForExpr(
                                      aStatement,
                                      sConcatElement->arithmeticOrList )
                                  != IDE_SUCCESS );
                    }
                    else
                    {
                        // Nothing to do.
                    }
                }

                // having
                if ( sQuerySet->SFWGH->having != NULL )
                {
                    IDE_TEST( processTransformForExpr(
                                  aStatement,
                                  sQuerySet->SFWGH->having )
                              != IDE_SUCCESS );
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
        else
        {
            // left subquery
            IDE_TEST( processTransformForQuerySet( aStatement,
                                                   sQuerySet->left )
                      != IDE_SUCCESS );

            // right subquery
            IDE_TEST( processTransformForQuerySet( aStatement,
                                                   sQuerySet->right )
                      != IDE_SUCCESS );
        }
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmvShardTransform::processTransformForDML( qcStatement  * aStatement )
{
/***********************************************************************
 *
 * Description : Shard View Transform
 *     DML�� ��� ���� ��ü�� �˻��Ѵ�.
 *
 * Implementation :
 *
 ***********************************************************************/

    qmmInsParseTree  * sInsParseTree;
    qmmUptParseTree  * sUptParseTree;
    qmmDelParseTree  * sDelParseTree;
    qsExecParseTree  * sExecParseTree;
    sdiObjectInfo    * sShardObjInfo = NULL;
    idBool             sIsShardQuery = ID_FALSE;
    sdiAnalyzeInfo   * sShardAnalysis = NULL;
    UShort             sShardParamOffset = ID_USHORT_MAX;
    UShort             sShardParamCount = 0;
    qcuSqlSourceInfo   sqlInfo;

    IDU_FIT_POINT_FATAL( "qmvShardTransform::processTransformForDML::__FT__" );

    //------------------------------------------
    // ���ռ� �˻�
    //------------------------------------------

    IDE_FT_ASSERT( aStatement != NULL );
    IDE_FT_ASSERT( aStatement->myPlan->parseTree != NULL );

    switch ( aStatement->myPlan->parseTree->stmtKind )
    {
        case QCI_STMT_INSERT:
            sInsParseTree = (qmmInsParseTree*) aStatement->myPlan->parseTree;
            sShardObjInfo = sInsParseTree->tableRef->mShardObjInfo;
            break;

        case QCI_STMT_UPDATE:
            sUptParseTree = (qmmUptParseTree *)aStatement->myPlan->parseTree;
            sShardObjInfo = sUptParseTree->querySet->SFWGH->from->tableRef->mShardObjInfo;
            break;

        case QCI_STMT_DELETE:
            sDelParseTree = (qmmDelParseTree *)aStatement->myPlan->parseTree;
            sShardObjInfo = sDelParseTree->querySet->SFWGH->from->tableRef->mShardObjInfo;
            break;

        case QCI_STMT_EXEC_PROC:
            sExecParseTree = (qsExecParseTree *)aStatement->myPlan->parseTree;
            sShardObjInfo = sExecParseTree->mShardObjInfo;
            break;

        default:
            IDE_FT_ASSERT(0);
            break;
    }

    //------------------------------------------
    // Shard View Transform�� ����
    //------------------------------------------

    switch ( aStatement->myPlan->parseTree->stmtShard )
    {
        case QC_STMT_SHARD_NONE:
        {
            if ( sShardObjInfo != NULL )
            {
                IDE_FT_ASSERT( aStatement->myPlan->parseTree->stmtPos.size > 0 );

                // shard object�� ��� �м������ �ʿ��ϴ�.
                IDE_TEST( isShardQuery( aStatement,
                                        & aStatement->myPlan->parseTree->stmtPos,
                                        & sIsShardQuery,
                                        & sShardAnalysis,
                                        & sShardParamOffset,
                                        & sShardParamCount )
                          != IDE_SUCCESS );

                if ( sIsShardQuery == ID_FALSE )
                {
                    if ( aStatement->myPlan->parseTree->stmtKind == QCI_STMT_INSERT )
                    {
                        sInsParseTree = (qmmInsParseTree*) aStatement->myPlan->parseTree;

                        // multi-table insert�� �ƴ϶�� shardInsert�� �����Ѵ�.
                        if ( ( sInsParseTree->flag & QMM_MULTI_INSERT_MASK )
                             == QMM_MULTI_INSERT_FALSE )
                        {
                            aStatement->myPlan->parseTree->optimize = qmo::optimizeShardInsert;
                            aStatement->myPlan->parseTree->execute  = qmx::executeShardInsert;
                        }
                        else
                        {
                            sqlInfo.setSourceInfo( aStatement,
                                                   & aStatement->myPlan->parseTree->stmtPos );
                            IDE_RAISE( ERR_INVALID_SHARD_QUERY );
                        }
                    }
                    else
                    {
                        sqlInfo.setSourceInfo( aStatement,
                                               & aStatement->myPlan->parseTree->stmtPos );
                        IDE_RAISE( ERR_INVALID_SHARD_QUERY );
                    }
                }
                else
                {
                    // analysis�� �������� ���� ���, ����ó���Ѵ�.
                    if ( sShardAnalysis == NULL )
                    {
                        sqlInfo.setSourceInfo( aStatement,
                                               & aStatement->myPlan->parseTree->stmtPos );
                        IDE_RAISE( ERR_INVALID_SHARD_QUERY );
                    }
                    else
                    {
                        if ( sShardAnalysis->mSplitMethod == SDI_SPLIT_NONE )
                        {
                            sqlInfo.setSourceInfo( aStatement,
                                                   & aStatement->myPlan->parseTree->stmtPos );
                            IDE_RAISE( ERR_INVALID_SHARD_QUERY );
                        }
                        else
                        {
                            // Nothing to do.
                        }
                    }

                    // �Լ��� �����Ѵ�.
                    aStatement->myPlan->parseTree->optimize = qmo::optimizeShardDML;
                    aStatement->myPlan->parseTree->execute  = qmx::executeShardDML;
                }

                // shard statement�� �����Ѵ�.
                aStatement->myPlan->parseTree->stmtShard = QC_STMT_SHARD_ANALYZE;

                // �м������ ����Ѵ�.
                aStatement->myPlan->mShardAnalysis = sShardAnalysis;
                aStatement->myPlan->mShardParamOffset = sShardParamOffset;
                aStatement->myPlan->mShardParamCount = sShardParamCount;
            }
            else
            {
                // shard object�� �ƴ� ���
                // Nothing to do.
            }

            break;
        }

        case QC_STMT_SHARD_ANALYZE:
        {
            // shard object�� �ƴѰ�� SHARD keyword�� ����� �� ����.
            IDE_TEST_RAISE( sShardObjInfo == NULL, ERR_NOT_SHARD_OBJECT );

            // insert DML������ ��������� ����� shard�� ���� shard query�� �˻��Ѵ�.
            if ( aStatement->myPlan->mShardAnalysis == NULL )
            {
                IDE_FT_ASSERT( aStatement->myPlan->parseTree->stmtPos.size > 0 );

                IDE_TEST( isShardQuery( aStatement,
                                        & aStatement->myPlan->parseTree->stmtPos,
                                        & sIsShardQuery,
                                        & sShardAnalysis,
                                        & sShardParamOffset,
                                        & sShardParamCount )
                          != IDE_SUCCESS );

                if ( sIsShardQuery == ID_FALSE )
                {
                    // insert�� ������ ó���Ѵ�.
                    if ( aStatement->myPlan->parseTree->stmtKind == QCI_STMT_INSERT )
                    {
                        sInsParseTree = (qmmInsParseTree*) aStatement->myPlan->parseTree;

                        // multi-table insert�� ����
                        if ( ( sInsParseTree->flag & QMM_MULTI_INSERT_MASK )
                             == QMM_MULTI_INSERT_FALSE )
                        {
                            aStatement->myPlan->parseTree->optimize = qmo::optimizeShardInsert;
                            aStatement->myPlan->parseTree->execute  = qmx::executeShardInsert;
                        }
                        else
                        {
                            sqlInfo.setSourceInfo( aStatement,
                                                   & aStatement->myPlan->parseTree->stmtPos );
                            IDE_RAISE( ERR_INVALID_SHARD_QUERY );
                        }
                    }
                    // exec proc�� insertó�� �ݵ�� �����尡 �����Ǿ���ϴ� ������ ����.
                    else if ( aStatement->myPlan->parseTree->stmtKind == QCI_STMT_EXEC_PROC )
                    {
                        sqlInfo.setSourceInfo( aStatement,
                                               & aStatement->myPlan->parseTree->stmtPos );
                        IDE_RAISE( ERR_INVALID_SHARD_QUERY );
                    }
                    else
                    {
                        // analysis�� �������� ���� ���, ����ó���Ѵ�.
                        if ( sShardAnalysis == NULL )
                        {
                            sqlInfo.setSourceInfo( aStatement,
                                                   & aStatement->myPlan->parseTree->stmtPos );
                            IDE_RAISE( ERR_INVALID_SHARD_QUERY );
                        }
                        else
                        {
                            if ( sShardAnalysis->mSplitMethod == SDI_SPLIT_NONE )
                            {
                                sqlInfo.setSourceInfo( aStatement,
                                                       & aStatement->myPlan->parseTree->stmtPos );
                                IDE_RAISE( ERR_INVALID_SHARD_QUERY );
                            }
                            else
                            {
                                // Nothing to do.
                            }
                        }

                        // �Լ��� �����Ѵ�.
                        aStatement->myPlan->parseTree->optimize = qmo::optimizeShardDML;
                        aStatement->myPlan->parseTree->execute  = qmx::executeShardDML;
                    }
                }
                else
                {
                    // �Լ��� �����Ѵ�.
                    aStatement->myPlan->parseTree->optimize = qmo::optimizeShardDML;
                    aStatement->myPlan->parseTree->execute  = qmx::executeShardDML;
                }

                // �м������ ����Ѵ�.
                aStatement->myPlan->mShardAnalysis = sShardAnalysis;
                aStatement->myPlan->mShardParamOffset = sShardParamOffset;
                aStatement->myPlan->mShardParamCount = sShardParamCount;
            }
            else
            {
                // Nothing to do.
            }

            break;
        }

        case QC_STMT_SHARD_DATA:
        {
            // �ϴ� DML�� �������� �ʴ´�.
            IDE_RAISE( ERR_UNSUPPORTED_SHARD_DATA_IN_DML );

            // shard object�� �ƴѰ�� SHARD keyword�� ����� �� ����.
            IDE_TEST_RAISE( sShardObjInfo == NULL, ERR_NOT_SHARD_OBJECT );

            if ( aStatement->myPlan->mShardAnalysis == NULL )
            {
                IDE_FT_ASSERT( aStatement->myPlan->parseTree->stmtPos.size > 0 );

                IDE_TEST( isShardQuery( aStatement,
                                        & aStatement->myPlan->parseTree->stmtPos,
                                        & sIsShardQuery,
                                        & sShardAnalysis,
                                        & sShardParamOffset,
                                        & sShardParamCount )
                          != IDE_SUCCESS );

                if ( aStatement->myPlan->parseTree->nodes == NULL )
                {
                    // �м������ ������� ����� �м������ ��ü�Ѵ�.
                    sShardAnalysis = sdi::getAnalysisResultForAllNodes();
                }
                else
                {
                    IDE_TEST( sdi::validateNodeNames( aStatement,
                                                      aStatement->myPlan->parseTree->nodes )
                              != IDE_SUCCESS );

                    if ( sShardAnalysis == NULL )
                    {
                        IDE_TEST( STRUCT_ALLOC( QC_QMP_MEM(aStatement),
                                                sdiAnalyzeInfo,
                                                &sShardAnalysis )
                                  != IDE_SUCCESS );
                    }
                    else
                    {
                        // Nothing to do
                    }

                    // BUG-45359
                    // Ư�� ������ ���� �м������ �����Ѵ�.
                    SDI_INIT_ANALYZE_INFO( sShardAnalysis );

                    sShardAnalysis->mSplitMethod = SDI_SPLIT_NODES;
                    sShardAnalysis->mNodeNames = aStatement->myPlan->parseTree->nodes;
                }

                // �Լ��� �����Ѵ�.
                aStatement->myPlan->parseTree->optimize = qmo::optimizeShardDML;
                aStatement->myPlan->parseTree->execute  = qmx::executeShardDML;

                // �м������ ����Ѵ�.
                aStatement->myPlan->mShardAnalysis = sShardAnalysis;
                aStatement->myPlan->mShardParamOffset = sShardParamOffset;
                aStatement->myPlan->mShardParamCount = sShardParamCount;
            }
            else
            {
                // Nothing to do.
            }

            break;
        }

        case QC_STMT_SHARD_META:
            break;

        default:
            IDE_DASSERT(0);
            break;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_UNSUPPORTED_SHARD_DATA_IN_DML )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_UNSUPPORTED_SHARD_DATA_IN_DML ) );
    }
    IDE_EXCEPTION( ERR_NOT_SHARD_OBJECT )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_SHARD_TABLE_NOT_EXIST ) );
    }
    IDE_EXCEPTION( ERR_INVALID_SHARD_QUERY )
    {
        (void)sqlInfo.initWithBeforeMessage(aStatement->qmeMem);
        IDE_SET( ideSetErrorCode( sdERR_ABORT_INVALID_SHARD_QUERY,
                                  sqlInfo.getBeforeErrMessage(),
                                  sqlInfo.getErrMessage() ) );
        (void)sqlInfo.fini();
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmvShardTransform::isShardQuery( qcStatement     * aStatement,
                                        qcNamePosition  * aParsePosition,
                                        idBool          * aIsShardQuery,
                                        sdiAnalyzeInfo ** aShardAnalysis,
                                        UShort          * aShardParamOffset,
                                        UShort          * aShardParamCount )
{
/***********************************************************************
 *
 * Description : Shard View Transform
 *     Shard Query���� �˻��Ѵ�.
 *
 * Implementation :
 *
 ***********************************************************************/

    qcStatement       sStatement;
    UShort            sAllParamCount = 0;
    sdiAnalyzeInfo  * sAnalyzeInfo = NULL;
    UShort            sShardParamOffset = ID_USHORT_MAX;
    UShort            sShardParamCount = 0;
    SInt              sStage = 0;
    UShort            i = 0;

    /* PROJ-2646 New shard analyzer */
    UShort            sShardValueIndex;
    UShort            sSubValueIndex;

    IDE_FT_BEGIN();

    IDU_FIT_POINT_FATAL( "qmvShardTransform::isShardQuery::__FT__" );

    //-----------------------------------------
    // �غ�
    //-----------------------------------------

    // resolve�� ���� aStatement�� session�� �ʿ��ϴ�.
    IDE_TEST( qcg::allocStatement( & sStatement,
                                   aStatement->session,
                                   NULL,
                                   NULL )
              != IDE_SUCCESS );
    sStage = 1;

    sStatement.myPlan->stmtText    = aParsePosition->stmtText;
    sStatement.myPlan->stmtTextLen = idlOS::strlen( aParsePosition->stmtText );

    qcg::setSmiStmt( &sStatement, QC_SMI_STMT(aStatement) );

    //-----------------------------------------
    // PARSING
    //-----------------------------------------

    // FT_END �������� �̵��Ҷ��� IDE ��ũ�θ� ����ص� �ȴ�.
    IDE_TEST_CONT( qcpManager::parsePartialForAnalyze(
                       &sStatement,
                       aParsePosition->stmtText,
                       aParsePosition->offset,
                       aParsePosition->size ) != IDE_SUCCESS,
                   NORMAL_EXIT );

    // �м�������� ���� Ȯ���Ѵ�.
    IDE_TEST_CONT( sdi::checkStmt( &sStatement ) != IDE_SUCCESS,
                   NORMAL_EXIT );

    //-----------------------------------------
    // BIND (undef type)
    //-----------------------------------------

    sShardParamCount = qcg::getBindCount( &sStatement );

    for ( i = 0; i < sShardParamCount; i++ )
    {
        IDE_TEST_CONT( qcg::setBindColumn( &sStatement,
                                           i,
                                           MTD_UNDEF_ID,
                                           0,
                                           0,
                                           0 ) != IDE_SUCCESS,
                       NORMAL_EXIT );
    }

    //-----------------------------------------
    // VALIDATE
    //-----------------------------------------

    QC_SHARED_TMPLATE(&sStatement)->flag &= ~QC_TMP_SHARD_TRANSFORM_MASK;
    QC_SHARED_TMPLATE(&sStatement)->flag |= QC_TMP_SHARD_TRANSFORM_DISABLE;

    IDE_TEST_CONT( sStatement.myPlan->parseTree->parse( &sStatement )
                   != IDE_SUCCESS, NORMAL_EXIT );

    IDE_TEST_CONT( qtc::fixAfterParsing( QC_SHARED_TMPLATE(&sStatement) )
                   != IDE_SUCCESS, NORMAL_EXIT );
    sStage = 2;

    IDE_TEST_CONT( sStatement.myPlan->parseTree->validate( &sStatement )
                   != IDE_SUCCESS, NORMAL_EXIT );

    IDE_TEST_CONT( qtc::fixAfterValidation( QC_QMP_MEM(&sStatement),
                                            QC_SHARED_TMPLATE(&sStatement) )
                   != IDE_SUCCESS, NORMAL_EXIT );

    QC_SHARED_TMPLATE(&sStatement)->flag &= ~QC_TMP_SHARD_TRANSFORM_MASK;
    QC_SHARED_TMPLATE(&sStatement)->flag |= QC_TMP_SHARD_TRANSFORM_ENABLE;

    // PROJ-2653
    // bind parameter ID ȹ��
    if ( sShardParamCount > 0 )
    {
        sAllParamCount = qcg::getBindCount( aStatement );
        IDE_FT_ASSERT( sAllParamCount > 0 );
        IDE_FT_ASSERT( sAllParamCount >= sShardParamCount );

        for ( i = 0; i < sAllParamCount; i++ )
        {
            if ( aStatement->myPlan->stmtListMgr->hostVarOffset[i] ==
                 sStatement.myPlan->stmtListMgr->hostVarOffset[0] )
            {
                sShardParamOffset = i;
                break;
            }
            else
            {
                // Nothing to do.
            }
        }
    }
    else
    {
        // Nothing to do.
    }

    //-----------------------------------------
    // ANALYZE
    //-----------------------------------------

    IDE_TEST_CONT( sdi::analyze( &sStatement ) != IDE_SUCCESS,
                   NORMAL_EXIT );

    IDE_TEST_CONT( sdi::setAnalysisResult( &sStatement ) != IDE_SUCCESS,
                   NORMAL_EXIT );

    //-----------------------------------------
    // ANALYSIS RESULT
    //-----------------------------------------

    IDE_EXCEPTION_CONT( NORMAL_EXIT );

    // setAnalysisResult�� �����ϴ��� mShardAnalysis�� ������ �� �ִ�.
    if ( sStatement.myPlan->mShardAnalysis != NULL )
    {
        IDE_TEST( STRUCT_ALLOC( QC_QMP_MEM(aStatement),
                                sdiAnalyzeInfo,
                                &sAnalyzeInfo )
                  != IDE_SUCCESS );

        IDE_TEST( sdi::copyAnalyzeInfo( aStatement,
                                        sAnalyzeInfo,
                                        sStatement.myPlan->mShardAnalysis )
                  != IDE_SUCCESS );

        if ( sShardParamOffset > 0 )
        {
            for ( sShardValueIndex = 0;
                  sShardValueIndex < sAnalyzeInfo->mValueCount;
                  sShardValueIndex++ )
            {
                if ( sAnalyzeInfo->mValue[sShardValueIndex].mType == 0 )
                {
                    sAnalyzeInfo->mValue[sShardValueIndex].mValue.mBindParamId +=
                        sShardParamOffset;
                }
                else
                {
                    // Not a bind parameter
                    // Nothing to do
                }
            }

            if ( sAnalyzeInfo->mSubKeyExists == 1 )
            {
                for ( sSubValueIndex = 0;
                      sSubValueIndex < sAnalyzeInfo->mSubValueCount;
                      sSubValueIndex++ )
                {
                    if ( sAnalyzeInfo->mSubValue[sSubValueIndex].mType == 0 )
                    {
                        sAnalyzeInfo->mSubValue[sSubValueIndex].mValue.mBindParamId +=
                            sShardParamOffset;
                    }
                    else
                    {
                        // Not a bind parameter
                        // Nothing to do.
                    }
                }
            }
            else
            {
                // Nothing to do.
            }
        }
        else
        {
            // Nothing to do
        }

        if ( sStatement.myPlan->mShardAnalysis->mIsCanMerge == 1 )
        {
            *aIsShardQuery = ID_TRUE;
        }
        else
        {
            *aIsShardQuery = ID_FALSE;
        }

        *aShardAnalysis = sAnalyzeInfo;
        *aShardParamOffset = sShardParamOffset;
        *aShardParamCount = sShardParamCount;
    }
    else
    {
        *aIsShardQuery = ID_FALSE;
        *aShardAnalysis = NULL;
        *aShardParamOffset = sShardParamOffset;
        *aShardParamCount = sShardParamCount;
    }

    //-----------------------------------------
    // ������
    //-----------------------------------------

    if( sStatement.spvEnv->latched == ID_TRUE )
    {
        IDE_TEST( qsxRelatedProc::unlatchObjects( sStatement.spvEnv->procPlanList )
                  != IDE_SUCCESS );
        sStatement.spvEnv->latched = ID_FALSE;
    }
    else
    {
        // Nothing To Do
    }

    // session�� ������ �ƴϴ�.
    sStatement.session = NULL;

    (void) qcg::freeStatement(&sStatement);

    IDE_FT_END();

    return IDE_SUCCESS;

    IDE_EXCEPTION_SIGNAL()  /* PROJ-2617 */
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_FAULT_TOLERATED ) );
    }
    IDE_EXCEPTION_END;

    IDE_FT_EXCEPTION_BEGIN();

    switch ( sStage )
    {
        case 2:
            if ( qsxRelatedProc::unlatchObjects( sStatement.spvEnv->procPlanList )
                 == IDE_SUCCESS )
            {
                sStatement.spvEnv->latched = ID_FALSE;
            }
            else
            {
                IDE_ERRLOG( IDE_QP_1 );
            }
        case 1:
            sStatement.session = NULL;
            (void) qcg::freeStatement(&sStatement);
        default:
            break;
    }

    IDE_FT_EXCEPTION_END();

    return IDE_FAILURE;
}

IDE_RC qmvShardTransform::makeShardStatement( qcStatement    * aStatement,
                                              qcNamePosition * aParsePosition,
                                              qcShardStmtType  aShardStmtType,
                                              sdiAnalyzeInfo * aShardAnalysis,
                                              UShort           aShardParamOffset,
                                              UShort           aShardParamCount )
{
/***********************************************************************
 *
 * Description : Shard View Transform
 *
 *     statement ��ü�� shard view�� �����Ѵ�.
 *
 *     select i1, i2 from t1 where i1=1 order by i1;
 *     --------------------------------------------
 *     -->
 *     select * from shard(select i1, i2 from t1 where i1=1 order by i1);
 *                         --------------------------------------------
 *
 *     aStatement-parseTree
 *     -->
 *     aStatement-parseTree'-querySet'-SFWGH'-from'-tableRef'-sStatement'-parseTree
 *
 * Implementation :
 *
 ***********************************************************************/

    qcStatement  * sStatement = NULL;
    qmsParseTree * sParseTree = NULL;
    qmsQuerySet  * sQuerySet  = NULL;
    qmsSFWGH     * sSFWGH     = NULL;
    qmsFrom      * sFrom      = NULL;
    qmsTableRef  * sTableRef  = NULL;
    qcNamePosition sNullPosition;

    IDU_FIT_POINT_FATAL( "qmvShardTransform::makeShardStatement::__FT__" );

    SET_EMPTY_POSITION( sNullPosition );

    IDE_TEST( STRUCT_ALLOC( QC_QMP_MEM(aStatement),
                            qcStatement,
                            &sStatement )
              != IDE_SUCCESS );

    IDE_TEST( STRUCT_ALLOC( QC_QMP_MEM(aStatement),
                            qmsParseTree,
                            &sParseTree )
              != IDE_SUCCESS );
    QC_SET_INIT_PARSE_TREE( sParseTree, sNullPosition );

    IDE_TEST( STRUCT_ALLOC( QC_QMP_MEM(aStatement),
                            qmsQuerySet,
                            &sQuerySet )
              != IDE_SUCCESS );
    QCP_SET_INIT_QMS_QUERY_SET( sQuerySet );

    IDE_TEST( STRUCT_ALLOC( QC_QMP_MEM(aStatement),
                            qmsSFWGH,
                            &sSFWGH )
              != IDE_SUCCESS );
    QCP_SET_INIT_QMS_SFWGH( sSFWGH );

    IDE_TEST( STRUCT_ALLOC( QC_QMP_MEM(aStatement),
                            qmsFrom,
                            &sFrom )
              != IDE_SUCCESS );
    QCP_SET_INIT_QMS_FROM( sFrom );

    IDE_TEST( STRUCT_ALLOC( QC_QMP_MEM(aStatement),
                            qmsTableRef,
                            &sTableRef )
              != IDE_SUCCESS );
    QCP_SET_INIT_QMS_TABLE_REF( sTableRef );

    // aStatement�� ��ü�� �� �����Ƿ� sStatement�� ���� �����Ѵ�.
    idlOS::memcpy( sStatement, aStatement, ID_SIZEOF(qcStatement) );
    // myPlan�� �缳���Ѵ�.
    sStatement->myPlan = & sStatement->privatePlan;

    sTableRef->view      = sStatement;
    sFrom->tableRef      = sTableRef;
    sSFWGH->from         = sFrom;
    sSFWGH->thisQuerySet = sQuerySet;
    sQuerySet->SFWGH     = sSFWGH;

    // parseTree�� �����Ѵ�.
    sParseTree->withClause         = NULL;
    sParseTree->querySet           = sQuerySet;
    sParseTree->orderBy            = NULL;
    sParseTree->limit              = NULL;
    sParseTree->loopNode           = NULL;
    sParseTree->forUpdate          = NULL;
    sParseTree->queue              = NULL;
    sParseTree->isTransformed      = ID_FALSE;
    sParseTree->isView             = ID_TRUE;
    sParseTree->isShardView        = ID_FALSE;
    sParseTree->common.currValSeqs = NULL;
    sParseTree->common.nextValSeqs = NULL;
    sParseTree->common.parse       = qmv::parseSelect;
    sParseTree->common.validate    = qmv::validateSelect;
    sParseTree->common.optimize    = qmo::optimizeSelect;
    sParseTree->common.execute     = qmx::executeSelect;

    // aStatement�� parseTree�� �����Ѵ�.
    aStatement->myPlan->parseTree = (qcParseTree*) sParseTree;
    aStatement->myPlan->parseTree->stmtKind =
        sStatement->myPlan->parseTree->stmtKind;

    // sStatement�� shard view�� �����Ѵ�.
    SET_POSITION( sStatement->myPlan->parseTree->stmtPos, *aParsePosition );
    sStatement->myPlan->parseTree->stmtShard = aShardStmtType;

    // �м������ ����Ѵ�.
    sStatement->myPlan->mShardAnalysis = aShardAnalysis;
    sStatement->myPlan->mShardParamOffset = aShardParamOffset;
    sStatement->myPlan->mShardParamCount = aShardParamCount;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmvShardTransform::makeShardQuerySet( qcStatement    * aStatement,
                                             qmsQuerySet    * aQuerySet,
                                             qcNamePosition * aParsePosition,
                                             sdiAnalyzeInfo * aShardAnalysis,
                                             UShort           aShardParamOffset,
                                             UShort           aShardParamCount )
{
/***********************************************************************
 *
 * Description : Shard View Transform
 *
 *     query set�� shard view�� �����Ѵ�.
 *
 *     select i1, i2 from t1 where i1=1 order by i1;
 *     --------------------------------
 *     -->
 *     select * from shard(select i1, i2 from t1 where i1=1) order by i1;
 *                         --------------------------------
 *
 *     aStatement-parseTree-querySet-SFWGH
 *                     |
 *                  orderBy
 *     -->
 *     aStatement-parseTree-querySet-SFWGH'-from'-tableRef'-sStatement'-parseTree'-querySet'-SFWGH
 *                     |
 *                  orderBy
 *
 * Implementation :
 *
 ***********************************************************************/

    qcStatement  * sStatement = NULL;
    qmsParseTree * sParseTree = NULL;
    qmsQuerySet  * sQuerySet  = NULL;
    qmsSFWGH     * sSFWGH     = NULL;
    qmsFrom      * sFrom      = NULL;
    qmsTableRef  * sTableRef  = NULL;
    qcNamePosition sNullPosition;

    IDU_FIT_POINT_FATAL( "qmvShardTransform::makeShardQuerySet::__FT__" );

    SET_EMPTY_POSITION( sNullPosition );

    IDE_TEST( STRUCT_ALLOC( QC_QMP_MEM(aStatement),
                            qcStatement,
                            &sStatement )
              != IDE_SUCCESS );

    IDE_TEST( STRUCT_ALLOC( QC_QMP_MEM(aStatement),
                            qmsParseTree,
                            &sParseTree )
              != IDE_SUCCESS );
    QC_SET_INIT_PARSE_TREE( sParseTree, sNullPosition );

    IDE_TEST( STRUCT_ALLOC( QC_QMP_MEM(aStatement),
                            qmsQuerySet,
                            &sQuerySet )
              != IDE_SUCCESS );
    QCP_SET_INIT_QMS_QUERY_SET( sQuerySet );

    IDE_TEST( STRUCT_ALLOC( QC_QMP_MEM(aStatement),
                            qmsSFWGH,
                            &sSFWGH )
              != IDE_SUCCESS );
    QCP_SET_INIT_QMS_SFWGH( sSFWGH );

    IDE_TEST( STRUCT_ALLOC( QC_QMP_MEM(aStatement),
                            qmsFrom,
                            &sFrom )
              != IDE_SUCCESS );
    QCP_SET_INIT_QMS_FROM( sFrom );

    IDE_TEST( STRUCT_ALLOC( QC_QMP_MEM(aStatement),
                            qmsTableRef,
                            &sTableRef )
              != IDE_SUCCESS );
    QCP_SET_INIT_QMS_TABLE_REF( sTableRef );

    // aQuerySet�� ��ü�� �� �����Ƿ� sQuerySet�� ���� �����Ѵ�.
    idlOS::memcpy( sQuerySet, aQuerySet, ID_SIZEOF(qmsQuerySet) );
    QCP_SET_INIT_QMS_QUERY_SET( aQuerySet );

    sParseTree->withClause         = NULL;
    sParseTree->querySet           = sQuerySet;
    sParseTree->orderBy            = NULL;
    sParseTree->limit              = NULL;
    sParseTree->loopNode           = NULL;
    sParseTree->forUpdate          = NULL;
    sParseTree->queue              = NULL;
    sParseTree->isTransformed      = ID_FALSE;
    sParseTree->isView             = ID_TRUE;
    sParseTree->isShardView        = ID_FALSE;
    sParseTree->common.currValSeqs = NULL;
    sParseTree->common.nextValSeqs = NULL;
    sParseTree->common.parse       = qmv::parseSelect;
    sParseTree->common.validate    = qmv::validateSelect;
    sParseTree->common.optimize    = qmo::optimizeSelect;
    sParseTree->common.execute     = qmx::executeSelect;

    QC_SET_STATEMENT( sStatement, aStatement, sParseTree );
    sStatement->myPlan->parseTree->stmtKind = QCI_STMT_SELECT;

    // sStatement�� shard view�� �����Ѵ�.
    SET_POSITION( sStatement->myPlan->parseTree->stmtPos, *aParsePosition );
    sStatement->myPlan->parseTree->stmtShard = QC_STMT_SHARD_ANALYZE;

    // �м������ ����Ѵ�.
    sStatement->myPlan->mShardAnalysis = aShardAnalysis;
    sStatement->myPlan->mShardParamOffset = aShardParamOffset;
    sStatement->myPlan->mShardParamCount = aShardParamCount;

    sTableRef->view      = sStatement;
    sFrom->tableRef      = sTableRef;
    sSFWGH->from         = sFrom;
    sSFWGH->thisQuerySet = aQuerySet;
    aQuerySet->SFWGH     = sSFWGH;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmvShardTransform::makeShardView( qcStatement    * aStatement,
                                         qmsTableRef    * aTableRef,
                                         sdiAnalyzeInfo * aShardAnalysis )
{
/***********************************************************************
 *
 * Description : Shard View Transform
 *
 *     table�� shard view�� �����Ѵ�.
 *
 *     select * from sys.t1, t2 where t1.i1=t2.i1;
 *                   ------
 *     -->
 *     select * from shard(select * from sys.t1) t1, t2 where t1.i1=t2.i1 and t1.i1=1;
 *                   ------------------------------
 *
 *     select * from t1 a, t2 where a.i1=t2.i1;
 *                   ----
 *     -->
 *     select * from shard(select * from t1 a) a, t2 where a.i1=t2.i1 and a.i1=1;
 *                   ---------------------------
 *
 *     select * from t1 pivot (...), t2 where ...;
 *                   --------------
 *     -->
 *     select * from shard(select * from t1 pivot (...)), t2 where ...;
 *                   -----------------------------------
 *
 * Implementation :
 *
 ***********************************************************************/

    qcStatement     * sStatement = NULL;
    qmsParseTree    * sParseTree = NULL;
    qmsQuerySet     * sQuerySet  = NULL;
    qmsSFWGH        * sSFWGH     = NULL;
    qmsFrom         * sFrom      = NULL;
    qmsTableRef     * sTableRef  = NULL;
    SChar           * sQueryBuf  = NULL;
    qcNamePosition    sQueryPosition;
    qcNamePosition    sNullPosition;

    IDU_FIT_POINT_FATAL( "qmvShardTransform::makeShardView::__FT__" );

    SET_EMPTY_POSITION( sNullPosition );

    IDE_TEST(STRUCT_ALLOC(QC_QMP_MEM(aStatement),
                          qcStatement,
                          &sStatement)
             != IDE_SUCCESS);
    IDE_TEST(STRUCT_ALLOC(QC_QMP_MEM(aStatement),
                          qmsParseTree,
                          &sParseTree)
             != IDE_SUCCESS);
    QC_SET_INIT_PARSE_TREE( sParseTree, sNullPosition );
    IDE_TEST(STRUCT_ALLOC(QC_QMP_MEM(aStatement),
                          qmsQuerySet,
                          &sQuerySet)
             != IDE_SUCCESS);
    QCP_SET_INIT_QMS_QUERY_SET( sQuerySet );
    IDE_TEST(STRUCT_ALLOC(QC_QMP_MEM(aStatement),
                          qmsSFWGH,
                          &sSFWGH)
             != IDE_SUCCESS);
    QCP_SET_INIT_QMS_SFWGH( sSFWGH );

    IDE_TEST(STRUCT_ALLOC(QC_QMP_MEM(aStatement),
                          qmsFrom,
                          &sFrom)
             != IDE_SUCCESS);
    QCP_SET_INIT_QMS_FROM(sFrom);
    IDE_TEST(STRUCT_ALLOC(QC_QMP_MEM(aStatement),
                          qmsTableRef,
                          &sTableRef)
             != IDE_SUCCESS);
    QCP_SET_INIT_QMS_TABLE_REF( sTableRef );

    /* Copy existed TableRef to new TableRef except alias */
    idlOS::memcpy( sTableRef, aTableRef, ID_SIZEOF( qmsTableRef ) );
    QCP_SET_INIT_QMS_TABLE_REF(aTableRef);

    /* Set alias */
    if (QC_IS_NULL_NAME(sTableRef->aliasName) == ID_TRUE)
    {
        SET_POSITION(aTableRef->aliasName, sTableRef->tableName);
    }
    else
    {
        SET_POSITION(aTableRef->aliasName, sTableRef->aliasName);
    }

    sFrom->tableRef      = sTableRef;
    sSFWGH->from         = sFrom;
    sSFWGH->thisQuerySet = sQuerySet;
    sQuerySet->SFWGH     = sSFWGH;

    sParseTree->withClause         = NULL;
    sParseTree->querySet           = sQuerySet;
    sParseTree->orderBy            = NULL;
    sParseTree->limit              = NULL;
    sParseTree->loopNode           = NULL;
    sParseTree->forUpdate          = NULL;
    sParseTree->queue              = NULL;
    sParseTree->isTransformed      = ID_FALSE;
    sParseTree->isSiblings         = ID_FALSE;
    sParseTree->isView             = ID_TRUE;
    sParseTree->isShardView        = ID_FALSE;
    sParseTree->common.currValSeqs = NULL;
    sParseTree->common.nextValSeqs = NULL;
    sParseTree->common.parse       = qmv::parseSelect;
    sParseTree->common.validate    = qmv::validateSelect;
    sParseTree->common.optimize    = qmo::optimizeSelect;
    sParseTree->common.execute     = qmx::executeSelect;

    QC_SET_STATEMENT( sStatement, aStatement, sParseTree );
    sStatement->myPlan->parseTree->stmtKind = QCI_STMT_SELECT;
    sStatement->myPlan->parseTree->stmtShard = QC_STMT_SHARD_ANALYZE;

    // set select query
    IDE_TEST_RAISE( ( QC_IS_NULL_NAME( sTableRef->position ) == ID_TRUE ) ||
                    ( QC_IS_NULL_NAME( aTableRef->aliasName ) == ID_TRUE ),
                    ERR_NULL_POSITION );
    IDE_TEST(STRUCT_ALLOC_WITH_COUNT(QC_QMP_MEM(aStatement),
                                     SChar,
                                     512,
                                     &sQueryBuf)
             != IDE_SUCCESS);
    sQueryPosition.stmtText = sQueryBuf;
    sQueryPosition.offset = 0;
    sQueryPosition.size = idlOS::snprintf( sQueryBuf, 512, "select * from " );
    if ( sTableRef->position.stmtText[sTableRef->position.offset - 1] == '"' )
    {
        // "SYS".t1
        sQueryBuf[sQueryPosition.size] = '"';
        sQueryPosition.size++;
    }
    else
    {
        /* Nothing to do */
    }
    sQueryPosition.size +=
        idlOS::snprintf( sQueryBuf + sQueryPosition.size,
                         512 - sQueryPosition.size,
                         "%.*s",
                         sTableRef->position.size,
                         sTableRef->position.stmtText + sTableRef->position.offset );
    if ( sTableRef->position.stmtText[sTableRef->position.offset +
                                      sTableRef->position.size] == '"' )
    {
        // sys."T1"
        sQueryBuf[sQueryPosition.size] = '"';
        sQueryPosition.size++;
    }
    else
    {
        /* Nothing to do */
    }
    sQueryPosition.size +=
        idlOS::snprintf( sQueryBuf + sQueryPosition.size,
                         512 - sQueryPosition.size,
                         " as " );
    if ( aTableRef->aliasName.stmtText[aTableRef->aliasName.offset +
                                       aTableRef->aliasName.size] == '"' )
    {
        // t1 "a"
        sQueryPosition.size +=
        idlOS::snprintf( sQueryBuf + sQueryPosition.size,
                         512 - sQueryPosition.size,
                         "\"%.*s\"",
                         aTableRef->aliasName.size,
                         aTableRef->aliasName.stmtText + aTableRef->aliasName.offset );
    }
    else if ( aTableRef->aliasName.stmtText[aTableRef->aliasName.offset +
                                            aTableRef->aliasName.size] == '\'' )
    {
        // t1 'a'
        sQueryPosition.size +=
        idlOS::snprintf( sQueryBuf + sQueryPosition.size,
                         512 - sQueryPosition.size,
                         "'%.*s'",
                         aTableRef->aliasName.size,
                         aTableRef->aliasName.stmtText + aTableRef->aliasName.offset );
    }
    else
    {
        // t1 a
        sQueryPosition.size +=
        idlOS::snprintf( sQueryBuf + sQueryPosition.size,
                         512 - sQueryPosition.size,
                         "%.*s",
                         aTableRef->aliasName.size,
                         aTableRef->aliasName.stmtText + aTableRef->aliasName.offset );
    }
    SET_POSITION( sStatement->myPlan->parseTree->stmtPos, sQueryPosition );

    // �м������ ����Ѵ�.
    sStatement->myPlan->mShardAnalysis = aShardAnalysis;

    /* Set transformed inline view */
    aTableRef->view = sStatement;

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_NULL_POSITION )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmvShardTransform::makeShardView",
                                  "null position" ));
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmvShardTransform::isTransformAbleQuery( qcStatement     * aStatement,
                                                qcNamePosition  * aParsePosition,
                                                idBool          * aIsTransformAbleQuery )
{
/***********************************************************************
 *
 * Description : PROJ-2687 Shard aggregation transform
 *               From + Where�� Shard Query���� �˻��Ѵ�.
 *
 * Implementation :
 *
 ***********************************************************************/

    qcStatement       sStatement;
    SInt              sStage = 0;

    qmsSFWGH        * sSFWGH;
    qmsFrom         * sFrom;
    qtcNode         * sWhere;
    qmsHierarchy    * sHierarchy;

    IDE_FT_BEGIN();

    IDU_FIT_POINT_FATAL( "qmvShardTransform::isTransformAbleQuery::__FT__" );

    //-----------------------------------------
    // �غ�
    //-----------------------------------------

    // resolve�� ���� aStatement�� session�� �ʿ��ϴ�.
    IDE_TEST( qcg::allocStatement( & sStatement,
                                   aStatement->session,
                                   NULL,
                                   NULL )
              != IDE_SUCCESS );
    sStage = 1;

    sStatement.myPlan->stmtText    = aParsePosition->stmtText;
    sStatement.myPlan->stmtTextLen = idlOS::strlen( aParsePosition->stmtText );

    qcg::setSmiStmt( &sStatement, QC_SMI_STMT(aStatement) );

    //-----------------------------------------
    // PARSING
    //-----------------------------------------

    // FT_END �������� �̵��Ҷ��� IDE ��ũ�θ� ����ص� �ȴ�.
    IDE_TEST_CONT( qcpManager::parsePartialForAnalyze(
                       &sStatement,
                       aParsePosition->stmtText,
                       aParsePosition->offset,
                       aParsePosition->size ) != IDE_SUCCESS,
                   NORMAL_EXIT );

    // �м�������� ���� Ȯ���Ѵ�.
    IDE_TEST_CONT( sdi::checkStmt( &sStatement ) != IDE_SUCCESS,
                   NORMAL_EXIT );

    //-----------------------------------------
    // MODIFY PARSE TREE
    //-----------------------------------------
    sSFWGH = ((qmsParseTree*)sStatement.myPlan->parseTree)->querySet->SFWGH;

    sFrom      = sSFWGH->from;
    sWhere     = sSFWGH->where;
    sHierarchy = sSFWGH->hierarchy;

    QCP_SET_INIT_QMS_SFWGH(sSFWGH);

    sSFWGH->from      = sFrom;
    sSFWGH->where     = sWhere;
    sSFWGH->hierarchy = sHierarchy;

    sSFWGH->thisQuerySet = ((qmsParseTree*)sStatement.myPlan->parseTree)->querySet;

    //-----------------------------------------
    // VALIDATE
    //-----------------------------------------

    QC_SHARED_TMPLATE(&sStatement)->flag &= ~QC_TMP_SHARD_TRANSFORM_MASK;
    QC_SHARED_TMPLATE(&sStatement)->flag |= QC_TMP_SHARD_TRANSFORM_DISABLE;

    IDE_TEST_CONT( sStatement.myPlan->parseTree->parse( &sStatement )
                   != IDE_SUCCESS, NORMAL_EXIT );

    IDE_TEST_CONT( qtc::fixAfterParsing( QC_SHARED_TMPLATE(&sStatement) )
                   != IDE_SUCCESS, NORMAL_EXIT );
    sStage = 2;

    IDE_TEST_CONT( sStatement.myPlan->parseTree->validate( &sStatement )
                   != IDE_SUCCESS, NORMAL_EXIT );

    IDE_TEST_CONT( qtc::fixAfterValidation( QC_QMP_MEM(&sStatement),
                                            QC_SHARED_TMPLATE(&sStatement) )
                   != IDE_SUCCESS, NORMAL_EXIT );

    QC_SHARED_TMPLATE(&sStatement)->flag &= ~QC_TMP_SHARD_TRANSFORM_MASK;
    QC_SHARED_TMPLATE(&sStatement)->flag |= QC_TMP_SHARD_TRANSFORM_ENABLE;

    //-----------------------------------------
    // ANALYZE
    //-----------------------------------------

    IDE_TEST_CONT( sdi::analyze( &sStatement ) != IDE_SUCCESS,
                   NORMAL_EXIT );

    IDE_TEST_CONT( sdi::setAnalysisResult( &sStatement ) != IDE_SUCCESS,
                   NORMAL_EXIT );

    //-----------------------------------------
    // ANALYSIS RESULT
    //-----------------------------------------

    IDE_EXCEPTION_CONT( NORMAL_EXIT );

    // setAnalysisResult�� �����ϴ��� mShardAnalysis�� ������ �� �ִ�.
    if ( sStatement.myPlan->mShardAnalysis != NULL )
    {
        if ( sStatement.myPlan->mShardAnalysis->mIsCanMerge == 1 )
        {
            *aIsTransformAbleQuery = ID_TRUE;
        }
        else
        {
            *aIsTransformAbleQuery = ID_FALSE;
        }
    }
    else
    {
        *aIsTransformAbleQuery = ID_FALSE;
    }

    //-----------------------------------------
    // ������
    //-----------------------------------------

    if ( sStatement.spvEnv->latched == ID_TRUE )
    {
        IDE_TEST( qsxRelatedProc::unlatchObjects( sStatement.spvEnv->procPlanList )
                  != IDE_SUCCESS );
        sStatement.spvEnv->latched = ID_FALSE;
    }
    else
    {
        // Nothing To Do
    }

    // session�� ������ �ƴϴ�.
    sStatement.session = NULL;

    (void) qcg::freeStatement(&sStatement);

    IDE_FT_END();

    return IDE_SUCCESS;

    IDE_EXCEPTION_SIGNAL()  /* PROJ-2617 */
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_FAULT_TOLERATED ) );
    }
    IDE_EXCEPTION_END;

    IDE_FT_EXCEPTION_BEGIN();

    switch ( sStage )
    {
        case 2:
            if ( qsxRelatedProc::unlatchObjects( sStatement.spvEnv->procPlanList )
                 == IDE_SUCCESS )
            {
                sStatement.spvEnv->latched = ID_FALSE;
            }
            else
            {
                IDE_ERRLOG( IDE_QP_1 );
            }
        case 1:
            sStatement.session = NULL;
            (void) qcg::freeStatement(&sStatement);
        default:
            break;
    }

    IDE_FT_EXCEPTION_END();

    return IDE_FAILURE;
}

IDE_RC qmvShardTransform::processAggrTransform( qcStatement    * aStatement,
                                                qmsQuerySet    * aQuerySet,
                                                sdiAnalyzeInfo * aShardAnalysis,
                                                idBool         * aIsTransformed )
{
/***********************************************************************
 *
 * Description : PROJ-2687 Shard aggregation transform
 *
 *     Aggregate function���� ���� non-shard query�� �Ǻ��� query set�� ���Ͽ�
 *     �л�/���պ� �� query set�� ������ transformation�� ���Ͽ� shard query�� �����Ѵ�.
 *
 *     select sum(i1), i2 from t1 where i1=1 order by 1;
 *     --------------------------------
 *     -->
 *     select sum(i1), i2 from shard(select sum(i1) i1, i2 from t1 where i1=1) order by 1;
 *            -----------      -----------------------------------------------
 *
 *     aStatement-parseTree-querySet-SFWGH
 *                     |
 *                  orderBy
 *     -->
 *     aStatement-parseTree-querySet-SFWGH'-from'-tableRef'-sStatement'-parseTree'-querySet'-SFWGH''
 *                     |
 *                  orderBy
 *
 * Implementation :
 *
 ***********************************************************************/

    qcStatement     * sStatement = NULL;
    qmsFrom         * sFrom      = NULL;
    qmsTableRef     * sTableRef  = NULL;

    SChar            * sQueryBuf  = NULL;
    UInt               sQueryBufSize = 0;
    qcNamePosition     sQueryPosition;

    qtcNode          * sNode     = NULL;
    qmsConcatElement * sGroup    = NULL;
    qmsTarget        * sTarget   = NULL;
    qmsFrom          * sMyFrom   = NULL;

    UInt               sAddedGroupKey   = 0;
    UInt               sAddedTargetAggr = 0;
    UInt               sAddedHavingAggr = 0;
    UInt               sAddedTotal      = 0;

    UInt               sFromWhereStart  = 0;
    UInt               sFromWhereEnd    = 0;

    UInt               sViewTargetOrder = 0;

    sdiAnalyzeInfo   * sAnalyzeInfo = NULL;

    idBool             sUnsupportedAggr = ID_FALSE;

    IDU_FIT_POINT_FATAL( "qmvShardTransform::processAggrTransform::__FT__" );

    //------------------------------------------
    // 1. Make shard view statement text(S'FWG')
    //------------------------------------------

    sQueryBufSize = aStatement->myPlan->parseTree->stmtPos.size + 512;

    IDE_TEST(STRUCT_ALLOC_WITH_COUNT(QC_QMP_MEM(aStatement),
                                     SChar,
                                     sQueryBufSize,
                                     &sQueryBuf)
             != IDE_SUCCESS);

    /* Make SELECT clause statement */
    sQueryPosition.stmtText = sQueryBuf;
    sQueryPosition.offset = 0;
    sQueryPosition.size = idlOS::snprintf( sQueryBuf, sQueryBufSize, "SELECT " );

    // Add pure column group key to target
    for ( sGroup  = aQuerySet->SFWGH->group;
          sGroup != NULL;
          sGroup  = sGroup->next )
    {
        /*
         * sGroup->arithmeticOrList == NULL �� ���(ROLLUP, CUBE, GROUPING SETS)��
         * �ռ� ����� shard analysis���� �ɷ�����.
         */
        IDE_TEST( addColumnListToText( sGroup->arithmeticOrList,
                                       sQueryBuf,
                                       sQueryBufSize,
                                       &sQueryPosition,
                                       &sAddedGroupKey,
                                       &sAddedTotal )
                  != IDE_SUCCESS );
    }

    // Add aggregate function to target
    for ( sTarget  = aQuerySet->SFWGH->target;
          sTarget != NULL;
          sTarget  = sTarget->next )
    {
        IDE_TEST( addAggrListToText( sTarget->targetColumn,
                                     sQueryBuf,
                                     sQueryBufSize,
                                     &sQueryPosition,
                                     &sAddedTargetAggr,
                                     &sAddedTotal,
                                     &sUnsupportedAggr )
                  != IDE_SUCCESS );
    }

    for ( sNode  = aQuerySet->SFWGH->having;
          sNode != NULL;
          sNode  = (qtcNode*)sNode->node.next )
    {
        IDE_TEST( addAggrListToText( sNode,
                                     sQueryBuf,
                                     sQueryBufSize,
                                     &sQueryPosition,
                                     &sAddedHavingAggr,
                                     &sAddedTotal,
                                     &sUnsupportedAggr )
                  != IDE_SUCCESS );
    }

    if ( sUnsupportedAggr == ID_FALSE )
    {
        /* Make FROM & WHERE clause statement */
        sQueryBuf[sQueryPosition.size] = ' ';
        sQueryPosition.size++;

        sFromWhereStart = aQuerySet->SFWGH->from->fromPosition.offset;

        /* Where clause�� �����ϸ� where�� ������ node�� end offset�� ã�´�. */
        if ( aQuerySet->SFWGH->where != NULL )
        {
            for ( sNode  = aQuerySet->SFWGH->where;
                  sNode != NULL;
                  sNode  = (qtcNode*)sNode->node.next )
            {
                if ( sNode->node.next == NULL )
                {
                    sFromWhereEnd = sNode->position.offset + sNode->position.size;

                    if ( sNode->position.stmtText[sNode->position.offset +
                                                  sNode->position.size] == '"' )
                    {
                        sFromWhereEnd++;
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
        }
        else
        {
            /* Where clause�� �������� ������ from�� ��ȸ�ϸ� from�� end offset�� ã�´�. */
            for ( sMyFrom  = aQuerySet->SFWGH->from;
                  sMyFrom != NULL;
                  sMyFrom  = sMyFrom->next )
            {
                IDE_TEST( getFromEnd( sMyFrom,
                                      &sFromWhereEnd )
                          != IDE_SUCCESS );
            }
        }

        sQueryPosition.size +=
            idlOS::snprintf( sQueryBuf + sQueryPosition.size,
                             sQueryBufSize - sQueryPosition.size,
                             "%.*s",
                             sFromWhereEnd - sFromWhereStart,
                             aQuerySet->SFWGH->startPos.stmtText + sFromWhereStart );

        /* Make GROUP-BY clause statement */
        sAddedGroupKey = 0;
        sAddedTotal = 0;

        for ( sGroup  = aQuerySet->SFWGH->group;
              sGroup != NULL;
              sGroup  = sGroup->next )
        {
            if ( sAddedGroupKey == 0 )
            {
                sQueryPosition.size +=
                    idlOS::snprintf( sQueryBuf + sQueryPosition.size,
                                     sQueryBufSize - sQueryPosition.size,
                                     " GROUP BY " );
            }
            else
            {
                // Nothing to do.
            }

            IDE_TEST( addColumnListToText( sGroup->arithmeticOrList,
                                           sQueryBuf,
                                           sQueryBufSize,
                                           &sQueryPosition,
                                           &sAddedGroupKey,
                                           &sAddedTotal )
                      != IDE_SUCCESS );
        }

        //----------------------------------------------------------
        // 2. Partial parsing with shard view statement text(S'FWG')
        //----------------------------------------------------------
        IDE_TEST(STRUCT_ALLOC(QC_QMP_MEM(aStatement),
                              qcStatement,
                              &sStatement)
                 != IDE_SUCCESS);

        QC_SET_STATEMENT( sStatement, aStatement, NULL );
        sStatement->myPlan->stmtText    = sQueryPosition.stmtText;
        sStatement->myPlan->stmtTextLen = idlOS::strlen( sQueryPosition.stmtText );

        IDE_TEST( qcpManager::parsePartialForQuerySet(
                      sStatement,
                      sQueryPosition.stmtText,
                      sQueryPosition.offset,
                      sQueryPosition.size )
                  != IDE_SUCCESS );

        sStatement->myPlan->parseTree->stmtShard = QC_STMT_SHARD_ANALYZE;

        IDE_TEST( STRUCT_ALLOC( QC_QMP_MEM(aStatement),
                                sdiAnalyzeInfo,
                                &sAnalyzeInfo )
                  != IDE_SUCCESS );

        idlOS::memcpy( (void*)sAnalyzeInfo,
                       (void*)aShardAnalysis,
                       ID_SIZEOF(sdiAnalyzeInfo) );

        sAnalyzeInfo->mIsCanMerge = 1;
        sAnalyzeInfo->mIsTransformAble = 0;

        sStatement->myPlan->mShardAnalysis = sAnalyzeInfo;

        //-----------------------------------------
        // 3. Modify original query set
        //-----------------------------------------

        // From
        IDE_TEST(STRUCT_ALLOC(QC_QMP_MEM(aStatement),
                              qmsFrom,
                              &sFrom)
                 != IDE_SUCCESS);

        QCP_SET_INIT_QMS_FROM(sFrom);

        IDE_TEST(STRUCT_ALLOC(QC_QMP_MEM(aStatement),
                              qmsTableRef,
                              &sTableRef)
                 != IDE_SUCCESS);

        QCP_SET_INIT_QMS_TABLE_REF( sTableRef );

        sTableRef->view = sStatement;
        sFrom->tableRef = sTableRef;
        aQuerySet->SFWGH->from = sFrom;

        aQuerySet->SFWGH->flag &= ~QMV_SFWGH_SHARD_TRANS_VIEW_MASK;
        aQuerySet->SFWGH->flag |= QMV_SFWGH_SHARD_TRANS_VIEW_TRUE;

        // Select
        sViewTargetOrder = sAddedGroupKey;

        for ( sTarget  = aQuerySet->SFWGH->target;
              sTarget != NULL;
              sTarget  = sTarget->next )
        {
            IDE_TEST( modifyOrgAggr( aStatement,
                                     &sTarget->targetColumn,
                                     &sViewTargetOrder )
                      != IDE_SUCCESS);
        }

        // Where
        aQuerySet->SFWGH->where = NULL;

        // Having
        for ( sNode  = aQuerySet->SFWGH->having;
              sNode != NULL;
              sNode  = (qtcNode*)sNode->node.next )
        {
            IDE_TEST( modifyOrgAggr( aStatement,
                                     &sNode,
                                     &sViewTargetOrder )
                      != IDE_SUCCESS);
        }

        *aIsTransformed = ID_TRUE;
    }
    else
    {
        /*
         * �������� �ʴ� aggregate function�� �������� aggr transform�� ������ �� ����.
         * ���� �ܰ� transformation(from transformation)�� ���� ��Ű�� ���ؼ� ����
         */
        *aIsTransformed = ID_FALSE;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmvShardTransform::addColumnListToText( qtcNode        * aNode,
                                               SChar          * aQueryBuf,
                                               UInt             aQueryBufSize,
                                               qcNamePosition * aQueryPosition,
                                               UInt           * aAddedColumnCount,
                                               UInt           * aAddedTotal )
{
/***********************************************************************
 *
 * Description : PROJ-2687 Shard aggregation transform
 *
 *               Node tree�� ��ȸ�ϸ鼭
 *               ���� column�� position�� string�� ����Ѵ�.
 *
 * Implementation :
 *
 ***********************************************************************/
    qtcNode * sNode = NULL;

    IDU_FIT_POINT_FATAL( "qmvShardTransform::addColumnListToText::__FT__" );

    IDE_DASSERT( aNode != NULL );

    if ( aNode->node.module == &qtc::columnModule)
    {
        // Add to text
        if ( *aAddedTotal != 0 )
        {
            aQueryBuf[aQueryPosition->size] = ',';
            aQueryPosition->size++;
        }
        else
        {
            /* Nothing to do */
        }

        if ( aNode->position.stmtText[aNode->position.offset - 1] == '"' )
        {
            aQueryBuf[aQueryPosition->size] = '"';
            aQueryPosition->size++;
        }
        else
        {
            // Nothing to do.
        }

        aQueryPosition->size +=
            idlOS::snprintf( aQueryBuf + aQueryPosition->size,
                             aQueryBufSize - aQueryPosition->size,
                             "%.*s",
                             aNode->position.size,
                             aNode->position.stmtText + aNode->position.offset );

        if ( aNode->position.stmtText[aNode->position.offset +
                                      aNode->position.size] == '"' )
        {
            aQueryBuf[aQueryPosition->size] = '"';
            aQueryPosition->size++;
        }
        else
        {
            // Nothing to do.
        }

        (*aAddedColumnCount)++;
        (*aAddedTotal)++;
    }
    else if ( ( aNode->node.lflag & MTC_NODE_OPERATOR_MASK ) == MTC_NODE_OPERATOR_SUBQUERY )
    {
        // Nothing to do.
    }
    else
    {
        // Traverse
        for ( sNode  = (qtcNode*)aNode->node.arguments;
              sNode != NULL;
              sNode  = (qtcNode*)sNode->node.next )
        {
            IDE_TEST( addColumnListToText( sNode,
                                           aQueryBuf,
                                           aQueryBufSize,
                                           aQueryPosition,
                                           aAddedColumnCount,
                                           aAddedTotal )
                      != IDE_SUCCESS );
        }
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmvShardTransform::addAggrListToText( qtcNode        * aNode,
                                             SChar          * aQueryBuf,
                                             UInt             aQueryBufSize,
                                             qcNamePosition * aQueryPosition,
                                             UInt           * aAddedCount,
                                             UInt           * aAddedTotal,
                                             idBool         * aUnsupportedAggr )
{
/***********************************************************************
 *
 * Description : PROJ-2687 Shard aggregation transform
 *
 *               Node tree�� ��ȸ�ϸ鼭
 *               Aggregate function�� real position�� string�� ����Ѵ�.
 *
 * Implementation :
 *
 ***********************************************************************/
    qtcNode * sNode = NULL;

    IDU_FIT_POINT_FATAL( "qmvShardTransform::addAggrListToText::__FT__" );

    IDE_DASSERT( aNode != NULL );

    if ( QTC_IS_AGGREGATE(aNode) == ID_TRUE )
    {
        if ( *aAddedTotal != 0 )
        {
            aQueryBuf[aQueryPosition->size] = ',';
            aQueryPosition->size++;
        }
        else
        {
            // Nothing to do.
        }

        if ( aNode->node.module == &mtfSum )
        {
            IDE_TEST( addSumMinMaxCountToText( aNode,
                                               aQueryBuf,
                                               aQueryBufSize,
                                               aQueryPosition )
                      != IDE_SUCCESS );
        }
        else if ( aNode->node.module == &mtfMin )
        {
            IDE_TEST( addSumMinMaxCountToText( aNode,
                                               aQueryBuf,
                                               aQueryBufSize,
                                               aQueryPosition )
                      != IDE_SUCCESS );
        }
        else if ( aNode->node.module == &mtfMax )
        {
            IDE_TEST( addSumMinMaxCountToText( aNode,
                                               aQueryBuf,
                                               aQueryBufSize,
                                               aQueryPosition )
                      != IDE_SUCCESS );
        }
        else if ( aNode->node.module == &mtfCount )
        {
            IDE_TEST( addSumMinMaxCountToText( aNode,
                                               aQueryBuf,
                                               aQueryBufSize,
                                               aQueryPosition )
                      != IDE_SUCCESS );
        }
        else if ( aNode->node.module == &mtfAvg )
        {
            IDE_TEST( addAvgToText( aNode,
                                    aQueryBuf,
                                    aQueryBufSize,
                                    aQueryPosition )
                      != IDE_SUCCESS );

            (*aAddedCount)++;
            (*aAddedTotal)++;
        }
        else
        {
            *aUnsupportedAggr = ID_TRUE;
        }

        (*aAddedCount)++;
        (*aAddedTotal)++;
    }
    else if ( ( aNode->node.lflag & MTC_NODE_OPERATOR_MASK ) == MTC_NODE_OPERATOR_SUBQUERY )
    {
        // Sub-query�� ���� query set�� ������� �� �� ����.
        IDE_RAISE(ERR_SUBQ_EXISTS);
    }
    else
    {
        // Traverse
        for ( sNode  = (qtcNode*)aNode->node.arguments;
              sNode != NULL;
              sNode  = (qtcNode*)sNode->node.next )
        {
            IDE_TEST( addAggrListToText( sNode,
                                         aQueryBuf,
                                         aQueryBufSize,
                                         aQueryPosition,
                                         aAddedCount,
                                         aAddedTotal,
                                         aUnsupportedAggr )
                      != IDE_SUCCESS );
        }
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_SUBQ_EXISTS )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmvShardTransform::addAggrListToText",
                                  "Invalid shard transformation" ));
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmvShardTransform::addSumMinMaxCountToText( qtcNode        * aNode,
                                                   SChar          * aQueryBuf,
                                                   UInt             aQueryBufSize,
                                                   qcNamePosition * aQueryPosition )
{
/***********************************************************************
 *
 * Description : PROJ-2687 Shard aggregation transform
 *
 *               Aggregate function�� ���� ���� �л�θ� �̷�� 4�� function�� ���ؼ�
 *               Query text�� �����Ѵ�.
 *
 *
 * Implementation :
 *
 *                SUM(arg),   MIN(arg),   MAX(arg),   COUNT(arg)
 *           ->  "SUM(arg)", "MIN(arg)", "MAX(arg)", "COUNT(arg)"
 *
 ***********************************************************************/

    IDU_FIT_POINT_FATAL( "qmvShardTransform::addSumMinMaxToText::__FT__" );

    // ���ռ� �˻�
    IDE_FT_ASSERT ( ( aNode->node.module == &mtfSum ) ||
                    ( aNode->node.module == &mtfMin ) ||
                    ( aNode->node.module == &mtfMax ) ||
                    ( aNode->node.module == &mtfCount ) );

    aQueryPosition->size +=
        idlOS::snprintf( aQueryBuf + aQueryPosition->size,
                         aQueryBufSize - aQueryPosition->size,
                         "%.*s",
                         aNode->position.size,
                         aNode->position.stmtText + aNode->position.offset );

    return IDE_SUCCESS;
}

IDE_RC qmvShardTransform::addAvgToText( qtcNode        * aNode,
                                        SChar          * aQueryBuf,
                                        UInt             aQueryBufSize,
                                        qcNamePosition * aQueryPosition )
{
/***********************************************************************
 *
 * Description : PROJ-2687 Shard aggregation transform
 *
 *               Aggregate function�� ������ �ʿ��� AVG(arg)�� ���ؼ�
 *               SUM(arg),COUNT(arg) �� ���� �� query text�� �����Ѵ�.
 *
 * Implementation :
 *
 *                AVG(arg)  ->  "SUM(arg),COUNT(arg)"
 *
 ***********************************************************************/
    qtcNode * sArg = NULL;

    IDU_FIT_POINT_FATAL( "qmvShardTransform::addAvgToText::__FT__" );

    // ���ռ� �˻�
    IDE_FT_ASSERT ( aNode->node.module == &mtfAvg );

    sArg = (qtcNode*)aNode->node.arguments;

    // 1. "SUM("
    aQueryPosition->size +=
        idlOS::snprintf( aQueryBuf + aQueryPosition->size,
                         aQueryBufSize - aQueryPosition->size,
                         "SUM(" );

    // 2. "SUM(arg"
    if ( sArg->position.stmtText[sArg->position.offset - 1] == '"' )
    {
        aQueryBuf[aQueryPosition->size] = '"';
        aQueryPosition->size++;
    }
    else
    {
        // Nothing to do.
    }

    aQueryPosition->size +=
        idlOS::snprintf( aQueryBuf + aQueryPosition->size,
                         aQueryBufSize - aQueryPosition->size,
                         "%.*s",
                         sArg->position.size,
                         sArg->position.stmtText + sArg->position.offset );

    if ( sArg->position.stmtText[sArg->position.offset +
                                 sArg->position.size] == '"' )
    {
        aQueryBuf[aQueryPosition->size] = '"';
        aQueryPosition->size++;
    }
    else
    {
        // Nothing to do.
    }

    // 3. "SUM(arg)"
    aQueryBuf[aQueryPosition->size] = ')';
    aQueryPosition->size++;

    // 4. "SUM(arg),"
    aQueryBuf[aQueryPosition->size] = ',';
    aQueryPosition->size++;

    // 5. "SUM(arg),COUNT("
    aQueryPosition->size +=
        idlOS::snprintf( aQueryBuf + aQueryPosition->size,
                         aQueryBufSize - aQueryPosition->size,
                         "COUNT(" );

    // 6. "SUM(arg),COUNT(arg"
    if ( sArg->position.stmtText[sArg->position.offset - 1] == '"' )
    {
        aQueryBuf[aQueryPosition->size] = '"';
        aQueryPosition->size++;
    }
    else
    {
        // Nothing to do.
    }

    aQueryPosition->size +=
        idlOS::snprintf( aQueryBuf + aQueryPosition->size,
                         aQueryBufSize - aQueryPosition->size,
                         "%.*s",
                         sArg->position.size,
                         sArg->position.stmtText + sArg->position.offset );

    if ( sArg->position.stmtText[sArg->position.offset +
                                 sArg->position.size] == '"' )
    {
        aQueryBuf[aQueryPosition->size] = '"';
        aQueryPosition->size++;
    }
    else
    {
        // Nothing to do.
    }

    // 7. "SUM(arg),COUNT(arg)"
    aQueryBuf[aQueryPosition->size] = ')';
    aQueryPosition->size++;

    return IDE_SUCCESS;
}

IDE_RC qmvShardTransform::getFromEnd( qmsFrom * aFrom,
                                      UInt    * aFromWhereEnd )
{
/***********************************************************************
 *
 * Description : PROJ-2687 Shard aggregation transform
 *
 *               From tree�� ��ȸ�ϸ� query string�� ������ ��ġ�� �ش��ϴ� from��
 *               End position�� ã�� ��ȯ�Ѵ�.
 *
 * Implementation :
 *
 ***********************************************************************/
    UInt sThisIsTheEnd = 0;

    IDU_FIT_POINT_FATAL( "qmvShardTransform::getFromEnd::__FT__" );

    if ( aFrom != NULL )
    {
        /* ON clause�� �����ϸ� on clause�� end position�� ����Ѵ�. */
        if ( aFrom->onCondition != NULL )
        {
            sThisIsTheEnd = aFrom->onCondition->position.offset + aFrom->onCondition->position.size;

            if ( aFrom->onCondition->position.stmtText[aFrom->onCondition->position.offset +
                                                       aFrom->onCondition->position.size] == '"' )
            {
                sThisIsTheEnd++;
            }
            else
            {
                // Nothing to do.
            }
            if ( *aFromWhereEnd < sThisIsTheEnd )
            {
                /*
                 * From tree�� ��ȸ�ϴ� ���� ��ϵ� from�� end position����
                 * �� ū(�� �ڿ� �����ϴ�) end position�� ��� ���� ����
                 */
                *aFromWhereEnd = sThisIsTheEnd;
            }
            else
            {
                // Nothing to do.
            }
        }
        else
        {
            /* ON clause�� �������� ������ tableRef�� end position�� ã�´�. */
            IDE_DASSERT( aFrom->tableRef != NULL );

            if ( QC_IS_NULL_NAME(aFrom->tableRef->aliasName) == ID_FALSE )
            {
                if ( aFrom->tableRef->aliasName.
                     stmtText[aFrom->tableRef->aliasName.offset +
                              aFrom->tableRef->aliasName.size] == '"' )
                {
                    sThisIsTheEnd = aFrom->tableRef->aliasName.offset +
                        aFrom->tableRef->aliasName.size + 1;
                }
                else if ( aFrom->tableRef->aliasName.
                          stmtText[aFrom->tableRef->aliasName.offset +
                                   aFrom->tableRef->aliasName.size] == '\'' )
                {
                    sThisIsTheEnd = aFrom->tableRef->aliasName.offset +
                        aFrom->tableRef->aliasName.size + 1;
                }
                else
                {
                    sThisIsTheEnd = aFrom->tableRef->aliasName.offset +
                        aFrom->tableRef->aliasName.size;
                }
            }
            else
            {
                sThisIsTheEnd = aFrom->tableRef->position.offset +
                    aFrom->tableRef->position.size;

                if ( aFrom->tableRef->position.
                     stmtText[aFrom->tableRef->position.offset +
                              aFrom->tableRef->position.size] == '"' )
                {
                    sThisIsTheEnd++;
                }
                else
                {
                    // Nothing to do.
                }
            }

            if ( *aFromWhereEnd < sThisIsTheEnd )
            {
                *aFromWhereEnd = sThisIsTheEnd;
            }
            else
            {
                // Nothing to do.
            }
        }

        // Traverse
        IDE_TEST( getFromEnd( aFrom->left,
                              aFromWhereEnd )
                  != IDE_SUCCESS );

        IDE_TEST( getFromEnd( aFrom->right,
                              aFromWhereEnd )
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

IDE_RC qmvShardTransform::modifyOrgAggr( qcStatement  * aStatement,
                                         qtcNode     ** aNode,
                                         UInt         * aViewTargetOrder )
{
/***********************************************************************
 *
 * Description : PROJ-2687 Shard aggregation transform
 *
 *               Original query block��
 *               SELECT, HAVING clause�� �����ϴ� aggregate function�� ���ؼ�
 *               ���պ�(coord-aggregation) aggregate function���� �����Ѵ�.
 *
 * Implementation :
 *
 ***********************************************************************/
    qtcNode ** sNode;

    IDU_FIT_POINT_FATAL( "qmvShardTransform::modifyOrgAggr::__FT__" );

    if ( QTC_IS_AGGREGATE(*aNode) == ID_TRUE )
    {
        IDE_TEST( changeAggrExpr( aStatement,
                                  aNode,
                                  aViewTargetOrder )
                  != IDE_SUCCESS );
    }
    else
    {
        for ( sNode  = (qtcNode**)&(*aNode)->node.arguments;
              *sNode != NULL;
              sNode  = (qtcNode**)&(*sNode)->node.next )
        {
            IDE_TEST( modifyOrgAggr( aStatement,
                                     sNode,
                                     aViewTargetOrder )
                      != IDE_SUCCESS );
        }
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmvShardTransform::changeAggrExpr( qcStatement  * aStatement,
                                          qtcNode     ** aNode,
                                          UInt         * aViewTargetOrder )
{
/***********************************************************************
 *
 * Description : PROJ-2687 Shard aggregation transform
 *
 *               Aggregate function�� transformation�� ����
 *
 *               SUM(expression)   ->  SUM(column_module)
 *                                         -------------
 *                                          for SUM(arg)
 *
 *               MIN(expression)   ->  MIN(column_module)
 *                                         -------------
 *                                          for MIN(arg)
 *
 *               MAX(expression)   ->  MAX(column_module)
 *                                         -------------
 *                                          for MAX(arg)
 *
 *               COUNT(expression) ->  SUM(column_module)
 *                                         -------------
 *                                          for COUNT(arg)
 *
 *               AVG(expression)   ->  SUM(column_module) / SUM(column_module)
 *                                         -------------        -------------
 *                                          for SUM(arg)         for COUNT(arg)
 *
 *               * column_module�� makeNode�� ���� ���Ƿ� �����Ѵ�.
 *                 column_module�� ������ node��
 *                 �л���� �ش� aggr�� ���� column order��
 *                 shardViewTargetPos�� ����Ѵ�.
 *
 * Implementation :
 *
 ***********************************************************************/
    qcNamePosition sNullPosition;

    qtcNode * sOrgNode;

    qtcNode * sSum1[2];
    qtcNode * sSum2[2];
    qtcNode * sDivide[2];

    qtcNode * sColumn1[2];
    qtcNode * sColumn2[2];

    IDU_FIT_POINT_FATAL( "qmvShardTransform::changeAggrExpr::__FT__" );

    SET_EMPTY_POSITION( sNullPosition );

    sOrgNode = *aNode;

    // ���ռ� �˻�
    IDE_FT_ASSERT ( ( sOrgNode->node.module == &mtfSum ) ||
                    ( sOrgNode->node.module == &mtfMin ) ||
                    ( sOrgNode->node.module == &mtfMax ) ||
                    ( sOrgNode->node.module == &mtfCount ) ||
                    ( sOrgNode->node.module == &mtfAvg ) );

    if ( ( sOrgNode->node.module == &mtfSum ) ||
         ( sOrgNode->node.module == &mtfMin ) ||
         ( sOrgNode->node.module == &mtfMax ) )
    {
        /*
         * before)
         *
         *         AGGR_FUNC
         *             |
         *         expression
         *             |
         *            ...
         *
         * after)
         *
         *         AGGR_FUNC
         *             |
         *        column_node
         *
         */
        IDE_TEST( qtc::makeNode( aStatement,
                                 sColumn1,
                                 &sNullPosition,
                                 &qtc::columnModule )
                  != IDE_SUCCESS );

        sColumn1[0]->lflag = 0;

        sColumn1[0]->shardViewTargetPos = (*aViewTargetOrder)++;
        sColumn1[0]->lflag &= ~QTC_NODE_SHARD_VIEW_TARGET_REF_MASK;
        sColumn1[0]->lflag |= QTC_NODE_SHARD_VIEW_TARGET_REF_TRUE;

        sOrgNode->node.arguments = &sColumn1[0]->node;

        sOrgNode->node.arguments->next = NULL;
        sOrgNode->node.arguments->arguments = NULL;

        sOrgNode->node.lflag &= ~MTC_NODE_ARGUMENT_COUNT_MASK;
        sOrgNode->node.lflag |= 1;
    }
    else if ( sOrgNode->node.module == &mtfCount )
    {
        /*
         * before)
         *
         *          COUNT()
         *             |
         *         expression
         *             |
         *            ...
         *
         * after)
         *
         *           SUM()
         *             |
         *        column_node
         *
         */
        IDE_TEST( qtc::makeNode( aStatement,
                                 sSum1,
                                 &sNullPosition,
                                 &mtfSum )
                  != IDE_SUCCESS );

        IDE_TEST( qtc::makeNode( aStatement,
                                 sColumn1,
                                 &sNullPosition,
                                 &qtc::columnModule )
                  != IDE_SUCCESS );

        sColumn1[0]->lflag = 0;

        sColumn1[0]->shardViewTargetPos = (*aViewTargetOrder)++;
        sColumn1[0]->lflag &= ~QTC_NODE_SHARD_VIEW_TARGET_REF_MASK;
        sColumn1[0]->lflag |= QTC_NODE_SHARD_VIEW_TARGET_REF_TRUE;

        sSum1[0]->node.arguments = &sColumn1[0]->node;

        sSum1[0]->node.next = sOrgNode->node.next;
        sSum1[0]->node.arguments->next = NULL;
        sSum1[0]->node.arguments->arguments = NULL;

        sSum1[0]->node.lflag &= ~MTC_NODE_ARGUMENT_COUNT_MASK;
        sSum1[0]->node.lflag |= 1;

        SET_POSITION( sSum1[0]->position, sOrgNode->position );
        SET_POSITION( sSum1[0]->userName, sOrgNode->userName );
        SET_POSITION( sSum1[0]->tableName, sOrgNode->tableName );
        SET_POSITION( sSum1[0]->columnName, sOrgNode->columnName );

        *aNode = sSum1[0];
    }
    else if ( sOrgNode->node.module == &mtfAvg )
    {
        /*
         * before)
         *
         *           AVG()
         *             |
         *         expression
         *             |
         *            ...
         *
         * after)
         *
         *          DIVIDE()
         *             |
         *            SUM()-------SUM()
         *             |            |
         *        column_node   column_node
         *
         */
        IDE_TEST( qtc::makeNode( aStatement,
                                 sDivide,
                                 &sNullPosition,
                                 &mtfDivide )
                  != IDE_SUCCESS );

        IDE_TEST( qtc::makeNode( aStatement,
                                 sSum1,
                                 &sNullPosition,
                                 &mtfSum )
                  != IDE_SUCCESS );

        IDE_TEST( qtc::makeNode( aStatement,
                                 sColumn1,
                                 &sNullPosition,
                                 &qtc::columnModule )
                  != IDE_SUCCESS );

        sColumn1[0]->lflag = 0;

        sColumn1[0]->shardViewTargetPos = (*aViewTargetOrder)++;
        sColumn1[0]->lflag &= ~QTC_NODE_SHARD_VIEW_TARGET_REF_MASK;
        sColumn1[0]->lflag |= QTC_NODE_SHARD_VIEW_TARGET_REF_TRUE;

        sSum1[0]->node.arguments = &sColumn1[0]->node;

        sSum1[0]->node.next = NULL;
        sSum1[0]->node.arguments->next = NULL;
        sSum1[0]->node.arguments->arguments = NULL;

        IDE_TEST( qtc::makeNode( aStatement,
                                 sSum2,
                                 &sNullPosition,
                                 &mtfSum )
                  != IDE_SUCCESS );

        IDE_TEST( qtc::makeNode( aStatement,
                                 sColumn2,
                                 &sNullPosition,
                                 &qtc::columnModule )
                  != IDE_SUCCESS );

        sColumn2[0]->lflag = 0;

        sColumn2[0]->shardViewTargetPos = (*aViewTargetOrder)++;
        sColumn2[0]->lflag &= ~QTC_NODE_SHARD_VIEW_TARGET_REF_MASK;
        sColumn2[0]->lflag |= QTC_NODE_SHARD_VIEW_TARGET_REF_TRUE;

        sSum2[0]->node.arguments = &sColumn2[0]->node;

        sSum2[0]->node.next = NULL;
        sSum2[0]->node.arguments->next = NULL;
        sSum2[0]->node.arguments->arguments = NULL;

        sDivide[0]->node.next = sOrgNode->node.next;
        sDivide[0]->node.arguments = &sSum1[0]->node;
        sDivide[0]->node.arguments->next = &sSum2[0]->node;

        sSum1[0]->node.lflag &= ~MTC_NODE_ARGUMENT_COUNT_MASK;
        sSum1[0]->node.lflag |= 1;

        sSum2[0]->node.lflag &= ~MTC_NODE_ARGUMENT_COUNT_MASK;
        sSum2[0]->node.lflag |= 1;

        sDivide[0]->node.lflag &= ~MTC_NODE_ARGUMENT_COUNT_MASK;
        sDivide[0]->node.lflag |= 2;

        SET_POSITION( sDivide[0]->position, sOrgNode->position );
        SET_POSITION( sDivide[0]->userName, sOrgNode->userName );
        SET_POSITION( sDivide[0]->tableName, sOrgNode->tableName );
        SET_POSITION( sDivide[0]->columnName, sOrgNode->columnName );

        *aNode = sDivide[0];
    }
    else
    {
        IDE_FT_ASSERT(0);
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmvShardTransform::doTransformForExpr( qcStatement  * aStatement,
                                              qtcNode      * aExpr )
{
/***********************************************************************
 *
 * Description : Shard View Transform
 *     expression�� shard table�� ���Ե� subquery�� �ִ� ��� ��ȯ�Ѵ�.
 *
 * Implementation :
 *
 ***********************************************************************/

    qmmInsParseTree  * sInsParseTree;
    qmmUptParseTree  * sUptParseTree;
    qmmDelParseTree  * sDelParseTree;
    qsExecParseTree  * sExecParseTree;
    sdiObjectInfo    * sShardObjInfo = NULL;
    idBool             sTransform = ID_FALSE;

    IDU_FIT_POINT_FATAL( "qmvShardTransform::doTransformForExpr::__FT__" );

    //------------------------------------------
    // ���ռ� �˻�
    //------------------------------------------

    IDE_FT_ASSERT( aStatement != NULL );

    //------------------------------------------
    // Shard View Transform ����
    //------------------------------------------

    if ( ( ( QC_SHARED_TMPLATE(aStatement)->flag & QC_TMP_SHARD_TRANSFORM_MASK )
           == QC_TMP_SHARD_TRANSFORM_ENABLE ) &&
         ( ( ( aStatement->mFlag & QC_STMT_SHARD_OBJ_MASK ) == QC_STMT_SHARD_OBJ_EXIST ) ||
           ( aStatement->myPlan->parseTree->stmtShard != QC_STMT_SHARD_NONE ) ) &&
         ( aStatement->myPlan->parseTree->stmtShard != QC_STMT_SHARD_META ) &&
         ( aStatement->spvEnv->createPkg == NULL ) &&
         ( aStatement->spvEnv->createProc == NULL ) &&
         ( qcg::isShardCoordinator( aStatement ) == ID_TRUE ) )
    {
        switch ( aStatement->myPlan->parseTree->stmtKind )
        {
            case QCI_STMT_INSERT:
                sInsParseTree = (qmmInsParseTree*) aStatement->myPlan->parseTree;
                sShardObjInfo = sInsParseTree->tableRef->mShardObjInfo;
                if ( ( aStatement->myPlan->parseTree->optimize == qmo::optimizeShardInsert ) ||
                     ( sShardObjInfo == NULL ) )
                {
                    sTransform = ID_TRUE;
                }
                else
                {
                    // Nothing to do.
                }
                break;

            case QCI_STMT_UPDATE:
                sUptParseTree = (qmmUptParseTree *)aStatement->myPlan->parseTree;
                if ( sUptParseTree->querySet->SFWGH->from != NULL )
                {
                    sShardObjInfo = sUptParseTree->querySet->SFWGH->from->tableRef->mShardObjInfo;
                }
                else
                {
                    // Nothing to do.
                }
                if ( sShardObjInfo == NULL )
                {
                    sTransform = ID_TRUE;
                }
                else
                {
                    // Nothing to do.
                }
                break;

            case QCI_STMT_DELETE:
                sDelParseTree = (qmmDelParseTree *)aStatement->myPlan->parseTree;
                if ( sDelParseTree->querySet->SFWGH->from != NULL )
                {
                    sShardObjInfo = sDelParseTree->querySet->SFWGH->from->tableRef->mShardObjInfo;
                }
                else
                {
                    /* Nothing to do */
                }
                if ( sShardObjInfo == NULL )
                {
                    sTransform = ID_TRUE;
                }
                else
                {
                    // Nothing to do.
                }
                break;

            case QCI_STMT_EXEC_PROC:
                sExecParseTree = (qsExecParseTree *)aStatement->myPlan->parseTree;
                sShardObjInfo = sExecParseTree->mShardObjInfo;
                if ( sShardObjInfo == NULL )
                {
                    sTransform = ID_TRUE;
                }
                else
                {
                    // Nothing to do.
                }
                break;

            default:
                break;
        }

        // shardInsert�� ��ȯ�� ����� value�� subquery�� ���� ���
        // DML�� set, where�� ��� subquery�� ���� ���
        if ( sTransform == ID_TRUE )
        {
            IDE_TEST( processTransformForExpr( aStatement,
                                               aExpr )
                      != IDE_SUCCESS );
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
}

IDE_RC qmvShardTransform::processTransformForFrom( qcStatement  * aStatement,
                                                   qmsFrom      * aFrom )
{
/***********************************************************************
 *
 * Description : Shard View Transform
 *     from���� shard table�� shard view�� ��ȯ�Ѵ�.
 *
 * Implementation :
 *
 ***********************************************************************/

    qmsTableRef     * sTableRef;
    sdiAnalyzeInfo  * sShardAnalysis;

    IDU_FIT_POINT_FATAL( "qmvShardTransform::processTransformForFrom::__FT__" );

    if ( aFrom->joinType == QMS_NO_JOIN )
    {
        sTableRef = aFrom->tableRef;

        if ( sTableRef->view != NULL )
        {
            // view
            IDE_TEST( processTransform( sTableRef->view ) != IDE_SUCCESS );
        }
        else
        {
            // table
            if ( sTableRef->mShardObjInfo != NULL )
            {
                IDE_TEST( STRUCT_ALLOC( QC_QMP_MEM(aStatement),
                                        sdiAnalyzeInfo,
                                        &sShardAnalysis )
                          != IDE_SUCCESS );

                // analyzer�� ������ �ʰ� ���� analyze ������ �����Ѵ�.
                IDE_TEST( sdi::setAnalysisResultForTable( aStatement,
                                                          sShardAnalysis,
                                                          sTableRef->mShardObjInfo )
                          != IDE_SUCCESS );

                // table�� shard view�� ��ȯ�Ѵ�.
                IDE_TEST( makeShardView( aStatement,
                                         sTableRef,
                                         sShardAnalysis )
                          != IDE_SUCCESS );
            }
            else
            {
                // Nothing to do.
            }
        }
    }
    else
    {
        IDE_TEST( processTransformForFrom( aStatement,
                                           aFrom->left )
                  != IDE_SUCCESS );

        IDE_TEST( processTransformForFrom( aStatement,
                                           aFrom->right )
                  != IDE_SUCCESS );

        IDE_TEST( processTransformForExpr( aStatement,
                                           aFrom->onCondition )
                  != IDE_SUCCESS );
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmvShardTransform::processTransformForExpr( qcStatement  * aStatement,
                                                   qtcNode      * aExpr )
{
/***********************************************************************
 *
 * Description : Shard View Transform
 *     expression�� subquery�� shard view�� ��ȯ�Ѵ�.
 *
 * Implementation :
 *
 ***********************************************************************/

    qcStatement    * sSubQStatement = NULL;
    qtcNode        * sNode = NULL;

    IDU_FIT_POINT_FATAL( "qmvShardTransform::processTransformForExpr::__FT__" );

    if ( ( aExpr->node.lflag & MTC_NODE_OPERATOR_MASK )
         == MTC_NODE_OPERATOR_SUBQUERY )
    {
        sSubQStatement = aExpr->subquery;

        IDE_TEST( processTransform( sSubQStatement ) != IDE_SUCCESS );
    }
    else
    {
        // traverse
        for ( sNode = (qtcNode*)aExpr->node.arguments;
              sNode != NULL;
              sNode = (qtcNode*)sNode->node.next )
        {
            IDE_TEST( processTransformForExpr( aStatement, sNode )
                      != IDE_SUCCESS );
        }
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmvShardTransform::raiseInvalidShardQuery( qcStatement  * aStatement )
{
/***********************************************************************
 *
 * Description : Shard Transform ������ ����Ѵ�.
 *
 * Implementation :
 *
 ***********************************************************************/

    idBool             sIsShardQuery = ID_FALSE;
    sdiAnalyzeInfo   * sShardAnalysis = NULL;
    UShort             sShardParamOffset = ID_USHORT_MAX;
    UShort             sShardParamCount = 0;
    qcuSqlSourceInfo   sqlInfo;

    IDU_FIT_POINT_FATAL( "qmvShardTransform::setErrorMsg::__FT__" );

    if ( QC_IS_NULL_NAME( aStatement->myPlan->parseTree->stmtPos ) == ID_FALSE )
    {
        IDE_TEST( isShardQuery( aStatement,
                                & aStatement->myPlan->parseTree->stmtPos,
                                & sIsShardQuery,
                                & sShardAnalysis,
                                & sShardParamOffset,
                                & sShardParamCount )
                  != IDE_SUCCESS );

        if ( sIsShardQuery == ID_FALSE )
        {
            sqlInfo.setSourceInfo( aStatement,
                                   & aStatement->myPlan->parseTree->stmtPos );
            IDE_RAISE( ERR_INVALID_SHARD_QUERY );
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

    IDE_EXCEPTION( ERR_INVALID_SHARD_QUERY )
    {
        (void)sqlInfo.initWithBeforeMessage(aStatement->qmeMem);
        IDE_SET( ideSetErrorCode( sdERR_ABORT_INVALID_SHARD_QUERY,
                                  sqlInfo.getBeforeErrMessage(),
                                  sqlInfo.getErrMessage() ) );
        (void)sqlInfo.fini();
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}
