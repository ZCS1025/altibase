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

SQLRETURN ulsdCloseCursor(ulnStmt      *aStmt)
{
    SQLRETURN      sNodeResult = SQL_ERROR;
    ulnFnContext   sFnContext;
    ulnFnContext   sFnContextSMNUpdate;

    ULN_INIT_FUNCTION_CONTEXT( sFnContext, ULN_FID_CLOSECURSOR, aStmt, ULN_OBJ_TYPE_STMT );

    sNodeResult = ulsdModuleCloseCursor( aStmt );

    /* BUG-46100 Session SMN Update */
    if ( ( aStmt->mParentDbc->mAttrAutoCommit == SQL_AUTOCOMMIT_ON ) &&
         ( ulsdIsTimeToUpdateShardMetaNumber( aStmt->mParentDbc ) == ACP_TRUE ) )
    {
        ULN_INIT_FUNCTION_CONTEXT( sFnContextSMNUpdate, ULN_FID_CLOSECURSOR, aStmt, ULN_OBJ_TYPE_STMT );

        ACI_TEST( ulsdUpdateShardMetaNumber( aStmt->mParentDbc, & sFnContextSMNUpdate ) != SQL_SUCCESS );

        if ( sNodeResult != SQL_SUCCESS )
        {
            ulnError( & sFnContext, ulERR_ABORT_SHARD_META_CHANGED );
        }
    }
    else
    {
        /* Nothing to do */
    }

    return sNodeResult;

    ACI_EXCEPTION_END;

    return SQL_ERROR;
}

/*
 * 어떤 Node 가 수행되었는지 별도로 체크를 하지 않으므로
 * Close 는 전체 Node 에 대해 Silent 로 수행함
 */
SQLRETURN ulsdNodeSilentCloseCursor(ulsdDbc      *aShard,
                                    ulnStmt      *aStmt)
{
    acp_uint16_t    i;

    for ( i = 0; i < aShard->mNodeCount; i++ )
    {
        SHARD_LOG("(Close Cursor) NodeId=%d, Server=%s:%d, StmtID=%d\n",
                  aShard->mNodeInfo[i]->mNodeId,
                  aShard->mNodeInfo[i]->mServerIP,
                  aShard->mNodeInfo[i]->mPortNo,
                  aStmt->mShardStmtCxt.mShardNodeStmt[i]->mStatementID);

        ulnCloseCursor(aStmt->mShardStmtCxt.mShardNodeStmt[i]);
    }

    return SQL_SUCCESS;
}
