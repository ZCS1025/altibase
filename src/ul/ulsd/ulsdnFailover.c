/** 
 *  Copyright (c) 1999~2018, Altibase Corp. and/or its affiliates. All rights reserved.
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
 

/**********************************************************************
 * $Id$
 **********************************************************************/
#include <uln.h>
#include <ulnPrivate.h>
#include <ulsdnFailover.h>
#include <ulnConnectCore.h>
#include <ulsdModule.h>
#include <ulsdError.h>
#include <mmErrorCodeClient.h>


SQLRETURN ulsdnGetFailoverIsNeeded( acp_sint16_t   aHandleType,
                                    ulnObject     *aObject,
                                    acp_sint32_t  *aIsNeed )
{
    ulnFnContext  sFnContext;
    ulnDbc       *sDbc           = NULL;

    ULN_INIT_FUNCTION_CONTEXT( sFnContext, ULN_FID_NONE, aObject, aHandleType );

    ACI_TEST_RAISE( aObject == NULL, InvalidHandle );

    ULN_FNCONTEXT_GET_DBC( &sFnContext, sDbc );

    ACI_TEST_RAISE( sDbc == NULL, InvalidHandle );

    /*
     * ======================================
     * Function BEGIN
     * ======================================
     */
    *aIsNeed = 0;

    if ( ulnDiagRecIsNeedFailover( aObject ) == ACP_TRUE )
    {
        *aIsNeed = 1;
    }

    /*
     * ======================================
     * Function END
     * ======================================
     */

    return ULN_FNCONTEXT_GET_RC( &sFnContext );

    ACI_EXCEPTION( InvalidHandle )
    {
        ULN_FNCONTEXT_SET_RC( &sFnContext, SQL_INVALID_HANDLE );
    }

    ACI_EXCEPTION_END;

    *aIsNeed = 0;

    return ULN_FNCONTEXT_GET_RC( &sFnContext );
}

/* Call from
 *  1. SQLReconnect()
 *     -> ulsdnReconnect()
 *      - Server-side retry connection.
 *  2. ulsdErrorHandleShardingError()
 *     -> ulsdModuleAlignDataNodeConnection()
 *     -> ulsdModuleAlignDataNodeConnection_COORD/META()
 *     -> ulsdAlignDataNodeConnection()
 *      - Server-side failover and align data node connection.
 */
ACI_RC ulsdnFailoverConnectToSpecificServer( ulnFnContext *aFnContext, ulnFailoverServerInfo *aNewServerInfo )
{
    ulnDbc             * sDbc    = NULL;
    ACI_RC               sRC     = ACI_FAILURE;
    ulsdFailoverResult   sResult = ULSDN_FAILOVER_FAILURE;

    ULN_FNCONTEXT_GET_DBC( aFnContext, sDbc );
    ACI_TEST_RAISE( sDbc == NULL, INVALID_HANDLE_EXCEPTION );

    ACI_TEST( ( ulnDbcGetConnType( sDbc ) != ULN_CONNTYPE_TCP     ) &&
              ( ulnDbcGetConnType( sDbc ) != ULN_CONNTYPE_IB      ) && /* PROJ-2681 */
              ( ulnDbcGetConnType( sDbc ) != ULN_CONNTYPE_SSL     ) );

    ULN_TRACE_LOG( aFnContext, ULN_TRACELOG_LOW, NULL, 0,
                   "%-18s| Server: %s:%d",
                   "ulsdnFailoverConnectToSpecificServer",
                   aNewServerInfo->mHost,
                   aNewServerInfo->mPort );

    sRC = ulsdnDoFailoverAvailable( aFnContext, &sResult );
    if ( sRC == ACI_SUCCESS )
    {
        ACI_TEST_CONT( sResult == ULSDN_FAILOVER_NODE_REMOVED,
                       CONNECTED );
    }
    else
    {
        /* continue to ulnFailoverConnect() */
    }

    sRC = ulnFailoverConnect( aFnContext, ULN_FAILOVER_TYPE_STF, aNewServerInfo, &sResult );
    ACI_TEST( sRC != ACI_SUCCESS );

    ACI_EXCEPTION_CONT( CONNECTED );

#ifdef COMPILE_SHARDCLI
    sRC = ulsdModuleNotifyFailOver( aFnContext );
    ACI_TEST( sRC != ACI_SUCCESS );
#endif /* COMPILE_SHARDCLI */

    if ( sResult == ULSDN_FAILOVER_SUCCESS )
    {
        ulnDbcCloseAllStatement( sDbc );
        if ( sDbc->mXaConnection != NULL )
        {
            ulnDbcSetFailoverCallbackState(sDbc, ULN_FAILOVER_CALLBACK_IN_STATE);
            sRC = ulnFailoverXaReOpen( sDbc );
            ulnDbcSetFailoverCallbackState(sDbc, ULN_FAILOVER_CALLBACK_OUT_STATE);

            ACI_TEST( sRC != ACI_SUCCESS );
        }

        sRC = ulnClearDiagnosticInfoFromObject( aFnContext->mHandle.mObj );
        ACI_TEST_RAISE( sRC != ACI_SUCCESS, LABEL_MEM_MAN_ERR );
    }
    else if ( sResult == ULSDN_FAILOVER_NODE_REMOVED )
    {
        ulnDbcSetIsConnected( sDbc, ACP_FALSE );
    }
    else
    {
        /* Impossible case */
        ACE_DASSERT(0);
    }

    ULN_TRACE_LOG( aFnContext, ULN_TRACELOG_LOW, NULL, 0,
                   "%-18s|", "ulsdnFailoverConnectToSpecificServer" );

    return ACI_SUCCESS;

    ACI_EXCEPTION( INVALID_HANDLE_EXCEPTION )
    {
        ULN_FNCONTEXT_SET_RC( aFnContext, SQL_INVALID_HANDLE );
    }
    ACI_EXCEPTION( LABEL_MEM_MAN_ERR )
    {
        (void)ulnError( aFnContext,
                        ulERR_FATAL_MEMORY_MANAGEMENT_ERROR,
                        "ulsdnFailoverConnectToSpecificServer" );
    }
    ACI_EXCEPTION_END;

    ULN_TRACE_LOG( aFnContext, ULN_TRACELOG_LOW, NULL, 0,
                   "%-18s| fail", "ulsdnFailoverConnectToSpecificServer" );

    if ( sDbc != NULL )
    {
        if ( ulnDbcIsConnected( sDbc ) == ACP_TRUE )
        {
            ulnDbcSetIsConnected( sDbc, ACP_FALSE );
            ulnClosePhysicalConn( sDbc );
        }
    }

    return sRC;
}

SQLRETURN ulsdnReconnectCore( ulnFnContext * aFnContext )
{
    ulnDbc                *sDbc           = NULL;
    ulnFailoverServerInfo *sServerInfo    = NULL;
    acp_bool_t             sHasAlternate  = ACP_TRUE;

    ULN_FNCONTEXT_GET_DBC( aFnContext, sDbc );

    ACI_TEST_RAISE( sDbc == NULL, InvalidHandle );

    sServerInfo = ulnDbcGetCurrentServer( sDbc );
    ACI_TEST_RAISE( sServerInfo == NULL, InvalidCurrrentServer );
    
    ACI_TEST( ulsdnFailoverConnectToSpecificServer( aFnContext,
                                                    sServerInfo )
              != ACI_SUCCESS );

    if ( sHasAlternate == ACP_FALSE )
    {
        ulnDbcSetCurrentServer( sDbc, NULL );
        ulnFailoverDestroyServerInfo( sServerInfo );
    }

    return ULN_FNCONTEXT_GET_RC(aFnContext);

    ACI_EXCEPTION( InvalidHandle )
    {
        ULN_FNCONTEXT_SET_RC( aFnContext, SQL_INVALID_HANDLE );
    }
    ACI_EXCEPTION( InvalidCurrrentServer )
    {
        (void)ulnError( aFnContext, ulERR_ABORT_NO_CONNECTION );
    }

    ACI_EXCEPTION_END;

    if ( sHasAlternate == ACP_FALSE )
    {
        ulnDbcSetCurrentServer( sDbc, NULL );
        ulnFailoverDestroyServerInfo( sServerInfo );
    }

    return ULN_FNCONTEXT_GET_RC(aFnContext);
}

SQLRETURN ulsdnReconnect( acp_sint16_t        aHandleType,
                          ulnObject         * aObject,
                          const acp_uint8_t * aCause )
{
    ULN_FLAG(sNeedExit);
    ulnFnContext   sFnContext;
    ulnErrorMgr    sErrorMgr;

    ULN_INIT_FUNCTION_CONTEXT( sFnContext, ULN_FID_FOR_SD, aObject, aHandleType );

    ACI_TEST_RAISE( aObject == NULL, InvalidHandle );

    ACI_TEST(ulnEnter(&sFnContext, NULL) != ACI_SUCCESS);
    ULN_FLAG_UP(sNeedExit);

    ulnErrorMgrSetUlError( &sErrorMgr,
                           ulERR_ABORT_COMMUNICATION_LINK_FAILURE,
                           aCause );

    ACI_TEST_RAISE( ulsdnReconnectCore( &sFnContext ) != SQL_SUCCESS,
                    FAILOVER_FAIL );

    ulnError( &sFnContext, ulERR_ABORT_FAILOVER_SUCCESS,
              ulnErrorMgrGetErrorCode(&sErrorMgr),
              ulnErrorMgrGetSQLSTATE(&sErrorMgr),
              ulnErrorMgrGetErrorMessage(&sErrorMgr) );

    ULN_FLAG_DOWN(sNeedExit);
    ACI_TEST(ulnExit(&sFnContext) != ACI_SUCCESS);

    return ULN_FNCONTEXT_GET_RC(&sFnContext);

    ACI_EXCEPTION( InvalidHandle )
    {
        ULN_FNCONTEXT_SET_RC( &sFnContext, SQL_INVALID_HANDLE );
    }
    ACI_EXCEPTION( FAILOVER_FAIL )
    {
        ulnError( &sFnContext,
                  ulERR_ABORT_COMMUNICATION_LINK_FAILURE,
                  aCause );
    }

    ACI_EXCEPTION_END;

    ULN_IS_FLAG_UP(sNeedExit)
    {
        ulnExit(&sFnContext);
    }

    return ULN_FNCONTEXT_GET_RC(&sFnContext);
}

void ulsdnCheckShardNodeRemoved( ulnFnContext * aFnContext,
                                 acp_bool_t   * aIsDrop )
{
    ulnDbc       * sDbc     = NULL;
    ulnDbc       * sMetaDbc = NULL;
    acp_uint64_t   sNewTargetSMN = 0UL;

    ULN_FNCONTEXT_GET_DBC( aFnContext, sDbc );

    ACE_DASSERT( sDbc->mShardDbcCxt.mParentDbc != NULL );

    if ( sDbc->mShardDbcCxt.mParentDbc != NULL )
    {
        sMetaDbc = ulnDbcGetShardParentDbc( sDbc );

        sNewTargetSMN = ulnDbcGetTargetShardMetaNumber( sMetaDbc );

        if ( sNewTargetSMN > sDbc->mShardDbcCxt.mNodeInfo->mSMN )
        {
            *aIsDrop = ACP_TRUE;
        }
    }
}

void ulsdnRaiseShardNodeFailoverIsNotAvailableError(ulnFnContext *aFnContext)
{
    (void)ulnClearDiagnosticInfoFromObject( aFnContext->mHandle.mObj );

    (void)ulnError( aFnContext, ulERR_ABORT_SHARD_NODE_FAILOVER_IS_NOT_AVAILABLE );
}

void ulsdnRaiseShardNodeFailoverAbortError(ulnFnContext *aFnContext)
{
    (void)ulnClearDiagnosticInfoFromObject( aFnContext->mHandle.mObj );

    (void)ulnError( aFnContext, ulERR_ABORT_FAILOVER_ABORT );
}

void ulsdnRaiseShardNodeFailoverSuccessError( ulnFnContext * aFnContext,
                                              ulnErrorMgr  * aErrorMgr )
{
    (void)ulnClearDiagnosticInfoFromObject( aFnContext->mHandle.mObj );

    ulnError( aFnContext,
              ulERR_ABORT_FAILOVER_SUCCESS,
              ulnErrorMgrGetErrorCode(aErrorMgr),
              ulnErrorMgrGetSQLSTATE(aErrorMgr),
              ulnErrorMgrGetErrorMessage(aErrorMgr) );
}

void ulsdnRaiseShardNodeFailoverError( ulnFnContext * aFnContext,
                                       ulnErrorMgr  * aErrorMgr )
{
    ulnDbc     * sDbc        = NULL;
    ulnDbc     * sMetaDbc    = NULL;
    acp_bool_t   sIsNodeDrop = ACP_FALSE;

    ULN_FNCONTEXT_GET_DBC( aFnContext, sDbc );

    sMetaDbc = ulnDbcGetShardParentDbc( sDbc );

    if ( ulsdnCheckConnectionLost( sMetaDbc ) == ACP_TRUE )
    {
        ulsdnRaiseShardNodeFailoverAbortError( aFnContext );
    }
    else
    {
        ulsdnCheckShardNodeRemoved( aFnContext, &sIsNodeDrop );

        if ( sIsNodeDrop == ACP_TRUE )
        {
            ulsdnRaiseShardNodeFailoverSuccessError( aFnContext, aErrorMgr );
        }
        else
        {
            ulsdnRaiseShardNodeFailoverIsNotAvailableError( aFnContext );
        }
    }
}

acp_bool_t ulsdnCheckConnectionLost( ulnDbc * aDbc )
{
    acp_bool_t sConnectLost = ACP_FALSE;

    if ( ulnDbcIsConnected( aDbc ) == ACP_FALSE )
    {
        sConnectLost = ACP_TRUE;
    }

    return sConnectLost;
}

ACI_RC ulsdnDoFailoverAvailable( ulnFnContext       * aFnContext,
                                 ulsdFailoverResult * aResult )
{
    ulnDbc     * sDbc          = NULL;
    acp_bool_t   sIsNodeDroped = ACP_FALSE;
#ifdef COMPILE_SHARDCLI
    SQLRETURN    sRc;
#endif
    ulnShardNodeRemovalCheckerContext * sCtx = NULL;

    ULN_FNCONTEXT_GET_DBC( aFnContext, sDbc );

    switch ( sDbc->mShardDbcCxt.mShardSessionType )
    {
        case ULSD_SESSION_TYPE_COORD :
            sCtx = ulnDbcGetShardNodeRemovalCheckerContext( sDbc );

            ACE_DASSERT( sCtx != NULL );
            if ( ( sCtx != NULL ) && ( sCtx->mFunc != NULL ) )
            {
                sCtx->mFunc( sCtx->mSession, sCtx->mConnectInfo, &sIsNodeDroped );
            }
            break;

        case ULSD_SESSION_TYPE_LIB :
#ifdef COMPILE_SHARDCLI
            sRc = ulsdCheckFailoverAvailable( aFnContext,
                                              &sIsNodeDroped );
            ACI_TEST( SQL_SUCCEEDED( sRc ) == 0 );
#else
            ACI_TEST( 0 );
#endif
            break;

        case ULSD_SESSION_TYPE_USER :
        default:
            break;

    }

    if ( sIsNodeDroped == ACP_TRUE )
    {
        *aResult = ULSDN_FAILOVER_NODE_REMOVED;
    }

    return ACI_SUCCESS;

    ACI_EXCEPTION_END;

    return ACI_FAILURE;
}

