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
 * $Id: rpxSenderHandshake.cpp 82075 2018-01-17 06:39:52Z jina.kim $
 **********************************************************************/

#include <idl.h>
#include <ide.h>
#include <idm.h>

#include <smi.h>
#include <smErrorCode.h>

#include <qci.h>

#include <rpDef.h>
#include <rpuProperty.h>
#include <rpcHBT.h>
#include <rpcManager.h>
#include <rpxSender.h>
#include <rpnComm.h>

/* PROJ-2240 */
#include <rpdCatalog.h>

//===================================================================
//
// Name:          attemptHandshake
//
// Return Value:  IDE_SUCCESS/IDE_FAILURE
//
// Argument:
//
// Called By:     rpxSender::run() or rpcManager::startSenderThread()
//
// Description:
//
//===================================================================

IDE_RC
rpxSender::attemptHandshake(idBool *aHandshakeFlag)
{
    SInt         sHostNum;
    SInt         sOldHostNum;
    rpMsgReturn  sRC             = RP_MSG_DISCONNECT;
    IDE_RC       sConnSuccess    = IDE_FAILURE;
    idBool       sRegistHost     = ID_FALSE;
    idBool       sTraceLogEnable = ID_TRUE;
    smSN         sReceiverXSN    = SM_SN_NULL;

    *aHandshakeFlag = ID_FALSE;

    IDE_TEST(rpdCatalog::getIndexByAddr(mMeta.mReplication.mLastUsedHostNo,
                                     mMeta.mReplication.mReplHosts,
                                     mMeta.mReplication.mHostCount,
                                     &sHostNum)
             != IDE_SUCCESS);

    sOldHostNum = sHostNum;

    // To Fix PR-4064
    idlOS::memset( mRCMsg, 0x00, RP_ACK_MSG_LEN );

    //--------------------------------------------------------------//
    //    handshake until success
    //--------------------------------------------------------------//
    while(checkInterrupt() != RP_INTR_EXIT)
    {
        IDE_CLEAR();
        sConnSuccess = IDE_FAILURE;

        //--------------------------------------------------------------//
        //    connect to Peer Server
        //--------------------------------------------------------------//
        /*
         *   IDE_SUCCESS : aRC is      ID_FALSE, ID_TRUE
         *   IDE_FAILURE : aRC is only ID_FALSE (create socket error)
         */
        do
        {
            sConnSuccess = connectPeer(sHostNum);

            if(sConnSuccess != IDE_SUCCESS)
            {
                IDE_TEST( isParallelChild() == ID_TRUE );
                if(mCurrentType != RP_OFFLINE)
                {
                    IDE_TEST(getNextLastUsedHostNo(&sHostNum) != IDE_SUCCESS);
                }

                IDE_TEST( ( mTryHandshakeOnce == ID_TRUE ) &&
                          ( sHostNum == sOldHostNum ) );
                /* ��Ʈ��ũ ��� �߻� �� ������ �Ͽ��µ��� ������ �ȵǴ� ��쿡��
                 * ���� ��� ��Ȳ���� �Ǵ��� 
                 */
                if( ( mIsRemoteFaultDetect == ID_TRUE ) && 
                    ( rpcManager::isStartupFailback() != ID_TRUE ) &&
                    ( isParallelChild() != ID_TRUE ) ) /*�ǹ̸� ��Ȯ�� �ϱ� ���� ���ܵ�*/ 
                {
                    // REMOTE_FAULT_DETECT_TIME �÷��� ���� �ð����� �����Ѵ�.
                    IDE_TEST( updateRemoteFaultDetectTime() != IDE_SUCCESS );
                }
                else
                {
                    /* nothing to do */
                }
                sleepForNextConnect();
            }
            else
            {
                break;
            }
        }
        // Parallel Child�� Network ��� �� Exit Flag�� �����ϱ� ����,
        // checkInterrupt()�� ����ؾ� �Ѵ�.
        while(checkInterrupt() != RP_INTR_EXIT);

        if(sConnSuccess == IDE_SUCCESS)
        {
            //--------------------------------------------------------------//
            //    Connect Success & check Replication Available
            //--------------------------------------------------------------//
            IDE_TEST(checkReplAvailable(&sRC, &mFailbackStatus, &sReceiverXSN) 
                     != IDE_SUCCESS);

            switch(sRC)
            {
                case RP_MSG_OK :            // success return
                    // PROJ-1537
                    if((mMeta.mReplication.mRole != RP_ROLE_ANALYSIS) && 
                       (mCurrentType != RP_OFFLINE)) //PROJ-1915
                    {
                        IDE_TEST( rpcHBT::registHost( &mRsc,
                                                     mMeta.mReplication.mReplHosts[sHostNum].mHostIp,
                                                     mMeta.mReplication.mReplHosts[sHostNum].mPortNo )
                                 != IDE_SUCCESS );
                        /*
                         *  unSet HBT Resource to Messenger
                         */
                        mMessenger.setHBTResource( mRsc );

                        sRegistHost = ID_TRUE;
                    }
                    IDE_CONT(VALIDATE_SUCCESS);

                case RP_MSG_DISCONNECT :    // continue to re-connect
                    // checkReplAvailable Network Error  ===> retry
                    disconnectPeer();
                    IDE_TEST_RAISE(isParallelChild() == ID_TRUE, ERR_NOT_AVAILABLE);
                    IDE_TEST(getNextLastUsedHostNo(&sHostNum) != IDE_SUCCESS);

                    IDE_TEST_RAISE((mTryHandshakeOnce == ID_TRUE) &&
                                   (sHostNum == sOldHostNum),
                                   ERR_NOT_AVAILABLE);

                    IDU_FIT_POINT( "1.BUG-15084@rpxSender::attemptHandshake" );

                    // BUG-15084
                    sleepForNextConnect();
                    break;

                case RP_MSG_DENY :
                case RP_MSG_NOK :
                    disconnectPeer();
                    IDE_TEST_RAISE(isParallelChild() == ID_TRUE, ERR_NOT_AVAILABLE);
                    IDE_TEST(getNextLastUsedHostNo(&sHostNum) != IDE_SUCCESS);
                    IDE_TEST_RAISE(mTryHandshakeOnce == ID_TRUE, ERR_NOT_AVAILABLE);
                    sleepForNextConnect();
                    break;

                case RP_MSG_PROTOCOL_DIFF :
                case RP_MSG_META_DIFF :     // error return to caller
                    IDE_RAISE(ERR_INVALID_REPL);

                default :
                    // TODO : ��Ŷ�� ���� ���ϱ�? ���� ó���� �ؾ��Ѵ�.
                    IDE_CALLBACK_FATAL("[Repl Sender] Can't be here");
            } // switch
        } // if
    } // while

    // Exit Flag�� ID_TRUE�� �����Ǿ��� ���� �Ʒ��� �۾��� �ϸ� �� �ȴ�.
    return IDE_SUCCESS;

    RP_LABEL(VALIDATE_SUCCESS);

    mSenderInfo->initReconnectCount();
    mSenderInfo->setOriginReplMode( mMeta.mReplication.mReplMode );

    // BUG-15507
    mRetry = 0;

    IDE_TEST(initXSN(sReceiverXSN) != IDE_SUCCESS);

    IDE_TEST( addXLogKeepAlive() != IDE_SUCCESS );

    //----------------------------------------------------------------//
    //   set TCP information
    //----------------------------------------------------------------//
    if ( mSocketType == RP_SOCKET_TYPE_TCP )
    {
        mMessenger.updateTcpAddress();
    }
    else
    {
        /* nothing to do */
    }

    *aHandshakeFlag = ID_TRUE;

    return IDE_SUCCESS;

    IDE_EXCEPTION(ERR_NOT_AVAILABLE);
    {
        sTraceLogEnable = ID_FALSE;
    }
    IDE_EXCEPTION(ERR_INVALID_REPL);
    {
        sTraceLogEnable = ID_FALSE;
    }
    IDE_EXCEPTION_END;

    if ( sTraceLogEnable == ID_TRUE )
    {
        IDE_ERRLOG( IDE_RP_0 );
    }

    if(sConnSuccess == IDE_SUCCESS)
    {
        disconnectPeer();

        if(sRegistHost == ID_TRUE)
        {
            mMessenger.setHBTResource( NULL );
            rpcHBT::unregistHost(mRsc);
            mRsc = NULL;
        }
    }

    if(idlOS::strlen(mRCMsg) == 0)
    {
        idlOS::sprintf(mRCMsg, "Handshake Process Error");
    }
    IDE_SET(ideSetErrorCode(rpERR_ABORT_RP_SENDER_HANDSHAKE, mRCMsg));

    return IDE_FAILURE;
}

//===================================================================
//
// Name:          releaseHandshake
//
// Return Value:  IDE_SUCCESS/IDE_FAILURE
//
// Argument:
//
// Called By:
//
// Description:
//             Handshake�� �Ҵ� ���� ��� ���ҽ��� �����Ѵ�.
//===================================================================
void rpxSender::releaseHandshake()
{
    disconnectPeer();

    /*
     *  unSet HBT Resource to Messenger
     */
    mMessenger.setHBTResource( NULL );

    // PROJ-1537
    if((mMeta.mReplication.mRole != RP_ROLE_ANALYSIS) &&
       (mCurrentType != RP_OFFLINE)) //PROJ-1915
    {
        rpcHBT::unregistHost(mRsc);
    }

    mRsc = NULL;

    return;
}



//===================================================================
//
// Name:          connectPeer
//
// Return Value:  IDE_SUCCESS/IDE_FAILURE
//
// Argument:
//
// Called By:     rpxSender::attemptHandshake()
//
// Description:
//
//===================================================================

IDE_RC rpxSender::connectPeer(SInt aHostNum)
{
    SChar * sHostIP = NULL;

    sHostIP = mMeta.mReplication.mReplHosts[aHostNum].mHostIp;

    // PROJ-1537
    if(idlOS::strMatch(RP_SOCKET_UNIX_DOMAIN_STR, RP_SOCKET_UNIX_DOMAIN_LEN,
                       sHostIP, idlOS::strlen(sHostIP)) == 0)
    {
        mSocketType = RP_SOCKET_TYPE_UNIX;

        idlOS::memset(mSocketFile, 0x00, RP_SOCKET_FILE_LEN);
        idlOS::snprintf(mSocketFile, RP_SOCKET_FILE_LEN, "%s%c%s%c%s%s",
                        idlOS::getenv(IDP_HOME_ENV), IDL_FILE_SEPARATOR,
                        "trc", IDL_FILE_SEPARATOR,
                        "rp-", mRepName);

        IDE_TEST( mMessenger.connectThroughUnix( mSocketFile )
                  != IDE_SUCCESS );
    }
    else
    {
        mSocketType = RP_SOCKET_TYPE_TCP;

        IDE_TEST( mMessenger.connectThroughTcp( 
                      sHostIP, mMeta.mReplication.mReplHosts[aHostNum].mPortNo )
                  != IDE_SUCCESS );
    }

    mNetworkError = ID_FALSE;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    mNetworkError = ID_TRUE;

    return IDE_FAILURE;
}

void rpxSender::disconnectPeer()
{
    mMessenger.disconnect();

    mNetworkError = ID_TRUE;

    return;
}

//===================================================================
//
// Name:          checkReplAvailable
//
// Return Value:  IDE_SUCCESS/IDE_FAILURE
//
// Argument:
//
// Called By:     rpxSender::attemptHandshake()
//
// Description:
//
//===================================================================

IDE_RC rpxSender::checkReplAvailable(rpMsgReturn *aRC,
                                     SInt        *aFailbackStatus,
                                     smSN        *aXSN)
{
    /* Server�� Startup �ܰ����� ���δ� �������̴�. */
    if ( rpcManager::isStartupFailback() == ID_TRUE )
    {
        rpdMeta::setReplFlagFailbackServerStartup( &(mMeta.mReplication) );
    }
    else
    {
        rpdMeta::clearReplFlagFailbackServerStartup( &(mMeta.mReplication) );
    }

    IDE_TEST( mMessenger.handshakeAndGetResults( &mMeta, aRC,
                                                 mRCMsg,
                                                 sizeof( mRCMsg ),
                                                 aFailbackStatus,
                                                 aXSN )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/***********************************************************************
 * Description : Network�� �������� �ʰ� Handshake�� �����Ѵ�.
 *
 ***********************************************************************/
IDE_RC rpxSender::handshakeWithoutReconnect()
{
    rpMsgReturn sRC = RP_MSG_DISCONNECT;
    SInt        sFailbackStatus;
    ULong       sDummyXSN;

    if(mMeta.mReplication.mRole == RP_ROLE_ANALYSIS)
    {
        ideLog::log( IDE_RP_0, "[%s] XLog Sender: Send XLog for Meta change.\n", mMeta.mReplication.mRepName );
        // Ala Receiver���� Handshake�� �ٽ� �� ���̶�� �˸��� �����Ѵ�.
        IDE_TEST(addXLogHandshake() != IDE_SUCCESS);

        /* Send Replication Stop Message */
        ideLog::log( IDE_RP_0, "[%s] XLog Sender: SEND Stop Message for meta change\n", mMeta.mReplication.mRepName );
        
        IDE_TEST( mMessenger.sendStop( getRestartSN() ) != IDE_SUCCESS );

        ideLog::log( IDE_RP_0, "SEND Stop Message SUCCESS!!!\n" );

        finalizeSenderApply();
        // Altibase Log Analyzer�� ������ ���� Network ������ �����Ѵ�.
        mNetworkError = ID_TRUE;
        mSenderInfo->deActivate(); //isDisconnect()
    }
    else
    {
        IDU_FIT_POINT( "rpxSender::handshakeWithoutReconnect::ERR_HANDSHAKE" );

        IDE_WARNING(IDE_RP_0, RP_TRC_SH_NTC_HANDSHAKE_XLOG);

        // Receiver���� Handshake�� �ٽ� �� ���̶�� �˸���.
        IDE_TEST(addXLogHandshake() != IDE_SUCCESS);

        // Sender Apply Thread�� Handshake Ready XLog�� ������ ������ ��ٸ���.
        while(mSenderApply->isSuspended() != ID_TRUE)
        {
            IDE_TEST_CONT(checkInterrupt() != RP_INTR_NONE, NORMAL_EXIT);
            idlOS::thr_yield();
        }

        // Handshake�� �����Ѵ�.
        IDE_TEST(checkReplAvailable(&sRC,
                                    &sFailbackStatus, // Dummy
                                    &sDummyXSN)
                 != IDE_SUCCESS);   // always return IDE_SUCCESS
        if(sRC != RP_MSG_OK)
        {
            mNetworkError = ID_TRUE;
            mSenderInfo->deActivate(); //isDisconnect()
        }

        // Sender Apply Thread�� ���� �����ϵ��� �Ѵ�.
        mSenderApply->resume();
    }

    RP_LABEL(NORMAL_EXIT);

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}
