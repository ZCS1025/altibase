/***********************************************************************
 * Copyright 1999-2015, ALTIBASE Corporation or its subsidiaries.
 * All rights reserved.
 **********************************************************************/

create or replace package body DBMS_SHARD is


-- CREATE META
procedure CREATE_META( meta_node_id in integer )
as
    dummy integer;
begin
    dummy := shard_create_meta( meta_node_id );
end CREATE_META;

-- RESET META NODE ID
procedure RESET_META_NODE_ID( meta_node_id in integer )
as
    dummy integer;
begin
    dummy := shard_reset_meta_node_id( meta_node_id );
end RESET_META_NODE_ID;

-- SET DATA NODE
procedure SET_NODE( node_name         in varchar(40),
                    host_ip           in varchar(16),
                    port_no           in integer,
                    alternate_host_ip in varchar(16) default NULL,
                    alternate_port_no in integer default NULL,
                    conn_type         in integer default NULL )
as
    dummy integer;
    up_node_name varchar(40);
begin
    if instr( node_name, chr(34) ) = 0 then
        up_node_name := upper( node_name );
    else
        up_node_name := replace2( node_name, chr(34) );
    end if;

    dummy := shard_set_node(up_node_name, host_ip, port_no, alternate_host_ip, alternate_port_no, conn_type);
end SET_NODE;

-- RESET DATA NODE (INTERNAL)
procedure RESET_NODE_INTERNAL( node_name         in varchar(40),
                               host_ip           in varchar(16),
                               port_no           in integer,
                               alternate_host_ip in varchar(16),
                               alternate_port_no in integer,
                               conn_type         in integer)
as
    dummy integer;
    up_node_name varchar(40);
begin
    if instr( node_name, chr(34) ) = 0 then
        up_node_name := upper( node_name );
    else
        up_node_name := replace2( node_name, chr(34) );
    end if;

    dummy := shard_reset_node_internal( up_node_name,
                                        host_ip,
                                        port_no,
                                        alternate_host_ip,
                                        alternate_port_no,
                                        conn_type );
end RESET_NODE_INTERNAL;

-- RESET DATA NODE (EXTERNAL)
procedure RESET_NODE_EXTERNAL( node_name         in varchar(40),
                               host_ip           in varchar(16),
                               port_no           in integer,
                               alternate_host_ip in varchar(16),
                               alternate_port_no in integer )
as
    dummy integer;
    up_node_name varchar(40);
begin
    if instr( node_name, chr(34) ) = 0 then
        up_node_name := upper( node_name );
    else
        up_node_name := replace2( node_name, chr(34) );
    end if;

    dummy := shard_reset_node_external( up_node_name,
                                        host_ip,
                                        port_no,
                                        alternate_host_ip,
                                        alternate_port_no );
end RESET_NODE_EXTERNAL;

-- UNSET DATA NODE
procedure UNSET_NODE( node_name in varchar(40) )
as
    dummy integer;
    up_node_name varchar(40);
begin
    if instr( node_name, chr(34) ) = 0 then
        up_node_name := upper( node_name );
    else
        up_node_name := replace2( node_name, chr(34) );
    end if;

    dummy := shard_unset_node( up_node_name );
end UNSET_NODE;

-- SET SHARD TABLE INFO
procedure SET_SHARD_TABLE( user_name         in varchar(128),
                           table_name        in varchar(128),
                           split_method      in varchar(1),
                           key_column_name   in varchar(128) default NULL,
                           default_node_name in varchar(40) default NULL )
as
    dummy integer;
    up_user_name varchar(128);
    up_table_name varchar(128);
    up_split_method varchar(1);
    up_key_column_name varchar(128);
    up_default_node_name varchar(40);
begin
    if instr( user_name, chr(34) ) = 0 then
        up_user_name := upper( user_name );
    else
        up_user_name := replace2( user_name, chr(34) );
    end if;

    if instr( table_name, chr(34) ) = 0 then
        up_table_name := upper( table_name );
    else
        up_table_name := replace2( table_name, chr(34) );
    end if;

    up_split_method := upper( split_method );

    if instr( key_column_name, chr(34) ) = 0 then
        up_key_column_name := upper( key_column_name );
    else
        up_key_column_name := replace2( key_column_name, chr(34) );
    end if;

    if instr( default_node_name, chr(34) ) = 0 then
        up_default_node_name := upper( default_node_name );
    else
        up_default_node_name := replace2( default_node_name, chr(34) );
    end if;

    dummy := shard_set_shard_table( up_user_name,
                                    up_table_name,
                                    up_split_method,
                                    up_key_column_name,
                                    NULL,                  -- sub_split_method
                                    NULL,                  -- sub_key_column_name
                                    up_default_node_name );
end SET_SHARD_TABLE;

--SET COMPOSITE SHARD TABLE INFO
procedure SET_SHARD_TABLE_COMPOSITE( user_name           in varchar(128),
                                     table_name          in varchar(128),
                                     split_method        in varchar(1),
                                     key_column_name     in varchar(128),
                                     sub_split_method    in varchar(1),
                                     sub_key_column_name in varchar(128),
                                     default_node_name   in varchar(40) default NULL )
as
    dummy integer;
    up_user_name varchar(128);
    up_table_name varchar(128);
    up_split_method varchar(1);
    up_key_column_name varchar(128);
    up_sub_split_method varchar(1);
    up_sub_key_column_name varchar(128);
    up_default_node_name varchar(40);
begin
    if instr( user_name, chr(34) ) = 0 then
        up_user_name := upper( user_name );
    else
        up_user_name := replace2( user_name, chr(34) );
    end if;

    if instr( table_name, chr(34) ) = 0 then
        up_table_name := upper( table_name );
    else
        up_table_name := replace2( table_name, chr(34) );
    end if;

    up_split_method := upper( split_method );

    if instr( key_column_name, chr(34) ) = 0 then
        up_key_column_name := upper( key_column_name );
    else
        up_key_column_name := replace2( key_column_name, chr(34) );
    end if;

    up_sub_split_method := upper( sub_split_method );

    if instr( sub_key_column_name, chr(34) ) = 0 then
        up_sub_key_column_name := upper( sub_key_column_name );
    else
        up_sub_key_column_name := replace2( sub_key_column_name, chr(34) );
    end if;

    if instr( default_node_name, chr(34) ) = 0 then
        up_default_node_name := upper( default_node_name );
    else
        up_default_node_name := replace2( default_node_name, chr(34) );
    end if;

    dummy := shard_set_shard_table( up_user_name,
                                    up_table_name,
                                    up_split_method,
                                    up_key_column_name,
                                    up_sub_split_method,
                                    up_sub_key_column_name,
                                    up_default_node_name );
end SET_SHARD_TABLE_COMPOSITE;

-- SET SHARD PROCEDURE INFO
procedure SET_SHARD_PROCEDURE( user_name          in varchar(128),
                               proc_name          in varchar(128),
                               split_method       in varchar(1),
                               key_parameter_name in varchar(128) default NULL,
                               default_node_name  in varchar(40) default NULL )
as
    dummy integer;
    up_user_name varchar(128);
    up_proc_name varchar(128);
    up_split_method varchar(1);
    up_key_parameter_name varchar(128);
    up_default_node_name varchar(40);
begin
    if instr( user_name, chr(34) ) = 0 then
        up_user_name := upper( user_name );
    else
        up_user_name := replace2( user_name, chr(34) );
    end if;

    if instr( proc_name, chr(34) ) = 0 then
        up_proc_name := upper( proc_name );
    else
        up_proc_name := replace2( proc_name, chr(34) );
    end if;

    up_split_method := upper( split_method );

    if instr( key_parameter_name, chr(34) ) = 0 then
        up_key_parameter_name := upper( key_parameter_name );
    else
        up_key_parameter_name := replace2( key_parameter_name, chr(34) );
    end if;

    if instr( default_node_name, chr(34) ) = 0 then
        up_default_node_name := upper( default_node_name );
    else
        up_default_node_name := replace2( default_node_name, chr(34) );
    end if;

    dummy := shard_set_shard_procedure( up_user_name,
                                        up_proc_name,
                                        up_split_method,
                                        up_key_parameter_name,
                                        NULL,                  -- sub_split_method
                                        NULL,                  -- sub_key_parameter_name
                                        up_default_node_name );
end SET_SHARD_PROCEDURE;

-- SET COMPOSITE SHARD PROCEDURE INFO
procedure SET_SHARD_PROCEDURE_COMPOSITE( user_name              in varchar(128),
                                         proc_name              in varchar(128),
                                         split_method           in varchar(1),
                                         key_parameter_name     in varchar(128),
                                         sub_split_method       in varchar(1),
                                         sub_key_parameter_name in varchar(128),
                                         default_node_name      in varchar(40) default NULL )
as
    dummy integer;
    up_user_name varchar(128);
    up_proc_name varchar(128);
    up_split_method varchar(1);
    up_key_parameter_name varchar(128);
    up_sub_split_method varchar(1);
    up_sub_key_parameter_name varchar(128);
    up_default_node_name varchar(40);
begin
    if instr( user_name, chr(34) ) = 0 then
        up_user_name := upper( user_name );
    else
        up_user_name := replace2( user_name, chr(34) );
    end if;

    if instr( proc_name, chr(34) ) = 0 then
        up_proc_name := upper( proc_name );
    else
        up_proc_name := replace2( proc_name, chr(34) );
    end if;

    up_split_method := upper( split_method );

    if instr( key_parameter_name, chr(34) ) = 0 then
        up_key_parameter_name := upper( key_parameter_name );
    else
        up_key_parameter_name := replace2( key_parameter_name, chr(34) );
    end if;

    up_sub_split_method := upper( sub_split_method );

    if instr( sub_key_parameter_name, chr(34) ) = 0 then
        up_sub_key_parameter_name := upper( sub_key_parameter_name );
    else
        up_sub_key_parameter_name := replace2( sub_key_parameter_name, chr(34) );
    end if;

    if instr( default_node_name, chr(34) ) = 0 then
        up_default_node_name := upper( default_node_name );
    else
        up_default_node_name := replace2( default_node_name, chr(34) );
    end if;

    dummy := shard_set_shard_procedure( up_user_name,
                                        up_proc_name,
                                        up_split_method,
                                        up_key_parameter_name,
                                        up_sub_split_method,
                                        up_sub_key_parameter_name,
                                        up_default_node_name );
end SET_SHARD_PROCEDURE_COMPOSITE;

-- SET SHARD HASH
procedure SET_SHARD_HASH( user_name   in varchar(128),
                          object_name in varchar(128),
                          value       in integer,
                          node_name   in varchar(40) )
as
    dummy integer;
    up_user_name varchar(128);
    up_object_name varchar(128);
    up_node_name varchar(40);

begin

    if instr( user_name, chr(34) ) = 0 then
        up_user_name := upper( user_name );
    else
        up_user_name := replace2( user_name, chr(34) );
    end if;

    if instr( object_name, chr(34) ) = 0 then
        up_object_name := upper( object_name );
    else
        up_object_name := replace2( object_name, chr(34) );
    end if;

    if instr( node_name, chr(34) ) = 0 then
        up_node_name := upper( node_name );
    else
        up_node_name := replace2( node_name, chr(34) );
    end if;

    dummy := shard_set_shard_range( up_user_name,
                                    up_object_name,
                                    value,
                                    NULL,          -- sub_value
                                    up_node_name,
                                    'H' );
end SET_SHARD_HASH;

-- SET SHARD RANGE
procedure SET_SHARD_RANGE( user_name   in varchar(128),
                           object_name in varchar(128),
                           value       in varchar(100),
                           node_name   in varchar(40) )
as
    dummy integer;
    up_user_name varchar(128);
    up_object_name varchar(128);
    up_node_name varchar(40);

begin

    if instr( user_name, chr(34) ) = 0 then
        up_user_name := upper( user_name );
    else
        up_user_name := replace2( user_name, chr(34) );
    end if;

    if instr( object_name, chr(34) ) = 0 then
        up_object_name := upper( object_name );
    else
        up_object_name := replace2( object_name, chr(34) );
    end if;

    if instr( node_name, chr(34) ) = 0 then
        up_node_name := upper( node_name );
    else
        up_node_name := replace2( node_name, chr(34) );
    end if;

    dummy := shard_set_shard_range( up_user_name,
                                    up_object_name,
                                    value,
                                    NULL,          -- sub_value
                                    up_node_name,
                                    'R' );
end SET_SHARD_RANGE;

-- SET SHARD LIST
procedure SET_SHARD_LIST( user_name   in varchar(128),
                          object_name in varchar(128),
                          value       in varchar(100),
                          node_name   in varchar(40) )
as
    dummy integer;
    up_user_name varchar(128);
    up_object_name varchar(128);
    up_node_name varchar(40);

begin

    if instr( user_name, chr(34) ) = 0 then
        up_user_name := upper( user_name );
    else
        up_user_name := replace2( user_name, chr(34) );
    end if;

    if instr( object_name, chr(34) ) = 0 then
        up_object_name := upper( object_name );
    else
        up_object_name := replace2( object_name, chr(34) );
    end if;

    if instr( node_name, chr(34) ) = 0 then
        up_node_name := upper( node_name );
    else
        up_node_name := replace2( node_name, chr(34) );
    end if;

    dummy := shard_set_shard_range( up_user_name,
                                    up_object_name,
                                    value,
                                    NULL,          -- sub_value
                                    up_node_name,
                                    'L' );
end SET_SHARD_LIST;

-- SET SHARD CLONE
procedure SET_SHARD_CLONE( user_name   in varchar(128),
                           object_name in varchar(128),
                           node_name   in varchar(40) )
as
    dummy integer;
    up_user_name varchar(128);
    up_object_name varchar(128);
    up_node_name varchar(40);

begin

    if instr( user_name, chr(34) ) = 0 then
        up_user_name := upper( user_name );
    else
        up_user_name := replace2( user_name, chr(34) );
    end if;

    if instr( object_name, chr(34) ) = 0 then
        up_object_name := upper( object_name );
    else
        up_object_name := replace2( object_name, chr(34) );
    end if;

    if instr( node_name, chr(34) ) = 0 then
        up_node_name := upper( node_name );
    else
        up_node_name := replace2( node_name, chr(34) );
    end if;

    dummy := shard_set_shard_clone( up_user_name,
                                    up_object_name,
                                    up_node_name );
end SET_SHARD_CLONE;

-- SET SHARD SOLO 
procedure SET_SHARD_SOLO( user_name   in varchar(128),
                          object_name in varchar(128),
                          node_name   in varchar(40) )
as
    dummy integer;
    up_user_name varchar(128);
    up_object_name varchar(128);
    up_node_name varchar(40);

begin

    if instr( user_name, chr(34) ) = 0 then
        up_user_name := upper( user_name );
    else
        up_user_name := replace2( user_name, chr(34) );
    end if;

    if instr( object_name, chr(34) ) = 0 then
        up_object_name := upper( object_name );
    else
        up_object_name := replace2( object_name, chr(34) );
    end if;

    if instr( node_name, chr(34) ) = 0 then
        up_node_name := upper( node_name );
    else
        up_node_name := replace2( node_name, chr(34) );
    end if;

    dummy := shard_set_shard_solo( up_user_name,
                                   up_object_name,
                                   up_node_name );
end SET_SHARD_SOLO;

-- SET SHARD COMPOSITE
procedure SET_SHARD_COMPOSITE( user_name   in varchar(128),
                               object_name in varchar(128),
                               value       in varchar(100),
                               sub_value   in varchar(100),
                               node_name   in varchar(40) )
as
    dummy integer;
    up_user_name varchar(128);
    up_object_name varchar(128);
    up_node_name varchar(40);

begin

    if instr( user_name, chr(34) ) = 0 then
        up_user_name := upper( user_name );
    else
        up_user_name := replace2( user_name, chr(34) );
    end if;

    if instr( object_name, chr(34) ) = 0 then
        up_object_name := upper( object_name );
    else
        up_object_name := replace2( object_name, chr(34) );
    end if;

    if instr( node_name, chr(34) ) = 0 then
        up_node_name := upper( node_name );
    else
        up_node_name := replace2( node_name, chr(34) );
    end if;

    dummy := shard_set_shard_range( up_user_name,
                                    up_object_name,
                                    value,
                                    sub_value,
                                    up_node_name,
                                    'P' );
end SET_SHARD_COMPOSITE;

-- RESET_SHARD_RESIDENT_NODE
procedure RESET_SHARD_RESIDENT_NODE( user_name     in varchar(128),
                                     object_name   in varchar(128),
                                     old_node_name in varchar(40),
                                     new_node_name in varchar(40),
                                     value         in varchar(100) default NULL,
                                     sub_value     in varchar(100) default NULL )
as
    dummy integer;
    up_user_name varchar(128);
    up_object_name varchar(128);
    up_old_node_name varchar(40);
    up_new_node_name varchar(40);

begin

    if instr( user_name, chr(34) ) = 0 then
        up_user_name := upper( user_name );
    else
        up_user_name := replace2( user_name, chr(34) );
    end if;

    if instr( object_name, chr(34) ) = 0 then
        up_object_name := upper( object_name );
    else
        up_object_name := replace2( object_name, chr(34) );
    end if;

    if instr( old_node_name, chr(34) ) = 0 then
        up_old_node_name := upper( old_node_name );
    else
        up_old_node_name := replace2( old_node_name, chr(34) );
    end if;

    if instr( new_node_name, chr(34) ) = 0 then
        up_new_node_name := upper( new_node_name );
    else
        up_new_node_name := replace2( new_node_name, chr(34) );
    end if;

    dummy := shard_reset_shard_resident_node( up_user_name,
                                              up_object_name,
                                              up_old_node_name,
                                              up_new_node_name,
                                              value,
                                              sub_value );
end RESET_SHARD_RESIDENT_NODE;

-- UNSET SHARD TABLE
procedure UNSET_SHARD_TABLE( user_name  in varchar(128),
                             table_name in varchar(128) )
as
    dummy integer;
    up_user_name varchar(128);
    up_table_name varchar(128);

begin
    if instr( user_name, chr(34) ) = 0 then
        up_user_name := upper( user_name );
    else
        up_user_name := replace2( user_name, chr(34) );
    end if;

    if instr( table_name, chr(34) ) = 0 then
        up_table_name := upper( table_name );
    else
        up_table_name := replace2( table_name, chr(34) );
    end if;

    dummy := shard_unset_shard_table( up_user_name, up_table_name );
end UNSET_SHARD_TABLE;

-- UNSET SHARD PROCEDURE
procedure UNSET_SHARD_PROCEDURE( user_name in varchar(128),
                                 proc_name in varchar(128) )
as
    dummy integer;
    up_user_name varchar(128);
    up_proc_name varchar(128);

begin
    if instr( user_name, chr(34) ) = 0 then
        up_user_name := upper( user_name );
    else
        up_user_name := replace2( user_name, chr(34) );
    end if;

    if instr( proc_name, chr(34) ) = 0 then
        up_proc_name := upper( proc_name );
    else
        up_proc_name := replace2( proc_name, chr(34) );
    end if;

    dummy := shard_unset_shard_procedure( up_user_name, up_proc_name );
end UNSET_SHARD_PROCEDURE;

-- UNSET SHARD TABLE BY ID
procedure UNSET_SHARD_TABLE_BY_ID( shard_id in integer )
as
    dummy integer;
begin
    dummy := shard_unset_shard_table_by_id( shard_id );
end UNSET_SHARD_TABLE_BY_ID;

-- UNSET SHARD PROCEDURE BY ID
procedure UNSET_SHARD_PROCEDURE_BY_ID( shard_id in integer )
as
    dummy integer;
begin
    dummy := shard_unset_shard_procedure_by_id( shard_id );
end UNSET_SHARD_PROCEDURE_BY_ID;

-- EXECUTE ADHOC QUERY
procedure EXECUTE_IMMEDIATE( query     in varchar(65534),
                             node_name in varchar(40) default NULL )
as
    dummy integer;
    up_node_name varchar(40);
begin
    if instr( node_name, chr(34) ) = 0 then
        up_node_name := upper( node_name );
    else
        up_node_name := replace2( node_name, chr(34) );
    end if;

    dummy := shard_execute_immediate( query, up_node_name );
end EXECUTE_IMMEDIATE;

-- CHECK SHARD DATA
procedure CHECK_DATA( user_name            in varchar(128),
                      table_name           in varchar(128),
                      additional_node_list in varchar(1000) default NULL )
as
  up_node_name varchar(40);
  up_user_name varchar(128);
  up_table_name varchar(128);
  key_column_name varchar(128);
  shard_info varchar(1000);
  node_name varchar(40);
  stmt varchar(4000);
  type my_cur is ref cursor;
  cur my_cur;
  record_count bigint;
  correct_count bigint;
  total_record_count bigint;
  total_incorrect_count bigint;

  type additional_node_arr_t is table of varchar(40) index by integer;
  additional_node_arr additional_node_arr_t;
  additional_node_arr_idx integer;
  additional_node_arr_length integer;
  additional_node_list_length integer;
  double_q_section integer;
  node_list varchar(1000);
begin

  if instr( user_name, chr(34) ) = 0 then
    up_user_name := upper( user_name );
  else
    up_user_name := replace2( user_name, chr(34) );
  end if;

  if instr( table_name, chr(34) ) = 0 then
    up_table_name := upper( table_name );
  else
    up_table_name := replace2( table_name, chr(34) );
  end if;

  if additional_node_list is not null then
    additional_node_list_length := length(additional_node_list);
    double_q_section := 0;
    additional_node_arr_idx := 0;
    additional_node_arr[additional_node_arr_idx] := '';

    for i in 1 .. additional_node_list_length loop
      if instr( substr( additional_node_list, i, 1 ), chr(34) ) = 1 then -- chr(34) = double quotation mark 
        double_q_section := abs(double_q_section-1);
      elseif instr( substr( additional_node_list, i, 1 ), chr(44) ) = 1 then -- chr(44) = comma 
        if double_q_section = 0 then
          additional_node_arr_idx := additional_node_arr_idx + 1;
          additional_node_arr[additional_node_arr_idx] := '';
          continue;
        end if;      
      end if;
      additional_node_arr[additional_node_arr_idx] := additional_Node_arr[additional_node_arr_idx]||substr( additional_node_list, i, 1 );
    end loop;

     for i in 0 .. additional_node_arr_idx loop
      additional_node_arr_length := length(additional_node_arr[i]);
      if ( instr( substr( additional_node_arr[i], 1, 1 ), chr(34) ) = 1 ) and 
         ( instr( substr( additional_node_arr[i], additional_node_arr_length, 1 ), chr(34) ) = 1 ) then
        additional_node_arr[i] := replace2( additional_node_arr[i], chr(34) );
      else
        additional_node_arr[i] := upper( additional_node_arr[i] );
      end if;
    end loop;
  end if;

  stmt :=
  'select /*+group bucket count(1000)*/ nvl2(sub_key_column_name,key_column_name||'', ''||sub_key_column_name,key_column_name), ''{"SplitMethod":''||nvl2(sub_split_method,''["''||split_method||''","''||sub_split_method||''"]'',''"''||split_method||''"'')||nvl2(n.node_name,'',"DefaultNode":"''||n.node_name||''"'',null)||'',"RangeInfo":[''||group_concat(''{"Value":''||decode(sub_value,''$$N/A'',''"''||value||''"'',''["''||value||''","''||sub_value||''"]'')||'',"Node":"''||v.node_name||''"}'','','')||'']}'' 
from (
  select o.key_column_name, o.sub_key_column_name, o.split_method, o.sub_split_method, o.default_node_id, r.value, r.sub_value, n.node_name, g.shard_meta_number smn
  from sys_shard.objects_ o, sys_shard.ranges_ r, sys_shard.nodes_ n, sys_shard.global_meta_info_ g
  where o.object_type=''T'' and o.user_name='''||up_user_name||''' and o.object_name='''||up_table_name||''' and o.split_method in (''H'',''R'',''L'') 
    and o.shard_id=r.shard_id and r.node_id=n.node_id
    and o.smn = g.shard_meta_number
    and r.smn = g.shard_meta_number
    and n.smn = g.shard_meta_number
) v left outer join ( select node_id, node_name
                        from sys_shard.nodes_ n, 
                             sys_shard.global_meta_info_ g
                       where n.smn = g.shard_meta_number ) n
    on default_node_id=n.node_id
group by key_column_name, sub_key_column_name, split_method, sub_split_method, n.node_name';
  --println(stmt);
  execute immediate stmt into key_column_name, shard_info;

  println('shard_key_column:'||key_column_name);
  println('shard_information:'||shard_info);
  println('');


  if additional_node_list is NULL then
    stmt := 'shard
      select shard_node_name() node_name, count(*) total_cnt, decode_count(1, shard_node_name(), home_name) correct_cnt
        from (select shard_condition('||key_column_name||', '''||shard_info||''') home_name from '||up_user_name||'.'||up_table_name||')';
  else
    stmt := 'select '||'''node[data(''''''||group_concat(distinct node_name,'''''','''''')||'''''')]''  node_list
               from ( select n.node_name 
                        from sys_shard.ranges_ r, 
                             sys_shard.nodes_ n, 
                             sys_shard.objects_ o,
                             sys_shard.global_meta_info_ g 
                       where r.node_id = n.node_id 
                         and r.shard_id = o.shard_id 
                         and r.smn = g.shard_meta_number
                         and n.smn = g.shard_meta_number
                         and o.smn = g.shard_meta_number
                         and o.user_name='''||up_user_name||'''
                         and o.object_name='''||up_table_name||'''';
     for i in 0 .. additional_node_arr_idx loop
       stmt := stmt||'union all select '''||additional_node_arr[i]||''' from dual ';
     end loop;

     stmt := stmt||')';

    execute immediate stmt into node_list;

    stmt := node_list||'select shard_node_name() node_name, count(*) total_cnt, decode_count(1, shard_node_name(), home_name) correct_cnt 
        from (select shard_condition('||key_column_name||', '''||shard_info||''') home_name from '||up_user_name||'.'||up_table_name||')';
  end if;

  --println(stmt);
  total_record_count := 0;
  total_incorrect_count := 0;
  open cur for stmt;
  loop
    fetch cur into node_name, record_count, correct_count;
    exit when cur%notfound;

    if instr( node_name, chr(34) ) = 0 then
      up_node_name := upper( node_name );
    else
      up_node_name := replace2( node_name, chr(34) );
    end if;

    total_record_count := total_record_count + record_count;
    total_incorrect_count := total_incorrect_count + (record_count-correct_count);
    println('node_name:'||up_node_name||', record_count:'||record_count||', correct_count:'||correct_count||', incorrect_count:'||(record_count-correct_count));
  end loop;
  close cur;
  println('');
  
  println('total_record_count   :'||total_record_count);
  println('total_incorrect_count:'||total_incorrect_count);

exception
when no_data_found then
  println('Non-shard objects or unsupported split method.');
--  println('SQLCODE: ' || SQLCODE); -- 에러코드 출력
--  println('SQLERRM: ' || SQLERRM); -- 에러메시지 출력
end CHECK_DATA;

-- REBUILD SHARD DATA FOR NODE
procedure REBUILD_DATA_NODE( user_name   in varchar(128),
                             table_name  in varchar(128),
                             node_name   in varchar(40),
                             batch_count in bigint default 0 )
as
  up_user_name varchar(128);
  up_table_name varchar(128);
  up_node_name varchar(40);
  key_column_name varchar(128);
  shard_info varchar(1000);
  stmt1 varchar(4000);
  stmt2 varchar(4000);
  batch_stmt varchar(100);
  total_count bigint;
  cur_count bigint;
  flag integer;
  cursor c1 is select autocommit_flag from v$session where id=session_id();
begin

  if instr( user_name, chr(34) ) = 0 then
    up_user_name := upper( user_name );
  else
    up_user_name := replace2( user_name, chr(34) );
  end if;

  if instr( table_name, chr(34) ) = 0 then
    up_table_name := upper( table_name );
  else
    up_table_name := replace2( table_name, chr(34) );
  end if;

  if instr( node_name, chr(34) ) = 0 then
    up_node_name := upper( node_name );
  else
    up_node_name := replace2( node_name, chr(34) );
  end if;

  if batch_count > 0 then
    open c1;
    fetch c1 into flag;
    close c1;

    if flag = 1 then
      raise_application_error(990000, 'Retry this statement in non-autocommit mode.');
    end if;
  end if;

  begin
    stmt1 :=
  'select /*+group bucket count(1000)*/ nvl2(sub_key_column_name,key_column_name||'', ''||sub_key_column_name,key_column_name), ''{"SplitMethod":''||nvl2(sub_split_method,''["''||split_method||''","''||sub_split_method||''"]'',''"''||split_method||''"'')||nvl2(n.node_name,'',"DefaultNode":"''||n.node_name||''"'',null)||'',"RangeInfo":[''||group_concat(''{"Value":''||decode(sub_value,''$$N/A'',''"''||value||''"'',''["''||value||''","''||sub_value||''"]'')||'',"Node":"''||v.node_name||''"}'','','')||'']}'' 
from (
  select o.key_column_name, o.sub_key_column_name, o.split_method, o.sub_split_method, o.default_node_id, r.value, r.sub_value, n.node_name, g.shard_meta_number smn
  from sys_shard.objects_ o, sys_shard.ranges_ r, sys_shard.nodes_ n, sys_shard.global_meta_info_ g
  where o.object_type=''T'' and o.user_name='''||up_user_name||''' and o.object_name='''||up_table_name||''' and o.split_method in (''H'',''R'',''L'') 
    and o.shard_id=r.shard_id and r.node_id=n.node_id
    and o.smn = g.shard_meta_number
    and r.smn = g.shard_meta_number
    and n.smn = g.shard_meta_number
) v left outer join ( select node_id, node_name
                        from sys_shard.nodes_ n, 
                             sys_shard.global_meta_info_ g
                       where n.smn = g.shard_meta_number ) n
    on default_node_id=n.node_id
group by key_column_name, sub_key_column_name, split_method, sub_split_method, n.node_name';
    --println(stmt1);
    execute immediate stmt1 into key_column_name, shard_info;
    exception
    when no_data_found then
      raise_application_error(990000, 'Non-shard objects or unsupported split method.');
    when others then
      raise;
  end;

  --println(key_column_name);
  --println(shard_info);

  if batch_count > 0 then
    batch_stmt := 'limit '||batch_count||' ';
  else
    batch_stmt := '';
  end if;

  stmt1 := '
  INSERT /*+DISABLE_INSERT_TRIGGER*/ INTO '||up_user_name||'.'||up_table_name||'
  NODE[DATA('''||up_node_name||''')] SELECT * FROM '||up_user_name||'.'||up_table_name||'
   WHERE shard_condition('||key_column_name||', '''||shard_info||''')!='''||up_node_name||''' '||batch_stmt||'
     FOR MOVE AND DELETE';
  --println(stmt1);

  stmt2 := '
   NODE[DATA('''||up_node_name||''')] SELECT COUNT(*) FROM '||up_user_name||'.'||up_table_name||'
   WHERE shard_condition('||key_column_name||', '''||shard_info||''')!='''||up_node_name||'''';
  --println(stmt2);

  total_count := 0;
  loop
    loop
      execute immediate stmt1;
      commit;
      cur_count := SQL%ROWCOUNT;
      exit when cur_count = 0;
      total_count := total_count + cur_count;
      if batch_count > 0 then
        println('['||to_char(sysdate, 'HH24:MI:SS')||'] '||total_count||' moved');
      else
        println(total_count||' moved');
      end if;
    end loop;
    execute immediate stmt2 into flag;
    exit when flag = 0;
  end loop;
  commit;

exception
when others then
  rollback;
  raise;
end REBUILD_DATA_NODE;

-- REBUILD SHARD DATA
procedure REBUILD_DATA( user_name            in varchar(128),
                        table_name           in varchar(128),
                        batch_count          in bigint default 0,
                        additional_node_list in varchar(1000) default NULL )
as
  up_user_name varchar(128);
  up_table_name varchar(128);
  type name_arr_t is table of varchar(40) index by integer;
  name_arr name_arr_t;
  node_name varchar(40);
  stmt1 varchar(4000);

  type additional_node_arr_t is table of varchar(40) index by integer;
  additional_node_arr additional_node_arr_t;
  additional_node_arr_idx integer;
  additional_node_arr_length integer;
  additional_node_list_length integer;
  double_q_section integer;
  stage integer := 0;
begin
  if instr( user_name, chr(34) ) = 0 then
    up_user_name := upper( user_name );
  else
    up_user_name := replace2( user_name, chr(34) );
  end if;

  if instr( table_name, chr(34) ) = 0 then
    up_table_name := upper( table_name );
  else
    up_table_name := replace2( table_name, chr(34) );
  end if;

  stmt1 := 'SELECT distinct node_name from (';

  if additional_node_list is not null then
    additional_node_list_length := length(additional_node_list);
    double_q_section := 0;
    additional_node_arr_idx := 0;
    additional_node_arr[additional_node_arr_idx] := '';

    for i in 1 .. additional_node_list_length loop
      if instr( substr( additional_node_list, i, 1 ), chr(34) ) = 1 then
        double_q_section := abs(double_q_section-1);
      elseif instr( substr( additional_node_list, i, 1 ), chr(44) ) = 1 then
        if double_q_section = 0 then
          additional_node_arr_idx := additional_node_arr_idx+1;
          additional_node_arr[additional_node_arr_idx] := '';
          continue;
        end if;      
      end if;
      additional_node_arr[additional_node_arr_idx] := additional_Node_arr[additional_node_arr_idx]||substr( additional_node_list, i, 1 );
    end loop;

    for i in 0 .. additional_node_arr_idx loop
      additional_node_arr_length := length(additional_node_arr[i]);
      if ( instr( substr( additional_node_arr[i], 1, 1 ), chr(34) ) = 1 ) and 
         ( instr( substr( additional_node_arr[i], additional_node_arr_length, 1 ), chr(34) ) = 1 ) then
        additional_node_arr[i] := replace2( additional_node_arr[i], chr(34) );
      else
        additional_node_arr[i] := upper( additional_node_arr[i] );
      end if;
        stmt1 := stmt1||' SELECT '''||additional_node_arr[i]||''' node_name FROM dual UNION ALL';
    end loop;
  end if;

  stmt1 := stmt1||'
  SELECT n.node_name node_name 
    FROM sys_shard.objects_ o, sys_shard.ranges_ r, sys_shard.nodes_ n, sys_shard.global_meta_info_ g
   WHERE o.split_method in (''H'',''R'',''L'') and r.node_id = n.node_id and o.shard_id = r.shard_id 
     AND o.user_name = '''|| up_user_name ||''' and o.object_name = '''|| up_table_name ||''' 
     AND o.smn = g.shard_meta_number
     AND r.smn = g.shard_meta_number
     AND n.smn = g.shard_meta_number
  UNION ALL
  SELECT n.node_name
    FROM sys_shard.objects_ o, sys_shard.nodes_ n, sys_shard.global_meta_info_ g 
   WHERE o.split_method in (''H'',''R'',''L'') and o.default_node_id = n.node_id
     AND o.user_name = '''|| up_user_name ||''' and o.object_name = '''|| up_table_name ||'''
     AND o.smn = g.shard_meta_number
     AND n.smn = g.shard_meta_number';
  stmt1 := stmt1||') ORDER BY node_name';
  --println(stmt1);
  execute immediate stmt1 bulk collect into name_arr;

  if name_arr.count() = 0 then
    raise_application_error(990000, 'Non-shard objects or unsupported split method.');
  end if;

  --println('['||to_char(sysdate, 'HH24:MI:SS')||'] node count: '||name_arr.count());
  stage := 1;
  for i in name_arr.first() .. name_arr.last() loop
    node_name := name_arr[i];
    stmt1 := 'select node_name 
                from sys_shard.nodes_ n, 
                     sys_shard.global_meta_info_ g 
               where n.smn = g.shard_meta_number 
                 and node_name = '''||node_name||'''';
    execute immediate stmt1 into node_name;
  end loop;
  stage := 0;
 
  for i in name_arr.first() .. name_arr.last() loop
    println('['||to_char(sysdate, 'HH24:MI:SS')||'] target node('||i||'/'||name_arr.count()||'): "'||name_arr[i]||'"');
    rebuild_data_node(user_name, table_name, name_arr[i], batch_count);
    commit;
  end loop;
  println('['||to_char(sysdate, 'HH24:MI:SS')||'] done.');

exception
when no_data_found then
  if stage = 1 then
    println('Invalid node name: '||node_name);
  else
    raise;
  end if;
when others then
  raise;
end REBUILD_DATA;

-- PURGE OLD SHARD META INFO
procedure PURGE_OLD_SHARD_META_INFO( smn in bigint default NULL )
as
    dummy integer;
begin
    dummy := shard_unset_old_smn( smn );
end PURGE_OLD_SHARD_META_INFO;

end DBMS_SHARD;
/
