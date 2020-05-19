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
 * $Id: sduProperty.h 85186 2019-04-09 07:37:00Z jayce.park $
 **********************************************************************/

#ifndef _O_SDU_PROPERTY_H_
#define _O_SDU_PROPERTY_H_ 1

#include <idl.h>
#include <idp.h>
#include <smi.h>

#define SDU_PROPERTY( aProperty ) (sduProperty::mStaticProperty.aProperty)

#define SDU_SHARD_ENABLE                            ( SDU_PROPERTY( mShardEnable ) )
#define SDU_SHARD_TEST_ENABLE                       ( SDU_PROPERTY( mShardTestEnable ) )
#define SDU_SHARD_AGGREGATION_TRANSFORM_DISABLE     ( SDU_PROPERTY( mShardAggrTransformDisable ) )
#define SDU_SHARD_INTERNAL_CONN_ATTR_RETRY_COUNT    ( SDU_PROPERTY( mShardInternalConnAttrRetryCount ) )
#define SDU_SHARD_INTERNAL_CONN_ATTR_RETRY_DELAY    ( SDU_PROPERTY( mShardInternalConnAttrRetryDelay ) )
#define SDU_SHARD_INTERNAL_CONN_ATTR_CONN_TIMEOUT   ( SDU_PROPERTY( mShardInternalConnAttrConnTimeout ) )
#define SDU_SHARD_INTERNAL_CONN_ATTR_LOGIN_TIMEOUT  ( SDU_PROPERTY( mShardInternalConnAttrLoginTimeout ) )
#define SDU_SHARD_REBUILD_DATA_STEP                 ( SDU_PROPERTY( mShardRebuildDataStep ) )
#define SDU_TRCLOG_DETAIL_SHARD                     ( SDU_PROPERTY( mTrclogDetailShard ) )
#define SDU_SHARD_IGNORE_SMN_PROPAGATION_FAILURE    ( SDU_PROPERTY( mShardIgnoreSmnPropagationFailure ) )
#define SDU_SHARD_ALLOW_OLD_SMN                     ( SDU_PROPERTY( mShardAllowOldSmn ) )

/* PROJ-2701 Sharding online data rebuild */
#define SDU_SHARD_META_HISTORY_AUTO_PURGE_DISABLE ( SDU_PROPERTY(mShardMetaHistoryAutoPurgeDisable) )
#define SDU_SHARD_REBUILD_PLAN_DETAIL_FORCE_ENABLE ( SDU_PROPERTY(mShardRebuildPlanDetailForceEnable) )
#define SDU_SHARD_REBUILD_LOCK_TABLE_WITH_DML_ENABLE ( SDU_PROPERTY(mShardRebuildLockTableWithDmlEnable) )
#define SDU_SHARD_META_PROPAGATION_TIMEOUT ( SDU_PROPERTY(mShardMetaPropagationTimeout) )
#define SDU_SHARD_TRANSFORM_STRING_LENGTH_MAX ( SDU_PROPERTY(mShardTransformStringLengthMax) )
#define SDU_SHARD_SMN_CACHE_APPLY_ENABLE ( SDU_PROPERTY(mShardSMNCacheApplyEnable) )

// 참조 : mmuPropertyArgument
typedef struct sduPropertyArgument
{
    idpArgument mArg;
    UInt        mUserID;
    smiTrans   *mTrans;
} sduPropertyArgument;

typedef struct sduProperties
{
    UInt   mShardEnable;
    UInt   mShardTestEnable;
    UInt   mShardAggrTransformDisable;
    UInt   mShardInternalConnAttrRetryCount;
    UInt   mShardInternalConnAttrRetryDelay;
    UInt   mShardInternalConnAttrConnTimeout;
    UInt   mShardInternalConnAttrLoginTimeout;
    SInt   mShardRebuildDataStep;

    /* BUG-45899 */
    UInt   mTrclogDetailShard;

    UInt   mShardIgnoreSmnPropagationFailure;
    UInt   mShardAllowOldSmn;

    /* PROJ-2701 Sharding online data rebuild */
    UInt   mShardMetaHistoryAutoPurgeDisable;
    UInt   mShardRebuildPlanDetailForceEnable;
    UInt   mShardRebuildLockTableWithDmlEnable;
    ULong  mShardMetaPropagationTimeout;
    UInt   mShardTransformStringLengthMax;
    UInt   mShardSMNCacheApplyEnable;

} sduProperties;

class sduProperty
{
public:

    static sduProperties mStaticProperty;

    static IDE_RC initProperty( idvSQL *aStatistics );
    static IDE_RC finalProperty( idvSQL *aStatistics );

    // System 구동 시 관련 Property를 Loading 함.
    static IDE_RC load();

    // System 구동 시 관련 Property의 callback 을 등록함
    static IDE_RC setupUpdateCallback();

    //----------------------------------------------
    // Writable Property를 위한 Call Back 함수들
    //----------------------------------------------

    /* PROJ-2687 Shard aggregation transform */
    static IDE_RC changeSHARD_AGGREGATION_TRANSFORM_DISABLE( idvSQL * /* aStatistics */,
                                                             SChar  * /* aName */,
                                                             void   * /* aOldValue */,
                                                             void   * aNewValue,
                                                             void   * /* aArg */ );

    static IDE_RC changeSHARD_INTERNAL_CONN_ATTR_RETRY_COUNT( idvSQL * /* aStatistics */,
                                                          SChar  * /* aName */,
                                                          void   * /* aOldValue */,
                                                          void   * aNewValue,
                                                          void   * /* aArg */ );

    static IDE_RC changeSHARD_INTERNAL_CONN_ATTR_RETRY_DELAY( idvSQL * /* aStatistics */,
                                                          SChar  * /* aName */,
                                                          void   * /* aOldValue */,
                                                          void   * aNewValue,
                                                          void   * /* aArg */ );

    static IDE_RC changeSHARD_INTERNAL_CONN_ATTR_CONN_TIMEOUT( idvSQL* /* aStatistics */,
                                                           SChar * /* aName */,
                                                           void  * /* aOldValue */,
                                                           void  * aNewValue,
                                                           void  * /* aArg */);

    static IDE_RC changeSHARD_INTERNAL_CONN_ATTR_LOGIN_TIMEOUT( idvSQL* /* aStatistics */,
                                                            SChar * /* aName */,
                                                            void  * /* aOldValue */,
                                                            void  * aNewValue,
                                                            void  * /* aArg */);

    /* BUG-45967 Rebuild Data 완료 대기 */
    static IDE_RC changeSHARD_REBUILD_DATA_STEP( idvSQL * /* aStatistics */,
                                                 SChar  * /* aName */,
                                                 void   * /* aOldValue */,
                                                 void   * aNewValue,
                                                 void   * /* aArg */ );

    /* BUG-45899 */
    static IDE_RC changeTRCLOG_DETAIL_SHARD( idvSQL* /* aStatistics */,
                                             SChar * aName,
                                             void  * aOldValue,
                                             void  * aNewValue,
                                             void  * /* aArg */ );

    /* BUG-46100 SMN Propagation Failure Ignore */
    static IDE_RC changeSHARD_IGNORE_SMN_PROPAGATION_FAILURE( idvSQL * /* aStatistics */,
                                                              SChar  * /* aName */,
                                                              void   * /* aOldValue */,
                                                              void   * aNewValue,
                                                              void   * /* aArg */ );

    /* BUG-46100 Session SMN Update */
    static IDE_RC changeSHARD_ALLOW_OLD_SMN( idvSQL * /* aStatistics */,
                                             SChar  * /* aName */,
                                             void   * /* aOldValue */,
                                             void   * aNewValue,
                                             void   * /* aArg */ );

    /* PROJ-2701 Sharding online data rebuild */
    static IDE_RC changeSHARD_META_HISTORY_AUTO_PURGE_DISABLE( idvSQL* /* aStatistics */,
                                                               SChar * /* aName */,
                                                               void  * /* aOldValue */,
                                                               void  * aNewValue,
                                                               void  * /* aArg */);

    static IDE_RC changeSHARD_REBUILD_PLAN_DETAIL_FORCE_ENABLE( idvSQL* /* aStatistics */,
                                                                SChar * /* aName */,
                                                                void  * /* aOldValue */,
                                                                void  * aNewValue,
                                                                void  * /* aArg */);

    static IDE_RC changeSHARD_REBUILD_LOCK_TABLE_WITH_DML_ENABLE( idvSQL* /* aStatistics */,
                                                                  SChar * /* aName */,
                                                                  void  * /* aOldValue */,
                                                                  void  * aNewValue,
                                                                  void  * /* aArg */);

    static IDE_RC changeSHARD_META_PROPAGATION_TIMEOUT( idvSQL* /* aStatistics */,
                                                        SChar * /* aName */,
                                                        void  * /* aOldValue */,
                                                        void  * aNewValue,
                                                        void  * /* aArg */);

    static IDE_RC changeSHARD_TRANSFORM_STRING_LENGTH_MAX( idvSQL* /* aStatistics */,
                                                           SChar * /* aName */,
                                                           void  * /* aOldValue */,
                                                           void  * aNewValue,
                                                           void  * /* aArg */);

    static IDE_RC changeSHARD_SMN_CACHE_APPLY_ENABLE( idvSQL* /* aStatistics */,
                                                      SChar * /* aName */,
                                                      void  * /* aOldValue */,
                                                      void  * aNewValue,
                                                      void  * /* aArg */);
};

#endif /* _O_SDU_PROPERTY_H_ */
