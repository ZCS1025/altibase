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
 * $Id$ sdiFailoverSuspend.h
 **********************************************************************/

#ifndef _O_SDI_FAILOVER_SUSPEND_H_
#define _O_SDI_FAILOVER_SUSPEND_H_ 1

#define SDI_FAILOVER_SUSPEND_DEFAULT_TYPE           (SDI_FAILOVER_SUSPEND_OFF)
#define SDI_FAILOVER_SUSPEND_DEFAULT_NEW_ERROR_CODE (idERR_IGNORE_NoError)

struct sdiClientInfo;
struct sdiConnectInfo;

typedef enum
{
    SDI_FAILOVER_SUSPEND_OFF         = 0,
    SDI_FAILOVER_SUSPEND_NONE        = 1,
    SDI_FAILOVER_SUSPEND_ALLOW_RETRY = 2,
    SDI_FAILOVER_SUSPEND_ALL         = 3,
    SDI_FAILOVER_SUSPEND_MAX         = 4,
} sdiFailoverSuspendType;                   /* = ulsdnFailoverSuspendState */

typedef struct sdiFailoverSuspendCmd
{
    sdiFailoverSuspendType   mSuspendType;
    UInt                     mNewErrorCode;
} sdiFailoverSuspendCmd;

typedef struct sdiFailoverSuspendContext
{
    sdiFailoverSuspendCmd    mBackup;
} sdiFailoverSuspendContext;

class sdiFailoverSuspend
{
  public:
    static void                   clear( sdiFailoverSuspendCmd * aCmd );
    static idBool                 isFailoverSuspendSet( sdiClientInfo * aClientInfo );
    static idBool                 isFailoverSuspendHasNewErrorCode( sdiClientInfo * aClientInfo );
    static sdiFailoverSuspendType getFailoverSuspendType( sdiClientInfo * aClientInfo );
    static UInt                   getFailoverSuspendNewErrorCode( sdiClientInfo * aClientInfo );

  public:
                sdiFailoverSuspend();
    void        initialize();
    void        set( sdiClientInfo          * aClientInfo,
                     sdiFailoverSuspendType   aSuspendType,
                     UInt                     aNewErrorCode = SDI_FAILOVER_SUSPEND_DEFAULT_NEW_ERROR_CODE );
    void        set( sdiClientInfo         * aClientInfo,
                     sdiFailoverSuspendCmd * aCmd );
    void        unset( sdiClientInfo * aClientInfo );

  private:
    void   copy( sdiFailoverSuspendCmd * aDst, sdiFailoverSuspendCmd * aSrc );

  private:
    sdiFailoverSuspendContext   mContext;
};

#endif  // _O_SDI_FAILOVER_SUSPEND_H_
