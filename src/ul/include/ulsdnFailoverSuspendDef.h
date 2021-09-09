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

#ifndef ULSD_FAILOVER_SUSPEND_DEF_H_
#define ULSD_FAILOVER_SUSPEND_DEF_H_

#define ULSDN_FAILOVER_SUSPEND_DEFAULT_TYPE                        (ULSDN_FAILOVER_SUSPEND_OFF)
#define ULSDN_FAILOVER_SUSPEND_DEFAULT_SKIP_ERROR                  (ACP_FALSE)
#define ULSDN_FAILOVER_SUSPEND_DEFAULT_ERROR_CODE                  (ulERR_IGNORE_NO_ERROR)
#define ULSDN_GET_FAILOVER_SUSPEND_TYPE(__aDBC)                    ((__aDBC)->mShardDbcCxt.mFailoverSuspendCmd.mFailoverSuspendState)
#define ULSDN_SET_FAILOVER_SUSPEND_TYPE(__aDBC, __aTYPE)           ((__aDBC)->mShardDbcCxt.mFailoverSuspendCmd.mFailoverSuspendState = __aTYPE)
#define ULSDN_GET_FAILOVER_SUSPEND_NEW_ERROR_CODE(__aDBC)          ((__aDBC)->mShardDbcCxt.mFailoverSuspendCmd.mFailoverSuspendErrorCode)
#define ULSDN_SET_FAILOVER_SUSPEND_NEW_ERROR_CODE(__aDBC, __aCode) ((__aDBC)->mShardDbcCxt.mFailoverSuspendCmd.mFailoverSuspendErrorCode = __aCode)
#define ULSDN_GET_FAILOVER_SUSPEND_SKIP_ERROR(__aDBC)              ((__aDBC)->mShardDbcCxt.mFailoverSuspendCmd.mFailoverSuspendSkipError)
#define ULSDN_SET_FAILOVER_SUSPEND_SKIP_ERROR(__aDBC, __aSkip)     ((__aDBC)->mShardDbcCxt.mFailoverSuspendCmd.mFailoverSuspendSkipError = __aSkip)
#define ULSDN_IS_FAILOVER_SUSPEND_ON(__aSuspendType)               \
        ( \
          ( __aSuspendType != ULSDN_FAILOVER_SUSPEND_DEFAULT_TYPE ) \
          ? ACP_TRUE : ACP_FALSE \
        )

typedef enum
{
    ULSDN_FAILOVER_SUSPEND_OFF         = 0,
    ULSDN_FAILOVER_SUSPEND_NONE        = 1,
    ULSDN_FAILOVER_SUSPEND_ALLOW_RETRY = 2,
    ULSDN_FAILOVER_SUSPEND_ALL         = 3,
    ULSDN_FAILOVER_SUSPEND_MAX         = 4,
} ulsdnFailoverSuspendState;                  /* BUG-47131 샤드 All meta 환경에서 Client failover 시 hang 발생
                                               = sdiFailoverSuspendType */

typedef struct ulsdnFailoverSuspendCmd
{
    ulsdnFailoverSuspendState   mFailoverSuspendState;      /* BUG-47131 샤드 All meta 환경에서 Client failover 시 hang 발생 */
    acp_bool_t                  mFailoverSuspendSkipError;
    acp_uint32_t                mFailoverSuspendErrorCode;
} ulsdnFailoverSuspendCmd;

typedef struct ulsdnFailoverSuspendBackup
{
    ulsdnFailoverSuspendState mSuspendType;
    acp_uint32_t              mNewErrorCode;
    acp_bool_t                mSkipError;
    acp_bool_t                mSet;
} ulsdnFailoverSuspendBackup;

#endif /* ULSD_FAILOVER_SUSPEND_DEF_H_ */

