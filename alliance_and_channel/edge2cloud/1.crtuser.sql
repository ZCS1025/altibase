alter replication rep1 stop;
alter replication rep2 stop;
alter replication rep3 stop;
alter replication rep4 stop;
alter replication rep5 stop;
alter replication rep6 stop;
alter replication rep7 stop;
alter replication rep8 stop;
alter replication rep9 stop;
alter replication rep10 stop;

drop replication rep1;
drop replication rep2;
drop replication rep3;
drop replication rep4;
drop replication rep5;
drop replication rep6;
drop replication rep7;
drop replication rep8;
drop replication rep9;
drop replication rep10;

drop user SMSSUSER cascade;
create user SMSSUSER identified by SMSSUSER;
grant all privileges to SMSSUSER; 
