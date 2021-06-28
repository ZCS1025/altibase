/***********************************************************************
 * Copyright 1999-2015, ALTIBASE Corporation or its subsidiaries.
 * All rights reserved.
 **********************************************************************/

CREATE OR REPLACE PACKAGE BODY SYS_SPATIAL IS

procedure ADD_SPATIAL_REF_SYS( SRID in integer,
                        AUTH_NAME in varchar(256),
                        AUTH_SRID in integer,
                        SRTEXT in varchar(2048),
                        PROJ4TEXT in varchar(2048) )
as
    dummy integer;
begin
    dummy := ADD_USER_SRS( SRID,
                           AUTH_NAME,
                           AUTH_SRID,
                           SRTEXT,
                           PROJ4TEXT );
end ADD_SPATIAL_REF_SYS;


procedure DELETE_SPATIAL_REF_SYS( SRID in integer,
                                  AUTH_NAME in varchar(256) )
as
    dummy integer;
begin
    dummy := DELETE_USER_SRS( SRID,
                              AUTH_NAME );
end DELETE_SPATIAL_REF_SYS;

end SYS_SPATIAL;
/
