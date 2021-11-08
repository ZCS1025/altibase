#!/bin/sh

HOST="localhost"
PORT="20370"
CONNTYPE="2"

THREADS="256"
RECORDS="1000000000"
UNIT="1000"
THRESHOLD="1000000"
CAP="0"
USER="SYS"
PASSWORD="MANAGER"
OPTION="DSN=$HOST;PORT_NO=$PORT;CONNTYPE=$CONNTYPE"

./configure.sh
make clean
make
echo

case $CONNTYPE in
    "1" )
        export ISQL_CONNECTION="TCP"
        ;;
    "2" )
        export ISQL_CONNECTION="UNIX"
        ;;
    "3" )
        export ISQL_CONNECTION="IPC"
        ;;
esac
isql -s $HOST -u $USER -p $PASSWORD -port $PORT -f schema.sql
echo

START="0"
for THREAD in $THREADS
do
./bmt_insert $THREAD $START $RECORDS $UNIT $THRESHOLD $CAP $USER $PASSWORD $OPTION
START=`expr $START + $RECORDS`
done

START="0"
for THREAD in $THREADS
do
./bmt_select $THREAD $START $RECORDS $UNIT $THRESHOLD $CAP $USER $PASSWORD $OPTION
START=`expr $START + $RECORDS`
done
