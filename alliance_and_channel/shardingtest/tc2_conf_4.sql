delete from test;

exec dbms_shard.unset_shard_table ('sys','TEST');

exec dbms_shard.unset_node('dn1');
exec dbms_shard.unset_node('dn2');
exec dbms_shard.unset_node('dn3');
exec dbms_shard.unset_node('dn4');
exec dbms_shard.unset_node('dn5');
exec dbms_shard.unset_node('dn6');
exec dbms_shard.unset_node('dn7');
exec dbms_shard.unset_node('dn8');
exec dbms_shard.unset_node('dn9');
exec dbms_shard.unset_node('dn10');
exec dbms_shard.unset_node('dn11');
exec dbms_shard.unset_node('dn12');
exec dbms_shard.unset_node('dn13');
exec dbms_shard.unset_node('dn14');
exec dbms_shard.unset_node('dn15');


exec dbms_shard.set_node('dn1','192.168.1.106',20301);
exec dbms_shard.set_node('dn2','192.168.1.107',20301);
exec dbms_shard.set_node('dn3','192.168.1.108',20301);
exec dbms_shard.set_node('dn4','192.168.1.112',20301);

alter session set shard linker=on;

exec dbms_shard.set_shard_table ('sys','TEST','h','K01');

exec dbms_shard.set_shard_hash ('sys','TEST', 250, 'dn1');
exec dbms_shard.set_shard_hash ('sys','TEST', 500, 'dn2');
exec dbms_shard.set_shard_hash ('sys','TEST', 750, 'dn3');
exec dbms_shard.set_shard_hash ('sys','TEST', 1000, 'dn4');



