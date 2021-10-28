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
JAVA_CP="${HOME}/solidDB/jdbc/SolidDriver2.0.jar:."

solsql -f schema.sql     $USER $PASSWORD

START="0"
for THREAD in $THREADS
do
java -cp $JAVA_CP bmt_insert $THREAD $START $RECORDS $UNIT $THRESHOLD $CAP $HOST $USER $PASSWORD $OPTION
START=`expr $START + $RECORDS`
done


START="0"
for THREAD in $THREADS
do
java -cp $JAVA_CP bmt_select $THREAD $START $RECORDS $UNIT $THRESHOLD $CAP $HOST $USER $PASSWORD $OPTION
START=`expr $START + $RECORDS`
done

solsql -f trunc.sql $USER $PASSWORD
