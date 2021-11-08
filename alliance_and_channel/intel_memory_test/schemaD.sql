DROP TABLESPACE intel_data
INCLUDING CONTENTS AND DATAFILES
CASCADE CONSTRAINTS;

CREATE TABLESPACE intel_data
DATAFILE '/data/tpch/db/dbs/intel_data.1' size 10G,
'/data/tpch/db/dbs/intel_data.2' size 10G,
'/data/tpch/db/dbs/intel_data.3' size 10G,
'/data/tpch/db/dbs/intel_data.4' size 10G,
'/data/tpch/db/dbs/intel_data.5' size 10G,
'/data/tpch/db/dbs/intel_data.6' size 10G,
'/data/tpch/db/dbs/intel_data.7' size 10G,
'/data/tpch/db/dbs/intel_data.8' size 10G,
'/data/tpch/db/dbs/intel_data.9' size 10G,
'/data/tpch/db/dbs/intel_data.10' size 10G,
'/data/tpch/db/dbs/intel_data.11' size 10G,
'/data/tpch/db/dbs/intel_data.12' size 10G,
'/data/tpch/db/dbs/intel_data.13' size 10G,
'/data/tpch/db/dbs/intel_data.14' size 10G,
'/data/tpch/db/dbs/intel_data.15' size 10G,
'/data/tpch/db/dbs/intel_data.16' size 10G,
'/data/tpch/db/dbs/intel_data.17' size 10G,
'/data/tpch/db/dbs/intel_data.18' size 10G,
'/data/tpch/db/dbs/intel_data.19' size 10G,
'/data/tpch/db/dbs/intel_data.20' size 10G
AUTOEXTEND ON NEXT 102400K MAXSIZE 33554424K;


ALTER SYSTEM CHECKPOINT;
ALTER SYSTEM CHECKPOINT;
ALTER SYSTEM CHECKPOINT;

DROP TABLE KOS_TEST;

CREATE TABLE KOS_TEST (
    K01  INTEGER PRIMARY KEY,
    K02  INTEGER,
    K03  INTEGER,
    K04  INTEGER,
    K05  INTEGER,
    K06  INTEGER,
    K07  INTEGER,
    K08  INTEGER,
    K09  INTEGER,
    K10  INTEGER,
    K11  INTEGER,
    K12  INTEGER,
    K13  INTEGER,
    K14  INTEGER,
    K15  INTEGER,
    K16  INTEGER,
    K17  INTEGER,
    K18  INTEGER,
    K19  INTEGER,
    K20  INTEGER,
    K21  INTEGER,
    K22  INTEGER,
    K23  INTEGER,
    K24  INTEGER,
    K25  INTEGER,
    K26  INTEGER,
    K27  INTEGER,
    K28  INTEGER,
    K29  INTEGER,
    K30  INTEGER,
    K31  INTEGER,
    K32  INTEGER,
    K33  INTEGER,
    K34  INTEGER,
    K35  INTEGER,
    K36  INTEGER,
    K37  INTEGER,
    K38  INTEGER,
    K39  INTEGER,
    K40  INTEGER,
    K41  INTEGER,
    K42  INTEGER,
    K43  INTEGER,
    K44  INTEGER,
    K45  INTEGER,
    K46  VARCHAR(30),
    K47  VARCHAR(30),
    K48  VARCHAR(30),
    K49  VARCHAR(30),
    K50  VARCHAR(30)
) tablespace intel_data;

ALTER SYSTEM CHECKPOINT;
ALTER SYSTEM CHECKPOINT;
ALTER SYSTEM CHECKPOINT;
