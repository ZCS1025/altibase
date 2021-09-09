#include <uln.h>
#include <ulnPrivate.h>
#include <ulnConnectCore.h>
#include <ulsdError.h>
#include <ulsdnFailover.h>
#include <ulsdnFailoverSuspend.h>
#include <sdErrorCodeClient.h>
#include <ulsdFailover.h>
#include <ulsdCommunication.h>

void ulsdSetNextFailoverServer( ulnDbc *sNodeDbc )
{
    ulnFailoverServerInfo * sNewServerInfo = NULL;
    ulnFailoverServerInfo * sOldServerInfo = ulnDbcGetCurrentServer( sNodeDbc );
    acp_uint32_t            sIdx;

    ACP_TEST_RAISE( sNodeDbc->mFailoverServers == NULL, END_FUNCTION );

    for ( sIdx = 0; sIdx < sNodeDbc->mFailoverServersCnt; ++sIdx )
    {
        sNewServerInfo = sNodeDbc->mFailoverServers[sIdx];
        if ( sNewServerInfo == sOldServerInfo )
        {
            continue;
        }
        ulnDbcSetCurrentServer( sNodeDbc, sNewServerInfo );
        break;
    }

    ACI_EXCEPTION_CONT( END_FUNCTION );

    return;
}

void ulsdAlignDataNodeConnection( ulnFnContext * aFnContext,
                                  ulnDbc       * aNodeDbc )
{
    ulnFnContext            sNodeFnContext;
    ulsdAlignInfo         * sAlignInfo         = NULL;
    ulnFailoverServerInfo * sNewServerInfo     = NULL;

    sAlignInfo = &aNodeDbc->mShardDbcCxt.mAlignInfo;

    ACI_TEST_RAISE( sAlignInfo->mIsNeedAlign == ACP_FALSE,
                    END_OF_FUNCTION )
    ACI_TEST_RAISE( sAlignInfo->mNativeErrorCode
                        != ACI_E_ERROR_CODE( sdERR_ABORT_SHARD_LIBRARY_LINK_FAILURE_ERROR ),
                    END_OF_FUNCTION )


    ULN_INIT_FUNCTION_CONTEXT( sNodeFnContext, ULN_FID_NONE, aNodeDbc, ULN_OBJ_TYPE_DBC );

    if ( ulnFailoverIsOn( aNodeDbc ) == ACP_TRUE )
    {
        ulsdSetNextFailoverServer( aNodeDbc );

        ulnDbcSetIsConnected( aNodeDbc, ACP_FALSE );
        ulnClosePhysicalConn( aNodeDbc );

        sNewServerInfo = ulnDbcGetCurrentServer( aNodeDbc );

        if ( ulsdnFailoverConnectToSpecificServer( &sNodeFnContext,
                                                   sNewServerInfo )
             == ACI_SUCCESS )
        {
            ulnError( &sNodeFnContext,
                      ulERR_ABORT_FAILOVER_SUCCESS,
                      sAlignInfo->mNativeErrorCode,
                      sAlignInfo->mSQLSTATE,
                      sAlignInfo->mMessageText );
        }
        else
        {
            ulsdnRaiseShardNodeFailoverIsNotAvailableError( &sNodeFnContext );
        }

        ulsdNativeErrorToUlnError( aFnContext,
                                   SQL_HANDLE_DBC,
                                   (ulnObject*)aNodeDbc,
                                   aNodeDbc->mShardDbcCxt.mNodeInfo,
                                   (acp_char_t *)"ulsdAlignDataNodeConnection");
    }

    ACI_EXCEPTION_CONT( END_OF_FUNCTION );

    sAlignInfo->mIsNeedAlign = ACP_FALSE;
}

acp_bool_t ulsdGetNodeConnectionReport( ulnDbc         * aNodeDbc,
                                        ulsdNodeReport * aReport,
                                        acp_bool_t       aOnRollback )
{
    ulnFailoverServerInfo   * sCurServerInfo = NULL;
    ulsdNodeInfo            * sDataNodeInfo  = NULL;
    ulsdNodeConnectReport   * sConnectReport = NULL;
    ulnDbc                  * sMetaDbc       = ulnDbcGetShardParentDbc( aNodeDbc );

    sDataNodeInfo = aNodeDbc->mShardDbcCxt.mNodeInfo;
    sConnectReport = &(aReport->mArg.mConnectReport);

    if ( aOnRollback == ACP_TRUE )
    {
        aReport->mType = ULSD_REPORT_TYPE_STATUS;
    }
    else
    {
        /* BUG-49010 resharding 으로 삭제되는 노드에 대한 connection report 를 전달하지 않아야 합니다. 
         *   Target SMN 과 NodeInfo 의 SMN 이 다르다는 것은
         *   Resharding 발생으로 제거될 노드라는 뜻이다.
         *   User session 에서 먼저 Resharding 이 진행된 경우
         *   삭제될 노드에 대해 Connection Report 를 전송하면
         *   Shard Coordinator 는 해당 노드를 찾을 수 없다.
         *   따라서 삭제될 노드에 대해서는 전송하지 않아야 한다.
         */
        ACI_TEST_CONT( ( ulnDbcGetTargetShardMetaNumber( sMetaDbc ) != 
                         aNodeDbc->mShardDbcCxt.mNodeInfo->mSMN ),
                       NODE_DROP_DETECTED );

        aReport->mType = ULSD_REPORT_TYPE_CONNECTION;
    }

    sCurServerInfo = ulnDbcGetCurrentServer( aNodeDbc );
    if ( sCurServerInfo == NULL )
    {
        /* No alternate server */
        sConnectReport->mDestination = ULSD_CONN_TO_ACTIVE;
    }
    else
    {
        /* 현재 data node 주소는 active, alternate 두개만 지정 가능하므로
         * active 와 비교하여 동일하면 active 에 접속,
         * active 와 비교하여 동일하지 않으면 alternate 에 접속한것으로 가정한다.
         */
        if ( ( acpCStrCmp( sCurServerInfo->mHost,
                           sDataNodeInfo->mServerIP,
                           ULSD_MAX_SERVER_IP_LEN ) == 0 )
             && ( sCurServerInfo->mPort == sDataNodeInfo->mPortNo ) )
        {
            sConnectReport->mDestination = ULSD_CONN_TO_ACTIVE;
        }
        else
        {
            sConnectReport->mDestination = ULSD_CONN_TO_ALTERNATE;
        }
    }

    sConnectReport->mNodeId = sDataNodeInfo->mNodeId;

    return ACP_TRUE;

    ACI_EXCEPTION_CONT( NODE_DROP_DETECTED );

    ACE_DASSERT( ulnDbcGetTargetShardMetaNumber( sMetaDbc ) > 
                 aNodeDbc->mShardDbcCxt.mNodeInfo->mSMN );

    aReport->mType = ULSD_REPORT_TYPE_TRANSACTION_BROKEN;

    return ACP_TRUE;
}

ACI_RC ulsdSendNodeConnectionReport( ulnFnContext *aMetaFnContext, ulsdNodeReport *aReport )
{
    ulnDbc  * sDbc   = NULL;
    ulsdDbc * sShard = NULL;

    ulsdnFailoverSuspendBackup sBackup;
    ulsdnClearFailoverSuspendBackup( &sBackup );

    ULN_FNCONTEXT_GET_DBC( aMetaFnContext, sDbc );

    ACI_TEST_RAISE( sDbc == NULL, InvalidHandleException );

    sShard = sDbc->mShardDbcCxt.mShardDbc;

    /* Failover Succes is not error.
     * Disconnect status (or failover failure) is not error.
     * All data node connection status will be sent after Meta failover success.
     */
    ulsdnDbcSetFailoverSuspendState( sDbc,
                                     ULSDN_FAILOVER_SUSPEND_ALL,
                                     &sBackup);
    ulnDbcSetFailoverSuspendSkipError( sDbc );

    if ( ulsdShardNodeReport( aMetaFnContext,
                              aReport )
         == ACI_SUCCESS )
    {
        if ( sDbc->mAttrAutoCommit == SQL_AUTOCOMMIT_OFF )
        {
            sShard->mTouched = ACP_TRUE;
        }
    }

    ulsdnDbcUnsetFailoverSuspendState( sDbc,
                                       &sBackup);

    ACI_TEST( SQL_SUCCEEDED( ULN_FNCONTEXT_GET_RC( aMetaFnContext ) ) == 0 );

    return ACI_SUCCESS;

    ACI_EXCEPTION( InvalidHandleException )
    {
        ULN_FNCONTEXT_SET_RC( aMetaFnContext, SQL_INVALID_HANDLE );
    }
    ACI_EXCEPTION_END;

    ulsdnDbcUnsetFailoverSuspendState( sDbc,
                                       &sBackup);

    return ACI_FAILURE;
}

ACI_RC ulsdNotifyFailoverOnMeta( ulnFnContext * aFnContext )
{
    ulnDbc                   *sMetaDbc       = NULL;
    ulnDbc                   *sNodeDbc       = NULL;
    ulsdDbc                  *sShard         = NULL;
    ulsdNodeReport            sReport;
    acp_sint32_t              sIdx;

    ULN_FNCONTEXT_GET_DBC( aFnContext, sMetaDbc );

    sShard = sMetaDbc->mShardDbcCxt.mShardDbc;

    for ( sIdx = 0; sIdx < sShard->mNodeCount; ++sIdx )
    {
        sNodeDbc = sShard->mNodeInfo[sIdx]->mNodeDbc;
        if ( sNodeDbc == NULL )
        {
            ACE_DASSERT( 0 );
            continue;
        }

        if ( ulsdGetNodeConnectionReport( sNodeDbc,
                                          &sReport,
                                          ULN_FNCONTEXT_IS_ROLLBACK( aFnContext ) )
             == ACP_TRUE )
        {
            ACI_TEST( ulsdSendNodeConnectionReport( aFnContext, &sReport ) != ACI_SUCCESS );
        }
    }

    return ACI_SUCCESS;

    ACI_EXCEPTION_END;

    return ACI_FAILURE;
}

acp_bool_t ulsdIsFailoverExecute( ulnFnContext  * aFnContext )
{                
    ulnDbc          * sDbc = NULL;
    acp_bool_t        sRet = ACP_FALSE;
                 
    ULN_FNCONTEXT_GET_DBC( aFnContext, sDbc );
    if ( sDbc != NULL )
    {            
        if ( ( aFnContext->mIsFailoverSuccess == ACP_TRUE ) ||
             ( ulnDbcIsConnected( sDbc ) == ACP_FALSE ) )
        {
            sRet = ACP_TRUE;
        }
    }

    return sRet;
}

ACI_RC ulsdFODoSTF( ulnFnContext     * aFnContext,
                    ulnErrorMgr      * aErrorMgr )
{
    ulnDbc  * sMetaDbc = NULL;
    ACI_RC    sRc      = ACI_FAILURE;

    ULN_FNCONTEXT_GET_DBC( aFnContext, sMetaDbc );

    ACI_TEST( ulnFailoverDoSTF( aFnContext ) != ACI_SUCCESS );

    (void)ulnError( aFnContext,
                    ulERR_ABORT_FAILOVER_SUCCESS,
                    ulnErrorMgrGetErrorCode(aErrorMgr),
                    ulnErrorMgrGetSQLSTATE(aErrorMgr),
                    ulnErrorMgrGetErrorMessage(aErrorMgr) );

    return ACI_SUCCESS;

    ACI_EXCEPTION_END;

    if ( ulsdnDbcIsSkipFailoverSuspendError( sMetaDbc ) == ACP_TRUE )
    {
        /* Error skip and Success */
        /* Nothing to do */
        sRc = ACI_SUCCESS;
    }
    else
    {
        (void)ulnErrHandleCmError( aFnContext, NULL );
    }

    return sRc;
}

ACI_RC ulsdFODoSTF4LibConn( ulnFnContext     * aFnContext,
                            ulnErrorMgr      * aErrorMgr )
{
    ulnDbc  * sNodeDbc = NULL;
    ulnDbc  * sMetaDbc = NULL;
    ulsdDbc * sShard   = NULL;
    ACI_RC    sRc      = ACI_FAILURE;

    ULN_FNCONTEXT_GET_DBC( aFnContext, sNodeDbc );

    sMetaDbc = sNodeDbc->mShardDbcCxt.mParentDbc;
    sShard   = sMetaDbc->mShardDbcCxt.mShardDbc;

    if ( sMetaDbc->mAttrAutoCommit == SQL_AUTOCOMMIT_OFF )
    {
        /* BUG-47143 샤드 All meta 환경에서 Failover 를 검증합니다. */
        sShard->mTouched = ACP_TRUE;
    }

    ACI_TEST( ulnFailoverDoSTF( aFnContext ) != ACI_SUCCESS );

    (void)ulnError( aFnContext,
                    ulERR_ABORT_FAILOVER_SUCCESS,
                    ulnErrorMgrGetErrorCode(aErrorMgr),
                    ulnErrorMgrGetSQLSTATE(aErrorMgr),
                    ulnErrorMgrGetErrorMessage(aErrorMgr) );

    return ACI_SUCCESS;

    ACI_EXCEPTION_END;

    if ( ulnDbcIsConnected( sNodeDbc ) == ACP_TRUE )
    {
        ulnDbcSetIsConnected( sNodeDbc, ACP_FALSE );
        ulnClosePhysicalConn( sNodeDbc );
    }

    if ( ulsdnDbcIsSkipFailoverSuspendError( sNodeDbc ) == ACP_TRUE )
    {
        /* Error skip and Success */
        /* Nothing to do */
        sRc = ACI_SUCCESS;
    }
    else if ( ulsdnDbcIsSetFailoverSuspendErrorCode( sNodeDbc ) == ACP_TRUE )
    {
        (void)ulnError( aFnContext,
                        ulsdnDbcGetFailoverSuspendErrorCode( sNodeDbc ),
                        ulnErrorMgrGetErrorCode(aErrorMgr),
                        ulnErrorMgrGetSQLSTATE(aErrorMgr),
                        ulnErrorMgrGetErrorMessage(aErrorMgr) );
    }
    else
    {
        ulsdnRaiseShardNodeFailoverError( aFnContext, aErrorMgr );
    }

    return sRc;
}

ACI_RC ulsdFailoverUserConnection( ulnFnContext *aMetaFnContext )
{
    ulnDbc    * sMetaDbc = NULL;
    SQLRETURN   sRc      = SQL_ERROR;

    ULN_FNCONTEXT_GET_DBC( aMetaFnContext, sMetaDbc );

    if ( ulnDbcIsConnected( sMetaDbc ) == ACP_FALSE )
    {
        sRc = ulsdnSimpleShardEndTranDbc( sMetaDbc,
                                          (acp_sint16_t)ULN_TRANSACT_ROLLBACK );

        ACI_TEST_RAISE( ulnDbcIsConnected( sMetaDbc ) == ACP_FALSE,
                        ErrFailoverFail );
    }

    return ACI_SUCCESS;

    ACI_EXCEPTION( ErrFailoverFail )
    {
        ULN_FNCONTEXT_SET_RC( aMetaFnContext, sRc );
    }
    ACI_EXCEPTION_END;

    return ACI_FAILURE;
}

