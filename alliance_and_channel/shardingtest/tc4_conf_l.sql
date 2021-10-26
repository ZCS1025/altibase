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
exec dbms_shard.set_node('dn2','192.168.1.106',20302);
exec dbms_shard.set_node('dn3','192.168.1.106',20303);
exec dbms_shard.set_node('dn4','192.168.1.107',20301);
exec dbms_shard.set_node('dn5','192.168.1.107',20302);
exec dbms_shard.set_node('dn6','192.168.1.107',20303);
exec dbms_shard.set_node('dn7','192.168.1.108',20301);
exec dbms_shard.set_node('dn8','192.168.1.108',20302);
exec dbms_shard.set_node('dn9','192.168.1.108',20303);
exec dbms_shard.set_node('dn10','192.168.1.112',20301);
exec dbms_shard.set_node('dn11','192.168.1.112',20302);
exec dbms_shard.set_node('dn12','192.168.1.112',20303);
exec dbms_shard.set_node('dn13','192.168.1.113',20301);
exec dbms_shard.set_node('dn14','192.168.1.113',20302);
exec dbms_shard.set_node('dn15','192.168.1.113',20303);

alter session set shard linker=on;

exec dbms_shard.set_shard_table ('sys','TEST','l','K02','dn1');

exec dbms_shard.set_shard_list ('sys','TEST',1, 'dn1');
exec dbms_shard.set_shard_list ('sys','TEST',2, 'dn2');
exec dbms_shard.set_shard_list ('sys','TEST',3, 'dn3');
exec dbms_shard.set_shard_list ('sys','TEST',4, 'dn4');
exec dbms_shard.set_shard_list ('sys','TEST',5, 'dn5');
exec dbms_shard.set_shard_list ('sys','TEST',6, 'dn6');
exec dbms_shard.set_shard_list ('sys','TEST',7, 'dn7');
exec dbms_shard.set_shard_list ('sys','TEST',8, 'dn8');
exec dbms_shard.set_shard_list ('sys','TEST',9, 'dn9');
exec dbms_shard.set_shard_list ('sys','TEST',10, 'dn10');
exec dbms_shard.set_shard_list ('sys','TEST',11, 'dn11');
exec dbms_shard.set_shard_list ('sys','TEST',12, 'dn12');
exec dbms_shard.set_shard_list ('sys','TEST',13, 'dn13');
exec dbms_shard.set_shard_list ('sys','TEST',14, 'dn14');
exec dbms_shard.set_shard_list ('sys','TEST',15, 'dn15');



