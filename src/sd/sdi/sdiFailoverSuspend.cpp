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
 * $Id$ sdiFailoverSuspend.cpp
 **********************************************************************/

#include <idl.h>
#include <sdi.h>
#include <sdl.h>
#include <sdiFailoverSuspend.h>

sdiFailoverSuspend::sdiFailoverSuspend()
{
    initialize();
}

void sdiFailoverSuspend::initialize()
{
    clear( &mContext.mBackup );
}

idBool sdiFailoverSuspend::isFailoverSuspendSet( sdiClientInfo * aClientInfo )
{
    idBool sRet = ID_FALSE;

    if ( aClientInfo != NULL )
    {
        if ( aClientInfo->mFailoverSuspendCmd.mSuspendType != SDI_FAILOVER_SUSPEND_DEFAULT_TYPE )
        {
            sRet = ID_TRUE;
        }
    }

    return sRet;
}

idBool sdiFailoverSuspend::isFailoverSuspendHasNewErrorCode( sdiClientInfo * aClientInfo )
{
    idBool sRet = ID_FALSE;

    if ( aClientInfo != NULL )
    {
        if ( ( aClientInfo->mFailoverSuspendCmd.mSuspendType != SDI_FAILOVER_SUSPEND_DEFAULT_TYPE ) &&
             ( aClientInfo->mFailoverSuspendCmd.mNewErrorCode != SDI_FAILOVER_SUSPEND_DEFAULT_NEW_ERROR_CODE ) )
        {
            sRet = ID_TRUE;
        }
    }

    return sRet;
}

sdiFailoverSuspendType sdiFailoverSuspend::getFailoverSuspendType( sdiClientInfo * aClientInfo )
{
    sdiFailoverSuspendType sRet = SDI_FAILOVER_SUSPEND_DEFAULT_TYPE;

    if ( aClientInfo != NULL )
    {
        sRet = aClientInfo->mFailoverSuspendCmd.mSuspendType;
    }

    return sRet;
}

UInt sdiFailoverSuspend::getFailoverSuspendNewErrorCode( sdiClientInfo * aClientInfo )
{
    UInt sRet = SDI_FAILOVER_SUSPEND_DEFAULT_NEW_ERROR_CODE;

    if ( aClientInfo != NULL )
    {
        sRet = aClientInfo->mFailoverSuspendCmd.mNewErrorCode;
    }

    return sRet;
}

void sdiFailoverSuspend::clear( sdiFailoverSuspendCmd * aCmd )
{
    aCmd->mSuspendType     = SDI_FAILOVER_SUSPEND_DEFAULT_TYPE;
    aCmd->mNewErrorCode    = SDI_FAILOVER_SUSPEND_DEFAULT_NEW_ERROR_CODE;
}

void sdiFailoverSuspend::copy( sdiFailoverSuspendCmd * aDst, sdiFailoverSuspendCmd * aSrc )
{
    aDst->mSuspendType     = aSrc->mSuspendType;
    aDst->mNewErrorCode    = aSrc->mNewErrorCode;
}

void sdiFailoverSuspend::set( sdiClientInfo          * aClientInfo,
                              sdiFailoverSuspendType   aSuspendType,
                              UInt                     aNewErrorCode )
{
    sdiFailoverSuspendCmd sCmd;

    sCmd.mSuspendType  = aSuspendType;
    sCmd.mNewErrorCode = aNewErrorCode;

    set( aClientInfo, &sCmd );
}

void sdiFailoverSuspend::set( sdiClientInfo         * aClientInfo,
                              sdiFailoverSuspendCmd * aCmd )
{
    sdiConnectInfo * sConnectInfo   = NULL;
    SInt             i;

    if ( aClientInfo != NULL )
    {
        if ( aClientInfo->mFailoverSuspendCmd.mSuspendType != SDI_FAILOVER_SUSPEND_DEFAULT_TYPE )
        {
            /* Backup */
            copy( &mContext.mBackup, &aClientInfo->mFailoverSuspendCmd );
        }

        if ( aCmd->mSuspendType != SDI_FAILOVER_SUSPEND_DEFAULT_TYPE )
        {
            copy( &aClientInfo->mFailoverSuspendCmd, aCmd );

            sConnectInfo = aClientInfo->mConnectInfo;
            for ( i = 0 ; i < aClientInfo->mCount; i++, sConnectInfo++ )
            {
                sdl::setFailoverSuspendToDbc( aCmd->mSuspendType,
                                              sConnectInfo->mDbc );
            }
        }
    }
}

void sdiFailoverSuspend::unset( sdiClientInfo * aClientInfo )
{
    sdiConnectInfo * sConnectInfo   = NULL;
    SInt             i;
    idBool           sSkip          = ID_FALSE;

    if ( aClientInfo != NULL )
    {
        if ( mContext.mBackup.mSuspendType != SDI_FAILOVER_SUSPEND_DEFAULT_TYPE )
        {
            /* restore */
            copy( &aClientInfo->mFailoverSuspendCmd, &mContext.mBackup );
        }
        else
        {
            if ( aClientInfo->mFailoverSuspendCmd.mSuspendType == SDI_FAILOVER_SUSPEND_DEFAULT_TYPE )
            {
                sSkip = ID_TRUE;
            }
            else
            {
                clear( &aClientInfo->mFailoverSuspendCmd );
            }
        }

        if ( sSkip == ID_FALSE )
        {
            sConnectInfo = aClientInfo->mConnectInfo;
            for ( i = 0 ; i < aClientInfo->mCount; i++, sConnectInfo++ )
            {
                sdl::setFailoverSuspendToDbc( aClientInfo->mFailoverSuspendCmd.mSuspendType,
                                              sConnectInfo->mDbc );
            }
        }

        initialize();
    }
    else
    {
        initialize();
    }
}

