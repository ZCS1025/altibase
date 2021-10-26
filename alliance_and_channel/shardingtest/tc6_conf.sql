EXEC DBMS_SHARD.CREATE_META();

exec dbms_shard.set_node('dn1','192.168.1.106',20301);
exec dbms_shard.set_node('dn2','192.168.1.107',20301);
exec dbms_shard.set_node('dn3','192.168.1.108',20301);
exec dbms_shard.set_node('dn4','192.168.1.112',20301);
exec dbms_shard.set_node('dn5','192.168.1.113',20301);


alter session set shard linker=on;

exec dbms_shard.set_shard_table ('sys','TEST','h','K01');

exec dbms_shard.set_shard_hash ('sys','TEST', 200, 'dn1');
exec dbms_shard.set_shard_hash ('sys','TEST', 400, 'dn2');
exec dbms_shard.set_shard_hash ('sys','TEST', 600, 'dn3');
exec dbms_shard.set_shard_hash ('sys','TEST', 800, 'dn4');
exec dbms_shard.set_shard_hash ('sys','TEST', 1000, 'dn5');

