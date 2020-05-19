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
 * $Id$
 **********************************************************************/

#ifndef _O_SDI_H_
#define _O_SDI_H_ 1

#include <idl.h>
#include <idu.h>
#include <ide.h>
#include <sdiTypes.h>
#include <sde.h>
#include <sduProperty.h>
#include <qci.h>
#include <smi.h>

#define SDI_HASH_MAX_VALUE                     (1000)
#define SDI_HASH_MAX_VALUE_FOR_TEST            (100)

// range, list sharding의 varchar shard key column의 max precision
#define SDI_RANGE_VARCHAR_MAX_PRECISION        (100)
#define SDI_RANGE_VARCHAR_MAX_PRECISION_STR    "100"
#define SDI_RANGE_VARCHAR_MAX_SIZE             (MTD_CHAR_TYPE_STRUCT_SIZE(SDI_RANGE_VARCHAR_MAX_PRECISION))

#define SDI_NODE_MAX_COUNT                     (128)
#define SDI_RANGE_MAX_COUNT                    (1000)
#define SDI_VALUE_MAX_COUNT                    (1000)

#define SDI_SERVER_IP_SIZE                     (16)
#define SDI_NODE_NAME_MAX_SIZE                 (QC_MAX_NAME_LEN)

/* Full address: NODE1:xxx.xxx.xxx.xxx:nnnn */
#define SDI_PORT_NUM_BUFFER_SIZE               (10)
#define SDI_FULL_SERVER_ADDRESS_SIZE           ( SDI_NODE_NAME_MAX_SIZE   \
                                               + SDI_SERVER_IP_SIZE       \
                                               + SDI_PORT_NUM_BUFFER_SIZE )

/* PROJ-2661 */
#define SDI_XA_RECOVER_RMID                    (0)
#define SDI_XA_RECOVER_COUNT                   (5)

/*               01234567890123456789
 * Max ShardPIN: 255-65535-4294967295
 */
#define SDI_MAX_SHARD_PIN_STR_LEN              (20) /* Without Null terminated */
#define SDI_SHARD_PIN_FORMAT_STR   "%"ID_UINT32_FMT"-%"ID_UINT32_FMT"-%"ID_UINT32_FMT
#define SDI_SHARD_PIN_FORMAT_ARG( __SHARD_PIN ) \
    ( __SHARD_PIN & ( (sdiShardPin)0xff       << SDI_OFFSET_VERSION      ) ) >> SDI_OFFSET_VERSION, \
    ( __SHARD_PIN & ( (sdiShardPin)0xffff     << SDI_OFFSET_META_NODE_ID ) ) >> SDI_OFFSET_META_NODE_ID, \
    ( __SHARD_PIN & ( (sdiShardPin)0xffffffff << SDI_OFFSET_SEQEUNCE     ) ) >> SDI_OFFSET_SEQEUNCE

#define SDI_ODBC_CONN_ATTR_RETRY_COUNT_DEFAULT   (IDP_SHARD_INTERNAL_CONN_ATTR_RETRY_COUNT_DEFAULT)
#define SDI_ODBC_CONN_ATTR_RETRY_DELAY_DEFAULT   (IDP_SHARD_INTERNAL_CONN_ATTR_RETRY_DELAY_DEFAULT)
#define SDI_ODBC_CONN_ATTR_CONN_TIMEOUT_DEFAULT  (IDP_SHARD_INTERNAL_CONN_ATTR_CONNECTION_TIMEOUT_DEFAULT)
#define SDI_ODBC_CONN_ATTR_LOGIN_TIMEOUT_DEFAULT (IDP_SHARD_INTERNAL_CONN_ATTR_LOGIN_TIMEOUT_DEFAULT)

typedef enum
{
    SDI_XA_RECOVER_START = 0,
    SDI_XA_RECOVER_CONT  = 1,
    SDI_XA_RECOVER_END   = 2
} sdiXARecoverOption;

typedef ULong  sdiShardPin;
typedef UShort sdiMetaNodeId;

typedef enum
{
    SDI_OFFSET_VERSION      = 56,   /* << 7 byte, sdmShardPinInfo.mVersion                  */
    SDI_OFFSET_RESERVED     = 48,   /* << 6 byte, sdmShardPinInfo.mReserved                 */
    SDI_OFFSET_META_NODE_ID = 32,   /* << 4 byte, sdmShardPinInfo.mMetaNodeInfo.mMetaNodeId */
    SDI_OFFSET_SEQEUNCE     = 0,    /* << 0 byte, sdmShardPinInfo.mSeq                      */
} sdiShardPinFactorOffset;

typedef struct sdiLocalMetaNodeInfo
{
    sdiMetaNodeId           mMetaNodeId;
} sdiLocalMetaNodeInfo;

typedef struct sdiGlobalMetaNodeInfo
{
    ULong                   mShardMetaNumber;
} sdiGlobalMetaNodeInfo;

typedef enum
{
    SDI_SHARD_PIN_INVALID = 0,
} sdiShardPinStatus;

typedef enum
{
    SDI_SPLIT_NONE  = 0,
    SDI_SPLIT_HASH  = 1,
    SDI_SPLIT_RANGE = 2,
    SDI_SPLIT_LIST  = 3,
    SDI_SPLIT_CLONE = 4,
    SDI_SPLIT_SOLO  = 5,
    SDI_SPLIT_NODES = 100
} sdiSplitMethod;

typedef enum
{
    SDI_DATA_NODE_CONNECT_TYPE_DEFAULT = 0,
    SDI_DATA_NODE_CONNECT_TYPE_TCP     = 1,  /* = ULN_CONNTYPE_TCP */
    SDI_DATA_NODE_CONNECT_TYPE_IB      = 8,  /* = ULN_CONNTYPE_IB  */
    /* else NOT SUPPORT */
} sdiDataNodeConnectType;

typedef enum {
    SDI_FAILOVER_ACTIVE_ONLY    = 0,        /* = ULSD_CONN_TO_ACTIVE */
    SDI_FAILOVER_ALTERNATE_ONLY = 1,        /* = ULSD_CONN_TO_ALTERNATE */
    SDI_FAILOVER_ALL            = 2,
    SDI_FAILOVER_MAX            = 3,
    SDI_FAILOVER_UCHAR_MAX      = 255,
    SDI_FAILOVER_NOT_USED       = SDI_FAILOVER_UCHAR_MAX,
    /* Do not over 0xff(255). Values cast into UChar. */
} sdiFailOverTarget;

typedef struct sdiNode
{
    UInt            mNodeId;
    SChar           mNodeName[SDI_NODE_NAME_MAX_SIZE + 1];
    SChar           mServerIP[SDI_SERVER_IP_SIZE];
    UShort          mPortNo;
    SChar           mAlternateServerIP[SDI_SERVER_IP_SIZE];
    UShort          mAlternatePortNo;
    UShort          mConnectType;
} sdiNode;

typedef struct sdiNodeInfo
{
    UShort          mCount;
    sdiNode         mNodes[SDI_NODE_MAX_COUNT];
} sdiNodeInfo;

typedef union sdiValue
{
    // hash shard인 경우
    UInt        mHashMax;

    // range shard인 경우
    UChar       mMax[1];      // 대표값
    SShort      mSmallintMax; // shard key가 smallint type인 경우
    SInt        mIntegerMax;  // shard key가 integer type인 경우
    SLong       mBigintMax;   // shard key가 bigint type인 경우
    mtdCharType mCharMax;     // shard key가 char/varchar type인 경우
    UShort      mCharMaxBuf[(SDI_RANGE_VARCHAR_MAX_SIZE + 1) / 2];  // 2byte align

    // bind parameter인 경우
    UShort      mBindParamId;

} sdiValue;

typedef struct sdiRange
{
    UInt     mNodeId;
    sdiValue mValue;
    sdiValue mSubValue;
} sdiRange;

typedef struct sdiRangeInfo
{
    UShort          mCount;
    sdiRange      * mRanges;
} sdiRangeInfo;

typedef struct sdiTableInfo
{
    UInt            mShardID;
    SChar           mUserName[QC_MAX_OBJECT_NAME_LEN + 1];
    SChar           mObjectName[QC_MAX_OBJECT_NAME_LEN + 1];
    SChar           mObjectType;
    SChar           mKeyColumnName[QC_MAX_OBJECT_NAME_LEN + 1];
    UInt            mKeyDataType;   // shard key의 mt type id
    UShort          mKeyColOrder;
    sdiSplitMethod  mSplitMethod;

    /* PROJ-2655 Composite shard key */
    idBool          mSubKeyExists;
    SChar           mSubKeyColumnName[QC_MAX_OBJECT_NAME_LEN + 1];
    UInt            mSubKeyDataType;   // shard key의 mt type id
    UShort          mSubKeyColOrder;
    sdiSplitMethod  mSubSplitMethod;

    UInt            mDefaultNodeId; // default node id
} sdiTableInfo;

#define SDI_INIT_TABLE_INFO( info )                 \
{                                                   \
    (info)->mShardID             = 0;               \
    (info)->mUserName[0]         = '\0';            \
    (info)->mObjectName[0]       = '\0';            \
    (info)->mObjectType          = 0;               \
    (info)->mKeyColumnName[0]    = '\0';            \
    (info)->mKeyDataType         = ID_UINT_MAX;     \
    (info)->mKeyColOrder         = 0;               \
    (info)->mSplitMethod         = SDI_SPLIT_NONE;  \
    (info)->mSubKeyExists        = ID_FALSE;        \
    (info)->mSubKeyColumnName[0] = '\0';            \
    (info)->mSubKeyDataType      = 0;               \
    (info)->mSubKeyColOrder      = 0;               \
    (info)->mSubSplitMethod      = SDI_SPLIT_NONE;  \
    (info)->mDefaultNodeId       = ID_UINT_MAX;     \
}

typedef struct sdiTableInfoList
{
    sdiTableInfo            * mTableInfo;
    struct sdiTableInfoList * mNext;
} sdiTableInfoList;

typedef struct sdiObjectInfo
{
    ULong           mSMN;
    sdiTableInfo    mTableInfo;
    sdiRangeInfo    mRangeInfo;

    /* PROJ-2701 Online data rebuild
     * SMN history별로 shard object information가 생성된다.
     * 현재는 sessionSMN, dataSMN 당 하나씩 총 2개가 생성될 수 있다.
     */
    struct sdiObjectInfo * mNext;

    // view인 경우 key가 여러개일 수 있어 컬럼 갯수만큼 할당하여 사용한다.
    UChar           mKeyFlags[1];

} sdiObjectInfo;

#define SDI_INIT_OBJECT_INFO( info )            \
{                                               \
    (info)->mSMN               = ID_ULONG(0);   \
    SDI_INIT_TABLE_INFO(&((info)->mTableInfo)); \
    (info)->mRangeInfo.mCount  = 0;             \
    (info)->mRangeInfo.mRanges = NULL;          \
    (info)->mNext              = NULL;          \
    (info)->mKeyFlags[0]       = '\0';          \
}

typedef struct sdiValueInfo
{
    UChar    mType;             // 0 : hostVariable, 1 : constant
    sdiValue mValue;

} sdiValueInfo;

typedef struct sdiAnalyzeInfo
{
    UShort             mValueCount;
    sdiValueInfo     * mValue;
    idBool             mIsCanMerge;
    idBool             mIsTransformAble; // aggr transformation 수행여부
    sdiSplitMethod     mSplitMethod;  // shard key의 split method
    UInt               mKeyDataType;  // shard key의 mt type id

    /* PROJ-2655 Composite shard key */
    idBool             mSubKeyExists;
    UShort             mSubValueCount;
    sdiValueInfo     * mSubValue;
    sdiSplitMethod     mSubSplitMethod;  // sub sub key의 split method
    UInt               mSubKeyDataType;  // sub shard key의 mt type id

    UInt               mDefaultNodeId;// shard query의 default node id
    sdiRangeInfo       mRangeInfo;    // shard query의 분산정보

    // BUG-45359
    qcShardNodes     * mNodeNames;

    // PROJ-2685 online rebuild
    sdiTableInfoList * mTableInfoList;

    /* BUG-45899 */
    UShort             mNonShardQueryReason;
} sdiAnalyzeInfo;

#define SDI_INIT_ANALYZE_INFO( info )            \
{                                                \
    (info)->mValueCount        = 0;              \
    (info)->mValue             = NULL;           \
    (info)->mIsCanMerge        = ID_FALSE;       \
    (info)->mIsTransformAble   = ID_FALSE;       \
    (info)->mSplitMethod       = SDI_SPLIT_NONE; \
    (info)->mKeyDataType       = ID_UINT_MAX;    \
    (info)->mSubKeyExists      = ID_FALSE;       \
    (info)->mSubValueCount     = 0;              \
    (info)->mSubValue          = NULL;           \
    (info)->mSubSplitMethod    = SDI_SPLIT_NONE; \
    (info)->mSubKeyDataType    = ID_UINT_MAX;    \
    (info)->mDefaultNodeId     = ID_UINT_MAX;    \
    (info)->mRangeInfo.mCount  = 0;              \
    (info)->mRangeInfo.mRanges = NULL;           \
    (info)->mNodeNames         = NULL;           \
    (info)->mTableInfoList     = NULL;           \
    (info)->mNonShardQueryReason = SDI_CAN_NOT_MERGE_REASON_MAX;   \
}

// PROJ-2646
typedef struct sdiShardInfo
{
    UInt                  mKeyDataType;
    UInt                  mDefaultNodeId;
    sdiSplitMethod        mSplitMethod;
    struct sdiRangeInfo   mRangeInfo;

} sdiShardInfo;

typedef struct sdiKeyTupleColumn
{
    UShort mTupleId;
    UShort mColumn;
    idBool mIsNullPadding;
    idBool mIsAntiJoinInner;

} sdiKeyTupleColumn;

typedef struct sdiKeyInfo
{
    UInt                  mKeyTargetCount;
    UShort              * mKeyTarget;

    UInt                  mKeyCount;
    sdiKeyTupleColumn   * mKey;

    UShort                mValueCount;
    sdiValueInfo        * mValue;

    sdiShardInfo          mShardInfo;

    idBool                mIsJoined;

    sdiKeyInfo          * mLeft;
    sdiKeyInfo          * mRight;
    sdiKeyInfo          * mOrgKeyInfo;

    sdiKeyInfo          * mNext;

} sdiKeyInfo;

#define SDI_INIT_KEY_INFO( info )                           \
{                                                           \
    (info)->mKeyTargetCount               = 0;              \
    (info)->mKeyTarget                    = NULL;           \
    (info)->mKeyCount                     = 0;              \
    (info)->mKey                          = NULL;           \
    (info)->mValueCount                   = 0;              \
    (info)->mValue                        = NULL;           \
    (info)->mShardInfo.mKeyDataType       = ID_UINT_MAX;    \
    (info)->mShardInfo.mDefaultNodeId     = ID_UINT_MAX;    \
    (info)->mShardInfo.mSplitMethod       = SDI_SPLIT_NONE; \
    (info)->mShardInfo.mRangeInfo.mCount  = 0;              \
    (info)->mShardInfo.mRangeInfo.mRanges = NULL;           \
    (info)->mIsJoined                     = ID_FALSE;       \
    (info)->mLeft                         = NULL;           \
    (info)->mRight                        = NULL;           \
    (info)->mOrgKeyInfo                   = NULL;           \
    (info)->mNext                         = NULL;           \
}

typedef struct sdiParseTree
{
    sdiQuerySet * mQuerySetAnalysis;

    idBool        mIsCanMerge;
    idBool        mCantMergeReason[SDI_CAN_NOT_MERGE_REASON_MAX];

    /* PROJ-2655 Composite shard key */
    idBool        mIsCanMerge4SubKey;
    idBool        mCantMergeReason4SubKey[SDI_CAN_NOT_MERGE_REASON_MAX];

} sdiParseTree;

typedef struct sdiQuerySet
{
    idBool                mIsCanMerge;
    idBool                mCantMergeReason[SDI_CAN_NOT_MERGE_REASON_MAX];
    sdiShardInfo          mShardInfo;
    sdiKeyInfo          * mKeyInfo;

    /* PROJ-2655 Composite shard key */
    idBool                mIsCanMerge4SubKey;
    idBool                mCantMergeReason4SubKey[SDI_CAN_NOT_MERGE_REASON_MAX];
    sdiShardInfo          mShardInfo4SubKey;
    sdiKeyInfo          * mKeyInfo4SubKey;

    /* PROJ-2685 online rebuild */
    sdiTableInfoList    * mTableInfoList;

} sdiQuerySet;

#define SDI_INIT_QUERY_SET( info )                                  \
{                                                                   \
    (info)->mIsCanMerge                          = ID_FALSE;        \
    (info)->mShardInfo.mKeyDataType              = ID_UINT_MAX;     \
    (info)->mShardInfo.mDefaultNodeId            = ID_UINT_MAX;     \
    (info)->mShardInfo.mSplitMethod              = SDI_SPLIT_NONE;  \
    (info)->mShardInfo.mRangeInfo.mCount         = 0;               \
    (info)->mShardInfo.mRangeInfo.mRanges        = NULL;            \
    (info)->mKeyInfo                             = NULL;            \
    (info)->mIsCanMerge4SubKey                   = ID_FALSE;        \
    (info)->mShardInfo4SubKey.mKeyDataType       = ID_UINT_MAX;     \
    (info)->mShardInfo4SubKey.mDefaultNodeId     = ID_UINT_MAX;     \
    (info)->mShardInfo4SubKey.mSplitMethod       = SDI_SPLIT_NONE;  \
    (info)->mShardInfo4SubKey.mRangeInfo.mCount  = 0;               \
    (info)->mShardInfo4SubKey.mRangeInfo.mRanges = NULL;            \
    (info)->mKeyInfo4SubKey                      = NULL;            \
    (info)->mTableInfoList                       = NULL;            \
    SDI_INIT_CAN_NOT_MERGE_REASON((info)->mCantMergeReason);        \
    SDI_INIT_CAN_NOT_MERGE_REASON((info)->mCantMergeReason4SubKey); \
}

/* BUG-46623
 * sdiBindParam: memcmp 함수를 사용하여 비교하는 구조체 이므로
 * 1. 멤버 변수의 크기에 따라 align 에 맞게 변수를 정의해야 하며
 *    (64 bit system 8 byte align 을 고려해야 한다.)
 * 2. padding 변수를 0으로 초기화 해야 한다.
 *    현재는 qmnSDEX::setParamInfo, qmnSDIN::setParamInfo,
 *    qmnSDSE::setParamInfo 함수에서 초기화 하고 있다.
 */
typedef struct sdiBindParam
{
    void       * mData;
    UInt         mInoutType;
    UInt         mType;
    UInt         mDataSize;
    SInt         mPrecision;
    SInt         mScale;
    UShort       mId;
    UShort       padding;
} sdiBindParam;

typedef struct sdlRemoteStmt sdlRemoteStmt;

typedef enum sdiSVPStep
{
    SDI_SVP_STEP_DO_NOT_NEED_SAVEPOINT   = 1,
    SDI_SVP_STEP_NEED_SAVEPOINT          = 2,
    SDI_SVP_STEP_SET_SAVEPOINT           = 3,
    SDI_SVP_STEP_ROLLBACK_TO_SAVEPOINT   = 4
} sdiSVPStep;

// PROJ-2638
typedef struct sdiDataNode
{
    sdlRemoteStmt * mRemoteStmt;          // data node stmt

    void          * mBuffer[SDI_NODE_MAX_COUNT];  // data node fetch buffer
    UInt          * mOffset;              // meta node column offset array
    UInt          * mMaxByteSize;         // meta node column max byte size array
    UInt            mBufferLength;        // data node fetch buffer length
    UShort          mColumnCount;         // data node column count

    sdiBindParam  * mBindParams;          // data node parameters
    UShort          mBindParamCount;
    idBool          mBindParamChanged;

    SChar         * mPlanText;            // data node plan text (alloc&free는 cli library에서한다.)
    UInt            mExecCount;           // data node execution count

    UChar           mState;               // date node state

    sdiSVPStep      mSVPStep;             // for shard stmt partial rollback
} sdiDataNode;

#define SDI_NODE_STATE_NONE               0    // 초기상태
#define SDI_NODE_STATE_PREPARE_CANDIDATED 1    // prepare 후보노드가 선정된 상태
#define SDI_NODE_STATE_PREPARE_SELECTED   2    // prepare 후보노드에서 prepare 노드로 선택된 생태
#define SDI_NODE_STATE_PREPARED           3    // prepared 상태
#define SDI_NODE_STATE_EXECUTE_CANDIDATED 4    // execute 후보노드가 선정된 상태
#define SDI_NODE_STATE_EXECUTE_SELECTED   5    // execute 후보노드에서 execute 노드로 선택된 상태
#define SDI_NODE_STATE_EXECUTED           6    // executed 상태
#define SDI_NODE_STATE_FETCHED            7    // fetch 완료 상태 (only SELECT)

typedef struct sdiDataNodes
{
    idBool         mInitialized;
    UShort         mCount;
    sdiDataNode    mNodes[SDI_NODE_MAX_COUNT];
} sdiDataNodes;

/* sdiConnectInfo.mFlag */
#define SDI_CONNECT_PLANATTR_CHANGE_MASK         (0x00000001)
#define SDI_CONNECT_PLANATTR_CHANGE_FALSE        (0x00000000)
#define SDI_CONNECT_PLANATTR_CHANGE_TRUE         (0x00000001)

/* sdiConnectInfo.mFlag */
#define SDI_CONNECT_INITIAL_BY_NOTIFIER_MASK     (0x00000002)
#define SDI_CONNECT_INITIAL_BY_NOTIFIER_FALSE    (0x00000000)
#define SDI_CONNECT_INITIAL_BY_NOTIFIER_TRUE     (0x00000002)

/* sdiConnectInfo.mFlag */
#define SDI_CONNECT_REMOTE_TX_CREATE_MASK        (0x00000004)
#define SDI_CONNECT_REMOTE_TX_CREATE_FALSE       (0x00000000)
#define SDI_CONNECT_REMOTE_TX_CREATE_TRUE        (0x00000004)

/* sdiConnectInfo.mFlag */
#define SDI_CONNECT_COMMIT_PREPARE_MASK          (0x00000008)
#define SDI_CONNECT_COMMIT_PREPARE_FALSE         (0x00000000)
#define SDI_CONNECT_COMMIT_PREPARE_TRUE          (0x00000008)

/* sdiConnectInfo.mFlag */
#define SDI_CONNECT_TRANSACTION_END_MASK         (0x00000010)
#define SDI_CONNECT_TRANSACTION_END_FALSE        (0x00000000)
#define SDI_CONNECT_TRANSACTION_END_TRUE         (0x00000010)

/* sdiConnectInfo.mFlag */
#define SDI_CONNECT_MESSAGE_FIRST_MASK           (0x00000020)
#define SDI_CONNECT_MESSAGE_FIRST_FALSE          (0x00000000)
#define SDI_CONNECT_MESSAGE_FIRST_TRUE           (0x00000020)

/* sdiConnectInfo.mFlag */
#define SDI_CONNECT_USER_AUTOCOMMIT_MODE_MASK    (0x00000040)
#define SDI_CONNECT_USER_AUTOCOMMIT_MODE_ON      (0x00000000)
#define SDI_CONNECT_USER_AUTOCOMMIT_MODE_OFF     (0x00000040)

/* sdiConnectInfo.mFlag */
#define SDI_CONNECT_COORD_AUTOCOMMIT_MODE_MASK    (0x00000080)
#define SDI_CONNECT_COORD_AUTOCOMMIT_MODE_ON      (0x00000000)
#define SDI_CONNECT_COORD_AUTOCOMMIT_MODE_OFF     (0x00000080)

typedef IDE_RC (*sdiMessageCallback)(SChar *aMessage, UInt aLength, void *aArgument);

typedef struct sdiMessageCallbackStruct
{
    sdiMessageCallback   mFunction;
    void               * mArgument;
} sdiMessageCallbackStruct;

typedef enum
{
    SDI_AFFECTED_ROW_INITIAL = -1,
    SDI_AFFECTED_ROW_SETTED  = 0,
} sdiAffectedRow;

typedef struct sdiConnectInfo
{
    // 접속정보
    qcSession      * mSession;
    void           * mDkiSession;
    sdiNode          mNodeInfo;
    UShort           mConnectType;
    sdiShardPin      mShardPin;
    sdiShardClient   mIsShardClient ;
    ULong            mShardMetaNumber;
    SChar            mUserName[QCI_MAX_OBJECT_NAME_LEN + 1];
    SChar            mUserPassword[IDS_MAX_PASSWORD_LEN + 1];

    // 접속결과
    void           * mDbc;           // client connection
    idBool           mLinkFailure;   // client connection state
    UInt             mTouchCount;
    UInt             mNodeId;
    SChar            mNodeName[SDI_NODE_NAME_MAX_SIZE + 1];
    SChar            mServerIP[SDI_SERVER_IP_SIZE];  // 실제 접속한 data node ip
    UShort           mPortNo;                        // 실제 접속한 data node port
    SChar            mFullAddress[SDI_FULL_SERVER_ADDRESS_SIZE + 1]; // 실제 접속한 data node ip + port

    // 런타임정보
    sdiMessageCallbackStruct  mMessageCallback;
    UInt             mFlag;
    UChar            mPlanAttr;
    UChar            mReadOnly;
    void           * mRemoteTx;
    ID_XID           mXID;           // for 2PC
    sdiFailOverTarget   mFailoverTarget;
    idBool              mIsConnectSuccess[SDI_FAILOVER_MAX];
    vSLong              mAffectedRowCount;
} sdiConnectInfo;

typedef struct sdiClientInfo
{
    idBool           mNeedToDisconnect;  /* BUG-46100 Session SMN Update */
    UShort           mCount;             // client count
    sdiConnectInfo   mConnectInfo[1];    // client connection info
} sdiClientInfo;

typedef enum
{
    SDI_LINKER_NONE        = 0,
    SDI_LINKER_COORDINATOR = 1
} sdiLinkerType;

typedef struct sdiRangeIndex
{
    UShort mRangeIndex;
    UShort mValueIndex;

} sdiRangeIndex;

/* PROJ-2701 Online data rebuild */
typedef enum
{
    SDI_REBUILD_RANGE_NONE    = 0,
    SDI_REBUILD_RANGE_INCLUDE = 1,
    SDI_REBUILD_RANGE_EXCLUDE = 2

} sdiRebuildRangeType;

/* PROJ-2701 Online data rebuild */
typedef struct sdiRebuildInfo
{
    sdiNodeInfo    * mNodeInfo;
    UShort           mMyNodeId;
    UShort           mSessionSMN;
    sdiAnalyzeInfo * mSessionSMNAnalysis;
    UShort           mDataSMN;
    sdiAnalyzeInfo * mDataSMNAnalysis;
    sdiValue       * mValue;

} sdiRebuildInfo;

/* PROJ-2701 Online data rebuild */
typedef struct sdiRebuildRangeList
{
    sdiValue            mValue;
    UShort              mFromNode;
    UShort              mToNode;
    sdiRebuildRangeType mType;
    idBool              mIsDefault;

    struct sdiRebuildRangeList * mNext;

} sdiRebuildRangeList;

class sdi
{
public:

    /*************************************************************************
     * 모듈 초기화 함수
     *************************************************************************/
    static void initialize();

    static void finalize();

    static IDE_RC addExtMT_Module( void );

    static IDE_RC initSystemTables( void );

    /*************************************************************************
     * shard query 분석
     *************************************************************************/

    static IDE_RC checkStmt( qcStatement * aStatement );

    static IDE_RC analyze( qcStatement * aStatement,
                           ULong         aSMN );

    static IDE_RC setAnalysisResult( qcStatement * aStatement );

    static IDE_RC setAnalysisResultForInsert( qcStatement    * aStatement,
                                              sdiAnalyzeInfo * aAnalyzeInfo,
                                              sdiObjectInfo  * aShardObjInfo );

    static IDE_RC setAnalysisResultForTable( qcStatement    * aStatement,
                                             sdiAnalyzeInfo * aAnalyzeInfo,
                                             sdiObjectInfo  * aShardObjInfo );

    static IDE_RC copyAnalyzeInfo( qcStatement    * aStatement,
                                   sdiAnalyzeInfo * aAnalyzeInfo,
                                   sdiAnalyzeInfo * aSrcAnalyzeInfo );

    static sdiAnalyzeInfo *  getAnalysisResultForAllNodes();

    static IDE_RC getExternalNodeInfo( sdiNodeInfo * aNodeInfo,
                                       ULong         aSMN );

    static IDE_RC getInternalNodeInfo( smiTrans    * aTrans,
                                       sdiNodeInfo * aNodeInfo,
                                       idBool        aIsShardMetaChanged,
                                       ULong         aSMN );

    /* PROJ-2655 Composite shard key */
    static IDE_RC getRangeIndexByValue( qcTemplate     * aTemplate,
                                        mtcTuple       * aShardKeyTuple,
                                        sdiAnalyzeInfo * aShardAnalysis,
                                        UShort           aValueIndex,
                                        sdiValueInfo   * aValue,
                                        sdiRangeIndex  * aRangeIndex,
                                        UShort         * aRangeIndexCount,
                                        idBool         * aHasDefaultNode,
                                        idBool           aIsSubKey );

    static IDE_RC checkValuesSame( qcTemplate   * aTemplate,
                                   mtcTuple     * aShardKeyTuple,
                                   UInt           aKeyDataType,
                                   sdiValueInfo * aValue1,
                                   sdiValueInfo * aValue2,
                                   idBool       * aIsSame );

    static IDE_RC validateNodeNames( qcStatement  * aStatement,
                                     qcShardNodes * aNodeNames );

    static IDE_RC allocAndCopyRanges( qcStatement  * aStatement,
                                      sdiRangeInfo * aTo,
                                      sdiRangeInfo * aFrom );

    static IDE_RC allocAndCopyValues( qcStatement   * aStatement,
                                      sdiValueInfo ** aTo,
                                      UShort        * aToCount,
                                      sdiValueInfo  * aFrom,
                                      UShort          aFromCount );

    /*************************************************************************
     * utility
     *************************************************************************/

    static IDE_RC checkShardLinker( qcStatement * aStatement );

    static sdiConnectInfo * findConnect( sdiClientInfo * aClientInfo,
                                         UInt            aNodeId );

    static idBool findBindParameter( sdiAnalyzeInfo * aAnalyzeInfo );

    static idBool findRangeInfo( sdiRangeInfo * aRangeInfo,
                                 UInt           aNodeId );

    static IDE_RC getProcedureInfo( qcStatement      * aStatement,
                                    UInt               aUserID,
                                    qcNamePosition     aUserName,
                                    qcNamePosition     aProcName,
                                    qsProcParseTree  * aProcPlanTree,
                                    sdiObjectInfo   ** aShardObjInfo );

    static IDE_RC getTableInfo( qcStatement    * aStatement,
                                qcmTableInfo   * aTableInfo,
                                sdiObjectInfo ** aShardObjInfo );

    static void   charXOR( SChar * aText, UInt aLen );

    static IDE_RC printMessage( SChar * aMessage,
                                UInt    aLength,
                                void  * aArgument );

    static void   setShardMetaTouched( qcSession * aSession );
    static void   unsetShardMetaTouched( qcSession * aSession );

    static IDE_RC touchShardNode( qcSession * aSession,
                                  idvSQL    * aStatistics,
                                  smTID       aTransID,
                                  UInt        aNodeId );

    static IDE_RC openAllShardConnections( qcSession * aSession );

    static idBool getNeedToDisconnect( qcSession * aSession );

    static void   setNeedToDisconnect( sdiClientInfo * aClientInfo,
                                       idBool          aNeedToDisconnect );

    /*************************************************************************
     * shard session
     *************************************************************************/

    /* PROJ-2638 */
    static void initOdbcLibrary();
    static void finiOdbcLibrary();

    static IDE_RC initializeSession( qcSession * aSession,
                                     void      * aDkiSession,
                                     smiTrans  * aTrans,
                                     idBool      aIsShardMetaChanged,
                                     ULong       aConnectSMN );

    static void finalizeSession( qcSession * aSession );

    static IDE_RC allocConnect( sdiConnectInfo * aConnectInfo );

    static void freeConnectImmediately( sdiConnectInfo * aConnectInfo );

    static IDE_RC checkNode( sdiConnectInfo * aConnectInfo );

    static idBool isShardEnable();
    static idBool isShardCoordinator( qcStatement * aStatement );

    static idBool checkNeedFailover( sdiConnectInfo * aConnectInfo );

    // BUG-45411
    static IDE_RC endPendingTran( sdiConnectInfo * aConnectInfo,
                                  idBool           aIsCommit );

    static IDE_RC commit( sdiConnectInfo * aConnectInfo );

    static IDE_RC rollback( sdiConnectInfo * aConnectInfo,
                            const SChar    * aSavePoint );

    static IDE_RC savepoint( sdiConnectInfo * aConnectInfo,
                             const SChar    * aSavePoint );

    static void xidInitialize( sdiConnectInfo * aConnectInfo );

    static IDE_RC addPrepareTranCallback( void           ** aCallback,
                                          sdiConnectInfo  * aNode );

    static IDE_RC addEndTranCallback( void           ** aCallback,
                                      sdiConnectInfo  * aNode,
                                      idBool            aIsCommit );

    static void doCallback( void * aCallback );

    static IDE_RC resultCallback( void           * aCallback,
                                  sdiConnectInfo * aNode,
                                  SChar          * aFuncName,
                                  idBool           aReCall );

    static void removeCallback( void * aCallback );

    static void shardStmtPartialRollbackUsingSavepoint( qcStatement    * aStatement,
                                                        sdiClientInfo  * aClientInfo,
                                                        sdiDataNodes   * aDataInfo );

    /*************************************************************************
     * etc
     *************************************************************************/

    static void setExplainPlanAttr( qcSession * aSession,
                                    UChar       aValue );

    static IDE_RC shardExecDirect( qcStatement * aStatement,
                                   SChar     * aNodeName,
                                   SChar     * aQuery,
                                   UInt        aQueryLen,
                                   UInt      * aExecCount );

    static IDE_RC setCommitMode( qcSession * aSession,
                                 idBool      aIsAutoCommit,
                                 UInt        aDBLinkGTXLevel );

    static idBool isUserAutoCommit( sdiConnectInfo * aConnectInfo );
    static idBool isMetaAutoCommit( sdiConnectInfo * aConnectInfo );

    static void initAffectedRow( sdiConnectInfo   * aConnectInfo );
    static idBool isAffectedRowSetted( sdiConnectInfo * aConnectInfo );

    /* BUG-45967 Rebuild Data 완료 대기 */
    static IDE_RC waitToRebuildData( idvSQL * aStatistics );

    /*************************************************************************
     * shard statement
     *************************************************************************/

    static void initDataInfo( qcShardExecData * aExecData );

    static IDE_RC allocDataInfo( qcShardExecData * aExecData,
                                 iduVarMemList   * aMemory );

    static void setDataNodePrepared( sdiClientInfo * aClientInfo,
                                     sdiDataNodes  * aDataNode );

    static void closeDataInfo( qcStatement     * aStatement,
                               qcShardExecData * aExecData );

    static void clearDataInfo( qcStatement     * aStatement,
                               qcShardExecData * aExecData );

    // 수행정보 초기화
    static IDE_RC initShardDataInfo( qcTemplate     * aTemplate,
                                     sdiAnalyzeInfo * aShardAnalysis,
                                     sdiClientInfo  * aClientInfo,
                                     sdiDataNodes   * aDataInfo,
                                     sdiDataNode    * aDataArg );

    // 수행정보 재초기화
    static IDE_RC reuseShardDataInfo( qcTemplate     * aTemplate,
                                      sdiClientInfo  * aClientInfo,
                                      sdiDataNodes   * aDataInfo,
                                      sdiBindParam   * aBindParams,
                                      UShort           aBindParamCount,
                                      sdiSVPStep       aSVPStep );

    // 수행노드 결정
    static IDE_RC decideShardDataInfo( qcTemplate     * aTemplate,
                                       mtcTuple       * aShardKeyTuple,
                                       sdiAnalyzeInfo * aShardAnalysis,
                                       sdiClientInfo  * aClientInfo,
                                       sdiDataNodes   * aDataInfo,
                                       qcNamePosition * aShardQuery );

    static IDE_RC getExecNodeRangeIndex( qcTemplate        * aTemplate,
                                         mtcTuple          * aShardKeyTuple,
                                         mtcTuple          * aShardSubKeyTuple,
                                         sdiAnalyzeInfo    * aShardAnalysis,
                                         UShort            * aRangeIndex,
                                         UShort            * aRangeIndexCount,
                                         idBool            * aExecDefaultNode,
                                         idBool            * aExecAllNodes );

    static IDE_RC setPrepareSelected( sdiClientInfo    * aClientInfo,
                                      sdiDataNodes     * aDataInfo,
                                      idBool             aAllNodes,
                                      UInt               aNodeId );

    static IDE_RC prepare( sdiClientInfo    * aClientInfo,
                           sdiDataNodes     * aDataInfo,
                           qcNamePosition   * aShardQuery );

    static IDE_RC executeDML( qcStatement    * aStatement,
                              sdiClientInfo  * aClientInfo,
                              sdiDataNodes   * aDataInfo,
                              vSLong         * aNumRows );

    static IDE_RC executeInsert( qcStatement    * aStatement,
                                 sdiClientInfo  * aClientInfo,
                                 sdiDataNodes   * aDataInfo,
                                 vSLong         * aNumRows );

    static IDE_RC executeSelect( qcStatement    * aStatement,
                                 sdiClientInfo  * aClientInfo,
                                 sdiDataNodes   * aDataInfo );

    static IDE_RC fetch( sdiConnectInfo  * aConnectInfo,
                         sdiDataNode     * aDataNode,
                         idBool          * aExist );

    static IDE_RC getPlan( sdiConnectInfo  * aConnectInfo,
                           sdiDataNode     * aDataNode );

    /*************************************************************************
     * shard meta info
     *************************************************************************/
    static UInt getShardUserID();

    static void setShardUserID( UInt aShardUserID );

    static IDE_RC loadMetaNodeInfo();

    static ULong  getSMNForMetaNode();

    static void   setSMNCacheForMetaNode( ULong aNewSMN );
    
    static IDE_RC getIncreasedSMNForMetaNode( smiTrans * aTrans,
                                              ULong    * aNewSMN );

    static IDE_RC reloadSMNForDataNode( smiStatement * aSmiStmt );

    /*************************************************************************
     * data node info
     *************************************************************************/

    static ULong  getSMNForDataNode();

    static void   setSMNForDataNode( ULong aNewSMN );

    /*************************************************************************
     * shard pin
     *************************************************************************/
    static idBool checkMetaCreated();

    static sdiShardPin makeShardPin();

    static void shardPinToString( SChar *aDst, UInt aLen, sdiShardPin aShardPIn );

    /*************************************************************************
     * ODBC Connect Attribute
     *************************************************************************/
    static UInt getShardInternalConnAttrRetryCount();
    static UInt getShardInternalConnAttrRetryDelay();
    static UInt getShardInternalConnAttrConnTimeout();
    static UInt getShardInternalConnAttrLoginTimeout();

    /*************************************************************************
     * BUG-45899 Print analysis information
     *************************************************************************/
    static idBool isAnalysisInfoPrintable( qcStatement * aStatement );

    static void printAnalysisInfo( qcStatement  * aStatement,
                                   iduVarString * aString );

    static sdiQueryType getQueryType( qciStatement * aStatement );

    static void setNonShardQueryReason( sdiPrintInfo * aPrintInfo,
                                        UShort         aReason );

    static void setPrintInfoFromAnalyzeInfo( sdiPrintInfo   * aPrintInfo,
                                             sdiAnalyzeInfo * aAnalyzeInfo );

    static void setPrintInfoFromPrintInfo( sdiPrintInfo * aDst,
                                           sdiPrintInfo * aSrc );

    static SChar * getCanNotMergeReasonArr( UShort aArrIdx );

    /*************************************************************************
     * shard client fail-over align
     *************************************************************************/
    static void closeShardSessionByNodeId( qcSession * aSession,
                                           UInt        aNodeId,
                                           UChar       aDestination );

    static void setTransactionBroken( idBool     aIsUserAutoCommit,
                                      void     * aDkiSession,
                                      smiTrans * aTrans );

    static idBool isSupportDataType( UInt aModuleID );

    /*************************************************************************
     * online data rebuild
     *************************************************************************/
    static idBool isRebuildCoordinator( qcStatement * aStatement );

    static IDE_RC waitAndSetSMNForMetaNode( idvSQL       * aStatistics,
                                            smiStatement * aSmiStmt,
                                            UInt           aFlag,
                                            ULong          aNeededSMN,
                                            ULong        * aDataSMN );

    static IDE_RC convertRangeValue( SChar       * aValue,
                                     UInt          aLength,
                                     UInt          aKeyType,
                                     sdiValue    * aRangeValue );

    static IDE_RC compareKeyData( UInt       aKeyDataType,
                                  sdiValue * aValue1,
                                  sdiValue * aValue2,
                                  SShort   * aResult );

    static idBool hasShardCoordPlan( qcStatement * aStatement );

    static void isShardSelectExists( qmnPlan * aPlan,
                                     idBool  * aIsSDSEExists  );

    static void getShardObjInfoForSMN( ULong            aSMN,
                                       sdiObjectInfo  * aShardObjectList,
                                       sdiObjectInfo ** aRet );

    static IDE_RC unionNodeInfo( sdiNodeInfo * aSourceNodeInfo,
                                 sdiNodeInfo * aTargetNodeInfo );

};

inline idBool sdi::isUserAutoCommit( sdiConnectInfo * aConnectInfo )
{
    idBool sIsAutoCommit;

    sIsAutoCommit = ( ( aConnectInfo->mFlag & SDI_CONNECT_USER_AUTOCOMMIT_MODE_MASK )
                      == SDI_CONNECT_USER_AUTOCOMMIT_MODE_ON )
                    ? ID_TRUE
                    : ID_FALSE;

    return sIsAutoCommit;
}

inline idBool sdi::isMetaAutoCommit( sdiConnectInfo * aConnectInfo )
{
    idBool sIsAutoCommit;

    sIsAutoCommit = ( ( aConnectInfo->mFlag & SDI_CONNECT_COORD_AUTOCOMMIT_MODE_MASK )
                      == SDI_CONNECT_COORD_AUTOCOMMIT_MODE_ON )
                    ? ID_TRUE
                    : ID_FALSE;

    return sIsAutoCommit;
}

inline void sdi::initAffectedRow( sdiConnectInfo    * aConnectInfo )
{
    aConnectInfo->mAffectedRowCount = SDI_AFFECTED_ROW_INITIAL;
}

inline idBool sdi::isAffectedRowSetted( sdiConnectInfo    * aConnectInfo )
{
    idBool  sIsAffectedRowSetted = ID_TRUE;

    if ( aConnectInfo->mAffectedRowCount >= SDI_AFFECTED_ROW_SETTED )
    {
        sIsAffectedRowSetted = ID_TRUE;
    }
    else
    {
        sIsAffectedRowSetted = ID_FALSE;
    }

    return sIsAffectedRowSetted;
}

#endif /* _O_SDI_H_ */
