ALTER SESSION SET dblink_global_transaction_level = 2;
ALTER SESSION SET autocommit = false;


EXEC dbms_shard.rebuild_data('sys','test',100,'dn1,dn2,dn3,dn4,dn5,dn6,dn7,dn8,dn9,dn10');
exec dbms_shard.check_data('sys','test');


commit;
