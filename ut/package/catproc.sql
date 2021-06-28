/***********************************************************************
 * Copyright 1999-2015, ALTIBASE Corporation or its subsidiaries.
 * All rights reserved.
 **********************************************************************/

-- NAME
--   catproc.sql
-- DESCRIPTION
--   Run all sql scripts
-- NOTES
--   This script must be run while connected AS SYS.
--   $ALTIBASE_HOME Directory = "?"

-- dbms package
START ?/packages/dbms_application_info.sql
START ?/packages/dbms_lock.sql
START ?/packages/dbms_output.sql
START ?/packages/dbms_random.sql
START ?/packages/dbms_sql.sql
START ?/packages/dbms_stats.sql
START ?/packages/dbms_alert.sql
START ?/packages/dbms_concurrent_exec.sql
START ?/packages/dbms_recyclebin.sql
START ?/packages/dbms_utility.sql
START ?/packages/dbms_sql_plan_cache.sql

START ?/packages/dbms_application_info.plb
START ?/packages/dbms_lock.plb
START ?/packages/dbms_output.plb
START ?/packages/dbms_random.plb
START ?/packages/dbms_sql.plb
START ?/packages/dbms_stats.plb
START ?/packages/dbms_alert.plb
START ?/packages/dbms_concurrent_exec.plb
START ?/packages/dbms_recyclebin.plb
START ?/packages/dbms_utility.plb
START ?/packages/dbms_sql_plan_cache.plb

-- utility package
START ?/packages/utl_file.sql
START ?/packages/utl_raw.sql
START ?/packages/utl_tcp.sql
START ?/packages/utl_smtp.sql
START ?/packages/utl_copyswap.sql
START ?/packages/utl_shard_online_rebuild.sql

START ?/packages/utl_file.plb
START ?/packages/utl_raw.plb
START ?/packages/utl_tcp.plb
START ?/packages/utl_smtp.plb
START ?/packages/utl_copyswap.plb
START ?/packages/utl_shard_online_rebuild.plb

-- STANDARD package
START ?/packages/standard.sql
START ?/packages/dbms_standard.sql
START ?/packages/dbms_standard.plb

-- SPATIAL package
START ?/packages/sys_spatial.sql
START ?/packages/sys_spatial.plb

-- SHARD package
START ?/packages/dbms_shard.sql
START ?/packages/dbms_shard_get_diagnostics.sql

START ?/packages/dbms_shard.plb
START ?/packages/dbms_shard_get_diagnositcs.plb

-- BUG-47355
START ?/packages/sys_context.sql
START ?/packages/dbms_metadata.sql
START ?/packages/dbms_metadata_body.sql
