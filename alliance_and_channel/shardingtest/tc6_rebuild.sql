ALTER SESSION SET dblink_global_transaction_level = 2;
ALTER SESSION SET autocommit = false;


EXEC dbms_shard.rebuild_data('sys','test');
exec dbms_shard.check_data('sys','test');


commit;

