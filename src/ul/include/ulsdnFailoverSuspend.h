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

#ifndef ULSD_FAILOVER_SUSPEND_H_
#define ULSD_FAILOVER_SUSPEND_H_

#include <ulsdnFailoverSuspendDef.h>


ulsdnFailoverSuspendState ulsdnDbcGetFailoverSuspendState(ulnDbc *aDbc);

void ulsdnClearFailoverSuspendBackup( ulsdnFailoverSuspendBackup * aBackup );

void ulsdnDbcClearFailoverSuspendState( ulnDbc * aDbc );

void ulsdnDbcSetFailoverSuspendState( ulnDbc                     * aDbc,
                                      ulsdnFailoverSuspendState    aState,
                                      ulsdnFailoverSuspendBackup * aBackup );

void ulsdnDbcSetFailoverSuspendStateWithoutBackup( ulnDbc                     * aDbc,
                                                   ulsdnFailoverSuspendState    aState );

void ulsdnDbcUnsetFailoverSuspendState( ulnDbc                     * aDbc,
                                        ulsdnFailoverSuspendBackup * aBackup );

void ulsdnDbcSetFailoverSuspendErrorCode( ulnDbc       * aDbc,
                                          acp_uint32_t   aErrorCode );

void ulnDbcSetFailoverSuspendSkipError( ulnDbc * aDbc );

acp_bool_t ulsdnDbcIsSetFailoverSuspendErrorCode( ulnDbc * aDbc );

acp_bool_t ulsdnDbcIsSkipFailoverSuspendError( ulnDbc * aDbc );

acp_uint32_t ulsdnDbcGetFailoverSuspendErrorCode(ulnDbc *aDbc);

#endif /* ULSD_FAILOVER_SUSPEND_H_ */
