/***********************************************************************
 * Copyright 1999-2012, ALTIBase Corporation or its subsidiaries.
 * All rights reserved.
 **********************************************************************/

/***********************************************************************
 * $Id$
 **********************************************************************/

#include <uln.h>
#include <ulnPrivate.h>
#include <mtcc.h>

#include <ulsd.h>

SQLRETURN ulsdSetConnectAttr(ulnDbc       *aMetaDbc,
                             acp_sint32_t  aAttribute,
                             void         *aValuePtr,
                             acp_sint32_t  aStringLength)
{
    acp_list_node_t     * sNode       = NULL;
    acp_list_node_t     * sNext       = NULL;
    ulsdConnectAttrInfo * sObj        = NULL;
    ulsdConnectAttrInfo * sNewObj     = NULL;
    ulnConnAttrID         sConnAttrID = ULN_CONN_ATTR_MAX;
    acp_sint32_t          sRealLength = 0;

    SQLRETURN           sRet = SQL_ERROR;
    ulsdDbc            *sShard;
    acp_uint32_t        sNodeArr[ULSD_SD_NODE_MAX_COUNT];
    acp_uint16_t        sNodeCount = 0;
    acp_uint16_t        sNodeIndex;
    ulnFnContext        sFnContext;
    acp_uint16_t        i;

    ULN_INIT_FUNCTION_CONTEXT(sFnContext, ULN_FID_SETCONNECTATTR, aMetaDbc, ULN_OBJ_TYPE_DBC);

    /* BUG-46257 shardcli에서 Node 추가/제거 지원 */
    ACI_TEST_RAISE( acpMemAlloc( (void **) & sNewObj,
                                 ACI_SIZEOF( ulsdConnectAttrInfo ) )
                    != ACP_RC_SUCCESS, LABEL_NOT_ENOUGH_MEMORY );

    sNewObj->mAttribute       = aAttribute;
    sNewObj->mValuePtr        = aValuePtr;
    sNewObj->mStringLength    = aStringLength;
    sNewObj->mBufferForString = NULL;

    sConnAttrID = ulnGetConnAttrIDfromSQL_ATTR_ID( aAttribute );

    if ( ( sConnAttrID != ULN_CONN_ATTR_MAX ) &&
         ( aValuePtr != NULL ) )
    {
        if ( ( gUlnConnAttrTable[sConnAttrID].mAttrType == ULN_CONN_ATTR_TYPE_STRING ) ||
             ( gUlnConnAttrTable[sConnAttrID].mAttrType == ULN_CONN_ATTR_TYPE_UPPERCASE_STRING ) )
        {
            if ( aStringLength == SQL_NTS )
            {
                sRealLength = (acp_sint32_t)acpCStrLen( (const acp_char_t *)aValuePtr, ACP_SINT32_MAX );
            }
            else
            {
                sRealLength = aStringLength;
            }

            if ( sRealLength > 0 )
            {
                ACI_TEST_RAISE( acpMemAlloc( (void **) & sNewObj->mBufferForString,
                                             sRealLength + 1 )
                                != ACP_RC_SUCCESS, LABEL_NOT_ENOUGH_MEMORY );

                acpMemCpy( (void *) sNewObj->mBufferForString,
                           (const void *) aValuePtr,
                           sRealLength );
                sNewObj->mBufferForString[sRealLength] = '\0';
            }
            else
            {
                /* Nothing to do */
            }
        }
        else
        {
            /* Nothing to do */
        }
    }
    else
    {
        /* Nothing to do */
    }

    acpListInitObj( & sNewObj->mListNode, (void *)sNewObj );

    ulsdGetShardFromDbc(aMetaDbc, &sShard);

    ulsdGetTouchedAllNodeList(sShard, sNodeArr, &sNodeCount);

    /* BUG-45411 touch한 data노드에서 먼저 수행하고, 모두 성공하면 meta노드에서 수행한다. */
    for ( i = 0; i < sNodeCount; i++ )
    {
        sNodeIndex = sNodeArr[i];

        sRet = ulnSetConnectAttr(sShard->mNodeInfo[sNodeIndex]->mNodeDbc,
                                 aAttribute,
                                 aValuePtr,
                                 aStringLength);
        ACI_TEST_RAISE(sRet != SQL_SUCCESS, LABEL_NODE_SETCONNECTATTR_FAIL);

        SHARD_LOG("(Set Connect Attr) Attr=%d, NodeId=%d, Server=%s:%d\n",
                  aAttribute,
                  sShard->mNodeInfo[sNodeIndex]->mNodeId,
                  sShard->mNodeInfo[sNodeIndex]->mServerIP,
                  sShard->mNodeInfo[sNodeIndex]->mPortNo);
    }

    sRet = ulnSetConnectAttr(aMetaDbc,
                             aAttribute,
                             aValuePtr,
                             aStringLength);
    ACI_TEST(sRet != SQL_SUCCESS);

    /* shard tx option 설정 */
    if ( ulnGetConnAttrIDfromSQL_ATTR_ID(aAttribute) == ULN_CONN_ATTR_AUTOCOMMIT )
    {
        if ( aStringLength == ALTIBASE_SHARD_SINGLE_NODE_TRANSACTION )
        {
            aMetaDbc->mShardDbcCxt.mShardTransactionLevel = ULN_SHARD_TX_ONE_NODE;
        }
        else if ( aStringLength == ALTIBASE_SHARD_GLOBAL_TRANSACTION )
        {
            ACI_TEST_RAISE( ulnSendConnectAttr( & sFnContext,
                                                ULN_PROPERTY_DBLINK_GLOBAL_TRANSACTION_LEVEL,
                                                (void *) 2 )  /* dblink global tx */
                            != ACI_SUCCESS, LABEL_NODE_SENDCONNECTATTR_FAIL );

            sRet = ULN_FNCONTEXT_GET_RC( & sFnContext );
            ACI_TEST( sRet != SQL_SUCCESS );

            aMetaDbc->mShardDbcCxt.mShardTransactionLevel = ULN_SHARD_TX_GLOBAL;
        }
        else
        {
            ACI_TEST_RAISE( ulnSendConnectAttr( & sFnContext,
                                                ULN_PROPERTY_DBLINK_GLOBAL_TRANSACTION_LEVEL,
                                                (void *) 1 )  /* dblink simple tx */
                            != ACI_SUCCESS, LABEL_NODE_SENDCONNECTATTR_FAIL );

            sRet = ULN_FNCONTEXT_GET_RC( & sFnContext );
            ACI_TEST( sRet != SQL_SUCCESS );

            aMetaDbc->mShardDbcCxt.mShardTransactionLevel = ULN_SHARD_TX_MULTI_NODE;
        }
    }

    /* BUG-46257 shardcli에서 Node 추가/제거 지원 */
    ACP_LIST_ITERATE_SAFE( & aMetaDbc->mShardDbcCxt.mConnectAttrList, sNode, sNext )
    {
        sObj = (ulsdConnectAttrInfo *)sNode->mObj;
        if ( sObj->mAttribute == aAttribute )
        {
            acpListDeleteNode( sNode );

            if ( sObj->mBufferForString != NULL )
            {
                acpMemFree( sObj->mBufferForString );
            }
            else
            {
                /* Nothing to do */
            }

            acpMemFree( sNode->mObj );
            break;
        }
        else
        {
            /* Nothing to do */
        }
    }

    acpListAppendNode( & aMetaDbc->mShardDbcCxt.mConnectAttrList,
                       & sNewObj->mListNode );

    return SQL_SUCCESS;

    ACI_EXCEPTION(LABEL_NODE_SETCONNECTATTR_FAIL)
    {
        ulsdNativeErrorToUlnError(&sFnContext,
                                  SQL_HANDLE_DBC,
                                  (ulnObject *)sShard->mNodeInfo[sNodeIndex]->mNodeDbc,
                                  sShard->mNodeInfo[sNodeIndex],
                                  "Set Connect Attr");
    }
    ACI_EXCEPTION( LABEL_NODE_SENDCONNECTATTR_FAIL )
    {
        sRet = ULN_FNCONTEXT_GET_RC( & sFnContext );
    }
    ACI_EXCEPTION( LABEL_NOT_ENOUGH_MEMORY )
    {
        ulnError( & sFnContext,
                  ulERR_ABORT_SHARD_ERROR,
                  "SetConnectAttr",
                  "Memory allocation error." );

        sRet = ULN_FNCONTEXT_GET_RC( & sFnContext );
    }
    ACI_EXCEPTION_END;

    if ( sNewObj != NULL )
    {
        if ( sNewObj->mBufferForString != NULL )
        {
            acpMemFree( sNewObj->mBufferForString );
        }
        else
        {
            /* Nothing to do */
        }

        acpMemFree( sNewObj );
    }
    else
    {
        /* Nothing to do */
    }

    return sRet;
}

SQLRETURN ulsdSetConnectAttrOnNode( ulnFnContext   * aFnContext,
                                    ulsdDbcContext * aMetaShardDbcCxt,
                                    ulsdNodeInfo   * aNodeInfo )
{
    acp_list_node_t       * sNode = NULL;
    ulsdConnectAttrInfo   * sAttr = NULL;
    SQLRETURN               sRet  = SQL_ERROR;

    ACP_LIST_ITERATE( & aMetaShardDbcCxt->mConnectAttrList, sNode )
    {
        sAttr = (ulsdConnectAttrInfo *)sNode->mObj;

        if ( sAttr->mBufferForString != NULL )
        {
            sRet = ulnSetConnectAttr( aNodeInfo->mNodeDbc,
                                      sAttr->mAttribute,
                                      sAttr->mBufferForString,
                                      sAttr->mStringLength );
        }
        else
        {
            sRet = ulnSetConnectAttr( aNodeInfo->mNodeDbc,
                                      sAttr->mAttribute,
                                      sAttr->mValuePtr,
                                      sAttr->mStringLength );
        }

        ACI_TEST_RAISE( sRet != SQL_SUCCESS, LABEL_NODE_SETCONNECTATTR_FAIL );
    }

    return SQL_SUCCESS;

    ACI_EXCEPTION( LABEL_NODE_SETCONNECTATTR_FAIL )
    {
        ulsdNativeErrorToUlnError( aFnContext,
                                   SQL_HANDLE_DBC,
                                   (ulnObject *)aNodeInfo->mNodeDbc,
                                   aNodeInfo,
                                   "Set Connect Attr" );
    }
    ACI_EXCEPTION_END;

    return sRet;
}
