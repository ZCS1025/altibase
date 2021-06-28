/***********************************************************************
 * Copyright 1999-2015, ALTIBASE Corporation or its subsidiaries.
 * All rights reserved.
 **********************************************************************/

create or replace package body dbms_shard_get_diagnostics as

function get_error_code ( seqno integer ) return integer
as
begin
  return sp_get_error_code( seqno );
end get_error_code;

function get_error_message ( seqno integer ) return varchar
as
begin
  return sp_get_error_message( seqno );
end get_error_message;

function get_error_count return integer
as
begin
  return sp_get_error_count();
end get_error_count;

function get_error_node_id ( seqno integer ) return integer
as
begin
  return sp_get_error_node_id( seqno );
end get_error_node_id;

function get_error_seqnum_by_node_id (nodeid integer ) return integer
as
begin
  return sp_get_error_seqnum_by_node_id ( nodeid );
end get_error_seqnum_by_node_id;

end dbms_shard_get_diagnostics;
/

