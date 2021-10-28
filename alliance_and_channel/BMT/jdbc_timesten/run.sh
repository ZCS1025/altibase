#!/bin/sh

HOST="localhost"
THREADS="2"
RECORDS="1000000"
UNIT="1000"
THRESHOLD="1000000"
CAP="0"
USER="jhpark"
PASSWORD="jhpark"
OPTION="tt_1122"
TNS=""

ttisql -f crtuser.sql    "dsn=TT_1122"
ttisql -f schema.sql     "dsn=$OPTION;uid=$USER;pwd=$PASSWORD"
ttisql -f checkpoint.sql "dsn=$OPTION;uid=$USER;pwd=$PASSWORD"

START="0"
for THREAD in $THREADS
do
java bmt_insert $THREAD $START $RECORDS $UNIT $THRESHOLD $CAP $HOST $USER $PASSWORD $OPTION
START=`expr $START + $RECORDS`
done

ttisql -f checkpoint.sql "dsn=$OPTION;uid=$USER;pwd=$PASSWORD"

START="0"
for THREAD in $THREADS
do
java bmt_select $THREAD $START $RECORDS $UNIT $THRESHOLD $CAP $HOST $USER $PASSWORD $OPTION
START=`expr $START + $RECORDS`
done

ttisql -f trunc.sql "dsn=$OPTION;uid=$USER;pwd=$PASSWORD"
