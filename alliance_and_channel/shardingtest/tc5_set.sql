exec dbms_shard.unset_shard_table ('sys','TEST');

exec dbms_shard.set_shard_table ('sys','TEST','h','K01');

exec dbms_shard.set_shard_hash ('sys','TEST', 200, 'dn1');
exec dbms_shard.set_shard_hash ('sys','TEST', 400, 'dn2');
exec dbms_shard.set_shard_hash ('sys','TEST', 600, 'dn3');
exec dbms_shard.set_shard_hash ('sys','TEST', 800, 'dn4');
exec dbms_shard.set_shard_hash ('sys','TEST', 1000, 'dn5');

exec dbms_shard.check_data('sys','test');

