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
#include <ulnConfigFile.h>
#include <ulnPrivate.h>
#include <mtcc.h>

#include <ulsd.h>


ACI_RC ulsdCallbackUpdateNodeListResult(cmiProtocolContext *aProtocolContext,
                                        cmiProtocol        *aProtocol,
                                        void               *aServiceSession,
                                        void               *aUserContext)
{
    ulnFnContext           *sFnContext      = (ulnFnContext *)aUserContext;
    ulnDbc                 *sDbc            = NULL;
    ulsdDbc                *sShard          = NULL;
    acp_uint64_t            sShardMetaNumber = 0;
    acp_uint16_t            sNodeCount = 0;
    acp_uint16_t            i;
    acp_uint8_t             sLen;

    acp_uint32_t            sNodeId;
    acp_char_t              sNodeName[ULSD_MAX_NODE_NAME_LEN+1];
    acp_char_t              sServerIP[ULSD_MAX_SERVER_IP_LEN];
    acp_uint16_t            sPortNo;
    acp_char_t              sAlternateServerIP[ULSD_MAX_SERVER_IP_LEN];
    acp_uint16_t            sAlternatePortNo;

    /* PROJ-2655 Composite shard key */
    acp_uint8_t            sIsTestEnable = 0;

    ACP_UNUSED(aProtocol);
    ACP_UNUSED(aServiceSession);

    SHARD_LOG("(Update Node List Result) Shard Node List Result\n");

    CMI_RD1(aProtocolContext, sIsTestEnable);

    sDbc = ulnFnContextGetDbc( sFnContext );

    ACI_TEST_RAISE( sDbc == NULL, LABEL_INVALID_SHARD_OBJECT );
    ACI_TEST_RAISE( sDbc->mParentEnv->mShardModule == &gShardModuleNODE, LABEL_INVALID_SHARD_OBJECT );

    ulsdGetShardFromDbc(sDbc, &sShard);

    ACI_TEST_RAISE(sIsTestEnable > 1, LABEL_INVALID_TEST_MARK);

    if ( sIsTestEnable == 0 )
    {
        sShard->mIsTestEnable = ACP_FALSE;
    }
    else
    {
        sShard->mIsTestEnable = ACP_TRUE;
    }

    CMI_RD2(aProtocolContext, &sNodeCount);

    ACI_TEST_RAISE(sNodeCount != sShard->mNodeCount, LABEL_NODE_COUNT_MISMATCH);

    if ( sNodeCount > 0 )
    {
        for ( i = 0; i < sNodeCount; i++ )
        {
            acpMemSet(sServerIP, 0, ACI_SIZEOF(sServerIP));
            acpMemSet(sAlternateServerIP, 0, ACI_SIZEOF(sAlternateServerIP));

            CMI_RD4(aProtocolContext, &sNodeId);
            CMI_RD1(aProtocolContext, sLen);
            CMI_RCP(aProtocolContext, sNodeName, sLen);
            CMI_RCP(aProtocolContext, sServerIP, ULSD_MAX_SERVER_IP_LEN);
            CMI_RD2(aProtocolContext, &sPortNo);
            CMI_RCP(aProtocolContext, sAlternateServerIP, ULSD_MAX_SERVER_IP_LEN);
            CMI_RD2(aProtocolContext, &sAlternatePortNo);

            sNodeName[sLen] = '\0';

            ACI_TEST_RAISE( sShard->mNodeInfo[i]->mNodeId != sNodeId, LABEL_NODE_ID_MISMATCH );

            ulsdSetNodeInfo( sShard->mNodeInfo[i],
                             sNodeId,
                             sNodeName,
                             sServerIP,
                             sPortNo,
                             sAlternateServerIP,
                             sAlternatePortNo );
        }
    }
    else
    {
        /* Nothing to do */
    }

    CMI_RD8( aProtocolContext, &sShardMetaNumber );
    ulnDbcSetShardMetaNumber( sDbc, sShardMetaNumber );

    ULN_TRACE_LOG( NULL, ULN_TRACELOG_LOW, NULL, 0,
                   "%-18s| ShardMetaNumber received from meta: [%"ACI_UINT64_FMT"]",
                   "ulsdCallbackUpdateNodeListResult", sShardMetaNumber );

    ACI_TEST_RAISE(sNodeCount == 0, LABEL_NO_SHARD_NODE);

    return ACI_SUCCESS;

    ACI_EXCEPTION(LABEL_INVALID_TEST_MARK)
    {
        ulnError(sFnContext,
                 ulERR_ABORT_SHARD_ERROR,
                 "UpdateNodeList",
                 "Invalid test mark.");
    }
    ACI_EXCEPTION(LABEL_NO_SHARD_NODE)
    {
        ulnError(sFnContext,
                 ulERR_ABORT_SHARD_ERROR,
                 "UpdateNodeList",
                 "No shard node.");
    }
    ACI_EXCEPTION( LABEL_NODE_COUNT_MISMATCH )
    {
        ulnError(sFnContext,
                 ulERR_ABORT_SHARD_ERROR,
                 "UpdateNodeList",
                 "Shard node count was changed.");
    }
    ACI_EXCEPTION( LABEL_NODE_ID_MISMATCH )
    {
        ulnError( sFnContext,
                  ulERR_ABORT_SHARD_ERROR,
                  "UpdateNodeList",
                  "Shard nodes were changed." );
    }
    ACI_EXCEPTION(LABEL_INVALID_SHARD_OBJECT)
    {
        ulnError(sFnContext,
                 ulERR_ABORT_SHARD_ERROR,
                 "UpdateNodeList",
                 "Shard object is invalid.");
    }
    ACI_EXCEPTION_END;

    /* CM 콜백 함수는 communication error가 아닌 한 ACI_SUCCESS를 반환해야 한다.
     * 콜백 에러는 function context에 설정된 값으로 판단한다. */
    return ACI_SUCCESS;
}

ACI_RC ulsdCallbackGetNodeListResult(cmiProtocolContext *aProtocolContext,
                                     cmiProtocol        *aProtocol,
                                     void               *aServiceSession,
                                     void               *aUserContext)
{
    ulnFnContext  * sFnContext       = (ulnFnContext *)aUserContext;
    ulsdNodeInfo ** sNodeInfo        = NULL;
    acp_uint64_t    sShardPin        = ULSD_SHARD_PIN_INVALID;
    acp_uint64_t    sShardMetaNumber = 0;
    acp_uint16_t    sNodeCount       = 0;
    acp_uint8_t     sLen             = 0;
    acp_uint16_t    i                = 0;

    acp_uint32_t  sNodeId;
    acp_char_t    sNodeName[ULSD_MAX_NODE_NAME_LEN+1];
    acp_char_t    sServerIP[ULSD_MAX_SERVER_IP_LEN];
    acp_uint16_t  sPortNo;
    acp_char_t    sAlternateServerIP[ULSD_MAX_SERVER_IP_LEN];
    acp_uint16_t  sAlternatePortNo;

    /* PROJ-2655 Composite shard key */
    acp_uint8_t            sIsTestEnable = 0;

    ACP_UNUSED(aProtocol);
    ACP_UNUSED(aServiceSession);

    SHARD_LOG("(Get Node List Result) Shard Node List Result\n");

    CMI_RD1(aProtocolContext, sIsTestEnable);
    
    ACI_TEST_RAISE(sIsTestEnable > 1, LABEL_INVALID_TEST_MARK);

    CMI_RD2(aProtocolContext, &sNodeCount);

    if ( sNodeCount > 0 )
    {
        ACI_TEST_RAISE( ACP_RC_NOT_SUCCESS( acpMemCalloc( (void **)&sNodeInfo,
                                                          sNodeCount,
                                                          ACI_SIZEOF(ulsdNodeInfo *) ) ),
                        LABEL_NOT_ENOUGH_MEMORY );

        for ( i = 0; i < sNodeCount; i++ )
        {
            acpMemSet(sServerIP, 0, ACI_SIZEOF(sServerIP));
            acpMemSet(sAlternateServerIP, 0, ACI_SIZEOF(sAlternateServerIP));

            CMI_RD4(aProtocolContext, &sNodeId);
            CMI_RD1(aProtocolContext, sLen);
            CMI_RCP(aProtocolContext, sNodeName, sLen);
            CMI_RCP(aProtocolContext, sServerIP, ULSD_MAX_SERVER_IP_LEN);
            CMI_RD2(aProtocolContext, &sPortNo);
            CMI_RCP(aProtocolContext, sAlternateServerIP, ULSD_MAX_SERVER_IP_LEN);
            CMI_RD2(aProtocolContext, &sAlternatePortNo);

            sNodeName[sLen] = '\0';

            ACI_TEST_RAISE( ACP_RC_NOT_SUCCESS( acpMemAlloc( (void **)&(sNodeInfo[i]),
                                                             ACI_SIZEOF(ulsdNodeInfo) ) ),
                            LABEL_NOT_ENOUGH_MEMORY );

            ulsdSetNodeInfo( sNodeInfo[i],
                             sNodeId,
                             sNodeName,
                             sServerIP,
                             sPortNo,
                             sAlternateServerIP,
                             sAlternatePortNo );

            /* run-time info */
            sNodeInfo[i]->mNodeDbc = SQL_NULL_HANDLE;
            sNodeInfo[i]->mTouched = ACP_FALSE;
        }
    }
    else
    {
        /* Nothing to do */
    }

    CMI_RD8( aProtocolContext, &sShardPin );

    ULN_TRACE_LOG( NULL, ULN_TRACELOG_LOW, NULL, 0,
                   "%-18s| ShardPin received from meta: ["ULSD_SHARD_PIN_FORMAT_STR"]",
                   "ulsdCallbackGetNodeListResult", ULSD_SHARD_PIN_FORMAT_ARG( sShardPin ) );

    CMI_RD8( aProtocolContext, &sShardMetaNumber );

    ULN_TRACE_LOG( NULL, ULN_TRACELOG_LOW, NULL, 0,
                   "%-18s| ShardMetaNumber received from meta: [%"ACI_UINT64_FMT"]",
                   "ulsdCallbackGetNodeListResult", sShardMetaNumber );

    /* 모든 데이터를 수신한 후에 반영 여부를 결정한다. */

    ACI_TEST_RAISE(sNodeCount == 0, LABEL_NO_SHARD_NODE);

    ACI_TEST( ulsdApplyNodesInfo( sFnContext,
                                  sNodeInfo,
                                  sNodeCount,
                                  sShardPin,
                                  sShardMetaNumber,
                                  sIsTestEnable )
              != ACI_SUCCESS );

    return ACI_SUCCESS;

    ACI_EXCEPTION(LABEL_INVALID_TEST_MARK)
    {
        ulnError(sFnContext,
                 ulERR_ABORT_SHARD_ERROR,
                 "GetNodeList",
                 "Invalid test mark.");
    }
    ACI_EXCEPTION(LABEL_NO_SHARD_NODE)
    {
        ulnError(sFnContext,
                 ulERR_ABORT_SHARD_ERROR,
                 "GetNodeList",
                 "No shard node.");
    }
    ACI_EXCEPTION( LABEL_NOT_ENOUGH_MEMORY )
    {
        ulnError( sFnContext,
                  ulERR_ABORT_SHARD_ERROR,
                  "GetNodeList",
                  "Memory allocation error." );
    }
    ACI_EXCEPTION_END;

    if ( sNodeInfo != NULL )
    {
        for ( i = 0; i < sNodeCount; i++ )
        {
            if ( sNodeInfo[i] != NULL )
            {
                acpMemFree( (void *)sNodeInfo[i] );
            }
            else
            {
                /* Nothing to do */
            }
        }

        acpMemFree( (void *)sNodeInfo );
    }
    else
    {
        /* Nothing to do */
    }

    /* CM 콜백 함수는 communication error가 아닌 한 ACI_SUCCESS를 반환해야 한다.
     * 콜백 에러는 function context에 설정된 값으로 판단한다. */
    return ACI_SUCCESS;
}

ACI_RC ulsdUpdateNodeList(ulnFnContext *aFnContext, ulnPtContext *aPtContext)
{
    ulnDbc             *sDbc;
    cmiProtocolContext *sCtx = &aPtContext->mCmiPtContext;

    ULN_FNCONTEXT_GET_DBC(aFnContext, sDbc);

    ACI_TEST(ulsdUpdateNodeListRequest(aFnContext,aPtContext) != ACI_SUCCESS);

    ACI_TEST(ulnFlushProtocol(aFnContext, aPtContext) != ACI_SUCCESS);

    if ( cmiGetLinkImpl( sCtx ) == CMI_LINK_IMPL_IPCDA )
    {
        ACI_TEST( ulnReadProtocolIPCDA( aFnContext,
                                        aPtContext,
                                        sDbc->mConnTimeoutValue )
                  != ACI_SUCCESS );
    }
    else
    {
        ACI_TEST( ulnReadProtocol( aFnContext,
                                   aPtContext,
                                   sDbc->mConnTimeoutValue )
                  != ACI_SUCCESS );
    }

    return ACI_SUCCESS;

    ACI_EXCEPTION_END;

    return ACI_FAILURE;
}

ACI_RC ulsdGetNodeList(ulnDbc *aDbc,
                       ulnFnContext *aFnContext,
                       ulnPtContext *aPtContext)
{
    cmiProtocolContext *sCtx = &aPtContext->mCmiPtContext;

    /*
     * Shard Node List Request 전송
     */
    ACI_TEST(ulsdGetNodeListRequest(aFnContext, aPtContext) != ACI_SUCCESS);

    ACI_TEST(ulnFlushProtocol(aFnContext, aPtContext) != ACI_SUCCESS);

    if ( cmiGetLinkImpl( sCtx ) == CMI_LINK_IMPL_IPCDA )
    {
        ACI_TEST( ulnReadProtocolIPCDA( aFnContext,
                                        aPtContext,
                                        aDbc->mConnTimeoutValue )
                  != ACI_SUCCESS );
    }
    else
    {
        ACI_TEST( ulnReadProtocol( aFnContext,
                                   aPtContext,
                                   aDbc->mConnTimeoutValue ) != ACI_SUCCESS );
    }

    return ACI_SUCCESS;

    ACI_EXCEPTION_END;

    return ACI_FAILURE;
}

SQLRETURN ulsdNodeDriverConnect(ulnDbc       *aMetaDbc,
                                ulnFnContext *aFnContext,
                                acp_char_t   *aConnString,
                                acp_sint16_t  aConnStringLength)
{
    ACI_TEST( ulsdGetNodeList( aMetaDbc,
                               aFnContext,
                               &(aMetaDbc->mPtContext ) )
            != ACI_SUCCESS );

    /* Server에서 Error Protocol로 응답하는 경우에도 ACI_SUCCESS를 반환합니다. */
    ACI_TEST( ULN_FNCONTEXT_GET_RC( aFnContext ) != SQL_SUCCESS );

    ACI_TEST( ulsdDriverConnectToNode( aMetaDbc,
                                       aFnContext,
                                       aConnString,
                                       aConnStringLength )
            != SQL_SUCCESS );

    return SQL_SUCCESS;

    ACI_EXCEPTION_END;

    /* To make Success ulnExit()
     * and Disconnect Meta node connection by ulnDisconnect() */
    ULN_FNCONTEXT_SET_RC( aFnContext, SQL_SUCCESS_WITH_INFO );

    return SQL_ERROR;
}

SQLRETURN ulsdDriverConnectToNode(ulnDbc       *aMetaDbc,
                                  ulnFnContext *aFnContext,
                                  acp_char_t   *aConnString,
                                  acp_sint16_t  aConnStringLength)
{
    ulsdDbc            *sShard;
    ulsdNodeInfo      **sShardNodeInfo;
    acp_char_t          sNodeBaseConnString[ULSD_MAX_CONN_STR_LEN + 1] = {0,};
    acp_sint16_t        sNodeBaseConnStringLength = 0;
    acp_uint16_t        i;

    sShard = aMetaDbc->mShardDbcCxt.mShardDbc;

    ACI_TEST(sShard->mNodeCount == 0);
    ACI_TEST(sShard->mNodeInfo == NULL);

    sShardNodeInfo = sShard->mNodeInfo;

    ACI_TEST(ulsdMakeNodeBaseConnStr(aFnContext,
                                     aConnString,
                                     aConnStringLength,
                                     sShard->mIsTestEnable,
                                     sNodeBaseConnString) != ACI_SUCCESS);
    SHARD_LOG("(Driver Connect) NodeBaseConnString=\"%s\"\n", sNodeBaseConnString);

    /* BUG-46257 shardcli에서 Node 추가/제거 지원 */
    sNodeBaseConnStringLength = acpCStrLen( sNodeBaseConnString, ULSD_MAX_CONN_STR_LEN );

    ACI_TEST_RAISE( ACP_RC_NOT_SUCCESS( acpMemAlloc( (void **) & aMetaDbc->mShardDbcCxt.mNodeBaseConnString,
                                                     sNodeBaseConnStringLength + 1 ) ),
                    LABEL_NOT_ENOUGH_MEMORY );

    acpMemCpy( (void *)aMetaDbc->mShardDbcCxt.mNodeBaseConnString,
               (const void *) sNodeBaseConnString,
               sNodeBaseConnStringLength + 1 );

    for ( i = 0; i < sShard->mNodeCount; i++ )
    {
        ACI_TEST(ulsdAllocHandleNodeDbc(aFnContext,
                                        aMetaDbc->mParentEnv,
                                        &(sShardNodeInfo[i]->mNodeDbc))
                 != SQL_SUCCESS);

        ulsdInitalizeNodeDbc(sShardNodeInfo[i]->mNodeDbc, aMetaDbc);
    }

    for ( i = 0; i < sShard->mNodeCount; i++ )
    {
        ACI_TEST( ulsdDriverConnectToNodeInternal( aMetaDbc,
                                                   aFnContext,
                                                   sShardNodeInfo[i],
                                                   sShard->mIsTestEnable )
                  != SQL_SUCCESS );
    }

    return SQL_SUCCESS;

    ACI_EXCEPTION( LABEL_NOT_ENOUGH_MEMORY )
    {
        ulnError( aFnContext,
                  ulERR_ABORT_SHARD_ERROR,
                  "Driver Connect To Node",
                  "Memory allocation error." );
    }
    ACI_EXCEPTION_END;

    return SQL_ERROR;
}

SQLRETURN ulsdDriverConnectToNodeInternal( ulnDbc       * aMetaDbc,
                                           ulnFnContext * aFnContext,
                                           ulsdNodeInfo * aNodeInfo,
                                           acp_bool_t     aIsTestEnable )
{
    acp_char_t      sNodeConnString[ULSD_MAX_CONN_STR_LEN + 1]        = { 0, };
    acp_char_t    * sDsn = ulnDbcGetDsnString( aMetaDbc );
    SQLRETURN       sRc  = SQL_ERROR;

    ACI_TEST_RAISE( ulsdMakeNodeConnString( aFnContext,
                                            aNodeInfo,
                                            aIsTestEnable,
                                            aMetaDbc->mShardDbcCxt.mNodeBaseConnString,
                                            sNodeConnString )
                    != ACI_SUCCESS, LABEL_NODE_MAKE_CONN_STRING_FAIL );

    if ( sDsn != NULL )
    {
        ulnDbcSetDsnString( aNodeInfo->mNodeDbc,
                            sDsn,
                            acpCStrLen( sDsn, ACP_SINT32_MAX ) );
    }
    else
    {
        /* Nothing to do */
    }

    aNodeInfo->mNodeDbc->mShardDbcCxt.mNodeInfo = aNodeInfo;

    ulnDbcSetShardPin( aNodeInfo->mNodeDbc,
                       aMetaDbc->mShardDbcCxt.mShardPin );

    ulnDbcSetShardMetaNumber( aNodeInfo->mNodeDbc,
                              aMetaDbc->mShardDbcCxt.mShardMetaNumber );

    /* BUG-45707 */
    ulsdDbcSetShardCli( aNodeInfo->mNodeDbc, ULSD_SHARD_CLIENT_TRUE );

    /* BUG-45967 Data Node의 Shard Session 정리 */
    aNodeInfo->mNodeDbc->mShardDbcCxt.mSMNOfDataNode = 0;

    /* BUG-46100 Session SMN Update */
    aNodeInfo->mNodeDbc->mShardDbcCxt.mNeedToDisconnect = ACP_FALSE;

    sRc = ulnDriverConnect( aNodeInfo->mNodeDbc,
                            sNodeConnString,
                            SQL_NTS,
                            NULL,
                            0,
                            NULL );
    ACI_TEST_RAISE( sRc != SQL_SUCCESS, LABEL_NODE_CONNECTION_FAIL );

    SHARD_LOG( "(Driver Connect) NodeId=%d, Server=%s:%d\n",
               aNodeInfo->mNodeId,
               aNodeInfo->mServerIP,
               aNodeInfo->mPortNo );

    return SQL_SUCCESS;

    ACI_EXCEPTION( LABEL_NODE_MAKE_CONN_STRING_FAIL )
    {
        sRc = ULN_FNCONTEXT_GET_RC( aFnContext );
    }
    ACI_EXCEPTION( LABEL_NODE_CONNECTION_FAIL )
    {
        ulsdNativeErrorToUlnError(aFnContext,
                                  SQL_HANDLE_DBC,
                                  (ulnObject *)aNodeInfo->mNodeDbc,
                                  aNodeInfo,
                                  (acp_char_t *)"SQLDriverConnect");
    }
    ACI_EXCEPTION_END;

    return sRc;
}

ACI_RC ulsdMakeNodeConnString(ulnFnContext        *aFnContext,
                              ulsdNodeInfo        *aNodeInfo,
                              acp_bool_t           aIsTestEnable,
                              acp_char_t          *aNodeBaseConnString,
                              acp_char_t          *aOutString)
{
    ulnDbc      *sMetaDbc       = NULL;
    ulnConnType  sShardConnType = ULN_CONNTYPE_INVALID;

    ULN_FNCONTEXT_GET_DBC( aFnContext, sMetaDbc );
    ACI_TEST_RAISE( sMetaDbc == NULL, LABEL_MEM_MAN_ERR );

    sShardConnType = ulnDbcGetShardConnType( sMetaDbc );

    if ( sShardConnType == ULN_CONNTYPE_INVALID )
    {
        sShardConnType = ulnDbcGetConnType( sMetaDbc );
    }

    if ( ( aNodeInfo->mAlternateServerIP[0] == '\0' ) ||
         ( aNodeInfo->mAlternatePortNo == 0 ) )
    {
        if ( aIsTestEnable == ACP_FALSE )
        {
            ACI_TEST_RAISE(acpSnprintf(aOutString,
                                       ULSD_MAX_CONN_STR_LEN + 1,
                                       "CONNTYPE=%d;SERVER=%s;PORT_NO=%d;SHARD_NODE_NAME=%s;%s",
                                       sShardConnType,
                                       aNodeInfo->mServerIP,
                                       aNodeInfo->mPortNo,
                                       aNodeInfo->mNodeName,
                                       aNodeBaseConnString) == ULSD_MAX_CONN_STR_LEN,
                           LABEL_CONN_STR_BUFFER_OVERFLOW);
        }
        else
        {
            ACI_TEST_RAISE(acpSnprintf(aOutString,
                                       ULSD_MAX_CONN_STR_LEN + 1,
                                       "CONNTYPE=%d;SERVER=%s;PORT_NO=%d;UID=%s;PWD=%s;SHARD_NODE_NAME=%s;%s",
                                       sShardConnType,
                                       aNodeInfo->mServerIP,
                                       aNodeInfo->mPortNo,
                                       aNodeInfo->mNodeName, // UID
                                       aNodeInfo->mNodeName, // PWD
                                       aNodeInfo->mNodeName,
                                       aNodeBaseConnString) == ULSD_MAX_CONN_STR_LEN,
                           LABEL_CONN_STR_BUFFER_OVERFLOW);
        }
    }
    else
    {
        if ( aIsTestEnable == ACP_FALSE )
        {
            ACI_TEST_RAISE(acpSnprintf(aOutString,
                                       ULSD_MAX_CONN_STR_LEN + 1,
                                       "CONNTYPE=%d;SERVER=%s;PORT_NO=%d;SHARD_NODE_NAME=%s;AlternateServers=(%s:%d);%s",
                                       sShardConnType,
                                       aNodeInfo->mServerIP,
                                       aNodeInfo->mPortNo,
                                       aNodeInfo->mNodeName,
                                       aNodeInfo->mAlternateServerIP,
                                       aNodeInfo->mAlternatePortNo,
                                       aNodeBaseConnString) == ULSD_MAX_CONN_STR_LEN,
                           LABEL_CONN_STR_BUFFER_OVERFLOW);
        }
        else
        {
            ACI_TEST_RAISE(acpSnprintf(aOutString,
                                       ULSD_MAX_CONN_STR_LEN + 1,
                                       "CONNTYPE=%d;SERVER=%s;PORT_NO=%d;UID=%s;PWD=%s;"
                                       "SHARD_NODE_NAME=%s;AlternateServers=(%s:%d);%s",
                                       sShardConnType,
                                       aNodeInfo->mServerIP,
                                       aNodeInfo->mPortNo,
                                       aNodeInfo->mNodeName, // UID
                                       aNodeInfo->mNodeName, // PWD
                                       aNodeInfo->mNodeName,
                                       aNodeInfo->mAlternateServerIP,
                                       aNodeInfo->mAlternatePortNo,
                                       aNodeBaseConnString) == ULSD_MAX_CONN_STR_LEN,
                           LABEL_CONN_STR_BUFFER_OVERFLOW);
        }
    }

    SHARD_LOG("(Driver Connect) NodeConnString=\"%s\"\n", aOutString);

    return ACI_SUCCESS;

    ACI_EXCEPTION( LABEL_MEM_MAN_ERR )
    {
        ulnError( aFnContext,
                  ulERR_FATAL_MEMORY_MANAGEMENT_ERROR,
                  "ulsdMakeNodeConnString : object type is neither DBC nor STMT." );
    }
    ACI_EXCEPTION(LABEL_CONN_STR_BUFFER_OVERFLOW)
    {
        ulnError(aFnContext,
                 ulERR_ABORT_SHARD_ERROR,
                 "Driver Connect",
                 "Shard node connection string value out of range.");
    }
    ACI_EXCEPTION_END;

    return ACI_FAILURE;
}

SQLRETURN ulsdAllocHandleNodeDbc(ulnFnContext  *aFnContext,
                                 ulnEnv        *aEnv,
                                 ulnDbc       **aDbc)
{
    ACI_TEST(ulnAllocHandle(SQL_HANDLE_DBC,
                            aEnv,
                            (void **)aDbc) != ACI_SUCCESS);

    /* Node Dbc 는 Env 에 매달리면 안되므로 여기서 제거함 */
    ACI_TEST_RAISE(ulnEnvRemoveDbc(aEnv,
                                   (*aDbc)) != ACI_SUCCESS,
                   LABEL_MEM_MAN_ERR);

    return SQL_SUCCESS;

    ACI_EXCEPTION(LABEL_MEM_MAN_ERR)
    {
        ulnError(aFnContext,
                 ulERR_FATAL_MEMORY_MANAGEMENT_ERROR,
                 "Freeing DBC handle.");
    }
    ACI_EXCEPTION_END;

    return SQL_ERROR;
}

void ulsdInitalizeNodeDbc(ulnDbc      *aNodeDbc,
                          ulnDbc      *aMetaDbc)
{
    aNodeDbc->mParentEnv = aMetaDbc->mParentEnv;

    /* BUG-45967 Data Node의 Shard Session 정리 */
    aNodeDbc->mShardDbcCxt.mParentDbc = aMetaDbc;

    ulsdSetDbcShardModule(aNodeDbc, &gShardModuleNODE);
}

void ulsdMakeNodeAlternateServersString(ulsdNodeInfo      *aShardNodeInfo,
                                        acp_char_t        *aAlternateServers,
                                        acp_sint32_t       aMaxAlternateServersLen)
{
    if ( ( aShardNodeInfo->mAlternateServerIP[0] == '\0' ) ||
         ( aShardNodeInfo->mAlternatePortNo == 0 ) )
    {
        acpSnprintf(aAlternateServers,
                    aMaxAlternateServersLen,
                    "(%s:%d)",
                    aShardNodeInfo->mServerIP,
                    aShardNodeInfo->mPortNo);
    }
    else
    {
        acpSnprintf(aAlternateServers,
                    aMaxAlternateServersLen,
                    "(%s:%d,%s:%d)",
                    aShardNodeInfo->mServerIP,
                    aShardNodeInfo->mPortNo,
                    aShardNodeInfo->mAlternateServerIP,
                    aShardNodeInfo->mAlternatePortNo);
    }
}

SQLRETURN ulsdDriverConnect(ulnDbc       *aDbc,
                            acp_char_t   *aConnString,
                            acp_sint16_t  aConnStringLength,
                            acp_char_t   *aOutConnStr,
                            acp_sint16_t  aOutBufferLength,
                            acp_sint16_t *aOutConnectionStringLength)
{
    SQLRETURN      sRet = SQL_ERROR;
    ulnFnContext   sFnContext;
    acp_char_t     sBackupErrorMessage[ULSD_MAX_ERROR_MESSAGE_LEN];

    /* BUG-45707 */
    ulsdDbcSetShardCli( aDbc, ULSD_SHARD_CLIENT_TRUE );

    /* BUG-45967 Data Node의 Shard Session 정리 */
    aDbc->mShardDbcCxt.mSMNOfDataNode = 0;

    /* BUG-46100 Session SMN Update */
    aDbc->mShardDbcCxt.mNeedToDisconnect = ACP_FALSE;

    /* BUG-46100 Session SMN Update */
    ulnDbcSetShardMetaNumber( aDbc, 0 );

    sRet = ulnDriverConnect(aDbc,
                            aConnString,
                            aConnStringLength,
                            aOutConnStr,
                            aOutBufferLength,
                            aOutConnectionStringLength);
    ACI_TEST_RAISE(!SQL_SUCCEEDED(sRet), LABEL_CONNECTION_FAIL);

    ACI_TEST_RAISE(sRet == SQL_SUCCESS_WITH_INFO, LABEL_SUCCESS_WITH_INFO); 

    return sRet;

    ACI_EXCEPTION(LABEL_SUCCESS_WITH_INFO)
    {
        if ( ulnGetDiagRec(SQL_HANDLE_DBC,
                           (ulnObject *)aDbc,
                           1,
                           NULL,
                           NULL,
                           sBackupErrorMessage, 
                           ULSD_MAX_ERROR_MESSAGE_LEN,
                           NULL,
                           ACP_FALSE) != SQL_NO_DATA )
        {
            /*
             * ulsdSilentDisconnect() 내 ulnEnter() 호출 후 MetaDbc 의 Diagnostic Record 가 초기화 되기 때문에
             * 먼저 에러 메세지를 sBackupErrorMessage 에 백업한 다음 나중에 ulnError() 로 MetaDbc 에 추가한다.
             * 반드시 아래 순서대로 호출 해야한다.
             *
             * 1. ulsdSilentDisconnect()
             * 2. ulnError()
             */
            ulsdSilentDisconnect(aDbc);

            ULN_INIT_FUNCTION_CONTEXT(sFnContext, ULN_FID_DRIVERCONNECT, aDbc, ULN_OBJ_TYPE_DBC);

            ulnError(&sFnContext,
                     ulERR_ABORT_SHARD_DATA_NODE_CONNECTION_FAILURE,
                     sBackupErrorMessage);
        }
        else
        {
            ulsdSilentDisconnect(aDbc);
        }
    }
    ACI_EXCEPTION(LABEL_CONNECTION_FAIL)
    {
        ulsdSilentDisconnect(aDbc);
    }
    ACI_EXCEPTION_END;

    return SQL_ERROR;
}

acp_bool_t ulsdHasNoData( ulnDbc * aMetaDbc )
{
    acp_list_node_t * sIterator = NULL;
    ulnStmt         * sStmt     = NULL;

    ACP_LIST_ITERATE( & aMetaDbc->mStmtList, sIterator )
    {
        sStmt = (ulnStmt *)sIterator;

        if ( ulnStmtIsPrepared( sStmt ) == ACP_TRUE )
        {
            ACI_TEST( ulsdModuleHasNoData( sStmt ) == ACP_FALSE );
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

acp_bool_t ulsdIsTimeToUpdateShardMetaNumber( ulnDbc * aMetaDbc )
{
    acp_uint64_t   sSMN           = ulnDbcGetShardMetaNumber( aMetaDbc );
    acp_uint64_t   sSMNOfDataNode = ulnDbcGetSMNOfDataNode( aMetaDbc );
    acp_bool_t     sResult        = ACP_FALSE;

    if ( ( sSMN != 0 ) &&
         ( sSMN < sSMNOfDataNode ) &&
         ( ulnDbcGetNeedToDisconnect( aMetaDbc ) == ACP_FALSE ) &&
         ( ulsdHasNoData( aMetaDbc ) == ACP_TRUE ) )
    {
        sResult = ACP_TRUE;
    }
    else
    {
        /* Nothing to do */
    }

    return sResult;
}

SQLRETURN ulsdUpdateShardMetaNumber( ulnDbc       * aMetaDbc,
                                     ulnFnContext * aFnContext )
{
    ulsdDbc      * sShard       = aMetaDbc->mShardDbcCxt.mShardDbc;
    acp_uint64_t   sOldShardPin = ulnDbcGetShardPin( aMetaDbc );
    acp_uint64_t   sOldSMN      = ulnDbcGetShardMetaNumber( aMetaDbc );
    acp_uint64_t   sNewSMN      = 0;
    acp_uint16_t   i            = 0;

    ACI_TEST_RAISE( aMetaDbc->mIsConnected == ACP_FALSE, LABEL_ABORT_NO_CONNECTION );

    /* User Connection의 SMN을 갱신합니다.
     *  - Get Node List를 사용하여 Node List와 Meta Node SMN을 얻습니다.
     */
    ACI_TEST( ulsdGetNodeList( aMetaDbc,
                               aFnContext,
                               & aMetaDbc->mPtContext )
              != ACI_SUCCESS );

    /* Server에서 Error Protocol로 응답하는 경우에도 ACI_SUCCESS를 반환합니다. */
    ACI_TEST( ULN_FNCONTEXT_GET_RC( aFnContext ) != SQL_SUCCESS );

    /* ShardPin은 불변입니다. */
    ACI_TEST_RAISE( sOldShardPin != ulnDbcGetShardPin( aMetaDbc ), LABEL_SHARD_PIN_CHANGED );

    /* Shard Library Connection의 SMN을 갱신합니다. */
    sNewSMN = ulnDbcGetShardMetaNumber( aMetaDbc );
    if ( sOldSMN < sNewSMN )
    {
        for ( i = 0; i < sShard->mNodeCount; i++ )
        {
            ACI_TEST_RAISE( ulsdSetConnectAttrNode( sShard->mNodeInfo[i]->mNodeDbc,
                                                    ULN_PROPERTY_SHARD_META_NUMBER,
                                                    (void *)sNewSMN )
                            != SQL_SUCCESS, LABEL_NODE_SETCONNECTATTR_FAIL );

            ulnDbcSetShardMetaNumber( sShard->mNodeInfo[i]->mNodeDbc,
                                      sNewSMN );
        }
    }
    else
    {
        /* Nothing to do */
    }

    return SQL_SUCCESS;

    ACI_EXCEPTION( LABEL_ABORT_NO_CONNECTION )
    {
        ulnError( aFnContext, ulERR_ABORT_NO_CONNECTION, "" );
    }
    ACI_EXCEPTION( LABEL_SHARD_PIN_CHANGED )
    {
        ulnError( aFnContext,
                  ulERR_ABORT_SHARD_ERROR,
                  "UpdateShardMetaNumber",
                  "Shard pin was changed." );
    }
    ACI_EXCEPTION( LABEL_NODE_SETCONNECTATTR_FAIL )
    {
        ulsdNativeErrorToUlnError( aFnContext,
                                   SQL_HANDLE_DBC,
                                   (ulnObject *)sShard->mNodeInfo[i]->mNodeDbc,
                                   sShard->mNodeInfo[i],
                                   "UpdateShardMetaNumber" );
    }
    ACI_EXCEPTION_END;

    return SQL_ERROR;
}

SQLRETURN ulsdSetConnectAttrNode( ulnDbc        * aDbc,
                                  ulnPropertyId   aCmPropertyID,
                                  void          * aValuePtr )
{
    ULN_FLAG( sNeedExit );
    ulnFnContext sFnContext;

    ULN_INIT_FUNCTION_CONTEXT( sFnContext, ULN_FID_SETCONNECTATTR, aDbc, ULN_OBJ_TYPE_DBC );

    ACI_TEST( ulnEnter( & sFnContext, (void *) & aCmPropertyID ) != ACI_SUCCESS );

    ULN_FLAG_UP( sNeedExit );

    ACI_TEST( ulnSendConnectAttr( & sFnContext,
                                  aCmPropertyID,
                                  aValuePtr )
              != ACI_SUCCESS );

    ULN_FLAG_DOWN( sNeedExit );

    ACI_TEST( ulnExit( & sFnContext ) != ACI_SUCCESS );

    return ULN_FNCONTEXT_GET_RC( & sFnContext );

    ACI_EXCEPTION_END;

    ULN_IS_FLAG_UP( sNeedExit )
    {
        (void)ulnExit( & sFnContext );
    }

    return ULN_FNCONTEXT_GET_RC( & sFnContext );
}
