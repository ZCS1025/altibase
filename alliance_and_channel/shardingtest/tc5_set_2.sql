exec dbms_shard.set_node('dn6','192.168.1.106',20302);
exec dbms_shard.set_node('dn7','192.168.1.107',20302);
exec dbms_shard.set_node('dn8','192.168.1.108',20302);
exec dbms_shard.set_node('dn9','192.168.1.112',20302);
exec dbms_shard.set_node('dn10','192.168.1.113',20302);



exec dbms_shard.unset_shard_table ('sys','TEST');

exec dbms_shard.set_shard_table ('sys','TEST','h','K01');

exec dbms_shard.set_shard_hash ('sys','TEST', 100, 'dn1');
exec dbms_shard.set_shard_hash ('sys','TEST', 200, 'dn2');
exec dbms_shard.set_shard_hash ('sys','TEST', 300, 'dn3');
exec dbms_shard.set_shard_hash ('sys','TEST', 400, 'dn4');
exec dbms_shard.set_shard_hash ('sys','TEST', 500, 'dn5');
exec dbms_shard.set_shard_hash ('sys','TEST', 600, 'dn6');
exec dbms_shard.set_shard_hash ('sys','TEST', 700, 'dn7');
exec dbms_shard.set_shard_hash ('sys','TEST', 800, 'dn8');
exec dbms_shard.set_shard_hash ('sys','TEST', 900, 'dn9');
exec dbms_shard.set_shard_hash ('sys','TEST', 1000, 'dn10');


exec dbms_shard.check_data('sys','test');

