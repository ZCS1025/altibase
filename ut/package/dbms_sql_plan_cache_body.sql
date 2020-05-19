/***********************************************************************
 * Copyright 1999-2015, ALTIBASE Corporation or its subsidiaries.
 * All rights reserved.
 **********************************************************************/

create or replace package body dbms_sql_plan_cache
as

procedure keep_plan( sql_text_id varchar(64) )
as
a integer;
begin
a := sp_keep_plan( sql_text_id );
end;

procedure unkeep_plan( sql_text_id varchar(64) )
as
a integer;
begin
a := sp_unkeep_plan( sql_text_id );
end;

end dbms_sql_plan_cache;
/
