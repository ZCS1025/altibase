/***********************************************************************
 * Copyright 1999-2019, ALTIBASE Corporation or its subsidiaries.
 * All rights reserved.
 **********************************************************************/

create or replace package body dbms_metadata as

TYPE INT_ARRAY      IS TABLE OF INTEGER       INDEX BY INTEGER;
TYPE BINT_ARRAY     IS TABLE OF BIGINT        INDEX BY INTEGER;
TYPE CHAR1_ARRAY    IS TABLE OF CHAR(1)       INDEX BY INTEGER;
TYPE CHAR10_ARRAY   IS TABLE OF VARCHAR(10)   INDEX BY INTEGER;
TYPE CHAR16_ARRAY   IS TABLE OF VARCHAR(16)   INDEX BY INTEGER;
TYPE CHAR40_ARRAY   IS TABLE OF VARCHAR(40)   INDEX BY INTEGER;
TYPE CHAR60_ARRAY   IS TABLE OF VARCHAR(60)   INDEX BY INTEGER;
TYPE CHAR128_ARRAY  IS TABLE OF VARCHAR(128)  INDEX BY INTEGER;
TYPE CHAR130_ARRAY  IS TABLE OF VARCHAR(130)  INDEX BY INTEGER;
TYPE CHAR150_ARRAY  IS TABLE OF VARCHAR(150)  INDEX BY INTEGER;
TYPE CHAR256_ARRAY  IS TABLE OF VARCHAR(256)  INDEX BY INTEGER;
TYPE CHAR512_ARRAY  IS TABLE OF VARCHAR(512)  INDEX BY INTEGER;
TYPE CHAR4000_ARRAY IS TABLE OF VARCHAR(4000) INDEX BY INTEGER;
TYPE CHAR4200_ARRAY IS TABLE OF VARCHAR(4200) INDEX BY INTEGER;

param_constraints        CHAR(1) := 'T';
param_ref_constraints    CHAR(1) := 'T';
param_segment_attributes CHAR(1) := 'T';
param_storage            CHAR(1) := 'T';
param_tablespace         CHAR(1) := 'T';
sql_terminator           CHAR(1) := '';
psm_terminator           CHAR(4) := '';
/* only for aexport */
param_notnull_constraints CHAR(1) := 'T';
param_access_mode         CHAR(1) := 'T';
param_schema              CHAR(1) := 'T';
param_export_db_mode      CHAR(1) := 'F'; /* BUG-49356 */
/* only for shard */
param_partition_orderby_name CHAR(1) := 'F';

procedure set_transform_param(name IN VARCHAR(40), value IN CHAR(1))
as
begin
    IF value NOT IN ('T', 'F') THEN
        RAISE_APPLICATION_ERROR( invalid_argval_errcode,
            'Invalid input value ' || value || ' for parameter VALUE.');
    END IF;
    
    IF name = 'CONSTRAINTS' THEN
        param_constraints := value;
    ELSIF name = 'REF_CONSTRAINTS' THEN
        param_ref_constraints := value;
    ELSIF name = 'SEGMENT_ATTRIBUTES' THEN
        param_segment_attributes := value;
    ELSIF name = 'STORAGE' THEN
        param_storage := value;
    ELSIF name = 'TABLESPACE' THEN
        param_tablespace := value;
    ELSIF name = 'SQLTERMINATOR' THEN
        IF value = 'T' THEN
            sql_terminator := ';';
            psm_terminator := chr(10) || ';' || chr(10) || '/';
        ELSE
            sql_terminator := '';
            psm_terminator := '';
        END IF;
    ELSIF name = 'NOTNULL_CONSTRAINTS' THEN
        param_notnull_constraints := value;
    ELSIF name = 'ACCESS_MODE' THEN
        param_access_mode := value;
    ELSIF name = 'SCHEMA' THEN
        param_schema := value;
    ELSIF name = 'EXPORT_DB_MODE' THEN
        param_export_db_mode := value;
    ELSIF name = 'PARTITION_ORDERBY_NAME' THEN
        param_partition_orderby_name := value;
    ELSE
        RAISE_APPLICATION_ERROR( invalid_argval_errcode,
            'Invalid input value ' || name || ' for parameter NAME.');
    END IF;
end set_transform_param;

procedure show_transform_params
as
begin
    PRINTLN('CONSTRAINTS        : ' || param_constraints);
    PRINTLN('REF_CONSTRAINTS    : ' || param_ref_constraints);
    PRINTLN('SEGMENT_ATTRIBUTES : ' || param_segment_attributes);
    PRINTLN('STORAGE            : ' || param_storage);
    PRINTLN('TABLESPACE         : ' || param_tablespace);
    PRINTLN('SQLTERMINATOR      : ' || NVL2(sql_terminator, 'T', 'F'));
end show_transform_params;
 
procedure get_schema_info(
                  schema      IN  VARCHAR(128),
                  userId      OUT INTEGER,
                  userName    OUT VARCHAR(128) )
as
begin
    SELECT USER_ID, USER_NAME
    INTO userId, userName
    FROM SYSTEM_.SYS_USERS_
    WHERE USER_NAME = NVL(schema, USER_NAME());
    
    EXCEPTION
    WHEN NO_DATA_FOUND THEN
        RAISE_APPLICATION_ERROR( schema_not_found_errcode,
            'SCHEMA "' || schema || '" not found.' );
end get_schema_info;

function get_columns(tableId IN INTEGER) return varchar(65534)
as
constCnt   INTEGER;
columns    CHAR128_ARRAY;
types      CHAR40_ARRAY;
storeTypes CHAR10_ARRAY;
policies   CHAR16_ARRAY;
defaults   CHAR4000_ARRAY;
columnDdl  varchar(65534);
sSRID      INTEGER;
begin
    SELECT /*+ USE_HASH( C, D ) */
        C.COLUMN_NAME,
        NVL2(F.COLUMN_ID, 'TIMESTAMP',
            CASE2(D.CREATE_PARAM is null OR (C.DATA_TYPE=6 AND C.PRECISION=38), D.TYPE_NAME,
            D.CREATE_PARAM = 'precision',
                DECODE(D.DATA_TYPE, 60, 'CHAR', 61, 'VARCHAR', D.TYPE_NAME) || '(' || C.PRECISION || ')',
            D.TYPE_NAME || '(' || C.PRECISION || ',' || C.SCALE || ')')), 
        C.DEFAULT_VAL,
        DECODE(C.STORE_TYPE, 'V', ' VARIABLE', ' FIXED'),
        E.POLICY_NAME
      BULK COLLECT INTO columns, types, defaults, storeTypes, policies
    FROM SYSTEM_.SYS_COLUMNS_ C, V$DATATYPE D, SYSTEM_.SYS_ENCRYPTED_COLUMNS_ E,
        (SELECT COLUMN_ID 
            FROM SYSTEM_.SYS_CONSTRAINT_COLUMNS_ A,
                SYSTEM_.SYS_CONSTRAINTS_ B
            WHERE A.CONSTRAINT_ID=B.CONSTRAINT_ID
            AND B.CONSTRAINT_TYPE=5
            AND B.TABLE_ID=tableId) F
    WHERE C.DATA_TYPE = D.DATA_TYPE
      AND C.TABLE_ID = tableId
      AND C.TABLE_ID = E.TABLE_ID(+) AND C.COLUMN_ID = E.COLUMN_ID(+)
      AND C.COLUMN_ID = F.COLUMN_ID(+)
      AND C.IS_HIDDEN = 'F'
    ORDER BY COLUMN_ORDER;
    
    FOR n IN columns.FIRST() .. columns.LAST() LOOP
        IF n > 1 THEN
            columnDdl := columnDdl || ',' || chr(10);
        END IF;

        columnDdl := columnDdl || ' "' || columns[n] || '" ' || types[n];

        /* BUG-48323 Support SRID */
        IF types[n] LIKE 'GEOMETRY%' THEN
            SELECT G.SRID INTO sSRID
            FROM GEOMETRY_COLUMNS G, 
                 SYSTEM_.SYS_TABLES_ T, 
                 SYSTEM_.SYS_USERS_ U
            WHERE T.TABLE_ID = tableId
            AND T.USER_ID = U.USER_ID
            AND G.F_TABLE_SCHEMA = U.USER_NAME
            AND G.F_TABLE_NAME = T.TABLE_NAME
            AND G.F_GEOMETRY_COLUMN = columns[n];

                columnDdl := columnDdl || ' SRID ' || sSRID;
                /*
            IF ( sSRID IS NOT NULL ) AND ( sSRID > 0 ) THEN
                columnDdl := columnDdl || ' SRID ' || sSRID;
            END IF;
            */
        END IF;

        IF policies[n] IS NOT NULL
        THEN
            columnDdl := columnDdl || ' ENCRYPT USING ''' || policies[n] || '''';
        END IF;

        columnDdl := columnDdl || storeTypes[n];

        IF defaults[n] IS NOT NULL
        THEN
            columnDdl := columnDdl || ' DEFAULT ' || defaults[n];
        END IF;

        SELECT COUNT(*) INTO constCnt                      
        FROM SYSTEM_.SYS_COLUMNS_ C,
          SYSTEM_.SYS_CONSTRAINT_COLUMNS_ CC,
          SYSTEM_.SYS_CONSTRAINTS_ CO
        WHERE CO.TABLE_ID = tableId
          AND C.COLUMN_NAME = columns[n]
          AND C.COLUMN_ID = CC.COLUMN_ID
          AND CC.CONSTRAINT_ID = CO.CONSTRAINT_ID
          AND CO.CONSTRAINT_TYPE = 1;

        IF constCnt > 0 THEN
            columnDdl := columnDdl || ' NOT NULL';
        END IF;
     
    END LOOP;
  
    return columnDdl;
  
end get_columns;

function get_index_columns(indexId IN INTEGER) return VARCHAR(32000)
as
sortOrders   CHAR1_ARRAY;
keyColumns   CHAR130_ARRAY;
columnDdl    VARCHAR(32000);
begin
    SELECT 
        DECODE(C.IS_HIDDEN, 'T', C.DEFAULT_VAL, '"' || C.COLUMN_NAME || '"'),
        A.SORT_ORDER
        BULK COLLECT INTO keyColumns, sortOrders
    FROM
        SYSTEM_.SYS_INDEX_COLUMNS_ A,
        SYSTEM_.SYS_INDICES_       B,
        SYSTEM_.SYS_COLUMNS_       C
    WHERE a.INDEX_ID        = b.INDEX_ID
        and a.COLUMN_ID     = c.COLUMN_ID
        and b.INDEX_ID      = indexId
    ORDER BY a.INDEX_COL_ORDER;

    FOR n IN keyColumns.FIRST() .. keyColumns.LAST() LOOP
        IF n > 1 THEN
            columnDdl := columnDdl || ', ';
        END IF;
        
        columnDdl := columnDdl || keyColumns[n];
        
        IF sortOrders[n] = 'D' THEN
            columnDdl := columnDdl || ' DESC';
        END IF;
    END LOOP;

    return columnDdl;

end get_index_columns;

function get_index_logging(indexId   IN INTEGER) return VARCHAR(20)
as
ddlStr VARCHAR(20);
begin
    SELECT A.LOGGING
    INTO ddlStr
    FROM
    (SELECT DECODE(IS_CREATED_WITH_LOGGING, 'T', '',
        ' NOLOGGING' || DECODE(IS_CREATED_WITH_FORCE, 'T', ' FORCE', ' NOFORCE'))
        AS LOGGING
    FROM V$DISK_BTREE_HEADER
    WHERE INDEX_ID = indexId
    UNION
    SELECT DECODE(IS_CREATED_WITH_LOGGING, 'T', '',
        ' NOLOGGING' || DECODE(IS_CREATED_WITH_FORCE, 'T', ' FORCE', ' NOFORCE'))
        AS LOGGING
    FROM V$DISK_RTREE_HEADER
    WHERE INDEX_ID = indexId) A;

    return ddlStr;

    EXCEPTION
    WHEN NO_DATA_FOUND THEN
        return null;
end get_index_logging;

function get_index_physical_attributes(indexId IN INTEGER) return VARCHAR(32000)
as
initTrans      INTEGER;
maxTranss      INTEGER;
initExt        INTEGER;
nextExt        INTEGER;
minExt         INTEGER;
maxExt         VARCHAR(10);
ddlStr         VARCHAR(32000);
begin
    SELECT INIT_TRANS, MAX_TRANS, INITEXTENTS, NEXTEXTENTS, MINEXTENTS, MAXEXT
        INTO initTrans, maxTranss, initExt, nextExt, minExt, maxExt
    FROM (
        SELECT INIT_TRANS, MAX_TRANS,
            INITEXTENTS, NEXTEXTENTS,
            MINEXTENTS, DECODE(MAXEXTENTS, -1, 'UNLIMITED', MAXEXTENTS) AS MAXEXT
        FROM V$DISK_BTREE_HEADER
        WHERE INDEX_ID = indexId
        UNION
        SELECT INIT_TRANS, MAX_TRANS,
            INITEXTENTS, NEXTEXTENTS,
            MINEXTENTS, DECODE(MAXEXTENTS, -1, 'UNLIMITED', MAXEXTENTS) AS MAXEXT
        FROM V$DISK_RTREE_HEADER
        WHERE INDEX_ID = indexId
        ) A;

    ddlStr := ddlStr || chr(10) || 'INITRANS ' || initTrans || ' MAXTRANS ' || maxTranss;

    IF param_storage = 'T' THEN
        ddlStr := ddlStr || chr(10) || 'STORAGE(' ||  'INITEXTENTS ' || initExt ||
                  ' NEXTEXTENTS ' || nextExt ||
                  ' MINEXTENTS ' || minExt ||
                  ' MAXEXTENTS ' || maxExt || ')';
    END IF;

    return ddlStr;

    EXCEPTION
    WHEN NO_DATA_FOUND THEN
        return null;
end get_index_physical_attributes;

function get_index_partitioning(indexId IN INTEGER,
                                tbsId   IN INTEGER) return VARCHAR(32000)
as
n            INTEGER;
m            INTEGER;
idxPartNames CHAR128_ARRAY;
partNames    CHAR128_ARRAY;
tbsNames     CHAR128_ARRAY;
ddlStr       VARCHAR(32000);
begin
    ddlStr := '';
    SELECT /*+ USE_HASH(A, C) */
           A.INDEX_PARTITION_NAME,
           B.PARTITION_NAME,
           C.NAME
        BULK COLLECT INTO idxPartNames, partNames, tbsNames
    FROM SYSTEM_.SYS_INDEX_PARTITIONS_ A,
         SYSTEM_.SYS_TABLE_PARTITIONS_ B,
         V$TABLESPACES C
    WHERE A.INDEX_ID = indexId
          AND A.TABLE_PARTITION_ID = B.PARTITION_ID
          AND A.TBS_ID = C.ID
          AND (A.TBS_ID != B.TBS_ID /* BUG-48873 partition index와 partition의 tbs가 다를 때 */
               OR A.INDEX_PARTITION_NAME NOT LIKE '__SYS_PART_IDX_ID_%')
          AND B.PARTITION_NAME IS NOT NULL
    ORDER BY A.INDEX_PARTITION_ID;
    m := 0;
    ddlStr := ddlStr || ' LOCAL';
    FOR n IN idxPartNames.FIRST() .. idxPartNames.LAST() LOOP
        m := 1;
        IF n = 1 THEN
            ddlStr := ddlStr || ' (' || chr(10);
        ELSIF n > 1 THEN
            ddlStr := ddlStr || ',' || chr(10);
        END IF;
        ddlStr := ddlStr || ' PARTITION "' || idxPartNames[n] || '" ON "' ||
                        partNames[n] || '" TABLESPACE "' || tbsNames[n] || '"';
    END LOOP;
    IF m = 1 THEN
        ddlStr := ddlStr || ')';
    END IF;

    return ddlStr;
end get_index_partitioning;

function get_unique_clause(constId       IN INTEGER,
                           tbsId         IN INTEGER,
                           isInAlter     IN BOOLEAN) return varchar(32000)
as
n             INTEGER;
m             INTEGER;
indexId       INTEGER;
tbsType       INTEGER;
indexTbsId    INTEGER;
isPartitioned CHAR(1);
directKey     VARCHAR(10);
isPersistent  VARCHAR(20);
tbsClause     VARCHAR(70);
sortOrders    CHAR1_ARRAY;
keyColumns    CHAR128_ARRAY;
usingIndex    VARCHAR(32000);
uniqueClause  VARCHAR(32000);
begin
    uniqueClause := '(';
    
    SELECT C.COLUMN_NAME, A.SORT_ORDER
        BULK COLLECT INTO keyColumns, sortOrders
    FROM
        SYSTEM_.SYS_INDEX_COLUMNS_ A,
        SYSTEM_.SYS_CONSTRAINTS_   B,
        SYSTEM_.SYS_COLUMNS_       C
    WHERE a.INDEX_ID        = b.INDEX_ID
        and a.COLUMN_ID     = c.COLUMN_ID
        and b.CONSTRAINT_ID = constId
    ORDER BY a.INDEX_COL_ORDER;

    FOR n IN keyColumns.FIRST() .. keyColumns.LAST() LOOP
        IF n > 1 THEN
            uniqueClause := uniqueClause || ', ';
        END IF;
        
        uniqueClause := uniqueClause || '"' || keyColumns[n] || '"';
        
        IF sortOrders[n] = 'D' THEN
            uniqueClause := uniqueClause || ' DESC';
        END IF;
    END LOOP;

    SELECT /*+ USE_HASH( B, C ) */
           B.INDEX_ID,
           DECODE(B.IS_PERS, 'T', ' SET PERSISTENT=ON'),
           DECODE(B.IS_DIRECTKEY, 'T', ' DIRECTKEY', ''),
           CASE2(isInAlter = TRUE,
             ' TABLESPACE "' || C.NAME || '"',
             DECODE(B.TBS_ID, tbsId, '', ' TABLESPACE "' || C.NAME || '"')),
           B.TBS_ID,
           B.IS_PARTITIONED,
           C.TYPE
        INTO indexId, isPersistent, directKey, tbsClause, indexTbsId, isPartitioned, tbsType
    FROM SYSTEM_.SYS_CONSTRAINTS_ A, SYSTEM_.SYS_INDICES_ B,
         v$tablespaces C
    WHERE A.INDEX_ID = B.INDEX_ID
        AND B.TBS_ID = C.ID
        AND CONSTRAINT_ID = constId;

    uniqueClause := uniqueClause || ')' || isPersistent;

    -- direct key
    IF directKey IS NOT NULL THEN
        uniqueClause := uniqueClause || directKey;
    END IF;
    
    -- using index
    usingIndex := '';

    -- logging (for disk)
    IF tbsType in (3, 4) THEN
        usingIndex := get_index_logging(indexId);
    END IF;

    IF param_segment_attributes = 'T' THEN
        IF param_tablespace = 'T' AND
           isPartitioned = 'F' THEN
            usingIndex := usingIndex || tbsClause;
        ELSIF isPartitioned = 'T' THEN
            usingIndex := usingIndex || get_index_partitioning(indexId, indexTbsId);
        END IF;
    END IF;

    IF length(usingIndex) > 0 THEN
        uniqueClause := uniqueClause || chr(10) || ' USING INDEX' || usingIndex;
    END IF;

    return uniqueClause;
  
    EXCEPTION
    WHEN NO_DATA_FOUND THEN
        return null;
end get_unique_clause;

function get_references_clause(constId IN INTEGER) return varchar(32000)
as
n          INTEGER;
refTableId INTEGER;
refIndexId INTEGER;
keyColumns CHAR128_ARRAY;
validated  VARCHAR(11);
deleteRule VARCHAR(20);
refSchema  VARCHAR(128);
refTable   VARCHAR(128);
refClause  VARCHAR(32000);
begin
    refClause := '(';
    
    SELECT REFERENCED_TABLE_ID, REFERENCED_INDEX_ID,
           DECODE(DELETE_RULE, 1, ' ON DELETE CASCADE', 2, ' ON DELETE SET NULL'),
           DECODE(VALIDATED, 'F', ' NOVALIDATE')
        INTO refTableId, refIndexId, deleteRule, validated
    FROM SYSTEM_.SYS_CONSTRAINTS_
    WHERE CONSTRAINT_ID = constId;
        
    SELECT 
        C.COLUMN_NAME
        BULK COLLECT INTO keyColumns
    FROM
        SYSTEM_.SYS_CONSTRAINT_COLUMNS_ A,
        SYSTEM_.SYS_CONSTRAINTS_   B,
        SYSTEM_.SYS_COLUMNS_       C
    WHERE a.CONSTRAINT_ID   = b.CONSTRAINT_ID
        and a.COLUMN_ID     = c.COLUMN_ID
        and b.CONSTRAINT_ID = constId
    ORDER BY a.CONSTRAINT_COL_ORDER;

    FOR n IN keyColumns.FIRST() .. keyColumns.LAST() LOOP
        IF n > 1 THEN
            refClause := refClause || ', ';
        END IF;
        refClause := refClause || '"' || keyColumns[n] || '"';
    END LOOP;

    refClause := refClause || ')' || chr(10) || ' REFERENCES ';
    
    SELECT USER_NAME, TABLE_NAME 
        INTO refSchema, refTable
    FROM SYSTEM_.SYS_INDICES_ A,
        SYSTEM_.SYS_TABLES_ B, SYSTEM_.SYS_USERS_ C
    WHERE A.INDEX_ID = refIndexId
        AND A.TABLE_ID = refTableId
        AND A.TABLE_ID = B.TABLE_ID
        AND A.USER_ID = C.USER_ID;
    
    refClause := refClause || '"' || refSchema || '"."' || refTable || '" (';
    
    SELECT 
        B.COLUMN_NAME
        BULK COLLECT INTO keyColumns
    FROM
        SYSTEM_.SYS_INDEX_COLUMNS_ A,
        SYSTEM_.SYS_COLUMNS_       B
    WHERE A.COLUMN_ID  = B.COLUMN_ID
        and A.INDEX_ID = refIndexId
    ORDER BY a.INDEX_COL_ORDER;

    FOR n IN keyColumns.FIRST() .. keyColumns.LAST() LOOP
        IF n > 1 THEN
            refClause := refClause || ', ';
        END IF;
        refClause := refClause || '"' || keyColumns[n] || '"';
    END LOOP;
    
    refClause := refClause || ')' || deleteRule || validated;
    
    return refClause;
  
    EXCEPTION
    WHEN NO_DATA_FOUND THEN
        return refClause;
end get_references_clause;

function get_check_clause(constId IN INTEGER) return varchar(4000)
as
checkCond  VARCHAR(4000);
begin
    SELECT CHECK_CONDITION
        INTO checkCond
    FROM SYSTEM_.SYS_CONSTRAINTS_
    WHERE CONSTRAINT_ID = constId;
    
    return checkCond;
    
    EXCEPTION
    WHEN NO_DATA_FOUND THEN
        return null;
end get_check_clause;

function get_partitions(tableId IN INTEGER) return VARCHAR(65534)
as
rowMovement        VARCHAR(1);
partitionMethod    VARCHAR(16);
partitionCondition VARCHAR(20);
partIds            INT_ARRAY;
tbsIds             INT_ARRAY;
keyColumns         CHAR128_ARRAY;
tbsNames           CHAR40_ARRAY;
partAccesses       CHAR16_ARRAY;
partitioningClause CHAR4200_ARRAY;
partDdl            VARCHAR(65534);
begin
    SELECT DECODE(PARTITION_METHOD, 0, 'RANGE', 1, 'HASH', 2, 'LIST', 3, 'RANGE_USING_HASH'),
            DECODE(PARTITION_METHOD, 0, 'VALUES LESS THAN', 1, '', 2, 'VALUES', 3, 'VALUES LESS THAN'),
            ROW_MOVEMENT
            INTO partitionMethod, partitionCondition, rowMovement
    FROM SYSTEM_.SYS_PART_TABLES_ 
    WHERE TABLE_ID=tableId;

    partDdl := partDdl || 'PARTITION BY ' || partitionMethod || '(';

    SELECT B.COLUMN_NAME
        BULK COLLECT INTO keyColumns
    FROM SYSTEM_.SYS_PART_KEY_COLUMNS_ A,
        SYSTEM_.SYS_COLUMNS_ B
    WHERE A.COLUMN_ID=B.COLUMN_ID AND
        A.PARTITION_OBJ_ID=B.TABLE_ID AND
        A.OBJECT_TYPE = 0 AND
        B.TABLE_ID=tableId
    ORDER BY A.PART_COL_ORDER;

    FOR m IN keyColumns.FIRST() .. keyColumns.LAST() LOOP
        IF m > 1 THEN
            partDdl := partDdl || ', ';
        END IF;
        partDdl := partDdl|| '"' || keyColumns[m] || '"' ;
    END LOOP;

    partDdl := partDdl || ')' || chr(10) || '(' || chr(10);
    
    IF param_partition_orderby_name = 'T' THEN
      SELECT /*+ USE_HASH(P, D) */
        P.PARTITION_ID,
        CASE2(partitionMethod='HASH', ' PARTITION "' || P.PARTITION_NAME || '"',
            P.PARTITION_MAX_VALUE IS NULL, ' PARTITION "' || P.PARTITION_NAME || '" VALUES DEFAULT',
            ' PARTITION "' || P.PARTITION_NAME || '" ' || partitionCondition || ' (' || P.PARTITION_MAX_VALUE || ')'),
        P.TBS_ID, DECODE(D.ID, T.TBS_ID, NULL, D.NAME),
        DECODE(P.PARTITION_ACCESS, 'R', ' READ ONLY',
                               'W', '',
                               'A', ' READ APPEND')
      BULK COLLECT INTO partIds, partitioningClause, tbsIds, tbsNames, partAccesses
      FROM SYSTEM_.SYS_TABLE_PARTITIONS_ P,
        SYSTEM_.SYS_TABLES_ T, V$TABLESPACES D
      WHERE P.TBS_ID=D.ID
        AND P.TABLE_ID=T.TABLE_ID
        AND P.TABLE_ID=tableId
        AND P.PARTITION_NAME IS NOT NULL
      ORDER BY P.PARTITION_NAME;
    ELSE
      SELECT /*+ USE_HASH(P, D) */
        P.PARTITION_ID,
        CASE2(partitionMethod='HASH', ' PARTITION "' || P.PARTITION_NAME || '"',
            P.PARTITION_MAX_VALUE IS NULL, ' PARTITION "' || P.PARTITION_NAME || '" VALUES DEFAULT',
            ' PARTITION "' || P.PARTITION_NAME || '" ' || partitionCondition || ' (' || P.PARTITION_MAX_VALUE || ')'),
        P.TBS_ID, DECODE(D.ID, T.TBS_ID, NULL, D.NAME),
        DECODE(P.PARTITION_ACCESS, 'R', ' READ ONLY',
                               'W', '',
                               'A', ' READ APPEND')
      BULK COLLECT INTO partIds, partitioningClause, tbsIds, tbsNames, partAccesses
      FROM SYSTEM_.SYS_TABLE_PARTITIONS_ P,
        SYSTEM_.SYS_TABLES_ T, V$TABLESPACES D
      WHERE P.TBS_ID=D.ID
        AND P.TABLE_ID=T.TABLE_ID
        AND P.TABLE_ID=tableId
        AND P.PARTITION_NAME IS NOT NULL
      ORDER BY P.PARTITION_ID;
    END IF;
    
    -- table partition description
    FOR m IN partIds.FIRST() .. partIds.LAST() LOOP
        IF m > 1 THEN
            partDdl := partDdl || ',' || chr(10);
        END IF;
        
        partDdl := partDdl || partitioningClause[m];
        
        IF param_segment_attributes = 'F' THEN
            GOTO LABEL_ACCESS_MODE;
        END IF;

        -- tablespace
        IF param_tablespace = 'T' AND tbsNames[m] IS NOT NULL THEN
            partDdl := partDdl || ' TABLESPACE "' || tbsNames[m] || '"';
        END IF;
        
        -- lob column properties
        DECLARE
        CURSOR C1 IS 
            SELECT /*+ USE_HASH(B, C) */
                A.COLUMN_NAME AS COL_NAME, C.NAME AS TBS_NAME,
                DECODE(B.LOGGING, 'T', 'LOGGING', 'NOLOGGING') AS LOGGING,
                DECODE(B.BUFFER, 'T', ' BUFFER', ' NOBUFFER') AS BUFFER
            FROM SYSTEM_.SYS_COLUMNS_ A,
                 SYSTEM_.SYS_LOBS_ L,
                 SYSTEM_.SYS_PART_LOBS_ B,
                 V$TABLESPACES C
            WHERE A.TABLE_ID  = B.TABLE_ID
                  AND A.COLUMN_ID = B.COLUMN_ID
                  AND B.COLUMN_ID = L.COLUMN_ID
                  AND B.TBS_ID = C.ID
                  AND B.PARTITION_ID = partIds[m]
                  AND B.TABLE_ID = tableId
            /* BUG-48873 partition LOB 저장 tbs가 partition의 tbs와도 LOB tbs와도 다들 때 */
                  AND B.TBS_ID != tbsIds[m]
                  AND B.TBS_ID != L.TBS_ID
                  AND A.IS_HIDDEN = 'F'
            ORDER BY B.COLUMN_ID;
            TAB_REC C1%ROWTYPE;
        BEGIN
            OPEN C1;
            LOOP
                FETCH C1 INTO TAB_REC;
                EXIT WHEN C1%NOTFOUND;
                
                partDdl := partDdl || chr(10) || ' LOB ("' || TAB_REC.COL_NAME || 
                        '") STORE AS (';

                IF param_tablespace = 'T' THEN
                    partDdl := partDdl || chr(10) || '  TABLESPACE "' || TAB_REC.TBS_NAME || '" ';
                END IF;

                partDdl := partDdl || TAB_REC.LOGGING || TAB_REC.BUFFER || ')';
            END LOOP;
            CLOSE C1;
        END;
        
        << LABEL_ACCESS_MODE >>
        -- access mode
        IF param_access_mode = 'T' AND partAccesses[m] IS NOT NULL THEN
            partDdl := partDdl || chr(10) || partAccesses[m];
        END IF;
    END LOOP;

    partDdl := partDdl || chr(10) || ')';
    
    --rowMovement : 'T' or 'F'
    IF rowMovement = 'T' THEN
        partDdl := partDdl || chr(10) || 'ENABLE ROW MOVEMENT';
    END IF;
    
    return partDdl;
    
end get_partitions;

function get_storage(tableId IN INTEGER) return varchar(256)
as
initExt   BIGINT;
nextExt   BIGINT;
minExt    BIGINT;
maxExt    BIGINT;
storageDdl VARCHAR(256);
begin
    SELECT INITEXTENTS, NEXTEXTENTS, MINEXTENTS, MAXEXTENTS
        INTO initExt, nextExt, minExt, maxExt
    FROM SYSTEM_.SYS_TABLES_
    WHERE TABLE_ID = tableId;
    
    storageDdl := chr(10) || 'STORAGE(INITEXTENTS ' || initExt || ' NEXTEXTENTS ' || nextExt ||
                  ' MINEXTENTS ' || minExt || ' MAXEXTENTS ' || maxExt || ')';
    
    return storageDdl;
    
    EXCEPTION
    WHEN NO_DATA_FOUND THEN
        return null;
end get_storage;

function get_log_compression(tableId IN INTEGER) return varchar(32)
as
compressionDdl VARCHAR(32);
begin
    SELECT DECODE(B.COMPRESSED_LOGGING, 0, chr(10) || 'UNCOMPRESSED LOGGING',
            chr(10) || 'COMPRESSED LOGGING')
        INTO compressionDdl
    FROM SYSTEM_.SYS_TABLES_ A, 
        (SELECT TABLE_OID, COMPRESSED_LOGGING
         FROM V$DISKTBL_INFO
         UNION
         SELECT TABLE_OID, COMPRESSED_LOGGING
         FROM V$MEMTBL_INFO) B
    WHERE A.TABLE_OID = B.TABLE_OID
        AND TABLE_ID = tableId;
    
    return compressionDdl;
    
    EXCEPTION
    WHEN NO_DATA_FOUND THEN
        return null;
end get_log_compression;

function get_temp_table_attr(tempMode IN CHAR(1)) return varchar(32)
as
attrClause VARCHAR(32);
begin
    return attrClause;
end get_temp_table_attr;

function get_table_compression(tableId IN INTEGER) return varchar(32000)
as
n               INTEGER;
columns         CHAR128_ARRAY;
compressionDdl  VARCHAR(32000);
begin
    SELECT '"' || C.COLUMN_NAME || '"' || CASE2(A.MAXROWS > 0, ' MAXROWS ' || A.MAXROWS, '')
      BULK COLLECT INTO columns
    FROM SYSTEM_.SYS_COMPRESSION_TABLES_ A, SYSTEM_.SYS_COLUMNS_ C
    WHERE A.TABLE_ID = tableId
      AND A.COLUMN_ID = C.COLUMN_ID
    ORDER BY C.COLUMN_ORDER;
    
    FOR n IN columns.FIRST() .. columns.LAST() LOOP
        IF n = 1 THEN
            compressionDdl := chr(10) || 'COMPRESS (' || chr(10);
        ELSE
            compressionDdl := compressionDdl || ',' || chr(10);        
        END IF;

        compressionDdl := compressionDdl || '     ' || columns[n];
    END LOOP;
    
    IF LENGTH(compressionDdl) > 0 THEN
        compressionDdl := compressionDdl || ')';
    END IF;
  
    return compressionDdl;
  
end get_table_compression;

function get_lob_storage(tableId IN INTEGER) return varchar(32000)
as
loggingStr      CHAR10_ARRAY;
bufferStr       CHAR10_ARRAY;
columns         CHAR128_ARRAY;
tbsNames        CHAR128_ARRAY;
compressionDdl  VARCHAR(32000);
begin
    SELECT /*+ USE_HASH(C, D) */
        B.COLUMN_NAME, D.NAME,
        DECODE(C.LOGGING, 'T', 'LOGGING', 'NOLOGGING'),
        DECODE(C.BUFFER, 'T', ' BUFFER', ' NOBUFFER')
      BULK COLLECT INTO columns, tbsNames, loggingStr, bufferStr
    FROM  SYSTEM_.SYS_COLUMNS_ B, SYSTEM_.SYS_LOBS_ C, V$TABLESPACES D
    WHERE B.COLUMN_ID = C.COLUMN_ID
      AND C.TBS_ID = D.ID
      AND C.IS_DEFAULT_TBS = 'T'
      AND B.IS_HIDDEN = 'F'
      AND C.TABLE_ID = tableId
    ORDER BY B.COLUMN_ORDER;
    
    compressionDdl := '';
    FOR n IN columns.FIRST() .. columns.LAST() LOOP
        compressionDdl := compressionDdl || chr(10) || 'LOB ("' || columns[n] ||
                        '") STORE AS (';

        IF param_tablespace = 'T' THEN
            compressionDdl := compressionDdl || chr(10) || ' TABLESPACE "' || tbsNames[n] || '" ';
        END IF;

        compressionDdl := compressionDdl || loggingStr[n] || bufferStr[n] || ')';

    END LOOP;
  
    return compressionDdl;
  
end get_lob_storage;

function get_constraints(tableId IN INTEGER,
                         tbsId   IN INTEGER) return varchar(65534)
as
n           INTEGER;
isInAlter   BOOLEAN;
constIds    INT_ARRAY;
constTypes  INT_ARRAY;
indexIds    INT_ARRAY;
constNames  CHAR150_ARRAY;
tabConst    VARCHAR(65534);
begin
    -- The primary key must precede foreign keys.
    SELECT CONSTRAINT_ID,
        CASE2(CONSTRAINT_NAME LIKE '%__SYS_CON_%', '', ' CONSTRAINT "' || CONSTRAINT_NAME || '"'),
        CONSTRAINT_TYPE, INDEX_ID
        BULK COLLECT INTO constIds, constNames, constTypes, indexIds
    FROM SYSTEM_.SYS_CONSTRAINTS_
    WHERE TABLE_ID = tableId
        AND CONSTRAINT_TYPE IN (0, 2, 3, 6, 7)
    ORDER BY CONSTRAINT_TYPE DESC;
    
    isInAlter := FALSE;
    tabConst := '';
    FOR n IN constTypes.FIRST() .. constTypes.LAST() LOOP
        IF constTypes[n] = 0 THEN
            IF param_ref_constraints = 'T' THEN
                tabConst := tabConst || ',' || chr(10) || constNames[n] || ' FOREIGN KEY ' ||
                            get_references_clause(constIds[n]);
            END IF;
        ELSIF constTypes[n] = 2 THEN
            IF param_constraints = 'T' THEN
                tabConst := tabConst || ',' || chr(10) || constNames[n] || ' UNIQUE ' ||
                            get_unique_clause(constIds[n], tbsId, isInAlter);
            END IF;
        ELSIF constTypes[n] = 3 THEN            
            IF param_constraints = 'T' THEN
                tabConst := tabConst || ',' || chr(10) || constNames[n] || ' PRIMARY KEY ' ||
                            get_unique_clause(constIds[n], tbsId, isInAlter);
            END IF;
        ELSIF constTypes[n] = 6 THEN
            IF param_constraints = 'T' THEN
                tabConst := tabConst || ',' || chr(10) || constNames[n] || ' LOCALUNIQUE ' ||
                            get_unique_clause(constIds[n], tbsId, isInAlter);
            END IF;
        ELSIF constTypes[n] = 7 THEN
            IF param_constraints = 'T' THEN
                tabConst := tabConst || ',' || chr(10) || constNames[n] || ' CHECK (' ||
                            get_check_clause(constIds[n]) || ')';
            END IF;
        END IF;
    END LOOP;

    return tabConst;
  
    EXCEPTION
    WHEN NO_DATA_FOUND THEN
        return null;
end get_constraints;

/* BUG-49356 Cross-schema reference index */
function get_object_clause_force(
                  object_name   IN VARCHAR(128),
                  userName      IN VARCHAR(128),
                  isWithschema  IN BOOLEAN) return varchar(270)
as
objectClause VARCHAR(270);
begin
    objectClause := '';
    IF isWithSchema = TRUE THEN
        objectClause := '"' || userName || '".';
    END IF;
    objectClause := objectClause || '"' || object_name || '"';

    return objectClause;
end get_object_clause_force;

function get_object_clause(
                  object_name IN VARCHAR(128),
                  userName    IN VARCHAR(128)) return varchar(270)
as
begin
    return get_object_clause_force(object_name, userName, (param_schema = 'T'));
end get_object_clause;

function get_table_ddl(
                  object_name IN VARCHAR(128),
                  userId      IN INTEGER,
                  userName    IN VARCHAR(128)) return varchar(65534)
as
n              INTEGER;
m              INTEGER;
tableId        INTEGER;
tbsId          INTEGER;
tbsType        INTEGER;
pctFree        INTEGER;
pctUsed        INTEGER;
initTrans      INTEGER;
maxTranss      INTEGER;
shardFlag      INTEGER;
isPartitioned  CHAR(1);
isTemporary    CHAR(1);
maxRow         VARCHAR(40);
parallelDegree INTEGER;
accessMode     VARCHAR(12);
tbsName        VARCHAR(40);
ddlStr         varchar(65534);
begin
    SELECT /*+ USE_HASH( A, B ) */
            A.TABLE_ID,
            DECODE(A.MAXROW, 0, '', 'MAXROWS '||A.MAXROW),
            TBS_ID,
            A.PCTFREE, A.PCTUSED, A.INIT_TRANS, A.MAX_TRANS,
            A.PARALLEL_DEGREE,
            A.IS_PARTITIONED, A.TEMPORARY,
            DECODE(A.ACCESS, 'R', ' READ ONLY', 'W', '', 'A', ' READ APPEND'),
            NUMAND(SHARD_FLAG,7),
            B.TYPE, B.NAME
        INTO tableId, maxRow, tbsId, pctFree, pctUsed, initTrans, maxTranss,
             parallelDegree, isPartitioned, isTemporary, accessMode, shardFlag, tbsType, tbsName
    FROM SYSTEM_.SYS_TABLES_ A, V$TABLESPACES B
    WHERE A.USER_ID = userId
        AND A.TBS_ID = B.ID
        AND A.TABLE_TYPE ='T'
--        AND A.TEMPORARY = 'N'
        AND A.HIDDEN = 'N'
        AND A.TABLE_NAME = object_name;

    ddlStr := 'CREATE' || DECODE(isTemporary, 'N', '', ' TEMPORARY') || ' TABLE ' ||
              get_object_clause(object_name, userName) || chr(10) || '(';
    
    -- column definition
    ddlStr := ddlStr || chr(10) || get_columns(tableId);

    -- table Constraints
    ddlStr := ddlStr || get_constraints(tableId, tbsId);

    ddlStr := ddlStr || chr(10) || ')';

    -- MAXROWS or PARTITION
    IF isPartitioned = 'T' THEN
        ddlStr := ddlStr || chr(10) || get_partitions(tableId);
    ELSE
        IF maxRow IS NOT NULL THEN
            ddlStr := ddlStr || chr(10) || maxRow;
        END IF;
    END IF;
    
    -- temporary attributes (only for temporary table)
    IF isTemporary != 'N' THEN
        ddlStr := ddlStr || chr(10) || 'ON COMMIT ' ||
                  DECODE(isTemporary, 'P', 'PRESERVE ', 'DELETE ') || 'ROWS' ||
                  chr(10) || 'TABLESPACE "'|| tbsName || '"' ||
                  get_log_compression(tableId);
        return ddlStr || sql_terminator;
    END IF;

    -- access mode
    IF param_access_mode = 'T' AND accessMode IS NOT NULL THEN
        ddlStr := ddlStr || chr(10) || accessMode;
    END IF;

    IF param_segment_attributes = 'T' THEN
        -- tablespace
        IF param_tablespace = 'T' THEN
            IF tbsName IS NOT NULL THEN
                ddlStr := ddlStr || chr(10) || 'TABLESPACE "'|| tbsName || '"';
            END IF;
        END IF;
        
        -- physical attributes (only for disk tablespace)
        IF tbsType in (3, 4) THEN
            ddlStr := ddlStr || chr(10) || 'PCTFREE ' || pctFree || ' PCTUSED ' || pctUsed ||
                      ' INITRANS ' || initTrans || ' MAXTRANS ' || maxTranss;
            -- storage
            IF param_storage = 'T' THEN
                ddlStr := ddlStr || get_storage(tableId);
            END IF;
        END IF;
    END IF;
            
    -- log compression
    ddlStr := ddlStr || get_log_compression(tableId);
    
    -- logging : logging info. does not exist in meta.
    
    -- parallel
    IF parallelDegree > 1 THEN
        ddlStr := ddlStr || chr(10) || 'PARALLEL ' || parallelDegree;
    END IF;
    
    -- table compression
    ddlStr := ddlStr || get_table_compression(tableId);
    
    -- lob column properties
    IF param_segment_attributes = 'T' THEN
        ddlStr := ddlStr || get_lob_storage(tableId);
    END IF;
    
    -- shard_flag
    IF shardFlag = 2 THEN
        ddlStr := ddlStr || ' SHARD BACKUP';
    END IF;
    
    return ddlStr || sql_terminator;
end get_table_ddl;

function get_constraint_ddl(
                  object_name IN VARCHAR(128),
                  userId      IN INTEGER,
                  userName    IN VARCHAR(128)) return varchar(65534)
as
isInAlter        BOOLEAN;
n                INTEGER;
constId          INTEGER;
constType        INTEGER;
indexId          INTEGER;
tbsId            INTEGER;
tableOwner       VARCHAR(128);
tableName        VARCHAR(128);
ddlStr           varchar(65534);
begin
    SELECT A.CONSTRAINT_ID, A.CONSTRAINT_TYPE, A.INDEX_ID,
           U.USER_NAME, B.TABLE_NAME, B.TBS_ID
        INTO constId, constType, indexId, tableOwner, tableName, tbsId
    FROM SYSTEM_.SYS_CONSTRAINTS_ A, SYSTEM_.SYS_TABLES_ B, SYSTEM_.SYS_USERS_ U
    WHERE A.TABLE_ID = B.TABLE_ID
          AND A.USER_ID = U.USER_ID
          AND A.USER_ID = userId
          AND A.CONSTRAINT_NAME = object_name;

    ddlStr := 'ALTER TABLE ' || get_object_clause(tableName, tableOwner) ||
              ' ADD CONSTRAINT "' ;

    isInAlter := TRUE;
    IF constType = 0 THEN
        ddlStr := ddlStr || object_name || '" FOREIGN KEY ' ||
                    get_references_clause(constId);
    ELSIF constType = 2 THEN
        ddlStr := ddlStr || object_name || '" UNIQUE ' ||
                    get_unique_clause(constId, tbsId, isInAlter);
    ELSIF constType = 3 THEN            
        ddlStr := ddlStr || object_name || '" PRIMARY KEY ' ||
                    get_unique_clause(constId, tbsId, isInAlter);
    ELSIF constType = 6 THEN
        ddlStr := ddlStr || object_name || '" LOCALUNIQUE ' ||
                    get_unique_clause(constId, tbsId, isInAlter);
    ELSIF constType = 7 THEN
        ddlStr := ddlStr || object_name || '" CHECK (' ||
                    get_check_clause(constId) || ')';
    END IF;

    return ddlStr || sql_terminator;
end get_constraint_ddl;

function get_index_ddl(
                  object_name IN VARCHAR(128),
                  userId      IN INTEGER,
                  userName    IN VARCHAR(128)) return varchar(65534)
as
n                INTEGER;
m                INTEGER;
indexId          INTEGER;
tableTbsId       INTEGER;
tbsType          INTEGER;
isPartitioned    CHAR(1);
parallelDegree   INTEGER;
uniqueClause     VARCHAR(12);
directkeyClause  VARCHAR(10);
indexType        VARCHAR(20);
tableOwner       VARCHAR(128);
tableName        VARCHAR(128);
accessMode       VARCHAR(12);
tbsName          VARCHAR(40);
isPersistent     VARCHAR(20);
ddlStr           varchar(65534);
tblSchemaObjName varchar(261);
idxSchemaObjName varchar(261);
begin
    SELECT /*+ USE_HASH( A, C ) */
            A.INDEX_ID,
            DECODE(A.INDEX_TYPE, 1, 'INDEXTYPE IS BTREE', 2, 'INDEXTYPE IS RTREE'),
            DECODE(A.IS_PARTITIONED, 'F', DECODE(A.IS_UNIQUE, 'T', ' UNIQUE', ''),
                DECODE(P.IS_LOCAL_UNIQUE, 'T', ' LOCALUNIQUE', '')),
            DECODE(A.IS_PERS, 'T', 'SET PERSISTENT=ON'),
            DECODE(A.IS_DIRECTKEY, 'T', 'DIRECTKEY'),
            A.IS_PARTITIONED,
            UT.USER_NAME,
            B.TABLE_NAME,
            B.TBS_ID,
            C.NAME, C.TYPE
        INTO indexId, indexType, uniqueClause, isPersistent, directkeyClause,
             isPartitioned, tableOwner, tableName, tableTbsId, tbsName, tbsType
    FROM SYSTEM_.SYS_INDICES_ A, SYSTEM_.SYS_PART_INDICES_ P,
         SYSTEM_.SYS_TABLES_ B, V$TABLESPACES C, SYSTEM_.SYS_USERS_ U,
         SYSTEM_.SYS_USERS_ UT
    WHERE A.USER_ID = userId
        AND A.INDEX_ID = P.INDEX_ID(+)
        AND A.TABLE_ID = B.TABLE_ID
        AND A.TBS_ID = C.ID AND A.USER_ID = U.USER_ID
        AND B.USER_ID = UT.USER_ID
        AND A.INDEX_NAME = object_name;

   /* 
    *  BUG-49356 DB mode export에서 ALL_CRT_INDEX.sql는 connect sys/manager로 접속하게 된다.
    *  따라서, SYS 이외의 스키마를 가진 객체는 모두 명시해야 한다.
    *  SYS만 예외로 하는 이유는 최대한 하위 호환성을 지키기 위해서다.
    */
    -- index schema
    IF param_export_db_mode = 'T' AND userName != 'SYS' THEN
        idxSchemaObjName := get_object_clause_force(object_name, userName, TRUE);
    ELSE
        idxSchemaObjName := get_object_clause(object_name, userName);
    END IF;

    -- table schema
    IF param_export_db_mode = 'T' AND tableOwner != 'SYS' THEN
        tblSchemaObjName := get_object_clause_force(tableName, tableOwner, TRUE);
    ELSE
        tblSchemaObjName := get_object_clause(tableName, tableOwner);
    END IF;
    
    ddlStr := 'CREATE' || uniqueClause || ' INDEX ' ||
              idxSchemaObjName || ' ON ' ||
              tblSchemaObjName || ' (';
    
    -- key columns
    ddlStr := ddlStr || get_index_columns(indexId) || ')';

    -- index_partitioning_clause
    IF param_segment_attributes = 'T' AND param_tablespace = 'T' THEN
        IF isPartitioned = 'T' THEN
            ddlStr := ddlStr || get_index_partitioning(indexId, tableTbsId);
        END IF;
    END IF;

    -- domain_index_clause (indextype)
    IF indexType IS NOT NULL THEN
        ddlStr := ddlStr || chr(10) || indexType;
    END IF;

    IF isPersistent IS NOT NULL THEN
        ddlStr := ddlStr || chr(10) || isPersistent;
    END IF;

    -- directkey_clause (for memory)
    IF directkeyClause IS NOT NULL THEN
        ddlStr := ddlStr || chr(10) || directkeyClause;
    END IF;

    -- tablespace
--    IF param_segment_attributes = 'T' THEN
        IF param_tablespace = 'T' AND
           isPartitioned = 'F' AND tbsName IS NOT NULL THEN
            ddlStr := ddlStr || chr(10) || 'TABLESPACE "'|| tbsName || '"';
        END IF;

        -- parallel
        IF tbsType in (3, 4) THEN
            -- logging (for disk)
            ddlStr := ddlStr || get_index_logging(indexId);

            -- physical_attribute_clause storage_clause (for disk)
--            ddlStr := ddlStr || get_index_physical_attributes(indexId);
        END IF;
--    END IF;

    return ddlStr || sql_terminator;
end get_index_ddl;

function get_psm_ddl(
                    objectType  IN INTEGER,
                    object_name IN VARCHAR(128),
                    userId      IN INTEGER,
                    userName    IN VARCHAR(128)) return varchar(65534)
as
n              INTEGER;
errMsg         VARCHAR(128);
ddlStr         varchar(65534);
begin
    n := 0;
    ddlStr := '';
    DECLARE
    CURSOR C1 IS
        SELECT 
            A.PARSE AS PARSE
        FROM
            SYSTEM_.SYS_PROC_PARSE_ A,
            SYSTEM_.SYS_PROCEDURES_ B
        WHERE A.USER_ID = userId
            AND A.PROC_OID = B.PROC_OID
            AND B.OBJECT_TYPE = objectType
            AND B.PROC_NAME = object_name
        ORDER BY A.SEQ_NO;
        TAB_REC C1%ROWTYPE;
    BEGIN
        OPEN C1;
        LOOP
            FETCH C1 INTO TAB_REC;
            EXIT WHEN C1%NOTFOUND;
            n := 1;
            ddlStr := ddlStr || TAB_REC.parse;
        END LOOP;
        CLOSE C1;
    END;
    
    IF n = 0 THEN
        RAISE NO_DATA_FOUND;
    END IF;
    
    return RTRIM(ddlStr, chr(10) || ' ') || psm_terminator;
end get_psm_ddl;

function get_package_parse(
                    object_type IN INTEGER,
                    object_name IN VARCHAR(128),
                    userName    IN VARCHAR(128),
                    userId      IN INTEGER DEFAULT NULL) return varchar(65534)
as
n              INTEGER;
ddlStr         varchar(65534);
begin
    n := 0;
    ddlStr := '';

    DECLARE
    CURSOR C1 IS
        SELECT 
            A.PARSE AS PARSE
        FROM
            SYSTEM_.SYS_PACKAGE_PARSE_ A,
            SYSTEM_.SYS_PACKAGES_ B
        WHERE A.USER_ID = userId
            AND A.PACKAGE_OID  = B.PACKAGE_OID
            AND B.PACKAGE_TYPE = object_type
            AND B.PACKAGE_NAME = object_name
            AND B.USER_ID = userId
        ORDER BY A.SEQ_NO;
        TAB_REC C1%ROWTYPE;
    BEGIN
        OPEN C1;
        LOOP
            FETCH C1 INTO TAB_REC;
            EXIT WHEN C1%NOTFOUND;
            n := 1;
            ddlStr := ddlStr || TAB_REC.parse;
        END LOOP;
        CLOSE C1;
    END;
    
    IF n = 0 THEN
        RAISE NO_DATA_FOUND;
    END IF;
    
    return RTRIM(ddlStr, ' ' || chr(10)) || psm_terminator;
end get_package_parse;

function get_package_ddl(
                    object_type IN INTEGER,
                    object_name IN VARCHAR(128),
                    userId      IN INTEGER,
                    userName    IN VARCHAR(128)) return varchar(65534)
as
ddlStr         varchar(65534);
begin
    IF object_type IN (6, 7) THEN
        ddlStr := get_package_parse(object_type, object_name, userName, userId);
    ELSE
        ddlStr := get_package_parse(6, object_name, userName, userId);
        ddlStr := ddlStr || chr(10) ||
                  get_package_parse(7, object_name, userName, userId);
    END IF;

    return ddlStr;
end get_package_ddl;

function get_view_ddl(
                  object_name IN VARCHAR(128),
                  userId      IN INTEGER,
                  userName    IN VARCHAR(128)) return varchar(65534)
as
n              INTEGER;
tableId        INTEGER;
ddlStr         varchar(65534);
begin
    n := 0;
    ddlStr := '';
    DECLARE
    CURSOR C1 IS
        SELECT 
            A.PARSE AS PARSE
        FROM
            SYSTEM_.SYS_VIEW_PARSE_ A,
            SYSTEM_.SYS_TABLES_     B
        WHERE A.USER_ID = userId
            AND A.VIEW_ID    = B.TABLE_ID
            AND B.TABLE_TYPE = 'V'
            AND B.TABLE_NAME = object_name
        ORDER BY A.SEQ_NO;
        TAB_REC C1%ROWTYPE;
    BEGIN
        OPEN C1;
        LOOP
            FETCH C1 INTO TAB_REC;
            EXIT WHEN C1%NOTFOUND;
            n := 1;
            ddlStr := ddlStr || TAB_REC.parse;
        END LOOP;
        CLOSE C1;
    END;
    
    IF n = 0 THEN
        RAISE NO_DATA_FOUND;
    END IF;
    
    IF sql_terminator IS NOT NULL THEN
        return RTRIM(ddlStr, ' ' || chr(10)) || chr(10) || sql_terminator;
    ELSE
        return ddlStr;
    END IF;
end get_view_ddl;

function get_mview_ddl(
                  object_name IN VARCHAR(128),
                  userId      IN INTEGER,
                  userName    IN VARCHAR(128)) return varchar(65534)
as
n              INTEGER;
tableId        INTEGER;
ddlStr         varchar(65534);
begin
    n := 0;
    ddlStr := '';
    DECLARE
    CURSOR C1 IS
        SELECT 
            A.PARSE AS PARSE
        FROM
            SYSTEM_.SYS_VIEW_PARSE_ A,
            SYSTEM_.SYS_MATERIALIZED_VIEWS_ B
        WHERE A.USER_ID = userId
            AND A.VIEW_ID = B.VIEW_ID
            AND B.MVIEW_NAME = object_name
        ORDER BY A.SEQ_NO;
        TAB_REC C1%ROWTYPE;
    BEGIN
        OPEN C1;
        LOOP
            FETCH C1 INTO TAB_REC;
            EXIT WHEN C1%NOTFOUND;
            n := 1;
            ddlStr := ddlStr || TAB_REC.parse;
        END LOOP;
        CLOSE C1;
    END;
    
    IF n = 0 THEN
        RAISE NO_DATA_FOUND;
    END IF;
    
    IF sql_terminator IS NOT NULL THEN
        return RTRIM(ddlStr, ' ' || chr(10)) || chr(10) || sql_terminator;
    ELSE
        return ddlStr;
    END IF;
end get_mview_ddl;

function get_trigger_ddl(
                  object_name IN VARCHAR(128),
                  userId      IN INTEGER,
                  userName    IN VARCHAR(128)) return varchar(65534)
as
n              INTEGER;
tableId        INTEGER;
ddlStr         varchar(65534);
begin
    n := 0;
    ddlStr := '';
    DECLARE
    CURSOR C1 IS
        SELECT 
            A.SUBSTRING AS text
        FROM
            SYSTEM_.SYS_TRIGGER_STRINGS_ A,
            SYSTEM_.SYS_TRIGGERS_ B
        WHERE B.USER_ID = userId
            AND A.TRIGGER_OID  = B.TRIGGER_OID
            AND B.TRIGGER_NAME = object_name
        ORDER BY A.SEQNO;
        TAB_REC C1%ROWTYPE;
    BEGIN
        OPEN C1;
        LOOP
            FETCH C1 INTO TAB_REC;
            EXIT WHEN C1%NOTFOUND;
            n := 1;
            ddlStr := ddlStr || TAB_REC.text;
        END LOOP;
        CLOSE C1;
    END;
    
    IF n = 0 THEN
        RAISE NO_DATA_FOUND;
    END IF;
    
    return RTRIM(ddlStr, ' ' || chr(10)) || psm_terminator;
end get_trigger_ddl;

function get_dblink_ddl(
                  object_name IN VARCHAR(128),
                  userId      IN INTEGER,
                  userName    IN VARCHAR(128)) return varchar(65534)
as
userMode       VARCHAR(128);
remoteUser     VARCHAR(130);
targetName     VARCHAR(40);
ddlStr         varchar(65534);
begin
    IF userId = 0 THEN
        SELECT 'PUBLIC',
           NVL(REMOTE_USER_ID, '[user]'),
           TARGET_NAME
        INTO userMode, remoteUser, targetName
        FROM SYSTEM_.SYS_DATABASE_LINKS_
        WHERE USER_ID IS NULL
            AND USER_MODE = 0
            AND LINK_NAME = object_name;
    ELSE
        SELECT DECODE(USER_MODE, 0, 'PUBLIC', 'PRIVATE'),
           NVL(REMOTE_USER_ID, '[user]'),
           TARGET_NAME
        INTO userMode, remoteUser, targetName
        FROM SYSTEM_.SYS_DATABASE_LINKS_
        WHERE USER_ID = userId
            AND LINK_NAME = object_name;
    END IF;

    ddlStr := 'CREATE ' || userMode || ' DATABASE LINK "' || object_name || '"' || chr(10) ||
              ' CONNECT TO "' || remoteUser || '" IDENTIFIED BY [password]' || chr(10) ||
              ' USING "' || targetName || '"';

    return ddlStr || sql_terminator;
end get_dblink_ddl;
                    
function get_role_ddl(object_name IN VARCHAR(128)) return varchar(65534)
as
userId         INTEGER;
ddlStr         varchar(65534);
begin
    SELECT USER_ID INTO userId
    FROM SYSTEM_.SYS_USERS_
    WHERE USER_NAME = object_name
        AND USER_TYPE = 'R';
        
    ddlStr := 'CREATE ROLE "' || object_name || '"' || sql_terminator;

    return ddlStr;
end get_role_ddl;

function get_user_ddl(
                 userName   IN VARCHAR(128),
                 userPasswd IN VARCHAR(128) DEFAULT NULL) return varchar(65534)
as
n              INTEGER;
userId         INTEGER;
tcpStr         VARCHAR(15);
defaultTbs     VARCHAR(40);
tempTbs        VARCHAR(40);
passwordParam  VARCHAR(400);
accessTbs      CHAR60_ARRAY;
ddlStr         varchar(65534);
begin
    SELECT
        A.USER_ID,
        DECODE(A.PASSWORD_LIMIT_FLAG, 'T', 'LIMIT (' ||
            chr(10) || ' FAILED_LOGIN_ATTEMPTS ' ||
                DECODE(A.FAILED_LOGIN_ATTEMPTS, 0, 'UNLIMITED', A.FAILED_LOGIN_ATTEMPTS) || ',' ||
            chr(10) || ' PASSWORD_LOCK_TIME ' ||
                DECODE(A.PASSWORD_LOCK_TIME, 0, 'UNLIMITED', A.PASSWORD_LOCK_TIME) || ',' ||
            chr(10) || ' PASSWORD_LIFE_TIME ' ||
                DECODE(A.PASSWORD_LIFE_TIME, 0, 'UNLIMITED', A.PASSWORD_LIFE_TIME) || ',' ||
            chr(10) || ' PASSWORD_GRACE_TIME ' ||
                DECODE(A.PASSWORD_GRACE_TIME, 0, 'UNLIMITED', A.PASSWORD_GRACE_TIME) || ',' ||
            chr(10) || ' PASSWORD_REUSE_TIME ' ||
                DECODE(A.PASSWORD_REUSE_TIME, 0, 'UNLIMITED', A.PASSWORD_REUSE_TIME) || ',' ||
            chr(10) || ' PASSWORD_REUSE_MAX ' ||
                DECODE(A.PASSWORD_REUSE_MAX, 0, 'UNLIMITED', A.PASSWORD_REUSE_MAX) || ',' ||
            chr(10) || ' PASSWORD_VERIFY_FUNCTION ' ||
                NVL2(A.PASSWORD_VERIFY_FUNCTION, '"' || A.PASSWORD_VERIFY_FUNCTION || '"', 'NULL') ||
            ')', ''),
        DECODE(A.DISABLE_TCP, 'T', 'DISABLE TCP', ''),
        (SELECT NAME FROM V$TABLESPACES WHERE ID = A.DEFAULT_TBS_ID),
        (SELECT NAME FROM V$TABLESPACES WHERE ID = A.TEMP_TBS_ID)
        INTO userId, passwordParam, tcpStr, defaultTbs, tempTbs
    FROM SYSTEM_.SYS_USERS_ A
    WHERE A.USER_NAME = userName
        AND A.USER_TYPE = 'U';

    ddlStr := 'CREATE USER "' || userName || '" IDENTIFIED BY ';
    
    IF userPasswd IS NULL THEN
        ddlStr := ddlStr || '[password]';
    ELSE
        ddlStr := ddlStr || '"' || userPasswd || '"';
    END IF;

    IF passwordParam IS NOT NULL THEN
        ddlStr := ddlStr || chr(10) || passwordParam;
    END IF;

    IF tempTbs IS NOT NULL THEN
        ddlStr := ddlStr || chr(10) || 'TEMPORARY TABLESPACE "' || tempTbs || '"';
    END IF;

    IF defaultTbs IS NOT NULL THEN
        ddlStr := ddlStr || chr(10) || 'DEFAULT TABLESPACE "' || defaultTbs || '"';
    END IF;

    -- access
    SELECT /*+ USE_HASH( A, B ) */
        'ACCESS "' || B.NAME || '" ' || DECODE(A.IS_ACCESS, 0, 'OFF', 'ON') 
    BULK COLLECT INTO accessTbs
    FROM SYSTEM_.SYS_TBS_USERS_ A, V$TABLESPACES B
    WHERE A.TBS_ID = B.ID
        AND A.USER_ID = userId;

    FOR n IN accessTbs.FIRST() .. accessTbs.LAST() LOOP
        ddlStr := ddlStr || chr(10) || accessTbs[n];
    END LOOP;

    IF tcpStr IS NOT NULL THEN
        ddlStr := ddlStr || chr(10) || tcpStr;
    END IF;

    return ddlStr || sql_terminator;
end get_user_ddl;

function get_job_ddl(object_name IN VARCHAR(128)) return varchar(65534)
as
userId         INTEGER;
isEnable       CHAR(1);
intervalVal    VARCHAR(40);
startTime      VARCHAR(70);
endTime        VARCHAR(70);
execQry        VARCHAR(1000);
commentStr     VARCHAR(4000);
ddlStr         varchar(65534);
begin
    SELECT EXEC_QUERY, TO_CHAR(START_TIME, 'YYYY/MM/DD HH24:MI:SS'),
           DECODE(END_TIME, NULL, '', TO_CHAR(END_TIME, 'YYYY/MM/DD HH24:MI:SS')),
           DECODE(INTERVAL_TYPE, 'YY', ' INTERVAL ' || INTERVAL || ' YEAR',
                                 'MM', ' INTERVAL ' || INTERVAL || ' MONTH',
                                 'DD', ' INTERVAL ' || INTERVAL || ' DAY',
                                 'HH', ' INTERVAL ' || INTERVAL || ' HOUR',
                                 'MI', ' INTERVAL ' || INTERVAL || ' MINUTE'),
           IS_ENABLE, COMMENT
        INTO execQry, startTime, endTime, intervalVal, isEnable, commentStr
    FROM SYSTEM_.SYS_JOBS_
    WHERE JOB_NAME = object_name;

    ddlStr := 'CREATE JOB "' || object_name || '"' || chr(10) ||
              ' EXEC ' || execQry || chr(10) ||
              ' START TO_DATE(''' || startTime || ''', ''YYYY/MM/DD HH24:MI:SS'')';

    IF endTime IS NOT NULL THEN
        ddlStr := ddlStr || chr(10) || ' END TO_DATE(''' || endTime || ''', ''YYYY/MM/DD HH24:MI:SS'')';
    END IF;

    IF intervalVal IS NOT NULL THEN
        ddlStr := ddlStr || chr(10) || intervalVal;
    END IF;

    IF isEnable = 'T' THEN
        ddlStr := ddlStr || chr(10) || ' ENABLE';
    END IF;

    IF commentStr IS NOT NULL THEN
        ddlStr := ddlStr || chr(10) || ' COMMENT ''' || REPLACE2(commentStr, '''', '''''') || '''';
    END IF;

    return ddlStr || sql_terminator;
end get_job_ddl;

function get_sequence_ddl(
                  object_name IN VARCHAR(128),
                  userId      IN INTEGER,
                  userName    IN VARCHAR(128)) return varchar(65534)
as
startVal       VARCHAR(40);
incrementVal   VARCHAR(40);
cacheSize      VARCHAR(40);
minVal         VARCHAR(40);
maxVal         VARCHAR(40);
isCycle        VARCHAR(6);
flag2          VARCHAR(20);
ddlStr         varchar(65534);
begin
    SELECT /*+ USE_HASH(A, B) NO_PLAN_CACHE */
           NVL2(CURRENT_SEQ, CURRENT_SEQ+INCREMENT_SEQ, START_SEQ),
           DECODE(INCREMENT_SEQ, 1, '', ' INCREMENT BY ' || INCREMENT_SEQ),
           DECODE(SYNC_INTERVAL, 0, ' NOCACHE', 20, '', ' CACHE ' || SYNC_INTERVAL),
           DECODE(MAX_SEQ, 9223372036854775806, '', ' MAXVALUE ' || MAX_SEQ),
           DECODE(MIN_SEQ, 1, '', ' MINVALUE ' || MIN_SEQ),
           DECODE(NUMAND(FLAG, 16), 16, ' CYCLE', ''),
           DECODE(NUMAND(FLAG, 32), 32, ' ENABLE SYNC TABLE', '')
        INTO startVal, incrementVal, cacheSize, maxVal, minVal, isCycle, flag2
    FROM SYSTEM_.SYS_TABLES_ A, X$SEQ B
    WHERE A.USER_ID = userId
        AND A.TABLE_NAME = object_name
        AND A.TABLE_OID = B.SEQ_OID;

    ddlStr := 'CREATE SEQUENCE ' || get_object_clause(object_name, userName);

    IF startVal != 1 THEN
        ddlStr := ddlStr || ' START WITH ' || startVal;
    END IF;
    
    ddlStr := ddlStr ||
              incrementVal ||
              minVal ||
              maxVal ||
              cacheSize ||
              isCycle ||
              flag2;

    return ddlStr || sql_terminator;
end get_sequence_ddl;

function get_synonym_ddl(
                  synonymName IN VARCHAR(128),
                  userId      IN INTEGER,
                  userName    IN VARCHAR(128)) return varchar(65534)
as
objectOwner    VARCHAR(128);
objectName     VARCHAR(128);
ddlStr         varchar(65534);
begin
    IF userId = 0 THEN
        SELECT OBJECT_OWNER_NAME, OBJECT_NAME
            INTO objectOwner, objectName
        FROM SYSTEM_.SYS_SYNONYMS_
        WHERE SYNONYM_OWNER_ID IS NULL
            AND SYNONYM_NAME = synonymName;

        ddlStr := 'CREATE PUBLIC SYNONYM "' || synonymName;
    ELSE
        SELECT OBJECT_OWNER_NAME, OBJECT_NAME
            INTO objectOwner, objectName
        FROM SYSTEM_.SYS_SYNONYMS_
        WHERE SYNONYM_OWNER_ID = userId
            AND SYNONYM_NAME = synonymName;

        ddlStr := 'CREATE SYNONYM "' || userName || '"."' || synonymName;
    END IF;

    ddlStr := ddlStr || '" FOR "' || objectOwner || '"."' || objectName || '"';

    return ddlStr || sql_terminator;
end get_synonym_ddl;

function get_library_ddl(
                  object_name IN VARCHAR(128),
                  userId      IN INTEGER,
                  userName    IN VARCHAR(128)) return varchar(65534)
as
fileSpec       VARCHAR(4000);
ddlStr         varchar(65534);
begin
    SELECT FILE_SPEC
        INTO fileSpec
    FROM SYSTEM_.SYS_LIBRARIES_
    WHERE USER_ID = userId
        AND LIBRARY_NAME = object_name;

    ddlStr := 'CREATE LIBRARY "' || userName || '"."' || object_name ||
              '" AS ''' || fileSpec || '''';

    return ddlStr || sql_terminator;
end get_library_ddl;

function get_directory_ddl(object_name IN VARCHAR(128)) return varchar(65534)
as
dirPath        VARCHAR(4000);
ddlStr         varchar(65534);
begin
    SELECT DIRECTORY_PATH
        INTO dirPath
    FROM SYSTEM_.SYS_DIRECTORIES_
    WHERE DIRECTORY_NAME = object_name;

    ddlStr := 'CREATE DIRECTORY "' || object_name ||
              '" AS ''' || dirPath || '''';

    return ddlStr || sql_terminator;
end get_directory_ddl;

function get_mem_tbs(tbsId IN INTEGER) return varchar(32000)
as
currSize         BIGINT;
tbsName          VARCHAR(512);
autoextendClause VARCHAR(512);
splitClause      VARCHAR(50);
checkpointPath   VARCHAR(32000);
ddlStr           varchar(65534);
begin
    SELECT SPACE_NAME,
           CURRENT_SIZE/1024,
           DECODE(AUTOEXTEND_MODE, 0, 'AUTOEXTEND OFF',
               'AUTOEXTEND ON NEXT ' || AUTOEXTEND_NEXTSIZE/1024 || 'K MAXSIZE ' ||
               DECODE(MAXSIZE, 0, 'UNLIMITED', MAXSIZE/1024 || 'K')),
           'SPLIT EACH ' || DBFILE_SIZE/1024 || 'K',
           (SELECT GROUP_CONCAT('''' || CHECKPOINT_PATH || '''', ',' || chr(10))
                        FROM V$MEM_TABLESPACE_CHECKPOINT_PATHS
                        WHERE SPACE_ID=tbsId)
        INTO tbsName, currSize, autoextendClause, splitClause, checkpointPath
    FROM V$MEM_TABLESPACES
    WHERE SPACE_ID = tbsId;

    ddlStr := 'CREATE MEMORY TABLESPACE "' || tbsName || '"' || chr(10) ||
              'SIZE ' || currSize || 'K' || chr(10) ||
              autoextendClause || chr(10) ||
              'CHECKPOINT PATH ' || checkpointPath || chr(10) ||
              splitClause;

    return ddlStr || sql_terminator;
end get_mem_tbs;

function get_data_files(tbsId    IN INTEGER,
                        pageSize IN INTEGER) return varchar(32000)
as
n                INTEGER;
files            CHAR256_ARRAY;
sizeClause       CHAR40_ARRAY;
autoextendClause CHAR128_ARRAY;
ddlStr           varchar(32000);
begin
    SELECT NAME,
           'SIZE ' || INITSIZE*pageSize || 'K',
           DECODE(AUTOEXTEND, 1, 'AUTOEXTEND ON NEXT ' || NEXTSIZE*pageSize ||
                      'K MAXSIZE ' || MAXSIZE*pageSize || 'K',
                  CASE2(CURRSIZE > INITSIZE, 'AUTOEXTEND ON MAXSIZE ' || CURRSIZE*pageSize || 'K',
                      'AUTOEXTEND OFF'))
        BULK COLLECT INTO files, sizeClause, autoextendClause
    FROM V$DATAFILES
    WHERE SPACEID = tbsId
    ORDER BY ID;

    ddlStr := '';
    FOR n IN files.FIRST() .. files.LAST() LOOP
        IF n > 1 THEN
            ddlStr := ddlStr || ',' || chr(10);
        END IF;

        ddlStr := ddlStr || '''' || files[n] || '''' || chr(10) ||
                  sizeClause[n] || chr(10) || autoextendClause[n];
    END LOOP;

    return ddlStr;
end get_data_files;

function get_vol_tbs(tbsId IN INTEGER) return varchar(32000)
as
initSize         BIGINT;
tbsName          VARCHAR(512);
autoextendClause VARCHAR(512);
ddlStr           varchar(65534);
begin
    SELECT SPACE_NAME,
           INIT_SIZE/1024,
           DECODE(AUTOEXTEND_MODE, 0, 'AUTOEXTEND OFF',
               'AUTOEXTEND ON NEXT ' || NEXT_SIZE/1024 || 'K MAXSIZE ' ||
               DECODE(MAX_SIZE, 0, 'UNLIMITED', MAX_SIZE/1024 || 'K'))
        INTO tbsName, initSize, autoextendClause
    FROM V$VOL_TABLESPACES
    WHERE SPACE_ID = tbsId;

    ddlStr := 'CREATE VOLATILE TABLESPACE "' || tbsName || '"' || chr(10) ||
              'SIZE ' || initSize || 'K' || chr(10) ||
              autoextendClause || sql_terminator;

    return ddlStr;
end get_vol_tbs;

function get_alter_tbs(tbsId     IN INTEGER,
                       tbsName   IN VARCHAR(128),
                       fileType  IN VARCHAR(10),
                       pageSize  IN INTEGER) return varchar(32000)
as
n                INTEGER;
fileIds          INT_ARRAY;
files            CHAR256_ARRAY;
sizeClause       CHAR40_ARRAY;
autoextendClause CHAR128_ARRAY;
alterStr         VARCHAR(128);
ddlStr           varchar(32000);
begin
    SELECT ID, NAME,
           'SIZE ' || INITSIZE*pageSize || 'K',
           DECODE(AUTOEXTEND, 1, 'AUTOEXTEND ON NEXT ' || NEXTSIZE*pageSize ||
                      'K MAXSIZE ' || MAXSIZE*pageSize || 'K',
                  CASE2(CURRSIZE > INITSIZE, 'AUTOEXTEND ON MAXSIZE ' || CURRSIZE*pageSize || 'K',
                      'AUTOEXTEND OFF'))
        BULK COLLECT INTO fileIds, files, sizeClause, autoextendClause
    FROM V$DATAFILES
    WHERE SPACEID = tbsId
    ORDER BY ID;

    alterStr := 'ALTER TABLESPACE "' || tbsName || '"' || chr(10);
    ddlStr := '';
    FOR n IN files.FIRST() .. files.LAST() LOOP
        IF n > 1 THEN
            ddlStr := ddlStr || sql_terminator || chr(10);
        END IF;

        IF fileIds[n] = 0 THEN
            ddlStr := ddlStr || alterStr || 'ALTER ' || fileType ||
                      ' ''' || files[n] || '''' || chr(10) || sizeClause[n] ||
                      sql_terminator || chr(10) || chr(10);
            ddlStr := ddlStr || alterStr || 'ALTER ' || fileType ||
                      ' ''' || files[n] || '''' || chr(10) || autoextendClause[n];
        ELSE
            ddlStr := ddlStr || alterStr || 'ADD ' || fileType || ' ''' || files[n] || '''' ||
                      chr(10) || sizeClause[n] || ' ' || autoextendClause[n];
        END IF;
    END LOOP;

    return ddlStr || sql_terminator;
end get_alter_tbs;

function get_tablespace_ddl(object_name IN VARCHAR(128)) return varchar(65534)
as
tbsId          INTEGER;
tbsType        INTEGER;
tbsState       INTEGER;
pageSize       INTEGER;
extentClause   VARCHAR(50);
errMsg         VARCHAR(256);
ddlStr         varchar(65534);
unsupport_ddl  EXCEPTION;
begin
    errMsg := '';
    ddlStr := '';

    SELECT ID, TYPE, STATE,
           ('EXTENTSIZE ' || EXTENT_PAGE_COUNT*PAGE_SIZE/1024 || 'K'),
           PAGE_SIZE/1024
        INTO tbsId, tbsType, tbsState, extentClause, pageSize
    FROM V$TABLESPACES
    WHERE NAME = object_name
        AND STATE IN (1,2,5,6);
    
    IF tbsType = 2 THEN
        ddlStr := get_mem_tbs(tbsId);
    ELSIF tbsType = 4 THEN
        ddlStr := 'CREATE TABLESPACE "' || object_name || '" ' || chr(10) ||
                  'DATAFILE ' || 
                  get_data_files(tbsId, pageSize) || ' ' || extentClause || sql_terminator;
    ELSIF tbsType = 6 THEN
        ddlStr := 'CREATE TEMPORARY TABLESPACE "' || object_name || '" ' || chr(10) ||
                  'TEMPFILE ' || 
                  get_data_files(tbsId, pageSize) || ' ' || extentClause || sql_terminator;
    ELSIF tbsType = 8 THEN
        ddlStr := get_vol_tbs(tbsId);
    ELSIF tbsType IN (3, 7) THEN
        ddlStr := get_alter_tbs(tbsId, object_name, 'DATAFILE', pageSize);
    ELSIF tbsType = 5 THEN
        ddlStr := get_alter_tbs(tbsId, object_name, 'TEMPFILE', pageSize);
    ELSE
        raise unsupport_ddl;
    END IF;

    IF tbsState IN (1, 5) THEN
        ddlStr := ddlStr || chr(10) || chr(10) ||
                  'ALTER TABLESPACE "' || object_name || '" OFFLINE' || sql_terminator;
    END IF;

    return ddlStr;

    EXCEPTION
    WHEN unsupport_ddl THEN
        RAISE_APPLICATION_ERROR( not_supported_ddl_errcode,
            'Not supported DDL for the specified tablespace.');
end get_tablespace_ddl;

function get_repl_hosts(object_name IN VARCHAR(128)) return varchar(16000)
as
n              INTEGER;
hostStr        CHAR256_ARRAY;
ddlStr         VARCHAR(16000);
begin
    ddlStr := '';

    SELECT DECODE(CONN_TYPE, 'TCP', '''' || HOST_IP || ''', ' || PORT_NO,
                'UNIX_DOMAIN', 'UNIX_DOMAIN',
                'IB', '''' || HOST_IP || ''', ' || PORT_NO || ' USING IB ' || IB_LATENCY, '') AS HOST
        BULK COLLECT INTO hostStr
    FROM SYSTEM_.SYS_REPL_HOSTS_
    WHERE REPLICATION_NAME = object_name
    ORDER BY HOST_NO;

    FOR n IN hostStr.FIRST() .. hostStr.LAST() LOOP
        IF n > 1 THEN
            ddlStr := ddlStr || chr(10) || ' ';
        END IF;

        ddlStr := ddlStr || hostStr[n];
    END LOOP;
    
    return ddlStr;
end get_repl_hosts;

function get_repl_items(object_name IN VARCHAR(128)) return varchar(16000)
as
n              INTEGER;
localUsers     CHAR128_ARRAY;
localTables    CHAR128_ARRAY;
localParts     CHAR128_ARRAY;
remoteUsers    CHAR128_ARRAY;
remoteTables   CHAR128_ARRAY;
remoteParts    CHAR128_ARRAY;
replUnits      CHAR1_ARRAY;
ddlStr         VARCHAR(16000);
begin
    ddlStr := '';

    SELECT distinct LOCAL_USER_NAME, LOCAL_TABLE_NAME,
            CASE2(IS_PARTITION = 'Y' AND REPLICATION_UNIT = 'P', LOCAL_PARTITION_NAME, ''),
            REMOTE_USER_NAME, REMOTE_TABLE_NAME,
            CASE2(IS_PARTITION = 'Y' AND REPLICATION_UNIT = 'P', REMOTE_PARTITION_NAME, ''),
            REPLICATION_UNIT
        BULK COLLECT INTO localUsers, localTables, localParts,
            remoteUsers, remoteTables, remoteParts, replUnits 
    FROM SYSTEM_.SYS_REPL_ITEMS_
    WHERE REPLICATION_NAME = object_name
    ORDER BY 1,2,3,4,5,6,7;

    FOR n IN localUsers.FIRST() .. localUsers.LAST() LOOP
        IF n > 1 THEN
            ddlStr := ddlStr || ',' || chr(10);
        END IF;

        IF replUnits[n] = 'T' THEN
            ddlStr := ddlStr || 'FROM "' || localUsers[n] || '"."' || localTables[n] ||
                      '" TO "' || remoteUsers[n] || '"."' || remoteTables[n] || '"';
        ELSE
            ddlStr := ddlStr || 'FROM "' || localUsers[n] || '"."' || localTables[n] ||
                      '" PARTITION "' || localParts[n] ||
                      '" TO "' || remoteUsers[n] || '"."' || remoteTables[n] ||
                      '" PARTITION "' || localParts[n] || '"';
        END IF;
    END LOOP;

    return ddlStr;
end get_repl_items;

function get_replication_ddl(object_name IN VARCHAR(128)) return varchar(65534)
as
replMode       VARCHAR(6);
replRole       VARCHAR(30);
replOptions    VARCHAR(16000);
replName       VARCHAR(40);
ddlStr         varchar(65534);
begin
    SELECT REPLICATION_NAME,
            DECODE(REPL_MODE, 2, ' EAGER', ''),
            DECODE(ROLE, 1, 'for analysis ', 2, 'for propagable logging ',
                         3, 'for propagation ', 4, 'for analysis propagation ', ''),
            DECODE(OPTIONS, 0, '', 'OPTIONS' || chr(10) ||
                   DECODE(NUMAND(OPTIONS, 128), 128, 'V6_PROTOCOL' || chr(10), '') ||
                   DECODE(NUMAND(OPTIONS, 1), 1, 'RECOVERY' || chr(10), '') ||
                   DECODE(NUMAND(OPTIONS, 2), 2, 'OFFLINE ' ||
                       (SELECT GROUP_CONCAT('''' || PATH || '''', ',')
                        FROM SYSTEM_.SYS_REPL_OFFLINE_DIR_
                        WHERE REPLICATION_NAME=object_name) || chr(10), '') ||
                   DECODE(NUMAND(OPTIONS, 4), 4, 'GAPLESS' || chr(10), '') ||
                   DECODE(NUMAND(OPTIONS, 8), 8, 'PARALLEL ' || PARALLEL_APPLIER_COUNT || chr(10), '') ||
                   DECODE(NUMAND(OPTIONS, 16), 16, 'GROUPING' || chr(10), '') ||
                   DECODE(NUMAND(OPTIONS, 64), 64, 'DDL_REPLICATE' || chr(10), '') ||
                   DECODE(NUMAND(OPTIONS, 32), 32, 'LOCAL "' || PEER_REPLICATION_NAME || '"' || chr(10), ''))
        INTO replName, replMode, replRole, replOptions
    FROM SYSTEM_.SYS_REPLICATIONS_
    WHERE REPLICATION_NAME = object_name;

    ddlStr := 'CREATE' || replMode || ' REPLICATION "' || object_name || '" ' ||
              replRole || replOptions || 'WITH';

    ddlStr := ddlStr || ' ' || get_repl_hosts(object_name);
    ddlStr := ddlStr || chr(10) || get_repl_items(object_name);

    return ddlStr || sql_terminator;
end get_replication_ddl;

function get_queue_columns(tableId IN INTEGER) return varchar(65534)
as
colCnt     int;
ddlStr     varchar(65534);
begin
    colCnt := 0;
    ddlStr := '';
    DECLARE
    CURSOR C1 IS
        SELECT ROW_NUMBER() OVER ( ORDER BY COLUMN_ORDER) AS COL_ORDER,
            PRECISION || DECODE(STORE_TYPE, 'V', ' VARIABLE', ' FIXED') AS COL
        FROM SYSTEM_.SYS_COLUMNS_
        WHERE COLUMN_ORDER < (SELECT COLUMN_ORDER 
                              FROM SYSTEM_.SYS_COLUMNS_
                              WHERE TABLE_ID = tableId AND COLUMN_NAME = 'ENQUEUE_TIME')
            AND TABLE_ID = tableId
            AND COLUMN_NAME = 'MESSAGE'
        UNION ALL
        SELECT /*+ USE_HASH( C, D ) */
            ROW_NUMBER() OVER ( ORDER BY C.COLUMN_ORDER) AS COL_ORDER,
            ' "' || C.COLUMN_NAME || '" ' ||
                CASE2(D.CREATE_PARAM is null OR (C.DATA_TYPE=6 AND C.PRECISION=38), D.TYPE_NAME,
                    D.CREATE_PARAM = 'precision', D.TYPE_NAME || '(' || C.PRECISION || ')',
                    D.TYPE_NAME || '(' || C.PRECISION || ',' || C.SCALE || ')') ||
                DECODE(C.STORE_TYPE, 'V', ' VARIABLE', ' FIXED') ||
                NVL2(C.DEFAULT_VAL, ' DEFAULT ' || C.DEFAULT_VAL, '') AS COL
        FROM SYSTEM_.SYS_COLUMNS_ C, V$DATATYPE D
        WHERE C.DATA_TYPE = D.DATA_TYPE
            AND C.TABLE_ID = tableId
            AND C.COLUMN_ORDER > (SELECT COLUMN_ORDER 
                                  FROM SYSTEM_.SYS_COLUMNS_
                                  WHERE TABLE_ID = tableId AND COLUMN_NAME = 'ENQUEUE_TIME');
        TAB_REC C1%ROWTYPE;
    BEGIN
        OPEN C1;
        LOOP
            FETCH C1 INTO TAB_REC;
            EXIT WHEN C1%NOTFOUND;
            colCnt := colCnt + 1;
            IF TAB_REC.col_order > 1 THEN
                ddlStr := ddlStr || ',' || chr(10);
            END IF;
            ddlStr := ddlStr || TAB_REC.col;
        END LOOP;
        CLOSE C1;
    END;
    
    IF colCnt > 1 THEN
        ddlStr := chr(10) || ddlStr || chr(10);
    END IF;

    return ddlStr;
end get_queue_columns;

function get_queue_ddl(
                  object_name IN VARCHAR(128),
                  userId      IN INTEGER,
                  userName    IN VARCHAR(128)) return varchar(65534)
as
n              INTEGER;
m              INTEGER;
tableId        INTEGER;
tbsType        INTEGER;
maxRow         VARCHAR(40);
tbsName        VARCHAR(40);
deleteOn       VARCHAR(1);
ddlStr         varchar(65534);
begin
    SELECT /*+ USE_HASH( A, B ) */
        A.TABLE_ID,
        DECODE(A.MAXROW, 0, '', 'MAXROWS '||A.MAXROW),
        B.TYPE, B.NAME, NVL2( Q.TABLE_OID, 'F', 'T')
        INTO tableId, maxRow, tbsType, tbsName, deleteOn
    FROM SYSTEM_.SYS_TABLES_ A 
        LEFT OUTER JOIN V$QUEUE_DELETE_OFF Q
        ON A.TABLE_OID = Q.TABLE_OID, 
        V$TABLESPACES B
    WHERE A.USER_ID = userId
        AND A.TBS_ID = B.ID
        AND A.TABLE_TYPE ='Q'
        AND A.TEMPORARY = 'N'
        AND A.HIDDEN = 'N'
        AND A.TABLE_NAME = object_name;

    ddlStr := 'CREATE QUEUE ' || get_object_clause(object_name, userName) || ' (';

    -- column definition
    ddlStr := ddlStr || get_queue_columns(tableId);

    ddlStr := ddlStr || ')';

    -- MAXROWS
    IF maxRow IS NOT NULL THEN
        ddlStr := ddlStr || chr(10) || maxRow;
    END IF;
    
    -- DELETE ON/OFF
    IF deleteOn = 'F' THEN
        ddlStr := ddlStr || chr(10) || 'DELETE OFF';
    END IF;

    IF param_segment_attributes = 'T' THEN
        -- tablespace
        IF param_tablespace = 'T' THEN
            IF tbsType != 1 THEN
                ddlStr := ddlStr || chr(10) || 'TABLESPACE "'|| tbsName || '"';
            END IF;
        END IF;
    END IF;
        
    return ddlStr || sql_terminator;
end get_queue_ddl;

function get_dep_comment(
                 base_object_name   IN VARCHAR(128),
                 userName           IN VARCHAR(128)) return varchar(65534)
as
n                INTEGER;
cnt              INTEGER;
colNames         CHAR128_ARRAY;
commentStr       CHAR4000_ARRAY;
ddlStr           varchar(65534);
begin

    SELECT COLUMN_NAME, COMMENTS
    BULK COLLECT INTO colNames, commentStr
    FROM SYSTEM_.SYS_COMMENTS_
    WHERE USER_NAME = userName
        AND TABLE_NAME = base_object_name
        AND COMMENTS IS NOT NULL;

    ddlStr := '';
    cnt := 0;
    FOR n IN colNames.FIRST() .. colNames.LAST() LOOP
        cnt := 1;
        IF n > 1 THEN
            ddlStr := ddlStr || sql_terminator || chr(10) || chr(10);
        END IF;

        IF colNames[n] IS NULL THEN
            ddlStr := ddlStr || 'COMMENT ON TABLE "' || userName || '"."' ||
                      base_object_name || '" IS ''' ||
                      REPLACE2(commentStr[n], '''', '''''') || '''';
        ELSE
            ddlStr := ddlStr || 'COMMENT ON COLUMN "' || userName || '"."' ||
                      base_object_name || '"."' || colNames[n] || '" IS ''' ||
                      REPLACE2(commentStr[n], '''', '''''') || '''';
        END IF;
    END LOOP;

    IF cnt = 0 THEN
        RAISE NO_DATA_FOUND;
    END IF;

    return ddlStr || sql_terminator;
end get_dep_comment;

function get_dep_constraint(
                 object_type        IN INTEGER,
                 base_object_name   IN VARCHAR(128),
                 userId             IN INTEGER,
                 userName           IN VARCHAR(128)) return varchar(65534)
as
isInAlter        BOOLEAN;
cnt              INTEGER;
n                INTEGER;
colName          VARCHAR(128);
alterAdd         VARCHAR(300);
alterMod         VARCHAR(300);
constIds         INT_ARRAY;
constTypes       INT_ARRAY;
indexIds         INT_ARRAY;
tbsIds           INT_ARRAY;
constNames       CHAR150_ARRAY;
tmpDdl           varchar(65534);
ddlStr           varchar(65534);
tblSchemaObjName varchar(261);
begin
    IF object_type = 0 THEN
        SELECT A.CONSTRAINT_ID,
           CASE2(A.CONSTRAINT_NAME LIKE '%__SYS_CON_%', '', ' CONSTRAINT "' || A.CONSTRAINT_NAME || '"'),
           A.CONSTRAINT_TYPE, A.INDEX_ID, B.TBS_ID
        BULK COLLECT INTO constIds, constNames, constTypes, indexIds, tbsIds
        FROM SYSTEM_.SYS_CONSTRAINTS_ A, SYSTEM_.SYS_TABLES_ B
        WHERE A.TABLE_ID = B.TABLE_ID
          AND B.USER_ID = userId
          AND B.TABLE_NAME = base_object_name
          AND A.CONSTRAINT_TYPE = 0
        ORDER BY A.INDEX_ID, A.CONSTRAINT_NAME ASC;
    ELSE
        SELECT A.CONSTRAINT_ID,
           CASE2(A.CONSTRAINT_NAME LIKE '%__SYS_CON_%', '', ' CONSTRAINT "' || A.CONSTRAINT_NAME || '"'),
           A.CONSTRAINT_TYPE, A.INDEX_ID, B.TBS_ID
        BULK COLLECT INTO constIds, constNames, constTypes, indexIds, tbsIds
        FROM SYSTEM_.SYS_CONSTRAINTS_ A, SYSTEM_.SYS_TABLES_ B
        WHERE A.TABLE_ID = B.TABLE_ID
          AND B.USER_ID = userId
          AND B.TABLE_NAME = base_object_name
          AND A.CONSTRAINT_TYPE IN (1, 2, 3, 6, 7)
        ORDER BY A.INDEX_ID, A.CONSTRAINT_NAME ASC;
    END IF;

    /* 
     *  BUG-49356 DB mode export에서 ALL_CRT_INDEX.sql는 connect sys/manager로 접속하게 된다.
     *  따라서, SYS 이외의 스키마를 가진 객체는 모두 명시해야 한다.
     *  SYS만 예외로 하는 이유는 최대한 하위 호환성을 지키기 위해서다.
     */
       
    IF param_export_db_mode = 'T' AND userName != 'SYS' THEN
        tblSchemaObjName := get_object_clause_force(base_object_name, userName, TRUE);
    ELSE
        tblSchemaObjName := get_object_clause(base_object_name, userName); 
    END IF;

    alterAdd := 'ALTER TABLE ' || tblSchemaObjName || ' ADD';
    alterMod := 'ALTER TABLE ' || tblSchemaObjName || ' MODIFY';
    tmpDdl := '';
    ddlStr := '';
    cnt := 0;
    isInAlter := TRUE;
    FOR n IN constIds.FIRST() .. constIds.LAST() LOOP
        IF n > 1 AND length(tmpDdl) > 0 THEN
            ddlStr := ddlStr || chr(10) || chr(10);
        END IF;

        IF constTypes[n] = 0 THEN
            tmpDdl := alterAdd || constNames[n] || ' FOREIGN KEY ' ||
                      get_references_clause(constIds[n]);
        ELSIF constTypes[n] = 1 AND param_notnull_constraints = 'T' THEN
            SELECT column_name into colName
            FROM system_.sys_columns_
            WHERE column_id=(SELECT column_id
                    FROM system_.sys_constraint_columns_
                    WHERE constraint_id=constIds[n]);
            tmpDdl := alterMod || ' "' || colName || '" NOT NULL';
        ELSIF constTypes[n] = 2 THEN
            tmpDdl := alterAdd || constNames[n] || ' UNIQUE ' ||
                      get_unique_clause(constIds[n], tbsIds[n], isInAlter);
        ELSIF constTypes[n] = 3 THEN            
            tmpDdl := alterAdd || constNames[n] || ' PRIMARY KEY ' ||
                      get_unique_clause(constIds[n], tbsIds[n], isInAlter);
        ELSIF constTypes[n] = 6 THEN
            tmpDdl := alterAdd || constNames[n] || ' LOCALUNIQUE ' ||
                      get_unique_clause(constIds[n], tbsIds[n], isInAlter);
        ELSIF constTypes[n] = 7 THEN
            tmpDdl := alterAdd || constNames[n] || ' CHECK (' ||
                      get_check_clause(constIds[n]) || ')';
        ELSE
            tmpDdl := '';
        END IF;

        IF length(tmpDdl) > 0 THEN
            cnt := 1;
            ddlStr := ddlStr || tmpDdl || sql_terminator;
        END IF;
    END LOOP;

    IF cnt = 0 THEN
        RAISE NO_DATA_FOUND;
    END IF;

    return ddlStr;
end get_dep_constraint;
                
function get_dep_index(
                 base_object_name   IN VARCHAR(128),
                 userId             IN INTEGER,
                 userName           IN VARCHAR(128)) return varchar(65534)
as
n                INTEGER;
cnt              INTEGER;
userIds          INT_ARRAY;
indexNames       CHAR128_ARRAY;
indexSchemas     CHAR128_ARRAY;
ddlStr           varchar(65534);
begin
    SELECT INDEX_NAME, A.USER_ID, B.USER_NAME
    BULK COLLECT INTO indexNames, userIds, indexSchemas
    FROM SYSTEM_.SYS_INDICES_ A, SYSTEM_.SYS_USERS_ B
    WHERE TABLE_ID = (SELECT TABLE_ID 
                      FROM SYSTEM_.SYS_TABLES_
                      WHERE USER_id = userId AND TABLE_NAME = base_object_name)
      AND INDEX_NAME NOT LIKE '__SYS_IDX_ID_%'
      AND A.USER_ID = B.USER_ID
      AND A.INDEX_ID NOT IN (
          SELECT INDEX_ID FROM SYSTEM_.SYS_CONSTRAINTS_
          WHERE TABLE_ID = (SELECT TABLE_ID
                            FROM SYSTEM_.SYS_TABLES_
                            WHERE USER_id = userId AND TABLE_NAME = base_object_name))
    ORDER BY A.INDEX_ID;

    ddlStr := '';
    cnt := 0;
    FOR n IN indexNames.FIRST() .. indexNames.LAST() LOOP
        /* BUG-49356 DB mode export가 아니면 테이블 스키마와 인덱스 스키마가 다르면 skip. */ 
        IF param_export_db_mode != 'T' AND userName != indexSchemas[n] THEN
            CONTINUE;
        END IF;
        cnt := 1;
        IF n > 1 THEN
            ddlStr := ddlStr || chr(10) || chr(10);
        END IF;

        ddlStr := ddlStr || get_index_ddl(indexNames[n], userIds[n], indexSchemas[n]);
    END LOOP;

    IF cnt = 0 THEN
        RAISE NO_DATA_FOUND;
    END IF;

    return ddlStr;
end get_dep_index;

function get_dep_access_mode(
                 base_object_name   IN VARCHAR(128),
                 userId             IN INTEGER,
                 userName           IN VARCHAR(128)) return varchar(65534)
as
n                INTEGER;
tableId          INTEGER;
accessMode       VARCHAR(15);
alterStr         VARCHAR(300);
partIds          INT_ARRAY;
partAccesses     CHAR16_ARRAY;
partNames        CHAR150_ARRAY;
ddlStr           varchar(65534);
begin
    ddlStr := '';

    SELECT /*+ USE_HASH( A, B ) */
            A.TABLE_ID,
            DECODE(A.ACCESS, 'R', 'READ ONLY', 'W', '', 'A', 'READ APPEND')
        INTO tableId, accessMode
    FROM SYSTEM_.SYS_TABLES_ A, V$TABLESPACES B
    WHERE A.USER_ID = userId
        AND A.TBS_ID = B.ID
        AND A.TABLE_TYPE ='T'
        AND A.TEMPORARY = 'N'
        AND A.HIDDEN = 'N'
        AND A.TABLE_NAME = base_object_name;

    alterStr := 'ALTER TABLE ' || get_object_clause(base_object_name, userName) ||
                ' ACCESS ';

    IF accessMode IS NOT NULL THEN
        ddlStr := alterStr || accessMode || sql_terminator || chr(10);
    END IF;

    SELECT PARTITION_ID, 
        'PARTITION "' || PARTITION_NAME || '"',
        DECODE(PARTITION_ACCESS, 'R', ' READ ONLY',
                               'W', '',
                               'A', ' READ APPEND')
    BULK COLLECT INTO partIds, partNames, partAccesses
    FROM SYSTEM_.SYS_TABLE_PARTITIONS_
    WHERE TABLE_ID = tableId
        AND PARTITION_NAME IS NOT NULL
    ORDER BY PARTITION_ID;
    
    FOR n IN partIds.FIRST() .. partIds.LAST() LOOP
        IF partAccesses[n] IS NOT NULL THEN
            ddlStr := ddlStr || alterStr || partNames[n] || partAccesses[n] || sql_terminator || chr(10);
        END IF;
    END LOOP;

    return ddlStr;

    EXCEPTION
    WHEN NO_DATA_FOUND THEN
        return ddlStr;
end get_dep_access_mode;

function get_dep_trigger(
                 base_object_name   IN VARCHAR(128),
                 userId             IN INTEGER,
                 userName           IN VARCHAR(128)) return varchar(65534)
as
n                INTEGER;
cnt              INTEGER;
triggerNames     CHAR128_ARRAY;
ddlStr           varchar(65534);
begin
    SELECT TRIGGER_NAME
    BULK COLLECT INTO triggerNames
    FROM SYSTEM_.SYS_TRIGGERS_
    WHERE TABLE_ID = (SELECT TABLE_ID 
                      FROM SYSTEM_.SYS_TABLES_
                      WHERE USER_id = userId AND TABLE_NAME = base_object_name)
    ORDER BY TRIGGER_NAME;

    ddlStr := '';
    cnt := 0;
    FOR n IN triggerNames.FIRST() .. triggerNames.LAST() LOOP
        cnt := 1;
        IF n > 1 THEN
            ddlStr := ddlStr || chr(10) || chr(10);
        END IF;

        ddlStr := ddlStr || get_trigger_ddl(triggerNames[n], userId, userName);
    END LOOP;

    IF cnt = 0 THEN
        RAISE NO_DATA_FOUND;
    END IF;

    return ddlStr;
end get_dep_trigger;

function get_dep_obj_grant(
                 base_object_name   IN VARCHAR(128),
                 userId             IN INTEGER,
                 userName           IN VARCHAR(128)) return varchar(65534)
as
n                INTEGER;
cnt              INTEGER;
grantOptions     INT_ARRAY;
objTypes         CHAR1_ARRAY;
granteeNames     CHAR128_ARRAY;
privNames        CHAR128_ARRAY;
ddlStr           varchar(65534);
begin
    SELECT DECODE(C.GRANTEE_ID, 0, 'PUBLIC',
             (SELECT '"'||USER_NAME||'"' FROM SYSTEM_.SYS_USERS_
              WHERE USER_ID=C.GRANTEE_ID)) AS GRANTEE_NAME,
           GROUP_CONCAT(D.PRIV_NAME, ', '),
           C.WITH_GRANT_OPTION AS GRANT_OPTION,
           C.OBJ_TYPE
      BULK COLLECT INTO granteeNames, privNames, grantOptions, objTypes
    FROM (
      SELECT GRANTEE_ID, PRIV_ID, WITH_GRANT_OPTION, A.OBJ_TYPE AS OBJ_TYPE
      FROM SYSTEM_.SYS_GRANT_OBJECT_ A, SYSTEM_.SYS_TABLES_ B
      WHERE A.OBJ_TYPE IN ('T', 'S', 'V', 'Q', 'M')
        AND A.USER_ID = B.USER_ID
        AND A.OBJ_ID = B.TABLE_ID
        AND B.USER_ID = userId
        AND B.TABLE_NAME = base_object_name
      UNION ALL
      SELECT GRANTEE_ID, PRIV_ID, WITH_GRANT_OPTION, A.OBJ_TYPE AS OBJ_TYPE
      FROM SYSTEM_.SYS_GRANT_OBJECT_ A, SYSTEM_.SYS_PACKAGES_ B
      WHERE A.OBJ_TYPE = 'A' AND B.PACKAGE_TYPE=6
        AND A.USER_ID = B.USER_ID
        AND A.OBJ_ID = B.PACKAGE_OID
        AND B.USER_ID = userId
        AND B.PACKAGE_NAME = base_object_name
        /* exclude non-schema object
      UNION ALL
      SELECT GRANTEE_ID, PRIV_ID, WITH_GRANT_OPTION, A.OBJ_TYPE AS OBJ_TYPE
      FROM SYSTEM_.SYS_GRANT_OBJECT_ A, SYSTEM_.SYS_DIRECTORIES_ B
      WHERE A.OBJ_TYPE = 'D'
        AND A.USER_ID = B.USER_ID
        AND A.OBJ_ID = B.DIRECTORY_ID
        AND B.USER_ID = userId
        AND B.DIRECTORY_NAME = base_object_name
        */
      UNION ALL
      SELECT GRANTEE_ID, PRIV_ID, WITH_GRANT_OPTION, A.OBJ_TYPE AS OBJ_TYPE
      FROM SYSTEM_.SYS_GRANT_OBJECT_ A, SYSTEM_.SYS_PROCEDURES_ B
      WHERE A.OBJ_TYPE = 'P'
        AND A.USER_ID = B.USER_ID
        AND A.OBJ_ID = B.PROC_OID
        AND B.USER_ID = userId
        AND B.PROC_NAME = base_object_name
      UNION ALL
      SELECT GRANTEE_ID, PRIV_ID, WITH_GRANT_OPTION, A.OBJ_TYPE AS OBJ_TYPE
      FROM SYSTEM_.SYS_GRANT_OBJECT_ A, SYSTEM_.SYS_LIBRARIES_ B
      WHERE A.OBJ_TYPE = 'Y'
        AND A.USER_ID = B.USER_ID
        AND A.OBJ_ID = B.LIBRARY_ID
        AND B.USER_ID = userId
        AND B.LIBRARY_NAME = base_object_name
      ) C, SYSTEM_.SYS_PRIVILEGES_ D
    WHERE C.PRIV_ID = D.PRIV_ID
    GROUP BY C.GRANTEE_ID, C.WITH_GRANT_OPTION, C.OBJ_TYPE;

    ddlStr := '';
    cnt := 0;
    FOR n IN granteeNames.FIRST() .. granteeNames.LAST() LOOP
        cnt := 1;
        IF n > 1 THEN
            ddlStr := ddlStr || chr(10) || chr(10);
        END IF;

        ddlStr := ddlStr || 'GRANT ' || privNames[n];

        IF objTypes[n] = 'D' THEN
            ddlStr := ddlStr || ' ON DIRECTORY "' || base_object_name;
        ELSE
            ddlStr := ddlStr || ' ON "' || userName || '"."' || base_object_name;
        END IF;

        ddlStr := ddlStr || '" TO ' || granteeNames[n];

        IF grantOptions[n] = 1 THEN
            ddlStr := ddlStr || ' WITH GRANT OPTION';
        END IF;

        ddlStr := ddlStr || sql_terminator;
    END LOOP;

    IF cnt = 0 THEN
        RAISE NO_DATA_FOUND;
    END IF;

    return ddlStr;
end get_dep_obj_grant;

function get_object_grant(
                 granteeId             IN INTEGER,
                 granteeName           IN VARCHAR(128)) return varchar(65534)
as
n                INTEGER;
cnt              INTEGER;
grantOptions     INT_ARRAY;
objTypes         CHAR1_ARRAY;
privNames        CHAR128_ARRAY;
baseSchemas      CHAR128_ARRAY;
baseObjs         CHAR128_ARRAY;
ddlStr           varchar(65534);
begin
    SELECT 
           LISTAGG(D.PRIV_NAME, ', ') WITHIN GROUP(ORDER BY D.PRIV_NAME),
           C.OBJ_TYPE, C.BASE_SCHEMA, C.BASE_OBJ,
           C.WITH_GRANT_OPTION
      BULK COLLECT INTO privNames, objTypes, baseSchemas, baseObjs, grantOptions
    FROM (
      SELECT PRIV_ID, A.OBJ_TYPE AS OBJ_TYPE,
             U.USER_NAME AS BASE_SCHEMA, B.TABLE_NAME AS BASE_OBJ,
             WITH_GRANT_OPTION
      FROM SYSTEM_.SYS_GRANT_OBJECT_ A, SYSTEM_.SYS_TABLES_ B, SYSTEM_.SYS_USERS_ U
      WHERE A.OBJ_TYPE IN ('T', 'S', 'V', 'Q', 'M')
        AND A.GRANTEE_ID = granteeId
        AND A.USER_ID = B.USER_ID
        AND A.OBJ_ID = B.TABLE_ID
        AND B.USER_ID = U.USER_ID
      UNION ALL
      SELECT PRIV_ID, A.OBJ_TYPE AS OBJ_TYPE,
             U.USER_NAME AS BASE_SCHEMA, B.PACKAGE_NAME AS BASE_OBJ,
             WITH_GRANT_OPTION
      FROM SYSTEM_.SYS_GRANT_OBJECT_ A, SYSTEM_.SYS_PACKAGES_ B, SYSTEM_.SYS_USERS_ U
      WHERE A.OBJ_TYPE = 'A' AND B.PACKAGE_TYPE=6
        AND A.GRANTEE_ID = granteeId
        AND A.USER_ID = B.USER_ID
        AND A.OBJ_ID = B.PACKAGE_OID
        AND B.USER_ID = U.USER_ID
      UNION ALL
      SELECT PRIV_ID, A.OBJ_TYPE AS OBJ_TYPE,
             U.USER_NAME AS BASE_SCHEMA, B.DIRECTORY_NAME AS BASE_OBJ,
             WITH_GRANT_OPTION
      FROM SYSTEM_.SYS_GRANT_OBJECT_ A, SYSTEM_.SYS_DIRECTORIES_ B, SYSTEM_.SYS_USERS_ U
      WHERE A.OBJ_TYPE = 'D'
        AND A.GRANTEE_ID = granteeId
        AND A.USER_ID = B.USER_ID
        AND A.OBJ_ID = B.DIRECTORY_ID
        AND B.USER_ID = U.USER_ID
      UNION ALL
      SELECT PRIV_ID, A.OBJ_TYPE AS OBJ_TYPE,
             U.USER_NAME AS BASE_SCHEMA, B.PROC_NAME AS BASE_OBJ,
             WITH_GRANT_OPTION
      FROM SYSTEM_.SYS_GRANT_OBJECT_ A, SYSTEM_.SYS_PROCEDURES_ B, SYSTEM_.SYS_USERS_ U
      WHERE A.OBJ_TYPE = 'P'
        AND A.GRANTEE_ID = granteeId
        AND A.USER_ID = B.USER_ID
        AND A.OBJ_ID = B.PROC_OID
        AND B.USER_ID = U.USER_ID
      UNION ALL
      SELECT PRIV_ID, A.OBJ_TYPE AS OBJ_TYPE,
             U.USER_NAME AS BASE_SCHEMA, B.LIBRARY_NAME AS BASE_OBJ,
             WITH_GRANT_OPTION
      FROM SYSTEM_.SYS_GRANT_OBJECT_ A, SYSTEM_.SYS_LIBRARIES_ B, SYSTEM_.SYS_USERS_ U
      WHERE A.OBJ_TYPE = 'Y'
        AND A.GRANTEE_ID = granteeId
        AND A.USER_ID = B.USER_ID
        AND A.OBJ_ID = B.LIBRARY_ID
        AND B.USER_ID = U.USER_ID
      ) C, SYSTEM_.SYS_PRIVILEGES_ D
    WHERE C.PRIV_ID = D.PRIV_ID
    GROUP BY C.OBJ_TYPE, C.BASE_SCHEMA, C.BASE_OBJ, C.WITH_GRANT_OPTION;

    ddlStr := '';
    cnt := 0;
    FOR n IN baseObjs.FIRST() .. baseObjs.LAST() LOOP
        cnt := 1;
        IF n > 1 THEN
            ddlStr := ddlStr || sql_terminator || chr(10) || chr(10);
        END IF;

        ddlStr := ddlStr || 'GRANT ' || privNames[n];

        IF objTypes[n] = 'D' THEN
            ddlStr := ddlStr || ' ON DIRECTORY "' || baseObjs[n];
        ELSE
            ddlStr := ddlStr || ' ON "' || baseSchemas[n] || '"."' || baseObjs[n];
        END IF;

        ddlStr := ddlStr || '" TO "' || granteeName || '"';

        IF grantOptions[n] = 1 THEN
            ddlStr := ddlStr || ' WITH GRANT OPTION';
        END IF;
    END LOOP;

    IF cnt = 0 THEN
        RAISE NO_DATA_FOUND;
    END IF;

    return ddlStr || sql_terminator;
end get_object_grant;

function get_system_grant(
                 userType           IN CHAR(1),
                 userId             IN INTEGER,
                 userName           IN VARCHAR(128)) return varchar(65534)
as
n                INTEGER;
cnt              INTEGER;
hasAllPriv       CHAR(1);
privNames        CHAR128_ARRAY;
grantedPriv      VARCHAR(9000);
ddlStr           varchar(65534);
begin
    IF userType = 'U' THEN
        SELECT REPLACE(PRIV_NAME, '_', ' ')
        BULK COLLECT INTO privNames
        FROM SYSTEM_.SYS_PRIVILEGES_ A, SYSTEM_.SYS_GRANT_SYSTEM_ B
        WHERE A.PRIV_ID = B.PRIV_ID
          AND B.GRANTEE_ID = userId
          AND A.PRIV_TYPE = 2
          AND A.PRIV_ID NOT IN (205,210,215,217,229,241,245,252,256,260) -- default priv.
        ORDER BY A.PRIV_ID;
    ELSE
        SELECT REPLACE(PRIV_NAME, '_', ' ')
        BULK COLLECT INTO privNames
        FROM SYSTEM_.SYS_PRIVILEGES_ A, SYSTEM_.SYS_GRANT_SYSTEM_ B
        WHERE A.PRIV_ID = B.PRIV_ID
          AND B.GRANTEE_ID = userId
          AND A.PRIV_TYPE = 2
        ORDER BY A.PRIV_ID;
    END IF;

    hasAllPriv := 'F';
    grantedPriv := '';
    cnt := 0;
    FOR n IN privNames.FIRST() .. privNames.LAST() LOOP

        IF privNames[n] = 'ALL' THEN
            hasAllPriv := 'T';
        ELSE
            IF cnt > 0 THEN
                grantedPriv := grantedPriv || ', ';
            END IF;
            cnt := 1;

            grantedPriv := grantedPriv || privNames[n];
        END IF;
    END LOOP;

    IF hasAllPriv = 'F' AND cnt = 0 THEN
        RAISE NO_DATA_FOUND;
    END IF;

    IF cnt > 0 THEN
        ddlStr := 'GRANT ' || grantedPriv || ' TO "' ||
                  userName || '"' || sql_terminator;
    END IF;

    -- To prevent error (Duplicate privilege names)
    IF hasAllPriv = 'T' THEN
        ddlStr := 'GRANT ALL PRIVILEGES TO "' || userName || '"' ||
                  sql_terminator || chr(10) || chr(10) || ddlStr;
    END IF;

    return ddlStr;
end get_system_grant;

function get_role_grant(
                 userId             IN INTEGER,
                 userName           IN VARCHAR(128)) return varchar(65534)
as
n                INTEGER;
cnt              INTEGER;
roles            CHAR128_ARRAY;
ddlStr           varchar(65534);
begin
    SELECT USER_NAME
    BULK COLLECT INTO roles
    FROM SYSTEM_.SYS_USERS_
    WHERE USER_ID IN (SELECT ROLE_ID 
                      FROM SYSTEM_.SYS_USER_ROLES_
                      WHERE GRANTEE_ID = userId
                        AND ROLE_ID != 0)
    ORDER BY USER_NAME;

    ddlStr := 'GRANT ';
    cnt := 0;
    FOR n IN roles.FIRST() .. roles.LAST() LOOP
        cnt := 1;
        IF n > 1 THEN
            ddlStr := ddlStr || ', ';
        END IF;

        ddlStr := ddlStr || roles[n];

    END LOOP;

    IF cnt = 0 THEN
        RAISE NO_DATA_FOUND;
    END IF;

    return ddlStr || ' TO "' || userName || '"' || sql_terminator;
end get_role_grant;

function get_nonschema_ddl(
                 object_type IN VARCHAR(20),
                 object_name IN VARCHAR(128),
                 schema      IN VARCHAR(128) DEFAULT NULL) return varchar(65534)
as
begin
    IF schema IS NOT NULL THEN
        RAISE_APPLICATION_ERROR( invalid_argval_errcode,
            'Invalid input value for parameter SCHEMA which accepts NULL only.' );
    END IF;

    IF object_type = 'JOB' THEN
        return get_job_ddl(object_name);
    ELSIF object_type = 'DIRECTORY' THEN
        return get_directory_ddl(object_name);
    ELSIF object_type = 'REPLICATION' THEN
        return get_replication_ddl(object_name);
    ELSIF object_type = 'TABLESPACE' THEN
        return get_tablespace_ddl(object_name);
    ELSIF object_type = 'USER' THEN
        return get_user_ddl(object_name);
    ELSIF object_type = 'ROLE' THEN
        return get_role_ddl(object_name);
    END IF;

    EXCEPTION
    WHEN NO_DATA_FOUND THEN
        RAISE_APPLICATION_ERROR( object_not_found_errcode,
            object_type || ' "' || object_name || '" not found.' );
end get_nonschema_ddl;

function get_ddl(object_type IN VARCHAR(20),
                 object_name IN VARCHAR(128),
                 schema      IN VARCHAR(128) DEFAULT NULL) return varchar(65534)
as
userId         INTEGER;
userName       VARCHAR(128);
begin
    IF object_type IS NULL THEN
        RAISE_APPLICATION_ERROR( invalid_argval_errcode,
            'Invalid argument found. NULL is not allowed for parameter OBJECT_TYPE.' );
    END IF;

    IF object_name IS NULL THEN
        RAISE_APPLICATION_ERROR( invalid_argval_errcode,
            'Invalid argument found. NULL is not allowed for parameter OBJECT_NAME.' );
    END IF;
    
    IF object_type = 'TABLE' THEN
        get_schema_info(schema, userId, userName);
        return get_table_ddl(object_name, userId, userName);
    ELSIF object_type = 'INDEX' THEN
        get_schema_info(schema, userId, userName);
        return get_index_ddl(object_name, userId, userName);
    ELSIF object_type = 'CONSTRAINT' THEN
        get_schema_info(schema, userId, userName);
        return get_constraint_ddl(object_name, userId, userName);
    ELSIF object_type = 'REF_CONSTRAINT' THEN
        get_schema_info(schema, userId, userName);
        return get_constraint_ddl(object_name, userId, userName);
    ELSIF object_type = 'PROCEDURE' THEN
        get_schema_info(schema, userId, userName);
        return get_psm_ddl(0, object_name, userId, userName);
    ELSIF object_type = 'FUNCTION' THEN
        get_schema_info(schema, userId, userName);
        return get_psm_ddl(1, object_name, userId, userName);
    ELSIF object_type = 'TYPESET' THEN
        get_schema_info(schema, userId, userName);
        return get_psm_ddl(3, object_name, userId, userName);
    ELSIF object_type = 'PACKAGE' THEN
        get_schema_info(schema, userId, userName);
        return get_package_ddl(1, object_name, userId, userName);
    ELSIF object_type = 'PACKAGE_SPEC' THEN
        get_schema_info(schema, userId, userName);
        return get_package_ddl(6, object_name, userId, userName);
    ELSIF object_type = 'PACKAGE_BODY' THEN
        get_schema_info(schema, userId, userName);
        return get_package_ddl(7, object_name, userId, userName);
    ELSIF object_type = 'VIEW' THEN
        get_schema_info(schema, userId, userName);
        return get_view_ddl(object_name, userId, userName);
    ELSIF object_type = 'MATERIALIZED_VIEW' THEN
        get_schema_info(schema, userId, userName);
        return get_mview_ddl(object_name, userId, userName);
    ELSIF object_type = 'TRIGGER' THEN
        get_schema_info(schema, userId, userName);
        return get_trigger_ddl(object_name, userId, userName);
    ELSIF object_type = 'SEQUENCE' THEN
        get_schema_info(schema, userId, userName);
        return get_sequence_ddl(object_name, userId, userName);
    ELSIF object_type = 'SYNONYM' THEN
        get_schema_info(schema, userId, userName);
        return get_synonym_ddl(object_name, userId, userName);
    ELSIF object_type = 'QUEUE' THEN
        get_schema_info(schema, userId, userName);
        return get_queue_ddl(object_name, userId, userName);
    ELSIF object_type = 'LIBRARY' THEN
        get_schema_info(schema, userId, userName);
        return get_library_ddl(object_name, userId, userName);
    ELSIF object_type = 'DB_LINK' THEN
        get_schema_info(schema, userId, userName);
        return get_dblink_ddl(object_name, userId, userName);
    ELSIF object_type IN ('JOB', 'DIRECTORY', 'REPLICATION',
                          'TABLESPACE', 'USER', 'ROLE') THEN
        return get_nonschema_ddl(object_type, object_name, schema);
    ELSE
        RAISE_APPLICATION_ERROR( not_supported_obj_type_errcode,
            'Not supported object type: ''' || object_type || '''.' );
    END IF;

    EXCEPTION
    WHEN NO_DATA_FOUND THEN
        RAISE_APPLICATION_ERROR( object_not_found_errcode,
            object_type || ' "' || object_name || '" not found in the schema "' || userName || '".' );
end get_ddl;

function get_dependent_ddl(
                 object_type        IN VARCHAR(20),
                 base_object_name   IN VARCHAR(128),
                 base_object_schema IN VARCHAR(128) DEFAULT NULL) return varchar(65534)
as
userId         INTEGER;
userName       VARCHAR(128);
begin
    IF object_type IS NULL THEN
        RAISE_APPLICATION_ERROR( invalid_argval_errcode,
            'Invalid input value NULL for parameter OBJECT_TYPE.' );
    END IF;

    IF base_object_name IS NULL THEN
        RAISE_APPLICATION_ERROR( invalid_argval_errcode,
            'Invalid input value NULL for parameter BASE_OBJECT_NAME.' );
    END IF;
    
    IF object_type = 'COMMENT' THEN
        get_schema_info(base_object_schema, userId, userName);
        return get_dep_comment(base_object_name, userName);
    ELSIF object_type = 'CONSTRAINT' THEN
        get_schema_info(base_object_schema, userId, userName);
        return get_dep_constraint(100, base_object_name, userId, userName);
    ELSIF object_type = 'REF_CONSTRAINT' THEN
        get_schema_info(base_object_schema, userId, userName);
        return get_dep_constraint(0, base_object_name, userId, userName);
    ELSIF object_type = 'INDEX' THEN
        get_schema_info(base_object_schema, userId, userName);
        return get_dep_index(base_object_name, userId, userName);
    ELSIF object_type = 'OBJECT_GRANT' THEN
        get_schema_info(base_object_schema, userId, userName);
        return get_dep_obj_grant(base_object_name, userId, userName);
    ELSIF object_type = 'TRIGGER' THEN
        get_schema_info(base_object_schema, userId, userName);
        return get_dep_trigger(base_object_name, userId, userName);
    ELSIF object_type = 'ACCESS_MODE' THEN
        get_schema_info(base_object_schema, userId, userName);
        return get_dep_access_mode(base_object_name, userId, userName);
    ELSE
        RAISE_APPLICATION_ERROR( not_supported_obj_type_errcode,
            'Not supported object type: ''' || object_type || '''.' );
    END IF;

    EXCEPTION
    WHEN NO_DATA_FOUND THEN
        RAISE_APPLICATION_ERROR( object_not_found_errcode,
            object_type || ' not found on the specified base object.' );
end get_dependent_ddl;

function get_granted_ddl(
                 object_type IN VARCHAR(20),
                 grantee     IN VARCHAR(128) DEFAULT NULL) return varchar(65534)
as
userId           INTEGER;
userType         CHAR(1);
userName         VARCHAR(128);
begin
    IF object_type IS NULL THEN
        RAISE_APPLICATION_ERROR( invalid_argval_errcode,
            'Invalid input value NULL for parameter OBJECT_TYPE.' );
    END IF;
    
    IF object_type NOT IN ('OBJECT_GRANT', 'SYSTEM_GRANT', 'ROLE_GRANT') THEN
        RAISE_APPLICATION_ERROR( not_supported_obj_type_errcode,
            'Not supported object type: ''' || object_type || '''.' );
    END IF;

    BEGIN
        SELECT USER_ID, USER_NAME, USER_TYPE
        INTO userId, userName, userType
        FROM SYSTEM_.SYS_USERS_
        WHERE USER_NAME = NVL(grantee, USER_NAME());
    EXCEPTION
    WHEN NO_DATA_FOUND THEN
        RAISE_APPLICATION_ERROR( schema_not_found_errcode,
            'GRANTEE "' || grantee || '" not found.' );
    END;

    IF object_type = 'OBJECT_GRANT' THEN
        return get_object_grant(userId, userName);
    ELSIF object_type = 'SYSTEM_GRANT' THEN
        return get_system_grant(userType, userId, userName);
    ELSIF object_type = 'ROLE_GRANT' THEN
        return get_role_grant(userId, userName);
    END IF;

    EXCEPTION
    WHEN NO_DATA_FOUND THEN
        RAISE_APPLICATION_ERROR( object_not_found_errcode,
            object_type || ' not found.' );
end get_granted_ddl;

function get_index_stats_ddl(
                 userId             IN INTEGER,
                 tableId            IN INTEGER,
                 schema4Proc        IN VARCHAR(130),
                 table4Proc         IN VARCHAR(130)) return varchar(65534)
as
n                INTEGER;
user4Proc        VARCHAR(130);
index4Proc       VARCHAR(130);
columnList       VARCHAR(32000);
keyCnt           BIGINT;
numPage          BIGINT;
numDist          BIGINT;
clstFct          BIGINT;
idxHeight        BIGINT;
cachePage        BIGINT;
avgSlotCnt       BIGINT;
indexIds         INT_ARRAY;
constTypes       INT_ARRAY;
indexNames       CHAR128_ARRAY;
userNames        CHAR128_ARRAY;
ddlStr           varchar(65534);
begin
    SELECT A.INDEX_ID, A.INDEX_NAME, 
           (select B.CONSTRAINT_TYPE from SYSTEM_.SYS_CONSTRAINTS_ B where b.index_id=a.index_id),
           C.USER_NAME
    BULK COLLECT INTO indexIds, indexNames, constTypes, userNames
    FROM SYSTEM_.SYS_INDICES_ A, SYSTEM_.SYS_USERS_ C
    WHERE A.TABLE_ID = tableId
      AND A.USER_ID = C.USER_ID
    ORDER BY A.INDEX_ID;

    ddlStr := '';
    FOR n IN indexNames.FIRST() .. indexNames.LAST() LOOP
        ddlStr := ddlStr || chr(10);

        user4Proc := '"' || userNames[n] || '"';
        index4Proc := '"' || indexNames[n] || '"';

        GET_INDEX_STATS(user4Proc, index4Proc, NULL,
            keyCnt, numPage, numDist, clstFct, idxHeight, cachePage, avgSlotCnt);

        IF indexNames[n] NOT LIKE '__SYS_IDX_ID_%' THEN
            ddlStr := ddlStr || 'EXEC SET_INDEX_STATS(''' ||
                      user4Proc || ''', ''' || index4Proc || ''', ' ||
                      keyCnt || ', ' || numPage || ', ' || numDist || ', ' ||
                      clstFct || ', ' || idxHeight || ', ' || avgSlotCnt || ')' || sql_terminator;
        ELSIF constTypes[n] = 2 THEN
            SELECT '''' || GROUP_CONCAT('"' || B.COLUMN_NAME || '"' ||
                DECODE(SORT_ORDER, 'D', ' DESC'), ',') || ''''
            INTO columnList
            FROM SYSTEM_.SYS_INDEX_COLUMNS_ A, SYSTEM_.SYS_COLUMNS_ B
            WHERE A.COLUMN_ID = B.COLUMN_ID
              AND A.INDEX_ID = indexIds[n]
            ORDER BY A.INDEX_COL_ORDER;

            ddlStr := ddlStr || 'EXEC DBMS_STATS.SET_UNIQUE_KEY_STATS(''' ||
                      schema4Proc || ''', ''' || table4Proc || ''', ' || columnList || ', ' ||
                      keyCnt || ', ' || numPage || ', ' || numDist || ', ' ||
                      clstFct || ', ' || idxHeight || ', ' || avgSlotCnt || ')' || sql_terminator;
        ELSIF constTypes[n] = 3 THEN
            ddlStr := ddlStr || 'EXEC DBMS_STATS.SET_PRIMARY_KEY_STATS(''' ||
                      schema4Proc || ''', ''' || table4Proc || ''', ' ||
                      keyCnt || ', ' || numPage || ', ' || numDist || ', ' ||
                      clstFct || ', ' || idxHeight || ', ' || avgSlotCnt || ')' || sql_terminator;
        END IF;
    END LOOP;

    return ddlStr;

end get_index_stats_ddl;

function get_stats_ddl(
                 object_name IN VARCHAR(128),
                 schema      IN VARCHAR(128) DEFAULT NULL) return varchar(65534)
as
userId         INTEGER;
tableId        INTEGER;
userName       VARCHAR(128);
numRow         BIGINT;
numPage        BIGINT;
avgLen         BIGINT;
cachePage      BIGINT;
numDist        BIGINT;
numNull        BIGINT;
oneRowReadTime double;
minVal         VARCHAR(48);
maxVal         VARCHAR(48);
user4Proc      VARCHAR(130);
table4Proc     VARCHAR(130);
col4Proc       VARCHAR(130);
part4Proc      VARCHAR(130);
columns        CHAR130_ARRAY;
partNames      CHAR130_ARRAY;
ddlStr         varchar(65534);
begin
    IF object_name IS NULL THEN
        RAISE_APPLICATION_ERROR( invalid_argval_errcode,
            'Invalid argument found. NULL is not allowed for parameter OBJECT_NAME.' );
    END IF;
    
    get_schema_info(schema, userId, userName);

    SELECT TABLE_ID INTO tableId
    FROM SYSTEM_.SYS_TABLES_
    WHERE TABLE_NAME = object_name
      AND USER_ID = userId;

    user4Proc := '"' || userName || '"';
    table4Proc := '"' || object_name || '"';

    GET_TABLE_STATS(user4Proc, table4Proc, NULL,
        numRow, numPage, avgLen, cachePage, oneRowReadTime);

    IF numRow IS NULL THEN
        return NULL;
    END IF;

    ddlStr := 'EXEC SET_TABLE_STATS(''' ||
              user4Proc || ''', ''' || table4Proc || ''', NULL, ' ||
              numRow || ', ' || numPage || ', ' || avgLen || ', ' ||
              oneRowReadTime || ')';

    SELECT '"' || COLUMN_NAME ||'"'
      BULK COLLECT INTO columns
    FROM SYSTEM_.SYS_COLUMNS_
    WHERE TABLE_ID = tableId
      AND IS_HIDDEN = 'F'
    ORDER BY COLUMN_ORDER;
    
    FOR n IN columns.FIRST() .. columns.LAST() LOOP
        ddlStr := ddlStr || sql_terminator || chr(10);
        
        GET_COLUMN_STATS(user4Proc, table4Proc, columns[n], NULL,
            numDist, numNull, avgLen, minVal, maxVal);

        ddlStr := ddlStr || 'EXEC SET_COLUMN_STATS(''' ||
              user4Proc || ''', ''' || table4Proc || ''', ''' || columns[n] || ''', NULL, ' ||
              numDist || ', ' || numNull || ', ' || avgLen;

        IF minVal IS NOT NULL THEN
            /* BUG-47568 */
            minVal := REPLACE2(minVal, '''', '''''');
            ddlStr := ddlStr || ', ''' || minVal || '''';
        ELSE
            ddlStr := ddlStr || ', NULL';
        END IF;

        IF maxVal IS NOT NULL THEN
            /* BUG-47568 */
            maxVal := REPLACE2(maxVal, '''', '''''');
            ddlStr := ddlStr || ', ''' || maxVal || ''')';
        ELSE
            ddlStr := ddlStr || ', NULL)';
        END IF;
    END LOOP;

    SELECT '"' || a.partition_name || '"'
      BULK COLLECT INTO partNames
    FROM system_.sys_table_partitions_ a, system_.sys_tables_ b
    WHERE a.table_id = b.table_id
      AND b.table_name = object_name
      AND b.user_id = userId
      AND a.partition_name IS NOT NULL
    ORDER BY a.partition_id;

    FOR n IN partNames.FIRST() .. partNames.LAST() LOOP
        ddlStr := ddlStr || sql_terminator || chr(10);
        
        GET_TABLE_STATS(user4Proc, table4Proc, partNames[n],
            numRow, numPage, avgLen, cachePage, oneRowReadTime);

        ddlStr := ddlStr || 'EXEC SET_TABLE_STATS(''' ||
              user4Proc || ''', ''' || table4Proc || ''', ''' || partNames[n] || ''', ' ||
              numRow || ', ' || numPage || ', ' || avgLen || ', ' ||
              oneRowReadTime || ')';

        SELECT '"' || b.column_name || '"'
          BULK COLLECT INTO columns
        FROM system_.sys_part_key_columns_ a, system_.sys_columns_ b, system_.sys_tables_ c
        WHERE a.partition_obj_id = c.table_id
          AND a.column_id = b.column_id
          AND a.object_type = 0
          AND c.table_name = object_name
          AND c.user_id = userId
        ORDER BY a.part_col_order;

        FOR m IN columns.FIRST() .. columns.LAST() LOOP
            ddlStr := ddlStr || sql_terminator || chr(10);

            GET_COLUMN_STATS(user4Proc, table4Proc, columns[m], partNames[n],
                numDist, numNull, avgLen, minVal, maxVal);
    
            ddlStr := ddlStr || 'EXEC SET_COLUMN_STATS(''' ||
                  user4Proc || ''', ''' || table4Proc || ''', ''' ||
                  columns[m] || ''', ''' || partNames[n] || ''', ' ||
                  numDist || ', ' || numNull || ', ' || avgLen;
    
            IF minVal IS NOT NULL THEN
                ddlStr := ddlStr || ', ''' || minVal || '''';
            ELSE
                ddlStr := ddlStr || ', NULL';
            END IF;
    
            IF maxVal IS NOT NULL THEN
                ddlStr := ddlStr || ', ''' || maxVal || ''')';
            ELSE
                ddlStr := ddlStr || ', NULL)';
            END IF;
        END LOOP;
        
    END LOOP;

    IF ddlStr IS NOT NULL THEN
        ddlStr := ddlStr || sql_terminator;
    END IF;

    return ddlStr || get_index_stats_ddl(userId, tableId, user4Proc, table4Proc);

    EXCEPTION
    WHEN NO_DATA_FOUND THEN
        RAISE_APPLICATION_ERROR( object_not_found_errcode,
            '"' || object_name || '" not found in the schema "' || userName || '".' );
end get_stats_ddl;

end dbms_metadata;
/
