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

#include <ulnPrivate.h>
#include <ulnConnectCore.h>
#include <ulnFailOver.h>
#include <ulsdnFailover.h>
#include <ulsdnFailoverSuspend.h>
#include <ulnFunctionContext.h>
#include <ulnDataSource.h>
#include <ulnTraceLog.h>
#include <ulnURL.h>
#include <ulnString.h>
#include <ulxXaProtocol.h>
#include <xa.h>
#include <mmErrorCodeClient.h>

/* BUGBUG (2016-11-10) 값 일치 필요: MMC_FAILOVER_SOURCE_MAX_LEN */
#define ULN_MAX_FAILOVER_SOURCE_LEN 256

static const acp_char_t* gFailoverTypeName[] =
{
    "CTF",
    "STF"
};

acp_bool_t ulnFailoverIsOn(ulnDbc *aDbc)
{
    acp_bool_t sRet = ACP_FALSE;

    if ( aDbc->mShardDbcCxt.mShardSessionType == ULSD_SESSION_TYPE_USER )
    {
        sRet = ((aDbc->mFailoverServersCnt > 0) ? ACP_TRUE : ACP_FALSE);
    }
    else
    {
        /* Coordinator or library connection 경우 무조건 Primary Server Info 를 구성한다.
         * ulnFailoverBuildServerList() 참고 */
        sRet = ((aDbc->mFailoverServersCnt > 1) ? ACP_TRUE : ACP_FALSE);
    }

    return sRet;
}

ACP_INLINE
acp_uint32_t ulnFailoverChooseRandomServer(ulnDbc *aDbc, acp_uint32_t aStart)
{
   return aStart + (acpRandSeedAuto() % (aDbc->mFailoverServersCnt - aStart));
}

/**
 * FAILOVER_SOURCE로 쓸 문자열을 만든다.
 *
 * 형식은 "{TYPE} {IP}:{PORT}[/{DBNAME}]".
 *
 * @param[out] aBuf          문자열을 저장할 버퍼
 * @param[in]  aBufSize      버퍼 크기 (단위: Byte)
 * @param[in]  aFailoverType Failover 타입
 * @param[in]  aServerInfo   서버 정보
 *
 * @return 만든 문자열의 길이
 */
ACP_INLINE
acp_sint32_t ulnFailoverMakeSource( acp_char_t            *aBuf,
                                    acp_sint32_t           aBufSize,
                                    ulnFailoverType        aFailoverType,
                                    ulnFailoverServerInfo *aServerInfo )
{
    if (aServerInfo->mDBName == NULL)
    {
        (void) acpSnprintf(aBuf, aBufSize,
                           "%s %s:%d",
                           gFailoverTypeName[aFailoverType],
                           aServerInfo->mHost,
                           aServerInfo->mPort);
    }
    else
    {
        (void) acpSnprintf(aBuf, aBufSize,
                           "%s %s:%d/%s",
                           gFailoverTypeName[aFailoverType],
                           aServerInfo->mHost,
                           aServerInfo->mPort,
                           aServerInfo->mDBName);
    }

    return acpCStrLen(aBuf, aBufSize);
}

/**
 * 지정 서버로 접속한다.
 *
 * @param[in]  aFnContext     Function Context
 * @param[in]  aFailoverType  Failover 타입
 * @param[in]  aNewServerInfo 접속할 서버 정보
 *
 * @return 접속 성공 여부: 성공하면 ACP_TRUE, 아니면 ACP_FALSE
 */
ACI_RC ulnFailoverConnect(ulnFnContext              *aFnContext, 
                          ulnFailoverType            aFailoverType, 
                          ulnFailoverServerInfo     *aNewServerInfo,
                          ulsdFailoverResult        *aResult)
{
    ulnDbc                *sDbc           = ulnFnContextGetDbc(aFnContext);
    ACI_RC                 sRC            = ACI_FAILURE;
    acp_time_t             sLoginTimeout  = acpTimeFrom(ulnDbcGetLoginTimeout(sDbc), 0);
    acp_uint32_t           sCurRetryCnt   = 0;
    acp_uint32_t           sMaxRetryCnt   = ulnDbcGetConnectionRetryCount(sDbc);
    acp_uint32_t           sRetryDelay    = ulnDbcGetConnectionRetryDelay(sDbc);
    cmiConnectArg         *sConnectArg    = ulnDbcGetConnectArg(sDbc);
    ulnFailoverServerInfo *sOldServerInfo = ulnDbcGetCurrentServer(sDbc);
    acp_char_t             sFOSrc[ULN_MAX_FAILOVER_SOURCE_LEN + 1] = {'\0', };
    acp_sint32_t           sFOSrcLen      = 0;
    ulnFnContext           sTmpFnContext;

    switch (ulnDbcGetConnType(sDbc))
    {
        case ULN_CONNTYPE_SSL:
            sConnectArg->mSSL.mAddr = aNewServerInfo->mHost;
            sConnectArg->mSSL.mPort = aNewServerInfo->mPort;
            break;

        case ULN_CONNTYPE_TCP:
            sConnectArg->mTCP.mAddr = aNewServerInfo->mHost;
            sConnectArg->mTCP.mPort = aNewServerInfo->mPort;
            break;

        /* PROJ-2681 */
        case ULN_CONNTYPE_IB:
            sConnectArg->mIB.mAddr = aNewServerInfo->mHost;
            sConnectArg->mIB.mPort = aNewServerInfo->mPort;
            break;

        default:
            /* unreachable */
            ACE_ASSERT(0);
            break;
    }

    for (sCurRetryCnt = 0; sCurRetryCnt < sMaxRetryCnt; sCurRetryCnt++)
    {
        ulnClearDiagnosticInfoFromObject(aFnContext->mHandle.mObj);  /* BUG-46586 */

        if (cmiConnect(&(sDbc->mPtContext.mCmiPtContext),
                       sConnectArg,
                       sLoginTimeout,
                       SO_LINGER) == ACI_SUCCESS)
        {
            ulnDbcSetIsConnected(sDbc, ACP_TRUE);
            sFOSrcLen = ulnFailoverMakeSource(sFOSrc, ACI_SIZEOF(sFOSrc),
                                              aFailoverType, sOldServerInfo);
            (void)ulnDbcSetFailoverSource(sDbc, sFOSrc, sFOSrcLen);
            ulnDbcSetCurrentServer(sDbc, aNewServerInfo);

            ULN_INIT_FUNCTION_CONTEXT(sTmpFnContext, ULN_FID_CONNECT, sDbc, ULN_OBJ_TYPE_DBC);

#ifdef COMPILE_SHARDCLI
            if (ulsdModuleHandshake(&sTmpFnContext) != ACI_SUCCESS)
            {
                ulnDbcSetIsConnected(sDbc, ACP_FALSE);
                ulnClosePhysicalConn(sDbc);
                sRC = ACI_FAILURE;
                break;
            }
            else
            {
                /* Do Nothing */
            }
#endif /* COMPILE_SHARDCLI */

            if (ulnConnectCoreLogicalConn(&sTmpFnContext) == ACI_SUCCESS)
            {
                ULN_FNCONTEXT_SET_RC(aFnContext, SQL_SUCCESS);

                *aResult = ULSDN_FAILOVER_SUCCESS;
                sRC = ACI_SUCCESS;
                break;
            }
            else
            {
                ulnDbcSetIsConnected(sDbc, ACP_FALSE);
                ulnClosePhysicalConn(sDbc);
            }
        }
        else
        {
            ulnErrHandleCmError(aFnContext, NULL);

            if ( ulsdnDoFailoverAvailable( aFnContext, aResult )
                 == ACI_SUCCESS )
            {
                if ( (*aResult) == ULSDN_FAILOVER_NODE_REMOVED )
                {
                    sRC = ACI_SUCCESS;
                    break;
                }
                else
                {
                    /* Continue next retry */
                    /* Nothing to do here */
                }
            }
            else
            {
                sRC = ACI_FAILURE;
                break;
            }

            acpSleepSec(sRetryDelay);
        }
    }

    return sRC;
}

static ACI_RC ulnFailoverCore( ulnFnContext        * aFnContext,
                               ulnFailoverType       aFailoverType,
                               ulsdFailoverResult  * aResult )
{
    ulnDbc                *sDbc           = ulnFnContextGetDbc(aFnContext);
    ACI_RC                 sRC            = ACI_FAILURE;
    ulnFailoverServerInfo *sOldServerInfo = ulnDbcGetCurrentServer(sDbc);
    ulnFailoverServerInfo *sNewServerInfo = NULL;
    acp_uint32_t           sChosen;
    acp_uint32_t           i;

    ACI_TEST_RAISE(sDbc == NULL, InvalidHandleException);

    if ( sDbc->mShardDbcCxt.mShardSessionType == ULSD_SESSION_TYPE_USER )
    {
        /* BUG-46453 SessionFailOver=on, AlternateServers를 지정하지 않은 경우 체크 */
        ACI_TEST_RAISE(ulnFailoverIsOn(sDbc) != ACP_TRUE, NotSetAlternateServers);
    }

    sRC = ulsdnDoFailoverAvailable( aFnContext, aResult );
    if ( sRC == ACI_SUCCESS )
    {
        ACI_TEST_CONT( (*aResult) == ULSDN_FAILOVER_NODE_REMOVED,
                       CTF_END );
    }
    else
    {
        /* continue to ulnFailoverConnect() */
    }

    sRC = ulnFailoverConnect(aFnContext, aFailoverType, sOldServerInfo, aResult);
    ACI_TEST_CONT(sRC == ACI_SUCCESS, CTF_END);

    ACI_TEST( ulsdnDbcGetFailoverSuspendState(sDbc) == ULSDN_FAILOVER_SUSPEND_ALLOW_RETRY );

    ACI_TEST( ulnFailoverIsOn(sDbc) != ACP_TRUE );

    for (i = 0; i < sDbc->mFailoverServersCnt; i++)
    {
        if (ulnDbcGetLoadBalance(sDbc) == ACP_TRUE)
        {
            if (i > 0 && sChosen != i - 1)
            {
                sNewServerInfo                  = sDbc->mFailoverServers[ i - 1 ];
                sDbc->mFailoverServers[ i - 1 ] = sDbc->mFailoverServers[sChosen];
                sDbc->mFailoverServers[sChosen] = sNewServerInfo;
            }

            sChosen = ulnFailoverChooseRandomServer(sDbc, i);
            sNewServerInfo = sDbc->mFailoverServers[sChosen];
        }
        else
        {
            sNewServerInfo = sDbc->mFailoverServers[i];
        }

        if (sNewServerInfo == sOldServerInfo)
        {
            continue;
        }

        sRC = ulnFailoverConnect(aFnContext, aFailoverType, sNewServerInfo, aResult);
        ACI_TEST_CONT(sRC == ACI_SUCCESS, CTF_END);
    }

    ACI_EXCEPTION_CONT(CTF_END);

    return sRC;

    ACI_EXCEPTION(InvalidHandleException)
    {
        ULN_FNCONTEXT_SET_RC(aFnContext, SQL_INVALID_HANDLE);
    }
    ACI_EXCEPTION(NotSetAlternateServers)
    {
        ulnError(aFnContext, ulERR_ABORT_ALTERNATE_SERVER_NOT_SET);
    }
    ACI_EXCEPTION_END;

    return ACI_FAILURE;
}

ACP_INLINE acp_bool_t ulnFailoverIsNeeded(ulnFnContext *aFnContext)
{
    ulnDbc * sDbc = NULL;

    ULN_FNCONTEXT_GET_DBC(aFnContext, sDbc);

    /* ULSD_SESSION_TYPE_COORD, ULSD_SESSION_TYPE_LIB 인 경우
     * Connection retry 를 수행하기 위해
     * alternate server 가 지정되어 있지 않아도
     * failover 를 수행해야 한다.
     */
    if ( sDbc->mShardDbcCxt.mShardSessionType == ULSD_SESSION_TYPE_USER )
    {
        ACI_TEST(ulnFailoverIsOn(sDbc) != ACP_TRUE);
    }

    ACI_TEST(ulnDiagRecIsNeedFailover(aFnContext->mHandle.mObj) != ACP_TRUE);

    return ACP_TRUE;

    ACI_EXCEPTION_END;

    return ACP_FALSE;
}

/* BUG-46092 */
acp_bool_t ulnDiagRecIsNeedFailover(ulnObject *aObject)
{
    ulnDiagHeader * sDiagHeader = NULL;
    ulnDiagRec    * sDiagRec    = NULL;
    acp_sint32_t    sDiagCount  = 0;
    acp_sint32_t    i           = 0;

    sDiagHeader = ulnGetDiagHeaderFromObject( aObject );
    ulnDiagGetNumber( sDiagHeader, &sDiagCount );

    ACI_TEST_CONT( sDiagCount == 0,
                   FAILOVER_NEED );

    while ( ( ulnGetDiagRecFromObject( aObject, &sDiagRec, ++i ) == ACI_SUCCESS ) )
    {
        /* Failover 대상
           - CM 에러, 연결 실패 (ref. ulnErrorMgrSetCmError)
           - 예외로 ADMIN_MODE_ERROR (ref. BUG-32092) */
        switch (sDiagRec->mNativeErrorCode)
        {
            case ACI_E_ERROR_CODE(ulERR_FATAL_FAIL_TO_ESTABLISH_CONNECTION):
            case ACI_E_ERROR_CODE(ulERR_ABORT_CONNECTION_TIME_OUT):
            case ACI_E_ERROR_CODE(ulERR_ABORT_COMMUNICATION_LINK_FAILURE):
            case ACI_E_ERROR_CODE(ulERR_ABORT_GETADDRINFO_ERROR):
            case ACI_E_ERROR_CODE(ulERR_ABORT_CONNECT_INVALIDARG):
            case ACI_E_ERROR_CODE(ulERR_ABORT_CM_GENERAL_ERROR):
            case ACI_E_ERROR_CODE(mmERR_ABORT_ADMIN_MODE_ERROR):
                /* need failover */
                ACI_CONT( FAILOVER_NEED );
                break;

            default:
                /* BUGBUG (2016-12-22) 안해도 될것 같은게, ul 에러로 바꾸기 때문.
                   그래도 일단 방어 차원에서 해둔다. 어떻게 바뀔지 모르고. */
                if (ulnIsCmError(sDiagRec->mNativeErrorCode) == ACP_TRUE)
                {
                    /* need failover */
                    ACI_CONT( FAILOVER_NEED );
                }
                else
                {
                    /* no need failover */
                }
                break;
        }
    }

    return ACP_FALSE;

    ACI_EXCEPTION_CONT( FAILOVER_NEED );

    return ACP_TRUE;
}

static acp_bool_t ulnFailoverCanCTF(ulnFnContext *aFnContext)
{
    ulnDbc     * sDbc                = NULL;
    acp_bool_t   sConnectionFailover = ACP_FALSE;

    ULN_FNCONTEXT_GET_DBC(aFnContext, sDbc);

    if (ulnFailoverIsNeeded(aFnContext) == ACP_TRUE)
    {
        sConnectionFailover = ACP_TRUE;

        /* BUG-47131 샤드 All meta 환경에서 Client failover 시 hang 발생 */
        if (ulsdnDbcGetFailoverSuspendState(sDbc) == ULSDN_FAILOVER_SUSPEND_ALL )
        {
            sConnectionFailover = ACP_FALSE;
        }
        else
        {
            //nothing to do.
        }
    }
    return sConnectionFailover;
}

ACI_RC ulnFailoverDoCTF(ulnFnContext *aFnContext)
{
    ulnDbc             * sDbc    = NULL;
    ulsdFailoverResult   sResult = ULSDN_FAILOVER_FAILURE;

    ULN_FNCONTEXT_GET_DBC(aFnContext, sDbc);
    /* BUG-46052 codesonar Null Pointer Dereference */
    ACI_TEST_RAISE(sDbc == NULL, InvalidHandleException);

    ACI_TEST(ulnFailoverCanCTF(aFnContext) != ACP_TRUE);

    ACI_TEST(ulnFailoverCore(aFnContext, ULN_FAILOVER_TYPE_CTF, &sResult) != ACI_SUCCESS);

    if ( sResult == ULSDN_FAILOVER_SUCCESS )
    {
        /* Nothing to do */
    }
    else if ( sResult == ULSDN_FAILOVER_NODE_REMOVED )
    {
        ACE_DASSERT( ulnDbcIsConnected( sDbc ) == ACP_FALSE );

        ulnDbcSetIsConnected( sDbc, ACP_FALSE );
    }
    else
    {
        /* Impossible case */
        ACE_DASSERT(0);
    }

#ifdef COMPILE_SHARDCLI /* BUG-46092 */
    ACI_TEST_RAISE( ulsdModuleNotifyFailOver( aFnContext ) != ACI_SUCCESS,
                    NotifyFailException );
#endif /* COMPILE_SHARDCLI */

    return ACI_SUCCESS;

    /* BUG-46052 codesonar Null Pointer Dereference */
    ACI_EXCEPTION(InvalidHandleException)
    {
        ULN_FNCONTEXT_SET_RC(aFnContext, SQL_INVALID_HANDLE);
    }
#ifdef COMPILE_SHARDCLI /* BUG-46092 */
    ACI_EXCEPTION( NotifyFailException )
    {
        if ( ulnDbcIsConnected( sDbc ) == ACP_TRUE )
        {
            ulnDbcSetIsConnected( sDbc, ACP_FALSE );
            ulnClosePhysicalConn( sDbc );
        }
    }
#endif /* COMPILE_SHARDCLI */
    ACI_EXCEPTION_END;

    return ACI_FAILURE;
}

static acp_bool_t ulnFailoverCanSTF( ulnFnContext * aFnContext )
{
    ulnDbc     * sDbc             = NULL;
    acp_bool_t   sSessionFailover = ACP_FALSE;

    ULN_FNCONTEXT_GET_DBC(aFnContext, sDbc);

    if ( (ulnFailoverIsNeeded(aFnContext) == ACP_TRUE) &&
         (ulnDbcGetSessionFailover(sDbc) == ACP_TRUE) )
    {
        sSessionFailover = ACP_TRUE;

        if (ulnDbcGetFailoverCallbackState(sDbc) == ULN_FAILOVER_CALLBACK_IN_STATE )
        {
            sSessionFailover = ACP_FALSE;
        }
        else
        {
            //nothing to do.
        }

        /* BUG-47131 샤드 All meta 환경에서 Client failover 시 hang 발생 */
        if (ulsdnDbcGetFailoverSuspendState(sDbc) == ULSDN_FAILOVER_SUSPEND_ALL )
        {
            sSessionFailover = ACP_FALSE;
        }
        else
        {
            //nothing to do.
        }
    }
    return sSessionFailover;
}

ACI_RC ulnFailoverXaReOpen(ulnDbc *  aDbc)
{
    ACI_RC       sRC;
    acp_sint32_t sResult;

    ACI_TEST(ulxConnectionProtocol(aDbc,
                                   CMP_DB_XA_OPEN,
                                   aDbc->mXaConnection->mRmid,
                                   aDbc->mXaConnection->mOpenFlag,
                                   &sResult) != ACI_SUCCESS);
    if (sResult == XA_OK)
    {
        ulxConnSetConn(aDbc->mXaConnection);
        sRC = ACI_SUCCESS;
    }
    else
    {
        sRC = ACI_FAILURE;
    }

    return sRC;

    ACI_EXCEPTION_END;

    return ACP_FALSE;
}

ACI_RC ulnFailoverDoSTF(ulnFnContext *aFnContext)
{
    ulnDbc                      *sDbc                     = NULL;
    ACI_RC                       sRC                      = ACI_FAILURE;
    SQLUINTEGER                  sFailoverIntention       = 0;
    SQLFailOverCallbackContext  *sFailoverCallbackContext = NULL;
    SQLFailOverCallbackContext   sDummyFailoverCallbackContext;
    ulsdFailoverResult           sResult                  = ULSDN_FAILOVER_FAILURE;

    ULN_FNCONTEXT_GET_DBC(aFnContext, sDbc);
    /* BUG-46052 codesonar Null Pointer Dereference */
    ACI_TEST_RAISE(sDbc == NULL, InvalidHandleException);

    sFailoverCallbackContext = ulnDbcGetFailoverCallbackContext(sDbc);
#ifdef COMPILE_SHARDCLI /* BUG-46092 */
    if ( sDbc->mShardDbcCxt.mParentDbc != NULL )
    {
        /* Node DBC. Not allow failover callback function. */
        sFailoverCallbackContext = NULL;
    }
#endif /* COMPILE_SHARDCLI */
    if (sFailoverCallbackContext == NULL)
    {
        sDummyFailoverCallbackContext.mFailOverCallbackFunc = ulnDummyFailoverCallbackFunction;
        sDummyFailoverCallbackContext.mDBC = sDbc;
        sFailoverCallbackContext = &sDummyFailoverCallbackContext;
    }
    else
    {
        if(sFailoverCallbackContext->mDBC == NULL)
        {
            sFailoverCallbackContext->mDBC = sDbc;
        }
    }

    if (ulnFailoverCanSTF(aFnContext) == ACP_TRUE)
    {
        ulnDbcSetFailoverCallbackState(sDbc, ULN_FAILOVER_CALLBACK_IN_STATE);
        sFailoverIntention = sFailoverCallbackContext->mFailOverCallbackFunc(sFailoverCallbackContext->mDBC,
                                                                             sFailoverCallbackContext->mAppContext,
                                                                             ALTIBASE_FO_BEGIN);
        ulnDbcSetFailoverCallbackState(sDbc, ULN_FAILOVER_CALLBACK_OUT_STATE);
        if( sFailoverIntention != ALTIBASE_FO_QUIT)
        {
            sRC = ulnFailoverCore(aFnContext, ULN_FAILOVER_TYPE_STF, &sResult);
#ifdef COMPILE_SHARDCLI /* BUG-46092 */
            if ( sRC == ACI_SUCCESS )
            {
                sRC = ulsdModuleNotifyFailOver( aFnContext );
            }
#endif /* COMPILE_SHARDCLI */
            if ( sRC == ACI_SUCCESS )
            {
                if ( sResult == ULSDN_FAILOVER_SUCCESS )
                {
                    ulnDbcSetFailoverCallbackState(sDbc, ULN_FAILOVER_CALLBACK_IN_STATE);

                    // 사용자 FailOver Callback에서 FailOver Validation을 위하여
                    //  statement 를 allc할수 있다.
                    ULN_OBJECT_UNLOCK(&(sDbc->mObj), ULN_FID_NONE);

                    sFailoverIntention  = sFailoverCallbackContext->mFailOverCallbackFunc(sFailoverCallbackContext->mDBC,
                                                                                          sFailoverCallbackContext->mAppContext,
                                                                                          ALTIBASE_FO_END);

                    // 다시 잡아준다.
                    ULN_OBJECT_LOCK(&(sDbc->mObj), ULN_FID_NONE);

                    ulnDbcSetFailoverCallbackState(sDbc, ULN_FAILOVER_CALLBACK_OUT_STATE);
                    if(sFailoverIntention != ALTIBASE_FO_QUIT)
                    {
                        ulnDbcCloseAllStatement(sDbc);
                        //XA Connection.
                        if (sDbc->mXaConnection != NULL)
                        {
                            ulnDbcSetFailoverCallbackState(sDbc, ULN_FAILOVER_CALLBACK_IN_STATE);
                            sRC = ulnFailoverXaReOpen(sDbc);
                            ulnDbcSetFailoverCallbackState(sDbc, ULN_FAILOVER_CALLBACK_OUT_STATE);
                        }
                        if (sRC == ACI_SUCCESS)
                        {
                            ACE_ASSERT(ulnClearDiagnosticInfoFromObject(aFnContext->mHandle.mObj) == ACI_SUCCESS);
                        }

                    }
                    else
                    {
                        sRC = ACI_FAILURE;
                    }
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
            }
            else
            {
                //STF 가  Fail되었음을 알림.

                (void)sFailoverCallbackContext->mFailOverCallbackFunc(sFailoverCallbackContext->mDBC,
                                                                      sFailoverCallbackContext->mAppContext,
                                                                      ALTIBASE_FO_ABORT);
                sRC = ACI_FAILURE;
            }
        }
        else
        {
            sRC = ACI_FAILURE;
        }

    }
    else
    {
        sRC = ACI_FAILURE;
    }

    ACI_TEST_RAISE( sRC != ACI_SUCCESS, FailoverFailException );

    return ACI_SUCCESS;

    /* BUG-46052 codesonar Null Pointer Dereference */
    ACI_EXCEPTION(InvalidHandleException)
    {
        ULN_FNCONTEXT_SET_RC(aFnContext, SQL_INVALID_HANDLE);
    }
    ACI_EXCEPTION( FailoverFailException )
    {
        if ( ulnDbcIsConnected( sDbc ) == ACP_TRUE )
        {
            ulnDbcSetIsConnected( sDbc, ACP_FALSE );
            ulnClosePhysicalConn( sDbc );
        }
    }
    ACI_EXCEPTION_END;

    return ACI_FAILURE;
}

void ulnFailoverInitialize(ulnDbc *aDbc)
{
    aDbc->mFailoverServers    = NULL;
    aDbc->mFailoverServersCnt = 0;
    aDbc->mFailoverServersMax = 0;

    ulnDbcSetCurrentServer(aDbc, NULL);
    ulnDbcSetFailoverCallbackContext(aDbc, NULL);
    ulnDbcSetFailoverCallbackState(aDbc, ULN_FAILOVER_CALLBACK_OUT_STATE);
}

void ulnFailoverFinalize(ulnDbc *aDbc)
{
    if (aDbc->mFailoverServers != NULL)
    {
        ulnFailoverClearServerList(aDbc);
        acpMemFree(aDbc->mFailoverServers);
        aDbc->mFailoverServers = NULL;
    }
    aDbc->mFailoverServersCnt = 0;
    aDbc->mFailoverServersMax = 0;
}

/* BUG-39160 */
ACI_RC ulnFailoverCreatePrimaryServerInfo( ulnFnContext           *aFnContext,
                                           ulnFailoverServerInfo **aServerInfo )
{
    ulnDbc                *sDbc            = NULL;
    acp_char_t            *sHostName       = NULL;
    acp_sint32_t           sPortNumber     = 0;
    acp_char_t            *sPortNoEnvValue = NULL;
    ulnFailoverServerInfo *sServerInfo     = NULL;

    ULN_FNCONTEXT_GET_DBC(aFnContext, sDbc);
    /* BUG-46052 codesonar Null Pointer Dereference */
    ACI_TEST_RAISE(sDbc == NULL, InvalidHandleException);

    if (ulnDbcGetHostNameString(sDbc) == NULL)
    {
         sHostName = ulnDbcGetDsnString(sDbc);
    }
    else
    {
        sHostName = ulnDbcGetHostNameString(sDbc);
    }
    if (sHostName == NULL)
    {
        sHostName = (acp_char_t*)"localhost";
    }

    sPortNumber = ulnDbcGetPortNumber(sDbc);
    if (sPortNumber == 0)
    {
        switch (ulnDbcGetConnType(sDbc))
        {
            case ULN_CONNTYPE_INVALID:
            case ULN_CONNTYPE_TCP:
                ACI_TEST_RAISE(acpEnvGet("ALTIBASE_PORT_NO", &sPortNoEnvValue) != ACP_RC_SUCCESS,
                               LABEL_PORT_NO_NOT_SET);
                break;
            case ULN_CONNTYPE_SSL:
                ACI_TEST_RAISE(acpEnvGet("ALTIBASE_SSL_PORT_NO", &sPortNoEnvValue) != ACP_RC_SUCCESS,
                               LABEL_SSL_PORT_NO_NOT_SET);
                break;
            case ULN_CONNTYPE_IB:
                ACI_TEST_RAISE(acpEnvGet("ALTIBASE_IB_PORT_NO", &sPortNoEnvValue) != ACP_RC_SUCCESS,
                               LABEL_IB_PORT_NO_NOT_SET);
                break;
            default:
                ACE_ASSERT(0);
                break;
        }

        /*
         * 32 비트 int 의 최대값 : 4294967295 : 10자리
         */
        ACI_TEST_RAISE(ulnDrvConnStrToInt(sPortNoEnvValue,
                                          acpCStrLen(sPortNoEnvValue, 10),
                                          &sPortNumber) != ACI_SUCCESS,
                       LABEL_INVALID_PORT_NO);
    }
    else
    {
        //nothing to do
    }

    ACI_TEST_RAISE(ulnFailoverCreateServerInfo(&sServerInfo,
                                               sHostName,
                                               sPortNumber,
                                               ulnDbcGetCurrentCatalog(sDbc))
                   != ACI_SUCCESS, LABEL_MEM_MAN_ERR);

    *aServerInfo = sServerInfo;

    return ACI_SUCCESS;

    /* BUG-46052 codesonar Null Pointer Dereference */
    ACI_EXCEPTION(InvalidHandleException)
    {
        ULN_FNCONTEXT_SET_RC(aFnContext, SQL_INVALID_HANDLE);
    }
    ACI_EXCEPTION(LABEL_INVALID_PORT_NO)
    {
        ulnError(aFnContext, ulERR_ABORT_INVALID_ALTIBASE_PORT_NO, sPortNoEnvValue);
    }
    ACI_EXCEPTION(LABEL_PORT_NO_NOT_SET)
    {
        ulnError(aFnContext, ulERR_ABORT_PORT_NO_ALTIBASE_PORT_NO_NOT_SET);
    }
    ACI_EXCEPTION(LABEL_SSL_PORT_NO_NOT_SET)
    {
        ulnError(aFnContext, ulERR_ABORT_PORT_NO_ALTIBASE_SSL_PORT_NO_NOT_SET);
    }
    ACI_EXCEPTION(LABEL_IB_PORT_NO_NOT_SET)
    {
        ulnError(aFnContext, ulERR_ABORT_PORT_NO_ALTIBASE_IB_PORT_NO_NOT_SET);
    }
    ACI_EXCEPTION(LABEL_MEM_MAN_ERR)
    {
        ulnError(aFnContext, ulERR_FATAL_MEMORY_MANAGEMENT_ERROR, "ulnFailoverCreatePrimaryServerInfo");
    }
    ACI_EXCEPTION_END;

    return ACI_FAILURE;
}

ACI_RC ulnFailoverBuildServerList(ulnFnContext *aFnContext)
{
    ulnDbc                *sDbc        = NULL;
    ulnFailoverServerInfo *sServerInfo = NULL;
    acp_uint32_t           sChosen;

    ULN_FNCONTEXT_GET_DBC(aFnContext, sDbc);
    /* BUG-46052 codesonar Null Pointer Dereference */
    ACI_TEST_RAISE(sDbc == NULL, InvalidHandleException);

    ulnFailoverClearServerList(sDbc);

    /* Failover는 TCP/SSL/IB에서만 허용. (미설정일 경우, TCP로 간주) */
    ACI_TEST_CONT( (ulnDbcGetConnType(sDbc) != ULN_CONNTYPE_INVALID) &&
                   (ulnDbcGetConnType(sDbc) != ULN_CONNTYPE_TCP) &&
                   (ulnDbcGetConnType(sDbc) != ULN_CONNTYPE_IB) && /* PROJ-2681 */
                   (ulnDbcGetConnType(sDbc) != ULN_CONNTYPE_SSL),
                   SKIP_BUILD_SERVER_LIST );

    /* ULSD_SESSION_TYPE_COORD, ULSD_SESSION_TYPE_LIB 인 경우
     * Connection retry 를 수행하기 위해
     * alternate server 가 지정되어 있지 않아도
     * Failvoer Server List 를 생성한다.
     */
    if ( sDbc->mShardDbcCxt.mShardSessionType == ULSD_SESSION_TYPE_USER )
    {
        ACI_TEST_CONT( sDbc->mAlternateServers == NULL,
                       SKIP_BUILD_SERVER_LIST );
    }

    ACI_TEST( ulnFailoverCreatePrimaryServerInfo(aFnContext, &sServerInfo) != ACI_SUCCESS );
    ulnFailoverAddServer(sDbc, sServerInfo);

    if ( sDbc->mAlternateServers != NULL )
    {
        ACI_TEST( ulnFailoverAddServerList(aFnContext, sDbc->mAlternateServers) != ACI_SUCCESS );

        if (ulnDbcGetLoadBalance(sDbc) == ACP_TRUE)
        {
            sChosen = ulnFailoverChooseRandomServer(sDbc, 0);
            sServerInfo = sDbc->mFailoverServers[sChosen];
        }
    }
    ulnDbcSetCurrentServer(sDbc, sServerInfo);

    ACI_EXCEPTION_CONT( SKIP_BUILD_SERVER_LIST );

    return ACI_SUCCESS;

    /* BUG-46052 codesonar Null Pointer Dereference */
    ACI_EXCEPTION(InvalidHandleException)
    {
        ULN_FNCONTEXT_SET_RC(aFnContext, SQL_INVALID_HANDLE);
    }
    ACI_EXCEPTION_END;

    ulnFailoverClearServerList(sDbc);
    ulnDbcSetCurrentServer(sDbc, NULL);

    return ACI_FAILURE;
}

void ulnFailoverClearServerList(ulnDbc *aDbc)
{
    ulnFailoverServerInfo   *sServerInfo = NULL;
    acp_uint32_t             i;

    for (i = 0; i < aDbc->mFailoverServersCnt; i++)
    {
        sServerInfo = aDbc->mFailoverServers[i];
        ulnFailoverDestroyServerInfo(sServerInfo);
        aDbc->mFailoverServers[i] = NULL;
    }
    aDbc->mFailoverServersCnt = 0;
}

/**
 * Failover를 위한 서버 정보를 생성한다.
 *
 * @param[in] aServerInfo 생성한 서버 정보를 가리킬 변수 포인터
 * @param[in] aHost       호스트
 * @param[in] aPort       포트 번호
 * @param[in] aDBName     데이타베이스 이름
 *
 * @return 서버 정보를 생성했으면 ACI_SUCCESS, 아니면 ACI_FAILURE
 */
ACI_RC ulnFailoverCreateServerInfo( ulnFailoverServerInfo **aServerInfo,
                                    acp_char_t             *aHost,
                                    acp_uint16_t            aPort,
                                    acp_char_t             *aDBName )
{
    ulnFailoverServerInfo  *sServerInfo = NULL;
    acp_sint32_t            sHostLength = 0;
    acp_sint32_t            sDBNameLen  = 0;

    sHostLength = acpCStrLen(aHost, ACP_SINT32_MAX);
    if (aDBName == NULL)
    {
        sDBNameLen = 0;
    }
    else
    {
        sDBNameLen = acpCStrLen(aDBName, ACP_SINT32_MAX);
    }

    ACI_TEST( acpMemAlloc((void**)&sServerInfo,
                          ACI_SIZEOF(ulnFailoverServerInfo) + (sHostLength + 1) + (sDBNameLen + 1))
              != ACI_SUCCESS );

    sServerInfo->mHost = ((acp_char_t *)sServerInfo) + ACI_SIZEOF(ulnFailoverServerInfo);
    acpMemCpy(sServerInfo->mHost, aHost, sHostLength + 1);

    if (sDBNameLen > 0)
    {
        sServerInfo->mDBName = sServerInfo->mHost + sHostLength + 1;
        acpMemCpy(sServerInfo->mDBName, aDBName, sDBNameLen + 1);
        sServerInfo->mDBNameLen = sDBNameLen;
    }
    else
    {
        sServerInfo->mDBName    = NULL;
        sServerInfo->mDBNameLen = 0;
    }

    sServerInfo->mPort =  aPort;

    *aServerInfo = sServerInfo;

    return ACI_SUCCESS;

    ACI_EXCEPTION_END;

    return ACI_FAILURE;
}

// BUG-39160
void ulnFailoverDestroyServerInfo(ulnFailoverServerInfo *aServerInfo)
{
    acpMemFree(aServerInfo);
}

void ulnFailoverAddServer( ulnDbc                *aDbc,
                           ulnFailoverServerInfo *aServerInfo )
{
    if (aDbc->mFailoverServersCnt <= aDbc->mFailoverServersMax)
    {
        aDbc->mFailoverServersMax += 10;
        acpMemRealloc((void **)&aDbc->mFailoverServers, ACI_SIZEOF(ulnFailoverServerInfo*) * aDbc->mFailoverServersMax);
    }
    aDbc->mFailoverServers[aDbc->mFailoverServersCnt++] = aServerInfo;
}

/**
 * Failover를 위한 대체 서버 목록 문자열을 파싱해 목록으로 만든다.
 *
 * 대체 서버 목록 문자열의 형식은 다음과 같다:
 * ( ip:port[/dbname][, ip:port[/dbname]]* )
 *
 * @param[in] aFnContext           Function Context
 * @param[in] aAlternateServerList 대체 서버 목록 문자열
 *
 * @return 성공하면 ACI_SUCCESS, 아니면 ACI_FAILURE
 */
ACI_RC ulnFailoverAddServerList( ulnFnContext *aFnContext,
                                 acp_char_t   *aAlternateServerList )
{
    ulnDbc                 *sDbc               = NULL;
    acp_char_t             *sAltServListStrOrg = NULL;
    acp_char_t             *sAltServListStr    = NULL;
    acp_sint32_t            sAltServListStrLen = 0;
    acp_char_t             *sLBraket           = NULL;
    acp_char_t             *sRBraket           = NULL;
    acp_char_t             *sCurrAltServStr    = NULL;
    acp_char_t             *sDelim             = NULL;
    acp_char_t             *sHostStr           = NULL;
    acp_sint32_t            sHostStrLen        = 0;
    acp_char_t             *sPortStr           = NULL;
    acp_char_t             *sDBName            = NULL;
    acp_sint32_t            sPortSign          = 0;
    acp_uint32_t            sPort              = 0;
    ulnFailoverServerInfo  *sServerInfo        = NULL;

    ACE_DASSERT( aAlternateServerList != NULL );

    ULN_FNCONTEXT_GET_DBC(aFnContext, sDbc);
    /* BUG-46052 codesonar Null Pointer Dereference */
    ACI_TEST_RAISE(sDbc == NULL, InvalidHandleException);

    sAltServListStrLen = acpCStrLen(aAlternateServerList, ACP_SINT32_MAX);
    ACI_TEST_RAISE( acpMemAlloc((void**)&sAltServListStrOrg, sAltServListStrLen + 1)
                    != ACI_SUCCESS, LABEL_MEM_MAN_ERR );
    
    /* BUG-46599 sAltServListStr는 포인터가 이동된다. */
    sAltServListStr = sAltServListStrOrg;

    acpMemCpy( sAltServListStr,
               aAlternateServerList,
               sAltServListStrLen + 1 );
    sAltServListStr = urlCleanKeyWord(sAltServListStr);

    sLBraket = ulnStrChr(sAltServListStr, '(');
    /* bug-31375: AlternateServers string syntax error */
    ACI_TEST_RAISE(sLBraket == NULL, INVALID_ALTSERV_LIST_FORMAT_ERROR);

    sRBraket = ulnStrRchr(sAltServListStr, ')');
    ACI_TEST_RAISE(sRBraket == NULL, INVALID_ALTSERV_LIST_FORMAT_ERROR);
    ACI_TEST_RAISE(sRBraket < sLBraket, INVALID_ALTSERV_LIST_FORMAT_ERROR);

    *sRBraket = '\0';
    sAltServListStr = sLBraket + 1;

    //192.168.3.54:20300,192.168.3.55:20301
    while ( (sCurrAltServStr = ulnStrSep(&sAltServListStr, ",")) != NULL )
    {
        if (sCurrAltServStr[0] == '\0')
        {
            break;
        }

        sDelim = ulnStrRchr(sCurrAltServStr, '/');
        if (sDelim == NULL)
        {
            sDBName = NULL;
        }
        else
        {
            *sDelim = '\0';
            sDBName = sDelim + 1;
        }

        /* proj-1538 ipv6: separate port from ip addr.
         * get the last colon char regarding IPv6.
         * ex) [::1]:20300 -> [::1](sHostStr)+20300(sPortStr) */
        sDelim = ulnStrRchr(sCurrAltServStr, ':');
        ACI_TEST_RAISE(sDelim == NULL || sDelim < ulnStrRchr(sCurrAltServStr, ']'),
                       INVALID_ALTSERV_FORMAT_ERROR);

        /* get host and port str */
        sHostStr = sCurrAltServStr;
        *sDelim = '\0';
        sPortStr = sDelim + 1;

        sHostStr = urlCleanValue(sHostStr);
        sHostStrLen = acpCStrLen(sHostStr, ACP_SINT32_MAX);
        ACI_TEST_RAISE((sHostStr[0] == '[' && sHostStr[sHostStrLen - 1] != ']') ||
                       (sHostStr[0] != '[' && sHostStr[sHostStrLen - 1] == ']'),
                       INVALID_ALTSERV_HOST_ERROR);
        sPortStr = urlCleanValue(sPortStr);

        /* proj-1538 ipv6: remove "[]" ex) [::1] -> ::1 */
        if ( ulnParseIsBracketedAddress(sHostStr, sHostStrLen) == ACP_TRUE )
        {
            sHostStr++;
            sHostStrLen -= 2;
            /* BUG-41366 The stripping of IPV6 address is not correct. */
            sHostStr[sHostStrLen] = '\0';
        }

        ACI_TEST(sPortStr == NULL);
        ACI_TEST_RAISE( acpCStrToInt32(sPortStr,
                                       acpCStrLen(sPortStr, ACP_SINT32_MAX),
                                       &sPortSign,
                                       &sPort,
                                       10,
                                       NULL)
                        != ACI_SUCCESS,
                        INVALID_ALTSERV_PORT_ERROR );
        ACI_TEST(sPortSign < 0 || sPort == 0 || sPort > ACP_UINT16_MAX);

        ACI_TEST_RAISE( ulnFailoverCreateServerInfo(&sServerInfo,
                                                    sHostStr,
                                                    sPort,
                                                    sDBName)
                        != ACI_SUCCESS,
                        LABEL_MEM_MAN_ERR );

        ulnFailoverAddServer(sDbc, sServerInfo);
    }

    acpMemFree(sAltServListStrOrg);

    return ACI_SUCCESS;

    /* BUG-46052 codesonar Null Pointer Dereference */
    ACI_EXCEPTION(InvalidHandleException)
    {
        ULN_FNCONTEXT_SET_RC(aFnContext, SQL_INVALID_HANDLE);
    }
    ACI_EXCEPTION(LABEL_MEM_MAN_ERR)
    {
        ulnError(aFnContext, ulERR_FATAL_MEMORY_MANAGEMENT_ERROR, "ulnFailoverAddServerList");
    }
    ACI_EXCEPTION(INVALID_ALTSERV_LIST_FORMAT_ERROR)
    {
        ulnError(aFnContext, ulERR_ABORT_INVALID_ALTERNATE_SERVER_FORMAT, sAltServListStr);
    }
    ACI_EXCEPTION(INVALID_ALTSERV_FORMAT_ERROR)
    {
        ulnError(aFnContext, ulERR_ABORT_INVALID_ALTERNATE_SERVER_FORMAT, sCurrAltServStr);
    }
    ACI_EXCEPTION(INVALID_ALTSERV_HOST_ERROR)
    {
        ulnError(aFnContext, ulERR_ABORT_INVALID_ALTERNATE_SERVER_HOST, sHostStr);
    }
    ACI_EXCEPTION(INVALID_ALTSERV_PORT_ERROR)
    {
        ulnError(aFnContext, ulERR_ABORT_INVALID_ALTERNATE_SERVER_PORT, sPortStr);
    }
    ACI_EXCEPTION_END;

    if (sAltServListStrOrg != NULL)
    {
        acpMemFree(sAltServListStrOrg);
    }

    return ACI_FAILURE;
}

SQLUINTEGER ulnDummyFailoverCallbackFunction( SQLHDBC      aDBC,
                                              void        *aAppContext,
                                              SQLUINTEGER  aFailoverEvent)
{
    ulnFnContext   sFnContext;

    ACP_UNUSED(aAppContext);

    ULN_INIT_FUNCTION_CONTEXT(sFnContext, ULN_FID_NONE, aDBC, ULN_OBJ_TYPE_DBC);

    switch (aFailoverEvent)
    {
        case ALTIBASE_FO_BEGIN:
            ULN_TRACE_LOG(&sFnContext, ULN_TRACELOG_LOW, NULL, 0,
                          "%-18s| Failover Begin AT SQLHDBC 0x%x ",
                          "ulnDummyFailoverCallbackFunction", aDBC);
            break;

        case ALTIBASE_FO_END:
            ULN_TRACE_LOG(&sFnContext, ULN_TRACELOG_LOW, NULL, 0,
                          "%-18s| Failover Successfully END !! AT SQLHDBC 0x%x",
                          "ulnDummyFailoverCallbackFunction", aDBC);
            break;

        case ALTIBASE_FO_ABORT:
            ULN_TRACE_LOG(&sFnContext, ULN_TRACELOG_LOW, NULL, 0,
                          "%-18s| Failover Failed AT SQLHDBC 0x%x",
                          "ulnDummyFailoverCallbackFunction", aDBC);
            break;

        default:
            break;
    }

    return ALTIBASE_FO_GO;
}
