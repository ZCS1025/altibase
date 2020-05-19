/***********************************************************************
 * Copyright 1999-2015, ALTIBASE Corporation or its subsidiaries.
 * All rights reserved.
 **********************************************************************/

CREATE OR REPLACE PACKAGE DBMS_SHARD IS

procedure CREATE_META( meta_node_id in integer );

procedure RESET_META_NODE_ID( meta_node_id in integer );

procedure SET_NODE( node_name         in varchar(40),
                    host_ip           in varchar(16),
                    port_no           in integer,
                    alternate_host_ip in varchar(16) default NULL,
                    alternate_port_no in integer default NULL,
                    conn_type         in integer default NULL );

procedure RESET_NODE_INTERNAL( node_name         in varchar(40),
                               host_ip           in varchar(16),
                               port_no           in integer,
                               alternate_host_ip in varchar(16),
                               alternate_port_no in integer,
                               conn_type         in integer );

procedure RESET_NODE_EXTERNAL( node_name         in varchar(40),
                               host_ip           in varchar(16),
                               port_no           in integer,
                               alternate_host_ip in varchar(16),
                               alternate_port_no in integer );

procedure UNSET_NODE( node_name in varchar(40) );

procedure SET_SHARD_TABLE( user_name         in varchar(128),
                           table_name        in varchar(128),
                           split_method      in varchar(1),
                           key_column_name   in varchar(128) default NULL,
                           default_node_name in varchar(40) default NULL );

procedure SET_SHARD_TABLE_COMPOSITE( user_name           in varchar(128),
                                     table_name          in varchar(128),
                                     split_method        in varchar(1),
                                     key_column_name     in varchar(128),
                                     sub_split_method    in varchar(1),
                                     sub_key_column_name in varchar(128),
                                     default_node_name   in varchar(40) default NULL );

procedure SET_SHARD_PROCEDURE( user_name          in varchar(128),
                               proc_name          in varchar(128),
                               split_method       in varchar(1),
                               key_parameter_name in varchar(128) default NULL,
                               default_node_name  in varchar(40) default NULL );

procedure SET_SHARD_PROCEDURE_COMPOSITE( user_name              in varchar(128),
                                         proc_name              in varchar(128),
                                         split_method           in varchar(1),
                                         key_parameter_name     in varchar(128),
                                         sub_split_method       in varchar(1),
                                         sub_key_parameter_name in varchar(128),
                                         default_node_name      in varchar(40) default NULL );

procedure SET_SHARD_COMPOSITE( user_name   in varchar(128),
                               object_name in varchar(128),
                               value       in varchar(100),
                               sub_value   in varchar(100),
                               node_name   in varchar(40) );

procedure SET_SHARD_HASH( user_name   in varchar(128),
                          object_name in varchar(128),
                          value       in integer,
                          node_name   in varchar(40) );

procedure SET_SHARD_RANGE( user_name   in varchar(128),
                           object_name in varchar(128),
                           value       in varchar(100),
                           node_name   in varchar(40) );

procedure SET_SHARD_LIST( user_name   in varchar(128),
                          object_name in varchar(128),
                          value       in varchar(100),
                          node_name   in varchar(40) );

procedure SET_SHARD_CLONE( user_name   in varchar(128),
                           object_name in varchar(128),
                           node_name   in varchar(40) );

procedure SET_SHARD_SOLO( user_name   in varchar(128),
                          object_name in varchar(128),
                          node_name   in varchar(40) );

procedure RESET_SHARD_RESIDENT_NODE( user_name     in varchar(128),
                                     object_name   in varchar(128),
                                     old_node_name in varchar(40),
                                     new_node_name in varchar(40),
                                     value         in varchar(100) default NULL,
                                     sub_value     in varchar(100) default NULL );

procedure UNSET_SHARD_TABLE( user_name  in varchar(128),
                             table_name in varchar(128) );

procedure UNSET_SHARD_PROCEDURE( user_name in varchar(128),
                                 proc_name in varchar(128) );

procedure UNSET_SHARD_TABLE_BY_ID( shard_id in integer );

procedure UNSET_SHARD_PROCEDURE_BY_ID( shard_id in integer );

procedure EXECUTE_IMMEDIATE( query     in varchar(65534),
                             node_name in varchar(40) default NULL );

procedure CHECK_DATA( user_name            in varchar(128),
                      table_name           in varchar(128),
                      additional_node_list in varchar(1000) default NULL );

procedure REBUILD_DATA_NODE( user_name   in varchar(128),
                             table_name  in varchar(128),
                             node_name   in varchar(40),
                             batch_count in bigint default 0 );

procedure REBUILD_DATA( user_name            in varchar(128),
                        table_name           in varchar(128),
                        batch_count          in bigint default 0,
                        additional_node_list in varchar(1000) default NULL );

procedure PURGE_OLD_SHARD_META_INFO( smn in bigint default NULL );

end dbms_shard;
/

CREATE OR REPLACE PUBLIC SYNONYM dbms_shard FOR sys.dbms_shard;
GRANT EXECUTE ON dbms_shard TO PUBLIC;

