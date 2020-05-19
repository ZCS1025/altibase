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
 * $Id$ sdl.cpp
 **********************************************************************/

#include <idl.h>
#include <sdl.h>
#include <sdlStatement.h>
#include <sdSql.h>
#include <qci.h>
#include <qdk.h>
#include <mtl.h>
#include <idu.h>
#include <dlfcn.h>
#include <sdlDataTypes.h>
#include <cmnDef.h>

#if defined(HP_HPUX) || defined(IA64_HP_HPUX)
#define LIB_ODBCCLI_NAME "libodbccli_sl.sl"
#else
#define LIB_ODBCCLI_NAME "libodbccli_sl.so"
#endif

#define SDL_CONN_STR_MAX_LEN     (512)
#define SDL_CONN_STR             "SERVER=%s;PORT_NO=%"ID_INT32_FMT";CONNTYPE=%d;NLS_USE=%s;UID=%s;PWD=%s;APP_INFO=shard_meta;SHARD_NODE_NAME=%s;SHARD_LINKER_TYPE=0;AUTOCOMMIT=%s;EXPLAIN_PLAN=%s;SHARD_SESSION_TYPE=1"
#define SDL_CONN_ALTERNATE_STR   SDL_CONN_STR";ALTERNATE_SERVERS=(%s:%"ID_INT32_FMT");SessionFailOver=on"

LIBRARY_HANDLE sdl::mOdbcLibHandle = NULL;
idBool         sdl::mInitialized = ID_FALSE;

static ODBCDriverConnect                  SQLDriverConnect        = NULL; // 01
static ODBCAllocHandle                    SQLAllocHandle          = NULL; // 02
static ODBCFreeHandle                     SQLFreeHandle           = NULL; // 03
static ODBCExecDirect                     SQLExecDirect           = NULL; // 04
static ODBCPrepare                        SQLPrepare              = NULL; // 05
static ODBCExecute                        SQLExecute              = NULL; // 06
static ODBCFetch                          SQLFetch                = NULL; // 07
static ODBCDisconnect                     SQLDisconnect           = NULL; // 08
static ODBCFreeConnect                    SQLFreeConnect          = NULL; // 09
static ODBCFreeStmt                       SQLFreeStmt             = NULL; // 10
static ODBCAllocEnv                       SQLAllocEnv             = NULL; // 11
static ODBCAllocConnect                   SQLAllocConnect         = NULL; // 12
static ODBCExecuteForMtDataRows           SQLExecuteForMtDataRows = NULL; // 13
static ODBCGetEnvHandle                   SQLGetEnvHandle         = NULL; // 14
static ODBCGetDiagRec                     SQLGetDiagRec           = NULL; // 15
static ODBCSetConnectOption               SQLSetConnectOption     = NULL; // 16
static ODBCGetConnectOption               SQLGetConnectOption     = NULL; // 17
static ODBCEndTran                        SQLEndTran              = NULL; // 18
static ODBCTransact                       SQLTransact             = NULL; // 19
static ODBCRowCount                       SQLRowCount             = NULL; // 20
static ODBCGetPlan                        SQLGetPlan              = NULL; // 21
static ODBCDescribeCol                    SQLDescribeCol          = NULL; // 22
static ODBCDescribeColEx                  SQLDescribeColEx        = NULL; // 23
static ODBCBindParameter                  SQLBindParameter        = NULL; // 24
static ODBCGetConnectAttr                 SQLGetConnectAttr       = NULL; // 25
static ODBCSetConnectAttr                 SQLSetConnectAttr       = NULL; // 26
static ODBCGetDbcShardTargetDataNodeName  SQLGetDbcShardTargetDataNodeName  = NULL;// 31
static ODBCGetStmtShardTargetDataNodeName SQLGetStmtShardTargetDataNodeName = NULL;// 32
static ODBCSetDbcShardTargetDataNodeName  SQLSetDbcShardTargetDataNodeName  = NULL;// 33
static ODBCSetStmtShardTargetDataNodeName SQLSetStmtShardTargetDataNodeName = NULL;// 34
static ODBCGetDbcLinkInfo                 SQLGetDbcLinkInfo                 = NULL;// 35
static ODBCGetParameterCount              SQLGetParameterCount              = NULL;// 36
static ODBCDisconnectLocal                SQLDisconnectLocal      = NULL; // 37
static ODBCSetShardPin                    SQLSetShardPin          = NULL; // 38
static ODBCEndPendingTran                 SQLEndPendingTran       = NULL; // 39
static ODBCPrepareAddCallback             SQLPrepareAddCallback   = NULL; // 40
static ODBCExecuteForMtDataRowsAddCallback SQLExecuteForMtDataRowsAddCallback = NULL; // 41
static ODBCExecuteForMtDataAddCallback    SQLExecuteForMtDataAddCallback = NULL; // 42
static ODBCPrepareTranAddCallback         SQLPrepareTranAddCallback = NULL;  // 43
static ODBCEndTranAddCallback             SQLEndTranAddCallback   = NULL; // 44
static ODBCDoCallback                     SQLDoCallback           = NULL; // 45
static ODBCGetResultCallback              SQLGetResultCallback    = NULL; // 46
static ODBCRemoveCallback                 SQLRemoveCallback       = NULL; // 47
static ODBCSetShardMetaNumber             SQLSetShardMetaNumber   = NULL; // 48
static ODBCGetSMNOfDataNode               SQLGetSMNOfDataNode     = NULL; // 49
static ODBCGetNeedFailover                SQLGetNeedFailover      = NULL; // 50
static ODBCReconnect                      SQLReconnect            = NULL; // 51
static ODBCGetNeedToDisconnect            SQLGetNeedToDisconnect  = NULL; // 52
static ODBCSetSavepoint                   SQLSetSavepoint         = NULL; // 53
static ODBCRollbackToSavepoint            SQLRollbackToSavepoint  = NULL; // 54
static ODBCShardStmtPartialRollback       SQLShardStmtPartialRollback = NULL; // 55

#define IS_SUCCEEDED( aRET ) {\
    ( ( aRET == SQL_SUCCESS ) || ( aRET == SQL_SUCCESS_WITH_INFO ) || ( aRet == SQL_NO_DATA ) )

#define LOAD_LIBRARY(aName, aHandle)\
        {\
            aHandle = idlOS::dlopen( aName, RTLD_LAZY|RTLD_GLOBAL );\
            IDE_TEST_RAISE( aHandle == NULL, errInitOdbcLibrary );\
        }

#define ODBC_FUNC_DEF( aType )\
        {\
            SQL##aType = (ODBC##aType)idlOS::dlsym( sdl::mOdbcLibHandle, STR_SQL##aType );\
            IDE_TEST_RAISE( SQL##aType == NULL, errInitOdbcLibrary );\
        }

#define IDE_EXCEPTION_UNINIT_LIBRARY( aConnectionInfo, aFunctionName )\
        IDE_EXCEPTION( UnInitializedError )\
        {\
            IDE_SET( ideSetErrorCode( sdERR_ABORT_UNINITIALIZED_LIBRARY, \
                                      ( (aConnectionInfo == NULL) ? (SChar*)"" : \
                                                                    (SChar*)aConnectionInfo->mNodeName ), \
                                      aFunctionName ) );\
        }

#define IDE_EXCEPTION_NULL_DBC( aConnectionInfo, aFunctionName )\
        IDE_EXCEPTION( ErrorNullDbc )\
        {\
            IDE_SET( ideSetErrorCode( sdERR_ABORT_EXECUTE_NULL_DBC, \
                                      ( (aConnectionInfo == NULL) ? (SChar*)"" : \
                                                                    (SChar*)aConnectionInfo->mNodeName ), \
                                      aFunctionName ) );\
        }

#define IDE_EXCEPTION_NULL_STMT( aConnectionInfo, aFunctionName )\
        IDE_EXCEPTION( ErrorNullStmt )\
        {\
            IDE_SET( ideSetErrorCode( sdERR_ABORT_EXECUTE_NULL_STMT, \
                                      ( (aConnectionInfo == NULL) ? (SChar*)"" : \
                                                                    (SChar*)aConnectionInfo->mNodeName ), \
                                      aFunctionName ) );\
        }

#define IDE_EXCEPTION_NULL_SD_STMT( aConnectionInfo, aFunctionName )\
        IDE_EXCEPTION( ErrorNullSdStmt )\
        {\
            IDE_SET( ideSetErrorCode( sdERR_ABORT_EXECUTE_NULL_SD_STMT, \
                                      ( (aConnectionInfo == NULL) ? (SChar*)"" : \
                                                                    (SChar*)aConnectionInfo->mNodeName ), \
                                      aFunctionName ) );\
        }

#define SET_LINK_FAILURE_FLAG( aFlagPtr, aValue )\
        if ( aFlagPtr != NULL ){ *aFlagPtr = aValue; }\
        else { /* Do Nothing. */ }

IDE_RC sdl::loadLibrary()
{
/***************************************************************
 * Description :
 *
 * Implementation : load odbc library.
 *
 * Arguments
 *
 * Return value
 *
 * IDE_SUCCESS/IDE_FAILURE
 *
 ***************************************************************/
    SChar sErrMsg[256];

    /* Load odbccli_sl Library */
    LOAD_LIBRARY(LIB_ODBCCLI_NAME, sdl::mOdbcLibHandle);

    /* SQLAllocHandle */
    ODBC_FUNC_DEF( AllocHandle );
    /* SQLFreeHandle */
    ODBC_FUNC_DEF( FreeHandle );
    /* SQLDriverConnect */
    ODBC_FUNC_DEF( DriverConnect );
    /* SQLExecDirect */
    ODBC_FUNC_DEF( ExecDirect );
    /* SQLPrepare */
    ODBC_FUNC_DEF( Prepare );
    /* SQLExecute */
    ODBC_FUNC_DEF( Execute );
    /* SQLFetch */
    ODBC_FUNC_DEF( Fetch );
    /* SQLDisconnect */
    ODBC_FUNC_DEF( Disconnect );
    /* SQLFreeConnect */
    ODBC_FUNC_DEF( FreeConnect );
    /* SQLFreeStmt */
    ODBC_FUNC_DEF( FreeStmt );
    /* SQLAllocEnv */
    ODBC_FUNC_DEF( AllocEnv );
    /* SQLAllocConnect */
    ODBC_FUNC_DEF( AllocConnect );
    /* SQLExecuteForMtDataRows */
    ODBC_FUNC_DEF( ExecuteForMtDataRows );
    /* SQLGetEnvHandle */
    ODBC_FUNC_DEF( GetEnvHandle );
    /* SQLGetDiagRec */
    ODBC_FUNC_DEF( GetDiagRec );
    /* SQLSetConnectOption */
    ODBC_FUNC_DEF( SetConnectOption );
    /* SQLGetConnectOption */
    ODBC_FUNC_DEF( GetConnectOption );
    /* SQLEndTran */
    ODBC_FUNC_DEF( EndTran );
    /* SQLTransact */
    ODBC_FUNC_DEF( Transact );
    /* SQLRowCount */
    ODBC_FUNC_DEF( RowCount );
    /* SQLGetPlan */
    ODBC_FUNC_DEF( GetPlan );
    /* SQLDescribeCol */
    ODBC_FUNC_DEF( DescribeCol );
    /* SQLDescribeColEx */
    ODBC_FUNC_DEF( DescribeColEx );
    /* SQLBindParameter */
    ODBC_FUNC_DEF( BindParameter );
    /* SQLGetConnectAttr */
    ODBC_FUNC_DEF( GetConnectAttr );
    /* SQLSetConnectAttr */
    ODBC_FUNC_DEF( SetConnectAttr );
    /* SQLGetDbcShardTargetDataNodeName */
    ODBC_FUNC_DEF( GetDbcShardTargetDataNodeName );
    /* SQLGetStmtShardTargetDataNodeName */
    ODBC_FUNC_DEF( GetStmtShardTargetDataNodeName );
    /* SQLSetDbcShardTargetDataNodeName */
    ODBC_FUNC_DEF( SetDbcShardTargetDataNodeName );
    /* SQLSetStmtShardTargetDataNodeName */
    ODBC_FUNC_DEF( SetStmtShardTargetDataNodeName );
    /* SQLGetDbcLinkInfo */
    ODBC_FUNC_DEF( GetDbcLinkInfo );
    /* SQLGetParameterCount */
    ODBC_FUNC_DEF( GetParameterCount );
    /* SQLDisconnectLocal */
    ODBC_FUNC_DEF( DisconnectLocal );
    /* SQLSetShardPin */
    ODBC_FUNC_DEF( SetShardPin );
    /* SQLEndPendingTran */
    ODBC_FUNC_DEF( EndPendingTran );
    /* SQLPrepareAddCallback */
    ODBC_FUNC_DEF( PrepareAddCallback );
    /* SQLExecuteForMtDataRowsAddCallback */
    ODBC_FUNC_DEF( ExecuteForMtDataRowsAddCallback );
    /* SQLExecuteAddCallback */
    ODBC_FUNC_DEF( ExecuteForMtDataAddCallback );
    /* SQLPrepareTranAddCallback */
    ODBC_FUNC_DEF( PrepareTranAddCallback );
    /* SQLEndTranAddCallback */
    ODBC_FUNC_DEF( EndTranAddCallback );
    /* SQLDoCallback */
    ODBC_FUNC_DEF( DoCallback );
    /* SQLGetResultCallback */
    ODBC_FUNC_DEF( GetResultCallback );
    /* SQLRemoveCallback */
    ODBC_FUNC_DEF( RemoveCallback );
    /* SQLSetShardMetaNumber */
    ODBC_FUNC_DEF( SetShardMetaNumber );
    /* SQLGetSMNOfDataNode */
    ODBC_FUNC_DEF( GetSMNOfDataNode );
    /* SQLGetNeedFailover */
    ODBC_FUNC_DEF( GetNeedFailover );
    /* SQLReconnect */
    ODBC_FUNC_DEF( Reconnect );
    /* SQLGetNeedToDisconnect */
    ODBC_FUNC_DEF( GetNeedToDisconnect );
    /* SQLSetSavepoint */
    ODBC_FUNC_DEF( SetSavepoint );
    /* SQLRollbackToSavepoint */
    ODBC_FUNC_DEF( RollbackToSavepoint );
    /* SQLShardStmtPartialRollback */
    ODBC_FUNC_DEF( ShardStmtPartialRollback );

    return IDE_SUCCESS;

    IDE_EXCEPTION( errInitOdbcLibrary )
    {
        idlOS::snprintf( sErrMsg, 256, "%s", idlOS::dlerror() );
        IDE_SET( ideSetErrorCode( sdERR_ABORT_INIT_SDL_ODBCCLI, sErrMsg ) );
        ideLog::log( IDE_SD_0, "[SHARD SDL : FAILURE] error = %s\n", sErrMsg );
    }
    IDE_EXCEPTION_END;

    if ( sdl::mOdbcLibHandle != NULL )
    {
        idlOS::dlclose( sdl::mOdbcLibHandle );
        sdl::mOdbcLibHandle = NULL;
    }

    return IDE_FAILURE;
}

IDE_RC sdl::initOdbcLibrary()
{
/***************************************************************
 * Description :
 *
 * Implementation : initialize sdl.
 *
 * Arguments
 *
 * Return value
 *
 * Error handle
 *
 ***************************************************************/

    IDE_TEST_CONT( mInitialized == ID_TRUE, skipIniOdbcLibrary );

    IDE_TEST( loadLibrary() != IDE_SUCCESS );

    mInitialized = ID_TRUE;

    IDE_EXCEPTION_CONT( skipIniOdbcLibrary );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    mInitialized = ID_FALSE;

    return IDE_FAILURE;
}

void sdl::finiOdbcLibrary()
{
/***************************************************************
 * Description :
 *
 * Implementation : finalize sdl.
 *
 * Arguments
 *
 * Return value
 *
 * Error handle
 *
 ***************************************************************/
    mInitialized = ID_FALSE;

    // release odbc library handle
    if ( sdl::mOdbcLibHandle != NULL )
    {
        idlOS::dlclose( sdl::mOdbcLibHandle );
        sdl::mOdbcLibHandle = NULL;
    }
    else
    {
        /*Do Nothing.*/
    }
}

IDE_RC sdl::allocConnect( sdiConnectInfo * aConnectInfo )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation : alloc dbc and connect to data node.
 *
 * Arguments
 *     - aUserName      : data node 에 대한 connect user name
 *     - aPassword      : data node 에 대한 connect password
 *     - aConnectType   : data node 에 대한 connect type
 *     - aConnectInfo   : data node의 접속 결과
 *           mNodeId
 *           mNodeName
 *           mServerIP
 *           mPortNo
 *           mDbc
 *           mLinkFailure : data node 에 대한 link failure
 *
 * Return value
 *     - aConnectInfo->mDbc : data node 에 대한 connection
 *
 * Error handle
 *     - SQL_SUCCESS
 *     - SQL_SUCCESS_WITH_INFO
 *     - SQL_NO_DATA_FOUND
 *     - SQL_INVALID_HANDLE
 *     - SQL_ERROR
 *
 ***********************************************************************/

    IDE_RC            sIdeReturn = IDE_FAILURE;
    SQLRETURN         sSqlReturn = SQL_ERROR;
    SQLHDBC           sDbc;
    SQLHENV           sEnv;
    SQLCHAR           sConnInStr[SDL_CONN_STR_MAX_LEN];
    UInt              sConnAttrValue;
    SLong             sS64;
    UInt              sState  = 0;
    SChar             sDBCharSet[MTL_NAME_LEN_MAX + 1];
    const mtlModule * sLanguage = mtl::mDBCharSet;

    sdiNode         * sDataNode = &(aConnectInfo->mNodeInfo);
    SChar             sPassword[IDS_MAX_PASSWORD_LEN + 1];
    UInt              sLen;
    idBool            sIsShardClient = ID_FALSE;

    IDE_TEST_RAISE( mInitialized == ID_FALSE, UnInitializedError );

    SET_LINK_FAILURE_FLAG( &aConnectInfo->mLinkFailure, ID_FALSE );

    // database char set
    idlOS::strncpy( sDBCharSet,
                    (SChar *)sLanguage->names->string,
                    sLanguage->names->length );
    sDBCharSet[sLanguage->names->length] = '\0';

    // password - simple scramble
    sLen = idlOS::strlen( aConnectInfo->mUserPassword );
    idlOS::memcpy( sPassword, aConnectInfo->mUserPassword, sLen + 1 );
    sdi::charXOR( sPassword, sLen );

    sSqlReturn = SQLAllocEnv( &sEnv ) ;
    IDE_TEST_RAISE( sSqlReturn != SQL_SUCCESS, AllocEnvError );
    sState = 1;

    sSqlReturn = SQLAllocConnect( sEnv, &sDbc );
    IDE_TEST_RAISE( sSqlReturn != SQL_SUCCESS, AllocDBCError );
    sState = 2;

    if ( aConnectInfo->mMessageCallback.mFunction != NULL )
    {
        sSqlReturn = SQLSetConnectAttr( sDbc, SDL_ALTIBASE_MESSAGE_CALLBACK,
                                        &(aConnectInfo->mMessageCallback), 0 );
        IDE_TEST_RAISE( sSqlReturn != SQL_SUCCESS, SetConnectAttrError );
    }
    else
    {
        // Nothing to do
    }

    sConnAttrValue = sdi::getShardInternalConnAttrRetryCount();
    if ( sConnAttrValue != SDI_ODBC_CONN_ATTR_RETRY_COUNT_DEFAULT )
    {
        sS64 = (SLong)sConnAttrValue;
        sSqlReturn = SQLSetConnectAttr( sDbc,
                                        SDL_ALTIBASE_CONNECTION_RETRY_COUNT,
                                        (SQLPOINTER)sS64,
                                        0 );
        IDE_TEST_RAISE( sSqlReturn != SQL_SUCCESS, SetConnectAttrError_RETRY_COUNT );
    }

    sConnAttrValue = sdi::getShardInternalConnAttrRetryDelay();
    if ( sConnAttrValue != SDI_ODBC_CONN_ATTR_RETRY_DELAY_DEFAULT )
    {
        sS64 = (SLong)sConnAttrValue;
        sSqlReturn = SQLSetConnectAttr( sDbc,
                                        SDL_ALTIBASE_CONNECTION_RETRY_DELAY,
                                        (SQLPOINTER)sS64,
                                        0 );
        IDE_TEST_RAISE( sSqlReturn != SQL_SUCCESS, SetConnectAttrError_RETRY_DELAY );
    }

    sConnAttrValue = sdi::getShardInternalConnAttrConnTimeout();
    if ( sConnAttrValue != SDI_ODBC_CONN_ATTR_CONN_TIMEOUT_DEFAULT )
    {
        sS64 = (SLong)sConnAttrValue;
        sSqlReturn = SQLSetConnectAttr( sDbc,
                                        SDL_SQL_ATTR_CONNECTION_TIMEOUT,
                                        (SQLPOINTER)sS64,
                                        0 );
        IDE_TEST_RAISE( sSqlReturn != SQL_SUCCESS, SetConnectAttrError_CONN_TIMEOUT );
    }

    sConnAttrValue = sdi::getShardInternalConnAttrLoginTimeout();
    if ( sConnAttrValue != SDI_ODBC_CONN_ATTR_LOGIN_TIMEOUT_DEFAULT )
    {
        sS64 = (SLong)sConnAttrValue;
        sSqlReturn = SQLSetConnectAttr( sDbc,
                                        SDL_SQL_ATTR_LOGIN_TIMEOUT,
                                        (SQLPOINTER)sS64,
                                        0 );
        IDE_TEST_RAISE( sSqlReturn != SQL_SUCCESS, SetConnectAttrError_LOGIN_TIMEOUT );
    }

    if ( ( aConnectInfo->mFlag & SDI_CONNECT_INITIAL_BY_NOTIFIER_MASK )
           == SDI_CONNECT_INITIAL_BY_NOTIFIER_FALSE )
    
    {
        if ( aConnectInfo->mSession != NULL )
        {
            if ( aConnectInfo->mIsShardClient == SDI_SHARD_CLIENT_TRUE )
            {
                /* shardcli is used. */
                sIsShardClient = ID_TRUE;

                aConnectInfo->mFailoverTarget
                    = (sdiFailOverTarget)qci::mSessionCallback.mGetShardFailoverType(
                            aConnectInfo->mSession->mMmSession,
                            sDataNode->mNodeId );
            }
            else
            {
                /* ODBC cli is used. */
                aConnectInfo->mFailoverTarget = SDI_FAILOVER_ALL;
            }
        }
        else
        {
            /* exception case. */
            IDE_DASSERT( 0 );
            aConnectInfo->mFailoverTarget = SDI_FAILOVER_ALL;
        }
    }
    else
    {
        /* dktNotifier::notifyXaResultForShard */
        aConnectInfo->mFailoverTarget = SDI_FAILOVER_ACTIVE_ONLY;
    }


    if ( hasNodeAlternate( sDataNode ) == ID_TRUE )
    {
        switch ( aConnectInfo->mFailoverTarget )
        {
            case SDI_FAILOVER_ACTIVE_ONLY:
                idlOS::snprintf( (SChar*)sConnInStr,
                                 ID_SIZEOF(sConnInStr),
                                 SDL_CONN_STR,
                                 sDataNode->mServerIP,
                                 (SInt)sDataNode->mPortNo,
                                 aConnectInfo->mConnectType,
                                 sDBCharSet,
                                 aConnectInfo->mUserName,
                                 sPassword,
                                 sDataNode->mNodeName,
                                 ( sdi::isMetaAutoCommit( aConnectInfo ) == ID_TRUE ? "ON" : "OFF" ),
                                 ( aConnectInfo->mPlanAttr > 0 ? "ON" : "OFF" ) );
                break;

            case SDI_FAILOVER_ALTERNATE_ONLY:
                idlOS::snprintf( (SChar*)sConnInStr,
                                 ID_SIZEOF(sConnInStr),
                                 SDL_CONN_STR,
                                 sDataNode->mAlternateServerIP,
                                 (SInt)sDataNode->mAlternatePortNo,
                                 aConnectInfo->mConnectType,
                                 sDBCharSet,
                                 aConnectInfo->mUserName,
                                 sPassword,
                                 sDataNode->mNodeName,
                                 ( sdi::isMetaAutoCommit( aConnectInfo ) == ID_TRUE ? "ON" : "OFF" ),
                                 ( aConnectInfo->mPlanAttr > 0 ? "ON" : "OFF" ) );
                break;

            case SDI_FAILOVER_ALL:
            /* fall through */
            default:
                idlOS::snprintf( (SChar*)sConnInStr,
                                 ID_SIZEOF(sConnInStr),
                                 SDL_CONN_ALTERNATE_STR,
                                 sDataNode->mServerIP,
                                 (SInt)sDataNode->mPortNo,
                                 aConnectInfo->mConnectType,
                                 sDBCharSet,
                                 aConnectInfo->mUserName,
                                 sPassword,
                                 sDataNode->mNodeName,
                                 ( sdi::isMetaAutoCommit( aConnectInfo ) == ID_TRUE ? "ON" : "OFF" ),
                                 ( aConnectInfo->mPlanAttr > 0 ? "ON" : "OFF" ),
                                 sDataNode->mAlternateServerIP,
                                 (SInt)sDataNode->mAlternatePortNo );
                break;
        }
    }
    else
    {
        IDE_TEST_RAISE( aConnectInfo->mFailoverTarget == SDI_FAILOVER_ALTERNATE_ONLY,
                        NodeAlternateSettingMismatch );

        idlOS::snprintf( (SChar*)sConnInStr,
                         ID_SIZEOF(sConnInStr),
                         SDL_CONN_STR,
                         sDataNode->mServerIP,
                         (SInt)sDataNode->mPortNo,
                         aConnectInfo->mConnectType,
                         sDBCharSet,
                         aConnectInfo->mUserName,
                         sPassword,
                         sDataNode->mNodeName,
                         ( sdi::isMetaAutoCommit( aConnectInfo ) == ID_TRUE ? "ON" : "OFF" ),
                         ( aConnectInfo->mPlanAttr > 0 ? "ON" : "OFF" ) );
    }

    SQLSetShardPin( sDbc, aConnectInfo->mShardPin );
    SQLSetShardMetaNumber( sDbc, aConnectInfo->mShardMetaNumber );


    sIdeReturn = internalConnect( sDbc,
                                  aConnectInfo,
                                  sConnInStr,
                                  sIsShardClient );

    IDE_TEST( sIdeReturn != IDE_SUCCESS );
    sState = 3;

    //------------------------------------
    // return connection info
    //------------------------------------

    aConnectInfo->mDbc = sDbc;

    IDE_TEST( getConnectedLinkFullAddress( aConnectInfo ) );

    return IDE_SUCCESS;

    IDE_EXCEPTION( UnInitializedError )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_UNINITIALIZED_LIBRARY,
                                  sDataNode->mNodeName,
                                  "allocConnect" ) );
    }
    IDE_EXCEPTION( AllocEnvError )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SHARD_LIBRARY_ERROR,
                                  sDataNode->mNodeName,
                                  "SQLAllocEnv" ) );
    }
    IDE_EXCEPTION( AllocDBCError )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SHARD_LIBRARY_ERROR,
                                  sDataNode->mNodeName,
                                  "SQLAllocConnect" ) );
    }
    IDE_EXCEPTION( NodeAlternateSettingMismatch )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_INTERNAL_ALTERNATE_NODE_SETTING_IS_MISSING,
                                  sDataNode->mNodeName ) );
    }
    IDE_EXCEPTION( SetConnectAttrError )
    {
        processError( SQL_HANDLE_DBC,
                      sDbc,
                      ( (SChar*)"SQLSetConnectAttr" ),
                      aConnectInfo,
                      &(aConnectInfo->mLinkFailure) );
    }
    IDE_EXCEPTION( SetConnectAttrError_RETRY_COUNT )
    {
        processError( SQL_HANDLE_DBC,
                      sDbc,
                      ( (SChar*)"SQLSetConnectAttr(ALTIBASE_CONNECTION_RETRY_COUNT)" ),
                      aConnectInfo,
                      &(aConnectInfo->mLinkFailure) );
    }
    IDE_EXCEPTION( SetConnectAttrError_RETRY_DELAY )
    {
        processError( SQL_HANDLE_DBC,
                      sDbc,
                      ( (SChar*)"SQLSetConnectAttr(ALTIBASE_CONNECTION_RETRY_DELAY)" ),
                      aConnectInfo,
                      &(aConnectInfo->mLinkFailure) );
    }
    IDE_EXCEPTION( SetConnectAttrError_CONN_TIMEOUT )
    {
        processError( SQL_HANDLE_DBC,
                      sDbc,
                      ( (SChar*)"SQLSetConnectAttr(ALTIBASE_CONNECTION_TIMEOUT)" ),
                      aConnectInfo,
                      &(aConnectInfo->mLinkFailure) );
    }
    IDE_EXCEPTION( SetConnectAttrError_LOGIN_TIMEOUT )
    {
        processError( SQL_HANDLE_DBC,
                      sDbc,
                      ( (SChar*)"SQLSetConnectAttr(ALTIBASE_LOGIN_TIMEOUT)" ),
                      aConnectInfo,
                      &(aConnectInfo->mLinkFailure) );
    }
    IDE_EXCEPTION_END;

    switch ( sState )
    {
        case 3:
            SQLDisconnectLocal(sDbc);
        case 2:
            SQLFreeHandle( SQL_HANDLE_DBC, sDbc );
        case 1:
            SQLFreeHandle( SQL_HANDLE_ENV, sEnv );
        default:
            break;
    }

    aConnectInfo->mDbc  = NULL;
    aConnectInfo->mLinkFailure = ID_FALSE;

    return IDE_FAILURE;
}

IDE_RC sdl::internalConnectCore( void            * aDbc,
                                 sdiConnectInfo  * aConnectInfo,
                                 UChar           * aConnectString )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation : Connect to data node.
 *                  Do retry connect if need.
 *
 * Arguments
 *     - aDbc           : DBC.
 *                        최초 접속 시 ( 이 함수에 들어오는 ) aConnectInfo->mDBC = NULL 이다.
 *     - aConnectInfo   : data node 접속 정보
 *     - aConnectString : Connect String
 *
 * Return value
 *     - IDE_RC         : SQLDriverConnect() result.
 *
 * Error handle
 *     - IDE_SUCCESS
 *     - IDE_FAILURE
 *
 ***********************************************************************/

    SQLRETURN   sRet             = SQL_ERROR;
    SQLRETURN   sRetNeedFailover = SQL_ERROR;
    SQLHDBC     sDbc             = (SQLHDBC)aDbc;
    SQLCHAR   * sConnectString   = (SQLCHAR *)aConnectString;
    idBool      sIsNotify        = ID_FALSE;
    UInt        sCurRetryCnt     = 0;
    UInt        sMaxRetryCnt     = sdi::getShardInternalConnAttrRetryCount();
    UInt        sRetryDelay      = sdi::getShardInternalConnAttrRetryDelay();
    SQLINTEGER  sIsNeedFailover  = 0; 

    IDE_ASSERT( mInitialized != ID_FALSE );
    IDE_ASSERT( sDbc != NULL );

    if ( ( aConnectInfo->mFlag & SDI_CONNECT_INITIAL_BY_NOTIFIER_MASK )
           == SDI_CONNECT_INITIAL_BY_NOTIFIER_TRUE )
    
    {
        /* dktNotifier::notifyXaResultForShard */
        sIsNotify = ID_TRUE;
    }

    sRet = SQLDriverConnect( sDbc,
                             NULL,
                             sConnectString,
                             SQL_NTS,
                             NULL,
                             0,
                             NULL,
                             SQL_DRIVER_NOPROMPT );

    if ( ( sRet                          != SQL_SUCCESS ) &&
         ( aConnectInfo->mFailoverTarget != SDI_FAILOVER_ALL ) &&
         ( sIsNotify                     == ID_FALSE ) )
    {
        for ( sCurRetryCnt = 0; sCurRetryCnt < sMaxRetryCnt; sCurRetryCnt++ )
        {

            if ( sCurRetryCnt > 0 )
            {
                idlOS::sleep( sRetryDelay );
            }

            sRetNeedFailover = SQLGetNeedFailover( SQL_HANDLE_DBC,
                                                   sDbc,
                                                   &sIsNeedFailover );
            if ( ( sRetNeedFailover != SQL_SUCCESS ) || ( sIsNeedFailover == 0 ) )
            {
                break;
            }

            sRet = SQLDriverConnect( sDbc,
                                     NULL,
                                     sConnectString,
                                     SQL_NTS,
                                     NULL,
                                     0,
                                     NULL,
                                     SQL_DRIVER_NOPROMPT );

            if ( sRet == SQL_SUCCESS )
            {
                break;
            }
        }
    }

    IDE_TEST_RAISE( sRet != SQL_SUCCESS, ConnectError );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ConnectError )
    {
        processError( SQL_HANDLE_DBC,
                      sDbc,
                      ( (SChar*)"SQLDriverConnect" ),
                      aConnectInfo,
                      &(aConnectInfo->mLinkFailure) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdl::disconnect( sdiConnectInfo * aConnectInfo,
                        idBool         * aIsLinkFailure )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation : SQLDisconnect
 *
 * Arguments
 *     - aConnectInfo   : data node 접속 정보
 *     - aIsLinkFailure : Link-실패 여부에 대한 플래그.
 *
 * Return value
 *
 * Error handle
 *     - SQL_SUCCESS
 *     - SQL_SUCCESS_WITH_INFO
 *     - SQL_INVALID_HANDLE
 *     - SQL_ERROR
 *
 ***********************************************************************/
    SQLRETURN    sRet     = SQL_ERROR;

    SET_LINK_FAILURE_FLAG( aIsLinkFailure, ID_FALSE );

    IDE_TEST_RAISE( mInitialized == ID_FALSE, UnInitializedError );
    IDE_TEST_RAISE( aConnectInfo->mDbc == NULL, ErrorNullDbc );

    sRet = SQLDisconnect( aConnectInfo->mDbc );
    IDE_TEST_RAISE( sRet != SQL_SUCCESS, DisConnectErr );

    return IDE_SUCCESS;

    IDE_EXCEPTION_UNINIT_LIBRARY( aConnectInfo, "disconnect" )
    IDE_EXCEPTION_NULL_DBC( aConnectInfo, "disconnect" )
    IDE_EXCEPTION( DisConnectErr )
    {
        processErrorOnNested( SQL_HANDLE_DBC,
                              aConnectInfo->mDbc,
                              ((SChar*)"SQLDisconnect"),
                              aConnectInfo,
                              aIsLinkFailure );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdl::disconnectLocal( sdiConnectInfo * aConnectInfo,
                             idBool *aIsLinkFailure )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation : SQLDisconnect
 *
 * Arguments
 *     - aConnectInfo   : data node 접속 정보
 *     - aIsLinkFailure : Link-실패 여부에 대한 플래그.
 *
 * Return value
 *
 * Error handle
 *     - SQL_SUCCESS
 *     - SQL_SUCCESS_WITH_INFO
 *     - SQL_INVALID_HANDLE
 *     - SQL_ERROR
 *
 ***********************************************************************/
    SQLRETURN    sRet     = SQL_ERROR;

    SET_LINK_FAILURE_FLAG( aIsLinkFailure, ID_FALSE );

    IDE_TEST_RAISE( mInitialized == ID_FALSE, UnInitializedError );
    IDE_TEST_RAISE( aConnectInfo->mDbc == NULL, ErrorNullDbc );

    sRet = SQLDisconnectLocal( aConnectInfo->mDbc );
    IDE_TEST_RAISE( sRet != SQL_SUCCESS, DisConnectErr );

    return IDE_SUCCESS;

    IDE_EXCEPTION_UNINIT_LIBRARY( aConnectInfo, "disconnectLocal" )
    IDE_EXCEPTION_NULL_DBC( aConnectInfo, "disconnectLocal" )
    IDE_EXCEPTION( DisConnectErr )
    {
        processErrorOnNested( SQL_HANDLE_DBC,
                              aConnectInfo->mDbc,
                              ((SChar*)"SQLDisconnectLocal"),
                              aConnectInfo,
                              aIsLinkFailure );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdl::freeConnect( sdiConnectInfo * aConnectInfo,
                         idBool *aIsLinkFailure )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation : SQLFreeConnect
 *
 * Arguments
 *     - aConnectInfo   : data node 접속 정보
 *     - aIsLinkFailure : Link-실패 여부에 대한 플래그.
 *
 * Return value
 *
 * Error handle
 *     - SQL_SUCCESS
 *     - SQL_INVALID_HANDLE
 *     - SQL_ERROR
 *
 ***********************************************************************/
    SQLHENV      sEnv = NULL;

    SET_LINK_FAILURE_FLAG( aIsLinkFailure, ID_FALSE );

    IDE_TEST_RAISE( mInitialized == ID_FALSE, UnInitializedError );
    IDE_TEST_RAISE( aConnectInfo->mDbc == NULL, ErrorNullDbc );

    sEnv = SQLGetEnvHandle( aConnectInfo->mDbc );

    (void)SQLFreeConnect( aConnectInfo->mDbc );

    (void)SQLFreeHandle( SQL_HANDLE_ENV, sEnv );

    return IDE_SUCCESS;

    IDE_EXCEPTION_UNINIT_LIBRARY( aConnectInfo, "freeConnect" )
    IDE_EXCEPTION_NULL_DBC( aConnectInfo, "freeConnect" )
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdl::allocStmt( sdiConnectInfo * aConnectInfo,
                       sdlRemoteStmt  * aRemoteStmt,
                       idBool         * aIsLinkFailure )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation : SQLAllocStmt
 *
 * Arguments
 *  - aConnectInfo   : data node 접속 정보
 *  - aRemoteStmt    : SdStatement Handler
 *  - aIsLinkFailure : Link-실패 여부에 대한 플래그.
 *
 * Return value
 *     - aRemoteStmt : data node 에 대한 sdlRemoteStmt.mStmt
 *
 * Error handle
 *     - SQL_SUCCESS
 *     - SQL_SUCCESS_WITH_INFO
 *     - SQL_INVALID_HANDLE
 *     - SQL_ERROR
 *
 ***********************************************************************/
    SQLRETURN sRet  = SQL_ERROR;
    SQLHSTMT  sStmt = NULL;

    SET_LINK_FAILURE_FLAG( aIsLinkFailure, ID_FALSE );

    IDE_TEST_RAISE( mInitialized == ID_FALSE, UnInitializedError );
    IDE_TEST_RAISE( aConnectInfo->mDbc == NULL, ErrorNullDbc );
    IDE_TEST_RAISE( aRemoteStmt == NULL, ErrorNullSdStmt );

    if ( ( aRemoteStmt->mFreeFlag & SDL_STMT_FREE_FLAG_ALLOCATED_MASK )
         == SDL_STMT_FREE_FLAG_ALLOCATED_TRUE )
    {
        /* Reuse statement. */
        IDE_DASSERT( aRemoteStmt->mStmt != NULL  );

        IDE_TEST( freeStmt( aConnectInfo,
                            aRemoteStmt,
                            SQL_RESET_PARAMS,
                            aIsLinkFailure )
                  != IDE_SUCCESS )

        IDE_TEST( freeStmt( aConnectInfo,
                            aRemoteStmt,
                            SQL_CLOSE,
                            aIsLinkFailure )
                  != IDE_SUCCESS )

    }
    else
    {
        sRet = SQLAllocHandle( SQL_HANDLE_STMT, (SQLHANDLE)aConnectInfo->mDbc, &sStmt );
        IDE_TEST_RAISE( sRet != SQL_SUCCESS, AllocHandleErr );

        aRemoteStmt->mStmt = (void*)sStmt;
        aRemoteStmt->mFreeFlag &= ~SDL_STMT_FREE_FLAG_ALLOCATED_MASK;
        aRemoteStmt->mFreeFlag |= SDL_STMT_FREE_FLAG_ALLOCATED_TRUE;
    }


    return IDE_SUCCESS;

    IDE_EXCEPTION_UNINIT_LIBRARY( aConnectInfo, "allocStmt" )
    IDE_EXCEPTION_NULL_DBC( aConnectInfo, "allocStmt" )
    IDE_EXCEPTION_NULL_SD_STMT( aConnectInfo, "allocStmt" )
    IDE_EXCEPTION( AllocHandleErr )
    {
        processErrorOnNested( SQL_HANDLE_DBC,
                              aConnectInfo->mDbc,
                              ((SChar*)"SQLAllocHandle"),
                              aConnectInfo,
                              aIsLinkFailure );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdl::freeStmt( sdiConnectInfo * aConnectInfo,
                      sdlRemoteStmt  * aRemoteStmt,
                      SShort           aOption,
                      idBool         * aIsLinkFailure )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation : SQLFreeStmt
 *
 * Arguments
 *  - aConnectInfo   : data node 접속 정보
 *  - aRemoteStmt    : SdStatement Handler
 *  - aOption : SQLFreeStmt option
 *                 SQL_CLOSE
 *                 SQL_DROP
 *  - aIsLinkFailure : Link-실패 여부에 대한 플래그.
 *
 * Return value
 *
 * Error handle
 *     - SQL_SUCCESS
 *     - SQL_SUCCESS_WITH_INFO
 *     - SQL_INVALID_HANDLE
 *     - SQL_ERROR
 *
 ***********************************************************************/
    SQLRETURN sRet  = SQL_ERROR;

    SET_LINK_FAILURE_FLAG( aIsLinkFailure, ID_FALSE );

    IDE_TEST_RAISE( mInitialized == ID_FALSE, UnInitializedError );
    IDE_TEST_RAISE( aRemoteStmt == NULL, ErrorNullSdStmt );
    IDE_TEST_RAISE( aRemoteStmt->mStmt == NULL, ErrorNullStmt );

    switch ( aOption )
    {
        case SQL_DROP :
            sRet = SQLFreeStmt( (SQLHANDLE)aRemoteStmt->mStmt, (SQLUSMALLINT)aOption );

            aRemoteStmt->mStmt = NULL;
            aRemoteStmt->mFreeFlag &= ~SDL_STMT_FREE_FLAG_DROP_MASK;
            aRemoteStmt->mFreeFlag |= SDL_STMT_FREE_FLAG_DROP;
            break;

        case SQL_CLOSE :
            if ( ( aRemoteStmt->mFreeFlag & SDL_STMT_FREE_FLAG_TRY_EXECUTE_MASK )
                   == SDL_STMT_FREE_FLAG_TRY_EXECUTE_TRUE )
            {
                sRet = SQLFreeStmt( (SQLHANDLE)aRemoteStmt->mStmt, (SQLUSMALLINT)aOption );

                aRemoteStmt->mFreeFlag &= ~SDL_STMT_FREE_FLAG_TRY_EXECUTE_MASK;
                aRemoteStmt->mFreeFlag |= SDL_STMT_FREE_FLAG_TRY_EXECUTE_FALSE;
            }
            else
            {
                sRet = SQL_SUCCESS;
            }
            break;

        case SQL_RESET_PARAMS :
            if ( ( aRemoteStmt->mFreeFlag & SDL_STMT_FREE_FLAG_TRY_BIND_PARAM_MASK )
                 == SDL_STMT_FREE_FLAG_TRY_BIND_PARAM_TRUE )
            {
                sRet = SQLFreeStmt( (SQLHANDLE)aRemoteStmt->mStmt, (SQLUSMALLINT)aOption );

                aRemoteStmt->mFreeFlag &= ~SDL_STMT_FREE_FLAG_TRY_BIND_PARAM_MASK;
                aRemoteStmt->mFreeFlag |= SDL_STMT_FREE_FLAG_TRY_BIND_PARAM_FALSE;
            }
            else
            {
                sRet = SQL_SUCCESS;
            }
            break;

        default:
            sRet = SQLFreeStmt( (SQLHANDLE)aRemoteStmt->mStmt, (SQLUSMALLINT)aOption );

            aRemoteStmt->mStmt = NULL;
            aRemoteStmt->mFreeFlag &= ~SDL_STMT_FREE_FLAG_DROP_MASK;
            aRemoteStmt->mFreeFlag |= SDL_STMT_FREE_FLAG_DROP;
            IDE_DASSERT( 0 );
            break;
    }

    IDE_TEST_RAISE( sRet != SQL_SUCCESS, FreeHandleErr );

    return IDE_SUCCESS;

    IDE_EXCEPTION_UNINIT_LIBRARY( aConnectInfo, "freeStmt" )
    IDE_EXCEPTION_NULL_SD_STMT( aConnectInfo, "freeStmt" )
    IDE_EXCEPTION_NULL_STMT( aConnectInfo, "freeStmt" )
    IDE_EXCEPTION( FreeHandleErr )
    {
        processErrorOnNested( SQL_HANDLE_STMT,
                              aRemoteStmt->mStmt,
                              ((SChar*)"SQLFreeStmt"),
                              aConnectInfo,
                              aIsLinkFailure );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdl::addPrepareCallback( void           ** aCallback,
                                UInt              aNodeIndex,
                                sdiConnectInfo *  aConnectInfo,
                                sdlRemoteStmt  *  aRemoteStmt,
                                SChar          *  aQuery,
                                SInt              aLength,
                                idBool         *  aIsLinkFailure )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation : SQLPrepare
 *
 * Arguments
 *  - aCallback      : Callback pointer for nested function call.
 *  - aNodeIndex     : Add Callback Index. Used to find matching resultCallback().
 *  - aConnectInfo   : data node 접속 정보
 *  - aRemoteStmt    : data node 에 대한 sdlRemoteStmt
 *  - aQuery         : query string
 *  - aLength        : query string size
 *  - aIsLinkFailure : Link-실패 여부에 대한 플래그.
 *
 * Return value
 *
 * Error handle
 *     - SQL_SUCCESS
 *     - SQL_SUCCESS_WITH_INFO
 *     - SQL_INVALID_HANDLE
 *     - SQL_ERROR
 *
 ***********************************************************************/
    SQLRETURN sRet  = SQL_ERROR;

    SET_LINK_FAILURE_FLAG( aIsLinkFailure, ID_FALSE );

    IDE_TEST_RAISE( mInitialized == ID_FALSE, UnInitializedError );
    IDE_TEST_RAISE( aRemoteStmt == NULL, ErrorNullSdStmt );
    IDE_TEST_RAISE( aRemoteStmt->mStmt == NULL, ErrorNullStmt );

    sRet = SQLPrepareAddCallback( (SQLUINTEGER)aNodeIndex,
                                  (SQLHANDLE)aRemoteStmt->mStmt,
                                  (SQLCHAR*)aQuery,
                                  (SQLINTEGER)aLength,
                                  (SQLPOINTER **)aCallback );

    IDE_TEST_RAISE( sRet == SQL_ERROR, PrepareErr );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    IDE_EXCEPTION_UNINIT_LIBRARY( aConnectInfo, "addPrepareCallback" )
    IDE_EXCEPTION_NULL_SD_STMT( aConnectInfo, "addPrepareCallback" )
    IDE_EXCEPTION_NULL_STMT( aConnectInfo, "addPrepareCallback" )
    IDE_EXCEPTION( PrepareErr )
    {
        processErrorOnNested( SQL_HANDLE_STMT,
                              aRemoteStmt->mStmt,
                              ((SChar*)"SQLPrepareAddCallback"),
                              aConnectInfo,
                              aIsLinkFailure );
    }
    return IDE_FAILURE;
}

IDE_RC sdl::getParamsCount( sdiConnectInfo * aConnectInfo,
                            sdlRemoteStmt  * aRemoteStmt,
                            UShort         * aParamCount,
                            idBool         * aIsLinkFailure )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation : Get count of parameters.
 *
 * Arguments
 *     - aConnectInfo    [IN] : data node 접속 정보
 *     - aRemoteStmt     [IN] : data node 에 대한 sdlRemoteStmt
 *     - aParamCount     [OUT]:
 *     - aIsLinkFailure  [OUT]:
 *
 * Return value
 *     - Count of parameter.
 * Error handle
 *     - SQL_SUCCESS
 *     - SQL_SUCCESS_WITH_INFO
 *     - SQL_INVALID_HANDLE
 *     - SQL_ERROR
 *
 ***********************************************************************/
    SET_LINK_FAILURE_FLAG( aIsLinkFailure, ID_FALSE );

    IDE_TEST_RAISE( mInitialized == ID_FALSE, UnInitializedError );
    IDE_TEST_RAISE( aRemoteStmt == NULL, ErrorNullSdStmt );
    IDE_TEST_RAISE( aRemoteStmt->mStmt == NULL, ErrorNullStmt );

    IDE_TEST_RAISE( SQLGetParameterCount( aRemoteStmt->mStmt,
                                          aParamCount )
                    == SQL_ERROR,
                    getParamsCountErr );

    return IDE_SUCCESS;

    IDE_EXCEPTION_UNINIT_LIBRARY( aConnectInfo, "getParamsCount" )
    IDE_EXCEPTION_NULL_SD_STMT( aConnectInfo, "getParamsCount" )
    IDE_EXCEPTION_NULL_STMT( aConnectInfo, "getParamsCount" )
    IDE_EXCEPTION( getParamsCountErr )
    {
        processErrorOnNested( SQL_HANDLE_STMT,
                              aRemoteStmt->mStmt,
                              ((SChar*)"SQLGetParameterCount"),
                              aConnectInfo,
                              aIsLinkFailure );
    }
    IDE_EXCEPTION_END;
    return IDE_FAILURE;
}

IDE_RC sdl::bindParam( sdiConnectInfo * aConnectInfo,
                       sdlRemoteStmt  * aRemoteStmt,
                       UShort           aParamNum,
                       UInt             aInOutType,
                       UInt             aValueType,
                       void           * aParamValuePtr,
                       SInt             aBufferLen,
                       SInt             aPrecision,
                       SInt             aScale,
                       idBool         * aIsLinkFailure )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation : SQLBindParameter
 *
 * Arguments
 *     - aConnectInfo    [IN] : data node 접속 정보
 *     - aRemoteStmt     [IN] : data node 에 대한 sdlRemoteStmt
 *     - aParamNum       [IN] :
 *     - aInOutType      [IN] :
 *     - aValueType      [IN] :
 *     - aParamValuePtr  [IN] :
 *     - aBufferLen      [IN] :
 *     - aPrecision      [IN] :
 *     - aScale          [IN] :
 *     - aIsLinkFailure  [OUT]:
 *
 * Return value
 *
 * Error handle
 *     - SQL_SUCCESS
 *     - SQL_SUCCESS_WITH_INFO
 *     - SQL_INVALID_HANDLE
 *     - SQL_ERROR
 *
 ***********************************************************************/
    SQLRETURN sRet = SQL_ERROR;

    SQLSMALLINT sSdlMType = 0;

    SET_LINK_FAILURE_FLAG( aIsLinkFailure, ID_FALSE );

    IDE_TEST_RAISE( mInitialized == ID_FALSE, UnInitializedError );
    IDE_TEST_RAISE( aRemoteStmt == NULL, ErrorNullSdStmt );
    IDE_TEST_RAISE( aRemoteStmt->mStmt == NULL, ErrorNullStmt );

    sSdlMType = (SQLSMALLINT)mtdType_to_SDLMType( aValueType );
    IDE_TEST_RAISE( ( (UShort)sSdlMType == SDL_MTYPE_MAX ),
                    BindParameterTypeNotSupport );

    aRemoteStmt->mFreeFlag &= ~SDL_STMT_FREE_FLAG_TRY_BIND_PARAM_MASK;
    aRemoteStmt->mFreeFlag |= SDL_STMT_FREE_FLAG_TRY_BIND_PARAM_TRUE;

    sRet = SQLBindParameter( (SQLHSTMT    )aRemoteStmt->mStmt,
                             (SQLUSMALLINT)aParamNum,
                             (SQLSMALLINT )aInOutType,
                             (SQLSMALLINT )sSdlMType,
                             (SQLSMALLINT )sSdlMType,
                             (SQLULEN     )aPrecision,
                             (SQLSMALLINT )aScale,
                             (SQLPOINTER  )aParamValuePtr,
                             (SQLLEN      )aBufferLen,
                             (SQLLEN     *)NULL );

    IDE_TEST_RAISE( sRet == SQL_ERROR, BindParameterErr);

    return IDE_SUCCESS;

    IDE_EXCEPTION_UNINIT_LIBRARY( aConnectInfo, "bindParam" )
    IDE_EXCEPTION_NULL_SD_STMT( aConnectInfo, "bindParam" )
    IDE_EXCEPTION_NULL_STMT( aConnectInfo, "bindParam" )
    IDE_EXCEPTION( BindParameterTypeNotSupport )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDC_UNEXPECTED_ERROR,
                                  "sdl::bindParam",
                                  "Bind parameter type is not supported" ) );
    }
    IDE_EXCEPTION( BindParameterErr )
    {
        processErrorOnNested( SQL_HANDLE_STMT,
                              aRemoteStmt->mStmt,
                              ((SChar*)"SQLBindParameter"),
                              aConnectInfo,
                              aIsLinkFailure );
    }
    IDE_EXCEPTION_END;
    return IDE_FAILURE;
}

IDE_RC sdl::describeCol( sdiConnectInfo * aConnectInfo,
                         sdlRemoteStmt  * aRemoteStmt,
                         UInt             aColNo,
                         SChar          * aColName,
                         SInt             aBufferLength,
                         UInt           * aNameLength,
                         UInt           * aTypeId,        // mt type id
                         SInt           * aPrecision,
                         SShort         * aScale,
                         SShort         * aNullable,
                         idBool         * aIsLinkFailure )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation : SQLDescribeCol
 *
 * Arguments
 *     - aConnectInfo   : data node 접속 정보
 *     - aRemoteStmt    : data node 에 대한 sdlRemoteStmt
 *     - aColNo         : column position
 *
 * Return value
 *     - aColName     : column name
 *     - aMaxByteSize : max byte size of column
 *     - aTypeId      : column type id (mtType)
 *     - aPrecision   : column precision
 *     - aScale       : column scale
 *     - aNullable    : column nullable
 *
 * Error handle
 *     - SQL_SUCCESS
 *     - SQL_SUCCESS_WITH_INFO
 *     - SQL_INVALID_HANDLE
 *     - SQL_ERROR
 *
 ***********************************************************************/
    SET_LINK_FAILURE_FLAG( aIsLinkFailure, ID_FALSE );

    IDE_TEST_RAISE( mInitialized == ID_FALSE, UnInitializedError );
    IDE_TEST_RAISE( aRemoteStmt == NULL, ErrorNullSdStmt );
    IDE_TEST_RAISE( aRemoteStmt->mStmt == NULL, ErrorNullStmt );
    IDE_TEST_RAISE( SQLDescribeColEx( (SQLHSTMT     )aRemoteStmt->mStmt,
                                      (SQLUSMALLINT )aColNo,
                                      (SQLCHAR     *)aColName,
                                      (SQLINTEGER   )aBufferLength,
                                      (SQLUINTEGER *)aNameLength,
                                      (SQLUINTEGER *)aTypeId,
                                      (SQLINTEGER  *)aPrecision,
                                      (SQLSMALLINT *)aScale,
                                      (SQLSMALLINT *)aNullable )
                     == SQL_ERROR,
                    DescribeColExErr );
    return IDE_SUCCESS;

    IDE_EXCEPTION_UNINIT_LIBRARY( aConnectInfo, "describeCol" )
    IDE_EXCEPTION_NULL_SD_STMT( aConnectInfo, "describeCol" )
    IDE_EXCEPTION_NULL_STMT( aConnectInfo, "describeCol" )
    IDE_EXCEPTION( DescribeColExErr )
    {
        processErrorOnNested( SQL_HANDLE_STMT,
                              aRemoteStmt->mStmt,
                              ((SChar*)"SQLDescribeColEx"),
                              aConnectInfo,
                              aIsLinkFailure );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdl::bindCol( sdiConnectInfo * aConnectInfo,
                     sdlRemoteStmt  * aRemoteStmt,
                     UInt             aColNo,
                     UInt             aColType,
                     UInt             aColSize,
                     void           * aBuffer,
                     idBool         * aIsLinkFailure )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation : SQLBindCol
 *
 * Arguments
 *     - aConnectInfo   : data node 접속 정보
 *     - aRemoteStmt    : data node 에 대한 sdlRemoteStmt
 *     - aColNo         : column position
 *     - aColtype       : column type (mtType)
 *     - aColSize       : column size (mtType)
 *
 * Return value
 *     - aBuffer : SQLFetch 이후 반환될 주소
 *
 * Error handle
 *     - SQL_SUCCESS
 *     - SQL_SUCCESS_WITH_INFO
 *     - SQL_INVALID_HANDLE
 *     - SQL_ERROR
 *
 ***********************************************************************/
    SET_LINK_FAILURE_FLAG( aIsLinkFailure, ID_FALSE );

    /* Not used variable */
    if ( aColNo   == 1 ) aColNo = 0;
    if ( aColType == 1 ) aColType = 0;
    if ( aColSize == 1 ) aColSize = 0;
    if ( aBuffer  != NULL ) aBuffer = NULL;

    IDE_TEST_RAISE( mInitialized == ID_FALSE, UnInitializedError );
    IDE_TEST_RAISE( aRemoteStmt == NULL, ErrorNullSdStmt );
    IDE_TEST_RAISE( aRemoteStmt->mStmt == NULL, ErrorNullStmt );

    return IDE_SUCCESS;

    IDE_EXCEPTION_UNINIT_LIBRARY( aConnectInfo, "bindCol" )
    IDE_EXCEPTION_NULL_SD_STMT( aConnectInfo, "bindCol" )
    IDE_EXCEPTION_NULL_STMT( aConnectInfo, "bindCol" )
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdl::execDirect( sdiConnectInfo * aConnectInfo,
                        sdlRemoteStmt  * aRemoteStmt,
                        SChar          * aQuery,
                        SInt             aQueryLen,
                        sdiClientInfo  * aClientInfo,
                        idBool         * aIsLinkFailure )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation : SQLExecDirect
 *
 * Arguments
 *     - aConnectInfo   [IN] : data node 접속 정보
 *     - aRemoteStmt    [IN] : data node 에 대한 sdlRemoteStmt
 *     - aQuery         [IN] : Query String
 *     - aQueryLen      [IN] : Query String Length
 *     - aClientInfo    [IN/OUT] : Client Info
 *     - aIsLinkFailure [OUT]: Link-실패 여부에 대한 플래그.
 *
 * Return value
 *
 * Error handle
 *     - SQL_SUCCESS
 *     - SQL_INVALID_HANDLE
 *     - SQL_ERROR
 *
 ***********************************************************************/
    SQLRETURN  sRet           = SQL_ERROR;

    SET_LINK_FAILURE_FLAG( aIsLinkFailure, ID_FALSE );

    IDE_TEST_RAISE( mInitialized == ID_FALSE, UnInitializedError );
    IDE_TEST_RAISE( aConnectInfo->mDbc == NULL, ErrorNullDbc );
    IDE_TEST_RAISE( aRemoteStmt == NULL, ErrorNullSdStmt );
    IDE_TEST_RAISE( aRemoteStmt->mStmt == NULL, ErrorNullStmt );

    sRet = SQLExecDirect((SQLHSTMT)aRemoteStmt->mStmt,
                         (SQLCHAR*)aQuery,
                         (SQLINTEGER)aQueryLen );

    IDE_TEST_RAISE( sRet == SQL_ERROR, ExecDirectErr );

    /* BUG-46100 Session SMN Update */
    sdi::setNeedToDisconnect( aClientInfo, (idBool)SQLGetNeedToDisconnect( aConnectInfo->mDbc ) );

    return IDE_SUCCESS;

    IDE_EXCEPTION_UNINIT_LIBRARY( aConnectInfo, "execDirect" )
    IDE_EXCEPTION_NULL_DBC( aConnectInfo, "execDirect" )
    IDE_EXCEPTION_NULL_SD_STMT( aConnectInfo, "execDirect" )
    IDE_EXCEPTION_NULL_STMT( aConnectInfo, "execDirect" )
    IDE_EXCEPTION( ExecDirectErr )
    {
        processErrorOnNested( SQL_HANDLE_STMT,
                              aRemoteStmt->mStmt,
                              ((SChar*)"SQLExecDirect"),
                              aConnectInfo,
                              aIsLinkFailure );
    }
    IDE_EXCEPTION_END;

    if ( aConnectInfo->mDbc != NULL )
    {
        /* BUG-46100 Session SMN Update */
        sdi::setNeedToDisconnect( aClientInfo, (idBool)SQLGetNeedToDisconnect( aConnectInfo->mDbc ) );
    }
    else
    {
        /* Nothing to do */
    }

    return IDE_FAILURE;
}

IDE_RC sdl::addExecuteCallback( void           ** aCallback,
                                UInt              aNodeIndex,
                                sdiConnectInfo  * aConnectInfo,
                                sdiDataNode     * aNode,
                                idBool          * aIsLinkFailure )
{
    SQLRETURN  sRet = SQL_ERROR;

    SET_LINK_FAILURE_FLAG( aIsLinkFailure, ID_FALSE );

    IDE_TEST_RAISE( mInitialized == ID_FALSE, UnInitializedError );
    IDE_TEST_RAISE( aNode->mRemoteStmt == NULL, ErrorNullSdStmt );
    IDE_TEST_RAISE( aNode->mRemoteStmt->mStmt == NULL, ErrorNullStmt );

    sdi::initAffectedRow( aConnectInfo );

    if ( ( aNode->mBuffer != NULL ) &&
         ( aNode->mOffset != NULL ) &&
         ( aNode->mMaxByteSize != NULL ) )
    {
        IDE_DASSERT( aNodeIndex < SDI_NODE_MAX_COUNT );
        IDE_DASSERT( aNode->mBufferLength > 0 );

        sRet = SQLExecuteForMtDataRowsAddCallback( (SQLUINTEGER  )aNodeIndex,
                                                   (SQLHSTMT     )aNode->mRemoteStmt->mStmt,
                                                   (SQLCHAR     *)aNode->mBuffer[aNodeIndex],
                                                   (SQLUINTEGER  )aNode->mBufferLength,
                                                   (SQLPOINTER  *)aNode->mOffset,
                                                   (SQLPOINTER  *)aNode->mMaxByteSize,
                                                   (SQLUSMALLINT )aNode->mColumnCount,
                                                   (SQLPOINTER **)aCallback );
    }
    else
    {
        sRet = SQLExecuteForMtDataAddCallback( (SQLUINTEGER  )aNodeIndex,
                                               (SQLHSTMT     )aNode->mRemoteStmt->mStmt,
                                               (SQLPOINTER **)aCallback );
    }

    IDE_TEST_RAISE( sRet == SQL_ERROR, ExecuteErr );

    aNode->mRemoteStmt->mFreeFlag &= ~SDL_STMT_FREE_FLAG_TRY_EXECUTE_MASK;
    aNode->mRemoteStmt->mFreeFlag |= SDL_STMT_FREE_FLAG_TRY_EXECUTE_TRUE;

    return IDE_SUCCESS;

    IDE_EXCEPTION_UNINIT_LIBRARY( aConnectInfo, "addExecuteCallback" )
    IDE_EXCEPTION_NULL_SD_STMT( aConnectInfo, "addExecuteCallback" )
    IDE_EXCEPTION_NULL_STMT( aConnectInfo, "addExecuteCallback" )
    IDE_EXCEPTION( ExecuteErr )
    {
        processErrorOnNested( SQL_HANDLE_STMT,
                              aNode->mRemoteStmt->mStmt,
                              ((SChar*)"SQLExecuteAddCallback"),
                              aConnectInfo,
                              aIsLinkFailure );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdl::addPrepareTranCallback( void   ** aCallback,
                                    UInt      aNodeIndex,
                                    sdiConnectInfo * aConnectInfo,
                                    ID_XID  * aXID,
                                    UChar   * aReadOnly,
                                    idBool  * aIsLinkFailure )
{
    SQLRETURN  sRet = SQL_ERROR;

    SET_LINK_FAILURE_FLAG( aIsLinkFailure, ID_FALSE );

    IDE_TEST_RAISE( mInitialized == ID_FALSE, UnInitializedError );
    IDE_TEST_RAISE( aConnectInfo->mDbc == NULL, ErrorNullDbc );

    sRet = SQLPrepareTranAddCallback( (SQLUINTEGER )aNodeIndex,
                                      (SQLHDBC     )aConnectInfo->mDbc,
                                      (SQLUINTEGER )ID_SIZEOF(ID_XID),
                                      (SQLPOINTER* )aXID,
                                      (SQLCHAR*    )aReadOnly,
                                      (SQLPOINTER**)aCallback );

    IDE_TEST_RAISE( sRet == SQL_ERROR, ExecuteErr );

    return IDE_SUCCESS;

    IDE_EXCEPTION_UNINIT_LIBRARY( aConnectInfo, "addPrepareTranCallback" )
    IDE_EXCEPTION_NULL_DBC( aConnectInfo, "addPrepareTranCallback" )
    IDE_EXCEPTION( ExecuteErr )
    {
        processErrorOnNested( SQL_HANDLE_DBC,
                              aConnectInfo->mDbc,
                              ((SChar*)"addPrepareTranCallback"),
                              aConnectInfo,
                              aIsLinkFailure );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdl::addEndTranCallback( void          ** aCallback,
                                UInt             aNodeIndex,
                                sdiConnectInfo * aConnectInfo,
                                idBool           aIsCommit,
                                idBool         * aIsLinkFailure )
{
    SQLRETURN  sRet = SQL_ERROR;

    SET_LINK_FAILURE_FLAG( aIsLinkFailure, ID_FALSE );

    IDE_TEST_RAISE( mInitialized == ID_FALSE, UnInitializedError );
    IDE_TEST_RAISE( aConnectInfo->mDbc == NULL, ErrorNullDbc );

    if ( aIsCommit == ID_TRUE )
    {
        sRet = SQLEndTranAddCallback( (SQLUINTEGER )aNodeIndex,
                                      (SQLHDBC     )aConnectInfo->mDbc,
                                      (SQLSMALLINT )SDL_TRANSACT_COMMIT,
                                      (SQLPOINTER**)aCallback );
    }
    else
    {
        sRet = SQLEndTranAddCallback( (SQLUINTEGER )aNodeIndex,
                                      (SQLHDBC     )aConnectInfo->mDbc,
                                      (SQLSMALLINT )SDL_TRANSACT_ROLLBACK,
                                      (SQLPOINTER**)aCallback );
    }

    IDE_TEST_RAISE( sRet == SQL_ERROR, ExecuteErr );

    return IDE_SUCCESS;

    IDE_EXCEPTION_UNINIT_LIBRARY( aConnectInfo, "addEndTranCallback" )
    IDE_EXCEPTION_NULL_DBC( aConnectInfo, "addEndTranCallback" )
    IDE_EXCEPTION( ExecuteErr )
    {
        processErrorOnNested( SQL_HANDLE_DBC,
                              aConnectInfo->mDbc,
                              ((SChar*)"SQLEndTranAddCallback"),
                              aConnectInfo,
                              aIsLinkFailure );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

void sdl::doCallback( void  * aCallback )
{
    if ( aCallback != NULL )
    {
        SQLDoCallback( (SQLPOINTER *)aCallback );
    }
    else
    {
        // Nothing to do.
    }
}

IDE_RC sdl::resultCallback( void           * aCallback,
                            UInt             aNodeIndex,
                            sdiConnectInfo * aConnectInfo,
                            idBool           aReCall,
                            sdiClientInfo  * aClientInfo,
                            SShort           aHandleType,
                            void           * aHandle,
                            SChar          * aFuncName,
                            idBool         * aIsLinkFailure )
{
    SQLRETURN  sRet           = SQL_ERROR;

    SET_LINK_FAILURE_FLAG( aIsLinkFailure, ID_FALSE );

    IDE_TEST_RAISE( mInitialized == ID_FALSE, UnInitializedError );

    IDE_TEST_RAISE( aConnectInfo->mDbc == NULL, ErrorNullDbc );

    if ( aHandleType == SQL_HANDLE_STMT )
    {
        IDE_TEST_RAISE( aHandle == NULL, ErrorNullStmt );
    }
    else
    {
        IDE_TEST_RAISE( aHandle == NULL, ErrorNullDbc );
    }

    sRet = SQLGetResultCallback( (SQLUINTEGER )aNodeIndex,
                                 (SQLPOINTER *)aCallback,
                                 (SQLCHAR)( ( aReCall == ID_TRUE ) ? 1: 0 ) );

    IDE_TEST_RAISE( sRet == SQL_ERROR, ExecuteErr );

    /* BUG-46100 Session SMN Update */
    sdi::setNeedToDisconnect( aClientInfo, (idBool)SQLGetNeedToDisconnect( aConnectInfo->mDbc ) );

    return IDE_SUCCESS;

    IDE_EXCEPTION_UNINIT_LIBRARY( aConnectInfo, "resultCallback" )
    IDE_EXCEPTION_NULL_DBC( aConnectInfo, "resultCallback" )
    IDE_EXCEPTION_NULL_STMT( aConnectInfo, "resultCallback" )
    IDE_EXCEPTION( ExecuteErr )
    {
        processErrorOnNested( aHandleType,
                              aHandle,
                              aFuncName,
                              aConnectInfo,
                              aIsLinkFailure );
    }
    IDE_EXCEPTION_END;

    if ( aConnectInfo->mDbc != NULL )
    {
        /* BUG-46100 Session SMN Update */
        sdi::setNeedToDisconnect( aClientInfo, (idBool)SQLGetNeedToDisconnect( aConnectInfo->mDbc ) );
    }
    else
    {
        /* Nothing to do */
    }

    return IDE_FAILURE;
}

void sdl::removeCallback( void * aCallback )
{
    if ( aCallback != NULL )
    {
        SQLRemoveCallback( (SQLPOINTER *)aCallback );
    }
    else
    {
        // Nothing to do.
    }
}

IDE_RC sdl::fetch( sdiConnectInfo * aConnectInfo,
                   sdlRemoteStmt  * aRemoteStmt,
                   SShort         * aResult,
                   idBool         * aIsLinkFailure )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation : SQLFetch
 *
 * Arguments
 *     - aConnectInfo   [IN] : data node 접속 정보
 *     - aRemoteStmt    [IN] : data node 에 대한 sdlRemoteStmt
 *     - aResult        [OUT]: 수행 결과에 대한 value
 *     - aIsLinkFailure [OUT]: Link-실패 여부에 대한 플래그.
 *
 * Return value
 *     - aResult : 수행 결과에 대한 value
 *
 * Error handle
 *     - SQL_SUCCESS
 *     - SQL_SUCCESS_WITH_INFO
 *     - SQL_NO_DATA_FOUND
 *     - SQL_INVALID_HANDLE
 *     - SQL_ERROR
 *
 ***********************************************************************/
    SQLRETURN sRet = SQL_ERROR;

    SET_LINK_FAILURE_FLAG( aIsLinkFailure, ID_FALSE );

    IDE_TEST_RAISE( mInitialized == ID_FALSE, UnInitializedError );
    IDE_TEST_RAISE( aRemoteStmt == NULL, ErrorNullSdStmt );
    IDE_TEST_RAISE( aRemoteStmt->mStmt == NULL, ErrorNullStmt );

    sRet = SQLFetch( (SQLHANDLE)aRemoteStmt->mStmt );
    IDE_TEST_RAISE( ( ( sRet != SQL_SUCCESS ) &&
                      ( sRet != SQL_NO_DATA_FOUND ) &&
                      ( sRet != SQL_SUCCESS_WITH_INFO ) ),
                    FetchErr );

    *aResult = sRet;

    return IDE_SUCCESS;

    IDE_EXCEPTION_UNINIT_LIBRARY( aConnectInfo, "fetch" )
    IDE_EXCEPTION_NULL_SD_STMT( aConnectInfo, "fetch" )
    IDE_EXCEPTION_NULL_STMT( aConnectInfo, "fetch" )
    IDE_EXCEPTION( FetchErr )
    {
        processErrorOnNested( SQL_HANDLE_STMT,
                              aRemoteStmt->mStmt,
                              ((SChar*)"SQLFetch"),
                              aConnectInfo,
                              aIsLinkFailure );
    }
    IDE_EXCEPTION_END;

    *aResult = sRet;

    return IDE_FAILURE;
}

IDE_RC sdl::rowCount( sdiConnectInfo * aConnectInfo,
                      sdlRemoteStmt  * aRemoteStmt,
                      vSLong         * aNumRows,
                      idBool         * aIsLinkFailure )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation : SQLRowCount for DML
 *
 * Arguments
 *     - aConnectInfo   [IN] : data node 접속 정보
 *     - aRemoteStmt    [IN] : data node 에 대한 sdlRemoteStmt
 *     - aNumRows       [OUT]: 로의 수를 반환하기 위한 포인터
 *     - aIsLinkFailure [OUT]: Link-실패 여부에 대한 플래그.
 * Return value
 *     - aNumRows : DML 수행 결과 행 수
 *
 * Error handle
 *     - SQL_SUCCESS
 *     - SQL_SUCCESS_WITH_INFO
 *     - SQL_INVALID_HANDLE
 *     - SQL_ERROR
 *
 ***********************************************************************/
    SQLRETURN  sRet = SQL_ERROR;
    SQLLEN    sRowCount;

    IDE_TEST_RAISE( mInitialized == ID_FALSE, UnInitializedError );
    IDE_TEST_RAISE( aRemoteStmt == NULL, ErrorNullSdStmt );
    IDE_TEST_RAISE( aRemoteStmt->mStmt == NULL, ErrorNullStmt );

    SET_LINK_FAILURE_FLAG( aIsLinkFailure, ID_FALSE );

    sRet = SQLRowCount( (SQLHANDLE)aRemoteStmt->mStmt, &sRowCount );
    IDE_TEST_RAISE( ( ( sRet != SQL_SUCCESS ) &&
                      ( sRet != SQL_NO_DATA_FOUND ) &&
                      ( sRet != SQL_SUCCESS_WITH_INFO ) ),
                    RowCountErr);

    *aNumRows = (vSLong)sRowCount;

    return IDE_SUCCESS;

    IDE_EXCEPTION_UNINIT_LIBRARY( aConnectInfo, "rowCount" )
    IDE_EXCEPTION_NULL_SD_STMT( aConnectInfo, "rowCount" )
    IDE_EXCEPTION_NULL_STMT( aConnectInfo, "rowCount" )
    IDE_EXCEPTION( RowCountErr )
    {
        processErrorOnNested( SQL_HANDLE_STMT,
                              aRemoteStmt->mStmt,
                              ((SChar*)"SQLRowCount"),
                              aConnectInfo,
                              aIsLinkFailure );
    }
    IDE_EXCEPTION_END;

    *aNumRows = 0;

    return IDE_FAILURE;
}

IDE_RC sdl::getPlan( sdiConnectInfo  * aConnectInfo,
                     sdlRemoteStmt   * aRemoteStmt,
                     SChar          ** aPlan,
                     idBool          * aIsLinkFailure )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation : SQLGetPlan
 *
 * Arguments
 *     - aConnectInfo   [IN] : data node 접속 정보
 *     - aRemoteStmt    [IN] : data node 에 대한 sdlRemoteStmt
 *     - aNodeName      [IN] : 노드 이름. 에러 처리 용.
 *     - aPlan          [OUT]: Plan 결과
 *     - aIsLinkFailure [OUT]: Link-실패 여부에 대한 플래그.
 *
 * Return value
 *     - aNumRows : DML 수행 결과, PLAN
 *
 * Error handle
 *     - SQL_SUCCESS
 *     - SQL_SUCCESS_WITH_INFO
 *     - SQL_INVALID_HANDLE
 *     - SQL_ERROR
 *
 ***********************************************************************/
    SQLRETURN  sRet = SQL_ERROR;

    IDE_TEST_RAISE( mInitialized == ID_FALSE, UnInitializedError );
    IDE_TEST_RAISE( aRemoteStmt == NULL, ErrorNullSdStmt );
    IDE_TEST_RAISE( aRemoteStmt->mStmt == NULL, ErrorNullStmt );

    SET_LINK_FAILURE_FLAG( aIsLinkFailure, ID_FALSE );

    sRet = SQLGetPlan( (SQLHANDLE)aRemoteStmt->mStmt, (SQLCHAR**)aPlan );
    IDE_TEST_RAISE( sRet==SQL_ERROR, GetPlanErr );

    return IDE_SUCCESS;

    IDE_EXCEPTION_UNINIT_LIBRARY( aConnectInfo, "getPlan" )
    IDE_EXCEPTION_NULL_SD_STMT( aConnectInfo, "getPlan" )
    IDE_EXCEPTION_NULL_STMT( aConnectInfo, "getPlan" )
    IDE_EXCEPTION( GetPlanErr )
    {
        processErrorOnNested( SQL_HANDLE_STMT,
                              aRemoteStmt->mStmt,
                              ((SChar*)"SQLGetPlan"),
                              aConnectInfo,
                              aIsLinkFailure );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdl::setConnAttr( sdiConnectInfo * aConnectInfo,
                         UShort  aAttrType,
                         ULong   aValue,
                         idBool *aIsLinkFailure )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation : SQLSetConnectOption.
 *
 * Arguments
 *     - aConnectInfo   [IN] : data node 접속 정보
 *     - aAttyType      [IN] : Attribute Type
 *     - aValue         [IN] : Attribute's Value.
 *     - aIsLinkFailure [OUT]: Link-실패 여부에 대한 플래그.
 *
 * Return value
 *     -
 *
 * Error handle
 *     - SQL_SUCCESS
 *     - SQL_SUCCESS_WITH_INFO
 *     - SQL_INVALID_HANDLE
 *     - SQL_ERROR
 *
 ***********************************************************************/
    IDE_TEST_RAISE( mInitialized == ID_FALSE, UnInitializedError );
    IDE_TEST_RAISE( aConnectInfo->mDbc == NULL, ErrorNullDbc );

    SET_LINK_FAILURE_FLAG( aIsLinkFailure, ID_FALSE );

    IDE_TEST_RAISE( SQLSetConnectOption( aConnectInfo->mDbc, aAttrType, aValue )
                     != SQL_SUCCESS,
                    SetConnectOptionErr );
    return IDE_SUCCESS;
    IDE_EXCEPTION_UNINIT_LIBRARY( aConnectInfo, "setConnAttr" );
    IDE_EXCEPTION_NULL_DBC( aConnectInfo, "setConnAttr" );
    IDE_EXCEPTION( SetConnectOptionErr )
    {
        processErrorOnNested( SQL_HANDLE_DBC,
                              aConnectInfo->mDbc,
                              ((SChar*)"SQLSetConnectOption"),
                              aConnectInfo,
                              aIsLinkFailure );
    };
    IDE_EXCEPTION_END;
    return IDE_FAILURE;
}

IDE_RC sdl::getConnAttr( sdiConnectInfo * aConnectInfo,
                         UShort           aAttrType,
                         void           * aValuePtr,
                         SInt             aBuffLength,
                         SInt           * aStringLength )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation : SQLGetConnectOption.
 *
 * Arguments
 *     - aConnectInfo   [IN] : data node 접속 정보
 *     - aAttyType     [IN] : Attribute Type
 *     - aValue        [OUT]: Pointer for returned attribute's value.
 *     - aBuffLength   [IN] : aValue의 길이
 *     - aStringLength [OUT]: 반환 된 데이터의 길
 *
 * Return value
 *     - Attribute에 대한 결과 값.
 *
 * Error handle
 *     - SQL_SUCCESS
 *     - SQL_SUCCESS_WITH_INFO
 *     - SQL_INVALID_HANDLE
 *     - SQL_ERROR
 *
 ***********************************************************************/
    IDE_TEST_RAISE( mInitialized == ID_FALSE, UnInitializedError );
    IDE_TEST_RAISE( aConnectInfo->mDbc == NULL, ErrorNullDbc );

    if ( ( aBuffLength <= 0 ) && ( aStringLength == NULL ) )
    {
        IDE_TEST_RAISE( SQLGetConnectOption( aConnectInfo->mDbc, aAttrType, aValuePtr )
                         == SQL_ERROR,
                        GetConnectOptionErr );
    }
    else
    {
        IDE_TEST_RAISE( SQLGetConnectAttr( aConnectInfo->mDbc,
                                           aAttrType,
                                           aValuePtr,
                                           aBuffLength,
                                           aStringLength )
                         == SQL_ERROR,
                        GetConnectOptionErr );
    }

    return IDE_SUCCESS;
    IDE_EXCEPTION_UNINIT_LIBRARY( aConnectInfo, "getConnAttr" );
    IDE_EXCEPTION_NULL_DBC( aConnectInfo, "getConnAttr" );
    IDE_EXCEPTION( GetConnectOptionErr )
    {
        processErrorOnNested( SQL_HANDLE_DBC,
                              aConnectInfo->mDbc,
                              ((SChar*)"SQLGetConnectOption"),
                              aConnectInfo,
                              NULL );
    };
    IDE_EXCEPTION_END;
    return IDE_FAILURE;
}

IDE_RC sdl::endPendingTran( sdiConnectInfo * aConnectInfo,
                            ID_XID         * aXID,
                            idBool           aIsCommit,
                            idBool         * aIsLinkFailure )
{
    IDE_TEST_RAISE( mInitialized == ID_FALSE, UnInitializedError );
    IDE_TEST_RAISE( aConnectInfo->mDbc == NULL, ErrorNullDbc );

    SET_LINK_FAILURE_FLAG( aIsLinkFailure, ID_FALSE );

    if ( aIsCommit == ID_TRUE )
    {
        IDE_TEST_RAISE( SQLEndPendingTran( (SQLHDBC)     aConnectInfo->mDbc,
                                           (SQLUINTEGER) ID_SIZEOF(ID_XID),
                                           (SQLPOINTER*) aXID,
                                           (SQLSMALLINT) SDL_TRANSACT_COMMIT )
                        != SQL_SUCCESS,
                        TransactErr );
    }
    else
    {
        IDE_TEST_RAISE( SQLEndPendingTran( (SQLHDBC)     aConnectInfo->mDbc,
                                           (SQLUINTEGER) ID_SIZEOF(ID_XID),
                                           (SQLPOINTER*) aXID,
                                           (SQLSMALLINT) SDL_TRANSACT_ROLLBACK )
                        != SQL_SUCCESS,
                        TransactErr );
    }

    return IDE_SUCCESS;
    IDE_EXCEPTION_UNINIT_LIBRARY( aConnectInfo, "endPendingTran" );
    IDE_EXCEPTION_NULL_DBC( aConnectInfo, "endPendingTran" );
    IDE_EXCEPTION( TransactErr )
    {
        processErrorOnNested( SQL_HANDLE_DBC,
                              aConnectInfo->mDbc,
                              ((SChar*)"SQLEndPendingTran"),
                              aConnectInfo,
                              aIsLinkFailure );
    };
    IDE_EXCEPTION_END;
    return IDE_FAILURE;
}

IDE_RC sdl::checkNode( sdiConnectInfo * aConnectInfo,
                       idBool         * aIsLinkFailure )
{
    IDE_TEST_RAISE( mInitialized == ID_FALSE, UnInitializedError );
    IDE_TEST_RAISE( aConnectInfo->mDbc == NULL, ErrorNullDbc );

    SET_LINK_FAILURE_FLAG( aIsLinkFailure, ID_FALSE );

    IDE_TEST_RAISE( checkDbcAlive( aConnectInfo->mDbc ) == ID_FALSE,
                    CheckNodeError );

    return IDE_SUCCESS;
    IDE_EXCEPTION_UNINIT_LIBRARY( aConnectInfo, "checkNode" );
    IDE_EXCEPTION_NULL_DBC( aConnectInfo, "checkNode" );
    IDE_EXCEPTION( CheckNodeError )
    {
        processErrorOnNested( SQL_HANDLE_DBC,
                              aConnectInfo->mDbc,
                              ((SChar*)"checkNode"),
                              aConnectInfo,
                              aIsLinkFailure );
    };
    IDE_EXCEPTION_END;
    return IDE_FAILURE;
}

IDE_RC sdl::commit( sdiConnectInfo * aConnectInfo,
                    sdiClientInfo  * aClientInfo,
                    idBool         * aIsLinkFailure )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation : SQLTransact for Commit.
 *
 * Arguments
 *     - aConnectInfo   [IN] : data node 접속 정보
 *     - aClientInfo    [IN/OUT] : Client Info
 *     - aIsLinkFailure [OUT]: Link-실패 여부에 대한 플래그.
 *
 * Return value
 *
 * Error handle
 *     - SQL_SUCCESS
 *     - SQL_SUCCESS_WITH_INFO
 *     - SQL_INVALID_HANDLE
 *     - SQL_ERROR
 *
 ***********************************************************************/
    SQLRETURN  sRet           = SQL_ERROR;

    IDE_TEST_RAISE( mInitialized == ID_FALSE, UnInitializedError );
    IDE_TEST_RAISE( aConnectInfo->mDbc == NULL, ErrorNullDbc );

    SET_LINK_FAILURE_FLAG( aIsLinkFailure, ID_FALSE );

    sRet = SQLTransact( NULL, aConnectInfo->mDbc, SDL_TRANSACT_COMMIT );
    IDE_TEST_RAISE( ( sRet != SQL_SUCCESS ) &&
                    ( sRet != SQL_SUCCESS_WITH_INFO ),
                    TransactErr );

    /* BUG-46100 Session SMN Update */
    sdi::setNeedToDisconnect( aClientInfo, (idBool)SQLGetNeedToDisconnect( aConnectInfo->mDbc ) );

    return IDE_SUCCESS;
    IDE_EXCEPTION_UNINIT_LIBRARY( aConnectInfo, "commit" );
    IDE_EXCEPTION_NULL_DBC( aConnectInfo, "commit" );
    IDE_EXCEPTION( TransactErr )
    {
        processErrorOnNested( SQL_HANDLE_DBC,
                              aConnectInfo->mDbc,
                              ((SChar*)"SQLTransact"),
                              aConnectInfo,
                              aIsLinkFailure );
    };
    IDE_EXCEPTION_END;

    if ( aConnectInfo->mDbc != NULL )
    {
        /* BUG-46100 Session SMN Update */
        sdi::setNeedToDisconnect( aClientInfo, (idBool)SQLGetNeedToDisconnect( aConnectInfo->mDbc ) );
    }
    else
    {
        /* Nothing to do */
    }

    return IDE_FAILURE;
}

IDE_RC sdl::rollback( sdiConnectInfo * aConnectInfo,
                      const SChar    * aSavepoint,
                      sdiClientInfo  * aClientInfo,
                      idBool         * aIsLinkFailure )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation : SQLTransact for rollback.
 *
 * Arguments
 *     - aConnectInfo   [IN] : data node 접속 정보
 *     - aSavePoint     [IN] : SavePoint
 *     - aClientInfo    [IN/OUT] : Client Info
 *     - aIsLinkFailure [OUT]: Link-실패 여부에 대한 플래그.
 *
 * Return value
 *
 * Error handle
 *     - SQL_SUCCESS
 *     - SQL_SUCCESS_WITH_INFO
 *     - SQL_INVALID_HANDLE
 *     - SQL_ERROR
 *
 ***********************************************************************/
    SQLRETURN  sRet           = SQL_ERROR;

    IDE_TEST_RAISE( mInitialized == ID_FALSE, UnInitializedError );

    SET_LINK_FAILURE_FLAG( aIsLinkFailure, ID_FALSE );

    IDE_TEST_RAISE( aConnectInfo->mDbc == NULL, ErrorNullDbc );

    if ( aSavepoint == NULL )
    {
        sRet = SQLTransact( NULL, aConnectInfo->mDbc, SDL_TRANSACT_ROLLBACK );
        IDE_TEST_RAISE( ( sRet != SQL_SUCCESS ) &&
                        ( sRet != SQL_SUCCESS_WITH_INFO ),
                        TransactErr );

        /* BUG-46100 Session SMN Update */
        sdi::setNeedToDisconnect( aClientInfo, (idBool)SQLGetNeedToDisconnect( aConnectInfo->mDbc ) );
    }
    else
    {
        IDE_TEST_RAISE( SQLRollbackToSavepoint( (SQLHDBC)aConnectInfo->mDbc,
                                                (SQLCHAR*)aSavepoint,
                                                (SQLINTEGER)idlOS::strlen( aSavepoint ) )
                        != SQL_SUCCESS, RollbackToSavepointErr );
    }

    return IDE_SUCCESS;
    IDE_EXCEPTION_UNINIT_LIBRARY( aConnectInfo, "rollback" )
    IDE_EXCEPTION_NULL_DBC( aConnectInfo, "rollback" )
    IDE_EXCEPTION( RollbackToSavepointErr )
    {
        processErrorOnNested( SQL_HANDLE_DBC,
                              aConnectInfo->mDbc,
                              ((SChar*)"SQLRollbackToSavepoint"),
                              aConnectInfo,
                              aIsLinkFailure );
    };
    IDE_EXCEPTION( TransactErr )
    {
        processErrorOnNested( SQL_HANDLE_DBC,
                              aConnectInfo->mDbc,
                              ((SChar*)"SQLTransact" ),
                              aConnectInfo,
                              aIsLinkFailure );
    }
    IDE_EXCEPTION_END;

    if ( ( aSavepoint == NULL ) && ( aConnectInfo->mDbc != NULL ) )
    {
        /* BUG-46100 Session SMN Update */
        sdi::setNeedToDisconnect( aClientInfo, (idBool)SQLGetNeedToDisconnect( aConnectInfo->mDbc ) );
    }
    else
    {
        /* Nothing to do */
    }

    return IDE_FAILURE;
}

IDE_RC sdl::setSavepoint( sdiConnectInfo * aConnectInfo,
                          const SChar    * aSavepoint,
                          idBool         * aIsLinkFailure )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation : Set savepoint.
 *
 * Arguments
 *     - aConnectInfo   [IN] : data node 접속 정보
 *     - aSavepoint     [IN] : Savepoint
 *     - aIsLinkFailure [OUT]: Link-실패 여부에 대한 플래그.
 *
 * Return value
 *
 * Error handle
 *     - SQL_SUCCESS
 *     - SQL_SUCCESS_WITH_INFO
 *     - SQL_INVALID_HANDLE
 *     - SQL_ERROR
 *
 ***********************************************************************/

    SET_LINK_FAILURE_FLAG( aIsLinkFailure, ID_FALSE );

    IDE_TEST_RAISE( mInitialized == ID_FALSE, UnInitializedError );
    IDE_TEST_RAISE( aConnectInfo->mDbc == NULL, ErrorNullDbc );

    IDE_TEST_RAISE( SQLSetSavepoint( (SQLHDBC)aConnectInfo->mDbc,
                                     (const SQLCHAR*)aSavepoint,
                                     (SQLINTEGER)idlOS::strlen( aSavepoint ) )
                    != SQL_SUCCESS, ExecDirectErr );
                                    
    return IDE_SUCCESS;

    IDE_EXCEPTION_UNINIT_LIBRARY( aConnectInfo, "setSavepoint" )
    IDE_EXCEPTION_NULL_DBC( aConnectInfo, "setSavepoint" )
    IDE_EXCEPTION( ExecDirectErr )
    {
        processErrorOnNested( SQL_HANDLE_DBC,
                              aConnectInfo->mDbc,
                              ((SChar*)"setSavepoint"),
                              aConnectInfo,
                              aIsLinkFailure );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdl::shardStmtPartialRollback( sdiConnectInfo * aConnectInfo,
                                      idBool         * aIsLinkFailure )
{
    SET_LINK_FAILURE_FLAG( aIsLinkFailure, ID_FALSE );

    IDE_TEST_RAISE( mInitialized == ID_FALSE, UnInitializedError );
    IDE_TEST_RAISE( aConnectInfo->mDbc == NULL, ErrorNullDbc );

    IDE_TEST_RAISE( SQLShardStmtPartialRollback( (SQLHDBC)aConnectInfo->mDbc ) 
                    != SQL_SUCCESS, ExecDirectErr );

    return IDE_SUCCESS;

    IDE_EXCEPTION_UNINIT_LIBRARY( aConnectInfo, "shardStmtPartialRollback" );
    IDE_EXCEPTION_NULL_DBC( aConnectInfo, "shardStmtPartialRollback" );
    IDE_EXCEPTION( ExecDirectErr )
    {
        processErrorOnNested( SQL_HANDLE_DBC,
                              aConnectInfo->mDbc,
                              ((SChar*)"SQLShardStmtPartialRollback"),
                              aConnectInfo,
                              aIsLinkFailure );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

idBool sdl::checkDbcAlive( void   * aDbc )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation : Check dbc is alive.
 *
 * Arguments
 *     - aDbc               [IN] : data node 에 대한 DBC
 *
 * Return value
 *  - DBC가 살아 있는지에 대한 결과 값
 * Error handle
 *     - SQL_SUCCESS
 *     - SQL_SUCCESS_WITH_INFO
 *     - SQL_INVALID_HANDLE
 *     - SQL_ERROR
 *
 ***********************************************************************/
    SQLUINTEGER sValue = SQL_FALSE;

    IDE_DASSERT( mInitialized == ID_TRUE );
    IDE_DASSERT( aDbc != NULL );

    IDE_TEST_CONT( SQLGetConnectOption( aDbc,
                                        SDL_SQL_ATTR_CONNECTION_DEAD,
                                        &sValue )
                   != SQL_SUCCESS, SET_FALSE );

    IDE_TEST_CONT( sValue == SQL_TRUE, SET_FALSE );

    return ID_TRUE;

    IDE_EXCEPTION_CONT( SET_FALSE );

    return ID_FALSE;
}

IDE_RC sdl::checkNeedFailover( sdiConnectInfo * aConnectInfo,
                               idBool         * aNeedFailover )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation : Diag Record 의 에러 코드를 이용하여
 *                  failover 가 필요한 상황인지 확인한다.
 *
 * Arguments
 *     - aConnectInfo   [IN] : data node 접속 정보
 *     - aNeedFailover  [OUT]: failover 필요 여부에 대한 결과 값
 *
 * Return value
 *  - failover 필요 여부에 대한 결과 값
 *
 * Error handle
 *     - IDE_SUCCESS
 *     - IDE_FAILURE
 *
 ***********************************************************************/
    SQLINTEGER sValue = SQL_FALSE;

    IDE_TEST_RAISE( mInitialized == ID_FALSE, UnInitializedError );
    IDE_TEST_RAISE( aConnectInfo->mDbc == NULL, ErrorNullDbc );

    IDE_TEST_RAISE( SQLGetNeedFailover( SQL_HANDLE_DBC,
                                        aConnectInfo->mDbc,
                                        &sValue )
                    != SQL_SUCCESS,
                    GetNeedFailoverErr );

    *aNeedFailover = ( sValue == SQL_TRUE ? ID_TRUE : ID_FALSE );

    return IDE_SUCCESS;

    IDE_EXCEPTION_UNINIT_LIBRARY( aConnectInfo, "checkNeedFailover" )
    IDE_EXCEPTION_NULL_DBC( aConnectInfo, "checkNeedFailover" )
    IDE_EXCEPTION( GetNeedFailoverErr )
    {
        processErrorOnNested( SQL_HANDLE_DBC,
                              aConnectInfo->mDbc,
                              ((SChar*)"SQLGetNeedFailover"),
                              aConnectInfo,
                              NULL );
    }
    IDE_EXCEPTION_END;

    *aNeedFailover = ID_FALSE;

    return IDE_FAILURE;
}

idBool sdl::retryConnect( sdiConnectInfo * aConnectInfo,
                          SShort           aHandleType,
                          void           * aHandle )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation :
 *   STF Failover 로직은 아래와 같이 동작한다.
 *     1. 기존 접속되어있던 서버로 재접속을 시도하여
 *     재접속을 실패하게 되면
 *     2. 가용한 Alternate 서버로 접속을 시도한다.
 *
 *   위 로직에서 1번의 재접속을 시도하는 함수이다.
 *
 * Arguments
 *     - aConnectInfo   [IN] : data node 접속 정보
 *     - aHandleType    [IN] : Handle type. ( DBC, STMT )
 *     - aHandle        [IN] : Handle object.
 *
 * Return value
 *  - 재접속 성공 여부
 *
 * Error handle
 *     - ID_TRUE
 *     - ID_FALSE
 *
 ***********************************************************************/
    IDE_TEST_CONT( mInitialized == ID_FALSE, SET_FALSE );

    /* aConnectInfo->mDbc == NULL 인 경우는 최초 접속이므로
     * 이 함수가 아니라 sdl::internalConnect() 함수에서 재시도를 수행해야 한다.
     */
    IDE_TEST_CONT( aConnectInfo->mDbc == NULL, SET_FALSE );

    IDE_TEST_CONT( ( aHandleType != SQL_HANDLE_DBC ) &&
                   ( aHandleType != SQL_HANDLE_STMT ),  SET_FALSE );

    IDE_TEST_CONT( aHandle == NULL, SET_FALSE );

    IDE_TEST_CONT( SQLReconnect( aHandleType, aHandle ) != SQL_SUCCESS, SET_FALSE );

    return ID_TRUE;

    IDE_EXCEPTION_CONT( SET_FALSE );

    return ID_FALSE;
}

IDE_RC sdl::getLinkInfo( sdiConnectInfo * aConnectInfo,
                         SChar  *aBuf,
                         UInt    aBufSize,
                         SInt    aKey )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation : Get link information for dbc.
 *
 * Arguments
 *     - aConnectInfo   [IN] : data node 접속 정보
 *     - aNodeName      [IN] : 노드 이름. 에러 처리 용.
 *     - aBuf           [OUT]: DBC의 링크 정보
 *     - aIsLinkFailure [OUT]: Link-실패 여부에 대한 플래그.
 *
 * Return value
 *  - DBC의 링크 정보
 * Error handle
 *     - SQL_SUCCESS
 *     - SQL_SUCCESS_WITH_INFO
 *     - SQL_INVALID_HANDLE
 *     - SQL_ERROR
 *
 ***********************************************************************/
    IDE_TEST_RAISE( mInitialized == ID_FALSE, UnInitializedError );
    IDE_TEST_RAISE( aConnectInfo->mDbc == NULL, ErrorNullDbc );

    SQLGetDbcLinkInfo( aConnectInfo->mDbc, (SQLCHAR*)aBuf, (SQLINTEGER)aBufSize, (SQLINTEGER)aKey );

    return IDE_SUCCESS;
    IDE_EXCEPTION_UNINIT_LIBRARY( aConnectInfo, "getLinkInfo" )
    IDE_EXCEPTION_NULL_DBC( aConnectInfo, "getLinkInfo" )
    IDE_EXCEPTION_END;
    return IDE_FAILURE;
}

IDE_RC sdl::isDataNode( sdiConnectInfo * aConnectInfo,
                        idBool * aIsDataNode )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation : DataNode에 대한 정보를 가져온다.
 *
 * Arguments
 *     - aConnectInfo   [IN] : data node 접속 정보
 *     - aIsDataNode   [OUT] : data_node 인지 아닌지에 대한 플래그
 *
 * Return value
 *  - IDE_FAILURE/IDE_SUCCESS
 * Error handle
 *
 ***********************************************************************/
    SQLUSMALLINT sIsDbc = 0;

    IDE_TEST_RAISE( mInitialized == ID_FALSE, UnInitializedError );
    IDE_TEST_RAISE( aConnectInfo->mDbc == NULL, ErrorNullDbc );
    IDE_TEST( aIsDataNode == NULL );

    IDE_TEST( SQLGetConnectAttr( aConnectInfo->mDbc,
                                 SDL_ALTIBASE_SHARD_LINKER_TYPE,
                                 &sIsDbc,
                                 ID_SIZEOF( SQLUSMALLINT ),
                                 NULL ) == SQL_ERROR );

    *aIsDataNode = ( sIsDbc == 1 ? ID_FALSE : ID_TRUE );

    return IDE_SUCCESS;

    IDE_EXCEPTION_UNINIT_LIBRARY( aConnectInfo, "isDataNode" )
    IDE_EXCEPTION_NULL_DBC( aConnectInfo, "isDataNode" )
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

void sdl::processError( SShort           aHandleType,
                        void           * aHandle,
                        SChar          * aCallFncName,
                        sdiConnectInfo * aConnectInfo,
                        idBool         * aIsLinkFailure )
{
/***********************************************************************
 *
 * Description :
 *     SQL함수들의 에러를 ideError로 변경한다. 일단은 4개만
 *
 * Implementation :
 *
 ***********************************************************************/
    typedef enum
    {
        SDL_ERROR_OTHER,
        SDL_ERROR_FAILOVER_SUCCESS,
        SDL_ERROR_FATAL
    } sdlErrorType;

    SQLRETURN   sSqlReturn      ;
    SInt        sIntReturn      ;
    SQLCHAR     sSQLSTATE       [SQL_SQLSTATE_SIZE + 1];
    SQLINTEGER  sNativeError    ;
    SQLCHAR     sMessage        [MAX_ODBC_MSG_LEN];
    SQLSMALLINT sMessageLength  ; 
    SChar       sErrorMsg       [MAX_LAST_ERROR_MSG_LEN];
    SChar      *sErrorMsgPos    = sErrorMsg;
    SInt        sErrorMsgRemain = ID_SIZEOF(sErrorMsg);
    SQLINTEGER  sIsNeedFailover = 0;
    idBool      sIsRetrySuccess = ID_FALSE;
    UInt        sNewErrorCode   = sdERR_ABORT_SHARD_LIBRARY_ERROR_1;
    UInt        i               ;

    SChar      *sNodeNamePtr = NULL;

    sdlErrorType    sErrorType = SDL_ERROR_OTHER;

    sNodeNamePtr = (SChar*)aConnectInfo->mNodeName;

    SET_LINK_FAILURE_FLAG( aIsLinkFailure, ID_FALSE );

    IDE_TEST_RAISE( mInitialized == ID_FALSE, UnInitializedError );
    if ( aHandleType == SQL_HANDLE_STMT )
    {
        IDE_TEST_RAISE( aHandle == NULL, ErrorNullStmt );
    }
    else
    {
        IDE_TEST_RAISE( aHandle == NULL, ErrorNullDbc );
    }

    sErrorMsgPos[0] = '\0';

    for ( i = 0; i < MAX_ODBC_ERROR_CNT; i++ )
    {
        sSqlReturn = SQLGetDiagRec( aHandleType,
                                    aHandle,
                                    i + 1,  // record no
                                    (SQLCHAR*)sSQLSTATE,
                                    (SQLINTEGER*)&sNativeError,
                                    sMessage,
                                    MAX_ODBC_MSG_LEN,
                                    &sMessageLength );

        if ( ( sSqlReturn == SQL_SUCCESS ) || ( sSqlReturn == SQL_SUCCESS_WITH_INFO ) )
        {
            sIntReturn = idlOS::snprintf( sErrorMsgPos,
                                          sErrorMsgRemain,
                                          "\nDiagnostic Record %"ID_UINT32_FMT"\n"
                                          "     SQLSTATE     : %s\n"
                                          "     Message text : %s\n"
                                          "     Message len  : %"ID_UINT32_FMT"\n"
                                          "     Native error : 0x%"ID_XINT32_FMT,
                                          i + 1, 
                                          sSQLSTATE,
                                          sMessage,
                                          (UInt)sMessageLength,
                                          sNativeError );

            sErrorMsgPos += sIntReturn;
            sErrorMsgRemain -= sIntReturn;

            if ( sNativeError == SD_ALTIBASE_FAILOVER_SUCCESS )
            {
                sErrorType = SDL_ERROR_FAILOVER_SUCCESS;
                break;
            }
            else
            {
                // Nothing to do.
            }
        }
        else
        {
            // SQL_NO_DATA등
            break;
        }
    }

    if ( sErrorType != SDL_ERROR_FAILOVER_SUCCESS )
    {
        if ( aConnectInfo->mDbc != NULL )
        {
            /*  aConnectInfo->mDbc != NULL : STF case
             *  어떤 오류가 발생했던간에
             *  한번 연결되었던 Connection 은 
             *  sdl::retryConnect 함수에서 재연결 한다.
             */
            if ( checkDbcAlive( aConnectInfo->mDbc ) == ID_FALSE )
            {
                sErrorType = SDL_ERROR_FATAL;
            }
        }
        else
        {
            if ( aHandleType == SQL_HANDLE_DBC )
            {
                /*  aConnectInfo->mDbc == NULL : Initial connect case
                 *  최초 접속이 실패 한 경우.
                 *  sdl::internalConnect 함수에서 Connect 재시도 수행을 이미 완료했다.
                 *  이곳에서 Connect 에러 타입에 따라
                 *  Link failure 에러 코드를 반환할지 여부를 결정한다.
                 *  예) Invalid password 인 경우는 Link failure 에러 코드를 반환하지 않는다.
                 */
                sSqlReturn = SQLGetNeedFailover( (SQLSMALLINT)aHandleType,
                                                 (SQLHANDLE)aHandle,
                                                 &sIsNeedFailover );
                if ( ( sSqlReturn == SQL_SUCCESS ) && ( sIsNeedFailover == 1 ) )
                {
                    sErrorType = SDL_ERROR_FATAL;
                }
            }
            else
            {
                /* No DBC */
            }
        }
    }

    /* Get new error code and reconnect if need. */
    switch ( sErrorType )
    {
        case SDL_ERROR_FAILOVER_SUCCESS:
            setTransactionBrokenByInternalConnection( aConnectInfo );

            sNewErrorCode = sdERR_ABORT_SHARD_LIBRARY_FAILOVER_SUCCESS;
            break;

        case SDL_ERROR_FATAL:
            setTransactionBrokenByInternalConnection( aConnectInfo );

            /* aConnectInfo->mDbc == NULL : Initial connect case
             * aConnectInfo->mDbc != NULL : STF case
             */
            if ( aConnectInfo->mDbc != NULL )
            {
                switch ( aConnectInfo->mFailoverTarget )
                {
                    case SDI_FAILOVER_ACTIVE_ONLY    :
                    case SDI_FAILOVER_ALTERNATE_ONLY :
                        sIsRetrySuccess = retryConnect( aConnectInfo,
                                                        aHandleType,
                                                        aHandle );
                        break;

                    case SDI_FAILOVER_ALL :
                        if ( hasNodeAlternate( &aConnectInfo->mNodeInfo ) == ID_FALSE )
                        {
                            sIsRetrySuccess = retryConnect( aConnectInfo,
                                                            aHandleType,
                                                            aHandle );
                        }
                        else
                        {
                            sIsRetrySuccess = ID_FALSE;
                        }
                        break;

                    default:
                        sIsRetrySuccess = ID_FALSE;
                        IDE_DASSERT( 0 );
                        break;
                }
            }

            if ( sIsRetrySuccess == ID_TRUE )
            {
                sNewErrorCode = sdERR_ABORT_SHARD_LIBRARY_FAILOVER_SUCCESS;
            }
            else
            {
                SET_LINK_FAILURE_FLAG( aIsLinkFailure, ID_TRUE );

                sNewErrorCode = sdERR_ABORT_SHARD_LIBRARY_LINK_FAILURE_ERROR;
            }
            break;

        default:
            sNewErrorCode = sdERR_ABORT_SHARD_LIBRARY_ERROR_1;
            break;

    }

    /* Set additional error info for shardcli. */
    if ( aConnectInfo->mIsShardClient == SDI_SHARD_CLIENT_TRUE )
    {
        sIntReturn = idlOS::snprintf( sErrorMsgPos,
                                      sErrorMsgRemain,
                                      "\n     Foot print   : [<<ERC-%"ID_XINT32_FMT" NODE-ID:%"ID_UINT32_FMT">>]",
                                      E_ERROR_CODE( sNewErrorCode ),
                                      aConnectInfo->mNodeInfo.mNodeId );

        sErrorMsgPos += sIntReturn;
        sErrorMsgRemain -= sIntReturn;
    }


    /* Set error by new error code. */
    switch ( sNewErrorCode )
    {
        case sdERR_ABORT_SHARD_LIBRARY_FAILOVER_SUCCESS :
            IDE_SET( ideSetErrorCode( sdERR_ABORT_SHARD_LIBRARY_FAILOVER_SUCCESS,
                                      sNodeNamePtr,
                                      aCallFncName,
                                      sErrorMsg ) );
            break;

        case sdERR_ABORT_SHARD_LIBRARY_LINK_FAILURE_ERROR :
            IDE_SET( ideSetErrorCode( sdERR_ABORT_SHARD_LIBRARY_LINK_FAILURE_ERROR,
                                      sNodeNamePtr,
                                      aCallFncName,
                                      sErrorMsg ) );
            break;

        case sdERR_ABORT_SHARD_LIBRARY_ERROR_1 :
            IDE_SET( ideSetErrorCode( sdERR_ABORT_SHARD_LIBRARY_ERROR_1,
                                      sNodeNamePtr,
                                      aCallFncName,
                                      sErrorMsg ) );
            break;

        default:
            IDE_DASSERT( 0 );
            IDE_SET( ideSetErrorCode( sdERR_ABORT_SDC_UNEXPECTED_ERROR,
                                      "sdl::processError",
                                      "Unexpected error code" ) );
            break;
    }

    switch ( sNewErrorCode )
    {
        case sdERR_ABORT_SHARD_LIBRARY_FAILOVER_SUCCESS :
        case sdERR_ABORT_SHARD_LIBRARY_LINK_FAILURE_ERROR :
            ideLog::log( IDE_SD_0, "[%s] ERR-%"ID_XINT32_FMT" %s\n"
                                   "     %s\n",
                                   aCallFncName,
                                   E_ERROR_CODE( ideGetErrorCode() ),
                                   ideGetErrorMsg(),
                                   aConnectInfo->mFullAddress );
            break;

        default:
            /* Nothing to do. */
            break;
    }

    return;

    IDE_EXCEPTION_UNINIT_LIBRARY( aConnectInfo, "processError" )
    IDE_EXCEPTION_NULL_DBC( aConnectInfo, "processError" )
    IDE_EXCEPTION_NULL_STMT( aConnectInfo, "processError" )
    IDE_EXCEPTION_END;
    return;
}

void sdl::appendFootPrintToErrorMessage( sdiConnectInfo * aConnectInfo )
{
    SInt    sTextLen;
    SInt    sRemainErrorTextLen;
    SChar * sNewErrorTextPtr;

    if ( aConnectInfo->mIsShardClient == SDI_SHARD_CLIENT_TRUE )
    {
        sTextLen            = idlOS::strlen( ideGetErrorMsg() );
        sRemainErrorTextLen = MAX_LAST_ERROR_MSG_LEN - sTextLen;
        sNewErrorTextPtr    = ideGetErrorMsg() + sTextLen;

        if ( sRemainErrorTextLen > 0 )
        {
            idlOS::snprintf( sNewErrorTextPtr,
                             sRemainErrorTextLen,
                             "\n    Foot print   : [<<ERC-%"ID_XINT32_FMT" NODE-ID:%"ID_UINT32_FMT">>]",
                             E_ERROR_CODE( ideGetErrorCode() ),
                             aConnectInfo->mNodeInfo.mNodeId );
        }
        else
        {
            /* Buffer overflow */
        }
    }
}

void sdl::appendResultInfoToErrorMessage( sdiConnectInfo    * aConnectInfo,
                                          qciStmtType         aStmtKind )
{
    SInt          sTextLen;
    SInt          sRemainErrorTextLen;
    SChar       * sNewErrorTextPtr;
    const SChar * sInfoDescription = NULL;

    IDE_TEST_CONT( sdi::isAffectedRowSetted( aConnectInfo ) != ID_TRUE,
                   NORMAL_EXIT );

    switch ( aStmtKind )
    {
        case QCI_STMT_SELECT:
            IDE_CONT( NORMAL_EXIT );
        break;

        case QCI_STMT_UPDATE:
        case QCI_STMT_DELETE:
        case QCI_STMT_SELECT_FOR_UPDATE:
            sInfoDescription = "affected";
            break;

        case QCI_STMT_INSERT:
            sInfoDescription = "inserted";
            break;

        default:
            IDE_DASSERT( 0 );
            sInfoDescription = (SChar *)"affected";
            break;
    }

    sTextLen            = idlOS::strlen( ideGetErrorMsg() );
    sRemainErrorTextLen = MAX_LAST_ERROR_MSG_LEN - sTextLen;
    sNewErrorTextPtr    = ideGetErrorMsg() + sTextLen;

    IDE_DASSERT( sTextLen > 0 );

    if ( sRemainErrorTextLen > 0 )
    {
        idlOS::snprintf( sNewErrorTextPtr,
                         sRemainErrorTextLen,
                         "\nNode=%s: %"ID_vSLONG_FMT" row %s.",
                         aConnectInfo->mNodeName,
                         aConnectInfo->mAffectedRowCount,
                         sInfoDescription );
    }
    else
    {
        /* Buffer overflow */
    }

    IDE_EXCEPTION_CONT( NORMAL_EXIT );
}

IDE_RC sdl::getConnectedLinkFullAddress( sdiConnectInfo * aConnectInfo )
{
    SChar sPortNum[SDI_PORT_NUM_BUFFER_SIZE];

    aConnectInfo->mFullAddress[0] = '\0';
    aConnectInfo->mServerIP[0] = '\0';
    aConnectInfo->mPortNo = 0;

    IDE_TEST( getLinkInfo( aConnectInfo,
                           aConnectInfo->mServerIP,
                           ID_SIZEOF(aConnectInfo->mServerIP),
                           CMN_LINK_INFO_REMOTE_IP_ADDRESS )
              != IDE_SUCCESS );
    
    IDE_TEST( getLinkInfo( aConnectInfo,
                           sPortNum,
                           ID_SIZEOF(sPortNum),
                           CMN_LINK_INFO_REMOTE_PORT )
              != IDE_SUCCESS );

    aConnectInfo->mPortNo = (UShort)idlOS::atoi( sPortNum );

    idlOS::snprintf( aConnectInfo->mFullAddress,
                     ID_SIZEOF(aConnectInfo->mFullAddress),
                     "%s,%s:%s",
                     aConnectInfo->mNodeName,
                     aConnectInfo->mServerIP,
                     sPortNum );

    return IDE_SUCCESS;
    IDE_EXCEPTION_END;
    return IDE_FAILURE;
}

void sdl::makeIdeErrorBackup( sdlNestedErrorMgr * aMgr )
{
    UInt   sLen = 0;
    idBool sBackupErrorCode = ID_TRUE;

    /* Only Sharding error will be merged. */
    sBackupErrorCode = ( ( ideGetErrorCode() & E_MODULE_MASK ) == E_MODULE_SD )
                       ? ID_TRUE
                       : ID_FALSE;

    /* Backup current message */
    if ( ( sBackupErrorCode == ID_TRUE ) && ( *ideGetErrorMsg() != '\0' ) )
    {
        aMgr->mOldErrorCode = ideGetErrorCode();

        sLen = idlOS::snprintf( aMgr->mErrorMsgBuffer,
                                ID_SIZEOF( aMgr->mErrorMsgBuffer ),
                                "%s", 
                                ideGetErrorMsg() );

        aMgr->mErrorMsgBufferLen = sLen;
    }
    else
    {
        aMgr->mErrorMsgBuffer[0] = '\0';
        aMgr->mErrorMsgBufferLen = 0;
    }
}

void sdl::mergeIdeErrorWithBackup( sdlNestedErrorMgr * aMgr )
{
    SInt    sRemainErrorTextLen;

    /* Packaging old and new error */
    if ( aMgr->mErrorMsgBufferLen != 0 )
    {
        sRemainErrorTextLen = ID_SIZEOF( aMgr->mErrorMsgBuffer ) - aMgr->mErrorMsgBufferLen;

        if ( sRemainErrorTextLen > 0 )
        {
            idlOS::snprintf( aMgr->mErrorMsgBuffer + aMgr->mErrorMsgBufferLen,
                             sRemainErrorTextLen,
                             "\n%s",
                             ideGetErrorMsg() );

            IDE_SET( ideSetErrorCodeAndMsg( aMgr->mOldErrorCode, aMgr->mErrorMsgBuffer ) );
        }
        else
        {
            /* Buffer overflow */
        }
    }
}

void sdl::processErrorOnNested( SShort           aHandleType,
                                void           * aHandle,
                                SChar          * aCallFncName,
                                sdiConnectInfo * aConnectInfo,
                                idBool         * aIsLinkFailure )
{
    sdlNestedErrorMgr sMgr;

    /* Backup current message */
    makeIdeErrorBackup( &sMgr );

    /* set ide error */
    processError( aHandleType,
                  aHandle,
                  aCallFncName,
                  aConnectInfo,
                  aIsLinkFailure );

    /* Packaging old and new error */
    mergeIdeErrorWithBackup( &sMgr );
}

void sdl::clearInternalConnectResult( sdiConnectInfo * aConnectInfo )
{
    UInt i;

    for ( i = 0; i < SDI_FAILOVER_MAX; ++i )
    {
        aConnectInfo->mIsConnectSuccess[ i ] = ID_TRUE;
    }
}

void sdl::setInternalConnectFailure( sdiConnectInfo * aConnectInfo )
{
    if ( aConnectInfo->mFailoverTarget < SDI_FAILOVER_MAX )
    {
        aConnectInfo->mIsConnectSuccess[ aConnectInfo->mFailoverTarget ] = ID_FALSE;
    }
    else
    {
        IDE_DASSERT( 0 );
    }
}

IDE_RC sdl::internalConnect( void           * aDbc,
                             sdiConnectInfo * aConnectInfo,
                             UChar          * aConnectString,
                             idBool           aIsShardClient )
{
    IDE_RC sConnectResult = IDE_FAILURE;

    /*  internalConnectCore() 함수는 Data node로 internal connect 를 시도하고
     *  그에 대한 성공/실패 결과인 sIdeReturn 값을 반한한다.
     */
    sConnectResult = internalConnectCore( aDbc,
                                          aConnectInfo,
                                          aConnectString );

    if ( aIsShardClient == ID_TRUE )
    {
        /*  - shardcli is used.
         *  - internal connect 결과 값을 이용하여
         *    Server-side failover 를 계속 수행할 것인가에 대해 판단한다.
         *    Active/Alternate 노드에서 연속된 Failover 가 발생한 경우 
         *    sdERR_ABORT_SHARD_NODE_FAIL_RETRY_AVAILABLE 에러 코드를 설정하고
         *    IDE_FAILURE 를 리턴하여 failover 가 중단되도록 한다.
         */
        if ( sConnectResult == IDE_SUCCESS )
        {
            clearInternalConnectResult( aConnectInfo );
        }
        else
        {
            /* Internal connect is failed */
            setInternalConnectFailure( aConnectInfo );

            if ( isInternalConnectFailoverLimited( aConnectInfo ) == ID_TRUE )
            {
                clearInternalConnectResult( aConnectInfo );

                /* replace error code */
                IDE_SET( ideSetErrorCode( sdERR_ABORT_SHARD_NODE_FAILOVER_IS_NOT_AVAILABLE ) );
                appendFootPrintToErrorMessage( aConnectInfo );
            }
        }
    }
    else
    {
        /* Nothing to do */
    }

    return sConnectResult;
}

idBool sdl::isInternalConnectFailoverLimited( sdiConnectInfo * aConnectInfo )
{
    return ( ( aConnectInfo->mIsConnectSuccess[SDI_FAILOVER_ACTIVE_ONLY] == ID_FALSE ) &&
             ( aConnectInfo->mIsConnectSuccess[SDI_FAILOVER_ALTERNATE_ONLY] == ID_FALSE ) )
           ? ID_TRUE
           : ID_FALSE;
}

inline void sdl::setTransactionBrokenByInternalConnection( sdiConnectInfo * aConnectInfo )
{
    if ( ( ( aConnectInfo->mFlag & SDI_CONNECT_USER_AUTOCOMMIT_MODE_MASK )
           == SDI_CONNECT_USER_AUTOCOMMIT_MODE_OFF ) &&
         ( ( aConnectInfo->mFlag & SDI_CONNECT_INITIAL_BY_NOTIFIER_MASK )
           == SDI_CONNECT_INITIAL_BY_NOTIFIER_FALSE ) )
    {
        IDE_DASSERT( aConnectInfo->mSession->mMmSession != NULL );

        (void) qdkSetTransactionBrokenOnGlobalCoordinator(
                aConnectInfo->mDkiSession,
                qci::mSessionCallback.mGetTrans( aConnectInfo->mSession->mMmSession )->getTransID() );
    }
}

void sdl::setTransactionBrokenByTransactionID( idBool     aIsUserAutoCommit,
                                               void     * aDkiSession,
                                               smiTrans * aTrans )
{
    if ( aIsUserAutoCommit == ID_FALSE )
    {
        IDE_DASSERT( aDkiSession != NULL );
        IDE_DASSERT( aTrans != NULL );

        (void) qdkSetTransactionBrokenOnGlobalCoordinator( aDkiSession, aTrans->getTransID() );
    }
}
