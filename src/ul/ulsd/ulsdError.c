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
#include <sdErrorCodeClient.h>

void ulsdMoveNodeDiagRec( ulnFnContext * aFnContext,
                          ulnObject    * aObjectTo,
                          ulnObject    * aObjectFrom,
                          acp_char_t   * aNodeString,
                          acp_char_t   * aOperation )
{
    ulnDiagRec      *sDiagRecFrom;
    ulnDiagRec      *sDiagRecTo;
    acp_char_t       sErrorMessage[ULSD_MAX_ERROR_MESSAGE_LEN];

    while ( ulnGetDiagRecFromObject( aObjectFrom,
                                     &sDiagRecFrom,
                                     1 )
            == ACI_SUCCESS )
    {
        ulnDiagRecCreate( &(aObjectTo->mDiagHeader), &sDiagRecTo );

        ulsdMakeErrorMessage( sErrorMessage,
                              ULSD_MAX_ERROR_MESSAGE_LEN,
                              sDiagRecFrom->mMessageText,
                              aNodeString,
                              aOperation );

        SHARD_LOG( "Error:(%s), Message:%s\n", aOperation, sErrorMessage );

        ulnDiagRecSetMessageText( sDiagRecTo, sErrorMessage );
        ulnDiagRecSetSqlState( sDiagRecTo, sDiagRecFrom->mSQLSTATE );
        ulnDiagRecSetNativeErrorCode( sDiagRecTo, sDiagRecFrom->mNativeErrorCode );
        ulnDiagRecSetRowNumber( sDiagRecTo, sDiagRecFrom->mRowNumber );
        ulnDiagRecSetColumnNumber( sDiagRecTo, sDiagRecFrom->mColumnNumber );

        ulnDiagHeaderAddDiagRec( sDiagRecTo->mHeader, sDiagRecTo );
        ULN_FNCONTEXT_SET_RC( aFnContext,
                              ulnErrorDecideSqlReturnCode( sDiagRecTo->mSQLSTATE ) );

        ulnDiagHeaderRemoveDiagRec( sDiagRecFrom->mHeader, sDiagRecFrom );
        (void)ulnDiagRecDestroy( sDiagRecFrom );
    }

    (void)ulnClearDiagnosticInfoFromObject( aObjectFrom );
}

void ulsdNativeErrorToUlnError( ulnFnContext       * aFnContext,
                                acp_sint16_t         aHandleType,
                                ulnObject          * aErrorRaiseObject,
                                ulsdNodeInfo       * aNodeInfo,
                                acp_char_t         * aOperation)
{
    ulnFnContext           sFnContext;
    ulnObject             *sObject          = NULL;
    ulnDbc                *sDbc             = NULL;
    ulnFailoverServerInfo *sServerInfo      = NULL;
    const acp_char_t      *sNodeName        = NULL;
    const acp_char_t      *sServerIP        = NULL;
    acp_uint16_t           sServerPort      = 0;
    acp_char_t             sNodeString[ULSD_MAX_ERROR_MESSAGE_LEN];

    sObject = aErrorRaiseObject;

    ACI_TEST_RAISE( sObject == NULL, InvalidHandle );

    ULN_INIT_FUNCTION_CONTEXT( sFnContext, ULN_FID_NONE, sObject, aHandleType );

    ULN_FNCONTEXT_GET_DBC( &sFnContext, sDbc );

    ACI_TEST_RAISE( sDbc == NULL, InvalidHandle );
    ACI_TEST_RAISE( sDbc->mShardDbcCxt.mParentDbc == NULL, InvalidHandle );

    if ( aNodeInfo != NULL )
    {
        sNodeName = aNodeInfo->mNodeName;
    }
    else
    {
        sNodeName = "NODE_UNKNOWN";
    }

    sServerInfo = ulnDbcGetCurrentServer( sDbc );
    if ( sServerInfo != NULL )
    {
        sServerIP   = sServerInfo->mHost;
        sServerPort = sServerInfo->mPort;
    }
    else
    {
        /* sServerInfo is null -> there is only active server */

        if ( aNodeInfo != NULL )
        {
            sServerIP   = aNodeInfo->mServerIP;
            sServerPort = aNodeInfo->mPortNo;
        }
        else
        {
            sServerIP   = "UNKNOWN";
            sServerPort = 0;
        }
    }

    acpSnprintf( sNodeString,
                 ACI_SIZEOF(sNodeString),
                 "%s,%s:%"ACI_INT32_FMT,
                 sNodeName,
                 sServerIP,
                 sServerPort );

    ulsdMoveNodeDiagRec( aFnContext,
                         aFnContext->mHandle.mObj,
                         sObject,
                         sNodeString,
                         aOperation );

    return;

    ACI_EXCEPTION( InvalidHandle );
    ACI_EXCEPTION_END;

    return;
}

void ulsdMakeErrorMessage( acp_char_t         * aOutputErrorMessage,
                           acp_uint16_t         aOutputErrorMessageLength,
                           acp_char_t         * aOriginalErrorMessage,
                           acp_char_t         * aNodeString,
                           acp_char_t         * aOperation )
{
    *aOutputErrorMessage = '\0';

    acpSnprintf( aOutputErrorMessage,
                 aOutputErrorMessageLength,
                 (acp_char_t *)"The %s of client-side sharding failed.: \"%s\" [%s]",
                 aOperation,
                 aOriginalErrorMessage,
                 aNodeString );
}

acp_bool_t ulsdNodeFailConnectionLost( acp_sint16_t   aHandleType,
                                       ulnObject    * aObject )
{
    acp_sint16_t        sRecNumber;
    acp_char_t          sSqlstate[SQL_SQLSTATE_SIZE + 1];
    acp_sint32_t        sNativeError;
    acp_char_t          sMessage[ULSD_MAX_ERROR_MESSAGE_LEN];
    acp_sint16_t        sMessageLength;

    acp_bool_t          sIsConnectionLost = ACP_FALSE;

    sRecNumber = 1;

    while ( ulnGetDiagRec(aHandleType,
                          aObject,
                          sRecNumber,
                          sSqlstate,
                          &sNativeError,
                          sMessage,
                          ACI_SIZEOF(sMessage),
                          &sMessageLength,
                          ACP_FALSE) != SQL_NO_DATA )
    {
        if ( ( sNativeError == ALTIBASE_FAILOVER_SUCCESS ) ||
             ( sNativeError == ALTIBASE_SHARD_NODE_FAILOVER_IS_NOT_AVAILABLE ) )
        {
            sIsConnectionLost = ACP_TRUE;
            break;
        }
        else
        {
            /* Do Nothing */
        }

        sRecNumber++;
    }

    return sIsConnectionLost;
}

acp_bool_t ulsdNodeInvalidTouch( acp_sint16_t   aHandleType,
                                 ulnObject    * aObject )
{
    acp_sint16_t        sRecNumber;
    acp_char_t          sSqlstate[6];
    acp_sint32_t        sNativeError;
    acp_char_t          sMessage[ULSD_MAX_ERROR_MESSAGE_LEN];
    acp_sint16_t        sMessageLength;

    acp_bool_t          sShardNodeInvalidTouchError = ACP_FALSE;

    sRecNumber = 1;

    while ( ulnGetDiagRec(aHandleType,
                          aObject,
                          sRecNumber,
                          sSqlstate,
                          &sNativeError,
                          sMessage,
                          sizeof(sMessage),
                          &sMessageLength,
                          ACP_FALSE) != SQL_NO_DATA )
    {
        if ( sNativeError == ALTIBASE_SHARD_NODE_INVALID_TOUCH )
        {
            sShardNodeInvalidTouchError = ACP_TRUE;
            break;
        }
        else
        {
            /* Do Nothing */
        }

        sRecNumber++;
    }

    return sShardNodeInvalidTouchError;
}

static acp_bool_t ulsdIsFailoverErrorCode( acp_uint32_t aNativeErrorCode )
{
    acp_bool_t sIsFailoverErrorCode = ACP_FALSE;

    switch ( aNativeErrorCode )
    {
        case ACI_E_ERROR_CODE( sdERR_ABORT_SHARD_LIBRARY_LINK_FAILURE_ERROR ) :
        case ACI_E_ERROR_CODE( sdERR_ABORT_SHARD_LIBRARY_FAILOVER_SUCCESS )   :
        case ACI_E_ERROR_CODE( sdERR_ABORT_SHARD_NODE_FAILOVER_IS_NOT_AVAILABLE ) :
            sIsFailoverErrorCode = ACP_TRUE;
            break;

        default:
            /* Do nothing */
            break;
    }

    return sIsFailoverErrorCode;
}

static acp_bool_t ulsdProcessShardingError( ulnFnContext * aFnContext,
                                            ulnDiagRec   * aDiagRec,
                                            acp_uint32_t   aNativeErrorCode,
                                            acp_uint32_t   aNodeId )
{
    ulnDbc                * sMetaDbc           = NULL;
    ulnDbc                * sNodeDbc           = NULL;
    ulsdDbc               * sShard             = NULL;
    ulsdAlignInfo         * sAlignInfo         = NULL;
    acp_sint32_t            sIdx;

    ULN_FNCONTEXT_GET_DBC( aFnContext, sMetaDbc );

    ACI_TEST_RAISE( sMetaDbc == NULL, InvalidHandleException );

    sShard = sMetaDbc->mShardDbcCxt.mShardDbc;

    if ( aNativeErrorCode == ACI_E_ERROR_CODE( sdERR_ABORT_SHARD_LIBRARY_LINK_FAILURE_ERROR ) )
    {
        for ( sIdx = 0; sIdx < sShard->mNodeCount; ++sIdx )
        {
            if ( sShard->mNodeInfo[sIdx]->mNodeId == aNodeId )
            {
                sNodeDbc = sShard->mNodeInfo[sIdx]->mNodeDbc;
                break;
            }
        }
        ACI_TEST_RAISE( sNodeDbc == NULL, NOT_FOUND_DATA_NODE );
        ACI_TEST_RAISE( ulnFailoverIsOn( sNodeDbc ) == ACP_FALSE, NOT_SUPPORTED_DATA_NODE_ALIGN );

        sAlignInfo = &sNodeDbc->mShardDbcCxt.mAlignInfo;

        ACI_TEST( ulsdReallocAlignInfo( aFnContext,
                                        sAlignInfo,
                                        acpCStrLen( aDiagRec->mMessageText,
                                                    ULSD_ALIGN_INFO_MAX_TEXT_LENGTH ) + 1 )
                  != ACP_RC_SUCCESS );

        acpSnprintf( sAlignInfo->mSQLSTATE, SQL_SQLSTATE_SIZE + 1, aDiagRec->mSQLSTATE );
        acpSnprintf( sAlignInfo->mMessageText,
                     sAlignInfo->mMessageTextAllocLength,
                     "%s",
                     aDiagRec->mMessageText );
        sAlignInfo->mNativeErrorCode = aNativeErrorCode;
        sAlignInfo->mIsNeedAlign     = ACP_TRUE;

        /* rest of data node align is will proceed at ulsdModuleErrorCheckAndAlignDataNode() */
    }
    else if ( aNativeErrorCode == ACI_E_ERROR_CODE( sdERR_ABORT_SHARD_LIBRARY_FAILOVER_SUCCESS ) )
    {
        /* server reconnect to data node by sdl::retryConnect */
        (void)ulnError( aFnContext,
                        ulERR_ABORT_FAILOVER_SUCCESS,
                        aNativeErrorCode,
                        aDiagRec->mSQLSTATE,
                        aDiagRec->mMessageText );
    }
    else if ( aNativeErrorCode == ACI_E_ERROR_CODE( sdERR_ABORT_SHARD_NODE_FAILOVER_IS_NOT_AVAILABLE ) )
    {
        (void)ulnError( aFnContext, ulERR_ABORT_SHARD_NODE_FAILOVER_IS_NOT_AVAILABLE );
    }
    else
    {
        ACI_RAISE( NOT_SURPPORT );
    }

    ULN_TRACE_LOG( aFnContext, ULN_TRACELOG_LOW, NULL, 0,
                   "%-18s|", "ulsdProcessShardingError" );

    return ACP_TRUE;

    ACI_EXCEPTION( InvalidHandleException )
    {
        ULN_TRACE_LOG( aFnContext, ULN_TRACELOG_LOW, NULL, 0,
                       "%-18s| Meta Dbc is invalid handle.",
                       "ulsdProcessShardingError" );
    }
    ACI_EXCEPTION( NOT_FOUND_DATA_NODE )
    {
        ULN_TRACE_LOG( aFnContext, ULN_TRACELOG_LOW, NULL, 0,
                       "%-18s| Data node ID %"ACI_INT32_FMT" not found.",
                       "ulsdProcessShardingError",
                       aNodeId );
    }
    ACI_EXCEPTION( NOT_SURPPORT );
    {
        /* Internal error.
         * ulnCallbackErrorResult 함수에서 이미 추가된 diag record 가 있으므로
         * 여기서 diag record 를 추가하지 않아도 무관하다.
         */
    }
    ACI_EXCEPTION( NOT_SUPPORTED_DATA_NODE_ALIGN );
    {
        (void)ulnError( aFnContext, ulERR_ABORT_SHARD_NODE_FAILOVER_IS_NOT_AVAILABLE );
    }
    ACI_EXCEPTION_END;

    return ACP_FALSE;
}

void ulsdErrorHandleShardingError( ulnFnContext * aFnContext )
{
    ulnDbc              * sMetaDbc       = NULL;
    acp_char_t          * sErrPos        = NULL;
    acp_char_t          * sErrNextPos    = NULL;
    acp_char_t          * sEndMarkPos    = NULL;
    acp_char_t          * sBlankPos      = NULL;
    acp_sint32_t          sFoundIdx      = 0;
    acp_sint32_t          sFromIdx       = 0;
    acp_sint32_t          sSign          = 0;
    acp_char_t          * sEnd           = NULL;
    const acp_char_t    * sStartMark     = "[<<";
    const acp_char_t    * sEndMark       = ">>]";
    const acp_char_t    * sNodeIdMark    = "NODE-ID:";
    const acp_sint32_t    sErrMarkLen    = 4; /* "ERC-" */
    const acp_sint32_t    sUnitMarkLen   = 3;
    const acp_sint32_t    sNodeIdMarkLen = 8; /* "NODE-ID:" */

    ulnDiagRec          * sDiagRec         = NULL;
    acp_uint32_t          sNativeErrorCode = 0;
    acp_uint32_t          sNodeId          = 0;
    

    ACI_TEST( aFnContext->mHandle.mObj == NULL );

    ULN_FNCONTEXT_GET_DBC( aFnContext, sMetaDbc );

    ACI_TEST( sMetaDbc == NULL );

    ACI_TEST( sMetaDbc->mShardDbcCxt.mParentDbc != NULL ); /* Meta dbc has not mParentDbc */

    ACI_TEST( ulnGetDiagRecFromObject( aFnContext->mHandle.mObj,
                                       &sDiagRec,
                                       1 )
              != ACI_SUCCESS );

    sErrPos = sDiagRec->mMessageText;

    while ( acpCStrFindCStr( sErrPos,
                             sStartMark,
                             &sFoundIdx,
                             sFromIdx,
                             0 )
            == ACP_RC_SUCCESS )
    {
        sErrPos = &sErrPos[sFoundIdx + sUnitMarkLen];

        ACI_TEST ( acpCStrFindCStr( sErrPos,
                                    sEndMark,
                                    &sFoundIdx,
                                    sFromIdx,
                                    0 )
                   != ACP_RC_SUCCESS );

        sEndMarkPos = &sErrPos[sFoundIdx];
        sErrNextPos = &sErrPos[sFoundIdx + sUnitMarkLen];
        
        ACI_TEST( acpCStrCmp( sErrPos,
                              "ERC-",
                              sErrMarkLen )
                  != 0 );

        sErrPos += sErrMarkLen;
        /* [<<ERC-NNNNN NODE-ID:NNN>>]
         *        ^
         */

        ACI_TEST( acpCStrFindCStr( sErrPos,
                                   " ",
                                   &sFoundIdx,
                                   sFromIdx,
                                   0 )
                  != ACP_RC_SUCCESS );

        sBlankPos = &sErrPos[sFoundIdx];

        ACI_TEST( acpCStrToInt32( sErrPos, 
                                  sBlankPos - sErrPos,
                                  &sSign,
                                  &sNativeErrorCode,
                                  16,
                                  &sEnd ) 
                  != ACP_RC_SUCCESS );
        
        ACI_TEST( sEnd != sBlankPos );

        sErrPos = sBlankPos + 1;
        /* [<<ERC-NNNNN NODE-ID:NNN>>]
         *              ^
         */

        ACI_TEST( acpCStrCmp( sErrPos,
                              sNodeIdMark,
                              sNodeIdMarkLen )
                  != 0 );

        sErrPos += sNodeIdMarkLen;
        /* [<<ERC-NNNNN NODE-ID:NNN>>]
         *                      ^
         */

        ACI_TEST( acpCStrToInt32( sErrPos, 
                                  sEndMarkPos - sErrPos,
                                  &sSign,
                                  &sNodeId,
                                  10,
                                  &sEnd ) 
                  != ACP_RC_SUCCESS );
        
        ACI_TEST( sEnd != sEndMarkPos );

        sErrPos = sErrNextPos;

        if ( ulsdIsFailoverErrorCode( sNativeErrorCode ) == ACP_TRUE )
        {
            (void)ulsdProcessShardingError( aFnContext,
                                            sDiagRec,
                                            sNativeErrorCode,
                                            sNodeId );
        }
    }

    return;

    ACI_EXCEPTION_END;

    return;
}

void ulsdErrorCheckAndAlignDataNode( ulnFnContext * aFnContext )
{
    ulnDbc                * sMetaDbc           = NULL;
    ulnDbc                * sNodeDbc           = NULL;
    ulsdDbc               * sShard             = NULL;
    ulsdAlignInfo         * sAlignInfo         = NULL;
    acp_sint32_t            sIdx;

    ULN_FNCONTEXT_GET_DBC( aFnContext, sMetaDbc );

    if ( sMetaDbc != NULL )
    {
        sShard = sMetaDbc->mShardDbcCxt.mShardDbc;

        for ( sIdx = 0; sIdx < sShard->mNodeCount; ++sIdx )
        {
            sNodeDbc = sShard->mNodeInfo[sIdx]->mNodeDbc;
            sAlignInfo = &sNodeDbc->mShardDbcCxt.mAlignInfo;

            if ( sAlignInfo->mIsNeedAlign == ACP_FALSE )
            {
                continue;
            }

            ulsdModuleAlignDataNodeConnection( aFnContext, sNodeDbc );
        }
    }
}
