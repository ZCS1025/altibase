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
#include <ulsdDef.h>

void ulnShardDbcContextInitialize(ulnDbc *aDbc)
{
    /* PROJ-2598 altibase sharding */
    aDbc->mShardDbcCxt.mShardDbc                    = NULL;
    aDbc->mShardDbcCxt.mParentDbc                   = NULL;
    aDbc->mShardDbcCxt.mNodeInfo                    = NULL;

    aDbc->mShardDbcCxt.mShardIsNodeTransactionBegin = ACP_FALSE;
    aDbc->mShardDbcCxt.mShardTransactionLevel       = ULN_SHARD_TX_MULTI_NODE;

    /* PROJ-2638 shard native linker */
    aDbc->mShardDbcCxt.mShardTargetDataNodeName[0]  = '\0';
    aDbc->mShardDbcCxt.mShardLinkerType             = 0;

    /* PROJ-2660 hybrid sharding */
    aDbc->mShardDbcCxt.mShardPin                    = ULSD_SHARD_PIN_INVALID;

    /* BUG-46090 Meta Node SMN 전파 */
    aDbc->mShardDbcCxt.mShardMetaNumber             = 0;

    /* BUG-45967 Data Node의 Shard Session 정리 */
    aDbc->mShardDbcCxt.mSMNOfDataNode               = 0;

    /* BUG-46100 Session SMN Update */
    aDbc->mShardDbcCxt.mNeedToDisconnect            = ACP_FALSE;

    /* BUG-45509 nested commit */
    aDbc->mShardDbcCxt.mCallback                    = NULL;

    /* BUG-45411 */
    aDbc->mShardDbcCxt.mReadOnlyTx                  = ACP_FALSE;

    aDbc->mShardDbcCxt.mShardConnType               = ULN_CONNTYPE_INVALID;

    /* BUG-45707 */
    aDbc->mShardDbcCxt.mShardClient                 = ULSD_SHARD_CLIENT_FALSE;
    aDbc->mShardDbcCxt.mShardSessionType            = ULSD_SESSION_TYPE_EXTERNAL;

    /* BUG-46257 shardcli에서 Node 추가/제거 지원 */
    aDbc->mShardDbcCxt.mNodeBaseConnString          = NULL;
    acpListInit( & aDbc->mShardDbcCxt.mConnectAttrList );

#ifdef COMPILE_SHARDCLI /* BUG-46092 */
    ulsdAlignInfoInitialize( aDbc );
#endif /* COMPILE_SHARDCLI */

    aDbc->mShardModule                              = NULL;
}

#ifdef COMPILE_SHARDCLI
void ulnShardDbcContextFinalize( ulnDbc *aDbc )
{
    /* BUG-46092 */
    ulsdAlignInfoFinalize( aDbc );
}
#endif /* COMPILE_SHARDCLI */

void ulnShardStmtContextInitialize(ulnStmt *aStmt)
{
    aStmt->mShardStmtCxt.mShardNodeStmt          = NULL;

    /* PROJ-2598 Shard pilot(shard analyze) */
    aStmt->mShardStmtCxt.mShardRangeInfoCnt      = 0;
    aStmt->mShardStmtCxt.mShardRangeInfo         = NULL;

    /* PROJ-2638 shard native linker */
    aStmt->mShardStmtCxt.mRowDataBuffer          = NULL;
    aStmt->mShardStmtCxt.mRowDataBufLen          = 0;
    aStmt->mShardStmtCxt.mIsMtDataFetch          = ACP_FALSE;

    /* PROJ-2646 New shard analyzer */
    aStmt->mShardStmtCxt.mShardValueCnt          = 0;
    aStmt->mShardStmtCxt.mShardValueInfo         = NULL;
    aStmt->mShardStmtCxt.mShardIsCanMerge        = ACP_FALSE;

    /* PROJ-2655 Composite shard key */
    aStmt->mShardStmtCxt.mShardIsSubKeyExists    = ACP_FALSE;
    aStmt->mShardStmtCxt.mShardSubValueCnt       = 0;
    aStmt->mShardStmtCxt.mShardSubValueInfo      = NULL;

    /* PROJ-2670 nested execution */
    aStmt->mShardStmtCxt.mCallback               = NULL;

    /* PROJ-2660 hybrid sharding */
    aStmt->mShardStmtCxt.mShardCoordQuery        = ACP_FALSE;

    /* BUG-45499 result merger */
    aStmt->mShardStmtCxt.mNodeDbcIndexCount      = 0;
    aStmt->mShardStmtCxt.mNodeDbcIndexCur        = -1;

    aStmt->mShardModule                          = NULL;

    /* BUG-46100 Session SMN Update */
    aStmt->mShardStmtCxt.mShardMetaNumber         = 0;
    aStmt->mShardStmtCxt.mOrgPrepareTextBuf       = NULL;
    aStmt->mShardStmtCxt.mOrgPrepareTextBufMaxLen = 0;
    aStmt->mShardStmtCxt.mOrgPrepareTextBufLen    = 0;

    /* BUG-46257 shardcli에서 Node 추가/제거 지원 */
    acpListInit( & aStmt->mShardStmtCxt.mStmtAttrList );
    acpListInit( & aStmt->mShardStmtCxt.mBindParameterList );
    acpListInit( & aStmt->mShardStmtCxt.mBindColList );
    aStmt->mShardStmtCxt.mRowCount               = 0;
}
