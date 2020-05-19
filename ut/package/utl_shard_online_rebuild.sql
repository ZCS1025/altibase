/***********************************************************************
 * Copyright 1999-2018, ALTIBASE Corporation or its subsidiaries.
 * All rights reserved.
 **********************************************************************/

CREATE OR REPLACE PACKAGE utl_shard_online_rebuild AUTHID CURRENT_USER IS

PROCEDURE get_connection_info( adjustment IN INTEGER DEFAULT 0 );
PRAGMA RESTRICT_REFERENCES( get_connection_info, WNDS );

PROCEDURE set_replication_properties();

PROCEDURE set_rebuild_data_step( value IN INTEGER );

PROCEDURE wait_rebuild_data_step( value IN INTEGER );

PROCEDURE print_rebuild_data_step();
PRAGMA RESTRICT_REFERENCES( print_rebuild_data_step, WNDS );

PROCEDURE insert_rebuild_state( value                     IN INTEGER,
                                rebuild_data_step_failure IN INTEGER );

PROCEDURE copy_table_schema( user_name         IN VARCHAR(128),
                             target_table_name IN VARCHAR(128),
                             source_table_name IN VARCHAR(128) );

PROCEDURE create_replication( user_name          IN VARCHAR(128),
                              my_table_name      IN VARCHAR(128),
                              peer_table_name    IN VARCHAR(128),
                              partition_name     IN VARCHAR(128),
                              rep_name           IN VARCHAR(40),
                              peer_ip            IN VARCHAR(64),
                              peer_repl_port_no  IN INTEGER,
                              ddl_replicate      IN BOOLEAN,
                              propagation        IN BOOLEAN,
                              propagable_logging IN BOOLEAN );

PROCEDURE sync_replication( rep_name             IN VARCHAR(40),
                            sync_parallel_factor IN INTEGER );

PROCEDURE start_replication( rep_name IN VARCHAR(40) );

PROCEDURE flush_replication( rep_name IN VARCHAR(40) );

PROCEDURE stop_replication( rep_name IN VARCHAR(40) );

PROCEDURE drop_replication( rep_name IN VARCHAR(40) );

PROCEDURE swap_table( user_name                      IN VARCHAR(128),
                      target_table_name              IN VARCHAR(128),
                      source_table_name              IN VARCHAR(128),
                      force_to_rename_encrypt_column IN BOOLEAN DEFAULT FALSE,
                      ignore_foreign_key_child       IN BOOLEAN DEFAULT FALSE );

PROCEDURE swap_partition( user_name         IN VARCHAR(128),
                          target_table_name IN VARCHAR(128),
                          source_table_name IN VARCHAR(128),
                          partition_name    IN VARCHAR(128) );

PROCEDURE lock_and_truncate( user_name                 IN VARCHAR(128),
                             table_name                IN VARCHAR(128),
                             partition_name            IN VARCHAR(128),
                             rep_name_for_rebuild      IN VARCHAR(40),
                             lock_wait_seconds         IN INTEGER,
                             rebuild_data_step_locked  IN INTEGER,
                             rebuild_data_step_success IN INTEGER,
                             rebuild_data_step_failure IN INTEGER );

PROCEDURE lock_and_release( user_name                 IN VARCHAR(128),
                            table_name                IN VARCHAR(128),
                            partition_name            IN VARCHAR(128),
                            rep_name_for_rebuild      IN VARCHAR(40),
                            lock_wait_seconds         IN INTEGER,
                            rebuild_data_step_locked  IN INTEGER,
                            rebuild_data_step_success IN INTEGER,
                            rebuild_data_step_failure IN INTEGER );

PROCEDURE lock_and_swap( user_name                 IN VARCHAR(128),
                         target_table_name         IN VARCHAR(128),
                         source_table_name         IN VARCHAR(128),
                         partition_name            IN VARCHAR(128),
                         rep_name_for_alternate    IN VARCHAR(40),
                         lock_wait_seconds         IN INTEGER,
                         rebuild_data_step_locked  IN INTEGER,
                         rebuild_data_step_success IN INTEGER,
                         rebuild_data_step_failure IN INTEGER );

PROCEDURE CHECK_PROPERTY( aCheck_property_name IN  VARCHAR(40),
                          aExpect_value        IN  INTEGER );

PROCEDURE CHECK_ALL_PROPERTY(); 

PROCEDURE CHECK_EXIST_REPLICATION( aReplication_name IN     VARCHAR(48),
                                   aExpect_exist     IN     BOOLEAN );

PROCEDURE CHECK_EXIST_TABLE( aUser_name      IN  VARCHAR(128),
                             aTable_name     IN  VARCHAR(128),
                             aExpect_exist   IN  BOOLEAN );

PROCEDURE CHECK_EXIST_PARTITION( aUser_name      IN   VARCHAR(128),
                                 aTable_name     IN   VARCHAR(128),
                                 aPartition_name IN   VARCHAR(128),
                                 aExpect_exist   IN   BOOLEAN );

PROCEDURE CHECK_EXIST_NODE( aNode_name      IN  VARCHAR(40),
                            aExpect_exist   IN  BOOLEAN );

PROCEDURE CHECK_REPLICATION_GAP( replication_name IN VARCHAR(40) );

PROCEDURE CHECK_REPLICATION_ROLE( aReplication_name IN  VARCHAR(40),
                                  aExpect_role      IN  INTEGER );

PROCEDURE CHECK_PARTITION_AND_SHARD_METHOD( aUser_name  IN  VARCHAR(128),
                                            aTable_name IN  VARCHAR(128) );

PROCEDURE CHECK_PARTITION_AND_SHARD_KEY_COLUMN( aUser_name    IN    VARCHAR(128),
                                                aTable_name   IN    VARCHAR(128) );

PROCEDURE CHECK_PARTITION_AND_SHARD_KEY_VALUE( aUser_name    IN  VARCHAR(128),
                                               aTable_name   IN  VARCHAR(128) );

PROCEDURE CHECK_EXIST_CLONE( aUser_name    IN    VARCHAR(128),
                             aTable_name   IN    VARCHAR(128),
                             aExpect_exist IN    BOOLEAN );

PROCEDURE CHECK_CLONE( aUser_name    IN    VARCHAR(128),
                       aTable_name   IN    VARCHAR(128),
                       aNode_name    IN    VARCHAR(40),
                       aExpect_exist IN    BOOLEAN );

PROCEDURE CHECK_EXIST_SOLO( aUser_name    IN    VARCHAR(128),
                            aTable_name   IN    VARCHAR(128),
                            aExpect_exist IN    BOOLEAN );

PROCEDURE CHECK_SOLO( aUser_name    IN    VARCHAR(128),
                      aTable_name   IN    VARCHAR(128),
                      aNode_name    IN    VARCHAR(40),
                      aExpect_exist IN    BOOLEAN );

PROCEDURE CHECK_GLOBAL_META_INFO();

PROCEDURE GET_NODE_INFO;

PROCEDURE GET_RANGE_INFO;

PROCEDURE GET_CLONE_INFO;

PROCEDURE GET_SHARD_INFO;

PROCEDURE SET_NEXT_SHARD_ID( aSequenceNo    IN BIGINT );

PROCEDURE SET_NEXT_NODE_ID( aSequenceNo    IN BIGINT );

END utl_shard_online_rebuild;
/

CREATE OR REPLACE PUBLIC SYNONYM utl_shard_online_rebuild FOR sys.utl_shard_online_rebuild;
