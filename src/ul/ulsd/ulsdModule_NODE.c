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

#include <ulsd.h>
#include <ulsdFailover.h>
#include <ulsdnFailover.h>

ACI_RC ulsdModuleHandshake_NODE(ulnFnContext *aFnContext)
{
    ACP_UNUSED(aFnContext);

    return ACI_SUCCESS;
}

SQLRETURN ulsdModuleNodeDriverConnect_NODE(ulnDbc       *aDbc,
                                           ulnFnContext *aFnContext,
                                           acp_char_t   *aConnString,
                                           acp_sint16_t  aConnStringLength)
{
    ACP_UNUSED(aDbc);
    ACP_UNUSED(aFnContext);
    ACP_UNUSED(aConnString);
    ACP_UNUSED(aConnStringLength);

    return SQL_SUCCESS;
}

ACI_RC ulsdModuleEnvRemoveDbc_NODE(ulnEnv *aEnv, ulnDbc *aDbc)
{
    ACP_UNUSED(aEnv);
    ACP_UNUSED(aDbc);

    return ACI_SUCCESS;
}

void ulsdModuleStmtDestroy_NODE(ulnStmt *aStmt)
{
    ACP_UNUSED(aStmt);
}

SQLRETURN ulsdModulePrepare_NODE(ulnFnContext *aFnContext,
                                 ulnStmt      *aStmt,
                                 acp_char_t   *aStatementText,
                                 acp_sint32_t  aTextLength,
                                 acp_char_t   *aAnalyzeText)
{
    ACP_UNUSED(aFnContext);

    return ulnPrepare(aStmt,
                      aStatementText,
                      aTextLength,
                      aAnalyzeText);
}

SQLRETURN ulsdModuleExecute_NODE(ulnFnContext *aFnContext,
                                 ulnStmt      *aStmt)
{
    ACP_UNUSED(aFnContext);

    return ulnExecute(aStmt);
}

SQLRETURN ulsdModuleFetch_NODE(ulnFnContext *aFnContext,
                               ulnStmt      *aStmt)
{
    ACP_UNUSED(aFnContext);

    return ulnFetch(aStmt);
}

SQLRETURN ulsdModuleCloseCursor_NODE(ulnStmt *aStmt)
{
    return ulnCloseCursor(aStmt);
}

SQLRETURN ulsdModuleRowCount_NODE(ulnFnContext *aFnContext,
                                  ulnStmt      *aStmt,
                                  acp_sint32_t *aRowCount)
{
    ACP_UNUSED(aFnContext);

    return ulnRowCount(aStmt, aRowCount);
}

SQLRETURN ulsdModuleMoreResults_NODE(ulnFnContext *aFnContext,
                                     ulnStmt      *aStmt)
{
    ACP_UNUSED(aFnContext);

    return ulnMoreResults(aStmt);
}

ulnStmt* ulsdModuleGetPreparedStmt_NODE(ulnStmt *aStmt)
{
    ulnStmt  *sStmt = NULL;

    if ( ulnStmtIsPrepared(aStmt) == ACP_TRUE )
    {
        sStmt = aStmt;
    }
    else
    {
        /* Nothing to do */
    }

    return sStmt;
}

void ulsdModuleOnCmError_NODE(ulnFnContext     *aFnContext,
                              ulnDbc           *aDbc,
                              ulnErrorMgr      *aErrorMgr)
{
    ACI_RC sRet = ACI_FAILURE;

    if ( ulnFailoverIsOn( aDbc ) == ACP_TRUE )
    {
        sRet = ulsdFODoSTF(aFnContext, aDbc, aErrorMgr);
    }
    else if ( ulnDbcGetSessionFailover( aDbc ) == ACP_TRUE )
    {
        sRet = ulsdFODoReconnect( aFnContext, aDbc, aErrorMgr );
    }
    else
    {
        /* sRet == ACI_FAILURE */
    }

    if ( sRet != ACI_SUCCESS )
    {
        ulsdnRaiseShardNodeFailoverIsNotAvailableError( aFnContext );
    }
}

ACI_RC ulsdModuleUpdateNodeList_NODE(ulnFnContext  *aFnContext,
                                     ulnDbc        *aDbc)
{
    ACP_UNUSED(aFnContext);
    ACP_UNUSED(aDbc);

    return ACI_SUCCESS;
}

ACI_RC ulsdModuleNotifyFailOver_NODE( ulnDbc *aDbc )
{
    ulnDbc                   * sMetaDbc = aDbc->mShardDbcCxt.mParentDbc;
    ulsdNodeReport             sReport;
    ulnFnContext               sMetaFnContext;

    ULN_INIT_FUNCTION_CONTEXT( sMetaFnContext, ULN_FID_NONE, sMetaDbc, ULN_OBJ_TYPE_DBC );

    ulsdGetNodeConnectionReport( aDbc, &sReport );

    ACI_TEST( ulsdSendNodeConnectionReport( &sMetaFnContext, &sReport ) != ACI_SUCCESS );

    return ACI_SUCCESS;

    ACI_EXCEPTION_END;

    return ACI_FAILURE;
}

void ulsdModuleAlignDataNodeConnection_NODE( ulnFnContext * aFnContext,
                                             ulnDbc       * aNodeDbc )
{
    ACP_UNUSED( aFnContext       );
    ACP_UNUSED( aNodeDbc         );
}

void ulsdModuleErrorCheckAndAlignDataNode_NODE( ulnFnContext * aFnContext )
{
    ACP_UNUSED( aFnContext       );
}

acp_bool_t ulsdModuleHasNoData_NODE( ulnStmt * aStmt )
{
    return ulnCursorHasNoData( ulnStmtGetCursor( aStmt ) );
}

ulsdModule gShardModuleNODE =
{
    ulsdModuleHandshake_NODE,
    ulsdModuleNodeDriverConnect_NODE,
    ulsdModuleEnvRemoveDbc_NODE,
    ulsdModuleStmtDestroy_NODE,
    ulsdModulePrepare_NODE,
    ulsdModuleExecute_NODE,
    ulsdModuleFetch_NODE,
    ulsdModuleCloseCursor_NODE,
    ulsdModuleRowCount_NODE,
    ulsdModuleMoreResults_NODE,
    ulsdModuleGetPreparedStmt_NODE,
    ulsdModuleOnCmError_NODE,
    ulsdModuleUpdateNodeList_NODE,
    ulsdModuleNotifyFailOver_NODE,
    ulsdModuleAlignDataNodeConnection_NODE,
    ulsdModuleErrorCheckAndAlignDataNode_NODE,
    ulsdModuleHasNoData_NODE
};
