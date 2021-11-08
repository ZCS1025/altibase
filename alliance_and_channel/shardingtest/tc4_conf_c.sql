delete from test;
exec dbms_shard.unset_shard_table ('sys','TEST');

exec dbms_shard.unset_node('dn1');
exec dbms_shard.unset_node('dn3');
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


exec dbms_shard.set_node('dn1','dn1',20301);
exec dbms_shard.set_node('dn3','dn1',20302);
exec dbms_shard.set_node('dn3','dn1',20303);
exec dbms_shard.set_node('dn4','dn1',20301);
exec dbms_shard.set_node('dn5','dn2',20302);
exec dbms_shard.set_node('dn6','dn2',20303);
exec dbms_shard.set_node('dn7','dn3',20301);
exec dbms_shard.set_node('dn8','dn3',20302);
exec dbms_shard.set_node('dn9','dn3',20303);
exec dbms_shard.set_node('dn10','dn4',20301);
exec dbms_shard.set_node('dn11','dn4',20302);
exec dbms_shard.set_node('dn12','dn4',20303);
exec dbms_shard.set_node('dn13','dn5',20301);
exec dbms_shard.set_node('dn14','dn5',20302);
exec dbms_shard.set_node('dn15','dn5',20303);

exec dbms_shard.set_shard_table ('sys','TEST','c');

exec dbms_shard.set_shard_clone ('sys','TEST', 'dn1');
exec dbms_shard.set_shard_clone ('sys','TEST', 'dn3');
exec dbms_shard.set_shard_clone ('sys','TEST', 'dn3');
exec dbms_shard.set_shard_clone ('sys','TEST', 'dn4');
exec dbms_shard.set_shard_clone ('sys','TEST', 'dn5');
exec dbms_shard.set_shard_clone ('sys','TEST', 'dn6');
exec dbms_shard.set_shard_clone ('sys','TEST', 'dn7');
exec dbms_shard.set_shard_clone ('sys','TEST', 'dn8');
exec dbms_shard.set_shard_clone ('sys','TEST', 'dn9');
exec dbms_shard.set_shard_clone ('sys','TEST', 'dn10');
exec dbms_shard.set_shard_clone ('sys','TEST', 'dn11');
exec dbms_shard.set_shard_clone ('sys','TEST', 'dn12');
exec dbms_shard.set_shard_clone ('sys','TEST', 'dn13');
exec dbms_shard.set_shard_clone ('sys','TEST', 'dn14');
exec dbms_shard.set_shard_clone ('sys','TEST', 'dn15');



