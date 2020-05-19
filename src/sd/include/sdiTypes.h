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
 * $Id: sdiTypes.h 83512 2018-07-18 00:47:26Z hykim $
 *
 * Description : SD 및 타 모듈에서도 사용하는 자료구조 정의
 **********************************************************************/

#ifndef _O_SDI_TYPES_H_
#define _O_SDI_TYPES_H_ 1

#include <idTypes.h>

typedef enum
{
    /*
     * PROJ-2646 shard analyzer enhancement
     * boolean array에 shard CAN-MERGE false인 이유를 체크한다.
     *
     * BUG-45718
     * Reason이 여러개 일 때
     * Enum의 순번이 낮을수록
     * setCanNotMergeReasonErr()에서 error message설정의 우선순위가 높다.
     */

    // SDI_CAN_NOT_MERGE_REASONS 의 순서와 동일해야 한다.

    SDI_NO_SHARD_OBJECT               =  0, // SHARD TABLE이 하나도 없음
    SDI_NON_SHARD_OBJECT_EXISTS       =  1, // SHARD META에 등록되지 않은 TABLE이 사용 됨
    SDI_MULTI_SHARD_INFO_EXISTS       =  2, // 분산 정의가 다른 SHARD TABLE들이 사용 됨
    SDI_MULTI_NODES_JOIN_EXISTS       =  3, // 노드 간 JOIN이 필요함
    SDI_HIERARCHY_EXISTS              =  4, // CONNECT BY가 사용 됨
    SDI_DISTINCT_EXISTS               =  5, // 노드 간 연산이 필요한 DISTINCT가 사용 됨
    SDI_GROUP_AGGREGATION_EXISTS      =  6, // 노드 간 연산이 필요한 GROUP BY가 사용 됨
    SDI_NESTED_AGGREGATION_EXISTS     =  7, // 노드 간 연산이 필요한 GROUP BY가 사용 됨
    SDI_SUBQUERY_EXISTS               =  8, // SHARD SUBQUERY가 사용 됨
    SDI_ORDER_BY_EXISTS               =  9, // 노드 간 연산이 필요한 ORDER BY가 사용 됨
    SDI_LIMIT_EXISTS                  = 10, // 노드 간 연산이 필요한 LIMIT가 사용 됨
    SDI_MULTI_NODES_SET_OP_EXISTS     = 11, // 노드 간 연산이 필요한 SET operator가 사용 됨
    SDI_LOOP_EXISTS                   = 12, // 노드 간 연산이 필요한 LOOP가 사용 됨
    SDI_INVALID_OUTER_JOIN_EXISTS     = 13, // CLONE table이 left-side에고, HASH,RANGE,LIST table이 right-side에 오는 outer join이 존재한다.
    SDI_INVALID_SEMI_ANTI_JOIN_EXISTS = 14, // HASH,RANGE,LIST table이 inner(subquery table)로 오는 semi/anti-join이 존재한다.
    SDI_GROUP_BY_EXTENSION_EXISTS     = 15, // Group by extension(ROLLUP,CUBE,GROUPING SETS)가 존재한다.
    SDI_INVALID_SHARD_KEY_CONDITION   = 16, // BUG-45899 
    SDI_MULTIPLE_ROW_INSERT           = 17,
    SDI_MULTIPLE_OBJECT_INSERT        = 18,
    SDI_TRIGGER_EXISTS                = 19,
    SDI_INSTEAD_OF_TRIGGER_EXISTS     = 20,
    SDI_VIEW_EXISTS                   = 21,
    SDI_LATERAL_VIEW_EXISTS           = 22,
    SDI_RECURSIVE_VIEW_EXISTS         = 23,
    SDI_PACKAGE_EXISTS                = 24,
    SDI_PIVOT_EXISTS                  = 25,
    SDI_UNSUPPORT_SHARD_QUERY         = 26,
    SDI_SUB_KEY_EXISTS                = 27, // Sub-shard key를 가진 table이 참조 됨
                                            // (Can not merge reason은 아니지만, query block 간 전달이 필요해 넣어둔다.)
                                            // SDI_SUB_KEY_EXISTS가 마지막에 와야한다.
    SDI_CAN_NOT_MERGE_REASON_MAX      = 28
} sdiCanNotMergeReason;

#define SDI_CAN_NOT_MERGE_REASONS \
    (SChar*)"Shard object doesn't exist",\
    (SChar*)"Non-shard object exists",\
    (SChar*)"Shard object info is not same",\
    (SChar*)"Shard value doesn't exist",\
    (SChar*)"HIERARCHY needed multiple nodes",\
    (SChar*)"DISTINCT needed multiple nodes",\
    (SChar*)"GROUP BY needed multiple nodes",\
    (SChar*)"NESTED AGGREGATE FUNCTION needed multiple nodes",\
    (SChar*)"Shard sub-query exists",\
    (SChar*)"ORDER BY cluase exists",\
    (SChar*)"LIMIT clause exists",\
    (SChar*)"SET operator needed multiple nodes",\
    (SChar*)"LOOP clause exists",\
    (SChar*)"Invalid outer join operator exists",\
    (SChar*)"Invalid semi/anti-join exists",\
    (SChar*)"Invalid group by extension exists",\
    (SChar*)"Invalid shard key condition",\
    (SChar*)"Multiple row insert",\
    (SChar*)"Multiple object insert",\
    (SChar*)"Trigger exists",\
    (SChar*)"Instead of trigger exists",\
    (SChar*)"View exists",\
    (SChar*)"Lateral view exists",\
    (SChar*)"Recursive view exists",\
    (SChar*)"Package exists",\
    (SChar*)"Pivot exists",\
    (SChar*)"The statement is not supported as a shard query"\
// 주의 : 마지막 reason 에는 ',' 를 생략할 것!

#define SDI_INIT_CAN_NOT_MERGE_REASON(_dst_)              \
{                                                         \
    _dst_[SDI_NO_SHARD_OBJECT]               = ID_FALSE;  \
    _dst_[SDI_NON_SHARD_OBJECT_EXISTS]       = ID_FALSE;  \
    _dst_[SDI_MULTI_SHARD_INFO_EXISTS]       = ID_FALSE;  \
    _dst_[SDI_MULTI_NODES_JOIN_EXISTS]       = ID_FALSE;  \
    _dst_[SDI_HIERARCHY_EXISTS]              = ID_FALSE;  \
    _dst_[SDI_DISTINCT_EXISTS]               = ID_FALSE;  \
    _dst_[SDI_GROUP_AGGREGATION_EXISTS]      = ID_FALSE;  \
    _dst_[SDI_NESTED_AGGREGATION_EXISTS]     = ID_FALSE;  \
    _dst_[SDI_SUBQUERY_EXISTS]               = ID_FALSE;  \
    _dst_[SDI_ORDER_BY_EXISTS]               = ID_FALSE;  \
    _dst_[SDI_LIMIT_EXISTS]                  = ID_FALSE;  \
    _dst_[SDI_MULTI_NODES_SET_OP_EXISTS]     = ID_FALSE;  \
    _dst_[SDI_LOOP_EXISTS]                   = ID_FALSE;  \
    _dst_[SDI_INVALID_OUTER_JOIN_EXISTS]     = ID_FALSE;  \
    _dst_[SDI_INVALID_SEMI_ANTI_JOIN_EXISTS] = ID_FALSE;  \
    _dst_[SDI_GROUP_BY_EXTENSION_EXISTS]     = ID_FALSE;  \
    _dst_[SDI_INVALID_SHARD_KEY_CONDITION]   = ID_FALSE;  \
    _dst_[SDI_MULTIPLE_ROW_INSERT]           = ID_FALSE;  \
    _dst_[SDI_MULTIPLE_OBJECT_INSERT]        = ID_FALSE;  \
    _dst_[SDI_TRIGGER_EXISTS]                = ID_FALSE;  \
    _dst_[SDI_INSTEAD_OF_TRIGGER_EXISTS]     = ID_FALSE;  \
    _dst_[SDI_VIEW_EXISTS]                   = ID_FALSE;  \
    _dst_[SDI_LATERAL_VIEW_EXISTS]           = ID_FALSE;  \
    _dst_[SDI_RECURSIVE_VIEW_EXISTS]         = ID_FALSE;  \
    _dst_[SDI_PACKAGE_EXISTS]                = ID_FALSE;  \
    _dst_[SDI_PIVOT_EXISTS]                  = ID_FALSE;  \
    _dst_[SDI_UNSUPPORT_SHARD_QUERY]         = ID_FALSE;  \
    _dst_[SDI_SUB_KEY_EXISTS]                = ID_FALSE;  \
}

typedef enum
{
    SDI_SHARD_CLIENT_FALSE = 0,
    SDI_SHARD_CLIENT_TRUE  = 1,
} sdiShardClient;

typedef enum
{
    SDI_SESSION_TYPE_EXTERNAL = 0,
    SDI_SESSION_TYPE_INTERNAL = 1,
} sdiSessionType;

typedef enum
{
    SDI_QUERY_TYPE_NONE     = 0,
    SDI_QUERY_TYPE_SHARD    = 1,
    SDI_QUERY_TYPE_NONSHARD = 2,
    SDI_QUERY_TYPE_MAX      = 3 
} sdiQueryType;

typedef struct sdiPrintInfo
{
    UInt           mAnalyzeCount;
    sdiQueryType   mQueryType;
    UShort         mNonShardQueryReason;
    idBool         mTransformable;
} sdiPrintInfo;

#define SDI_INIT_PRINT_INFO( info )                              \
{                                                                \
    (info)->mAnalyzeCount        = 0;                            \
    (info)->mQueryType           = SDI_QUERY_TYPE_NONE;          \
    (info)->mTransformable       = ID_FALSE;                     \
    (info)->mNonShardQueryReason = SDI_CAN_NOT_MERGE_REASON_MAX; \
}

#endif /* _O_SDI_TYPES_H_ */
