/** 
 *  Copyright (c) 1999~2017, Altibase Corp. and/or its affiliates. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License, version 3,
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
 

/***********************************************************************
 * $Id$
 **********************************************************************/

#include <uln.h>
#include <ulnPrivate.h>
#include <ulnStateMachine.h>
#include <mtcc.h>

#include <ulsd.h>
#include <ulsdFailover.h>
#include <ulsdCommunication.h>
#include <ulsdnExecute.h>

static acp_bool_t ulsdNeedShardStmtPartialRollback( acp_uint32_t aStmtKind )
{
    acp_bool_t  sNeedShardStmtPartialRollback = ACP_FALSE;

    if ( ( aStmtKind == ULN_STMT_INSERT ) ||
         ( aStmtKind == ULN_STMT_UPDATE ) ||
         ( aStmtKind == ULN_STMT_DELETE ) ||
         ( aStmtKind == ULN_STMT_SELECT_FOR_UPDATE ) )
    {
        sNeedShardStmtPartialRollback = ACP_TRUE;
    }
    else
    {
        sNeedShardStmtPartialRollback = ACP_FALSE;
    }

    return sNeedShardStmtPartialRollback;
}

static acp_rc_t ulsdShardStmtPartialRollback( ulnFnContext      * aFnContext,
                                              ulsdDbc           * aShard,
                                              ulsdFuncCallback  * aCallback )
{
    ulsdFuncCallback * sCallback = aCallback;
    acp_rc_t           sRet = ACI_SUCCESS;

    while ( sCallback != NULL )
    {
        switch( sCallback->mRet )
        {
            case SQL_SUCCESS:
            case SQL_SUCCESS_WITH_INFO:
            case SQL_NO_DATA:
                if ( ulsdnStmtShardStmtPartialRollback( sCallback->mStmt->mParentDbc ) != SQL_SUCCESS )
                {
                    ulsdNativeErrorToUlnError( aFnContext,
                                               SQL_HANDLE_DBC,
                                               (ulnObject*)sCallback->mStmt->mParentDbc,
                                               aShard->mNodeInfo[sCallback->mIndex],
                                               (acp_char_t *)"ShardStmtPartialRollback" );

                    sRet = ACI_FAILURE;
                }
                break;

            default:
                break;
        }
        
        sCallback = sCallback->mNext;
    }

    return sRet;
}

static void ulsdProcessExecuteError( ulnFnContext       * aFnContext,
                                     ulnStmt            * aStmt,
                                     ulsdFuncCallback   * aCallback )
{
    
    ulsdNodeReport      sReport;
    ulsdDbc           * sShard = NULL;

    if ( ( aStmt->mParentDbc->mAttrAutoCommit == SQL_AUTOCOMMIT_OFF ) &&
         ( ulsdNeedShardStmtPartialRollback( ulnStmtGetStatementType( aStmt ) ) == ACP_TRUE ) )
    {
        ulsdGetShardFromDbc( aStmt->mParentDbc, &sShard );

        if ( ulsdShardStmtPartialRollback( aFnContext,
                                           sShard,
                                           aCallback ) 
             != ACI_SUCCESS )
        {
            if ( ulsdIsFailoverExecute( aFnContext ) == ACP_TRUE )
            {
                /* do nothing */
            }
            else
            {
                sReport.mType = ULSD_REPORT_TYPE_TRANSACTION_BROKEN;

                if ( ulsdShardNodeReport( aFnContext, &sReport ) != ACI_SUCCESS )
                {
                    if ( ulsdIsFailoverExecute( aFnContext ) == ACP_TRUE )
                    {
                        /* do nothing */
                    }
                    else
                    {
                        ulnError( aFnContext,
                                  ulERR_ABORT_SHARD_INTERNAL_ERROR,
                                  "ShardNodeReport(Transaction Broken) fails" );
                    }
                }
            }
        }
    }
}

SQLRETURN ulsdFetch(ulnStmt *aStmt)
{
    /* PROJ-2598 altibase sharding */
    SQLRETURN     sNodeResult = SQL_ERROR;
    ulnFnContext  sFnContext;
    ulnFnContext  sFnContextSMNUpdate;

    ULN_INIT_FUNCTION_CONTEXT(sFnContext, ULN_FID_FETCH, aStmt, ULN_OBJ_TYPE_STMT);

    /* clear diagnostic info */
    ACI_TEST(ulnClearDiagnosticInfoFromObject((ulnObject*)aStmt) != ACI_SUCCESS);

    sNodeResult = ulsdModuleFetch(&sFnContext, aStmt);

    /* BUG-46100 Session SMN Update */
    if ( ( aStmt->mParentDbc->mAttrAutoCommit == SQL_AUTOCOMMIT_ON ) &&
         ( ulsdIsTimeToUpdateShardMetaNumber( aStmt->mParentDbc ) == ACP_TRUE ) )
    {
        if ( sNodeResult != SQL_SUCCESS )
        {
            /* ulsdModuleFetch is fail or no data. */
            ULN_INIT_FUNCTION_CONTEXT( sFnContextSMNUpdate, ULN_FID_FETCH, aStmt, ULN_OBJ_TYPE_STMT );

            ACI_TEST( ulsdUpdateShardMetaNumber( aStmt->mParentDbc, & sFnContextSMNUpdate ) != SQL_SUCCESS );

            if ( sNodeResult != SQL_NO_DATA )
            {
                ulnError( & sFnContext, ulERR_ABORT_SHARD_META_CHANGED );
            }
        }
    }
    else
    {
        /* Nothing to do */
    }

    return sNodeResult;

    ACI_EXCEPTION_END;

    return SQL_ERROR;
}

SQLRETURN ulsdFetchNodes(ulnFnContext *aFnContext,
                         ulnStmt      *aStmt)
{
    SQLRETURN          sNodeResult = SQL_ERROR;
    SQLRETURN          sSuccessResult = SQL_SUCCESS;
    SQLRETURN          sErrorResult = SQL_ERROR;
    ulsdDbc           *sShard;
    ulnStmt           *sNodeStmt;
    acp_uint16_t       sNodeDbcIndex;
    acp_uint16_t       i;

    ulsdGetShardFromDbc(aStmt->mParentDbc, &sShard);

    ACI_TEST_RAISE( aStmt->mShardStmtCxt.mNodeDbcIndexCur == -1, LABEL_NOT_EXECUTED );

    while ( 1 )
    {
        i = aStmt->mShardStmtCxt.mNodeDbcIndexCur;

        if ( i < aStmt->mShardStmtCxt.mNodeDbcIndexCount )
        {
            sNodeDbcIndex = aStmt->mShardStmtCxt.mNodeDbcIndexArr[i];
            sNodeStmt = aStmt->mShardStmtCxt.mShardNodeStmt[sNodeDbcIndex];

            sNodeResult = ulnFetch(sNodeStmt);

            SHARD_LOG("(Fetch) NodeId=%d, Server=%s:%d, StmtID=%d\n",
                      sShard->mNodeInfo[sNodeDbcIndex]->mNodeId,
                      sShard->mNodeInfo[sNodeDbcIndex]->mServerIP,
                      sShard->mNodeInfo[sNodeDbcIndex]->mPortNo,
                      sNodeStmt->mStatementID);

            if ( sNodeResult == SQL_SUCCESS )
            {
                /* Nothing to do */
            }
            else if ( sNodeResult == SQL_SUCCESS_WITH_INFO )
            {
                /* info의 전달 */
                ulsdNativeErrorToUlnError(aFnContext,
                                          SQL_HANDLE_STMT,
                                          (ulnObject *)sNodeStmt,
                                          sShard->mNodeInfo[sNodeDbcIndex],
                                          (acp_char_t *)"Fetch");

                sSuccessResult = sNodeResult;
            }
            else if ( sNodeResult == SQL_NO_DATA )
            {
                /* next 데이터 노드를 확인해야 함 */
                aStmt->mShardStmtCxt.mNodeDbcIndexCur++;
                continue;
            }
            else
            {
                sErrorResult = sNodeResult;

                ACI_RAISE(LABEL_NODE_FETCH_FAIL);
            }
        }
        else
        {
            /* 모든 데이터 노드에서 no_data이면 no_data 리턴 */
            sSuccessResult = SQL_NO_DATA;
        }

        break;
    }

    return sSuccessResult;

    ACI_EXCEPTION(LABEL_NOT_EXECUTED)
    {
        ulnError(aFnContext,
                 ulERR_ABORT_SHARD_ERROR,
                 "Shard Executor",
                 "Not executed");
    }
    ACI_EXCEPTION(LABEL_NODE_FETCH_FAIL)
    {
        ulsdNativeErrorToUlnError(aFnContext,
                                  SQL_HANDLE_STMT,
                                  (ulnObject *)sNodeStmt,
                                  sShard->mNodeInfo[sNodeDbcIndex],
                                  (acp_char_t *)"Fetch");
    }
    ACI_EXCEPTION_END;

    return sErrorResult;
}

SQLRETURN ulsdExecute(ulnStmt *aStmt)
{
    /* PROJ-2598 altibase sharding */
    SQLRETURN     sNodeResult;
    ulnFnContext  sFnContext;
    ulnFnContext  sFnContextSMNUpdate;

    ULN_INIT_FUNCTION_CONTEXT(sFnContext, ULN_FID_EXECUTE, aStmt, ULN_OBJ_TYPE_STMT);

    /* clear diagnostic info */
    ACI_TEST(ulnClearDiagnosticInfoFromObject((ulnObject*)aStmt) != ACI_SUCCESS);

    sNodeResult = ulsdModuleExecute(&sFnContext, aStmt);

    /* BUG-46100 Session SMN Update */
    if ( ( aStmt->mParentDbc->mAttrAutoCommit == SQL_AUTOCOMMIT_ON ) &&
         ( ulsdIsTimeToUpdateShardMetaNumber( aStmt->mParentDbc ) == ACP_TRUE ) )
    {
        ULN_INIT_FUNCTION_CONTEXT( sFnContextSMNUpdate, ULN_FID_EXECUTE, aStmt, ULN_OBJ_TYPE_STMT );

        ACI_TEST( ulsdUpdateShardMetaNumber( aStmt->mParentDbc, & sFnContextSMNUpdate ) != SQL_SUCCESS );

        if ( sNodeResult != SQL_SUCCESS )
        {
            ulnError( & sFnContext, ulERR_ABORT_SHARD_META_CHANGED );
        }
    }
    else
    {
        /* Nothing to do */
    }

    return sNodeResult;

    ACI_EXCEPTION_END;

    return SQL_ERROR;
}

SQLRETURN ulsdExecuteNodes(ulnFnContext *aFnContext,
                           ulnStmt      *aStmt)
{
    SQLRETURN          sNodeResult    = SQL_ERROR;
    SQLRETURN          sSuccessResult = SQL_SUCCESS;
    SQLRETURN          sErrorResult   = SQL_ERROR;
    acp_bool_t         sSuccess       = ACP_TRUE;
    ulsdDbc           *sShard;
    ulnStmt           *sNodeStmt;
    acp_uint16_t       sNodeDbcIndex;
    ulsdFuncCallback  *sCallback = NULL;
    ulnResult         *sResult   = NULL;
    acp_uint16_t       sNoDataCount = 0;
    acp_uint16_t       i;

    /* BUG-46257 shardcli에서 Node 추가/제거 지원 */
    aStmt->mShardStmtCxt.mRowCount = 0;

    ulsdGetShardFromDbc(aStmt->mParentDbc, &sShard);

    for ( i = 0; i < aStmt->mShardStmtCxt.mNodeDbcIndexCount; i++ )
    {
        sNodeDbcIndex = aStmt->mShardStmtCxt.mNodeDbcIndexArr[i];
        sNodeStmt = aStmt->mShardStmtCxt.mShardNodeStmt[sNodeDbcIndex];

        sErrorResult = ulsdExecuteAddCallback( sNodeDbcIndex,
                                               sNodeStmt,
                                               &sCallback );
        ACI_TEST( sErrorResult != SQL_SUCCESS );
    }

    /* node execute 병렬수행 */
    ulsdDoCallback( sCallback );

    for ( i = 0; i < aStmt->mShardStmtCxt.mNodeDbcIndexCount; i++ )
    {
        sNodeDbcIndex = aStmt->mShardStmtCxt.mNodeDbcIndexArr[i];
        sNodeStmt = aStmt->mShardStmtCxt.mShardNodeStmt[sNodeDbcIndex];

        sNodeResult = ulsdGetResultCallback( sNodeDbcIndex, sCallback, (acp_uint8_t)0 );

        SHARD_LOG("(Execute) NodeId=%d, Server=%s:%d, StmtID=%d\n",
                  sShard->mNodeInfo[sNodeDbcIndex]->mNodeId,
                  sShard->mNodeInfo[sNodeDbcIndex]->mServerIP,
                  sShard->mNodeInfo[sNodeDbcIndex]->mPortNo,
                  sNodeStmt->mStatementID);

        if ( sNodeResult == SQL_SUCCESS )
        {
            /* BUG-46257 shardcli에서 Node 추가/제거 지원 */
            sResult = ulnStmtGetCurrentResult( sNodeStmt );
            aStmt->mShardStmtCxt.mRowCount += ulnResultGetAffectedRowCount( sResult );
        }
        else if ( sNodeResult == SQL_SUCCESS_WITH_INFO )
        {
            /* info의 전달 */
            ulsdNativeErrorToUlnError(aFnContext,
                                      SQL_HANDLE_STMT,
                                      (ulnObject *)sNodeStmt,
                                      sShard->mNodeInfo[sNodeDbcIndex],
                                      (acp_char_t *)"Execute");

            sSuccessResult = sNodeResult;

            /* BUG-46257 shardcli에서 Node 추가/제거 지원 */
            sResult = ulnStmtGetCurrentResult( sNodeStmt );
            aStmt->mShardStmtCxt.mRowCount += ulnResultGetAffectedRowCount( sResult );
        }
        else if ( sNodeResult == SQL_NO_DATA )
        {
            sNoDataCount++;
        }
        else
        {
            /* BUGBUG 여러개의 데이터 노드에 대해서 retry가 가능한가? */
            if ( ulsdNodeFailConnectionLost( SQL_HANDLE_STMT,
                                             (ulnObject *)sNodeStmt ) == ACP_TRUE )
            {
                if ( aStmt->mParentDbc->mAttrAutoCommit == SQL_AUTOCOMMIT_OFF )
                {
                    ulsdClearOnTransactionNode(aStmt->mParentDbc);
                }
                else
                {
                    /* Do Nothing */
                }
            }

            ulsdNativeErrorToUlnError(aFnContext,
                                      SQL_HANDLE_STMT,
                                      (ulnObject *)sNodeStmt,
                                      sShard->mNodeInfo[sNodeDbcIndex],
                                      (acp_char_t *)"Execute");

            sErrorResult = sNodeResult;
            sSuccess = ACP_FALSE;
        }
    }

    /* 모두 no_data이면 no_data 리턴, 에러가 하나라도 있으면 에러 리턴 */
    if ( sNoDataCount == aStmt->mShardStmtCxt.mNodeDbcIndexCount )
    {
        sSuccessResult = SQL_NO_DATA;
    }
    else
    {
        ACI_TEST_RAISE( sSuccess == ACP_FALSE, ERR_EXECUTE_FAIL );
    }

    /* execute가 성공하고, select인 경우 fetch할 준비가 됨 */
    aStmt->mShardStmtCxt.mNodeDbcIndexCur = 0;

    ulsdRemoveCallback( sCallback );

    return sSuccessResult;

    ACI_EXCEPTION( ERR_EXECUTE_FAIL )
    {
        ulsdProcessExecuteError( aFnContext, aStmt, sCallback );
    }
    ACI_EXCEPTION_END;

    ulsdRemoveCallback( sCallback );

    return sErrorResult;
}

SQLRETURN ulsdRowCount(ulnStmt *aStmt,
                       ulvSLen *aRowCountPtr)
{
    SQLRETURN     sNodeResult = SQL_ERROR;
    ulnFnContext  sFnContext;

    ULN_INIT_FUNCTION_CONTEXT(sFnContext, ULN_FID_ROWCOUNT, aStmt, ULN_OBJ_TYPE_STMT);

    /* clear diagnostic info */
    ACI_TEST(ulnClearDiagnosticInfoFromObject((ulnObject*)aStmt) != ACI_SUCCESS);

    sNodeResult = ulsdModuleRowCount(&sFnContext, aStmt, aRowCountPtr);

    return sNodeResult;

    ACI_EXCEPTION_END;

    return SQL_ERROR;
}

SQLRETURN ulsdRowCountNodes(ulnFnContext *aFnContext,
                            ulnStmt      *aStmt,
                            ulvSLen      *aRowCount)
{
    ACI_TEST_RAISE( aRowCount == NULL, LABEL_INVALID_NULL );

    /* BUG-46257 shardcli에서 Node 추가/제거 지원 */
    *aRowCount = (ulvSLen)aStmt->mShardStmtCxt.mRowCount;

    return SQL_SUCCESS;

    ACI_EXCEPTION( LABEL_INVALID_NULL )
    {
        ulnError( aFnContext, ulERR_ABORT_INVALID_USE_OF_NULL_POINTER );
    }
    ACI_EXCEPTION_END;

    return ULN_FNCONTEXT_GET_RC( aFnContext );
}

SQLRETURN ulsdMoreResults(ulnStmt *aStmt)
{
    SQLRETURN      sNodeResult = SQL_ERROR;
    ulnFnContext   sFnContext;
    ulnFnContext   sFnContextSMNUpdate;

    ULN_INIT_FUNCTION_CONTEXT(sFnContext, ULN_FID_MORERESULTS, aStmt, ULN_OBJ_TYPE_STMT);

    /* clear diagnostic info */
    ACI_TEST(ulnClearDiagnosticInfoFromObject((ulnObject*)aStmt) != ACI_SUCCESS);

    sNodeResult = ulsdModuleMoreResults(&sFnContext, aStmt);

    /* BUG-46100 Session SMN Update */
    if ( ( aStmt->mParentDbc->mAttrAutoCommit == SQL_AUTOCOMMIT_ON ) &&
         ( ulsdIsTimeToUpdateShardMetaNumber( aStmt->mParentDbc ) == ACP_TRUE ) )
    {
        if ( sNodeResult != SQL_SUCCESS )
        {
            /* ulsdModuleMoreResults is fail or no data. */
            ULN_INIT_FUNCTION_CONTEXT( sFnContextSMNUpdate, ULN_FID_MORERESULTS, aStmt, ULN_OBJ_TYPE_STMT );

            ACI_TEST( ulsdUpdateShardMetaNumber( aStmt->mParentDbc, & sFnContextSMNUpdate ) != SQL_SUCCESS );

            if ( sNodeResult != SQL_NO_DATA )
            {
                ulnError( & sFnContext, ulERR_ABORT_SHARD_META_CHANGED );
            }
        }
    }
    else
    {
        /* Nothing to do */
    }

    return sNodeResult;

    ACI_EXCEPTION_END;

    return SQL_ERROR;
}

SQLRETURN ulsdMoreResultsNodes(ulnFnContext *aFnContext,
                               ulnStmt      *aStmt)
{
    SQLRETURN          sNodeResult = SQL_ERROR;
    SQLRETURN          sSuccessResult = SQL_SUCCESS;
    SQLRETURN          sErrorResult = SQL_ERROR;
    ulsdDbc           *sShard;
    ulnStmt           *sNodeStmt;
    acp_uint16_t       sNodeDbcIndex;
    acp_uint16_t       sNoDataCount = 0;
    acp_uint16_t       i;

    ulsdGetShardFromDbc(aStmt->mParentDbc, &sShard);

    for ( i = 0; i < aStmt->mShardStmtCxt.mNodeDbcIndexCount; i++ )
    {
        sNodeDbcIndex = aStmt->mShardStmtCxt.mNodeDbcIndexArr[i];
        sNodeStmt = aStmt->mShardStmtCxt.mShardNodeStmt[sNodeDbcIndex];

        sNodeResult = ulnMoreResults(sNodeStmt);

        SHARD_LOG("(More Results) NodeId=%d, Server=%s:%d, StmtID=%d\n",
                  sShard->mNodeInfo[sNodeDbcIndex]->mNodeId,
                  sShard->mNodeInfo[sNodeDbcIndex]->mServerIP,
                  sShard->mNodeInfo[sNodeDbcIndex]->mPortNo,
                  sNodeStmt->mStatementID);

        if ( sNodeResult == SQL_SUCCESS )
        {
            /* Nothing to do */
        }
        else if ( sNodeResult == SQL_NO_DATA )
        {
            sNoDataCount++;
        }
        else
        {
            sErrorResult = sNodeResult;

            ACI_RAISE(LABEL_NODE_MORERESULTS_FAIL);
        }
    }

    /* 모두 no_data이면 no_data 리턴 */
    if ( sNoDataCount == aStmt->mShardStmtCxt.mNodeDbcIndexCount )
    {
        sSuccessResult = SQL_NO_DATA;
    }
    else
    {
        /* Nothing to do */
    }

    return sSuccessResult;

    ACI_EXCEPTION(LABEL_NODE_MORERESULTS_FAIL)
    {
        ulsdNativeErrorToUlnError(aFnContext,
                                  SQL_HANDLE_STMT,
                                  (ulnObject *)sNodeStmt,
                                  sShard->mNodeInfo[sNodeDbcIndex],
                                  (acp_char_t *)"MoreResults");
    }
    ACI_EXCEPTION_END;

    return sErrorResult;
}

acp_bool_t ulsdStmtHasNoDataOnNodes( ulnStmt * aMetaStmt )
{
    ulnStmt      * sNodeStmt     = NULL;
    acp_uint16_t   sNodeDbcIndex = 0;
    acp_uint16_t   i             = 0;

    for ( i = 0; i < aMetaStmt->mShardStmtCxt.mNodeDbcIndexCount; i++ )
    {
        sNodeDbcIndex = aMetaStmt->mShardStmtCxt.mNodeDbcIndexArr[i];
        sNodeStmt = aMetaStmt->mShardStmtCxt.mShardNodeStmt[sNodeDbcIndex];

        if ( ulnStmtIsPrepared( sNodeStmt ) == ACP_TRUE )
        {
            ACI_TEST( ulnCursorHasNoData( ulnStmtGetCursor( sNodeStmt ) ) == ACP_FALSE );
        }
        else
        {
            /* Nothing to do */
        }
    }

    return ACP_TRUE;

    ACI_EXCEPTION_END;

    return ACP_FALSE;
}
