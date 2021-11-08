EXEC DBMS_SHARD.CREATE_META();

exec dbms_shard.set_node('dn1','dn1',20301);
exec dbms_shard.set_node('dn2','dn2',20301);
exec dbms_shard.set_node('dn3','dn3',20301);
exec dbms_shard.set_node('dn4','dn4',20301);
exec dbms_shard.set_node('dn5','dn5',20301);


alter session set shard linker=on;

exec dbms_shard.set_shard_table ('sys','TEST','h','K01');

exec dbms_shard.set_shard_hash ('sys','TEST', 200, 'dn1');
exec dbms_shard.set_shard_hash ('sys','TEST', 400, 'dn2');
exec dbms_shard.set_shard_hash ('sys','TEST', 600, 'dn3');
exec dbms_shard.set_shard_hash ('sys','TEST', 800, 'dn4');
exec dbms_shard.set_shard_hash ('sys','TEST', 1000, 'dn5');

