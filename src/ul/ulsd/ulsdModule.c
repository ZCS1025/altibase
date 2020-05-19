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
#include <mtcc.h>

ACI_RC ulsdModuleHandshake(ulnFnContext *aFnContext)
{
    return aFnContext->mHandle.mDbc->mShardModule->ulsdModuleHandshake(aFnContext);
}

SQLRETURN ulsdModuleNodeDriverConnect(ulnDbc       *aDbc,
                                      ulnFnContext *aFnContext,
                                      acp_char_t   *aConnString,
                                      acp_sint16_t  aConnStringLength)
{
    return aDbc->mShardModule->ulsdModuleNodeDriverConnect(aDbc,
                                                           aFnContext,
                                                           aConnString,
                                                           aConnStringLength);
}

ACI_RC ulsdModuleEnvRemoveDbc(ulnEnv *aEnv, ulnDbc *aDbc)
{
    return aDbc->mShardModule->ulsdModuleEnvRemoveDbc(aEnv, aDbc);
}

void ulsdModuleStmtDestroy(ulnStmt *aStmt)
{
    aStmt->mShardModule->ulsdModuleStmtDestroy(aStmt);
}

SQLRETURN ulsdModulePrepare(ulnFnContext *aFnContext,
                            ulnStmt      *aStmt,
                            acp_char_t   *aStatementText,
                            acp_sint32_t  aTextLength,
                            acp_char_t   *aAnalyzeText)
{
    return aStmt->mShardModule->ulsdModulePrepare(aFnContext,
                                                  aStmt,
                                                  aStatementText,
                                                  aTextLength,
                                                  aAnalyzeText);
}

SQLRETURN ulsdModuleExecute(ulnFnContext *aFnContext,
                            ulnStmt      *aStmt)
{
    return aStmt->mShardModule->ulsdModuleExecute(aFnContext, aStmt);
}

SQLRETURN ulsdModuleFetch(ulnFnContext *aFnContext,
                          ulnStmt      *aStmt)
{
    return aStmt->mShardModule->ulsdModuleFetch(aFnContext, aStmt);
}

SQLRETURN ulsdModuleCloseCursor(ulnStmt *aStmt)
{
    return aStmt->mShardModule->ulsdModuleCloseCursor(aStmt);
}

SQLRETURN ulsdModuleRowCount(ulnFnContext *aFnContext,
                             ulnStmt      *aStmt,
                             ulvSLen      *aRowCount)
{
    return aStmt->mShardModule->ulsdModuleRowCount(aFnContext, aStmt, aRowCount);
}

SQLRETURN ulsdModuleMoreResults(ulnFnContext *aFnContext,
                                ulnStmt      *aStmt)
{
    return aStmt->mShardModule->ulsdModuleMoreResults(aFnContext, aStmt);
}

ulnStmt* ulsdModuleGetPreparedStmt(ulnStmt *aStmt)
{
    return aStmt->mShardModule->ulsdModuleGetPreparedStmt(aStmt);
}

void ulsdModuleOnCmError(ulnFnContext     *aFnContext,
                         ulnDbc           *aDbc,
                         ulnErrorMgr      *aErrorMgr)
{
    aDbc->mShardModule->ulsdModuleOnCmError(aFnContext, aDbc, aErrorMgr);
}

ACI_RC ulsdModuleUpdateNodeList(ulnFnContext  *aFnContext,
                                ulnDbc        *aDbc)
{
    return aDbc->mShardModule->ulsdModuleUpdateNodeList(aFnContext, aDbc);
}

ACI_RC ulsdModuleNotifyFailOver( ulnDbc *aDbc )
{
    return aDbc->mShardModule->ulsdModuleNotifyFailOver( aDbc );
}

void ulsdModuleAlignDataNodeConnection( ulnFnContext * aFnContext,
                                        ulnDbc       * aNodeDbc )
{
    ulnDbc * sDbc = NULL;

    ULN_FNCONTEXT_GET_DBC( aFnContext, sDbc );

    ACE_DASSERT( sDbc != NULL );

    sDbc->mShardModule->ulsdModuleAlignDataNodeConnection( aFnContext,
                                                           aNodeDbc );
    return;
}

void ulsdModuleErrorCheckAndAlignDataNode( ulnFnContext * aFnContext )
{
    ulnDbc * sDbc = NULL;

    ULN_FNCONTEXT_GET_DBC( aFnContext, sDbc );

    ACE_DASSERT( sDbc != NULL );

    sDbc->mShardModule->ulsdModuleErrorCheckAndAlignDataNode( aFnContext );

    return;
}


acp_bool_t ulsdModuleHasNoData( ulnStmt * aStmt )
{
    return aStmt->mShardModule->ulsdModuleHasNoData( aStmt );
}
