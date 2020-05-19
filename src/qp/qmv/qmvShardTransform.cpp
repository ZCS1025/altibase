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
 *     shard table이 포함된 쿼리에서 명시적인 shard view가 아니거나,
 *     shard 키워드가 없는 쿼리를 shard view나 shard 쿼리로
 *     변환한다.
 *
 *     예1) top query, query 전체가 shard query인 경우
 *          select * from t1 where i1=1 order by i1;
 *          --> select * from shard(select * from t1 where i1=1 order by i1);
 *
 *     예2) view가 shard query인 경우
 *          select * from (select * from t1 where i1=1);
 *          --> select * from shard(select * from t1 where i1=1);
 *
 *     예3) querySet이 shard query인 경우
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
 *     예4) from-where가 shard query인 경우 (미구현)
 *          select func1(i1) from t1 where i1=1;
 *          --> select func1(i1) from (select * from t1 where i1=1);
 *
 *          select * from t1, t2 where t1.i1=t2.i1 and t1.i1=1;
 *          --> select * from (select * from t1 where t1.i1=1) v1, t2 where v1.i1=t2.i1;
 *
 *     예5) from만 shard table인 경우
 *          select * from t1, t2 where t1.i1=t2.i1 and t1.i1=1;
 *          --> select * from (select * from t1) v1, t2 where v1.i1=t2.i1 and v1.i1=1;
 *
 *     예6) DML, query 전체가 shard query인 경우
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
    // 적합성 검사
    //------------------------------------------

    IDE_FT_ASSERT( aStatement != NULL );

    //------------------------------------------
    // Shard View Transform 수행
    //------------------------------------------

    // shard_meta는 변환하지 않는다.
    if ( ( sdi::isShardCoordinator( aStatement ) == ID_TRUE ) ||
         ( sdi::isRebuildCoordinator( aStatement ) == ID_TRUE ) )
    {
        if ( ( aStatement->spvEnv->createPkg == NULL ) &&
             ( aStatement->spvEnv->createProc == NULL ) &&
             ( aStatement->myPlan->parseTree->stmtShard != QC_STMT_SHARD_META ) )
        {
            if ( ( ( aStatement->mFlag & QC_STMT_SHARD_OBJ_MASK ) == QC_STMT_SHARD_OBJ_EXIST ) ||
                 ( aStatement->myPlan->parseTree->stmtShard != QC_STMT_SHARD_NONE ) )
            {
                if ( ( QC_SHARED_TMPLATE(aStatement)->flag & QC_TMP_SHARD_TRANSFORM_MASK )
                     == QC_TMP_SHARD_TRANSFORM_ENABLE )
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
            }
            else
            {
                /* BUG-45899 */
                // shard keyword 가 없고 shard object 가 없는 경우
                // select * from normal_table
                sdi::setNonShardQueryReason( &(aStatement->mShardPrintInfo), SDI_NO_SHARD_OBJECT );
            }
        }
        else
        {
            /* BUG-45899 */
            sdi::setNonShardQueryReason( &(aStatement->mShardPrintInfo), SDI_UNSUPPORT_SHARD_QUERY );
        }
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
 *     top query block이나 subquery의 경우 inline view를 한번 더 씌워야 한다.
 *
 ***********************************************************************/

    qmsParseTree     * sParseTree;
    qmsSortColumns   * sCurrSort;
    idBool             sIsShardQuery  = ID_FALSE;
    sdiAnalyzeInfo   * sShardAnalysis = NULL;
    UShort             sShardParamOffset = ID_USHORT_MAX;
    UShort             sShardParamCount = 0;
    qcuSqlSourceInfo   sqlInfo;

    /* BUG-45899 */
    idBool sIsCanMerge = ID_FALSE;
    idBool sIsTransformable = ID_FALSE;
    UShort sNonShardQueryReason = SDI_CAN_NOT_MERGE_REASON_MAX;

    /* PROJ-2701 Online data rebuild */
    ULong sTransformSMN = ID_ULONG(0);
    ULong sSessionSMN   = ID_ULONG(0);

    IDU_FIT_POINT_FATAL( "qmvShardTransform::processTransform::__FT__" );

    //------------------------------------------
    // 적합성 검사
    //------------------------------------------

    IDE_FT_ASSERT( aStatement != NULL );

    //------------------------------------------
    // 초기화
    //------------------------------------------

    sParseTree = (qmsParseTree *) aStatement->myPlan->parseTree;

    //------------------------------------------
    // Coordinator type에 따른 수행결정
    //------------------------------------------
    if ( sdi::isRebuildCoordinator( aStatement ) == ID_TRUE )
    {
        // rebuild coordinator
        sTransformSMN = sdi::getSMNForDataNode(); /* dataSMN */
        sSessionSMN   = QCG_GET_SESSION_SHARD_META_NUMBER( aStatement ); /* sessionSMN */

        // process rebuild transformation
        IDE_TEST( rebuildTransform( aStatement,
                                    &sParseTree->common.stmtPos,
                                    sSessionSMN,
                                    sTransformSMN )
                  != IDE_SUCCESS );
    }
    else
    {
        // shard coordinator
        sTransformSMN = QCG_GET_SESSION_SHARD_META_NUMBER( aStatement ); /* sessionSMN */

        //------------------------------------------
        // Shard View Transform의 수행
        //------------------------------------------

        switch ( sParseTree->common.stmtShard )
        {
            case QC_STMT_SHARD_NONE:
            {
                if ( QC_IS_NULL_NAME( sParseTree->common.stmtPos ) == ID_FALSE )
                {
                    // shard query인지 검사한다.
                    IDE_TEST( isShardQuery( aStatement,
                                            & sParseTree->common.stmtPos,
                                              sTransformSMN,
                                            & sIsShardQuery,
                                            & sShardAnalysis,
                                            & sShardParamOffset,
                                            & sShardParamCount )
                              != IDE_SUCCESS );

                    if ( sIsShardQuery == ID_TRUE )
                    {
                        if ( sParseTree->isView == ID_TRUE )
                        {
                            // view인 경우 shard view로 변경한다.
                            sParseTree->common.stmtShard = QC_STMT_SHARD_ANALYZE;

                            // 분석결과를 기록한다.
                            aStatement->myPlan->mShardAnalysis = sShardAnalysis;
                            aStatement->myPlan->mShardParamOffset = sShardParamOffset;
                            aStatement->myPlan->mShardParamCount = sShardParamCount;
                        }
                        else
                        {
                            // top query이거나 subquery인 경우 shard view를 생성한다.
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
                                                           sParseTree->querySet,
                                                           sTransformSMN )
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

                    // loop (subquery가 허용되지는 않으나 에러가 잘못발생한다.)
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
                // select에서 명시적으로 사용된 shard view는
                // shard query가 아니더라도 허용한다.
                if ( aStatement->myPlan->mShardAnalysis == NULL )
                {
                    IDE_FT_ASSERT( sParseTree->common.stmtPos.size > 0 );

                    IDE_TEST( isShardQuery( aStatement,
                                            & sParseTree->common.stmtPos,
                                              sTransformSMN,
                                            & sIsShardQuery,
                                            & sShardAnalysis,
                                            & sShardParamOffset,
                                            & sShardParamCount )
                              != IDE_SUCCESS );

                    // 명시적인 shard query이나 analysis를 생성하지 못한 경우, 에러처리한다.
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
                        // top query인 경우 shard view를 생성한다.
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
                // select에서 명시적으로 사용된 shard view는
                // shard query가 아니더라도 허용한다.
                if ( aStatement->myPlan->mShardAnalysis == NULL )
                {
                    IDE_FT_ASSERT( sParseTree->common.stmtPos.size > 0 );

                    IDE_TEST( isShardQuery( aStatement,
                                            & sParseTree->common.stmtPos,
                                              sTransformSMN,
                                            & sIsShardQuery,
                                            & sShardAnalysis,
                                            & sShardParamOffset,
                                            & sShardParamCount )
                              != IDE_SUCCESS );

                    if ( sParseTree->common.nodes == NULL )
                    {
                        // 분석결과에 상관없이 전노드 분석결과로 교체한다.
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
                            /* BUG-45899 */
                            sIsCanMerge = sShardAnalysis->mIsCanMerge;
                            sNonShardQueryReason = sShardAnalysis->mNonShardQueryReason;
                            sIsTransformable = sShardAnalysis->mIsTransformAble;
                        }

                        // BUG-45359
                        // 특정 데이터 노드로 분석결과를 생성한다.
                        SDI_INIT_ANALYZE_INFO( sShardAnalysis );

                        sShardAnalysis->mSplitMethod = SDI_SPLIT_NODES;
                        sShardAnalysis->mNodeNames = sParseTree->common.nodes;

                        /* BUG-45899 */
                        sShardAnalysis->mIsCanMerge = sIsCanMerge;
                        sShardAnalysis->mNonShardQueryReason = sNonShardQueryReason;
                        sShardAnalysis->mIsTransformAble = sIsTransformable;
                    }

                    if ( sParseTree->isView == ID_TRUE )
                    {
                        aStatement->myPlan->mShardAnalysis = sShardAnalysis;
                        aStatement->myPlan->mShardParamOffset = sShardParamOffset;
                        aStatement->myPlan->mShardParamCount = sShardParamCount;
                    }
                    else
                    {
                        // top query인 경우 shard view를 생성한다.
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
    }

    /* BUG-45899 */
    if ( QC_SHARED_TMPLATE(aStatement)->stmt->myPlan->mShardAnalysis != NULL )
    {
        sdi::setPrintInfoFromAnalyzeInfo(
                &(QC_SHARED_TMPLATE(aStatement)->stmt->mShardPrintInfo),
                QC_SHARED_TMPLATE(aStatement)->stmt->myPlan->mShardAnalysis );
    }
    else
    {
        // 다음의 경우 aStatement->myPlan->mShardAnalysis 는 NULL
        // select * from t1 where i1 > 1 order by i2;
        // select count(*) from t1;
        if ( sShardAnalysis != NULL )
        {
            sdi::setPrintInfoFromAnalyzeInfo(
                &(QC_SHARED_TMPLATE(aStatement)->stmt->mShardPrintInfo),
                sShardAnalysis );
        }
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
                                                       qmsQuerySet  * aQuerySet,
                                                       ULong          aTransformSMN )
{
/***********************************************************************
 *
 * Description : Shard View Transform
 *     query set에 대하여 top-up으로 순회하며 Shard View Transform을 수행한다.
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
    // 적합성 검사
    //------------------------------------------

    IDE_FT_ASSERT( aStatement != NULL );
    IDE_FT_ASSERT( aQuerySet  != NULL );

    //------------------------------------------
    // 초기화
    //------------------------------------------

    sQuerySet = aQuerySet;

    //------------------------------------------
    // Shard View Transform의 수행
    //------------------------------------------

    // shard querySet인지 검사한다.
    if ( ( QC_IS_NULL_NAME( sQuerySet->startPos ) == ID_FALSE ) &&
         ( QC_IS_NULL_NAME( sQuerySet->endPos ) == ID_FALSE ) )
    {
        // startPos는 첫번째 token
        // endPos는 마지막 token
        sParsePosition.stmtText = sQuerySet->startPos.stmtText;
        sParsePosition.offset   = sQuerySet->startPos.offset;
        sParsePosition.size     =
            sQuerySet->endPos.offset + sQuerySet->endPos.size -
            sQuerySet->startPos.offset;

        IDE_TEST( isShardQuery( aStatement,
                                & sParsePosition,
                                aTransformSMN,
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
                if ( ( SDU_SHARD_AGGREGATION_TRANSFORM_DISABLE == 0 ) &&
                     ( sShardAnalysis->mIsTransformAble == ID_TRUE ) )
                {
                    IDE_TEST( isTransformAbleQuery( aStatement,
                                                    &sParsePosition,
                                                    aTransformSMN,
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
                                                   sQuerySet->left,
                                                   aTransformSMN )
                      != IDE_SUCCESS );

            // right subquery
            IDE_TEST( processTransformForQuerySet( aStatement,
                                                   sQuerySet->right,
                                                   aTransformSMN )
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
 *     DML의 경우 쿼리 전체를 검사한다.
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

    /* BUG-45899 */
    idBool sIsCanMerge = ID_FALSE;
    idBool sIsTransformable = ID_FALSE;
    UShort sNonShardQueryReason = SDI_CAN_NOT_MERGE_REASON_MAX;

    /* PROJ-2701 Online data rebuild */
    ULong              sTransformSMN = ID_ULONG(0);
    ULong              sSessionSMN   = ID_ULONG(0);

    IDU_FIT_POINT_FATAL( "qmvShardTransform::processTransformForDML::__FT__" );

    //------------------------------------------
    // 적합성 검사
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
    // Coordinator type에 따른 수행결정
    //------------------------------------------
    if ( sdi::isRebuildCoordinator( aStatement ) == ID_TRUE )
    {
        // rebuild coordinator
        sTransformSMN = sdi::getSMNForDataNode();
        sSessionSMN   = QCG_GET_SESSION_SHARD_META_NUMBER( aStatement );

        // process rebuild transformation
        IDE_TEST( rebuildTransform( aStatement,
                                    &aStatement->myPlan->parseTree->stmtPos,
                                    sSessionSMN,
                                    sTransformSMN )
                  != IDE_SUCCESS );
    }
    else
    {
        // shard coordinator
        sTransformSMN = QCG_GET_SESSION_SHARD_META_NUMBER( aStatement );

        //------------------------------------------
        // Shard View Transform의 수행
        //------------------------------------------

        switch ( aStatement->myPlan->parseTree->stmtShard )
        {
            case QC_STMT_SHARD_NONE:
            {
                if ( sShardObjInfo != NULL )
                {
                    IDE_FT_ASSERT( aStatement->myPlan->parseTree->stmtPos.size > 0 );

                    // shard object인 경우 분석결과가 필요하다.
                    IDE_TEST( isShardQuery( aStatement,
                                            & aStatement->myPlan->parseTree->stmtPos,
                                            sTransformSMN,
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

                            // multi-table insert가 아니라면 shardInsert로 변경한다.
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
                        // analysis를 생성하지 못한 경우, 에러처리한다.
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

                        // 함수를 변경한다.
                        aStatement->myPlan->parseTree->optimize = qmo::optimizeShardDML;
                        aStatement->myPlan->parseTree->execute  = qmx::executeShardDML;
                    }

                    // shard statement로 변경한다.
                    aStatement->myPlan->parseTree->stmtShard = QC_STMT_SHARD_ANALYZE;

                    // 분석결과를 기록한다.
                    aStatement->myPlan->mShardAnalysis = sShardAnalysis;
                    aStatement->myPlan->mShardParamOffset = sShardParamOffset;
                    aStatement->myPlan->mShardParamCount = sShardParamCount;
                }
                else
                {
                    // shard object가 아닌 경우
                    // Nothing to do.
                }

                break;
            }

            case QC_STMT_SHARD_ANALYZE:
            {
                // shard object가 아닌경우 SHARD keyword를 사용할 수 없다.
                IDE_TEST_RAISE( sShardObjInfo == NULL, ERR_NOT_SHARD_OBJECT );

                // insert DML에서는 명시적으로 사용한 shard인 경우라도 shard query를 검사한다.
                if ( aStatement->myPlan->mShardAnalysis == NULL )
                {
                    IDE_FT_ASSERT( aStatement->myPlan->parseTree->stmtPos.size > 0 );

                    IDE_TEST( isShardQuery( aStatement,
                                            & aStatement->myPlan->parseTree->stmtPos,
                                            sTransformSMN,
                                            & sIsShardQuery,
                                            & sShardAnalysis,
                                            & sShardParamOffset,
                                            & sShardParamCount )
                              != IDE_SUCCESS );

                    if ( sIsShardQuery == ID_FALSE )
                    {
                        // insert는 별도로 처리한다.
                        if ( aStatement->myPlan->parseTree->stmtKind == QCI_STMT_INSERT )
                        {
                            sInsParseTree = (qmmInsParseTree*) aStatement->myPlan->parseTree;

                            // multi-table insert는 제외
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
                        // exec proc는 insert처럼 반드시 실행노드가 결정되어야하는 것으로 본다.
                        else if ( aStatement->myPlan->parseTree->stmtKind == QCI_STMT_EXEC_PROC )
                        {
                            sqlInfo.setSourceInfo( aStatement,
                                                   & aStatement->myPlan->parseTree->stmtPos );
                            IDE_RAISE( ERR_INVALID_SHARD_QUERY );
                        }
                        else
                        {
                            // analysis를 생성하지 못한 경우, 에러처리한다.
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

                            // 함수를 변경한다.
                            aStatement->myPlan->parseTree->optimize = qmo::optimizeShardDML;
                            aStatement->myPlan->parseTree->execute  = qmx::executeShardDML;
                        }
                    }
                    else
                    {
                        // 함수를 변경한다.
                        aStatement->myPlan->parseTree->optimize = qmo::optimizeShardDML;
                        aStatement->myPlan->parseTree->execute  = qmx::executeShardDML;
                    }

                    // 분석결과를 기록한다.
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
                // 일단 DML은 지원하지 않는다.
                IDE_RAISE( ERR_UNSUPPORTED_SHARD_DATA_IN_DML );

                // shard object가 아닌경우 SHARD keyword를 사용할 수 없다.
                IDE_TEST_RAISE( sShardObjInfo == NULL, ERR_NOT_SHARD_OBJECT );

                if ( aStatement->myPlan->mShardAnalysis == NULL )
                {
                    IDE_FT_ASSERT( aStatement->myPlan->parseTree->stmtPos.size > 0 );

                    IDE_TEST( isShardQuery( aStatement,
                                            & aStatement->myPlan->parseTree->stmtPos,
                                            sTransformSMN,
                                            & sIsShardQuery,
                                            & sShardAnalysis,
                                            & sShardParamOffset,
                                            & sShardParamCount )
                              != IDE_SUCCESS );

                    if ( aStatement->myPlan->parseTree->nodes == NULL )
                    {
                        // 분석결과에 상관없이 전노드 분석결과로 교체한다.
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
                            /* BUG-45899 */
                            sIsCanMerge = sShardAnalysis->mIsCanMerge;
                            sNonShardQueryReason = sShardAnalysis->mNonShardQueryReason;
                            sIsTransformable = sShardAnalysis->mIsTransformAble;
                        }

                        // BUG-45359
                        // 특정 데이터 노드로 분석결과를 생성한다.
                        SDI_INIT_ANALYZE_INFO( sShardAnalysis );

                        sShardAnalysis->mSplitMethod = SDI_SPLIT_NODES;
                        sShardAnalysis->mNodeNames = aStatement->myPlan->parseTree->nodes;

                        /* BUG-45899 */
                        sShardAnalysis->mIsCanMerge = sIsCanMerge;
                        sShardAnalysis->mNonShardQueryReason = sNonShardQueryReason;
                        sShardAnalysis->mIsTransformAble = sIsTransformable;
                    }

                    // 함수를 변경한다.
                    aStatement->myPlan->parseTree->optimize = qmo::optimizeShardDML;
                    aStatement->myPlan->parseTree->execute  = qmx::executeShardDML;

                    // 분석결과를 기록한다.
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
    }

    /* BUG-45899 */
    if ( QC_SHARED_TMPLATE(aStatement)->stmt->myPlan->mShardAnalysis != NULL )
    {
        sdi::setPrintInfoFromAnalyzeInfo(
                &(QC_SHARED_TMPLATE(aStatement)->stmt->mShardPrintInfo),
                QC_SHARED_TMPLATE(aStatement)->stmt->myPlan->mShardAnalysis );
    }
    else
    {
        if ( sShardAnalysis != NULL )
        {
            sdi::setPrintInfoFromAnalyzeInfo(
                    &(QC_SHARED_TMPLATE(aStatement)->stmt->mShardPrintInfo),
                    sShardAnalysis );
        }
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
                                        ULong             aSMN,
                                        idBool          * aIsShardQuery,
                                        sdiAnalyzeInfo ** aShardAnalysis,
                                        UShort          * aShardParamOffset,
                                        UShort          * aShardParamCount )
{
/***********************************************************************
 *
 * Description : Shard View Transform
 *     Shard Query인지 검사한다.
 *
 * Implementation :
 *
 ***********************************************************************/

    UShort            sAllParamCount = 0;
    sdiAnalyzeInfo  * sAnalyzeInfo = NULL;
    UShort            sShardParamOffset = ID_USHORT_MAX;
    UShort            sShardParamCount = 0;
    UShort            i = 0;

    volatile qcStatement sStatement;
    volatile SInt        sStage;

    /* PROJ-2646 New shard analyzer */
    UShort            sShardValueIndex;
    UShort            sSubValueIndex;

    IDE_FT_BEGIN();

    IDU_FIT_POINT_FATAL( "qmvShardTransform::isShardQuery::__FT__" );

    sStage = 0; /* BUG-45994 - 컴파일러 최적화 회피 */

    //-----------------------------------------
    // 준비
    //-----------------------------------------

    // resolve를 위해 aStatement의 session이 필요하다.
    IDE_TEST( qcg::allocStatement( (qcStatement *)&sStatement,
                                   aStatement->session,
                                   NULL,
                                   NULL )
              != IDE_SUCCESS );
    sStage = 1;

    sStatement.myPlan->stmtText    = aParsePosition->stmtText;
    sStatement.myPlan->stmtTextLen = idlOS::strlen( aParsePosition->stmtText );

    qcg::setSmiStmt( (qcStatement *)&sStatement, QC_SMI_STMT(aStatement) );

    //-----------------------------------------
    // PARSING
    //-----------------------------------------

    // FT_END 이전으로 이동할때는 IDE 매크로를 사용해도 된다.
    IDE_TEST_CONT( qcpManager::parsePartialForAnalyze(
                       (qcStatement *)&sStatement,
                       aParsePosition->stmtText,
                       aParsePosition->offset,
                       aParsePosition->size ) != IDE_SUCCESS,
                   NORMAL_EXIT );

    // 분석대상인지 먼저 확인한다.
    IDE_TEST_CONT( sdi::checkStmt( (qcStatement *)&sStatement ) != IDE_SUCCESS,
                   NORMAL_EXIT );

    //-----------------------------------------
    // BIND (undef type)
    //-----------------------------------------

    sShardParamCount = qcg::getBindCount( (qcStatement *)&sStatement );

    for ( i = 0; i < sShardParamCount; i++ )
    {
        IDE_TEST_CONT( qcg::setBindColumn( (qcStatement *)&sStatement,
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

    QC_SHARED_TMPLATE((qcStatement *)&sStatement)->flag &= ~QC_TMP_SHARD_TRANSFORM_MASK;
    QC_SHARED_TMPLATE((qcStatement *)&sStatement)->flag |= QC_TMP_SHARD_TRANSFORM_DISABLE;

    IDE_TEST_CONT( sStatement.myPlan->parseTree->parse( (qcStatement *)&sStatement )
                   != IDE_SUCCESS, NORMAL_EXIT );

    IDE_TEST_CONT( qtc::fixAfterParsing( QC_SHARED_TMPLATE( (qcStatement *)&sStatement) )
                   != IDE_SUCCESS, NORMAL_EXIT );
    sStage = 2;

    IDE_TEST_CONT( sStatement.myPlan->parseTree->validate( (qcStatement *)&sStatement )
                   != IDE_SUCCESS, NORMAL_EXIT );

    IDE_TEST_CONT( qtc::fixAfterValidation( QC_QMP_MEM((qcStatement *)&sStatement),
                                            QC_SHARED_TMPLATE((qcStatement *)&sStatement) )
                   != IDE_SUCCESS, NORMAL_EXIT );

    QC_SHARED_TMPLATE((qcStatement *)&sStatement)->flag &= ~QC_TMP_SHARD_TRANSFORM_MASK;
    QC_SHARED_TMPLATE((qcStatement *)&sStatement)->flag |= QC_TMP_SHARD_TRANSFORM_ENABLE;

    // PROJ-2653
    // bind parameter ID 획득
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

    IDE_TEST_CONT( sdi::analyze( (qcStatement *)&sStatement,
                                 aSMN ) != IDE_SUCCESS,
                   NORMAL_EXIT );

    IDE_TEST_CONT( sdi::setAnalysisResult( (qcStatement *)&sStatement ) != IDE_SUCCESS,
                   NORMAL_EXIT );

    //-----------------------------------------
    // ANALYSIS RESULT
    //-----------------------------------------

    IDE_EXCEPTION_CONT( NORMAL_EXIT );

    // setAnalysisResult가 실패하더라도 mShardAnalysis는 생성될 수 있다.
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

            if ( sAnalyzeInfo->mSubKeyExists == ID_TRUE )
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

        *aIsShardQuery = sStatement.myPlan->mShardAnalysis->mIsCanMerge;
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

    /* BUG-45899 */
    QC_SHARED_TMPLATE(aStatement)->stmt->mShardPrintInfo.mAnalyzeCount
        += sStatement.mShardPrintInfo.mAnalyzeCount;

    if ( sStatement.myPlan->mShardAnalysis != NULL )
    {
        sdi::setPrintInfoFromAnalyzeInfo(
                &(QC_SHARED_TMPLATE(aStatement)->stmt->mShardPrintInfo),
                sStatement.myPlan->mShardAnalysis );
    }
    else
    {
        sdi::setPrintInfoFromPrintInfo(
                &(QC_SHARED_TMPLATE(aStatement)->stmt->mShardPrintInfo),
                (sdiPrintInfo *)(&(sStatement.mShardPrintInfo)) );
    }

    //-----------------------------------------
    // 마무리
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

    // session은 내것이 아니다.
    sStatement.session = NULL;

    (void) qcg::freeStatement((qcStatement *)&sStatement);

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
            (void) qcg::freeStatement( (qcStatement *)&sStatement );
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
 *     statement 전체를 shard view로 생성한다.
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

    // aStatement를 교체할 수 없으므로 sStatement를 복사 생성한다.
    idlOS::memcpy( sStatement, aStatement, ID_SIZEOF(qcStatement) );
    // myPlan을 재설정한다.
    sStatement->myPlan = & sStatement->privatePlan;

    sTableRef->view      = sStatement;
    sFrom->tableRef      = sTableRef;
    sSFWGH->from         = sFrom;
    sSFWGH->thisQuerySet = sQuerySet;
    sQuerySet->SFWGH     = sSFWGH;

    // parseTree를 생성한다.
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

    // aStatement의 parseTree를 변경한다.
    aStatement->myPlan->parseTree = (qcParseTree*) sParseTree;
    aStatement->myPlan->parseTree->stmtKind =
        sStatement->myPlan->parseTree->stmtKind;

    // sStatement를 shard view로 변경한다.
    SET_POSITION( sStatement->myPlan->parseTree->stmtPos, *aParsePosition );
    sStatement->myPlan->parseTree->stmtShard = aShardStmtType;

    // 분석결과를 기록한다.
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
 *     query set을 shard view로 생성한다.
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

    // aQuerySet를 교체할 수 없으므로 sQuerySet를 복사 생성한다.
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

    // sStatement를 shard view로 변경한다.
    SET_POSITION( sStatement->myPlan->parseTree->stmtPos, *aParsePosition );
    sStatement->myPlan->parseTree->stmtShard = QC_STMT_SHARD_ANALYZE;

    // 분석결과를 기록한다.
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
                                         sdiAnalyzeInfo * aShardAnalysis,
                                         qcNamePosition * aFilter )
{
/***********************************************************************
 *
 * Description : Shard View Transform
 *
 *     table을 shard view로 생성한다.
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
    SInt              sTransformStringMaxSize = (SDU_SHARD_TRANSFORM_STRING_LENGTH_MAX+1);

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
                                     sTransformStringMaxSize,
                                     &sQueryBuf)
             != IDE_SUCCESS);
    sQueryPosition.stmtText = sQueryBuf;
    sQueryPosition.offset = 0;
    sQueryPosition.size = idlOS::snprintf( sQueryBuf, sTransformStringMaxSize, "select * from " );
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
                         sTransformStringMaxSize - sQueryPosition.size,
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
                         sTransformStringMaxSize - sQueryPosition.size,
                         " as " );
    if ( aTableRef->aliasName.stmtText[aTableRef->aliasName.offset +
                                       aTableRef->aliasName.size] == '"' )
    {
        // t1 "a"
        sQueryPosition.size +=
        idlOS::snprintf( sQueryBuf + sQueryPosition.size,
                         sTransformStringMaxSize - sQueryPosition.size,
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
                         sTransformStringMaxSize - sQueryPosition.size,
                         "'%.*s'",
                         aTableRef->aliasName.size,
                         aTableRef->aliasName.stmtText + aTableRef->aliasName.offset );
    }
    else
    {
        // t1 a
        sQueryPosition.size +=
        idlOS::snprintf( sQueryBuf + sQueryPosition.size,
                         sTransformStringMaxSize - sQueryPosition.size,
                         "%.*s",
                         aTableRef->aliasName.size,
                         aTableRef->aliasName.stmtText + aTableRef->aliasName.offset );
    }

    /* PROJ-2701 Sharding online data rebuild */
    if ( aFilter != NULL )
    {
        sQueryPosition.size +=
            idlOS::snprintf( sQueryBuf + sQueryPosition.size,
                             sTransformStringMaxSize - sQueryPosition.size,
                             " where  " );
        sQueryPosition.size +=
            idlOS::snprintf( sQueryBuf + sQueryPosition.size,
                             sTransformStringMaxSize - sQueryPosition.size,
                             "%.*s",
                             aFilter->size,
                             aFilter->stmtText + aFilter->offset );
        
    }
    else
    {
        // Nothing to do.
    }

    // Transformation string buffer overflow
    IDE_TEST_RAISE( sQueryPosition.size >= (sTransformStringMaxSize-1), ERR_TRANSFORM_STRING_BUFFER_OVERFLOW );
    
    SET_POSITION( sStatement->myPlan->parseTree->stmtPos, sQueryPosition );

    // 분석결과를 기록한다.
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
    IDE_EXCEPTION( ERR_TRANSFORM_STRING_BUFFER_OVERFLOW )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmvShardTransform::makeShardView",
                                  "Transformation string buffer overflow" ));
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmvShardTransform::isTransformAbleQuery( qcStatement     * aStatement,
                                                qcNamePosition  * aParsePosition,
                                                ULong             aTransformSMN,
                                                idBool          * aIsTransformAbleQuery )
{
/***********************************************************************
 *
 * Description : PROJ-2687 Shard aggregation transform
 *               From + Where가 Shard Query인지 검사한다.
 *
 * Implementation :
 *
 ***********************************************************************/

    volatile qcStatement sStatement;
    volatile SInt        sStage;

    qmsSFWGH        * sSFWGH;
    qmsFrom         * sFrom;
    qtcNode         * sWhere;
    qmsHierarchy    * sHierarchy;

    IDE_FT_BEGIN();

    IDU_FIT_POINT_FATAL( "qmvShardTransform::isTransformAbleQuery::__FT__" );

    sStage = 0; /* BUG-45994 - 컴파일러 최적화 회피 */

    /* PROJ-2701 Online data rebuild */
    if ( aTransformSMN < sdi::getSMNForDataNode() )
    {
        // sessionSMN < dataSMN
        *aIsTransformAbleQuery = ID_FALSE;
        IDE_CONT(NORMAL_RETURN);
    }
    else
    {
        // Nothing to do.
    }

    //-----------------------------------------
    // 준비
    //-----------------------------------------

    // resolve를 위해 aStatement의 session이 필요하다.
    IDE_TEST( qcg::allocStatement( (qcStatement *)&sStatement,
                                   aStatement->session,
                                   NULL,
                                   NULL )
              != IDE_SUCCESS );
    sStage = 1;

    sStatement.myPlan->stmtText    = aParsePosition->stmtText;
    sStatement.myPlan->stmtTextLen = idlOS::strlen( aParsePosition->stmtText );

    qcg::setSmiStmt( (qcStatement *)&sStatement, QC_SMI_STMT(aStatement) );

    //-----------------------------------------
    // PARSING
    //-----------------------------------------

    // FT_END 이전으로 이동할때는 IDE 매크로를 사용해도 된다.
    IDE_TEST_CONT( qcpManager::parsePartialForAnalyze(
                       (qcStatement *)&sStatement,
                       aParsePosition->stmtText,
                       aParsePosition->offset,
                       aParsePosition->size ) != IDE_SUCCESS,
                   NORMAL_EXIT );

    // 분석대상인지 먼저 확인한다.
    IDE_TEST_CONT( sdi::checkStmt( (qcStatement *)&sStatement ) != IDE_SUCCESS,
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

    QC_SHARED_TMPLATE((qcStatement *)&sStatement)->flag &= ~QC_TMP_SHARD_TRANSFORM_MASK;
    QC_SHARED_TMPLATE((qcStatement *)&sStatement)->flag |= QC_TMP_SHARD_TRANSFORM_DISABLE;

    IDE_TEST_CONT( sStatement.myPlan->parseTree->parse( (qcStatement *)&sStatement )
                   != IDE_SUCCESS, NORMAL_EXIT );

    IDE_TEST_CONT( qtc::fixAfterParsing( QC_SHARED_TMPLATE( (qcStatement *)&sStatement) )
                   != IDE_SUCCESS, NORMAL_EXIT );
    sStage = 2;

    IDE_TEST_CONT( sStatement.myPlan->parseTree->validate( (qcStatement *)&sStatement )
                   != IDE_SUCCESS, NORMAL_EXIT );

    IDE_TEST_CONT( qtc::fixAfterValidation( QC_QMP_MEM((qcStatement *)&sStatement),
                                            QC_SHARED_TMPLATE((qcStatement *)&sStatement) )
                   != IDE_SUCCESS, NORMAL_EXIT );

    QC_SHARED_TMPLATE((qcStatement *)&sStatement)->flag &= ~QC_TMP_SHARD_TRANSFORM_MASK;
    QC_SHARED_TMPLATE((qcStatement *)&sStatement)->flag |= QC_TMP_SHARD_TRANSFORM_ENABLE;

    //-----------------------------------------
    // ANALYZE
    //-----------------------------------------

    IDE_TEST_CONT( sdi::analyze( (qcStatement *)&sStatement,
                                 aTransformSMN ) != IDE_SUCCESS,
                   NORMAL_EXIT );

    IDE_TEST_CONT( sdi::setAnalysisResult( (qcStatement *)&sStatement ) != IDE_SUCCESS,
                   NORMAL_EXIT );

    //-----------------------------------------
    // ANALYSIS RESULT
    //-----------------------------------------

    IDE_EXCEPTION_CONT( NORMAL_EXIT );

    // setAnalysisResult가 실패하더라도 mShardAnalysis는 생성될 수 있다.
    if ( sStatement.myPlan->mShardAnalysis != NULL )
    {
        *aIsTransformAbleQuery = sStatement.myPlan->mShardAnalysis->mIsCanMerge;
    }
    else
    {
        *aIsTransformAbleQuery = ID_FALSE;
    }

    /* BUG-45899 transform 관련한 분석 횟수만 반영 */
    QC_SHARED_TMPLATE(aStatement)->stmt->mShardPrintInfo.mAnalyzeCount
        += sStatement.mShardPrintInfo.mAnalyzeCount;

    //-----------------------------------------
    // 마무리
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

    // session은 내것이 아니다.
    sStatement.session = NULL;

    (void) qcg::freeStatement( (qcStatement *)&sStatement );

    IDE_EXCEPTION_CONT( NORMAL_RETURN );

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
            (void) qcg::freeStatement( (qcStatement *)&sStatement );
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
 *     Aggregate function으로 인해 non-shard query로 판별된 query set에 대하여
 *     분산/통합부 로 query set을 나누는 transformation을 통하여 shard query로 변형한다.
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

    sQueryBufSize = aStatement->myPlan->parseTree->stmtPos.size + (SDU_SHARD_TRANSFORM_STRING_LENGTH_MAX+1);

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
         * sGroup->arithmeticOrList == NULL 인 경우(ROLLUP, CUBE, GROUPING SETS)는
         * 앞서 수행된 shard analysis에서 걸러진다.
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

        /* Where clause가 존재하면 where의 마지막 node의 end offset을 찾는다. */
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
            /* Where clause가 존재하지 않으면 from를 순회하며 from의 end offset을 찾는다. */
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

        IDE_TEST( sdi::copyAnalyzeInfo( aStatement,
                                        sAnalyzeInfo,
                                        aShardAnalysis )
                  != IDE_SUCCESS );

        sAnalyzeInfo->mIsCanMerge = ID_TRUE;
        sAnalyzeInfo->mIsTransformAble = ID_FALSE;

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
         * 지원하지 않는 aggregate function의 등장으로 aggr transform을 수행할 수 없다.
         * 하위 단계 transformation(from transformation)을 수행 시키기 위해서 설정
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
 *               Node tree를 순회하면서
 *               순수 column의 position을 string에 기록한다.
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
 *               Node tree를 순회하면서
 *               Aggregate function의 real position을 string에 기록한다.
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

        // BUG-45176
        if ( ( aNode->node.lflag & MTC_NODE_DISTINCT_MASK ) == MTC_NODE_DISTINCT_FALSE )
        {
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
        else
        {
            // MTC_NODE_DISTINCT_TRUE
            // Aggregate function에 distinct가 있는 경우 aggregation 분산수행의 결과를 보장하지 못한다.
            *aUnsupportedAggr = ID_TRUE;
        }
    }
    else if ( ( aNode->node.lflag & MTC_NODE_OPERATOR_MASK ) == MTC_NODE_OPERATOR_SUBQUERY )
    {
        // Sub-query를 가진 query set이 여기까지 올 수 없다.
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
 *               Aggregate function의 변형 없이 분산부를 이루는 4개 function에 대해서
 *               Query text를 생성한다.
 *
 *
 * Implementation :
 *
 *                SUM(arg),   MIN(arg),   MAX(arg),   COUNT(arg)
 *           ->  "SUM(arg)", "MIN(arg)", "MAX(arg)", "COUNT(arg)"
 *
 ***********************************************************************/

    IDU_FIT_POINT_FATAL( "qmvShardTransform::addSumMinMaxToText::__FT__" );

    // 정합성 검사
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
 *               Aggregate function의 변형이 필요한 AVG(arg)에 대해서
 *               SUM(arg),COUNT(arg) 로 변형 된 query text를 생성한다.
 *
 * Implementation :
 *
 *                AVG(arg)  ->  "SUM(arg),COUNT(arg)"
 *
 ***********************************************************************/
    qtcNode * sArg = NULL;

    IDU_FIT_POINT_FATAL( "qmvShardTransform::addAvgToText::__FT__" );

    // 정합성 검사
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
 *               From tree를 순회하며 query string의 마지막 위치에 해당하는 from의
 *               End position을 찾아 반환한다.
 *
 * Implementation :
 *
 ***********************************************************************/
    UInt sThisIsTheEnd = 0;

    IDU_FIT_POINT_FATAL( "qmvShardTransform::getFromEnd::__FT__" );

    if ( aFrom != NULL )
    {
        /* ON clause가 존재하면 on clause의 end position을 기록한다. */
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
                 * From tree를 순회하던 도중 기록된 from의 end position보다
                 * 더 큰(더 뒤에 등장하는) end position일 경우 값을 갱신
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
            /* ON clause가 존재하지 않으면 tableRef의 end position을 찾는다. */
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
 *               Original query block의
 *               SELECT, HAVING clause에 존재하는 aggregate function에 대해서
 *               통합부(coord-aggregation) aggregate function으로 변형한다.
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
 *               Aggregate function의 transformation을 수행
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
 *               * column_module은 makeNode를 통해 임의로 생성한다.
 *                 column_module로 생성된 node는
 *                 분산부의 해당 aggr에 대한 column order를
 *                 shardViewTargetPos에 기록한다.
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

    // 정합성 검사
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
 *     expression에 shard table이 포함된 subquery가 있는 경우 변환한다.
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
    // 적합성 검사
    //------------------------------------------

    IDE_FT_ASSERT( aStatement != NULL );

    //------------------------------------------
    // Shard View Transform 수행
    //------------------------------------------

    if ( ( ( QC_SHARED_TMPLATE(aStatement)->flag & QC_TMP_SHARD_TRANSFORM_MASK )
           == QC_TMP_SHARD_TRANSFORM_ENABLE ) &&
         ( ( ( aStatement->mFlag & QC_STMT_SHARD_OBJ_MASK ) == QC_STMT_SHARD_OBJ_EXIST ) ||
           ( aStatement->myPlan->parseTree->stmtShard != QC_STMT_SHARD_NONE ) ) &&
         ( aStatement->myPlan->parseTree->stmtShard != QC_STMT_SHARD_META ) &&
         ( aStatement->spvEnv->createPkg == NULL ) &&
         ( aStatement->spvEnv->createProc == NULL ) &&
         ( sdi::isShardCoordinator( aStatement ) == ID_TRUE ) )
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

        // shardInsert로 변환된 경우의 value에 subquery가 사용된 경우
        // DML의 set, where절 등에서 subquery가 사용된 경우
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
 *     from절의 shard table을 shard view로 변환한다.
 *
 * Implementation :
 *
 ***********************************************************************/

    qmsTableRef     * sTableRef;
    sdiAnalyzeInfo  * sShardAnalysis;
    sdiObjectInfo   * sShardObjectInfo = NULL;
    ULong             sSessionSMN = ID_ULONG(0);

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
                //------------------------------------------
                // Get shard object of the sessionSMN
                //------------------------------------------
                sSessionSMN = QCG_GET_SESSION_SHARD_META_NUMBER( aStatement );

                sdi::getShardObjInfoForSMN( sSessionSMN,
                                            sTableRef->mShardObjInfo,
                                            &sShardObjectInfo );

                // table
                if ( sShardObjectInfo != NULL )
                {
                    IDE_TEST( STRUCT_ALLOC( QC_QMP_MEM(aStatement),
                                            sdiAnalyzeInfo,
                                            &sShardAnalysis )
                              != IDE_SUCCESS );

                    // analyzer를 통하지 않고 직접 analyze 정보를 생성한다.
                    IDE_TEST( sdi::setAnalysisResultForTable( aStatement,
                                                              sShardAnalysis,
                                                              sShardObjectInfo )
                              != IDE_SUCCESS );

                    // table을 shard view로 변환한다.
                    IDE_TEST( makeShardView( aStatement,
                                             sTableRef,
                                             sShardAnalysis,
                                             NULL )
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
 *     expression의 subquery를 shard view로 변환한다.
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
 * Description : Shard Transform 에러를 출력한다.
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
                                QCG_GET_SESSION_SHARD_META_NUMBER( aStatement ),
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

IDE_RC qmvShardTransform::rebuildTransform( qcStatement    * aStatement,
                                            qcNamePosition * aStmtPos,
                                            ULong            aSessionSMN,
                                            ULong            aDataSMN )
{
    idBool             sSessionSMNIsShardQuery = ID_FALSE;
    idBool             sDataSMNIsShardQuery    = ID_FALSE;
    sdiAnalyzeInfo   * sSessionSMNAnalysis     = NULL;
    sdiAnalyzeInfo   * sDataSMNAnalysis        = NULL;
    sdiAnalyzeInfo   * sRebuildAnalysis        = NULL;
    UShort             sParamOffset            = ID_USHORT_MAX;
    UShort             sParamCount             = 0;
    idBool             sIsTansformable         = ID_TRUE;
    sdiNodeInfo      * sNodeInfo4SessionSMN    = NULL;
    sdiNodeInfo      * sNodeInfo4DataSMN       = NULL;
    qcShardNodes     * sExecNodes              = NULL;
    SChar            * sMyNodeName             = NULL;
    UShort             sMyNodeId               = ID_USHORT_MAX;
    sdiValue         * sValue                  = NULL;
    sdiRebuildInfo     sRebuildInfo;

    //------------------------------------------
    // get shard SQL type for SMN
    //------------------------------------------

    // analyze for sessionSMN
    IDE_TEST( isShardQuery( aStatement,
                            aStmtPos,
                            aSessionSMN,
                            & sSessionSMNIsShardQuery,
                            & sSessionSMNAnalysis,
                            & sParamOffset,
                            & sParamCount )
              != IDE_SUCCESS );

    // 임의적(node prefix, shard view keyword사용)으로 분산 정의를 무시하고 수행시킨
    // SQL에 대해서는 rebuild error를 발생시킨다.
    IDE_TEST_RAISE( sSessionSMNIsShardQuery != ID_TRUE, ERR_SHARD_REBUILD_ERROR );

    // analyze for dataSMN
    IDE_TEST( isShardQuery( aStatement,
                            aStmtPos,
                            aDataSMN,
                            & sDataSMNIsShardQuery,
                            & sDataSMNAnalysis,
                            & sParamOffset,
                            & sParamCount )
              != IDE_SUCCESS );

    //------------------------------------------
    // check rebuild transformable
    //------------------------------------------
    IDE_TEST( checkRebuildTransformable( aStatement,
                                         sSessionSMNAnalysis,
                                         sDataSMNIsShardQuery,
                                         sDataSMNAnalysis,
                                         &sIsTansformable )
              != IDE_SUCCESS );

    if ( sIsTansformable == ID_TRUE )
    {
        //------------------------------------------
        // get node info of sessionSMN & dataSMN
        //------------------------------------------
        IDE_TEST(STRUCT_ALLOC(QC_QMP_MEM(aStatement),
                              sdiNodeInfo,
                              &sNodeInfo4SessionSMN)
                 != IDE_SUCCESS);

        IDE_TEST( sdi::getInternalNodeInfo( NULL,
                                            sNodeInfo4SessionSMN,
                                            ID_FALSE,
                                            aSessionSMN )
                  != IDE_SUCCESS );

        IDE_TEST(STRUCT_ALLOC(QC_QMP_MEM(aStatement),
                              sdiNodeInfo,
                              &sNodeInfo4DataSMN)
                 != IDE_SUCCESS);

        IDE_TEST( sdi::getInternalNodeInfo( NULL,
                                            sNodeInfo4DataSMN,
                                            ID_FALSE,
                                            aDataSMN )
                  != IDE_SUCCESS );

        //------------------------------------------
        // get my node ID
        //------------------------------------------
        sMyNodeName = qcg::getSessionShardNodeName( aStatement );

        getNodeIdFromName( sNodeInfo4SessionSMN,
                           sMyNodeName,
                           &sMyNodeId );

        IDE_TEST_RAISE( sMyNodeId == ID_USHORT_MAX, ERR_INVALID_SHARD_NODE_ID );

        //------------------------------------------
        // make rebuild values
        //------------------------------------------
        if ( ( sSessionSMNAnalysis->mValueCount > 0 ) &&
             ( sSessionSMNAnalysis->mSplitMethod != SDI_SPLIT_CLONE ) )
        {
            IDE_DASSERT( sSessionSMNAnalysis->mSplitMethod != SDI_SPLIT_SOLO );

            IDE_TEST( QC_QMP_MEM(aStatement)->alloc( ID_SIZEOF(sdiValue) *
                                                     sSessionSMNAnalysis->mValueCount,
                                                     (void**) &sValue )
                      != IDE_SUCCESS );

            IDE_TEST( makeSdiValueWithBindParam( aStatement,
                                                 sSessionSMNAnalysis->mKeyDataType,
                                                 sSessionSMNAnalysis->mValueCount,
                                                 sSessionSMNAnalysis->mValue,
                                                 sValue )
                      != IDE_SUCCESS );
        }
        else
        {
            // Nothing to do.
        }

        //------------------------------------------
        // set rebuild information
        //------------------------------------------
        sRebuildInfo.mNodeInfo           = sNodeInfo4DataSMN;
        sRebuildInfo.mMyNodeId           = sMyNodeId;
        sRebuildInfo.mSessionSMN         = aSessionSMN;
        sRebuildInfo.mSessionSMNAnalysis = sSessionSMNAnalysis;
        sRebuildInfo.mDataSMN            = aDataSMN;
        sRebuildInfo.mDataSMNAnalysis    = sDataSMNAnalysis;
        sRebuildInfo.mValue              = sValue;

        //------------------------------------------
        // Rebuild coordinator plan이 생성 될 수 있도록
        // Transformation을 수행한다.
        //------------------------------------------
        if ( sDataSMNIsShardQuery == ID_TRUE )
        {
            /* shard query to shard query */

            // make rebuild range info
            IDE_TEST( makeExecNodes4ShardQuery( aStatement,
                                                &sRebuildInfo,
                                                &sExecNodes )
                      != IDE_SUCCESS );

            // do transform
            if ( sExecNodes != NULL )
            {
                // 특정 노드를 지정하여 수행하는 분석결과를 생성한다.
                IDE_TEST( STRUCT_ALLOC( QC_QMP_MEM(aStatement),
                                        sdiAnalyzeInfo,
                                        &sRebuildAnalysis )
                          != IDE_SUCCESS );

                SDI_INIT_ANALYZE_INFO( sRebuildAnalysis );

                sRebuildAnalysis->mSplitMethod = SDI_SPLIT_NODES;
                sRebuildAnalysis->mNodeNames = sExecNodes;

                if ( ( aStatement->myPlan->parseTree->stmtKind == QCI_STMT_SELECT ) ||
                     ( aStatement->myPlan->parseTree->stmtKind == QCI_STMT_SELECT_FOR_UPDATE ) )
                {
                    // Shard view transform
                    IDE_TEST( makeShardStatement( aStatement,
                                                  aStmtPos,
                                                  QC_STMT_SHARD_ANALYZE,
                                                  sRebuildAnalysis,
                                                  sParamOffset,
                                                  sParamCount )
                              != IDE_SUCCESS );
                }
                else if ( ( aStatement->myPlan->parseTree->stmtKind == QCI_STMT_INSERT ) ||
                          ( aStatement->myPlan->parseTree->stmtKind == QCI_STMT_UPDATE ) ||
                          ( aStatement->myPlan->parseTree->stmtKind == QCI_STMT_DELETE ) ||
                          ( aStatement->myPlan->parseTree->stmtKind == QCI_STMT_EXEC_PROC ) )
                {
                    // 함수를 변경한다.
                    aStatement->myPlan->parseTree->optimize = qmo::optimizeShardDML;
                    aStatement->myPlan->parseTree->execute  = qmx::executeShardDML;

                    // Shard statement로 변경한다.
                    aStatement->myPlan->parseTree->stmtShard = QC_STMT_SHARD_ANALYZE;

                    // 분석결과를 기록한다.
                    aStatement->myPlan->mShardAnalysis = sRebuildAnalysis;
                    aStatement->myPlan->mShardParamOffset = sParamOffset;
                    aStatement->myPlan->mShardParamCount = sParamCount;
                }
                else
                {
                    // 발생하지 않는다.
                    IDE_DASSERT(0);
                    IDE_RAISE( ERR_UNSUPPORTED_SHARD_STMT_TYPE );
                }
            }
            else
            {
                // Local로 수행한다.
                // Nothing to do.
            }
        }
        else /* sDataSMNIsShardQuery == ID_FALSE */
        {
            /* shard query to non-shard query */
            if ( ( aStatement->myPlan->parseTree->stmtKind == QCI_STMT_SELECT ) ||
                 ( aStatement->myPlan->parseTree->stmtKind == QCI_STMT_SELECT_FOR_UPDATE ) )
            {
                /* because of JOIN operation or SET operation or Sub-query operation */

                sRebuildInfo.mDataSMNAnalysis = NULL;

                IDE_TEST( rebuildNonShardTransform( aStatement,
                                                    &sRebuildInfo )
                          != IDE_SUCCESS );
            }
            else
            {
                // non-shard DML(distributed sub-query)은 현재 sharding에서 지원하지 않고있다.
                IDE_RAISE( ERR_SHARD_REBUILD_ERROR );
            }
        }
    }
    else
    {
        // Nothing to do.
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_SHARD_REBUILD_ERROR )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SHARD_REBUILD_ERROR ) );
    }
    IDE_EXCEPTION( ERR_UNSUPPORTED_SHARD_STMT_TYPE )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmvShardTransform::rebuildTransform",
                                  "Unsupported shard statement type" ));
    }
    IDE_EXCEPTION( ERR_INVALID_SHARD_NODE_ID )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmvShardTransform::rebuildTransform",
                                  "Invalid shard nodeID" ));
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmvShardTransform::checkRebuildTransformable( qcStatement    * aStatement,
                                                     sdiAnalyzeInfo * aSessionSMNAnalysis,
                                                     idBool           aDataSMNIsShardQuery,
                                                     sdiAnalyzeInfo * aDataSMNAnalysis,
                                                     idBool         * aIsTransformable )
{
    UShort i = 0;
    idBool sIsTransformable = ID_TRUE;

    IDE_DASSERT( aSessionSMNAnalysis != NULL );
    IDE_DASSERT( ( ( aDataSMNIsShardQuery == ID_TRUE ) && ( aDataSMNAnalysis != NULL ) ) ||
                 ( aDataSMNIsShardQuery == ID_FALSE ) );

    //------------------------------------------
    // Check online data rebuild enable
    //------------------------------------------

    IDE_TEST_RAISE( aSessionSMNAnalysis->mSubKeyExists != ID_FALSE,
                    ERR_SHARD_REBUILD_ERROR );

    if ( aDataSMNIsShardQuery == ID_TRUE )
    {
        IDE_TEST_RAISE( aSessionSMNAnalysis->mSplitMethod != aDataSMNAnalysis->mSplitMethod,
                        ERR_SHARD_REBUILD_ERROR );

        if ( ( aSessionSMNAnalysis->mSplitMethod == SDI_SPLIT_HASH ) ||
             ( aSessionSMNAnalysis->mSplitMethod == SDI_SPLIT_RANGE ) ||
             ( aSessionSMNAnalysis->mSplitMethod == SDI_SPLIT_LIST ) )
        {
            IDE_TEST_RAISE( aSessionSMNAnalysis->mSubKeyExists != aDataSMNAnalysis->mSubKeyExists,
                            ERR_SHARD_REBUILD_ERROR );
        }
        else
        {
            // Nothing to do.
        }
    }
    else
    {
        // Shard query to non-shard query에 대해서는
        // rebuildNonShardTransform()에서 각 object별로 확인한다.
        // Nothing to do.
    }

    if ( ( aSessionSMNAnalysis->mSplitMethod == SDI_SPLIT_HASH ) ||
         ( aSessionSMNAnalysis->mSplitMethod == SDI_SPLIT_RANGE ) ||
         ( aSessionSMNAnalysis->mSplitMethod == SDI_SPLIT_LIST ) )
    {
        //------------------------------------------
        // Check bind param info exists
        //------------------------------------------
        for ( i = 0; i < aSessionSMNAnalysis->mValueCount; i++ )
        {
            if ( aSessionSMNAnalysis->mValue[i].mType == 0 )
            {
                // bind variable
                if ( aStatement->pBindParam == NULL )
                {
                    // shard key value로 bind parameter를 사용 했는데
                    // bind 정보가 없다면 prepare중이므로
                    // transform을 중단하고 가짜 plan을 생성 하도록 한다.
                    // 이후 rebuild of execute 에서 진짜 plan을 생성해서 수행한다.
                    sIsTransformable = ID_FALSE;

                    aStatement->mFlag &= ~QC_STMT_SHARD_REBUILD_FORCE_MASK;
                    aStatement->mFlag |=  QC_STMT_SHARD_REBUILD_FORCE_TRUE;
                }
                else
                {
                    // Nothing to do.
                }

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

    *aIsTransformable = sIsTransformable;

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_SHARD_REBUILD_ERROR )
    {
        //*aIsTransformable = ID_FALSE;
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SHARD_REBUILD_ERROR ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

void qmvShardTransform::getNodeIdFromName( sdiNodeInfo * aNodeInfo,
                                           SChar       * aNodeName,
                                           UShort      * aNodeId )
{
    UShort i = 0;

    *aNodeId = ID_USHORT_MAX;

    for ( i = 0;
          i < aNodeInfo->mCount;
          i++ )
    {
        if ( idlOS::strncmp(aNodeName,
                            aNodeInfo->mNodes[i].mNodeName,
                            SDI_NODE_NAME_MAX_SIZE + 1) == 0 )
        {
            *aNodeId = aNodeInfo->mNodes[i].mNodeId;
            break;
        }
        else
        {
            // Nothing to do.
        }
    }
}

void qmvShardTransform::getNodeNameFromId( sdiNodeInfo * aNodeInfo,
                                           UShort        aNodeId,
                                           SChar      ** aNodeName )
{
    UShort i = 0;

    *aNodeName = NULL;

    for ( i = 0;
          i < aNodeInfo->mCount;
          i++ )
    {
        if ( aNodeId == aNodeInfo->mNodes[i].mNodeId )
        {
            *aNodeName = aNodeInfo->mNodes[i].mNodeName;
            break;
        }
        else
        {
            // Nothing to do.
        }
    }
}

IDE_RC qmvShardTransform::makeSdiValueWithBindParam( qcStatement   * aStatement,
                                                     UInt            aKeyDataType,
                                                     UShort          aValueCount,
                                                     sdiValueInfo  * aValues,
                                                     sdiValue      * aTargetValue )
{
    UShort i = 0;
    sdiValue sValue;
    qciBindParamInfo * sBindParam = NULL;
    
    for ( i = 0;
          i < aValueCount;
          i++ )
    {
        if ( aValues[i].mType == 1 )
        {
            // Constant value
            idlOS::memcpy( (void*) &aTargetValue[i],
                           (void*) &aValues[i].mValue,
                           ID_SIZEOF(sdiValue) );
        }
        else
        {
            // Bind variable
            sBindParam = &aStatement->pBindParam[aValues[i].mValue.mBindParamId];

            // Bind value의 data type별로 string value를 만들어 낸다.
            switch( sBindParam->param.type )
            {
                case MTD_CHAR_ID:
                case MTD_VARCHAR_ID:
                    sValue.mCharMax.length = (UShort)((mtdCharType*)sBindParam->param.data)->length;
                    idlOS::memcpy( (void*) sValue.mCharMax.value,
                                   (void*) ((mtdCharType*)sBindParam->param.data)->value,
                                   sValue.mCharMax.length );
                    break;
                case MTD_SMALLINT_ID:
                    sValue.mCharMax.length = idlOS::snprintf( (SChar*)sValue.mCharMax.value,
                                                              ID_SIZEOF(sValue),
                                                              "%"ID_INT32_FMT,
                                                              *(SShort*)sBindParam->param.data );
                    break;
                case MTD_INTEGER_ID:
                    sValue.mCharMax.length = idlOS::snprintf( (SChar*)sValue.mCharMax.value,
                                                              ID_SIZEOF(sValue),
                                                              "%"ID_INT32_FMT,
                                                              *(SInt*)sBindParam->param.data );
                    break;
                case MTD_BIGINT_ID:
                    sValue.mCharMax.length = idlOS::snprintf( (SChar*)sValue.mCharMax.value,
                                                              ID_SIZEOF(sValue),
                                                              "%"ID_INT64_FMT,
                                                              *(SLong*)sBindParam->param.data );
                    break;
                default:
                    // Shard key data type으로 지원하지 않는 type의 bind 사용시 에러
                    IDE_RAISE(ERR_SHARD_REBUILD_ERROR);
                    break;
            }

            // String으로 부터 shard key data type에 해당하는 sdiValue를 만들어 낸다.
            IDE_TEST( sdi::convertRangeValue( (SChar*)&sValue.mCharMax.value,
                                              sValue.mCharMax.length,
                                              aKeyDataType,
                                              &aTargetValue[i] )
                      != IDE_SUCCESS );
        }
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_SHARD_REBUILD_ERROR )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SHARD_REBUILD_ERROR ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmvShardTransform::makeExecNodes4ShardQuery( qcStatement     * aStatement,
                                                    sdiRebuildInfo  * aRebuildInfo,
                                                    qcShardNodes   ** aExecNodes )
{
    if ( ( aRebuildInfo->mSessionSMNAnalysis->mValueCount > 0 ) &&
         ( ( aRebuildInfo->mSessionSMNAnalysis->mSplitMethod == SDI_SPLIT_HASH )  ||
           ( aRebuildInfo->mSessionSMNAnalysis->mSplitMethod == SDI_SPLIT_RANGE ) ||
           ( aRebuildInfo->mSessionSMNAnalysis->mSplitMethod == SDI_SPLIT_LIST ) ) )
    {
        //------------------------------------------
        // Shard key value 를 기준으로 수행 노드 판단
        //------------------------------------------
        IDE_TEST( makeExecNodeWithValue( aStatement,
                                         aRebuildInfo,
                                         aExecNodes )
                  != IDE_SUCCESS );
    }
    else
    {
        if ( ( aRebuildInfo->mSessionSMNAnalysis->mSplitMethod != SDI_SPLIT_CLONE ) ||
             ( ( aStatement->myPlan->parseTree->stmtKind != QCI_STMT_SELECT ) &&
               ( aStatement->myPlan->parseTree->stmtKind != QCI_STMT_SELECT_FOR_UPDATE ) ) )
        {
            /*
             * INSERT(CLONE ONLY)/DELETE/UPDATE : SDI_SPLIT_HASH
             *                                    SDI_SPLIT_RANGE
             *                                    SDI_SPLIT_LIST
             *                                    SDI_SPLIT_SOLO
             *                                    SDI_SPLIT_CLONE
             *
             * SELECT : SDI_SPLIT_HASH
             *          SDI_SPLIT_RANGE
             *          SDI_SPLIT_LIST
             *          SDI_SPLIT_SOLO
             *
             * CLONE인 경우 client를 위한 dummy shard value가 존재한다
             *
             */

            //------------------------------------------
            // Shard key value가 없다.
            // Range info를 기준으로 수행 노드 판단.
            //------------------------------------------
            IDE_TEST( makeExecNodeWithoutValue( aStatement,
                                                aRebuildInfo,
                                                aExecNodes )
                      != IDE_SUCCESS );
        }
        else
        {
            //------------------------------------------
            // CLONE의 SELECT는 local에 있으면
            // Local에서 수행(transform수행하지 않음)하고,
            // Local에 없으면 복제 노드 중 마지막 거주 노드로 수행을 전달한다.
            //------------------------------------------
            IDE_TEST( makeExecNodeForCloneSelect( aStatement,
                                                  aRebuildInfo,
                                                  aExecNodes )
                      != IDE_SUCCESS );
        }
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmvShardTransform::makeExecNodeWithValue( qcStatement     * aStatement,
                                                 sdiRebuildInfo  * aRebuildInfo,
                                                 qcShardNodes   ** aExecNodes )
{
    UShort * sSessionSMNExecNodeId;
    UShort * sDataSMNExecNodeId;

    UShort   i = 0;

    sdiRebuildRangeList * sRebuildRanges      = NULL;
    sdiRebuildRangeList * sCurrRange          = NULL;
    idBool                sDummy              = ID_FALSE;
    idBool                sIsRebuildCoordNode = ID_FALSE;

    IDE_TEST( QC_QMP_MEM(aStatement)->alloc( ID_SIZEOF(UShort) *
                                             aRebuildInfo->mSessionSMNAnalysis->mValueCount,
                                             (void**) &sSessionSMNExecNodeId )
              != IDE_SUCCESS );

    IDE_TEST( QC_QMP_MEM(aStatement)->alloc( ID_SIZEOF(UShort) *
                                             aRebuildInfo->mDataSMNAnalysis->mValueCount,
                                             (void**) &sDataSMNExecNodeId )
              != IDE_SUCCESS );

    for ( i = 0;
          i < aRebuildInfo->mSessionSMNAnalysis->mValueCount;
          i++ )
    {
        // Get sessionSMN execution nodes
        IDE_TEST( getExecNodeId( aRebuildInfo->mSessionSMNAnalysis->mSplitMethod,
                                 aRebuildInfo->mSessionSMNAnalysis->mKeyDataType,
                                 aRebuildInfo->mSessionSMNAnalysis->mDefaultNodeId,
                                 &aRebuildInfo->mSessionSMNAnalysis->mRangeInfo,
                                 &aRebuildInfo->mValue[i],
                                 &sSessionSMNExecNodeId[i] )
                  != IDE_SUCCESS );

        // Get dataSMN execution nodes
        IDE_TEST( getExecNodeId( aRebuildInfo->mDataSMNAnalysis->mSplitMethod,
                                 aRebuildInfo->mDataSMNAnalysis->mKeyDataType,
                                 aRebuildInfo->mDataSMNAnalysis->mDefaultNodeId,
                                 &aRebuildInfo->mDataSMNAnalysis->mRangeInfo,
                                 &aRebuildInfo->mValue[i],
                                 &sDataSMNExecNodeId[i] )
                  != IDE_SUCCESS );
    }

    //------------------------------------------
    // SessionSMN 과 dataSMN 에 대한
    // 분산정의 변경정보를 얻어온다.
    //------------------------------------------
    IDE_TEST( makeRebuildRanges( aStatement,
                                 aRebuildInfo->mMyNodeId,
                                 aRebuildInfo->mSessionSMNAnalysis->mSplitMethod,
                                 aRebuildInfo->mSessionSMNAnalysis->mKeyDataType,
                                 &aRebuildInfo->mSessionSMNAnalysis->mRangeInfo,
                                 aRebuildInfo->mSessionSMNAnalysis->mDefaultNodeId,
                                 &aRebuildInfo->mDataSMNAnalysis->mRangeInfo,
                                 aRebuildInfo->mDataSMNAnalysis->mDefaultNodeId,
                                 &sRebuildRanges,
                                 &sDummy )
              != IDE_SUCCESS );

    //------------------------------------------
    // 데이터의 거주노드가 변경된 sessionSMN의 노드들 중
    // 가장 처음(nodeId가 가장 낮은) 노드가
    // Rebuild coordinator를 수행한다.
    //------------------------------------------
    for ( i = 0;
          i < aRebuildInfo->mSessionSMNAnalysis->mValueCount;
          i++ )
    {
        for ( sCurrRange = sRebuildRanges;
              sCurrRange != NULL;
              sCurrRange = sCurrRange->mNext )
        {
            if ( ( sCurrRange->mFromNode == sSessionSMNExecNodeId[i] ) &&
                 ( sCurrRange->mFromNode != sCurrRange->mToNode ) )
            {
                if ( sCurrRange->mFromNode == aRebuildInfo->mMyNodeId )
                {
                    sIsRebuildCoordNode = ID_TRUE;
                }
                else
                {
                    // sIsTransformNode = ID_FALSE;
                    // Nothing to do.
                }

                break;
            }
        }
    }

    if ( sIsRebuildCoordNode == ID_TRUE )
    {
        //------------------------------------------
        // 내가 rebuild coordinator노드로 결정 된 경우
        // 추가 된 노드에 대해서 대신 수행 해 준다.
        //------------------------------------------
        IDE_TEST( makeExecNode( aStatement,
                                aRebuildInfo->mNodeInfo,
                                aRebuildInfo->mMyNodeId,
                                aRebuildInfo->mSessionSMNAnalysis->mValueCount,
                                sSessionSMNExecNodeId,
                                aRebuildInfo->mDataSMNAnalysis->mValueCount,
                                sDataSMNExecNodeId,
                                aExecNodes )
                  != IDE_SUCCESS );
    }
    else
    {
        // Rebuild coordinator로 선택되지 않음.
        // Transform을 수행하지 않고 local plan으로 수행하면 된다.
        // Nothing to do.
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmvShardTransform::getExecNodeId( sdiSplitMethod   aSplitMethod,
                                         UInt             aKeyDataType,
                                         UShort           aDefaultNodeId,
                                         sdiRangeInfo   * aRangeInfo,
                                         sdiValue       * aShardKeyValue,
                                         UShort         * aExecNodeId )
{
    UShort            i = 0;
    const mtdModule * sKeyModule   = NULL;
    idBool            sIsFound     = ID_FALSE;
    SShort            sCompare     = ID_SSHORT_MAX; // -1 : less, 0 : equal, 1 : greater
    UInt              sHashValue   = ID_UINT_MAX;
    UInt              sKeyDataType = ID_UINT_MAX;
    sdiValue          sValue;

    IDE_DASSERT( ( aSplitMethod == SDI_SPLIT_HASH ) ||
                 ( aSplitMethod == SDI_SPLIT_RANGE ) ||
                 ( aSplitMethod == SDI_SPLIT_LIST ) );

    *aExecNodeId = ID_USHORT_MAX;

    // get compare data type
    if ( aSplitMethod == SDI_SPLIT_HASH )
    {
        // get key module for hash
        IDE_TEST( mtd::moduleById( &sKeyModule,
                                   aKeyDataType )
                  != IDE_SUCCESS );

        sKeyDataType = MTD_INTEGER_ID;

        sHashValue = sKeyModule->hash( mtc::hashInitialValue,
                                       NULL,
                                       (void*)aShardKeyValue );

        sHashValue = sHashValue % SDI_HASH_MAX_VALUE;
        sValue.mIntegerMax = sHashValue;
    }
    else
    {
        sKeyDataType = aKeyDataType;

        idlOS::memcpy( (void*)&sValue,
                       (void*)aShardKeyValue,
                       ID_SIZEOF(sdiValue) );
    }

    // get exec node
    for ( i = 0, sIsFound = ID_FALSE; i < aRangeInfo->mCount; i++ )
    {
        // compare shard key
        IDE_TEST( sdi::compareKeyData( sKeyDataType,
                                       &sValue, // A
                                       &aRangeInfo->mRanges[i].mValue, // B
                                       &sCompare ) // A = B : 0, A > B : 1, A < B : -1
                  != IDE_SUCCESS );

        if ( aSplitMethod == SDI_SPLIT_LIST )
        {
            if ( sCompare == 0 )
            {
                *aExecNodeId = aRangeInfo->mRanges[i].mNodeId;
                sIsFound = ID_TRUE;
                break;
            }
            else
            {
                // Nothing to do.
            }
        }
        else
        {
            if ( sCompare == -1 )
            {
                *aExecNodeId = aRangeInfo->mRanges[i].mNodeId;
                sIsFound = ID_TRUE;
                break;
            }
            else
            {
                // Nothing to do.
            }
        }
    }

    if ( sIsFound == ID_FALSE )
    {
        //set default node
        if ( aDefaultNodeId != ID_USHORT_MAX )
        {
            *aExecNodeId = aDefaultNodeId;
        }
        else
        {
            IDE_RAISE( ERR_INVALID_REBUILD );
        }
    }
    else
    {
        // Nothing to do.
    }
    
    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_INVALID_REBUILD )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDC_UNEXPECTED_ERROR,
                                  "qmvShardTransform::getExecNodeId",
                                  "No execution node exists." ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmvShardTransform::makeExecNode( qcStatement    * aStatement,
                                        sdiNodeInfo    * aNodeInfo,
                                        UShort           aMyNodeId,
                                        UShort           aSessionSMNExecNodeCount,
                                        UShort         * aSessionSMNExecNodeId,
                                        UShort           aDataSMNExecNodeCount,
                                        UShort         * aDataSMNExecNodeId,
                                        qcShardNodes  ** aExecNodes )
{
    UShort i        = 0;
    UShort j        = 0;

    idBool sIsFound = ID_FALSE;

    UShort * sExecNodes = NULL;
    UShort   sExecNodeCount = 0;

    qcShardNodes * sExecNodeList = NULL;

    SChar * sNodeName = NULL;

    IDE_TEST( QC_QMP_MEM(aStatement)->alloc( ID_SIZEOF(UShort) *
                                             aDataSMNExecNodeCount,
                                             (void**) &sExecNodes )
              != IDE_SUCCESS );

    //------------------------------------------
    // MyNode를 포함하여
    // SessionSMN 대비 dataSMN에서 추가 수행 되어야 할 노드를 찾는다.
    //------------------------------------------
    for ( i = 0; i < aDataSMNExecNodeCount; i++ )
    {
        for ( j = 0, sIsFound = ID_FALSE;
              j < aSessionSMNExecNodeCount;
              j++ )
        {
            if ( aDataSMNExecNodeId[i] == aSessionSMNExecNodeId[j] )
            {
                // myNode는 무조건 수행대상
                if ( aDataSMNExecNodeId[i] != aMyNodeId )
                {
                    sIsFound = ID_TRUE;
                }
                else
                {
                    // sIsFound = ID_FALSE;
                    // Nothing to do.
                }

                break;
            }
            else
            {
                // Nothing to do.
            }
        }

        if ( sIsFound == ID_FALSE )
        {
            // Rebuild coordinator가 추가로 수행 해야할 노드

            // distinction
            for ( j = 0; j < sExecNodeCount; j++ )
            {
                if ( sExecNodes[j] == aDataSMNExecNodeId[i] )
                {
                    sIsFound = ID_TRUE;
                    break;
                }
                else
                {
                    // Nothing to do.
                }
            }

            if ( sIsFound == ID_FALSE )
            {
                sExecNodes[sExecNodeCount] =  aDataSMNExecNodeId[i];
                sExecNodeCount++;
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

    if ( ( sExecNodeCount == 0 ) ||
         ( ( sExecNodeCount == 1 ) && ( sExecNodes[0] == aMyNodeId ) ) )
    {
        // 추가 수행 대상 노드가 없거나, 내 노드만이면 local만 수행하면 된다.
        // Nothing to do.
    }
    else
    {
        //------------------------------------------
        // Rebuild coordinator의 수행 노드에 대한 namePos를 생성한다.
        //------------------------------------------
        for ( i = 0; i < sExecNodeCount; i++ )
        {
            getNodeNameFromId( aNodeInfo,
                               sExecNodes[i],
                               &sNodeName );

            IDE_TEST_RAISE( sNodeName == NULL, ERR_NODE_NOT_FOUND );

            IDE_TEST( QC_QMP_MEM(aStatement)->alloc( ID_SIZEOF(qcShardNodes),
                                                     (void**) &sExecNodeList )
              != IDE_SUCCESS );

            sExecNodeList->namePos.stmtText = sNodeName;
            sExecNodeList->namePos.offset   = 0;
            sExecNodeList->namePos.size     = idlOS::strlen(sNodeName);

            sExecNodeList->next = *aExecNodes;
            *aExecNodes = sExecNodeList;
        }

        IDE_TEST( sdi::validateNodeNames( aStatement,
                                          *aExecNodes )
                  != IDE_SUCCESS );
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION(ERR_NODE_NOT_FOUND)
    {
        IDE_SET(ideSetErrorCode(sdERR_ABORT_SDC_UNEXPECTED_ERROR,
                                "sda::makeExecNode",
                                "The rebuild node is not found."));
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmvShardTransform::makeExecNodeWithoutValue( qcStatement     * aStatement,
                                                    sdiRebuildInfo  * aRebuildInfo,
                                                    qcShardNodes   ** aExecNodes )
{
    UShort   sSessionSMNExecNodeCount = 0;
    UShort * sSessionSMNExecNodes;
    UShort   sDataSMNExecNodeCount = 0;
    UShort * sDataSMNExecNodes;

    UInt    sMinNodeId = ID_UINT_MAX;
    UInt    i = 0;

    sdiRebuildRangeList * sRebuildRanges      = NULL;
    sdiRebuildRangeList * sCurrRange          = NULL;
    idBool                sDummy              = ID_FALSE;
    idBool                sIsRebuildCoordNode = ID_FALSE;

    // range count + default node 로 낭낭하게 할당한다.
    IDE_TEST( QC_QMP_MEM(aStatement)->alloc( ID_SIZEOF(UShort) *
                                             ( aRebuildInfo->mSessionSMNAnalysis->mRangeInfo.mCount + 1 ),
                                             (void**) &sSessionSMNExecNodes )
              != IDE_SUCCESS );

    IDE_TEST( QC_QMP_MEM(aStatement)->alloc( ID_SIZEOF(UShort) *
                                             ( aRebuildInfo->mDataSMNAnalysis->mRangeInfo.mCount + 1 ),
                                             (void**) &sDataSMNExecNodes )
              != IDE_SUCCESS );

    getExecNodeIdWithoutValue( aRebuildInfo->mSessionSMNAnalysis,
                               &sSessionSMNExecNodeCount,
                               sSessionSMNExecNodes );

    getExecNodeIdWithoutValue( aRebuildInfo->mDataSMNAnalysis,
                               &sDataSMNExecNodeCount,
                               sDataSMNExecNodes );

    if ( ( aRebuildInfo->mSessionSMNAnalysis->mSplitMethod == SDI_SPLIT_CLONE ) ||
         ( aRebuildInfo->mSessionSMNAnalysis->mSplitMethod == SDI_SPLIT_SOLO ) )
    {
        /* Clone또는 solo split인 경우 nodeId가 가장 작은 노드에서 추가된 노드에 DML을 대신 수행 시켜준다. */
        for ( i = 0; i < aRebuildInfo->mSessionSMNAnalysis->mRangeInfo.mCount; i++ )
        {
            if ( sMinNodeId > aRebuildInfo->mSessionSMNAnalysis->mRangeInfo.mRanges[i].mNodeId )
            {
                sMinNodeId = aRebuildInfo->mSessionSMNAnalysis->mRangeInfo.mRanges[i].mNodeId;
            }
            else
            {
                // Nothing to do.
            }
        }

        if ( sMinNodeId == aRebuildInfo->mMyNodeId )
        {
            sIsRebuildCoordNode = ID_TRUE;
        }
        else
        {
            // sIsTransformNode = ID_FALSE;
            // Nothing to do.
        }
    }
    else /* Clone또는 solo split이 아닌 경우 */
    {
        //------------------------------------------
        // SessionSMN 과 dataSMN 에 대한
        // 분산정의 변경정보를 얻어온다.
        //------------------------------------------
        IDE_TEST( makeRebuildRanges( aStatement,
                                     aRebuildInfo->mMyNodeId,
                                     aRebuildInfo->mSessionSMNAnalysis->mSplitMethod,
                                     aRebuildInfo->mSessionSMNAnalysis->mKeyDataType,
                                     &aRebuildInfo->mSessionSMNAnalysis->mRangeInfo,
                                     aRebuildInfo->mSessionSMNAnalysis->mDefaultNodeId,
                                     &aRebuildInfo->mDataSMNAnalysis->mRangeInfo,
                                     aRebuildInfo->mDataSMNAnalysis->mDefaultNodeId,
                                     &sRebuildRanges,
                                     &sDummy )
                  != IDE_SUCCESS );

        //------------------------------------------
        // 데이터의 거주노드가 변경된 sessionSMN의 노드들 중
        // 가장 처음(nodeId가 가장 낮은) 노드가
        // Rebuild coordinator를 수행한다.
        //------------------------------------------
        for ( i = 0;
              i < sSessionSMNExecNodeCount;
              i++ )
        {
            for ( sCurrRange = sRebuildRanges;
                  sCurrRange != NULL;
                  sCurrRange = sCurrRange->mNext )
            {
                if ( ( sCurrRange->mFromNode == sSessionSMNExecNodes[i] ) &&
                     ( sCurrRange->mFromNode != sCurrRange->mToNode ) )
                {
                    if ( sCurrRange->mFromNode == aRebuildInfo->mMyNodeId )
                    {
                        sIsRebuildCoordNode = ID_TRUE;
                    }
                    else
                    {
                        // sIsTransformNode = ID_FALSE;
                        // Nothing to do.
                    }

                    break;
                }
            }
        }
    }

    //------------------------------------------
    // 내가 rebuild coordinator노드로 결정 된 경우
    // 추가 된 노드에 대해서 대신 수행 해 준다.
    //------------------------------------------
    if ( sIsRebuildCoordNode == ID_TRUE )
    {
        IDE_TEST( makeExecNode( aStatement,
                                aRebuildInfo->mNodeInfo,
                                aRebuildInfo->mMyNodeId,
                                sSessionSMNExecNodeCount,
                                sSessionSMNExecNodes,
                                sDataSMNExecNodeCount,
                                sDataSMNExecNodes,
                                aExecNodes )
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

IDE_RC qmvShardTransform::makeExecNodeForCloneSelect( qcStatement     * aStatement,
                                                      sdiRebuildInfo  * aRebuildInfo,
                                                      qcShardNodes   ** aExecNodes )
{
    UShort         i             = 0;
    sdiRange     * sRange        = NULL;
    idBool         sIsLocalClone = ID_FALSE;
    qcShardNodes * sExecNode     = NULL;
    SChar        * sNodeName     = NULL;

    // Init for local clone
    *aExecNodes = NULL;

    IDE_TEST_RAISE( aRebuildInfo->mDataSMNAnalysis->mRangeInfo.mCount == 0, ERR_NODE_NOT_FOUND );

    for ( i = 0;
          i < aRebuildInfo->mDataSMNAnalysis->mRangeInfo.mCount;
          i++ )
    {
        sRange = &aRebuildInfo->mDataSMNAnalysis->mRangeInfo.mRanges[i];
           
        if ( sRange->mNodeId == aRebuildInfo->mMyNodeId )
        {
            sIsLocalClone = ID_TRUE;
            break;
        }
        else
        {
            // Nothing to do.
        }
    }

    if ( sIsLocalClone == ID_FALSE )
    {
        // sessionSMN시점에는 내 노드에 해당 clone (들)이 있었는데,
        // dataSMN시점에는 내 노드에서 해당 clone (들)을 빼버렸다.
        // 해당 clone(들)이 존재하는 마지막 노드에서 가져온다.
        IDE_TEST( QC_QMP_MEM(aStatement)->alloc( ID_SIZEOF(qcShardNodes),
                                                 (void**) &sExecNode )
                  != IDE_SUCCESS );

        // get execNodeName
        getNodeNameFromId( aRebuildInfo->mNodeInfo,
                           sRange->mNodeId,
                           &sNodeName );

        IDE_TEST_RAISE( sNodeName == NULL, ERR_NODE_NOT_FOUND );

        sExecNode->namePos.stmtText = sNodeName;
        sExecNode->namePos.offset   = 0;
        sExecNode->namePos.size     = idlOS::strlen(sNodeName);
        sExecNode->next             = NULL;

        *aExecNodes = sExecNode;
    }
    else
    {
        // dataSMN기준으로도 내 노드에 해당 clone(들)이 있으므로,
        // Transform 수행 하지않고 local 수행한다.
        // *aExecNodes = NULL;

        // Nothing to do.
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION(ERR_NODE_NOT_FOUND)
    {
        IDE_SET(ideSetErrorCode(sdERR_ABORT_SDC_UNEXPECTED_ERROR,
                                "sda::makeExecNodeForClone",
                                "The rebuild node is not found."));
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

void qmvShardTransform::getExecNodeIdWithoutValue( sdiAnalyzeInfo * aAnalysis,
                                                   UShort         * aExecNodeCount,
                                                   UShort         * aExecNodeId )
{
    UShort  i        = 0;
    UShort  j        = 0;
    idBool  sIsFound = ID_FALSE;

    // make exec node for ranges
    for ( i = 0; i < aAnalysis->mRangeInfo.mCount; i++ )
    {
        for ( j = 0, sIsFound = ID_FALSE;
              j < *aExecNodeCount;
              j++ )
        {
            // distinct add
            if ( aAnalysis->mRangeInfo.mRanges[i].mNodeId == aExecNodeId[j] )
            {
                sIsFound = ID_TRUE;
                break;
            }
            else
            {
                // Nothing to do.
            }
        }

        if ( sIsFound == ID_FALSE )
        {
            aExecNodeId[*aExecNodeCount] = aAnalysis->mRangeInfo.mRanges[i].mNodeId;
            (*aExecNodeCount)++;
        }
        else
        {
            // Nothing to do.
        }
    }

    // make exec node for default node
    if ( aAnalysis->mDefaultNodeId != ID_USHORT_MAX )
    {
        for ( j = 0, sIsFound = ID_FALSE;
              j < *aExecNodeCount;
              j++ )
        {
            // distinct add
            if ( aAnalysis->mDefaultNodeId == aExecNodeId[j] )
            {
                sIsFound = ID_TRUE;
                break;
            }
            else
            {
                // Nothing to do.
            }
        }

        if ( sIsFound == ID_FALSE )
        {
            aExecNodeId[*aExecNodeCount] = aAnalysis->mDefaultNodeId;
            (*aExecNodeCount)++;
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

IDE_RC qmvShardTransform::rebuildNonShardTransform( qcStatement    * aStatement,
                                                    sdiRebuildInfo * aRebuildInfo )
{
    qmsParseTree     * sParseTree;
    qmsSortColumns   * sCurrSort;

    sParseTree = (qmsParseTree*)aStatement->myPlan->parseTree;

    // transform querySet
    IDE_TEST( rebuildTransformQuerySet( aStatement,
                                        aRebuildInfo,
                                        sParseTree->querySet )
              != IDE_SUCCESS );

    // transform sub-query of orderBy
    for ( sCurrSort = sParseTree->orderBy;
          sCurrSort != NULL;
          sCurrSort = sCurrSort->next )
    {
        if ( sCurrSort->targetPosition <= QMV_EMPTY_TARGET_POSITION )
        {
            IDE_TEST( rebuildTransformExpr( aStatement,
                                            aRebuildInfo,
                                            sCurrSort->sortColumn )
                      != IDE_SUCCESS );
        }
        else
        {
            // Nothing to do.
        }
    }

    // transform sub-query of loop
    if ( sParseTree->loopNode != NULL )
    {
        IDE_TEST( rebuildTransformExpr( aStatement,
                                        aRebuildInfo,
                                        sParseTree->loopNode )
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

IDE_RC qmvShardTransform::rebuildTransformQuerySet( qcStatement    * aStatement,
                                                    sdiRebuildInfo * aRebuildInfo,
                                                    qmsQuerySet    * aQuerySet )
{
    qmsFrom          * sFrom = NULL;
    qmsTarget        * sTarget = NULL;
    qmsConcatElement * sConcatElement = NULL;

    //------------------------------------------
    // Rebuild Transform의 수행
    //------------------------------------------
    if ( aQuerySet->setOp == QMS_NONE )
    {
        IDE_DASSERT( aQuerySet->SFWGH != NULL );

        // select target
        for ( sTarget  = aQuerySet->SFWGH->target;
              sTarget != NULL;
              sTarget  = sTarget->next )
        {
            if ( ( sTarget->flag & QMS_TARGET_ASTERISK_MASK )
                 != QMS_TARGET_ASTERISK_TRUE )
            {
                IDE_TEST( rebuildTransformExpr( aStatement,
                                                aRebuildInfo,
                                                sTarget->targetColumn )
                          != IDE_SUCCESS );
            }
            else
            {
                // Nothing to do.
            }
            
        }

        // from
        for ( sFrom  = aQuerySet->SFWGH->from;
              sFrom != NULL;
              sFrom  = sFrom->next )
        {
            IDE_TEST( rebuildTransformFrom( aStatement,
                                            aRebuildInfo,
                                            sFrom )
                      != IDE_SUCCESS );
        }

        // where
        if ( aQuerySet->SFWGH->where != NULL )
        {
            IDE_TEST( rebuildTransformExpr( aStatement,
                                            aRebuildInfo,
                                            aQuerySet->SFWGH->where )
                      != IDE_SUCCESS );
        }
        else
        {
            // Nothing to do.
        }

        // hierarchy
        IDE_TEST_RAISE( aQuerySet->SFWGH->hierarchy != NULL, ERR_INVALID_SHARD_QUERY_TYPE );

        // group by
        for ( sConcatElement  = aQuerySet->SFWGH->group;
              sConcatElement != NULL;
              sConcatElement  = sConcatElement->next )
        {
            if ( sConcatElement->arithmeticOrList != NULL )
            {
                IDE_TEST( rebuildTransformExpr( aStatement,
                                                aRebuildInfo,
                                                sConcatElement->arithmeticOrList )
                          != IDE_SUCCESS );
            }
            else
            {
                // Nothing to do.
            }
        }

        // having
        if ( aQuerySet->SFWGH->having != NULL )
        {
            IDE_TEST( rebuildTransformExpr( aStatement,
                                            aRebuildInfo,
                                            aQuerySet->SFWGH->having )
                      != IDE_SUCCESS );
        }
        else
        {
            // Noting to do.
        }
    }
    else
    {
        // left subQuery
        IDE_TEST( rebuildTransformQuerySet( aStatement,
                                            aRebuildInfo,
                                            aQuerySet->left )
                  != IDE_SUCCESS );

        // right subQuery
        IDE_TEST( rebuildTransformQuerySet( aStatement,
                                            aRebuildInfo,
                                            aQuerySet->right )
                  != IDE_SUCCESS );
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION(ERR_INVALID_SHARD_QUERY_TYPE)
    {
        IDE_SET(ideSetErrorCode(sdERR_ABORT_SDC_UNEXPECTED_ERROR,
                                "sda::rebuildTransformQuerySet",
                                "Invalid shard query type"));
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmvShardTransform::rebuildTransformFrom( qcStatement    * aStatement,
                                                sdiRebuildInfo * aRebuildInfo,
                                                qmsFrom        * aFrom )
{
    qmsTableRef         * sTableRef      = NULL;
    sdiRebuildRangeList * sRebuildRanges = NULL;

    sdiObjectInfo       * sSessionSMNObj = NULL;
    sdiObjectInfo       * sDataSMNObj    = NULL;

    idBool                sIsTransformNeeded = ID_FALSE;

    qcNamePosition        sRebuildFilter;

    sdiAnalyzeInfo      * sRebuildAnalysis = NULL;

    UShort i;
    idBool sIsFound = ID_FALSE;

    if ( aFrom->joinType == QMS_NO_JOIN )
    {
        sTableRef = aFrom->tableRef;

        if ( sTableRef->view != NULL )
        {
            // view
            IDE_TEST( rebuildNonShardTransform( sTableRef->view,
                                                aRebuildInfo )
                      != IDE_SUCCESS );
        }
        else
        {
            // table

            //------------------------------------------
            // Shard query to non-shard query 에 대한
            // Rebuild transformation을 object별로 수행한다.
            //------------------------------------------

            //------------------------------------------
            // Get the shard object of the each SMN
            //------------------------------------------
            sdi::getShardObjInfoForSMN( aRebuildInfo->mSessionSMN,
                                        sTableRef->mShardObjInfo,
                                        &sSessionSMNObj );

            IDE_TEST_RAISE( sSessionSMNObj == NULL, ERR_INVALID_SHARD_OBJECT_INFO );

            sdi::getShardObjInfoForSMN( aRebuildInfo->mDataSMN,
                                        sTableRef->mShardObjInfo,
                                        &sDataSMNObj );
            
            IDE_TEST_RAISE( sDataSMNObj == NULL, ERR_INVALID_SHARD_OBJECT_INFO );

            //------------------------------------------
            // Check non-shard rebuild transformable
            //------------------------------------------
            if ( checkRebuildTransformable4NonShard( sSessionSMNObj,
                                                     sDataSMNObj ) == ID_TRUE )
            {
                if ( ( sDataSMNObj->mTableInfo.mSplitMethod == SDI_SPLIT_HASH ) ||
                     ( sDataSMNObj->mTableInfo.mSplitMethod == SDI_SPLIT_RANGE ) ||
                     ( sDataSMNObj->mTableInfo.mSplitMethod == SDI_SPLIT_LIST ) )
                {
                    SET_EMPTY_POSITION( sRebuildFilter );

                    //------------------------------------------
                    // Make rebuild ranges
                    //------------------------------------------
                    IDE_TEST( makeRebuildRanges( aStatement,
                                                 aRebuildInfo->mMyNodeId,
                                                 sSessionSMNObj->mTableInfo.mSplitMethod,
                                                 sSessionSMNObj->mTableInfo.mKeyDataType,
                                                 &sSessionSMNObj->mRangeInfo,
                                                 sSessionSMNObj->mTableInfo.mDefaultNodeId,
                                                 &sDataSMNObj->mRangeInfo,
                                                 sDataSMNObj->mTableInfo.mDefaultNodeId,
                                                 &sRebuildRanges,
                                                 &sIsTransformNeeded )
                              != IDE_SUCCESS );

                    if ( sIsTransformNeeded == ID_TRUE )
                    {
                        //------------------------------------------
                        // Make rebuild filter string
                        //------------------------------------------
                        IDE_TEST( makeRebuildFilterString( aStatement,
                                                           sDataSMNObj,
                                                           aRebuildInfo,
                                                           sRebuildRanges,
                                                           &sRebuildFilter )
                                  != IDE_SUCCESS );

                        //------------------------------------------
                        // Process rebuild view transform
                        //------------------------------------------
                        IDE_TEST( STRUCT_ALLOC( QC_QMP_MEM(aStatement),
                                                sdiAnalyzeInfo,
                                                &sRebuildAnalysis )
                                  != IDE_SUCCESS );

                        // DataSMN기준 해당 object가 분산되어있는 모든 노드에 수행하는 분석결과를 생성한다.
                        // TODO : SDI_REBUILD_RANGE_INCLUDE 노드에 대해서만 수행해도 됨.
                        IDE_TEST( sdi::setAnalysisResultForTable( aStatement,
                                                                  sRebuildAnalysis,
                                                                  sDataSMNObj )
                                  != IDE_SUCCESS );

                        IDE_TEST( makeShardView( aStatement,
                                                 sTableRef,
                                                 sRebuildAnalysis,
                                                 &sRebuildFilter )
                                  != IDE_SUCCESS );
                    }
                    else
                    {
                        // Nothing to do.
                    }
                }
                else
                {
                    // CLONE or SOLO split
                    // Local에 있으면 내걸 그냥 쓰고, 없으면 가져온다.
                    for ( i = 0, sIsFound = ID_FALSE;
                          i < sDataSMNObj->mRangeInfo.mCount;
                          i++ )
                    {
                        if ( sDataSMNObj->mRangeInfo.mRanges[i].mNodeId == aRebuildInfo->mMyNodeId )
                        {
                            sIsFound = ID_TRUE;
                            break;
                        }
                        else
                        {
                            // Nothing to do.
                        }
                    }

                    if ( sIsFound == ID_FALSE )
                    {
                        //------------------------------------------
                        // Process rebuild view transform
                        //------------------------------------------
                        IDE_TEST( STRUCT_ALLOC( QC_QMP_MEM(aStatement),
                                                sdiAnalyzeInfo,
                                                &sRebuildAnalysis )
                                  != IDE_SUCCESS );
                        IDE_TEST( sdi::setAnalysisResultForTable( aStatement,
                                                                  sRebuildAnalysis,
                                                                  sDataSMNObj )
                                  != IDE_SUCCESS );

                        IDE_TEST( makeShardView( aStatement,
                                                 sTableRef,
                                                 sRebuildAnalysis,
                                                 NULL )
                                  != IDE_SUCCESS );
                    }
                    else
                    {
                        // Local execution
                        // Nothing to do.
                    }
                }

                //------------------------------------------
                // Do transform
                //------------------------------------------
                if ( sIsTransformNeeded == ID_TRUE )
                {
                    // Set Exec Nodes
                }
                else
                {
                    // Local only
                    // Nothing to do.
                }
            }
            else
            {
                IDE_RAISE(ERR_SHARD_REBUILD_ERROR);
            }
        }
    }
    else
    {
        IDE_TEST( rebuildTransformFrom( aStatement,
                                        aRebuildInfo,
                                        aFrom->left )
                  != IDE_SUCCESS );

        IDE_TEST( rebuildTransformFrom( aStatement,
                                        aRebuildInfo,
                                        aFrom->right )
                  != IDE_SUCCESS );

        IDE_TEST( rebuildTransformExpr( aStatement,
                                        aRebuildInfo,
                                        aFrom->onCondition )
                  != IDE_SUCCESS );
    }
    
    return IDE_SUCCESS;

    IDE_EXCEPTION(ERR_INVALID_SHARD_OBJECT_INFO)
    {
        IDE_SET(ideSetErrorCode(sdERR_ABORT_SDC_UNEXPECTED_ERROR,
                                "sda::rebuildTransformFrom",
                                "Invalid shard object"));
    }
    IDE_EXCEPTION( ERR_SHARD_REBUILD_ERROR )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SHARD_REBUILD_ERROR ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

idBool qmvShardTransform::checkRebuildTransformable4NonShard( sdiObjectInfo * aSessionSMNObj,
                                                              sdiObjectInfo * aDataSMNObj )
{
    
    idBool sIsTransformable = ID_FALSE;

    if ( ( aSessionSMNObj->mTableInfo.mSplitMethod == aDataSMNObj->mTableInfo.mSplitMethod ) &&
         ( aSessionSMNObj->mTableInfo.mSubKeyExists == aDataSMNObj->mTableInfo.mSubKeyExists ) &&
         ( aSessionSMNObj->mTableInfo.mSubKeyExists == ID_FALSE ) )
    {
        // BUGBUG : 해당 table의 shard key column의 이름 등 이
        //          변경 되었을 경우도 체크해서 rebuild error를 올려주면 좋다.
        sIsTransformable = ID_TRUE;
    }
    else
    {
        // Nothing to do.
    }

    return sIsTransformable;
}

IDE_RC qmvShardTransform::makeRebuildRanges( qcStatement          * aStatement,
                                             UShort                 aMyNodeId,
                                             sdiSplitMethod         aSplitMethod,
                                             UInt                   aKeyDataType,
                                             sdiRangeInfo         * aSessionSMNRangeInfo,
                                             UShort                 aSessionSMNDefaultNodeId,
                                             sdiRangeInfo         * aDataSMNRangeInfo,
                                             UShort                 aDataSMNDefaultNodeId,
                                             sdiRebuildRangeList ** aRebuildRanges,
                                             idBool               * aIsTransformNeeded )
{
    UInt sKeyDataType = ID_USHORT_MAX;

    UShort i = 0; // sessionSMN's range index
    UShort j = 0; // dataSMN's range index

    sdiValue * sSessionSMNRangeValue = NULL;
    sdiValue * sDataSMNRangeValue    = NULL;

    UShort sFromNode = ID_USHORT_MAX;
    UShort sToNode   = ID_USHORT_MAX;

    SShort sCompare = 0; // -1 : less, 0 : equal, 1 : greater

    sdiRebuildRangeList * sRebuildRanges = NULL;
    sdiRebuildRangeList * sCurrRange = NULL;

    *aRebuildRanges = NULL;
    *aIsTransformNeeded = ID_FALSE;

    // get shard key data type for comparison
    if ( aSplitMethod == SDI_SPLIT_HASH )
    {
        sKeyDataType = MTD_INTEGER_ID;
    }
    else
    {
        sKeyDataType = aKeyDataType;
    }

    //------------------------------------------
    // SessionSMNRanges와 dataSMNRanges가
    // union된 rangeInfo를 구성한다.
    //------------------------------------------
    while ( sCompare != ID_SSHORT_MAX )
    {
        if ( i < aSessionSMNRangeInfo->mCount )
        {
            sSessionSMNRangeValue = &aSessionSMNRangeInfo->mRanges[i].mValue;
            sFromNode = aSessionSMNRangeInfo->mRanges[i].mNodeId;
        }
        else
        {
            sSessionSMNRangeValue = NULL;
            sFromNode = aSessionSMNDefaultNodeId;
        }

        if ( j < aDataSMNRangeInfo->mCount )
        {
            sDataSMNRangeValue = &aDataSMNRangeInfo->mRanges[j].mValue;
            sToNode = aDataSMNRangeInfo->mRanges[j].mNodeId;
        }
        else
        {
            sDataSMNRangeValue = NULL;
            sToNode = aDataSMNDefaultNodeId;
        }

        if ( ( sSessionSMNRangeValue != NULL ) && ( sDataSMNRangeValue != NULL ) )
        {
            IDE_TEST( sdi::compareKeyData( sKeyDataType,
                                           sSessionSMNRangeValue, // A
                                           sDataSMNRangeValue,    // B
                                           &sCompare )       // A = B : 0, A > B : 1, A < B : -1
                      != IDE_SUCCESS );

            if ( aSplitMethod == SDI_SPLIT_LIST )
            {
                if ( sCompare == -1 )
                {
                    sToNode = aDataSMNDefaultNodeId;
                }
                else if ( sCompare == 1 )
                {
                    sFromNode = aSessionSMNDefaultNodeId;
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
        else if ( ( sSessionSMNRangeValue == NULL ) && ( sDataSMNRangeValue != NULL ) )
        {
            sCompare = 1;
        }
        else if ( ( sSessionSMNRangeValue != NULL ) && ( sDataSMNRangeValue == NULL ) )
        {
            sCompare = -1;
        }
        else // ( ( sSeesionSMNRangeValue == NULL ) && ( sDataSMNRangeValue == NULL ) )
        {
            sCompare = ID_SSHORT_MAX;
        }

        // alloc
        IDE_TEST( STRUCT_ALLOC( QC_QMP_MEM(aStatement),
                                sdiRebuildRangeList,
                                &sRebuildRanges )
                  != IDE_SUCCESS );

        // set value
        if ( sCompare == -1 ) // sessionSMNValue < dataSMNValue
        {
            idlOS::memcpy( (void*) &sRebuildRanges->mValue,
                           (void*) sSessionSMNRangeValue,
                           ID_SIZEOF(sdiValue) );
            i++;

            sRebuildRanges->mIsDefault = ID_FALSE;
        }
        else if ( sCompare == 1 ) // sessionSMNVale > dataSMNValue
        {
            idlOS::memcpy( (void*) &sRebuildRanges->mValue,
                           (void*) sDataSMNRangeValue,
                           ID_SIZEOF(sdiValue) );
            j++;

            sRebuildRanges->mIsDefault = ID_FALSE;
        }
        else if ( sCompare == 0 ) // sessionSMNValue == dataSMNValue
        {
            idlOS::memcpy( (void*) &sRebuildRanges->mValue,
                           (void*) sSessionSMNRangeValue,
                           ID_SIZEOF(sdiValue) );
            i++;
            j++;

            sRebuildRanges->mIsDefault = ID_FALSE;
        }
        else //( sCompare == ID_SSHORT_MAX )
        {
            IDE_DASSERT( sCompare == ID_SSHORT_MAX );
            sRebuildRanges->mIsDefault = ID_TRUE;
        }

        // set exec nodes
        sRebuildRanges->mFromNode = sFromNode;
        sRebuildRanges->mToNode = sToNode;
        sRebuildRanges->mNext = NULL;

        // set filter type
        if ( sFromNode == aMyNodeId )
        {
            // sessionSMN 기준으로 내가 가지고있는 데이터이다.
            sRebuildRanges->mType = SDI_REBUILD_RANGE_INCLUDE;

            if ( sToNode != aMyNodeId )
            {
                *aIsTransformNeeded = ID_TRUE;
            }
            else
            {
                // Nothing to do.
            }
        }
        else // ( sFromNode != aMyNodeId )
        {
            if ( sToNode == aMyNodeId )
            {
                // 내가 가지고 있지 않던 데이터를 받았다.
                sRebuildRanges->mType = SDI_REBUILD_RANGE_EXCLUDE;
                *aIsTransformNeeded = ID_TRUE;
            }
            else // ( sToNode != aMyNodeId )
            {
                // 나와 관련없는 데이터이다.
                sRebuildRanges->mType = SDI_REBUILD_RANGE_NONE;
            }
        }

        IDE_TEST_RAISE( ( ( sRebuildRanges->mFromNode != ID_USHORT_MAX ) &&
                          ( sRebuildRanges->mToNode == ID_USHORT_MAX ) ), ERR_SHARD_REBUILD_ERROR );

        IDE_TEST_RAISE( ( ( sRebuildRanges->mFromNode == ID_USHORT_MAX ) &&
                          ( sRebuildRanges->mToNode != ID_USHORT_MAX ) ), ERR_SHARD_REBUILD_ERROR );

        // Add to list tail
        if ( *aRebuildRanges == NULL )
        {
            *aRebuildRanges = sRebuildRanges;
            sCurrRange = sRebuildRanges;
        }
        else
        {
            sCurrRange->mNext = sRebuildRanges;
            sCurrRange = sRebuildRanges;
        }
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_SHARD_REBUILD_ERROR )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SHARD_REBUILD_ERROR ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmvShardTransform::makeRebuildFilterString( qcStatement          * aStatement,
                                                   sdiObjectInfo        * aShardObject,
                                                   sdiRebuildInfo       * aRebuildInfo,
                                                   sdiRebuildRangeList  * aRebuildRanges,
                                                   qcNamePosition       * aRebuildFilter )
{
    SChar * sRebuildFilterString = NULL;
    SChar * sShardKeyCol = NULL;
    SInt    sRebuildFilterMaxSize = (SDU_SHARD_TRANSFORM_STRING_LENGTH_MAX+1);
   
    sShardKeyCol = aShardObject->mTableInfo.mKeyColumnName;
        
    IDE_TEST(STRUCT_ALLOC_WITH_COUNT(QC_QMP_MEM(aStatement),
                                     SChar,
                                     sRebuildFilterMaxSize,
                                     &sRebuildFilterString)
             != IDE_SUCCESS);

    // Init string
    aRebuildFilter->stmtText = sRebuildFilterString;
    aRebuildFilter->offset = 0;    
    aRebuildFilter->size = 0;

    if ( aRebuildInfo->mValue != NULL )
    {
        //------------------------------------------
        // Query에 shard key condition이 존재하기 때문에
        // Shard key condition을 활용하여 filter string을 생성한다.
        //------------------------------------------
        IDE_TEST( makeFilterStringWithValue( aStatement,
                                             aShardObject,
                                             aRebuildInfo,
                                             sShardKeyCol,
                                             aRebuildRanges,
                                             aRebuildFilter )
                  != IDE_SUCCESS );
    }
    else
    {
        //------------------------------------------
        // Query에 shard key condition이 존재하지 않기 때문에
        // Range를 활용하여 filter string을 생성한다.
        //------------------------------------------
        IDE_TEST( makeFilterStringWithRange( aShardObject,
                                             sShardKeyCol,
                                             aRebuildRanges,
                                             aRebuildFilter,
                                             sRebuildFilterMaxSize )
                  != IDE_SUCCESS );
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmvShardTransform::makeFilterStringWithValue( qcStatement         * aStatement,
                                                     sdiObjectInfo       * aShardObject,
                                                     sdiRebuildInfo      * aRebuildInfo,
                                                     SChar               * aShardKeyCol,
                                                     sdiRebuildRangeList * aRebuildRanges,
                                                     qcNamePosition      * aRebuildFilter )
{
    UShort i = 0;
    sdiRebuildRangeType sFilterType = SDI_REBUILD_RANGE_NONE;
    sdiValue sValueStr;
    SInt sTransformStringMaxSize = (SDU_SHARD_TRANSFORM_STRING_LENGTH_MAX+1);

    aRebuildFilter->offset = 0;
    aRebuildFilter->size = 0;

    for ( i = 0;
          i < aRebuildInfo->mSessionSMNAnalysis->mValueCount;
          i++ )
    {
        IDE_TEST( getFilterType( aShardObject,
                                 aRebuildRanges,
                                 &aRebuildInfo->mValue[i],
                                 &sFilterType )
                  != IDE_SUCCESS );

        IDE_TEST( getValueStr( aShardObject->mTableInfo.mKeyDataType,
                               &aRebuildInfo->mValue[i],
                               &sValueStr )
                  != IDE_SUCCESS );

        if ( sFilterType == SDI_REBUILD_RANGE_INCLUDE )
        {
            if ( aRebuildFilter->size == 0 )
            {
                //------------------------------------------
                // 수행에 포함 할 filter
                // ex. ( i1 = 3 or i1 = 6... )
                //------------------------------------------
                IDE_TEST(STRUCT_ALLOC_WITH_COUNT(QC_QMP_MEM(aStatement),
                                                 SChar,
                                                 sTransformStringMaxSize,
                                                 &aRebuildFilter->stmtText)
                         != IDE_SUCCESS);
            }
            else
            {
                // Step 6. ( I1 = '3' ) OR
                aRebuildFilter->size +=
                    idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                     sTransformStringMaxSize - aRebuildFilter->size,
                                     " OR" );

            }

            // Step 1. (
            aRebuildFilter->size +=
                idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                 sTransformStringMaxSize - aRebuildFilter->size,
                                 " ( " );

            // Step 2. ( I1
            aRebuildFilter->size +=
                idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                 sTransformStringMaxSize - aRebuildFilter->size,
                                 aShardKeyCol );

            // Step 3. ( I1 =
            aRebuildFilter->size +=
                idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                 sTransformStringMaxSize - aRebuildFilter->size,
                                 " = " );

            if ( ( aShardObject->mTableInfo.mKeyDataType == MTD_CHAR_ID ) ||
                 ( aShardObject->mTableInfo.mKeyDataType == MTD_VARCHAR_ID ) )
            {
                aRebuildFilter->stmtText[aRebuildFilter->size] = '\'';
                aRebuildFilter->size++;
            }
            else
            {
                // Nothing to do.
            }

            // Step 4. ( I1 = '3
            aRebuildFilter->size +=
                idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                 sTransformStringMaxSize - aRebuildFilter->size,
                                 "%.*s",
                                 sValueStr.mCharMax.length,
                                 (SChar*)sValueStr.mCharMax.value );

            if ( ( aShardObject->mTableInfo.mKeyDataType == MTD_CHAR_ID ) ||
                 ( aShardObject->mTableInfo.mKeyDataType == MTD_VARCHAR_ID ) )
            {
                aRebuildFilter->stmtText[aRebuildFilter->size] = '\'';
                aRebuildFilter->size++;
            }
            else
            {
                // Nothing to do.
            }

            // Step 5. ( I1 = '3' )
            aRebuildFilter->size +=
                idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                 sTransformStringMaxSize - aRebuildFilter->size,
                                 " ) " );
        }
        else
        {
            // Nothing to do.
        }
    }

    //------------------------------------------
    // Include filter가 없으면 안된다.
    //------------------------------------------
    IDE_TEST_RAISE( aRebuildFilter->size == 0, ERR_SHARD_REBUILD_ERROR );

    // Transformation string buffer overflow
    IDE_TEST_RAISE( aRebuildFilter->size >= (sTransformStringMaxSize-1), ERR_TRANSFORM_STRING_BUFFER_OVERFLOW );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_SHARD_REBUILD_ERROR )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SHARD_REBUILD_ERROR ) );
    }
    IDE_EXCEPTION( ERR_TRANSFORM_STRING_BUFFER_OVERFLOW )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmvShardTransform::makeFilterStringWithValue",
                                  "Transformation string buffer overflow" ));
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmvShardTransform::getFilterType( sdiObjectInfo       * aShardObject,
                                         sdiRebuildRangeList * aRebuildRanges,
                                         sdiValue            * aFilterValue,
                                         sdiRebuildRangeType * aFilterType )
{
    sdiRebuildRangeList * sRebuildRanges = NULL;
    const mtdModule * sKeyModule   = NULL;
    UInt sKeyDataType = ID_USHORT_MAX;
    UInt sHashValue = ID_UINT_MAX;
    SShort sCompare = ID_SSHORT_MAX; // -1 : less, 0 : equal, 1 : greater
    sdiValue sValue;

    // get shard key data type for comparison
    if ( aShardObject->mTableInfo.mSplitMethod == SDI_SPLIT_HASH )
    {
        // get key module for hash
        IDE_TEST( mtd::moduleById( &sKeyModule,
                                   aShardObject->mTableInfo.mKeyDataType )
                  != IDE_SUCCESS );

        sKeyDataType = MTD_INTEGER_ID;

        sHashValue = sKeyModule->hash( mtc::hashInitialValue,
                                       NULL,
                                       (void*)aFilterValue );

        sHashValue = sHashValue % SDI_HASH_MAX_VALUE;
        sValue.mIntegerMax = sHashValue;
    }
    else
    {
        sKeyDataType = aShardObject->mTableInfo.mKeyDataType;

        idlOS::memcpy( (void*)&sValue,
                       (void*)aFilterValue,
                       ID_SIZEOF(sdiValue) );
    }

    for ( sRebuildRanges  = aRebuildRanges;
          sRebuildRanges != NULL;
          sRebuildRanges  = sRebuildRanges->mNext )
    {
        if ( sRebuildRanges->mIsDefault == ID_FALSE )
        {
            // compare shard key
            IDE_TEST( sdi::compareKeyData( sKeyDataType,
                                           &sValue, // A
                                           &sRebuildRanges->mValue, // B
                                           &sCompare ) // A = B : 0, A > B : 1, A < B : -1
                      != IDE_SUCCESS );

            if ( aShardObject->mTableInfo.mSplitMethod == SDI_SPLIT_LIST )
            {
                if ( sCompare == 0 )
                {
                    *aFilterType = sRebuildRanges->mType;
                    break;
                }
                else
                {
                    // Nothing to do.
                }
            }
            else
            {
                IDE_DASSERT( ( aShardObject->mTableInfo.mSplitMethod == SDI_SPLIT_HASH ) ||
                             ( aShardObject->mTableInfo.mSplitMethod == SDI_SPLIT_RANGE ) );

                if ( sCompare == -1 )
                {
                    *aFilterType = sRebuildRanges->mType;
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
            // default node ( at the last of rebuildRanges )
            *aFilterType = sRebuildRanges->mType;
            break;
        }
    }
    
    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmvShardTransform::getValueStr( UInt       aKeyDataType,
                                       sdiValue * aValue,
                                       sdiValue * aValueStr )
{
    switch(aKeyDataType)
    {
        case MTD_CHAR_ID:
        case MTD_VARCHAR_ID:
            idlOS::memcpy( (void*)aValueStr,
                           (void*)aValue,
                           ID_SIZEOF(sdiValue) );
            break;
        case MTD_SMALLINT_ID:
            aValueStr->mCharMax.length = idlOS::snprintf( (SChar*)aValueStr->mCharMax.value,
                                                          ID_SIZEOF(aValueStr),
                                                          "%"ID_INT32_FMT,
                                                          (SShort)aValue->mSmallintMax );
            break;
        case MTD_INTEGER_ID:
            aValueStr->mCharMax.length = idlOS::snprintf( (SChar*)aValueStr->mCharMax.value,
                                                          ID_SIZEOF(aValueStr),
                                                          "%"ID_INT32_FMT,
                                                          (SInt)aValue->mIntegerMax );
            break;
        case MTD_BIGINT_ID:
            aValueStr->mCharMax.length = idlOS::snprintf( (SChar*)aValueStr->mCharMax.value,
                                                          ID_SIZEOF(aValueStr),
                                                          "%"ID_INT64_FMT,
                                                          (SLong)aValue->mBigintMax );
            break;
        default :
            // 발생하지 않는다.
            IDE_DASSERT(0);
            IDE_RAISE(ERR_SHARD_REBUILD_ERROR);
            break;
    }
    
    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_SHARD_REBUILD_ERROR )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SHARD_REBUILD_ERROR ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmvShardTransform::makeFilterStringWithRange( sdiObjectInfo       * aShardObject,
                                                     SChar               * aShardKeyCol,
                                                     sdiRebuildRangeList * aRebuildRanges,
                                                     qcNamePosition      * aRebuildFilter,
                                                     SInt                  aRebuildFilterMaxSize )
{
    UInt sKeyDataType = ID_UINT_MAX;

    if ( aShardObject->mTableInfo.mSplitMethod == SDI_SPLIT_HASH )
    {
        sKeyDataType = MTD_INTEGER_ID;

        IDE_TEST( makeHashFilter( sKeyDataType,
                                  aShardKeyCol,
                                  aRebuildRanges,
                                  aRebuildFilter,
                                  aRebuildFilterMaxSize )
                  != IDE_SUCCESS );
    }
    else if ( aShardObject->mTableInfo.mSplitMethod == SDI_SPLIT_RANGE )
    {
        sKeyDataType = aShardObject->mTableInfo.mKeyDataType;

        IDE_TEST( makeRangeFilter( sKeyDataType,
                                   aShardKeyCol,
                                   aRebuildRanges,
                                   aRebuildFilter,
                                   aRebuildFilterMaxSize )
                  != IDE_SUCCESS );
    }
    else // ( aShardObject->mTableInfo.mSplitMethod == SDI_SPLIT_LIST )
    {
        sKeyDataType = aShardObject->mTableInfo.mKeyDataType;

        IDE_TEST( makeListFilter( sKeyDataType,
                                  aShardKeyCol,
                                  aRebuildRanges,
                                  aRebuildFilter,
                                  aRebuildFilterMaxSize )
                  != IDE_SUCCESS );
    }
    
    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmvShardTransform::makeHashFilter( UInt                  aKeyDataType,
                                          SChar               * aShardKeyCol,
                                          sdiRebuildRangeList * aRebuildRanges,
                                          qcNamePosition      * aRebuildFilter,
                                          SInt                  aRebuildFilterMaxSize )
{
    sdiRebuildRangeList * sRangeList = NULL;

    sdiValue * sMinValue = NULL;
    sdiValue * sMaxValue = NULL;

    sdiValue sValueStr;

    for ( sRangeList  = aRebuildRanges;
          sRangeList != NULL;
          sRangeList  = sRangeList->mNext )
    {
        sMinValue = sMaxValue;
        sMaxValue = &sRangeList->mValue;

        if ( sRangeList->mType == SDI_REBUILD_RANGE_INCLUDE )
        {
            if ( aRebuildFilter->size > 0 )
            {
                // Step 6. ( MOD(HASH(I1),1000) < 2 ) OR
                aRebuildFilter->size +=
                    idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                     aRebuildFilterMaxSize - aRebuildFilter->size,
                                     " OR " );
            }
            else
            {
                // Nothing to do.
            }

            // Step 1. (
            aRebuildFilter->size +=
                idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                 aRebuildFilterMaxSize - aRebuildFilter->size,
                                 " ( " );

            if ( sRangeList->mIsDefault == ID_FALSE )
            {
                if ( sMinValue != NULL )
                {
                    // Step 7-1. ( MOD(HASH(I1),1000) < 2 ) OR ( MOD(HASH(I1),1000)
                    aRebuildFilter->size +=
                        idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                         aRebuildFilterMaxSize - aRebuildFilter->size,
                                         " MOD(HASH(" );
                    
                    aRebuildFilter->size +=
                        idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                         aRebuildFilterMaxSize - aRebuildFilter->size,
                                         aShardKeyCol );

                    aRebuildFilter->size +=
                        idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                         aRebuildFilterMaxSize - aRebuildFilter->size,
                                         "),1000)" );

                    // Step 7-2. ( MOD(HASH(I1),1000) < 2 ) OR ( MOD(HASH(I1),1000) >= 
                    aRebuildFilter->size +=
                        idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                         aRebuildFilterMaxSize - aRebuildFilter->size,
                                         " >= " );

                    IDE_TEST( getValueStr( aKeyDataType,
                                           sMinValue,
                                           &sValueStr )
                              != IDE_SUCCESS );

                    // Step 7-3. ( MOD(HASH(I1),1000) < 2 ) OR ( MOD(HASH(I1),1000) >= 2
                    aRebuildFilter->size +=
                        idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                         aRebuildFilterMaxSize - aRebuildFilter->size,
                                         "%.*s",
                                         sValueStr.mCharMax.length,
                                         (SChar*)sValueStr.mCharMax.value );

                    // Step 7-4. ( MOD(HASH(I1),1000) < 2 ) OR ( MOD(HASH(I1),1000) >= 2 AND
                    aRebuildFilter->size +=
                        idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                         aRebuildFilterMaxSize - aRebuildFilter->size,
                                         " AND " );
                }
                else
                {
                    // The first range
                    // Nothing to do.
                }

                // Step 2. ( MOD(HASH(I1),1000)
                // Step 7-5. ( MOD(HASH(I1),1000) < 2 ) OR ( MOD(HASH(I1),1000) >= 2 AND MOD(HASH(I1,1000))
                aRebuildFilter->size +=
                    idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                     aRebuildFilterMaxSize - aRebuildFilter->size,
                                     " MOD(HASH(" );
                    
                aRebuildFilter->size +=
                    idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                     aRebuildFilterMaxSize - aRebuildFilter->size,
                                     aShardKeyCol );

                aRebuildFilter->size +=
                    idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                     aRebuildFilterMaxSize - aRebuildFilter->size,
                                     "),1000)" );

                // Step 3. ( MOD(HASH(I1),1000)
                // Step 7-5. ( MOD(HASH(I1),1000) < 2 ) OR ( MOD(HASH(I1),1000) >= 2 AND MOD(HASH(I1,1000)) <
                aRebuildFilter->size +=
                    idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                     aRebuildFilterMaxSize - aRebuildFilter->size,
                                     " < " );

                IDE_TEST( getValueStr( aKeyDataType,
                                       sMaxValue,
                                       &sValueStr )
                          != IDE_SUCCESS );

                // Step 4. ( I1 < '2'     Step 7-7. ( I1 < '2' ) OR ( I1 >= '2' AND I1 < '5'
                aRebuildFilter->size +=
                    idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                     aRebuildFilterMaxSize - aRebuildFilter->size,
                                     "%.*s",
                                     sValueStr.mCharMax.length,
                                     (SChar*)sValueStr.mCharMax.value );
            }
            else
            {
                // Default
                if ( sMinValue != NULL )
                {
                    //Step 8-1. ( MOD(HASH(I1),1000) < 2 ) OR ( MOD(HASH(I1),1000) >= 2 AND MOD(HASH(I1),1000) < 5 )
                    //       OR ( MOD(HASH(I1),1000)
                    aRebuildFilter->size +=
                        idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                         aRebuildFilterMaxSize - aRebuildFilter->size,
                                         " MOD(HASH(" );
                    
                    aRebuildFilter->size +=
                        idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                         aRebuildFilterMaxSize - aRebuildFilter->size,
                                         aShardKeyCol );

                    aRebuildFilter->size +=
                        idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                         aRebuildFilterMaxSize - aRebuildFilter->size,
                                         "),1000)" );

                    //Step 8-2. ( MOD(HASH(I1),1000) < 2 ) OR ( MOD(HASH(I1),1000) >= 2 AND MOD(HASH(I1),1000) < 5 )
                    //       OR ( MOD(HASH(I1),1000) >=
                    aRebuildFilter->size +=
                        idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                         aRebuildFilterMaxSize - aRebuildFilter->size,
                                         " >= " );

                    IDE_TEST( getValueStr( aKeyDataType,
                                           sMinValue,
                                           &sValueStr )
                              != IDE_SUCCESS );

                    // Step 8-3. ( MOD(HASH(I1),1000) < 2 ) OR ( MOD(HASH(I1),1000) >= 2 AND MOD(HASH(I1),1000) < 5 )
                    //        OR ( MOD(HASH(I1),1000) >= 5 )
                    aRebuildFilter->size +=
                        idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                         aRebuildFilterMaxSize - aRebuildFilter->size,
                                         "%.*s",
                                         sValueStr.mCharMax.length,
                                         (SChar*)sValueStr.mCharMax.value );
                }
                else
                {
                    // Default 외에 다른 분산 정보가 없다.
                    aRebuildFilter->size +=
                        idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                         aRebuildFilterMaxSize - aRebuildFilter->size,
                                         " 1 = 1 " );
                }
            }

            // Step 5. ( MOD(HASH(I1),1000) < 2 )
            // Step 7-8. ( MOD(HASH(I1),1000) < 2 ) OR ( MOD(HASH(I1),1000) >= 2 AND MOD(HASH(I1),1000) < 5 )
            aRebuildFilter->size +=
                idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                 aRebuildFilterMaxSize - aRebuildFilter->size,
                                 " ) " );
        }
        else
        {
            // Nothing to do.
        }
    }

    // Include node가 하나도 없다.
    // 가지고 있던 데이터가 하나도 없었으므로, 아무것도 리턴하면 안된다.
    if ( aRebuildFilter->size == 0 )
    {
        aRebuildFilter->size +=
            idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                             aRebuildFilterMaxSize - aRebuildFilter->size,
                             " 1 != 1 " );
    }
    else
    {
        // Nothing to do.
    }

    // Transformation string buffer overflow
    IDE_TEST_RAISE( aRebuildFilter->size >= (aRebuildFilterMaxSize-1), ERR_TRANSFORM_STRING_BUFFER_OVERFLOW );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_TRANSFORM_STRING_BUFFER_OVERFLOW )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmvShardTransform::makeHashFilter",
                                  "Transformation string buffer overflow" ));
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmvShardTransform::makeRangeFilter( UInt                  aKeyDataType,
                                           SChar               * aShardKeyCol,
                                           sdiRebuildRangeList * aRebuildRanges,
                                           qcNamePosition      * aRebuildFilter,
                                           SInt                  aRebuildFilterMaxSize )
{
    sdiRebuildRangeList * sRangeList = NULL;

    sdiValue * sMinValue = NULL;
    sdiValue * sMaxValue = NULL;

    sdiValue sValueStr;

    for ( sRangeList  = aRebuildRanges;
          sRangeList != NULL;
          sRangeList  = sRangeList->mNext )
    {
        sMinValue = sMaxValue;
        sMaxValue = &sRangeList->mValue;

        if ( sRangeList->mType == SDI_REBUILD_RANGE_INCLUDE )
        {
            if ( aRebuildFilter->size > 0 )
            {
                // Step 6. ( I1 < '2' ) OR
                aRebuildFilter->size +=
                    idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                     aRebuildFilterMaxSize - aRebuildFilter->size,
                                     " OR " );
            }
            else
            {
                // Nothing to do.
            }

            // Step 1. (
            aRebuildFilter->size +=
                idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                 aRebuildFilterMaxSize - aRebuildFilter->size,
                                 " ( " );

            if ( sRangeList->mIsDefault == ID_FALSE )
            {
                if ( sMinValue != NULL )
                {
                    // Step 7-1. ( I1 < '2' ) OR ( I1 
                    aRebuildFilter->size +=
                        idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                         aRebuildFilterMaxSize - aRebuildFilter->size,
                                         aShardKeyCol );

                    // Step 7-2. ( I1 < '2' ) OR ( I1 >= 
                    aRebuildFilter->size +=
                        idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                         aRebuildFilterMaxSize - aRebuildFilter->size,
                                         " >= " );

                    IDE_TEST( getValueStr( aKeyDataType,
                                           sMinValue,
                                           &sValueStr )
                              != IDE_SUCCESS );

                    if ( ( aKeyDataType == MTD_CHAR_ID ) ||
                         ( aKeyDataType == MTD_VARCHAR_ID ) )
                    {
                        aRebuildFilter->stmtText[aRebuildFilter->size] = '\'';
                        aRebuildFilter->size++;
                    }
                    else
                    {
                        // Nothing to do.
                    }

                    // Step 7-3. ( I1 < '2' ) OR ( I1 >= '2'
                    aRebuildFilter->size +=
                        idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                         aRebuildFilterMaxSize - aRebuildFilter->size,
                                         "%.*s",
                                         sValueStr.mCharMax.length,
                                         (SChar*)sValueStr.mCharMax.value );

                    if ( ( aKeyDataType == MTD_CHAR_ID ) ||
                         ( aKeyDataType == MTD_VARCHAR_ID ) )
                    {
                        aRebuildFilter->stmtText[aRebuildFilter->size] = '\'';
                        aRebuildFilter->size++;
                    }
                    else
                    {
                        // Nothing to do.
                    }

                    // Step 7-4. ( I1 < '2' ) OR ( I1 >= '2' AND
                    aRebuildFilter->size +=
                        idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                         aRebuildFilterMaxSize - aRebuildFilter->size,
                                         " AND " );
                }
                else
                {
                    // The first range
                    // Nothing to do.
                }

                // Step 2. ( I1           Step 7-5. ( I1 < '2' ) OR ( I1 >= '2' AND I1 
                aRebuildFilter->size +=
                    idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                     aRebuildFilterMaxSize - aRebuildFilter->size,
                                     aShardKeyCol );

                // Step 3. ( I1 <         Step 7-6. ( I1 < '2' ) OR ( I1 >= '2' AND I1 <
                aRebuildFilter->size +=
                    idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                     aRebuildFilterMaxSize - aRebuildFilter->size,
                                     " < " );

                IDE_TEST( getValueStr( aKeyDataType,
                                       sMaxValue,
                                       &sValueStr )
                          != IDE_SUCCESS );

                if ( ( aKeyDataType == MTD_CHAR_ID ) ||
                     ( aKeyDataType == MTD_VARCHAR_ID ) )
                {
                    aRebuildFilter->stmtText[aRebuildFilter->size] = '\'';
                    aRebuildFilter->size++;
                }
                else
                {
                    // Nothing to do.
                }

                // Step 4. ( I1 < '2'     Step 7-7. ( I1 < '2' ) OR ( I1 >= '2' AND I1 < '5'
                aRebuildFilter->size +=
                    idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                     aRebuildFilterMaxSize - aRebuildFilter->size,
                                     "%.*s",
                                     sValueStr.mCharMax.length,
                                     (SChar*)sValueStr.mCharMax.value );

                if ( ( aKeyDataType == MTD_CHAR_ID ) ||
                     ( aKeyDataType == MTD_VARCHAR_ID ) )
                {
                    aRebuildFilter->stmtText[aRebuildFilter->size] = '\'';
                    aRebuildFilter->size++;
                }
                else
                {
                    // Nothing to do.
                }
            }
            else
            {
                // Default
                if ( sMinValue != NULL )
                {
                    //Step 8-1. ( I1 < '2' ) OR ( I1 >= '2' AND I1 < '5' ) OR ( I1
                    aRebuildFilter->size +=
                        idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                         aRebuildFilterMaxSize - aRebuildFilter->size,
                                         aShardKeyCol );

                    //Step 8-2. ( I1 < '2' ) OR ( I1 >= '2' AND I1 < '5' ) OR ( I1 >=
                    aRebuildFilter->size +=
                        idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                         aRebuildFilterMaxSize - aRebuildFilter->size,
                                         " >= " );

                    IDE_TEST( getValueStr( aKeyDataType,
                                           sMinValue,
                                           &sValueStr )
                              != IDE_SUCCESS );

                    if ( ( aKeyDataType == MTD_CHAR_ID ) ||
                         ( aKeyDataType == MTD_VARCHAR_ID ) )
                    {
                        aRebuildFilter->stmtText[aRebuildFilter->size] = '\'';
                        aRebuildFilter->size++;
                    }
                    else
                    {
                        // Nothing to do.
                    }

                    // Step 8-3. ( I1 < '2' ) OR ( I1 >= '2' AND I1 < '5' ) OR ( I1 >= '5' )
                    aRebuildFilter->size +=
                        idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                         aRebuildFilterMaxSize - aRebuildFilter->size,
                                         "%.*s",
                                         sValueStr.mCharMax.length,
                                         (SChar*)sValueStr.mCharMax.value );

                    if ( ( aKeyDataType == MTD_CHAR_ID ) ||
                         ( aKeyDataType == MTD_VARCHAR_ID ) )
                    {
                        aRebuildFilter->stmtText[aRebuildFilter->size] = '\'';
                        aRebuildFilter->size++;
                    }
                    else
                    {
                        // Nothing to do.
                    }

                }
                else
                {
                    // Default 외에 다른 분산 정보가 없다.
                    aRebuildFilter->size +=
                        idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                         aRebuildFilterMaxSize - aRebuildFilter->size,
                                         " 1 = 1 " );
                }
            }

            // Step 5. ( I1 < '2' )     Step 7-8. ( I1 < '2' ) OR ( I1 >= '2' AND I1 < '5' )
            aRebuildFilter->size +=
                idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                 aRebuildFilterMaxSize - aRebuildFilter->size,
                                 " ) " );
        }
        else
        {
            // Nothing to do.
        }
    }

    // Include node가 하나도 없다.
    // 가지고 있던 데이터가 하나도 없었으므로, 아무것도 리턴하면 안된다.
    if ( aRebuildFilter->size == 0 )
    {
        aRebuildFilter->size +=
            idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                             aRebuildFilterMaxSize - aRebuildFilter->size,
                             " 1 != 1 " );
    }
    else
    {
        // Nothing to do.
    }

    // Transformation string buffer overflow
    IDE_TEST_RAISE( aRebuildFilter->size >= (aRebuildFilterMaxSize-1), ERR_TRANSFORM_STRING_BUFFER_OVERFLOW );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_TRANSFORM_STRING_BUFFER_OVERFLOW )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmvShardTransform::makeRangeFilter",
                                  "Transformation string buffer overflow" ));
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmvShardTransform::makeListFilter( UInt                  aKeyDataType,
                                          SChar               * aShardKeyCol,
                                          sdiRebuildRangeList * aRebuildRanges,
                                          qcNamePosition      * aRebuildFilter,
                                          SInt                  aRebuildFilterMaxSize )
{
    sdiRebuildRangeList * sRangeList = NULL;
    sdiRebuildRangeList * sNotEqualList = NULL;
    sdiValue sValueStr;

    for ( sRangeList  = aRebuildRanges;
          sRangeList != NULL;
          sRangeList  = sRangeList->mNext )
    {
        if ( sRangeList->mType == SDI_REBUILD_RANGE_INCLUDE )
        {
            if ( aRebuildFilter->size > 0 )
            {
                // Step 6. ( I1 = 3 ) OR
                aRebuildFilter->size +=
                    idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                     aRebuildFilterMaxSize - aRebuildFilter->size,
                                     " OR " );
            }
            else
            {
                // Nothing to do.
            }

            if ( sRangeList->mIsDefault == ID_FALSE )
            {
                // Step 1. (
                aRebuildFilter->size +=
                    idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                     aRebuildFilterMaxSize - aRebuildFilter->size,
                                     " ( " );

                // Step 2. ( I1
                aRebuildFilter->size +=
                    idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                     aRebuildFilterMaxSize - aRebuildFilter->size,
                                     aShardKeyCol );
                // Step 3. ( I1 = 
                aRebuildFilter->size +=
                    idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                     aRebuildFilterMaxSize - aRebuildFilter->size,
                                     " = " );

                IDE_TEST( getValueStr( aKeyDataType,
                                       &sRangeList->mValue,
                                       &sValueStr )
                          != IDE_SUCCESS );

                if ( ( aKeyDataType == MTD_CHAR_ID ) ||
                     ( aKeyDataType == MTD_VARCHAR_ID ) )
                {
                    aRebuildFilter->stmtText[aRebuildFilter->size] = '\'';
                    aRebuildFilter->size++;
                }
                else
                {
                    // Nothing to do.
                }

                // Step 4. ( I1 = 3
                aRebuildFilter->size +=
                    idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                     aRebuildFilterMaxSize - aRebuildFilter->size,
                                     "%.*s",
                                     sValueStr.mCharMax.length,
                                     (SChar*)sValueStr.mCharMax.value );

                if ( ( aKeyDataType == MTD_CHAR_ID ) ||
                     ( aKeyDataType == MTD_VARCHAR_ID ) )
                {
                    aRebuildFilter->stmtText[aRebuildFilter->size] = '\'';
                    aRebuildFilter->size++;
                }
                else
                {
                    // Nothing to do.
                }

                // Step 5. ( I1 = 3 )
                aRebuildFilter->size +=
                    idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                     aRebuildFilterMaxSize - aRebuildFilter->size,
                                     " ) " );
            }
            else
            {
                if ( aRebuildFilter->size > 0 )
                {
                    // LIST range가 존재 한다면, not equal(!=) filter로 default node filter를 달아준다.
                    aRebuildFilter->size +=
                        idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                         aRebuildFilterMaxSize - aRebuildFilter->size,
                                         " ( " );
                    // Default
                    for ( sNotEqualList  = aRebuildRanges;
                          sNotEqualList != NULL;
                          sNotEqualList  = sNotEqualList->mNext )
                    {
                        if ( sNotEqualList->mIsDefault == ID_FALSE )
                        {
                            if ( sNotEqualList != aRebuildRanges )
                            {
                                aRebuildFilter->size +=
                                    idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                                     aRebuildFilterMaxSize - aRebuildFilter->size,
                                                     " AND " );
                            }
                            else
                            {
                                // Nothing to do.
                            }

                            aRebuildFilter->size +=
                                idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                                 aRebuildFilterMaxSize - aRebuildFilter->size,
                                                 aShardKeyCol );

                            aRebuildFilter->size +=
                                idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                                 aRebuildFilterMaxSize - aRebuildFilter->size,
                                                 " != " );

                            IDE_TEST( getValueStr( aKeyDataType,
                                                   &sNotEqualList->mValue,
                                                   &sValueStr )
                                      != IDE_SUCCESS );

                            if ( ( aKeyDataType == MTD_CHAR_ID ) ||
                                 ( aKeyDataType == MTD_VARCHAR_ID ) )
                            {
                                aRebuildFilter->stmtText[aRebuildFilter->size] = '\'';
                                aRebuildFilter->size++;
                            }
                            else
                            {
                                // Nothing to do.
                            }

                            aRebuildFilter->size +=
                                idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                                 aRebuildFilterMaxSize - aRebuildFilter->size,
                                                 "%.*s",
                                                 sValueStr.mCharMax.length,
                                                 (SChar*)sValueStr.mCharMax.value );

                            if ( ( aKeyDataType == MTD_CHAR_ID ) ||
                                 ( aKeyDataType == MTD_VARCHAR_ID ) )
                            {
                                aRebuildFilter->stmtText[aRebuildFilter->size] = '\'';
                                aRebuildFilter->size++;
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

                    aRebuildFilter->size +=
                        idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                         aRebuildFilterMaxSize - aRebuildFilter->size,
                                         " ) " );
                }
                else
                {
                    // Default 외에 다른 분산 정보가 없다.
                    aRebuildFilter->size +=
                        idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                                         aRebuildFilterMaxSize - aRebuildFilter->size,
                                         " 1 = 1 " );
                }
            }
        }
        else
        {
            // Nothing to do.
        }
    }

    // Include node가 하나도 없다.
    // 가지고 있던 데이터가 하나도 없었으므로, 아무것도 리턴하면 안된다.
    if ( aRebuildFilter->size == 0 )
    {
        aRebuildFilter->size +=
            idlOS::snprintf( aRebuildFilter->stmtText + aRebuildFilter->size,
                             aRebuildFilterMaxSize - aRebuildFilter->size,
                             " 1 != 1 " );
    }
    else
    {
        // Nothing to do.
    }

    // Transformation string buffer overflow
    IDE_TEST_RAISE( aRebuildFilter->size >= (aRebuildFilterMaxSize-1), ERR_TRANSFORM_STRING_BUFFER_OVERFLOW );
    
    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_TRANSFORM_STRING_BUFFER_OVERFLOW )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QMC_UNEXPECTED_ERROR,
                                  "qmvShardTransform::makeListFilter",
                                  "Transformation string buffer overflow" ));
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC qmvShardTransform::rebuildTransformExpr( qcStatement    * aStatement,
                                                sdiRebuildInfo * aRebuildInfo,
                                                qtcNode        * aExpr )
{
    qtcNode * sNode = NULL;

    if ( ( aExpr->node.lflag & MTC_NODE_OPERATOR_MASK )
         == MTC_NODE_OPERATOR_SUBQUERY )
    {
        IDE_TEST( rebuildNonShardTransform( aExpr->subquery,
                                            aRebuildInfo )
                  != IDE_SUCCESS );
    }
    else
    {
        // traverse
        for ( sNode = (qtcNode*)aExpr->node.arguments;
              sNode != NULL;
              sNode = (qtcNode*)sNode->node.next )
        {
            IDE_TEST( rebuildTransformExpr( aStatement,
                                            aRebuildInfo,
                                            sNode )
                      != IDE_SUCCESS );
        }
    }
    
    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}
