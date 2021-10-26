drop replication   rep3;
create replication rep3
with '10.1.10.4', 20500
from smssuser.tbl_sm_data to smssuser.tbl_sm_data;
