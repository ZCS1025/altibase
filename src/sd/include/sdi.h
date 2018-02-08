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
#include <sde.h>
#include <qci.h>
#include <qcuProperty.h>
#include <smi.h>

#define SDI_HASH_MAX_VALUE                     (1000)
#define SDI_HASH_MAX_VALUE_FOR_TEST            (100)

// range, list sharding�� varchar shard key column�� max precision
#define SDI_RANGE_VARCHAR_MAX_PRECISION        (100)
#define SDI_RANGE_VARCHAR_MAX_PRECISION_STR    "100"
#define SDI_RANGE_VARCHAR_MAX_SIZE             (MTD_CHAR_TYPE_STRUCT_SIZE(SDI_RANGE_VARCHAR_MAX_PRECISION))

#define SDI_NODE_MAX_COUNT                     (128)
#define SDI_RANGE_MAX_COUNT                    (1000)
#define SDI_VALUE_MAX_COUNT                    (1000)

#define SDI_SERVER_IP_SIZE                     (16)
#define SDI_NODE_NAME_MAX_SIZE                 (40)

/* PROJ-2661 */
#define SDI_XA_RECOVER_RMID                    (0)
#define SDI_XA_RECOVER_COUNT                   (5)

typedef enum
{
    SDI_XA_RECOVER_START = 0,
    SDI_XA_RECOVER_CONT  = 1,
    SDI_XA_RECOVER_END   = 2
} sdiXARecoverOption;

typedef enum
{
    /*
     * PROJ-2646 shard analyzer enhancement
     * boolean array�� shard CAN-MERGE false�� ������ üũ�Ѵ�.
     */

    SDI_MULTI_NODES_JOIN_EXISTS       =  0, // ��� �� JOIN�� �ʿ���
    SDI_MULTI_SHARD_INFO_EXISTS       =  1, // �л� ���ǰ� �ٸ� SHARD TABLE���� ��� ��
    SDI_HIERARCHY_EXISTS              =  2, // CONNECT BY�� ��� ��
    SDI_DISTINCT_EXISTS               =  3, // ��� �� ������ �ʿ��� DISTINCT�� ��� ��
    SDI_GROUP_AGGREGATION_EXISTS      =  4, // ��� �� ������ �ʿ��� GROUP BY�� ��� ��
    SDI_SHARD_SUBQUERY_EXISTS         =  5, // SHARD SUBQUERY�� ��� ��
    SDI_ORDER_BY_EXISTS               =  6, // ��� �� ������ �ʿ��� ORDER BY�� ��� ��
    SDI_LIMIT_EXISTS                  =  7, // ��� �� ������ �ʿ��� LIMIT�� ��� ��
    SDI_MULTI_NODES_SET_OP_EXISTS     =  8, // ��� �� ������ �ʿ��� SET operator�� ��� ��
    SDI_NO_SHARD_TABLE_EXISTS         =  9, // SHARD TABLE�� �ϳ��� ����
    SDI_NON_SHARD_TABLE_EXISTS        = 10, // SHARD META�� ��ϵ��� ���� TABLE�� ��� ��
    SDI_LOOP_EXISTS                   = 11, // ��� �� ������ �ʿ��� LOOP�� ��� ��
    SDI_INVALID_OUTER_JOIN_EXISTS     = 12, // CLONE table�� left-side����, HASH,RANGE,LIST table�� right-side�� ���� outer join�� �����Ѵ�.
    SDI_INVALID_SEMI_ANTI_JOIN_EXISTS = 13, // HASH,RANGE,LIST table�� inner(subquery table)�� ���� semi/anti-join�� �����Ѵ�.
    SDI_NESTED_AGGREGATION_EXISTS     = 14, // ��� �� ������ �ʿ��� GROUP BY�� ��� ��
    SDI_GROUP_BY_EXTENSION_EXISTS     = 15, // Group by extension(ROLLUP,CUBE,GROUPING SETS)�� �����Ѵ�.
    SDI_SUB_KEY_EXISTS                = 16  // Sub-shard key�� ���� table�� ���� ��
                                             // (Can't merge reason�� �ƴ�����, query block �� ������ �ʿ��� �־�д�.)
                                             // SDI_SUB_KEY_EXISTS�� �������� �;��Ѵ�.

    /*
     * �Ʒ� ������ shard analyze �� �߰� ��� ������ �߻� ��Ų��.
     *
     * + Pivot �Ǵ� Unpivot�� ����
     * + Recursive with�� ����
     * + Lateral view�� ����
     *
     * �Ʒ� ������ ���� ó������ �ʴ´�.
     *
     * + User-defined function�� ����( local function���� ���� )
     * + Nested aggregation�� ����
     * + Analytic function�� ����
     */

} sdiCanMergeReason;

#define SDI_SHARD_CAN_MERGE_REASON_ARRAY       (17)

#define SDI_SET_INIT_CAN_MERGE_REASON(_dst_)              \
{                                                         \
    _dst_[SDI_MULTI_NODES_JOIN_EXISTS]       = ID_FALSE;  \
    _dst_[SDI_MULTI_SHARD_INFO_EXISTS]       = ID_FALSE;  \
    _dst_[SDI_HIERARCHY_EXISTS]              = ID_FALSE;  \
    _dst_[SDI_DISTINCT_EXISTS]               = ID_FALSE;  \
    _dst_[SDI_GROUP_AGGREGATION_EXISTS]      = ID_FALSE;  \
    _dst_[SDI_SHARD_SUBQUERY_EXISTS]         = ID_FALSE;  \
    _dst_[SDI_ORDER_BY_EXISTS]               = ID_FALSE;  \
    _dst_[SDI_LIMIT_EXISTS]                  = ID_FALSE;  \
    _dst_[SDI_MULTI_NODES_SET_OP_EXISTS]     = ID_FALSE;  \
    _dst_[SDI_NO_SHARD_TABLE_EXISTS]         = ID_FALSE;  \
    _dst_[SDI_NON_SHARD_TABLE_EXISTS]        = ID_FALSE;  \
    _dst_[SDI_LOOP_EXISTS]                   = ID_FALSE;  \
    _dst_[SDI_SUB_KEY_EXISTS]                = ID_FALSE;  \
    _dst_[SDI_INVALID_OUTER_JOIN_EXISTS]     = ID_FALSE;  \
    _dst_[SDI_INVALID_SEMI_ANTI_JOIN_EXISTS] = ID_FALSE;  \
    _dst_[SDI_NESTED_AGGREGATION_EXISTS]     = ID_FALSE;  \
    _dst_[SDI_GROUP_BY_EXTENSION_EXISTS]     = ID_FALSE;  \
}

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

typedef struct sdiNode
{
    UInt            mNodeId;
    SChar           mNodeName[SDI_NODE_NAME_MAX_SIZE + 1];
    SChar           mServerIP[SDI_SERVER_IP_SIZE];
    UShort          mPortNo;
    SChar           mAlternateServerIP[SDI_SERVER_IP_SIZE];
    UShort          mAlternatePortNo;
} sdiNode;

typedef struct sdiNodeInfo
{
    UShort          mCount;
    sdiNode         mNodes[SDI_NODE_MAX_COUNT];
} sdiNodeInfo;

typedef union sdiValue
{
    // hash shard�� ���
    UInt        mHashMax;

    // range shard�� ���
    UChar       mMax[1];      // ��ǥ��
    SShort      mSmallintMax; // shard key�� smallint type�� ���
    SInt        mIntegerMax;  // shard key�� integer type�� ���
    SLong       mBigintMax;   // shard key�� bigint type�� ���
    mtdCharType mCharMax;     // shard key�� char/varchar type�� ���
    UShort      mCharMaxBuf[(SDI_RANGE_VARCHAR_MAX_SIZE + 1) / 2];  // 2byte align

    // bind parameter�� ���
    UShort      mBindParamId;

} sdiValue;

typedef struct sdiRange
{
    UShort   mNodeId;
    sdiValue mValue;
    sdiValue mSubValue;
} sdiRange;

typedef struct sdiRangeInfo
{
    UShort          mCount;
    sdiRange        mRanges[SDI_RANGE_MAX_COUNT];
} sdiRangeInfo;

typedef struct sdiTableInfo
{
    UInt            mShardID;
    SChar           mUserName[QC_MAX_OBJECT_NAME_LEN + 1];
    SChar           mObjectName[QC_MAX_OBJECT_NAME_LEN + 1];
    SChar           mObjectType;
    SChar           mKeyColumnName[QC_MAX_OBJECT_NAME_LEN + 1];
    UInt            mKeyDataType;   // shard key�� mt type id
    UShort          mKeyColOrder;
    sdiSplitMethod  mSplitMethod;

    /* PROJ-2655 Composite shard key */
    idBool          mSubKeyExists;
    SChar           mSubKeyColumnName[QC_MAX_OBJECT_NAME_LEN + 1];
    UInt            mSubKeyDataType;   // shard key�� mt type id
    UShort          mSubKeyColOrder;
    sdiSplitMethod  mSubSplitMethod;

    UShort          mDefaultNodeId; // default node id
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
    (info)->mDefaultNodeId       = ID_USHORT_MAX;   \
}

typedef struct sdiTableInfoList
{
    sdiTableInfo            * mTableInfo;
    struct sdiTableInfoList * mNext;
} sdiTableInfoList;

typedef struct sdiObjectInfo
{
    sdiTableInfo    mTableInfo;
    sdiRangeInfo    mRangeInfo;

    // view�� ��� key�� �������� �� �־� �÷� ������ŭ �Ҵ��Ͽ� ����Ѵ�.
    UChar           mKeyFlags[1];
} sdiObjectInfo;

typedef struct sdiValueInfo
{
    UChar    mType;             // 0 : hostVariable, 1 : constant
    sdiValue mValue;

} sdiValueInfo;

typedef struct sdiAnalyzeInfo
{
    UShort             mValueCount;
    sdiValueInfo       mValue[SDI_VALUE_MAX_COUNT];
    UChar              mIsCanMerge;
    UChar              mIsTransformAble; // aggr transformation ���࿩��
    sdiSplitMethod     mSplitMethod;  // shard key�� split method
    UInt               mKeyDataType;  // shard key�� mt type id

    /* PROJ-2655 Composite shard key */
    UChar              mSubKeyExists;
    UShort             mSubValueCount;
    sdiValueInfo       mSubValue[SDI_VALUE_MAX_COUNT];
    sdiSplitMethod     mSubSplitMethod;  // sub sub key�� split method
    UInt               mSubKeyDataType;  // sub shard key�� mt type id

    UShort             mDefaultNodeId;// shard query�� default node id
    sdiRangeInfo       mRangeInfo;    // shard query�� �л�����

    // BUG-45359
    qcShardNodes     * mNodeNames;

    // PROJ-2685 online rebuild
    sdiTableInfoList * mTableInfoList;
} sdiAnalyzeInfo;

#define SDI_INIT_ANALYZE_INFO( info )           \
{                                               \
    (info)->mValueCount       = 0;              \
    (info)->mIsCanMerge       = '\0';           \
    (info)->mSplitMethod      = SDI_SPLIT_NONE; \
    (info)->mKeyDataType      = ID_UINT_MAX;    \
    (info)->mSubKeyExists     = '\0';           \
    (info)->mSubValueCount    = 0;              \
    (info)->mSubSplitMethod   = SDI_SPLIT_NONE; \
    (info)->mSubKeyDataType   = ID_UINT_MAX;    \
    (info)->mDefaultNodeId    = ID_USHORT_MAX;  \
    (info)->mRangeInfo.mCount = 0;              \
    (info)->mNodeNames        = NULL;           \
    (info)->mTableInfoList    = NULL;           \
}

// PROJ-2646
typedef struct sdiShardInfo
{
    UInt                  mKeyDataType;
    UShort                mDefaultNodeId;
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

    UInt                  mValueCount;
    sdiValueInfo        * mValue;

    sdiShardInfo          mShardInfo;

    idBool                mIsJoined;

    sdiKeyInfo          * mLeft;
    sdiKeyInfo          * mRight;
    sdiKeyInfo          * mOrgKeyInfo;

    sdiKeyInfo          * mNext;

} sdiKeyInfo;

typedef struct sdiParseTree
{
    sdiQuerySet * mQuerySetAnalysis;

    idBool        mIsCanMerge;
    idBool        mCantMergeReason[SDI_SHARD_CAN_MERGE_REASON_ARRAY];

    /* PROJ-2655 Composite shard key */
    idBool        mIsCanMerge4SubKey;
    idBool        mCantMergeReason4SubKey[SDI_SHARD_CAN_MERGE_REASON_ARRAY];

} sdiParseTree;

typedef struct sdiQuerySet
{
    idBool                mIsCanMerge;
    idBool                mCantMergeReason[SDI_SHARD_CAN_MERGE_REASON_ARRAY];
    sdiShardInfo          mShardInfo;
    sdiKeyInfo          * mKeyInfo;

    /* PROJ-2655 Composite shard key */
    idBool                mIsCanMerge4SubKey;
    idBool                mCantMergeReason4SubKey[SDI_SHARD_CAN_MERGE_REASON_ARRAY];
    sdiShardInfo          mShardInfo4SubKey;
    sdiKeyInfo          * mKeyInfo4SubKey;

    /* PROJ-2685 online rebuild */
    sdiTableInfoList    * mTableInfoList;

} sdiQuerySet;

typedef struct sdiBindParam
{
    UShort       mId;
    UInt         mInoutType;
    UInt         mType;
    void       * mData;
    UInt         mDataSize;
    SInt         mPrecision;
    SInt         mScale;
} sdiBindParam;

// PROJ-2638
typedef struct sdiDataNode
{
    void         * mStmt;                // data node stmt

    void         * mBuffer[SDI_NODE_MAX_COUNT];  // data node fetch buffer
    UInt         * mOffset;              // meta node column offset array
    UInt         * mMaxByteSize;         // meta node column max byte size array
    UInt           mBufferLength;        // data node fetch buffer length
    UShort         mColumnCount;         // data node column count

    sdiBindParam * mBindParams;          // data node parameters
    UShort         mBindParamCount;
    idBool         mBindParamChanged;

    SChar        * mPlanText;            // data node plan text (alloc&free�� cli library�����Ѵ�.)
    UInt           mExecCount;           // data node execution count

    UChar          mState;               // date node state
} sdiDataNode;

#define SDI_NODE_STATE_NONE               0    // �ʱ����
#define SDI_NODE_STATE_PREPARE_CANDIDATED 1    // prepare �ĺ���尡 ������ ����
#define SDI_NODE_STATE_PREPARE_SELECTED   2    // prepare �ĺ���忡�� prepare ���� ���õ� ����
#define SDI_NODE_STATE_PREPARED           3    // prepared ����
#define SDI_NODE_STATE_EXECUTE_CANDIDATED 4    // execute �ĺ���尡 ������ ����
#define SDI_NODE_STATE_EXECUTE_SELECTED   5    // execute �ĺ���忡�� execute ���� ���õ� ����
#define SDI_NODE_STATE_EXECUTED           6    // executed ����
#define SDI_NODE_STATE_FETCHED            7    // fetch �Ϸ� ���� (only SELECT)

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
#define SDI_CONNECT_COORDINATOR_CREATE_MASK      (0x00000002)
#define SDI_CONNECT_COORDINATOR_CREATE_FALSE     (0x00000000)
#define SDI_CONNECT_COORDINATOR_CREATE_TRUE      (0x00000002)

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

typedef IDE_RC (*sdiMessageCallback)(SChar *aMessage, UInt aLength, void *aArgument);

typedef struct sdiMessageCallbackStruct
{
    sdiMessageCallback   mFunction;
    void               * mArgument;
} sdiMessageCallbackStruct;

typedef struct sdiConnectInfo
{
    // ��������
    qcSession * mSession;
    void      * mDkiSession;
    sdiNode     mNodeInfo;
    UShort      mConnectType;
    ULong       mShardPin;
    SChar       mUserName[QCI_MAX_OBJECT_NAME_LEN + 1];
    SChar       mUserPassword[IDS_MAX_PASSWORD_LEN + 1];

    // ���Ӱ��
    void      * mDbc;           // client connection
    idBool      mLinkFailure;   // client connection state
    UInt        mTouchCount;
    UInt        mNodeId;
    SChar       mNodeName[SDI_NODE_NAME_MAX_SIZE + 1];
    SChar       mServerIP[SDI_SERVER_IP_SIZE];  // ���� ������ data node ip
    UShort      mPortNo;                        // ���� ������ data node port

    // ��Ÿ������
    sdiMessageCallbackStruct  mMessageCallback;
    UInt        mFlag;
    UChar       mPlanAttr;
    UChar       mReadOnly;
    void      * mGlobalCoordinator;
    void      * mRemoteTx;
    ID_XID      mXID;           // for 2PC
} sdiConnectInfo;

typedef struct sdiClientInfo
{
    UInt             mMetaSessionID;     // meta session ID
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

class sdi
{
public:

    /*************************************************************************
     * ��� �ʱ�ȭ �Լ�
     *************************************************************************/

    static IDE_RC addExtMT_Module( void );

    static IDE_RC initSystemTables( void );

    /*************************************************************************
     * shard query �м�
     *************************************************************************/

    static IDE_RC checkStmt( qcStatement * aStatement );

    static IDE_RC analyze( qcStatement * aStatement );

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

    static void   getNodeInfo( sdiNodeInfo * aNodeInfo );

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

    /*************************************************************************
     * utility
     *************************************************************************/

    static IDE_RC checkShardLinker( qcStatement * aStatement );

    static sdiConnectInfo * findConnect( sdiClientInfo * aClientInfo,
                                         UShort          aNodeId );

    static idBool findBindParameter( sdiAnalyzeInfo * aAnalyzeInfo );

    static idBool findRangeInfo( sdiRangeInfo * aRangeInfo,
                                 UShort         aNodeId );

    static IDE_RC getProcedureInfo( qcStatement      * aStatement,
                                    UInt               aUserID,
                                    qcNamePosition     aUserName,
                                    qcNamePosition     aProcName,
                                    qsProcParseTree  * aProcPlanTree,
                                    sdiObjectInfo   ** aShardObjInfo );

    static IDE_RC getTableInfo( qcStatement    * aStatement,
                                qcmTableInfo   * aTableInfo,
                                sdiObjectInfo ** aShardObjInfo );

    static IDE_RC getViewInfo( qcStatement    * aStatement,
                               qmsQuerySet    * aQuerySet,
                               sdiObjectInfo ** aShardObjInfo );

    static void   charXOR( SChar * aText, UInt aLen );

    static IDE_RC printMessage( SChar * aMessage,
                                UInt    aLength,
                                void  * aArgument );

    static void   touchShardMeta( qcSession * aSession );

    static IDE_RC touchShardNode( qcSession * aSession,
                                  idvSQL    * aStatistics,
                                  smTID       aTransID,
                                  UInt        aNodeId );

    /*************************************************************************
     * shard session
     *************************************************************************/

    // PROJ-2638
    static void initOdbcLibrary();
    static void finiOdbcLibrary();

    static IDE_RC initializeSession( qcSession  * aSession,
                                     void       * aDkiSession,
                                     UInt         aSessionID,
                                     SChar      * aUserName,
                                     SChar      * aPassword,
                                     ULong        aShardPin );

    static void finalizeSession( qcSession * aSession );

    static IDE_RC allocConnect( sdiConnectInfo * aConnectInfo );

    static void freeConnect( sdiConnectInfo * aConnectInfo );

    static void freeConnectImmediately( sdiConnectInfo * aConnectInfo );

    static IDE_RC checkNode( sdiConnectInfo * aConnectInfo );

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

    /*************************************************************************
     * etc
     *************************************************************************/

    static UInt getShardLinkerChangeNumber();

    static void incShardLinkerChangeNumber();

    static void setExplainPlanAttr( qcSession * aSession,
                                    UChar       aValue );

    static IDE_RC shardExecDirect( qcStatement * aStatement,
                                   SChar     * aNodeName,
                                   SChar     * aQuery,
                                   UInt        aQueryLen,
                                   UInt      * aExecCount );

    /*************************************************************************
     * shard statement
     *************************************************************************/

    static void initDataInfo( qcShardExecData * aExecData );

    static IDE_RC allocDataInfo( qcShardExecData * aExecData,
                                 iduVarMemList   * aMemory );

    static void closeDataNode( sdiClientInfo * aClientInfo,
                               sdiDataNodes  * aDataNode );

    static void closeDataInfo( qcStatement     * aStatement,
                               qcShardExecData * aExecData );

    static void clearDataInfo( qcStatement     * aStatement,
                               qcShardExecData * aExecData );

    // �������� �ʱ�ȭ
    static IDE_RC initShardDataInfo( qcTemplate     * aTemplate,
                                     sdiAnalyzeInfo * aShardAnalysis,
                                     sdiClientInfo  * aClientInfo,
                                     sdiDataNodes   * aDataInfo,
                                     sdiDataNode    * aDataArg );

    // �������� ���ʱ�ȭ
    static IDE_RC reuseShardDataInfo( qcTemplate     * aTemplate,
                                      sdiClientInfo  * aClientInfo,
                                      sdiDataNodes   * aDataInfo,
                                      sdiBindParam   * aBindParams,
                                      UShort           aBindParamCount );

    // ������ ����
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

    static void setPrepareSelected( sdiClientInfo    * aClientInfo,
                                    sdiDataNodes     * aDataInfo,
                                    idBool             aAllNodes,
                                    UShort             aNodeId );

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
};

#endif /* _O_SDI_H_ */
