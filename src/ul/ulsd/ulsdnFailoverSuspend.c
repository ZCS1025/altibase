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
#include <ulnPrivate.h>

#include <ulsdnFailoverSuspend.h>

void ulsdnClearFailoverSuspendBackup( ulsdnFailoverSuspendBackup * aBackup )
{
    aBackup->mSuspendType           = ULSDN_FAILOVER_SUSPEND_DEFAULT_TYPE;
    aBackup->mSkipError             = ULSDN_FAILOVER_SUSPEND_DEFAULT_SKIP_ERROR;
    aBackup->mNewErrorCode          = ULSDN_FAILOVER_SUSPEND_DEFAULT_ERROR_CODE;
    aBackup->mSet                   = ACP_FALSE;
}

void ulsdnDbcClearFailoverSuspendState( ulnDbc * aDbc )
{
    aDbc->mShardDbcCxt.mFailoverSuspendCmd.mFailoverSuspendState     = ULSDN_FAILOVER_SUSPEND_DEFAULT_TYPE;
    aDbc->mShardDbcCxt.mFailoverSuspendCmd.mFailoverSuspendSkipError = ULSDN_FAILOVER_SUSPEND_DEFAULT_SKIP_ERROR;
    aDbc->mShardDbcCxt.mFailoverSuspendCmd.mFailoverSuspendErrorCode = ULSDN_FAILOVER_SUSPEND_DEFAULT_ERROR_CODE;
}

void ulsdnDbcBackupFailoverSuspend( ulsdnFailoverSuspendBackup * aBackup, ulnDbc * aDbc )
{
    aBackup->mSuspendType           = aDbc->mShardDbcCxt.mFailoverSuspendCmd.mFailoverSuspendState;
    aBackup->mSkipError             = aDbc->mShardDbcCxt.mFailoverSuspendCmd.mFailoverSuspendSkipError;
    aBackup->mNewErrorCode          = aDbc->mShardDbcCxt.mFailoverSuspendCmd.mFailoverSuspendErrorCode;
}

void ulsdnDbcRestoreFailoverSuspend( ulnDbc * aDbc, ulsdnFailoverSuspendBackup * aBackup )
{
    aDbc->mShardDbcCxt.mFailoverSuspendCmd.mFailoverSuspendState     = aBackup->mSuspendType;
    aDbc->mShardDbcCxt.mFailoverSuspendCmd.mFailoverSuspendSkipError = aBackup->mSkipError;
    aDbc->mShardDbcCxt.mFailoverSuspendCmd.mFailoverSuspendErrorCode = aBackup->mNewErrorCode;
}

ulsdnFailoverSuspendState ulsdnDbcGetFailoverSuspendState(ulnDbc *aDbc)
{
    ulnDbc                    * sMetaDbc = ulnDbcGetShardParentDbc( aDbc );
    ulsdnFailoverSuspendState   sRet     = ULSDN_FAILOVER_SUSPEND_DEFAULT_TYPE;

    if ( sMetaDbc != NULL )
    {
        sRet = ULSDN_GET_FAILOVER_SUSPEND_TYPE( sMetaDbc );
    }

    return sRet;
}

static void ulsdnDbcSetFailoverSuspendStateCore( ulnDbc                     * aDbc,
                                                 ulsdnFailoverSuspendState    aState,
                                                 ulsdnFailoverSuspendBackup * aBackup )
{
    ulnDbc * sMetaDbc = ulnDbcGetShardParentDbc( aDbc );

    ACE_DASSERT( aState <  ULSDN_FAILOVER_SUSPEND_MAX );

    if ( sMetaDbc != NULL )
    {
        ACE_DASSERT( aDbc->mShardDbcCxt.mParentDbc == NULL );

        if ( aState == ULSDN_FAILOVER_SUSPEND_DEFAULT_TYPE )
        {
            /* Suspend Off */
            if ( ( aBackup != NULL ) && ( aBackup->mSet == ACP_TRUE ) )
            {
                ulsdnDbcClearFailoverSuspendState( sMetaDbc );

                if ( ULSDN_IS_FAILOVER_SUSPEND_ON( aBackup->mSuspendType ) == ACP_TRUE )
                {
                    /* Restore backuped setting */
                    ulsdnDbcRestoreFailoverSuspend( sMetaDbc, aBackup );
                }

                aBackup->mSet = ACP_FALSE;
            }
            else if ( aBackup == NULL )
            {
                ulsdnDbcClearFailoverSuspendState( sMetaDbc );
            }
            else
            {
                /* ( aBackup != NULL ) && ( aBackup->mSet == ACP_FALSE )
                 * Nothing to do */
            }
        }
        else
        {
            /* Suspend On */
            if ( ULSDN_IS_FAILOVER_SUSPEND_ON( ULSDN_GET_FAILOVER_SUSPEND_TYPE( sMetaDbc ) ) == ACP_TRUE )
            {
                /* Backup and clear current setting */
                if ( aBackup != NULL )
                {
                    ulsdnDbcBackupFailoverSuspend( aBackup, sMetaDbc );
                }
                ulsdnDbcClearFailoverSuspendState( sMetaDbc );
            }

            if ( aBackup != NULL )
            {
                aBackup->mSet = ACP_TRUE;
            }

            ULSDN_SET_FAILOVER_SUSPEND_TYPE( sMetaDbc, aState );
        }
    }
}

void ulsdnDbcSetFailoverSuspendState( ulnDbc                     * aDbc,
                                      ulsdnFailoverSuspendState    aState,
                                      ulsdnFailoverSuspendBackup * aBackup )
{
    ACE_DASSERT( aBackup != NULL );

    ulsdnDbcSetFailoverSuspendStateCore( aDbc, aState, aBackup );
}

void ulsdnDbcSetFailoverSuspendStateWithoutBackup( ulnDbc                     * aDbc,
                                                   ulsdnFailoverSuspendState    aState )
{
    ulsdnDbcSetFailoverSuspendStateCore( aDbc, aState, NULL );
}

void ulsdnDbcUnsetFailoverSuspendState( ulnDbc                     * aDbc,
                                        ulsdnFailoverSuspendBackup * aBackup )
{
    ulsdnDbcSetFailoverSuspendState( aDbc,
                                     ULSDN_FAILOVER_SUSPEND_DEFAULT_TYPE,
                                     aBackup );
}

acp_uint32_t ulsdnDbcGetFailoverSuspendErrorCode(ulnDbc *aDbc)
{
    ulnDbc       * sMetaDbc = ulnDbcGetShardParentDbc( aDbc );
    acp_uint32_t   sRet     = ULSDN_FAILOVER_SUSPEND_DEFAULT_ERROR_CODE;

    if ( sMetaDbc != NULL )
    {
        if ( ULSDN_IS_FAILOVER_SUSPEND_ON( ULSDN_GET_FAILOVER_SUSPEND_TYPE( sMetaDbc ) )
             == ACP_TRUE )
        {
            sRet = ULSDN_GET_FAILOVER_SUSPEND_NEW_ERROR_CODE( sMetaDbc );
        }
    }

    return sRet;
}

void ulsdnDbcSetFailoverSuspendErrorCode( ulnDbc       * aDbc,
                                          acp_uint32_t   aErrorCode )
{
    ulnDbc * sMetaDbc = ulnDbcGetShardParentDbc( aDbc );

    if ( sMetaDbc != NULL )
    {
        ACE_DASSERT( aDbc->mShardDbcCxt.mParentDbc == NULL );
        ACE_DASSERT( ULSDN_IS_FAILOVER_SUSPEND_ON( ULSDN_GET_FAILOVER_SUSPEND_TYPE( sMetaDbc ) )
                     == ACP_TRUE );

        if ( ULSDN_IS_FAILOVER_SUSPEND_ON( ULSDN_GET_FAILOVER_SUSPEND_TYPE( sMetaDbc ) )
             == ACP_FALSE )
        {
            /* Nothing to do. */
        }
        else
        {
            ACE_DASSERT( ULSDN_GET_FAILOVER_SUSPEND_NEW_ERROR_CODE( sMetaDbc )
                         == ULSDN_FAILOVER_SUSPEND_DEFAULT_ERROR_CODE );
            ACE_DASSERT( ULSDN_GET_FAILOVER_SUSPEND_SKIP_ERROR( sMetaDbc )
                         == ULSDN_FAILOVER_SUSPEND_DEFAULT_SKIP_ERROR );

            ULSDN_SET_FAILOVER_SUSPEND_SKIP_ERROR    ( sMetaDbc, ULSDN_FAILOVER_SUSPEND_DEFAULT_SKIP_ERROR );
            ULSDN_SET_FAILOVER_SUSPEND_NEW_ERROR_CODE( sMetaDbc, aErrorCode );
        }
    }
}

void ulnDbcSetFailoverSuspendSkipError( ulnDbc * aDbc )
{
    ulnDbc * sMetaDbc = ulnDbcGetShardParentDbc( aDbc );

    if ( sMetaDbc != NULL )
    {
        ACE_DASSERT( aDbc->mShardDbcCxt.mParentDbc == NULL );
        ACE_DASSERT( ULSDN_IS_FAILOVER_SUSPEND_ON( ULSDN_GET_FAILOVER_SUSPEND_TYPE( sMetaDbc ) )
                     == ACP_TRUE );

        if ( ULSDN_IS_FAILOVER_SUSPEND_ON( ULSDN_GET_FAILOVER_SUSPEND_TYPE( sMetaDbc ) )
             == ACP_FALSE )
        {
            /* Nothing to do. */
        }
        else
        {
            ACE_DASSERT( ULSDN_GET_FAILOVER_SUSPEND_NEW_ERROR_CODE( sMetaDbc )
                         == ULSDN_FAILOVER_SUSPEND_DEFAULT_ERROR_CODE );
            ACE_DASSERT( ULSDN_GET_FAILOVER_SUSPEND_SKIP_ERROR( sMetaDbc )
                         == ULSDN_FAILOVER_SUSPEND_DEFAULT_SKIP_ERROR );

            ULSDN_SET_FAILOVER_SUSPEND_SKIP_ERROR    ( sMetaDbc, ACP_TRUE );
            ULSDN_SET_FAILOVER_SUSPEND_NEW_ERROR_CODE( sMetaDbc, ULSDN_FAILOVER_SUSPEND_DEFAULT_ERROR_CODE );
        }
    }
}

acp_bool_t ulsdnDbcIsSetFailoverSuspendErrorCode( ulnDbc * aDbc )
{
    ulnDbc     * sMetaDbc = ulnDbcGetShardParentDbc( aDbc );
    acp_bool_t   sRet     = ACP_FALSE;

    if ( sMetaDbc != NULL )
    {
        if ( ( ULSDN_IS_FAILOVER_SUSPEND_ON( ULSDN_GET_FAILOVER_SUSPEND_TYPE( sMetaDbc ) )
               == ACP_TRUE ) &&
             ( ULSDN_GET_FAILOVER_SUSPEND_NEW_ERROR_CODE( sMetaDbc )
               != ULSDN_FAILOVER_SUSPEND_DEFAULT_ERROR_CODE ) )
        {
            sRet = ACP_TRUE;
        }
    }

    return sRet;
}

acp_bool_t ulsdnDbcIsSkipFailoverSuspendError( ulnDbc * aDbc )
{
    ulnDbc     * sMetaDbc = ulnDbcGetShardParentDbc( aDbc );
    acp_bool_t   sRet     = ACP_FALSE;

    if ( sMetaDbc != NULL )
    {
        if ( ( ULSDN_IS_FAILOVER_SUSPEND_ON( ULSDN_GET_FAILOVER_SUSPEND_TYPE( sMetaDbc ) )
               == ACP_TRUE ) &&
             ( ULSDN_GET_FAILOVER_SUSPEND_SKIP_ERROR( sMetaDbc )
               != ULSDN_FAILOVER_SUSPEND_DEFAULT_SKIP_ERROR ) )
        {
            sRet = ACP_TRUE;
        }
    }

    return sRet;
}


