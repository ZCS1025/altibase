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
#include <mtcc.h>

#include <ulsd.h>

SQLRETURN ulsdShardCreate(ulnDbc *aDbc)
{
    ulnFnContext    sFnContext;
    ulsdDbc        *sShard;

    SHARD_LOG("(Create Shard) Create Shard Object\n");

    ACI_TEST_RAISE(ACP_RC_NOT_SUCCESS(acpMemAlloc((void**)&sShard,
                                                  ACI_SIZEOF(ulsdDbc))),
                   LABEL_SHARD_OBJECT_MEM_ALLOC_FAIL);

    sShard->mMetaDbc   = aDbc;
    sShard->mTouched   = ACP_FALSE;
    sShard->mNodeCount = 0;
    sShard->mNodeInfo  = NULL;

    aDbc->mShardDbcCxt.mShardDbc  = sShard;
    aDbc->mShardDbcCxt.mParentDbc = NULL;

    return SQL_SUCCESS;

    ACI_EXCEPTION(LABEL_SHARD_OBJECT_MEM_ALLOC_FAIL)
    {
        ULN_INIT_FUNCTION_CONTEXT(sFnContext, ULN_FID_ALLOCHANDLE, aDbc, ULN_OBJ_TYPE_DBC);

        ulnError(&sFnContext,
                 ulERR_ABORT_SHARD_ERROR,
                 "Create Shard",
                 "Failure to allocate shard object memory");
    }
    ACI_EXCEPTION_END;

    return SQL_ERROR;
}

void ulsdShardDestroy(ulnDbc *aDbc)
{
    acp_list_node_t     * sNode      = NULL;
    acp_list_node_t     * sNext      = NULL;
    ulsdConnectAttrInfo * sObj       = NULL;
    ulsdDbc             * sShard;
    acp_uint32_t          i;

    SHARD_LOG("(Destroy Shard) Destroy Shard Object\n");

    sShard = aDbc->mShardDbcCxt.mShardDbc;

    ACI_TEST_RAISE( sShard == NULL, END_OF_DESTROY_SHARD);

    if ( sShard->mNodeInfo != NULL )
    {
        for ( i = 0; i < sShard->mNodeCount; ++i )
        {
            acpMemFree( (void*)sShard->mNodeInfo[i] );
        }
        acpMemFree( (void*)sShard->mNodeInfo );
    }

    acpMemFree( (void*)sShard );

    ACI_EXCEPTION_CONT( END_OF_DESTROY_SHARD );

    aDbc->mShardDbcCxt.mShardDbc  = NULL;
    aDbc->mShardDbcCxt.mParentDbc = NULL;
    aDbc->mShardDbcCxt.mNodeInfo  = NULL;

    /* BUG-46257 shardcli에서 Node 추가/제거 지원 */
    if ( aDbc->mShardDbcCxt.mNodeBaseConnString != NULL )
    {
        acpMemFree( (void *)aDbc->mShardDbcCxt.mNodeBaseConnString );
        aDbc->mShardDbcCxt.mNodeBaseConnString = NULL;
    }
    else
    {
        /* Nothing to do */
    }

    /* BUG-46257 shardcli에서 Node 추가/제거 지원 */
    ACP_LIST_ITERATE_SAFE( & aDbc->mShardDbcCxt.mConnectAttrList, sNode, sNext )
    {
        sObj = (ulsdConnectAttrInfo *)sNode->mObj;

        acpListDeleteNode( sNode );

        if ( sObj->mBufferForString != NULL )
        {
            acpMemFree( sObj->mBufferForString );
        }
        else
        {
            /* Nothing to do */
        }

        acpMemFree( sNode->mObj );
    }
}

ACI_RC ulsdReallocAlignInfo( ulnFnContext  * aFnContext,
                             ulsdAlignInfo * aAlignInfo,
                             acp_sint32_t    aLen )
{
    acp_sint32_t sAllocLen = ACP_MIN( aLen, ULSD_ALIGN_INFO_MAX_TEXT_LENGTH );

    if ( sAllocLen > aAlignInfo->mMessageTextAllocLength )
    {
        ACI_TEST_RAISE( acpMemRealloc( (void **)&aAlignInfo->mMessageText,
                                       sAllocLen )
                        != ACP_RC_SUCCESS,
                        LABEL_MEMORY_ALLOC_EXCEPTION );

        aAlignInfo->mMessageTextAllocLength = sAllocLen;
    }

    return ACI_SUCCESS;

    ACI_EXCEPTION( LABEL_MEMORY_ALLOC_EXCEPTION );
    {
        ulnError( aFnContext,
                  ulERR_FATAL_MEMORY_ALLOC_ERROR,
                  "ulsdReallocAlignInfo" );
    }
    ACI_EXCEPTION_END;

    return ACI_FAILURE;
}

void ulsdAlignInfoInitialize( ulnDbc *aDbc )
{
    aDbc->mShardDbcCxt.mAlignInfo.mIsNeedAlign            = ACP_FALSE;
    aDbc->mShardDbcCxt.mAlignInfo.mSQLSTATE[0]            = '\0';
    aDbc->mShardDbcCxt.mAlignInfo.mNativeErrorCode        = 0;
    aDbc->mShardDbcCxt.mAlignInfo.mMessageText            = NULL;
    aDbc->mShardDbcCxt.mAlignInfo.mMessageTextAllocLength = 0;
}

void ulsdAlignInfoFinalize( ulnDbc *aDbc )
{
    ulsdAlignInfo *sAlignInfo = &aDbc->mShardDbcCxt.mAlignInfo;

    if ( sAlignInfo->mMessageText != NULL )
    {
        acpMemFree( sAlignInfo->mMessageText );
        sAlignInfo->mMessageText = NULL;
    }

    sAlignInfo->mMessageTextAllocLength = 0;
}

void ulsdGetShardFromDbc(ulnDbc       *aDbc,
                         ulsdDbc     **aShard)
{
    (*aShard) = aDbc->mShardDbcCxt.mShardDbc;
}
