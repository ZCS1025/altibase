/*
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

package Altibase.jdbc.driver.cm;

import java.io.IOException;
import java.io.InputStream;
import java.net.*;
import java.nio.ByteBuffer;
import java.nio.channels.Channels;
import java.nio.channels.ReadableByteChannel;
import java.nio.channels.WritableByteChannel;
import java.security.*;
import java.security.cert.CertificateException;
import java.sql.SQLException;

import javax.net.ssl.*;

import Altibase.jdbc.driver.ex.Error;
import Altibase.jdbc.driver.ex.ErrorDef;
import Altibase.jdbc.driver.util.AltibaseProperties;
import Altibase.jdbc.driver.util.SocketUtils;
import Altibase.jdbc.driver.util.StringUtils;

class CmSecureSocket implements CmSocket
{
    private Socket mSocket;
    private int mSocketFD = INVALID_SOCKTFD; // PROJ-2625

    private int mSockSndBufSize = CmChannel.CM_DEFAULT_SNDBUF_SIZE;
    private int mSockRcvBufSize = CmChannel.CM_DEFAULT_RCVBUF_SIZE;
    private WritableByteChannel mWriteChannel;
    private ReadableByteChannel mReadChannel;

    // PROJ-2474 keystore파일을 접근하는 기본 프로토콜은 file:이다.
    private static final String DEFAULT_URL_FILE_SCHEME = "file:";
    private static final String DEFAULT_KEYSTORE_TYPE = "JKS";

    // PROJ-2474 SSL 관련된 속성들을 가지고 있는 객체
    private SSLProperties mSslProps;

    public CmSecureSocket(AltibaseProperties aProps)
    {
        mSslProps = new SSLProperties(aProps);
    }

    public CmConnType getConnType()
    {
        return CmConnType.SSL;
    }

    public void open(SocketAddress aSockAddr,
                     String        aBindAddr,
                     int           aLoginTimeout,
                     int           aResponseTimeout) throws SQLException
    {
        try
        {
            mSocket = getSslSocketFactory(mSslProps).createSocket();

            if (mSslProps.getCipherSuiteList() != null)
            {
                ((SSLSocket)mSocket).setEnabledCipherSuites(mSslProps.getCipherSuiteList());
            }

            if (aBindAddr != null)
            {
                mSocket.bind(new InetSocketAddress(aBindAddr, 0));
            }

            if (aResponseTimeout > 0)
            {
                mSocket.setSoTimeout(aResponseTimeout * 1000);
            }

            mSocket.setKeepAlive(true);
            mSocket.setReceiveBufferSize(mSockRcvBufSize); // PROJ-2625
            mSocket.setSendBufferSize(mSockSndBufSize);
            mSocket.setTcpNoDelay(true);  // BUG-45275 disable nagle algorithm

            mSocket.connect(aSockAddr, aLoginTimeout * 1000);

            ((SSLSocket)mSocket).setEnabledProtocols(new String[] { "TLSv1" });
            ((SSLSocket)mSocket).startHandshake();

            mWriteChannel = Channels.newChannel(mSocket.getOutputStream());
            mReadChannel = Channels.newChannel(mSocket.getInputStream());
        }
        catch (SQLException e)
        {
            throw e;
        }
        // connect 실패시 날 수 있는 예외가 한종류가 아니므로 모든 예외를 잡아 연결 실패로 처리한다.
        // 예를들어, AIX 6.1에서는 ClosedSelectorException가 나는데 이는 RuntimeException이다. (ref. BUG-33341)
        catch (Exception e)
        {
            Error.throwCommunicationErrorException(e);
        }
    }

    public void close() throws IOException
    {
        if (mWriteChannel != null)
        {
            mWriteChannel.close();
            mWriteChannel = null;
        }

        if (mReadChannel != null)
        {
            mReadChannel.close();
            mReadChannel = null;
        }

        if (mSocket != null)
        {
            mSocket.close();
            mSocket = null;
        }

        mSocketFD = INVALID_SOCKTFD;
    }

    /**
     * 인증서 정보를 이용해 SSLSocketFactory 객체를 생성한다.
     * @param aCertiProps ssl관련 설정 정보를 가지고 있는 객체
     * @return SSLSocketFactory 객체
     * @throws SQLException keystore에서 에러가 발생한 경우
     */
    private SSLSocketFactory getSslSocketFactory(SSLProperties aCertiProps) throws SQLException
    {
        String sKeyStoreUrl = aCertiProps.getKeyStoreUrl();
        String sKeyStorePassword = aCertiProps.getKeyStorePassword();
        String sKeyStoreType = aCertiProps.getKeyStoreType();
        String sTrustStoreUrl = aCertiProps.getTrustStoreUrl();
        String sTrustStorePassword = aCertiProps.getTrustStorePassword();
        String sTrustStoreType = aCertiProps.getTrustStoreType();

        TrustManagerFactory sTmf = null;
        KeyManagerFactory sKmf = null;

        try
        {
            sTmf = TrustManagerFactory.getInstance(TrustManagerFactory.getDefaultAlgorithm());
            sKmf = KeyManagerFactory.getInstance(KeyManagerFactory.getDefaultAlgorithm());
        }
        catch (NoSuchAlgorithmException e)
        {
            Error.throwSQLException(ErrorDef.DEFAULT_ALGORITHM_DEFINITION_INVALID, e);
        }

        loadKeyStore(sKeyStoreUrl, sKeyStorePassword, sKeyStoreType, sKmf);

        // BUG-40165 verify_server_certificate가 true일때만 TrustManagerFactory를 초기화 해준다.
        if (aCertiProps.verifyServerCertificate())
        {
            loadKeyStore(sTrustStoreUrl, sTrustStorePassword, sTrustStoreType, sTmf);
        }
        
        return createAndInitSslContext(aCertiProps, sKeyStoreUrl, sTrustStoreUrl, sTmf, sKmf).getSocketFactory();
    }

    private SSLContext createAndInitSslContext(SSLProperties       aCertiInfo,
                                               String              aKeyStoreUrl,
                                               String              aTrustStoreUrl,
                                               TrustManagerFactory aTmf,
                                               KeyManagerFactory   aKmf) throws SQLException
    {
        SSLContext sSslContext = null;

        try 
        {
            sSslContext = SSLContext.getInstance("TLS");
            
            KeyManager[] sKeyManagers = (StringUtils.isEmpty(aKeyStoreUrl)) ?  null : aKmf.getKeyManagers();
            TrustManager[] sTrustManagers;
            if (aCertiInfo.verifyServerCertificate())
            {
                sTrustManagers = (StringUtils.isEmpty(aTrustStoreUrl)) ? null : aTmf.getTrustManagers();
            }
            else
            {
                sTrustManagers = new X509TrustManager[] { BlindTrustManager.getInstance() };
            }
            sSslContext.init(sKeyManagers, sTrustManagers, null);
            
        }
        catch (NoSuchAlgorithmException e) 
        {
            Error.throwSQLException(ErrorDef.UNSUPPORTED_KEYSTORE_ALGORITHM, e.getMessage(), e);
        } 
        catch (KeyManagementException e) 
        {
            Error.throwSQLException(ErrorDef.KEY_MANAGEMENT_EXCEPTION_OCCURRED, e.getMessage(), e);
        }
        return sSslContext;
    }

    /**
     * PROJ-2474 KeyStore 및 TrustStore 파일을 읽어들인다.
     */
    private void loadKeyStore(String aKeyStoreUrl,
                              String aKeyStorePassword,
                              String aKeyStoreType, 
                              Object aKeystoreFactory) throws SQLException
    {
        if (StringUtils.isEmpty(aKeyStoreUrl)) return;
        InputStream keyStoreStream = null;
        
        try 
        {
            aKeyStoreType = (StringUtils.isEmpty(aKeyStoreType)) ? DEFAULT_KEYSTORE_TYPE : aKeyStoreType;
            KeyStore sClientKeyStore = KeyStore.getInstance(aKeyStoreType);
            URL sKsURL = new URL(DEFAULT_URL_FILE_SCHEME + aKeyStoreUrl);
            char[] sPassword = (aKeyStorePassword == null) ? new char[0] : aKeyStorePassword.toCharArray();
            keyStoreStream = sKsURL.openStream();
            sClientKeyStore.load(keyStoreStream, sPassword);
            keyStoreStream.close();
            if (aKeystoreFactory instanceof KeyManagerFactory)
            {
                ((KeyManagerFactory)aKeystoreFactory).init(sClientKeyStore, sPassword);
            }
            else if (aKeystoreFactory instanceof TrustManagerFactory)
            {
                ((TrustManagerFactory)aKeystoreFactory).init(sClientKeyStore);
            }
        }
        catch (UnrecoverableKeyException e) 
        {
            Error.throwSQLException(ErrorDef.CAN_NOT_RETREIVE_KEY_FROM_KEYSTORE, e);
        } 
        catch (NoSuchAlgorithmException e) 
        {
            Error.throwSQLException(ErrorDef.UNSUPPORTED_KEYSTORE_ALGORITHM, e.getMessage(), e);   
        } 
        catch (KeyStoreException e) 
        {
            Error.throwSQLException(ErrorDef.CAN_NOT_CREATE_KEYSTORE_INSTANCE, e.getMessage(), e);
        } 
        catch (CertificateException e) 
        {
            Error.throwSQLException(ErrorDef.CAN_NOT_LOAD_KEYSTORE, aKeyStoreType, e);
        } 
        catch (MalformedURLException e) 
        {
            Error.throwSQLException(ErrorDef.INVALID_KEYSTORE_URL, e);
        } 
        catch (IOException e) 
        {
            Error.throwSQLException(ErrorDef.CAN_NOT_OPEN_KEYSTORE, e);
        }
        finally
        {
            try
            {
                if (keyStoreStream != null)
                {
                    keyStoreStream.close();
                }
            }
            catch (IOException e)
            {
                Error.throwSQLExceptionForIOException(e.getCause());
            }
        }
    }

    public int read(ByteBuffer aBuffer) throws IOException
    {
        return mReadChannel.read(aBuffer);
    }

    public int write(ByteBuffer aBuffer) throws IOException
    {
        return mWriteChannel.write(aBuffer);
    }

    public int getSockSndBufSize()
    {
        return mSockSndBufSize;
    }

    public int getSockRcvBufSize()
    {
        return mSockRcvBufSize;
    }

    /**
     * Socket send buffer 크기를 설정한다.
     */
    public void setSockSndBufSize(int aSockSndBufSize) throws IOException
    {
        if (mSocket != null)
        {
            mSocket.setSendBufferSize(aSockSndBufSize);
        }

        mSockSndBufSize = aSockSndBufSize;
    }

    /**
     * Socket receive buffer 크기를 설정한다.
     */
    public void setSockRcvBufSize(int aSockRcvBufSize) throws IOException
    {
        if (mSocket != null)
        {
            mSocket.setReceiveBufferSize(aSockRcvBufSize);
        }

        mSockRcvBufSize = aSockRcvBufSize;
    }

    public int getSocketFD() throws SQLException
    {
        try
        {
            if (mSocketFD == INVALID_SOCKTFD)
            {
                mSocketFD = SocketUtils.getFileDescriptor(mSocket);
            }
        }
        catch (Exception e)
        {
            Error.throwSQLException(ErrorDef.COMMUNICATION_ERROR, "Failed to get a file descriptor of the socket.", e);
        }

        return mSocketFD;
    }

    public String[] getEnabledCipherSuites()
    {
        return ((SSLSocket)mSocket).getEnabledCipherSuites();
    }
}
