#!/bin/sh

HOST="localhost"
if [ "$ALTIBASE_PORT_NO" ];
then
    PORT=$ALTIBASE_PORT_NO
else
    PORT="20300"
fi
CONNTYPE="2"

THREADS="2"
RECORDS="1000000"
UNIT="1000"
THRESHOLD="1000000"
CAP="0"
USER="SYS"
PASSWORD="MANAGER"
OPTION=":$PORT/mydb"

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
isql -s $HOST -u $USER -p $PASSWORD -port $PORT -f checkpoint.sql
sleep 1
java bmt_insert $THREAD $START $RECORDS $UNIT $THRESHOLD $CAP $HOST $USER $PASSWORD $OPTION
START=`expr $START + $RECORDS`
done

START="0"
for THREAD in $THREADS
do
isql -s $HOST -u $USER -p $PASSWORD -port $PORT -f checkpoint.sql
sleep 1
java bmt_select $THREAD $START $RECORDS $UNIT $THRESHOLD $CAP $HOST $USER $PASSWORD $OPTION
START=`expr $START + $RECORDS`
done

isql -s $HOST -u $USER -p $PASSWORD -port $PORT -f drop.sql
