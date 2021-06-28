/***********************************************************************
 * Copyright 1999-2019, ALTIBASE Corporation or its subsidiaries.
 * All rights reserved.
 **********************************************************************/

create or replace package dbms_metadata authid current_user as

invalid_argval_errcode          CONSTANT INTEGER := 990010;
not_supported_obj_type_errcode  CONSTANT INTEGER := 990011;
schema_not_found_errcode        CONSTANT INTEGER := 990012;
object_not_found_errcode        CONSTANT INTEGER := 990013;
not_supported_ddl_errcode       CONSTANT INTEGER := 990014;

procedure set_transform_param(name IN VARCHAR(40), value IN CHAR(1));

procedure show_transform_params;
                    
function get_ddl(object_type IN VARCHAR(20),
                 object_name IN VARCHAR(128),
                 schema      IN VARCHAR(128) DEFAULT NULL) return varchar(65534);

function get_dependent_ddl(
                 object_type        IN VARCHAR(20),
                 base_object_name   IN VARCHAR(128),
                 base_object_schema IN VARCHAR(128) DEFAULT NULL) return varchar(65534);

function get_granted_ddl(
                 object_type IN VARCHAR(20),
                 grantee     IN VARCHAR(128) DEFAULT NULL) return varchar(65534);

function get_stats_ddl(
                 object_name IN VARCHAR(128),
                 schema      IN VARCHAR(128) DEFAULT NULL) return varchar(65534);
  
function get_user_ddl(
                 userName   IN VARCHAR(128),
                 userPasswd IN VARCHAR(128) DEFAULT NULL) return varchar(65534);

end dbms_metadata;
/

create public synonym dbms_metadata for sys.dbms_metadata;
grant execute on sys.dbms_metadata to public;
