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
 * $Id: sduProperty.cpp 85186 2019-04-09 07:37:00Z jayce.park $
 **********************************************************************/

#include <idl.h>
#include <ide.h>
#include <sduProperty.h>

sduProperties sduProperty::mStaticProperty;

IDE_RC sduProperty::initProperty( idvSQL * /* aStatistics */ )
{
    IDE_TEST( load() != IDE_SUCCESS );
    IDE_TEST( setupUpdateCallback() != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sduProperty::finalProperty( idvSQL * /* aStatistics */ )
{
    return IDE_SUCCESS;
}

IDE_RC sduProperty::load()
{
/***********************************************************************
 *
 * Description :
 *    Server 구동 시 System Property들을 Loading한다.
 *
 * Implementation :
 *    Writable Property의 경우 CallBack을 등록해야 함.
 *
 ***********************************************************************/

    //--------------------------------------------------------
    // Trace Log 관련 property loading
    //    - Writable Property이므로 CallBack을 등록한다.
    //    - Atomic Operation이므로, BeforeCallBack은 필요 없다.
    //--------------------------------------------------------

    IDE_ASSERT( idp::read( "SHARD_ENABLE",
                           &SDU_PROPERTY( mShardEnable ) ) == IDE_SUCCESS );

    IDE_ASSERT( idp::read( "__SHARD_TEST_ENABLE",
                           &SDU_PROPERTY( mShardTestEnable ) ) == IDE_SUCCESS );

    IDE_ASSERT( idp::read( "__SHARD_AGGREGATION_TRANSFORM_DISABLE",
                           &SDU_PROPERTY( mShardAggrTransformDisable ) ) == IDE_SUCCESS );

    IDE_ASSERT( idp::read( "SHARD_INTERNAL_CONN_ATTR_RETRY_COUNT",
                           &SDU_PROPERTY( mShardInternalConnAttrRetryCount ) ) == IDE_SUCCESS );

    IDE_ASSERT( idp::read( "SHARD_INTERNAL_CONN_ATTR_RETRY_DELAY",
                           &SDU_PROPERTY( mShardInternalConnAttrRetryDelay ) ) == IDE_SUCCESS );

    IDE_ASSERT( idp::read( "SHARD_INTERNAL_CONN_ATTR_CONNECTION_TIMEOUT",
                           &SDU_PROPERTY( mShardInternalConnAttrConnTimeout ) ) == IDE_SUCCESS );

    IDE_ASSERT( idp::read( "SHARD_INTERNAL_CONN_ATTR_LOGIN_TIMEOUT",
                           &SDU_PROPERTY( mShardInternalConnAttrLoginTimeout ) ) == IDE_SUCCESS );

    IDE_ASSERT( idp::read( "SHARD_REBUILD_DATA_STEP",
                           &SDU_PROPERTY( mShardRebuildDataStep ) )
                == IDE_SUCCESS );

    /* BUG-45899 */ 
    IDE_ASSERT( idp::read( "TRCLOG_DETAIL_SHARD",
                           &SDU_PROPERTY( mTrclogDetailShard ) ) == IDE_SUCCESS );

    IDE_ASSERT( idp::read( "SHARD_IGNORE_SMN_PROPAGATION_FAILURE",
                           & SDU_PROPERTY( mShardIgnoreSmnPropagationFailure ) )
                == IDE_SUCCESS );

    IDE_ASSERT( idp::read( "SHARD_ALLOW_OLD_SMN",
                           & SDU_PROPERTY( mShardAllowOldSmn ) )
                == IDE_SUCCESS );

    /* PROJ-2701 Sharding online data rebuild */
    IDE_ASSERT( idp::read( "SHARD_META_HISTORY_AUTO_PURGE_DISABLE",
                           & SDU_PROPERTY(mShardMetaHistoryAutoPurgeDisable) ) == IDE_SUCCESS );

    IDE_ASSERT( idp::read( "SHARD_REBUILD_PLAN_DETAIL_FORCE_ENABLE",
                           & SDU_PROPERTY(mShardRebuildPlanDetailForceEnable) ) == IDE_SUCCESS );

    IDE_ASSERT( idp::read( "SHARD_REBUILD_LOCK_TABLE_WITH_DML_ENABLE",
                           & SDU_PROPERTY(mShardRebuildLockTableWithDmlEnable) ) == IDE_SUCCESS );

    IDE_ASSERT( idp::read( "SHARD_META_PROPAGATION_TIMEOUT",
                           & SDU_PROPERTY(mShardMetaPropagationTimeout) ) == IDE_SUCCESS );

    IDE_ASSERT( idp::read( "SHARD_TRANSFORM_STRING_LENGTH_MAX",
                           & SDU_PROPERTY(mShardTransformStringLengthMax) ) == IDE_SUCCESS );

    IDE_ASSERT( idp::read( "SHARD_SMN_CACHE_APPLY_ENABLE",
                           & SDU_PROPERTY(mShardSMNCacheApplyEnable) ) == IDE_SUCCESS );

    return IDE_SUCCESS;
}

IDE_RC sduProperty::setupUpdateCallback()
{
/***********************************************************************
 *
 * Description :
 *    Writable Property의 경우 CallBack을 등록해야 함.
 *
 * Implementation :
 *
 ***********************************************************************/

    //--------------------------------------------------------
    // Trace Log 관련 property loading
    //    - Writable Property이므로 CallBack을 등록한다.
    //    - Atomic Operation이므로, BeforeCallBack은 필요 없다.
    //--------------------------------------------------------

    /* PROJ-2687 Shard aggregation transform */ 
    IDE_TEST( idp::setupAfterUpdateCallback( "__SHARD_AGGREGATION_TRANSFORM_DISABLE",
                                             sduProperty::changeSHARD_AGGREGATION_TRANSFORM_DISABLE )
              != IDE_SUCCESS );

    IDE_TEST( idp::setupAfterUpdateCallback( "SHARD_INTERNAL_CONN_ATTR_RETRY_COUNT",
                                             sduProperty::changeSHARD_INTERNAL_CONN_ATTR_RETRY_COUNT )
              != IDE_SUCCESS );

    IDE_TEST( idp::setupAfterUpdateCallback( "SHARD_INTERNAL_CONN_ATTR_RETRY_DELAY",
                                             sduProperty::changeSHARD_INTERNAL_CONN_ATTR_RETRY_DELAY )
              != IDE_SUCCESS );

    IDE_TEST( idp::setupAfterUpdateCallback( "SHARD_INTERNAL_CONN_ATTR_CONNECTION_TIMEOUT",
                                             sduProperty::changeSHARD_INTERNAL_CONN_ATTR_CONN_TIMEOUT )
              != IDE_SUCCESS );

    IDE_TEST( idp::setupAfterUpdateCallback( "SHARD_INTERNAL_CONN_ATTR_LOGIN_TIMEOUT",
                                             sduProperty::changeSHARD_INTERNAL_CONN_ATTR_LOGIN_TIMEOUT )
              != IDE_SUCCESS );

    /* BUG-45967 Rebuild Data 완료 대기 */
    IDE_TEST( idp::setupAfterUpdateCallback( "SHARD_REBUILD_DATA_STEP",
                                             sduProperty::changeSHARD_REBUILD_DATA_STEP )
              != IDE_SUCCESS );

    /* BUG-45899 */ 
    IDE_TEST( idp::setupAfterUpdateCallback( "TRCLOG_DETAIL_SHARD",
                                             sduProperty::changeTRCLOG_DETAIL_SHARD )
              != IDE_SUCCESS );

    /* BUG-46100 SMN Propagation Failure Ignore */
    IDE_TEST( idp::setupAfterUpdateCallback( "SHARD_IGNORE_SMN_PROPAGATION_FAILURE",
                                             sduProperty::changeSHARD_IGNORE_SMN_PROPAGATION_FAILURE )
              != IDE_SUCCESS );

    /* BUG-46100 Session SMN Update */
    IDE_TEST( idp::setupAfterUpdateCallback( "SHARD_ALLOW_OLD_SMN",
                                             sduProperty::changeSHARD_ALLOW_OLD_SMN )
              != IDE_SUCCESS );

    /* PROJ-2701 Online data rebuild */
    IDE_TEST( idp::setupAfterUpdateCallback("SHARD_META_HISTORY_AUTO_PURGE_DISABLE",
                                            sduProperty::changeSHARD_META_HISTORY_AUTO_PURGE_DISABLE)
              != IDE_SUCCESS );

    IDE_TEST( idp::setupAfterUpdateCallback("SHARD_REBUILD_PLAN_DETAIL_FORCE_ENABLE",
                                            sduProperty::changeSHARD_REBUILD_PLAN_DETAIL_FORCE_ENABLE)
              != IDE_SUCCESS );

    IDE_TEST( idp::setupAfterUpdateCallback("SHARD_REBUILD_LOCK_TABLE_WITH_DML_ENABLE",
                                            sduProperty::changeSHARD_REBUILD_LOCK_TABLE_WITH_DML_ENABLE)
              != IDE_SUCCESS );

    IDE_TEST( idp::setupAfterUpdateCallback("SHARD_META_PROPAGATION_TIMEOUT",
                                            sduProperty::changeSHARD_META_PROPAGATION_TIMEOUT)
              != IDE_SUCCESS );

    IDE_TEST( idp::setupAfterUpdateCallback("SHARD_TRANSFORM_STRING_LENGTH_MAX",
                                            sduProperty::changeSHARD_TRANSFORM_STRING_LENGTH_MAX)
              != IDE_SUCCESS );

    IDE_TEST( idp::setupAfterUpdateCallback("SHARD_SMN_CACHE_APPLY_ENABLE",
                                            sduProperty::changeSHARD_SMN_CACHE_APPLY_ENABLE)
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/* PROJ-2687 Shard aggregation transform */
IDE_RC sduProperty::changeSHARD_AGGREGATION_TRANSFORM_DISABLE( idvSQL * /* aStatistics */,
                                                               SChar  * /* aName */,
                                                               void   * /* aOldValue */,
                                                               void   * aNewValue,
                                                               void   * /* aArg */ )
{
    idlOS::memcpy( &SDU_PROPERTY( mShardAggrTransformDisable ),
                   aNewValue,
                   ID_SIZEOF( UInt ) );

    return IDE_SUCCESS;
}

IDE_RC sduProperty::changeSHARD_INTERNAL_CONN_ATTR_RETRY_COUNT( idvSQL * /* aStatistics */,
                                                                SChar  * /* aName */,
                                                                void   * /* aOldValue */,
                                                                void   * aNewValue,
                                                                void   * /* aArg */ )
{
    idlOS::memcpy( &SDU_PROPERTY( mShardInternalConnAttrRetryCount ),
                   aNewValue,
                   ID_SIZEOF( UInt ) );

    return IDE_SUCCESS;
}

IDE_RC sduProperty::changeSHARD_INTERNAL_CONN_ATTR_RETRY_DELAY( idvSQL * /* aStatistics */,
                                                                SChar  * /* aName */,
                                                                void   * /* aOldValue */,
                                                                void   * aNewValue,
                                                                void   * /* aArg */ )
{
    idlOS::memcpy( &SDU_PROPERTY( mShardInternalConnAttrRetryDelay ),
                   aNewValue,
                   ID_SIZEOF( UInt ) );

    return IDE_SUCCESS;
}

IDE_RC sduProperty::changeSHARD_INTERNAL_CONN_ATTR_CONN_TIMEOUT( idvSQL* /* aStatistics */,
                                                                 SChar * /* aName */,
                                                                 void  * /* aOldValue */,
                                                                 void  * aNewValue,
                                                                 void  * /* aArg */)
{
    idlOS::memcpy( &SDU_PROPERTY( mShardInternalConnAttrConnTimeout ),
                   aNewValue,
                   ID_SIZEOF( UInt ) );

    return IDE_SUCCESS;
}

IDE_RC sduProperty::changeSHARD_INTERNAL_CONN_ATTR_LOGIN_TIMEOUT( idvSQL* /* aStatistics */,
                                                                  SChar * /* aName */,
                                                                  void  * /* aOldValue */,
                                                                  void  * aNewValue,
                                                                  void  * /* aArg */)
{
    idlOS::memcpy( &SDU_PROPERTY( mShardInternalConnAttrLoginTimeout ),
                   aNewValue,
                   ID_SIZEOF( UInt ) );

    return IDE_SUCCESS;
}

/* BUG-45967 Rebuild Data 완료 대기 */
IDE_RC sduProperty::changeSHARD_REBUILD_DATA_STEP( idvSQL * /* aStatistics */,
                                                   SChar  * /* aName */,
                                                   void   * /* aOldValue */,
                                                   void   * aNewValue,
                                                   void   * /* aArg */ )
{
    idlOS::memcpy( & SDU_PROPERTY( mShardRebuildDataStep ),
                   aNewValue,
                   ID_SIZEOF( SInt ) );

    return IDE_SUCCESS;
}

/* BUG-45899 */
IDE_RC sduProperty::changeTRCLOG_DETAIL_SHARD( idvSQL* /* aStatistics */,
                                               SChar * /* aName */,
                                               void  * /* aOldValue */,
                                               void  * aNewValue,
                                               void  * /* aArg */)
{
    idlOS::memcpy( &SDU_PROPERTY( mTrclogDetailShard ),
                   aNewValue,
                   ID_SIZEOF(UInt) );

    return IDE_SUCCESS;
}

/* BUG-46100 SMN Propagation Failure Ignore */
IDE_RC sduProperty::changeSHARD_IGNORE_SMN_PROPAGATION_FAILURE( idvSQL * /* aStatistics */,
                                                                SChar  * /* aName */,
                                                                void   * /* aOldValue */,
                                                                void   * aNewValue,
                                                                void   * /* aArg */ )
{
    idlOS::memcpy( & SDU_PROPERTY( mShardIgnoreSmnPropagationFailure ),
                   aNewValue,
                   ID_SIZEOF( UInt ) );

    return IDE_SUCCESS;
}

/* BUG-46100 Session SMN Update */
IDE_RC sduProperty::changeSHARD_ALLOW_OLD_SMN( idvSQL * /* aStatistics */,
                                               SChar  * /* aName */,
                                               void   * /* aOldValue */,
                                               void   * aNewValue,
                                               void   * /* aArg */ )
{
    idlOS::memcpy( & SDU_PROPERTY( mShardAllowOldSmn ),
                   aNewValue,
                   ID_SIZEOF( UInt ) );

    return IDE_SUCCESS;
}

/* PROJ-2701 Sharding online data rebuild */
IDE_RC sduProperty::changeSHARD_META_HISTORY_AUTO_PURGE_DISABLE( idvSQL* /* aStatistics */,
                                                                 SChar * /* aName */,
                                                                 void  * /* aOldValue */,
                                                                 void  * aNewValue,
                                                                 void  * /* aArg */)
{
    idlOS::memcpy( &SDU_PROPERTY( mShardMetaHistoryAutoPurgeDisable ),
                   aNewValue,
                   ID_SIZEOF( UInt ) );

    return IDE_SUCCESS;
}

IDE_RC sduProperty::changeSHARD_REBUILD_PLAN_DETAIL_FORCE_ENABLE( idvSQL* /* aStatistics */,
                                                                  SChar * /* aName */,
                                                                  void  * /* aOldValue */,
                                                                  void  * aNewValue,
                                                                  void  * /* aArg */)
{
    idlOS::memcpy( &SDU_PROPERTY( mShardRebuildPlanDetailForceEnable ),
                   aNewValue,
                   ID_SIZEOF( UInt ) );

    return IDE_SUCCESS;
}

IDE_RC sduProperty::changeSHARD_REBUILD_LOCK_TABLE_WITH_DML_ENABLE( idvSQL* /* aStatistics */,
                                                                    SChar * /* aName */,
                                                                    void  * /* aOldValue */,
                                                                    void  * aNewValue,
                                                                    void  * /* aArg */)
{
    idlOS::memcpy( &SDU_PROPERTY( mShardRebuildLockTableWithDmlEnable ),
                   aNewValue,
                   ID_SIZEOF( UInt ) );

    return IDE_SUCCESS;
}

IDE_RC sduProperty::changeSHARD_META_PROPAGATION_TIMEOUT( idvSQL* /* aStatistics */,
                                                          SChar * /* aName */,
                                                          void  * /* aOldValue */,
                                                          void  * aNewValue,
                                                          void  * /* aArg */)
{
    idlOS::memcpy( &SDU_PROPERTY( mShardMetaPropagationTimeout ),
                   aNewValue,
                   ID_SIZEOF( ULong ) );

    return IDE_SUCCESS;
}

IDE_RC sduProperty::changeSHARD_TRANSFORM_STRING_LENGTH_MAX( idvSQL* /* aStatistics */,
                                                             SChar * /* aName */,
                                                             void  * /* aOldValue */,
                                                             void  * aNewValue,
                                                             void  * /* aArg */)
{
    idlOS::memcpy( &SDU_PROPERTY( mShardTransformStringLengthMax ),
                   aNewValue,
                   ID_SIZEOF( UInt ) );

    return IDE_SUCCESS;
}

IDE_RC sduProperty::changeSHARD_SMN_CACHE_APPLY_ENABLE( idvSQL* /* aStatistics */,
                                                        SChar * /* aName */,
                                                        void  * /* aOldValue */,
                                                        void  * aNewValue,
                                                        void  * /* aArg */)
{
    idlOS::memcpy( &SDU_PROPERTY( mShardSMNCacheApplyEnable ),
                   aNewValue,
                   ID_SIZEOF( UInt ) );

    return IDE_SUCCESS;
}
