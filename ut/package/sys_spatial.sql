/***********************************************************************
 * Copyright 1999-2015, ALTIBASE Corporation or its subsidiaries.
 * All rights reserved.
 **********************************************************************/

CREATE OR REPLACE PACKAGE SYS_SPATIAL IS

procedure ADD_SPATIAL_REF_SYS( SRID in integer,
                               AUTH_NAME in varchar(256),
                               AUTH_SRID in integer,
                               SRTEXT in varchar(2048),
                               PROJ4TEXT in varchar(2048) );

procedure DELETE_SPATIAL_REF_SYS( SRID in integer,
                                  AUTH_NAME in varchar(256) );
end SYS_SPATIAL;
/

CREATE OR REPLACE PUBLIC SYNONYM sys_spatial FOR sys.sys_spatial;
--GRANT EXECUTE ON sys.sys_spatial TO PUBLIC;

