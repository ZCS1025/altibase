/***********************************************************************
 * Copyright 1999-2015, ALTIBASE Corporation or its subsidiaries.
 * All rights reserved.
 **********************************************************************/

create or replace package dbms_shard_get_diagnostics as

function get_error_code ( seqno integer ) return integer;

function get_error_message ( seqno integer ) return varchar;

function get_error_count return integer;

function get_error_node_id ( seqno integer ) return integer;

function get_error_seqnum_by_node_id ( nodeid integer ) return integer;

end dbms_shard_get_diagnostics;
/

create public synonym dbms_shard_get_diagnostics for sys.dbms_shard_get_diagnostics;
grant execute on sys.dbms_shard_get_diagnostics to public;
