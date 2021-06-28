/***********************************************************************
 * Copyright 1999-2018, ALTIBASE Corporation or its subsidiaries.
 * All rights reserved.
 **********************************************************************/

CREATE OR REPLACE PACKAGE BODY utl_shard_online_rebuild IS

--  Get Connection Information
PROCEDURE get_connection_info( adjustment IN INTEGER DEFAULT 0 )
AS
    code            INTEGER;
    errm            VARCHAR(1024);
BEGIN
    DECLARE
        CURSOR c1 IS SELECT NODE_NAME,
                            INTERNAL_HOST_IP AS HOST_IP,
                            INTERNAL_PORT_NO AS PORT_NO,
                            CASE2( INTERNAL_ALTERNATE_HOST_IP IS NOT NULL, INTERNAL_ALTERNATE_HOST_IP, ALTERNATE_HOST_IP ) AS ALTERNATE_HOST_IP,
                            CASE2( INTERNAL_ALTERNATE_PORT_NO IS NOT NULL, INTERNAL_ALTERNATE_PORT_NO, ALTERNATE_PORT_NO ) AS ALTERNATE_PORT_NO
                       FROM SYS_SHARD.NODES_ A, SYS_SHARD.GLOBAL_META_INFO_ B
                      WHERE A.SMN = B.SHARD_META_NUMBER + adjustment;
        row_record c1%ROWTYPE;
    BEGIN
        PRINTLN( '[NODE_NAME] [HOST_IP] [PORT_NO] [ALTERNATE_HOST_IP] [ALTERNATE_PORT_NO]' );

        OPEN c1;
        LOOP
            FETCH c1 INTO row_record;
            EXIT WHEN c1%NOTFOUND;
            PRINT( row_record.NODE_NAME || ' ' || row_record.HOST_IP || ' ' || row_record.PORT_NO || ' ' );
            IF row_record.ALTERNATE_HOST_IP IS NULL THEN
                PRINTLN( 'N/A N/A' );
            ELSE
                PRINTLN( row_record.ALTERNATE_HOST_IP || ' ' || row_record.ALTERNATE_PORT_NO );
            END IF;
        END LOOP;
        CLOSE c1;
    END;
EXCEPTION WHEN OTHERS THEN
    code := SQLCODE;
    errm := SUBSTRING( SQLERRM, 1, 1024 );
    PRINTLN( 'SQLCODE : ' || code );
    PRINTLN( 'SQLERRM : ' || errm );
END get_connection_info;

--  Set Replication Properties
PROCEDURE set_replication_properties()
AS
    sql_buffer      VARCHAR(1024);
    code            INTEGER;
    errm            VARCHAR(1024);
BEGIN
    sql_buffer := 'ALTER SYSTEM SET REPLICATION_DDL_ENABLE = 1';
    PRINTLN( 'System Property : ' || sql_buffer );
    EXECUTE IMMEDIATE sql_buffer;

    sql_buffer := 'ALTER SYSTEM SET REPLICATION_DDL_ENABLE_LEVEL = 1';
    PRINTLN( 'System Property : ' || sql_buffer );
    EXECUTE IMMEDIATE sql_buffer;

    sql_buffer := 'ALTER SYSTEM SET REPLICATION_SQL_APPLY_ENABLE = 1';
    PRINTLN( 'System Property : ' || sql_buffer );
    EXECUTE IMMEDIATE sql_buffer;
EXCEPTION WHEN OTHERS THEN
    code := SQLCODE;
    errm := SUBSTRING( SQLERRM, 1, 1024 );
    PRINTLN( 'SQLCODE : ' || code );
    PRINTLN( 'SQLERRM : ' || errm );
END set_replication_properties;

--  Set Rebuild Data Step
PROCEDURE set_rebuild_data_step( value IN INTEGER )
AS
    sql_buffer      VARCHAR(1024);
    code            INTEGER;
    errm            VARCHAR(1024);
BEGIN
    sql_buffer := 'ALTER SYSTEM SET SHARD_REBUILD_DATA_STEP = ' || value;
    PRINTLN( 'System Property : ' || sql_buffer );
    EXECUTE IMMEDIATE sql_buffer;
EXCEPTION WHEN OTHERS THEN
    code := SQLCODE;
    errm := SUBSTRING( SQLERRM, 1, 1024 );
    PRINTLN( 'SQLCODE : ' || code );
    PRINTLN( 'SQLERRM : ' || errm );
END set_rebuild_data_step;

--  Wait Rebuild Data Step
PROCEDURE wait_rebuild_data_step( value IN INTEGER )
AS
    result          INTEGER;
    code            INTEGER;
    errm            VARCHAR(1024);
BEGIN
    SELECT MEMORY_VALUE1 INTO result FROM X$PROPERTY WHERE NAME = 'SHARD_REBUILD_DATA_STEP';

    WHILE result != value LOOP
        dbms_lock.sleep2( 0, 1000 );
        SELECT MEMORY_VALUE1 INTO result FROM X$PROPERTY WHERE NAME = 'SHARD_REBUILD_DATA_STEP';
    END LOOP;
EXCEPTION WHEN OTHERS THEN
    code := SQLCODE;
    errm := SUBSTRING( SQLERRM, 1, 1024 );
    PRINTLN( 'SQLCODE : ' || code );
    PRINTLN( 'SQLERRM : ' || errm );
END wait_rebuild_data_step;

--  Print Rebuild Data Step
PROCEDURE print_rebuild_data_step()
AS
    result          INTEGER;
    code            INTEGER;
    errm            VARCHAR(1024);
BEGIN
    SELECT MEMORY_VALUE1 INTO result FROM X$PROPERTY WHERE NAME = 'SHARD_REBUILD_DATA_STEP';
    PRINTLN( result );
EXCEPTION WHEN OTHERS THEN
    code := SQLCODE;
    errm := SUBSTRING( SQLERRM, 1, 1024 );
    PRINTLN( 'SQLCODE : ' || code );
    PRINTLN( 'SQLERRM : ' || errm );
END print_rebuild_data_step;

--  Insert Rebuild State
PROCEDURE insert_rebuild_state( value                     IN INTEGER,
                                rebuild_data_step_failure IN INTEGER )
AS
    sql_buffer      VARCHAR(1024);
    code            INTEGER;
    errm            VARCHAR(1024);
BEGIN
    sql_buffer := 'INSERT INTO SYS_SHARD.REBUILD_STATE_ VALUES ( ' || value || ' )';
    PRINTLN( 'DML : ' || sql_buffer );
    EXECUTE IMMEDIATE sql_buffer;
EXCEPTION WHEN OTHERS THEN
    code := SQLCODE;
    errm := SUBSTRING( SQLERRM, 1, 1024 );
    PRINTLN( 'SQLCODE : ' || code );
    PRINTLN( 'SQLERRM : ' || errm );

    sql_buffer := 'ALTER SYSTEM SET SHARD_REBUILD_DATA_STEP = ' || rebuild_data_step_failure;
    PRINTLN( 'Property : ' || sql_buffer );
    EXECUTE IMMEDIATE sql_buffer;
END insert_rebuild_state;

--  Copy Table Schema
PROCEDURE copy_table_schema( user_name         IN VARCHAR(128),
                             target_table_name IN VARCHAR(128),
                             source_table_name IN VARCHAR(128) )
AS
    sql_buffer      VARCHAR(1024);
    code            INTEGER;
    errm            VARCHAR(1024);
BEGIN
    sql_buffer := 'CREATE TABLE "' || user_name || '"."' || target_table_name || '" FROM TABLE SCHEMA "' ||
                                      user_name || '"."' || source_table_name || '" USING PREFIX __COPY_';
    PRINTLN( 'DDL : ' || sql_buffer );
    EXECUTE IMMEDIATE sql_buffer;
EXCEPTION WHEN OTHERS THEN
    code := SQLCODE;
    errm := SUBSTRING( SQLERRM, 1, 1024 );
    PRINTLN( 'SQLCODE : ' || code );
    PRINTLN( 'SQLERRM : ' || errm );
END copy_table_schema;

--  Create Replication
PROCEDURE create_replication( user_name          IN VARCHAR(128),
                              my_table_name      IN VARCHAR(128),
                              peer_table_name    IN VARCHAR(128),
                              partition_name     IN VARCHAR(128),
                              rep_name           IN VARCHAR(40),
                              peer_ip            IN VARCHAR(64),
                              peer_repl_port_no  IN INTEGER,
                              ddl_replicate      IN BOOLEAN,
                              propagation        IN BOOLEAN,
                              propagable_logging IN BOOLEAN )
AS
    opt_role        VARCHAR(256);
    opt_options     VARCHAR(256);
    opt_partition   VARCHAR(256);
    sql_buffer      VARCHAR(1024);
    code            INTEGER;
    errm            VARCHAR(1024);
BEGIN
    IF ( propagation = TRUE ) THEN
        opt_role := ' FOR PROPAGATION';
    ELSIF ( propagable_logging = TRUE ) THEN
        opt_role := ' FOR PROPAGABLE LOGGING';
    ELSE
        opt_role := '';
    END IF;

    IF ( ddl_replicate = TRUE ) THEN
        opt_options := ' OPTIONS DDL_REPLICATE';
    ELSE
        opt_options := '';
    END IF;

    IF ( partition_name IS NULL ) THEN
        opt_partition := '';
    ELSE
        opt_partition := ' PARTITION "' || partition_name || '"';
    END IF;

    sql_buffer := 'CREATE REPLICATION "' || rep_name || '"' || opt_role || opt_options ||
                  ' WITH ''' || peer_ip || ''', ' || peer_repl_port_no ||
                  ' FROM "' || user_name || '"."' || my_table_name   || '"' || opt_partition ||
                  ' TO "'   || user_name || '"."' || peer_table_name || '"' || opt_partition;
    PRINTLN( 'DDL : ' || sql_buffer );
    EXECUTE IMMEDIATE sql_buffer;
EXCEPTION WHEN OTHERS THEN
    code := SQLCODE;
    errm := SUBSTRING( SQLERRM, 1, 1024 );
    PRINTLN( 'SQLCODE : ' || code );
    PRINTLN( 'SQLERRM : ' || errm );
END create_replication;

--  Sync Replication
PROCEDURE sync_replication( rep_name             IN VARCHAR(40),
                            sync_parallel_factor IN INTEGER )
AS
    sql_buffer      VARCHAR(1024);
    code            INTEGER;
    errm            VARCHAR(1024);
BEGIN
    sql_buffer := 'ALTER REPLICATION "' || rep_name || '" SYNC PARALLEL ' || sync_parallel_factor;
    PRINTLN( 'DDL : ' || sql_buffer );
    EXECUTE IMMEDIATE sql_buffer;

    sql_buffer := 'ALTER REPLICATION "' || rep_name || '" FLUSH';
    PRINTLN( 'DCL : ' || sql_buffer );
    EXECUTE IMMEDIATE sql_buffer;
EXCEPTION WHEN OTHERS THEN
    code := SQLCODE;
    errm := SUBSTRING( SQLERRM, 1, 1024 );
    PRINTLN( 'SQLCODE : ' || code );
    PRINTLN( 'SQLERRM : ' || errm );
END sync_replication;

--  Start Replication
PROCEDURE start_replication( rep_name IN VARCHAR(40) )
AS
    sql_buffer      VARCHAR(1024);
    code            INTEGER;
    errm            VARCHAR(1024);
BEGIN
    sql_buffer := 'ALTER REPLICATION "' || rep_name || '" START';
    PRINTLN( 'DDL : ' || sql_buffer );
    EXECUTE IMMEDIATE sql_buffer;

    sql_buffer := 'ALTER REPLICATION "' || rep_name || '" FLUSH';
    PRINTLN( 'DCL : ' || sql_buffer );
    EXECUTE IMMEDIATE sql_buffer;
EXCEPTION WHEN OTHERS THEN
    code := SQLCODE;
    errm := SUBSTRING( SQLERRM, 1, 1024 );
    PRINTLN( 'SQLCODE : ' || code );
    PRINTLN( 'SQLERRM : ' || errm );
END start_replication;

--  Flush Replication
PROCEDURE flush_replication( rep_name IN VARCHAR(40) )
AS
    sql_buffer      VARCHAR(1024);
    code            INTEGER;
    errm            VARCHAR(1024);
BEGIN
    sql_buffer := 'ALTER REPLICATION "' || rep_name || '" FLUSH';
    PRINTLN( 'DCL : ' || sql_buffer );
    EXECUTE IMMEDIATE sql_buffer;
EXCEPTION WHEN OTHERS THEN
    code := SQLCODE;
    errm := SUBSTRING( SQLERRM, 1, 1024 );
    PRINTLN( 'SQLCODE : ' || code );
    PRINTLN( 'SQLERRM : ' || errm );
END flush_replication;

--  Stop Replication
PROCEDURE stop_replication( rep_name IN VARCHAR(40) )
AS
    sql_buffer      VARCHAR(1024);
    code            INTEGER;
    errm            VARCHAR(1024);
BEGIN
    sql_buffer := 'ALTER REPLICATION "' || rep_name || '" STOP';
    PRINTLN( 'DCL : ' || sql_buffer );
    EXECUTE IMMEDIATE sql_buffer;
EXCEPTION WHEN OTHERS THEN
    code := SQLCODE;
    errm := SUBSTRING( SQLERRM, 1, 1024 );
    PRINTLN( 'SQLCODE : ' || code );
    PRINTLN( 'SQLERRM : ' || errm );
END stop_replication;

--  Drop Replication
PROCEDURE drop_replication( rep_name IN VARCHAR(40) )
AS
    sql_buffer      VARCHAR(1024);
    code            INTEGER;
    errm            VARCHAR(1024);
BEGIN
    sql_buffer := 'DROP REPLICATION "' || rep_name || '"';
    PRINTLN( 'DDL : ' || sql_buffer );
    EXECUTE IMMEDIATE sql_buffer;
EXCEPTION WHEN OTHERS THEN
    code := SQLCODE;
    errm := SUBSTRING( SQLERRM, 1, 1024 );
    PRINTLN( 'SQLCODE : ' || code );
    PRINTLN( 'SQLERRM : ' || errm );
END drop_replication;

--  Swap Table
PROCEDURE swap_table( user_name                      IN VARCHAR(128),
                      target_table_name              IN VARCHAR(128),
                      source_table_name              IN VARCHAR(128),
                      force_to_rename_encrypt_column IN BOOLEAN DEFAULT FALSE,
                      ignore_foreign_key_child       IN BOOLEAN DEFAULT FALSE )
AS
    opt_encrypt_column      VARCHAR(128);
    opt_foreign_key_child   VARCHAR(128);
    sql_buffer              VARCHAR(1024);
    code                    INTEGER;
    errm                    VARCHAR(1024);
BEGIN
    IF ( force_to_rename_encrypt_column = TRUE ) THEN
        opt_encrypt_column := ' RENAME FORCE';
    ELSE
        opt_encrypt_column := '';
    END IF;

    IF ( ignore_foreign_key_child = TRUE ) THEN
        opt_foreign_key_child := ' IGNORE FOREIGN KEY CHILD';
    ELSE
        opt_foreign_key_child := '';
    END IF;

    sql_buffer := 'ALTER TABLE "' || user_name || '"."' || target_table_name ||
                    '" REPLACE "' || user_name || '"."' || source_table_name ||
                    '" USING PREFIX __COPY_' || opt_encrypt_column || opt_foreign_key_child;
    PRINTLN( 'DDL : ' || sql_buffer );
    EXECUTE IMMEDIATE sql_buffer;
EXCEPTION WHEN OTHERS THEN
    code := SQLCODE;
    errm := SUBSTRING( SQLERRM, 1, 1024 );
    PRINTLN( 'SQLCODE : ' || code );
    PRINTLN( 'SQLERRM : ' || errm );
END swap_table;

--  Swap Partition
PROCEDURE swap_partition( user_name         IN VARCHAR(128),
                          target_table_name IN VARCHAR(128),
                          source_table_name IN VARCHAR(128),
                          partition_name    IN VARCHAR(128) )
AS
    sql_buffer      VARCHAR(1024);
    code            INTEGER;
    errm            VARCHAR(1024);
BEGIN
    sql_buffer := 'ALTER TABLE "' || user_name || '"."' || target_table_name ||
                    '" REPLACE "' || user_name || '"."' || source_table_name ||
                    '" PARTITION "' || partition_name || '"';
    PRINTLN( 'DDL : ' || sql_buffer );
    EXECUTE IMMEDIATE sql_buffer;
EXCEPTION WHEN OTHERS THEN
    code := SQLCODE;
    errm := SUBSTRING( SQLERRM, 1, 1024 );
    PRINTLN( 'SQLCODE : ' || code );
    PRINTLN( 'SQLERRM : ' || errm );
END swap_partition;

--  Lock Table/Partition & Flush Replication & Wait & Truncate Table/Partition
PROCEDURE lock_and_truncate( user_name                 IN VARCHAR(128),
                             table_name                IN VARCHAR(128),
                             partition_name            IN VARCHAR(128),
                             rep_name_for_rebuild      IN VARCHAR(40),
                             lock_wait_seconds         IN INTEGER,
                             rebuild_data_step_locked  IN INTEGER,
                             rebuild_data_step_success IN INTEGER,
                             rebuild_data_step_failure IN INTEGER )
AS
    rebuild_step    INTEGER;
    rebuild_state   INTEGER;
    opt_partition   VARCHAR(256);
    sql_buffer      VARCHAR(1024);
    code            INTEGER;
    errm            VARCHAR(1024);
BEGIN
    IF ( partition_name IS NULL ) THEN
        opt_partition := '';
    ELSE
        opt_partition := ' PARTITION ("' || partition_name || '")';
    END IF;

    -- Lock Table/Partition
    sql_buffer := 'LOCK TABLE "' || user_name || '"."' || table_name || '"' || opt_partition ||
                   ' IN EXCLUSIVE MODE WAIT ' || lock_wait_seconds || ' SECOND UNTIL NEXT DDL';
    PRINTLN( 'DML : ' || sql_buffer );
    EXECUTE IMMEDIATE sql_buffer;

    -- Flush Replication
    sql_buffer := 'ALTER REPLICATION "' || rep_name_for_rebuild || '" FLUSH';
    PRINTLN( 'DCL : ' || sql_buffer );
    EXECUTE IMMEDIATE sql_buffer;

    sql_buffer := 'ALTER SYSTEM SET SHARD_REBUILD_DATA_STEP = ' || rebuild_data_step_locked;
    PRINTLN( 'Property : ' || sql_buffer );
    EXECUTE IMMEDIATE sql_buffer;

    -- Wait
    SELECT MEMORY_VALUE1 INTO rebuild_step FROM X$PROPERTY WHERE NAME = 'SHARD_REBUILD_DATA_STEP';
    SELECT COUNT(*) INTO rebuild_state FROM SYS_SHARD.REBUILD_STATE_ WHERE VALUE = rebuild_data_step_locked;

    WHILE ( NOT ( rebuild_step = rebuild_data_step_success AND rebuild_state = 1 ) ) AND
          ( rebuild_step != rebuild_data_step_failure ) LOOP
        dbms_lock.sleep2( 0, 1000 );
        SELECT MEMORY_VALUE1 INTO rebuild_step FROM X$PROPERTY WHERE NAME = 'SHARD_REBUILD_DATA_STEP';
        SELECT COUNT(*) INTO rebuild_state FROM SYS_SHARD.REBUILD_STATE_ WHERE VALUE = rebuild_data_step_locked;
    END LOOP;

    IF ( rebuild_step = rebuild_data_step_failure ) THEN
        ROLLBACK;
        PRINTLN( 'The table or partition is unlocked.' );
        RETURN;
    END IF;

    -- Truncate Table/Partition
    IF ( partition_name IS NULL ) THEN
        sql_buffer := 'TRUNCATE TABLE "' || user_name || '"."' || table_name || '"';
    ELSE
        sql_buffer := 'ALTER TABLE "' || user_name || '"."' || table_name || '" TRUNCATE PARTITION "' || partition_name || '"';
    END IF;

    PRINTLN( 'DDL : ' || sql_buffer );
    EXECUTE IMMEDIATE sql_buffer;
EXCEPTION WHEN OTHERS THEN
    code := SQLCODE;
    errm := SUBSTRING( SQLERRM, 1, 1024 );
    PRINTLN( 'SQLCODE : ' || code );
    PRINTLN( 'SQLERRM : ' || errm );

    sql_buffer := 'ALTER SYSTEM SET SHARD_REBUILD_DATA_STEP = ' || rebuild_data_step_failure;
    PRINTLN( 'Property : ' || sql_buffer );
    EXECUTE IMMEDIATE sql_buffer;
END lock_and_truncate;

--  Lock Table/Partition & Flush Replication & Wait & Release
PROCEDURE lock_and_release( user_name                 IN VARCHAR(128),
                            table_name                IN VARCHAR(128),
                            partition_name            IN VARCHAR(128),
                            rep_name_for_rebuild      IN VARCHAR(40),
                            lock_wait_seconds         IN INTEGER,
                            rebuild_data_step_locked  IN INTEGER,
                            rebuild_data_step_success IN INTEGER,
                            rebuild_data_step_failure IN INTEGER )
AS
    rebuild_step    INTEGER;
    rebuild_state   INTEGER;
    opt_partition   VARCHAR(256);
    sql_buffer      VARCHAR(1024);
    code            INTEGER;
    errm            VARCHAR(1024);
BEGIN
    IF ( partition_name IS NULL ) THEN
        opt_partition := '';
    ELSE
        opt_partition := ' PARTITION ("' || partition_name || '")';
    END IF;

    -- Lock Table/Partition
    sql_buffer := 'LOCK TABLE "' || user_name || '"."' || table_name || '"' || opt_partition ||
                   ' IN EXCLUSIVE MODE WAIT ' || lock_wait_seconds || ' SECOND UNTIL NEXT DDL';
    PRINTLN( 'DML : ' || sql_buffer );
    EXECUTE IMMEDIATE sql_buffer;

    -- Flush Replication
    sql_buffer := 'ALTER REPLICATION "' || rep_name_for_rebuild || '" FLUSH';
    PRINTLN( 'DCL : ' || sql_buffer );
    EXECUTE IMMEDIATE sql_buffer;

    sql_buffer := 'ALTER SYSTEM SET SHARD_REBUILD_DATA_STEP = ' || rebuild_data_step_locked;
    PRINTLN( 'Property : ' || sql_buffer );
    EXECUTE IMMEDIATE sql_buffer;

    -- Wait
    SELECT MEMORY_VALUE1 INTO rebuild_step FROM X$PROPERTY WHERE NAME = 'SHARD_REBUILD_DATA_STEP';
    SELECT COUNT(*) INTO rebuild_state FROM SYS_SHARD.REBUILD_STATE_ WHERE VALUE = rebuild_data_step_locked;

    WHILE ( NOT ( rebuild_step = rebuild_data_step_success AND rebuild_state = 1 ) ) AND
          ( rebuild_step != rebuild_data_step_failure ) LOOP
        dbms_lock.sleep2( 0, 1000 );
        SELECT MEMORY_VALUE1 INTO rebuild_step FROM X$PROPERTY WHERE NAME = 'SHARD_REBUILD_DATA_STEP';
        SELECT COUNT(*) INTO rebuild_state FROM SYS_SHARD.REBUILD_STATE_ WHERE VALUE = rebuild_data_step_locked;
    END LOOP;

    IF ( rebuild_step = rebuild_data_step_failure ) THEN
        ROLLBACK;
        PRINTLN( 'The table or partition is unlocked.' );
        RETURN;
    END IF;

    -- Release
    sql_buffer := 'COMMIT';
    PRINTLN( 'DCL : ' || sql_buffer );
    EXECUTE IMMEDIATE sql_buffer;
EXCEPTION WHEN OTHERS THEN
    code := SQLCODE;
    errm := SUBSTRING( SQLERRM, 1, 1024 );
    PRINTLN( 'SQLCODE : ' || code );
    PRINTLN( 'SQLERRM : ' || errm );

    sql_buffer := 'ALTER SYSTEM SET SHARD_REBUILD_DATA_STEP = ' || rebuild_data_step_failure;
    PRINTLN( 'Property : ' || sql_buffer );
    EXECUTE IMMEDIATE sql_buffer;
END lock_and_release;

--  Lock Table/Partition & Wait & Swap Table/Partition & Flush Replication
PROCEDURE lock_and_swap( user_name                 IN VARCHAR(128),
                         target_table_name         IN VARCHAR(128),
                         source_table_name         IN VARCHAR(128),
                         partition_name            IN VARCHAR(128),
                         rep_name_for_alternate    IN VARCHAR(40),
                         lock_wait_seconds         IN INTEGER,
                         rebuild_data_step_locked  IN INTEGER,
                         rebuild_data_step_success IN INTEGER,
                         rebuild_data_step_failure IN INTEGER )
AS
    rebuild_step    INTEGER;
    is_natc         INTEGER;
    opt_partition   VARCHAR(256);
    sql_buffer      VARCHAR(1024);
    code            INTEGER;
    errm            VARCHAR(1024);
BEGIN
    IF ( partition_name IS NULL ) THEN
        opt_partition := '';
    ELSE
        opt_partition := ' PARTITION ("' || partition_name || '")';
    END IF;

    -- Lock Table/Partition
    sql_buffer := 'LOCK TABLE "' || user_name || '"."' || target_table_name || '"' || opt_partition ||
                   ' IN EXCLUSIVE MODE WAIT ' || lock_wait_seconds || ' SECOND UNTIL NEXT DDL';
    PRINTLN( 'DML : ' || sql_buffer );
    EXECUTE IMMEDIATE sql_buffer;

    sql_buffer := 'ALTER SYSTEM SET SHARD_REBUILD_DATA_STEP = ' || rebuild_data_step_locked;
    PRINTLN( 'Property : ' || sql_buffer );
    EXECUTE IMMEDIATE sql_buffer;

    -- Wait
    SELECT MEMORY_VALUE1 INTO rebuild_step FROM X$PROPERTY WHERE NAME = 'SHARD_REBUILD_DATA_STEP';

    WHILE ( rebuild_step != rebuild_data_step_success ) AND
          ( rebuild_step != rebuild_data_step_failure ) LOOP
        dbms_lock.sleep2( 0, 1000 );
        SELECT MEMORY_VALUE1 INTO rebuild_step FROM X$PROPERTY WHERE NAME = 'SHARD_REBUILD_DATA_STEP';
    END LOOP;

    IF ( rebuild_step = rebuild_data_step_failure ) THEN
        ROLLBACK;
        PRINTLN( 'The table or partition is unlocked.' );
        RETURN;
    END IF;

    SELECT CASE2( MEMORY_VALUE1 != '0', 1, 0 ) INTO is_natc FROM X$PROPERTY WHERE NAME = '__DISPLAY_PLAN_FOR_NATC';

    IF ( is_natc = 1 ) THEN
        dbms_lock.sleep2( rebuild_data_step_locked * 2, 0 );
    END IF;

    -- Swap Table/Partition
    IF ( partition_name IS NULL ) THEN
        sql_buffer := 'ALTER TABLE "' || user_name || '"."' || source_table_name ||
                        '" REPLACE "' || user_name || '"."' || target_table_name ||
                        '" USING PREFIX __COPY_';
    ELSE
        sql_buffer := 'ALTER TABLE "' || user_name || '"."' || source_table_name ||
                        '" REPLACE "' || user_name || '"."' || target_table_name ||
                        '" PARTITION "' || partition_name || '"';
    END IF;

    PRINTLN( 'DDL : ' || sql_buffer );
    EXECUTE IMMEDIATE sql_buffer;

    -- Flush Replication
    IF ( rep_name_for_alternate IS NOT NULL ) THEN
        sql_buffer := 'ALTER REPLICATION "' || rep_name_for_alternate || '" FLUSH';
        PRINTLN( 'DCL : ' || sql_buffer );
        EXECUTE IMMEDIATE sql_buffer;
    END IF;
EXCEPTION WHEN OTHERS THEN
    code := SQLCODE;
    errm := SUBSTRING( SQLERRM, 1, 1024 );
    PRINTLN( 'SQLCODE : ' || code );
    PRINTLN( 'SQLERRM : ' || errm );

    sql_buffer := 'ALTER SYSTEM SET SHARD_REBUILD_DATA_STEP = ' || rebuild_data_step_failure;
    PRINTLN( 'Property : ' || sql_buffer );
    EXECUTE IMMEDIATE sql_buffer;
END lock_and_swap;

-- check
PROCEDURE CHECK_PROPERTY( aCheck_property_name IN  VARCHAR(40),
                          aExpect_value        IN  INTEGER )
AS
value INTEGER;
BEGIN
    SELECT MEMORY_VALUE1 INTO VALUE 
    FROM X$PROPERTY 
    WHERE NAME = aCheck_property_name;

    PRINT( aCheck_property_name || ' : ' || VALUE );
    IF value = aExpect_value THEN
        PRINTLN( ' -> Success' );
    ELSE

        PRINTLN( ' -> Failure ( EXPECT VALUE : ' || aExpect_value || ' )' );
    END IF;
END CHECK_PROPERTY;

PROCEDURE CHECK_EXIST_REPLICATION( aReplication_name IN     VARCHAR(48),
                                   aExpect_exist     IN     BOOLEAN )
AS
count           INTEGER;
expect_count    INTEGER;
BEGIN

    SELECT COUNT(*) INTO count
    FROM SYSTEM_.SYS_REPLICATIONS_
    WHERE REPLICATION_NAME = aReplication_name;

    PRINT( aReplication_name );
    IF count = 1 THEN
        PRINT( ' exists.' );
    ELSE
        PRINT( ' does not exist.' );
    END IF;

    IF aExpect_exist = TRUE THEN
        expect_count := 1;
    ELSE
        expect_count := 0;
    END IF;

    IF count = expect_count THEN
        PRINTLN( ' -> Success' );
    ELSE
        PRINTLN( ' -> Failure' );
    END IF;
END CHECK_EXIST_REPLICATION;

PROCEDURE CHECK_EXIST_TABLE( aUser_name      IN  VARCHAR(128),
                             aTable_name     IN  VARCHAR(128),
                             aExpect_exist   IN  BOOLEAN )
AS
count        INTEGER;
expect_count INTEGER;
BEGIN

    SELECT COUNT(*)
    INTO count
    FROM SYSTEM_.SYS_TABLES_           T,
         SYSTEM_.SYS_USERS_            U
    WHERE U.USER_ID = T.USER_ID      AND
          U.USER_NAME = aUser_name   AND
          T.TABLE_NAME = aTable_name;

    PRINT( aUser_name || '.' ||  aTable_name );

    IF count = 1 THEN
        PRINT( ' exists. ' );
    ELSE
        PRINT( ' does not exist. ' );
    END IF;

    IF aExpect_exist = TRUE THEN
        expect_count := 1;
    ELSE
        expect_count := 0;
    END IF;

    IF expect_count = count THEN
        PRINTLN( '-> Success' );
    ELSE
        PRINTLN( '-> Failure' );
    END IF;
END CHECK_EXIST_TABLE;


PROCEDURE CHECK_EXIST_PARTITION( aUser_name      IN   VARCHAR(128),
                                 aTable_name     IN   VARCHAR(128),
                                 aPartition_name IN   VARCHAR(128),
                                 aExpect_exist   IN   BOOLEAN )
AS
count        INTEGER;
expect_count INTEGER;
BEGIN

    SELECT COUNT(*)
    INTO count
    FROM SYSTEM_.SYS_TABLES_           T,
         SYSTEM_.SYS_USERS_            U,
         SYSTEM_.SYS_TABLE_PARTITIONS_ P
    WHERE U.USER_ID = T.USER_ID      AND
          U.USER_ID = P.USER_ID      AND
          T.TABLE_ID = P.TABLE_ID    AND
          U.USER_NAME = aUser_name   AND
          T.TABLE_NAME = aTable_name AND
          P.PARTITION_NAME = aPartition_name;

    PRINT( aUser_name || '.' ||  aTable_name || '.' || aPartition_name );

    IF count = 1 THEN
        PRINT( ' exists. ' );
    ELSE
        PRINT( ' does not exist. ' );
    END IF;

    IF aExpect_exist = TRUE THEN
        expect_count := 1;
    ELSE
        expect_count := 0;
    END IF;

    IF expect_count = count THEN
        PRINTLN( '-> Success' );
    ELSE
        PRINTLN( '-> Failure' );
    END IF;
END CHECK_EXIST_PARTITION;

PROCEDURE CHECK_EXIST_NODE( aNode_name      IN  VARCHAR(40),
                            aExpect_exist   IN  BOOLEAN )
AS
count        INTEGER;
expect_count INTEGER;
BEGIN

    SELECT COUNT(*)
    INTO count
    FROM SYS_SHARD.NODES_ N,
         SYS_SHARD.GLOBAL_META_INFO_ G
    WHERE N.SMN = G.SHARD_META_NUMBER AND
          N.NODE_NAME = aNode_name;

    PRINT( aNode_name );
    IF count > 0 THEN
        PRINT( ' exists.' );
    ELSE
        PRINT( ' does not exist.' );
    END IF;

    IF aExpect_exist = TRUE THEN
        expect_count := 1;
    ELSE
        expect_count := 0;
    END IF;

    IF count = expect_count THEN
        PRINTLN( ' -> Success' );
    ELSE
        PRINTLN( ' -> Failure' );
    END IF;

END CHECK_EXIST_NODE;

PROCEDURE CHECK_REPLICATION_GAP( replication_name IN VARCHAR(40) )
AS
gap BIGINT;
CURSOR c1 IS SELECT REP_GAP
             FROM V$REPGAP
             WHERE REP_NAME = replication_name;
BEGIN
    OPEN c1;
    FETCH c1 INTO gap;

    IF c1%ROWCOUNT > 0 THEN
        PRINTLN( 'Replication ( ' || REPLICATION_NAME || ' ) gap is ' || gap );
    ELSE
        PRINTLN( REPLICATION_NAME || ' does not exist.' );
    END IF;

    CLOSE c1;

END CHECK_REPLICATION_GAP;

PROCEDURE CHECK_REPLICATION_ROLE( aReplication_name IN  VARCHAR(40),
                                  aExpect_role      IN  INTEGER )
AS
role INTEGER;
CURSOR c1 IS SELECT ROLE
             FROM SYSTEM_.SYS_REPLICATIONS_
             WHERE REPLICATION_NAME = aReplication_name;
BEGIN
    OPEN c1;
    FETCH c1 INTO role;

    PRINT( aReplication_name );

    IF c1%ROWCOUNT > 0 THEN
        CASE role
            WHEN 0 THEN
                PRINT( ' is NORMAL' );
            WHEN 1 THEN
                PRINT( ' is ANALYSIS' );
            WHEN 2 THEN
                PRINT( ' is PROPAGABLE_LOGGING' );
            WHEN 3 THEN
                PRINT( ' is PROPAGATION' );
            WHEN 4 THEN
                PRINT( ' is ANALYSIS PROPAGATION' );
            ELSE
                PRINT( ' is unkown role' );
        END CASE;

        IF role = aExpect_role THEN
            PRINTLN( ' -> Success' );
        ELSE
            PRINTLN( ' -> Failure' );
        END IF;
    ELSE
        PRINTLN( ' does not exit -> Failure');
    END IF;

    CLOSE C1;

END CHECK_REPLICATION_ROLE;

PROCEDURE CHECK_PARTITION_AND_SHARD_METHOD( aUser_name  IN  VARCHAR(128),
                                            aTable_name IN  VARCHAR(128) )
AS
partition_method   CHAR(1);
shard_split_method CHAR(1);

CURSOR c1 IS SELECT O.SPLIT_METHOD
             INTO shard_split_method
             FROM SYS_SHARD.OBJECTS_          O,
                  SYS_SHARD.GLOBAL_META_INFO_ G
             WHERE O.SMN = G.SHARD_META_NUMBER AND
                   USER_NAME = aUser_name AND
                   OBJECT_NAME = aTable_name;

CURSOR c2 IS SELECT DECODE( P.PARTITION_METHOD,
                            0, 'R',
                            2, 'L',
                            3, 'H',
                            '?' )
             INTO partition_method
             FROM SYSTEM_.SYS_TABLES_      T,
                  SYSTEM_.SYS_USERS_       U,
                  SYSTEM_.SYS_PART_TABLES_ P
             WHERE U.USER_ID = T.USER_ID     AND
                   U.USER_ID = P.USER_ID     AND
                   T.TABLE_ID = P.TABLE_ID   AND
                   U.USER_NAME = aUser_name  AND
                   T.TABLE_NAME = aTable_name;
BEGIN
    OPEN c1;
    OPEN c2;

    FETCH c1 INTO shard_split_method;
    FETCH c2 INTO partition_method;

    PRINT( 'SHARD and PARTITION METHOD : ' );

    IF c1%NOTFOUND THEN
        PRINTLN( 'different( Partition does not exist ) -> Failure' );
    ELSIF c2%NOTFOUND THEN
        PRINTLN( 'different( Shard Object does not exist ) -> Failure' );
    ELSIF partition_method = shard_split_method THEN
        PRINTLN( 'same( ' || shard_split_method || ' )' || ' -> Success' );
    ELSE
        PRINTLN( 'different( PARTITION : ' || partition_method ||
                 ', SHARD SPLIT : ' || shard_split_method || ' ) -> Failure' );
    END IF;

    CLOSE c1;
    CLOSE c2;

END CHECK_PARTITION_AND_SHARD_METHOD;

PROCEDURE CHECK_PARTITION_AND_SHARD_KEY_COLUMN( aUser_name    IN    VARCHAR(128),
                                                aTable_name   IN    VARCHAR(128) )
AS
CURSOR partition_key_column_cursor IS
SELECT C.COLUMN_NAME
FROM SYSTEM_.SYS_TABLES_           T,
     SYSTEM_.SYS_USERS_            U,
     SYSTEM_.SYS_PART_KEY_COLUMNS_ K,
     SYSTEM_.SYS_COLUMNS_          C
WHERE U.USER_ID = T.USER_ID           AND
      U.USER_ID = K.USER_ID           AND
      T.TABLE_ID = K.PARTITION_OBJ_ID AND
      K.COLUMN_ID = C.COLUMN_ID       AND
      U.USER_NAME = aUser_name        AND
      T.TABLE_NAME = aTable_name
GROUP BY C.COLUMN_NAME
ORDER BY C.COLUMN_NAME;

CURSOR split_key_column_cursor IS
SELECT O.KEY_COLUMN_NAME
FROM SYS_SHARD.OBJECTS_ O,
     SYS_SHARD.GLOBAL_META_INFO_ G
WHERE O.SMN = G.SHARD_META_NUMBER AND
      O.USER_NAME = aUser_name    AND
      O.OBJECT_NAME = aTable_name
GROUP BY O.KEY_COLUMN_NAME
ORDER BY O.KEY_COLUMN_NAME;

partition_key_column VARCHAR(128);
split_key_column     VARCHAR(128);

BEGIN
    OPEN partition_key_column_cursor;
    OPEN split_key_column_cursor;

    LOOP
        FETCH partition_key_column_cursor INTO partition_key_column;
        FETCH split_key_column_cursor INTO split_key_column;

        EXIT WHEN partition_key_column_cursor%NOTFOUND;
        EXIT WHEN split_key_column_cursor%NOTFOUND;

        IF partition_key_column = split_key_column THEN
            PRINTLN( 'Key Column( ' || partition_key_column || ' ) is same -> Success' );
        ELSE
            PRINTLN( 'Key Column( Partition : ' || partition_key_column ||
                     ' , Shard : ' || split_key_column ||
                     ' ) is different -> Failure' );
        END IF;
    END LOOP;

    IF partition_key_column_cursor%ROWCOUNT > 1 OR
       partition_key_column_cursor%ROWCOUNT = 0 THEN
         PRINTLN( 'PARTITION KEY COLUMN COUNT is not 1 -> Failure' );
    END IF;

    IF split_key_column_cursor%ROWCOUNT > 1 OR
       split_key_column_cursor%ROWCOUNT = 0 THEN
         PRINTLN( 'SHARD SPLIT KEY COLUMN COUNT is not 1 -> Failure' );
    END IF;

    CLOSE partition_key_column_cursor;
    CLOSE split_key_column_cursor;
END CHECK_PARTITION_AND_SHARD_KEY_COLUMN;

PROCEDURE CHECK_PARTITION_AND_SHARD_KEY_VALUE( aUser_name    IN  VARCHAR(128),
                                               aTable_name   IN  VARCHAR(128) )
AS
CURSOR partition_key_value_cursor IS
SELECT P.PARTITION_MAX_VALUE
FROM SYSTEM_.SYS_TABLES_           T,
     SYSTEM_.SYS_USERS_            U,
     SYSTEM_.SYS_TABLE_PARTITIONS_ P
WHERE U.USER_ID = T.USER_ID           AND
      U.USER_ID = P.USER_ID           AND
      T.TABLE_ID = P.TABLE_ID         AND
      U.USER_NAME = aUser_name        AND
      T.TABLE_NAME = aTable_name      AND
      P.PARTITION_NAME IS NOT NULL
ORDER BY P.PARTITION_MAX_VALUE;

CURSOR shard_key_value_cursor IS
SELECT R.VALUE, R.NODE_ID, O.SPLIT_METHOD
FROM  SYS_SHARD.OBJECTS_          O,
      SYS_SHARD.RANGES_           R,
      SYS_SHARD.GLOBAL_META_INFO_ G
WHERE O.SHARD_ID = R.SHARD_ID         AND
      O.SMN = R.SMN                   AND
      O.SMN = G.SHARD_META_NUMBER     AND
      O.USER_NAME = aUser_name        AND
      O.OBJECT_NAME = aTable_name
ORDER BY R.VALUE, R.NODE_ID, O.SPLIT_METHOD;

partition_key_value                VARCHAR(4000);
shard_key_value                    VARCHAR(4000);
shard_node_ID                      INTEGER;
prev_shard_node_ID                 INTEGER;
shard_split_method                 CHAR(1);
need_single_quotation_for_partiton BOOLEAN;
need_single_quotation_for_shard    BOOLEAN;

need_fetch_for_partition           BOOLEAN;
nedd_fetch_for_shard               BOOLEAN;

convert_partition_key_value        VARCHAR(4000);
convert_shard_key_value            VARCHAR(4000);

is_matched                         BOOLEAN;
BEGIN

    OPEN partition_key_value_cursor;
    OPEN shard_key_value_cursor;

    need_fetch_for_partition := TRUE;
    nedd_fetch_for_shard := TRUE;

    LOOP
        IF need_fetch_for_partition = TRUE THEN
            FETCH partition_key_value_cursor INTO partition_key_value;
            need_fetch_for_partition := FALSE;
        END IF;

        IF nedd_fetch_for_shard = TRUE THEN
            FETCH shard_key_value_cursor INTO shard_key_value, shard_node_ID, shard_split_method;
            nedd_fetch_for_shard := FALSE;
        END IF;

        EXIT WHEN partition_key_value_cursor%NOTFOUND;
        EXIT WHEN shard_key_value_cursor%NOTFOUND;

        need_single_quotation_for_partiton := FALSE;
        need_single_quotation_for_shard := FALSE;
        IF INSTR( partition_key_value, CHR(39) ) = 1 THEN
            IF INSTR( shard_key_value, CHR(39) ) != 1 THEN
                need_single_quotation_for_shard := TRUE;
            END IF;
        ELSE
            IF INSTR( shard_key_value, CHR(39) ) = 1 THEN
                need_single_quotation_for_partiton := TRUE;
            END IF;
        END IF;

        IF need_single_quotation_for_partiton = TRUE THEN
            convert_partition_key_value := CHR(39) || partition_key_value || CHR(39);
        ELSE
            convert_partition_key_value := partition_key_value;
        END IF;

        IF need_single_quotation_for_shard = TRUE THEN
            convert_shard_key_value := CHR(39) || shard_key_value || CHR(39);
        ELSE
            convert_shard_key_value := shard_key_value;
        END IF;

        IF convert_partition_key_value = convert_shard_key_value THEN
            is_matched := TRUE;
            need_fetch_for_partition := TRUE;
            nedd_fetch_for_shard := TRUE;
            prev_shard_node_ID := NULL;
        ELSEIF convert_partition_key_value < convert_shard_key_value THEN

            IF shard_split_method = 'R' OR
               shard_split_method = 'H' THEN

               IF prev_shard_node_ID IS NULL OR
                  prev_shard_node_ID = shard_node_ID THEN

                    need_fetch_for_partition := TRUE;
                    prev_shard_node_ID := shard_node_ID;
                ELSE
                    is_matched := FALSE;
                    EXIT;
                END IF;
            ELSE
                is_matched := FALSE;
                EXIT;
            END IF;
        ELSE
            is_matched := FALSE;
            EXIT;
        END IF;
    END LOOP;

    IF is_matched = TRUE THEN
        PRINTLN( 'Partition key value and sharding key value are matched -> Success' );
    ELSE
        PRINTLN( 'Partition key value and sharding key value are not matched -> Failure' );
    END IF;

    CLOSE partition_key_value_cursor;
    CLOSE shard_key_value_cursor;

END CHECK_PARTITION_AND_SHARD_KEY_VALUE;

PROCEDURE CHECK_EXIST_CLONE( aUser_name    IN    VARCHAR(128),
                             aTable_name   IN    VARCHAR(128),
                             aExpect_exist IN    BOOLEAN )
AS
CNT             INTEGER;
EXPECT_COUNT    INTEGER;
BEGIN
    SELECT COUNT(*) INTO CNT
    FROM SYS_SHARD.OBJECTS_          O,
         SYS_SHARD.GLOBAL_META_INFO_ G
    WHERE
        O.SMN = G.SHARD_META_NUMBER AND
        SPLIT_METHOD = 'C'          AND
        O.USER_NAME = aUser_name    AND
        O.OBJECT_NAME = aTable_name;

    IF aExpect_exist = TRUE THEN
        EXPECT_COUNT := 1;
    ELSE
        EXPECT_COUNT := 0;
    END IF;

    IF CNT = 0 THEN
        PRINT( aUser_name || '.' || aTable_name || ' is not a clone table' );
    ELSE
        PRINT( aUser_name || '.' || aTable_name || ' is a clone table' );
    END IF;

    IF CNT = EXPECT_COUNT THEN
        PRINTLN( ' -> Success' );
    ELSE
        PRINTLN( ' -> Failure' );
    END IF;

END CHECK_EXIST_CLONE;

PROCEDURE CHECK_CLONE( aUser_name    IN    VARCHAR(128),
                       aTable_name   IN    VARCHAR(128),
                       aNode_name    IN    VARCHAR(40),
                       aExpect_exist IN    BOOLEAN )
AS
CNT          INTEGER;
EXPECT_COUNT INTEGER;
BEGIN
    SELECT COUNT(*) INTO CNT
    FROM SYS_SHARD.OBJECTS_          O,
         SYS_SHARD.CLONES_           C,
         SYS_SHARD.NODES_            N,
         SYS_SHARD.GLOBAL_META_INFO_ G
    WHERE
        O.SMN = G.SHARD_META_NUMBER AND
        C.SMN = G.SHARD_META_NUMBER AND
        N.SMN = G.SHARD_META_NUMBER AND
        O.SPLIT_METHOD = 'C'        AND
        C.SHARD_ID = O.SHARD_ID     AND
        C.NODE_ID = N.NODE_ID       AND
        N.NODE_NAME = aNode_name    AND
        O.USER_NAME = aUser_name    AND
        O.OBJECT_NAME = aTable_name;

    IF aExpect_exist = TRUE THEN
        EXPECT_COUNT := 1;
    ELSE
        EXPECT_COUNT := 0;
    END IF;

    IF CNT = 0 THEN
        PRINT( aUser_name || '.' || aTable_name || ' is not in ' || aNode_name );
    ELSE
        PRINT( aUser_name || '.' || aTable_name || ' is in ' || aNode_name );
    END IF;

    IF CNT = EXPECT_COUNT THEN
        PRINTLN( ' -> Success' );
    ELSE
        PRINTLN( ' -> Failure' );
    END IF;

END CHECK_CLONE;

PROCEDURE CHECK_EXIST_SOLO( aUser_name    IN    VARCHAR(128),
                            aTable_name   IN    VARCHAR(128),
                            aExpect_exist IN    BOOLEAN )
AS
CNT          INTEGER;
EXPECT_COUNT INTEGER;
BEGIN
    SELECT COUNT(*) INTO CNT
    FROM SYS_SHARD.OBJECTS_          O,
         SYS_SHARD.GLOBAL_META_INFO_ G
    WHERE
        O.SMN = G.SHARD_META_NUMBER AND
        SPLIT_METHOD = 'S'          AND
        O.USER_NAME = aUser_name    AND
        O.OBJECT_NAME = aTable_name;

    IF aExpect_exist = TRUE THEN
        EXPECT_COUNT := 1;
    ELSE
        EXPECT_COUNT := 0;
    END IF;

    IF CNT = 0 THEN
        PRINT( aUser_name || '.' || aTable_name || ' is not a solo table' );
    ELSE
        PRINT( aUser_name || '.' || aTable_name || ' is solo a table' );
    END IF;

    IF CNT = EXPECT_COUNT THEN
        PRINTLN( ' -> Success' );
    ELSE
        PRINTLN( ' -> Failure' );
    END IF;

END CHECK_EXIST_SOLO;

PROCEDURE CHECK_SOLO( aUser_name    IN    VARCHAR(128),
                      aTable_name   IN    VARCHAR(128),
                      aNode_name    IN    VARCHAR(40),
                      aExpect_exist IN    BOOLEAN )
AS
CNT          INTEGER;
EXPECT_COUNT INTEGER;
BEGIN
    SELECT COUNT(*) INTO CNT
    FROM SYS_SHARD.OBJECTS_          O,
         SYS_SHARD.SOLOS_            S,
         SYS_SHARD.NODES_            N,
         SYS_SHARD.GLOBAL_META_INFO_ G
    WHERE
        O.SMN = G.SHARD_META_NUMBER AND
        S.SMN = G.SHARD_META_NUMBER AND
        N.SMN = G.SHARD_META_NUMBER AND
        O.SPLIT_METHOD = 'S'        AND
        S.SHARD_ID = O.SHARD_ID     AND
        S.NODE_ID = N.NODE_ID       AND
        N.NODE_NAME = aNode_name    AND
        O.USER_NAME = aUser_name    AND
        O.OBJECT_NAME = aTable_name;

    IF aExpect_exist = TRUE THEN
        EXPECT_COUNT := 1;
    ELSE
        EXPECT_COUNT := 0;
    END IF;

    IF CNT = 0 THEN
        PRINT( aUser_name || '.' || aTable_name || ' is not in ' || aNode_name );
    ELSE
        PRINT( aUser_name || '.' || aTable_name || ' is in ' || aNode_name );
    END IF;

    IF CNT = EXPECT_COUNT THEN
        PRINTLN( ' -> Success' );
    ELSE
        PRINTLN( ' -> Failure' );
    END IF;

END CHECK_SOLO;

PROCEDURE CHECK_ALL_PROPERTY()
AS
BEGIN

    CHECK_PROPERTY( 'REPLICATION_DDL_ENABLE', 1 );
    CHECK_PROPERTY( 'REPLICATION_DDL_ENABLE_LEVEL', 1 );
    CHECK_PROPERTY( 'REPLICATION_SQL_APPLY_ENABLE', 1 );
    CHECK_PROPERTY( 'REPLICATION_ALLOW_DUPLICATE_HOSTS', 1 );
    CHECK_PROPERTY( 'REPLICATION_SYNC_LOCK_TIMEOUT', 0 );
    CHECK_PROPERTY( 'SHARD_ENABLE', 1 );
    CHECK_PROPERTY( 'SHARD_META_HISTORY_AUTO_PURGE_DISABLE', 1 );
    CHECK_PROPERTY( 'SHARD_ALLOW_OLD_SMN', 1 );
    CHECK_PROPERTY( 'SHARD_REBUILD_LOCK_TABLE_WITH_DML_ENABLE', 1 );

END CHECK_ALL_PROPERTY;

PROCEDURE CHECK_GLOBAL_META_INFO()
AS
COUNT INTEGER;
BEGIN
    SELECT COUNT(*) INTO COUNT 
    FROM SYS_SHARD.GLOBAL_META_INFO_;

    IF COUNT = 1 THEN
        PRINTLN( 'SYS_SHARD.GLOBAL_META_INFO_ table has 1 row. -> Success' );
    ELSE
        PRINTLN( 'SYS_SHARD.GLOBAL_META_INFO_ table have ' || COUNT || ' row. -> Failure' );
    END IF;

END CHECK_GLOBAL_META_INFO;

-- DONE

-- INFO
PROCEDURE GET_NODE_INFO
AS
CURSOR c1 IS
SELECT N.NODE_NAME,
       N.HOST_IP,
       N.PORT_NO,
       NVL( N.ALTERNATE_HOST_IP, '-' ),
       NVL( TO_CHAR( N.ALTERNATE_PORT_NO ), '-' ),
       N.SMN
FROM SYS_SHARD.NODES_ N,
     SYS_SHARD.GLOBAL_META_INFO_ G
WHERE N.SMN = G.SHARD_META_NUMBER
ORDER BY N.NODE_NAME;

node_name         VARCHAR(40);
host_ip           VARCHAR(64);
port_no           INTEGER;
alternate_host_ip VARCHAR(64);
alternate_port_no VARCHAR(10);
smn               INTEGER;

BEGIN

    PRINTLN( '[GET NODE INFO]' );
    OPEN c1;

    LOOP
        FETCH c1 INTO node_name,
                      host_ip,
                      port_no,
                      alternate_host_ip,
                      alternate_port_no,
                      smn;
        EXIT WHEN c1%NOTFOUND;

        PRINTLN( node_name || ' ' || host_ip || ' ' || port_no || ' ' ||
                 alternate_host_ip || ' ' || alternate_port_no || ' ' ||
                 smn );
    END LOOP;

    IF c1%ROWCOUNT = 0 THEN
        PRINTLN( 'Nothing' );
    END IF;

    CLOSE c1;
END GET_NODE_INFO;

PROCEDURE GET_RANGE_INFO
AS
CURSOR c1 IS
SELECT O.SHARD_ID,
       O.USER_NAME,
       O.OBJECT_NAME,
       O.SPLIT_METHOD,
       R.VALUE,
       N.NODE_NAME,
       R.SMN
FROM SYS_SHARD.RANGES_           R,
     SYS_SHARD.OBJECTS_          O,
     SYS_SHARD.NODES_            N,
     SYS_SHARD.GLOBAL_META_INFO_ G
WHERE R.SHARD_ID = O.SHARD_ID AND
      R.NODE_ID = N.NODE_ID   AND
      R.SMN = O.SMN           AND
      R.SMN = N.SMN           AND
      R.SMN = G.SHARD_META_NUMBER
ORDER BY R.SHARD_ID,
         O.USER_NAME,
         O.OBJECT_NAME,
         R.VALUE,
         N.NODE_NAME;

shard_id          INTEGER;
user_name         VARCHAR(128);
object_name       VARCHAR(128);
split_method      CHAR(1);
value             VARCHAR(100);
node_name         VARCHAR(40);
smn               INTEGER;

BEGIN

    PRINTLN( '[GET RANGE INFO]' );
    OPEN c1;

    LOOP
        FETCH c1 INTO shard_id,
                      user_name,
                      object_name,
                      split_method,
                      value,
                      node_name,
                      smn;

        EXIT WHEN c1%NOTFOUND;

        PRINTLN( shard_id || ' ' || user_name || ' ' || object_name || ' ' ||
                 split_method || ' ' || value || ' ' ||
                 node_name || ' ' || smn );
    END LOOP;

    IF c1%ROWCOUNT = 0 THEN
        PRINTLN( 'Nothing' );
    END IF;

    CLOSE c1;
END GET_RANGE_INFO;

PROCEDURE GET_CLONE_INFO
AS
CURSOR c1 IS
SELECT C.SHARD_ID,
       O.USER_NAME,
       O.OBJECT_NAME,
       O.SPLIT_METHOD,
       N.NODE_NAME,
       C.SMN
FROM SYS_SHARD.CLONES_           C,
     SYS_SHARD.OBJECTS_          O,
     SYS_SHARD.NODES_            N,
     SYS_SHARD.GLOBAL_META_INFO_ G
WHERE C.SHARD_ID = O.SHARD_ID AND
      C.NODE_ID = N.NODE_ID   AND
      C.SMN = O.SMN           AND
      C.SMN = N.SMN           AND
      C.SMN = G.SHARD_META_NUMBER
ORDER BY C.SHARD_ID,
         O.USER_NAME,
         O.SPLIT_METHOD,
         N.NODE_NAME;

shard_id          INTEGER;
user_name         VARCHAR(128);
object_name       VARCHAR(128);
split_method      CHAR(1);
node_name         VARCHAR(40);
smn               INTEGER;

BEGIN

    PRINTLN( '[GET CLONE INFO]' );
    OPEN c1;

    LOOP
        FETCH c1 INTO shard_id,
                      user_name,
                      object_name,
                      split_method,
                      node_name,
                      smn;

        EXIT WHEN c1%NOTFOUND;

        PRINTLN( shard_id || ' ' || user_name || ' ' || object_name || ' ' ||
                 split_method || ' ' || node_name || ' ' || smn );
    END LOOP;

    IF c1%ROWCOUNT = 0 THEN
        PRINTLN( 'Nothing' );
    END IF;

    CLOSE c1;
END GET_CLONE_INFO;

PROCEDURE GET_SHARD_INFO
AS
BEGIN
    GET_NODE_INFO;
    GET_RANGE_INFO;
    GET_CLONE_INFO;
END GET_SHARD_INFO;

PROCEDURE SET_NEXT_SHARD_ID( aSequenceNo    IN BIGINT )
AS
start_value     BIGINT;
current_value   BIGINT;
increase_value  BIGINT;
max_value       BIGINT;
sql_string      VARCHAR(1024);
code            INTEGER;
errm            VARCHAR(1024);
BEGIN
    SELECT S.START_SEQ, S.CURRENT_SEQ, S.MAX_SEQ
    INTO start_value, current_value, max_value
    FROM SYSTEM_.SYS_TABLES_ T,
         V$SEQ               S
    WHERE T.TABLE_OID = S.SEQ_OID AND
          T.TABLE_NAME = 'NEXT_SHARD_ID';

    IF current_value IS null THEN
        current_value := start_value;
    END IF;

    IF aSequenceNo < current_value THEN
        PRINTLN( 'Wanted next shard id( ' || aSequenceNo || ' ) is less than current next shard id( ' ||
                 current_value || ' )' );
        RETURN;
    END IF;

    IF aSequenceNo > max_value THEN
        PRINTLN( 'Wanted next shard id( ' || aSequenceNo || ' ) is greater than max next shard id( ' ||
                 max_value || ' )' );
        RETURN;
    END IF;

    IF aSequenceNo = current_value THEN
        PRINTLN( 'Next shard id is already ' || aSequenceNo );
        RETURN;
    END IF;

    increase_value := aSequenceNo - current_value;

    sql_string := 'ALTER SEQUENCE SYS_SHARD.NEXT_SHARD_ID INCREMENT BY ' || increase_value;
    EXECUTE IMMEDIATE sql_string;

    WHILE current_value != aSequenceNo LOOP
        SELECT SYS_SHARD.NEXT_SHARD_ID.NEXTVAL INTO current_value
        FROM DUAL;
    END LOOP;

    sql_string := 'ALTER SEQUENCE SYS_SHARD.NEXT_SHARD_ID INCREMENT BY 1';
    EXECUTE IMMEDIATE sql_string;

    PRINTLN( 'Set next shard id : ' || current_value );

EXCEPTION WHEN OTHERS THEN
    code := SQLCODE;
    errm := SUBSTRING( SQLERRM, 1, 1024 );
    PRINTLN( 'SQLCODE : ' || code );
    PRINTLN( 'SQLERRM : ' || errm );
END SET_NEXT_SHARD_ID;

PROCEDURE SET_NEXT_NODE_ID( aSequenceNo    IN BIGINT )
AS
start_value     BIGINT;
current_value   BIGINT;
increase_value  BIGINT;
max_value       BIGINT;
sql_string      VARCHAR(1024);
code            INTEGER;
errm            VARCHAR(1024);
BEGIN
    SELECT S.START_SEQ, S.CURRENT_SEQ, S.MAX_SEQ
    INTO start_value, current_value, max_value
    FROM SYSTEM_.SYS_TABLES_ T,
         V$SEQ               S
    WHERE T.TABLE_OID = S.SEQ_OID AND
          T.TABLE_NAME = 'NEXT_NODE_ID';

    IF current_value IS null THEN
        current_value := start_value;
    END IF;

    IF aSequenceNo < current_value THEN
        PRINTLN( 'Wanted next node id( ' || aSequenceNo || ' ) is less than current next node id( ' ||
                 current_value || ' )' );
        RETURN;
    END IF;

    IF aSequenceNo > max_value THEN
        PRINTLN( 'Wanted next node id( ' || aSequenceNo || ' ) is greater than max next node id( ' ||
                 max_value || ' )' );
        RETURN;
    END IF;

    IF aSequenceNo = current_value THEN
        PRINTLN( 'Next node id is already ' || aSequenceNo );
        RETURN;
    END IF;

    increase_value := aSequenceNo - current_value;

    sql_string := 'ALTER SEQUENCE SYS_SHARD.NEXT_NODE_ID INCREMENT BY ' || increase_value;
    EXECUTE IMMEDIATE sql_string;

    WHILE current_value != aSequenceNo LOOP
        SELECT SYS_SHARD.NEXT_NODE_ID.NEXTVAL INTO current_value
        FROM DUAL;
    END LOOP;

    sql_string := 'ALTER SEQUENCE SYS_SHARD.NEXT_NODE_ID INCREMENT BY 1';
    EXECUTE IMMEDIATE sql_string;

    PRINTLN( 'Set next node id : ' || current_value );

EXCEPTION WHEN OTHERS THEN
    code := SQLCODE;
    errm := SUBSTRING( SQLERRM, 1, 1024 );
    PRINTLN( 'SQLCODE : ' || code );
    PRINTLN( 'SQLERRM : ' || errm );

END SET_NEXT_NODE_ID;


END utl_shard_online_rebuild;
/
