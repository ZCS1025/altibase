drop replication   rep1;
drop replication   rep2;
drop replication   rep3;
drop replication   rep4;

create replication rep1
with '10.1.10.4', 20401
from smssuser.tbl_sm_data to smssuser.tbl_sm_data;

create replication rep2
with '10.1.10.4', 20402
from smssuser.tbl_sm_data to smssuser.tbl_sm_data;

create replication rep3
with '10.1.10.4', 20403
from smssuser.tbl_sm_data to smssuser.tbl_sm_data;

create replication rep4
with '10.1.10.4', 20404
from smssuser.tbl_sm_data to smssuser.tbl_sm_data;
