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
    return ulsdModuleCloseCursor(aStmt);
}

/*
 * � Node �� ����Ǿ����� ������ üũ�� ���� �����Ƿ�
 * Close �� ��ü Node �� ���� Silent �� ������
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
