/***********************************************************************
 * Copyright 1999-2015, ALTIBASE Corporation or its subsidiaries.
 * All rights reserved.
 **********************************************************************/

create or replace package dbms_sql_plan_cache authid current_user 
as

procedure keep_plan( sql_text_id varchar(64) );
procedure unkeep_plan( sql_text_id varchar(64) );

end dbms_sql_plan_cache;
/
