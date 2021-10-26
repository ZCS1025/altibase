#!/bin/bash

ALTIBASE_HOME=/home/niceguy/AB71/DB0;export ALTIBASE_HOME
PATH=${ALTIBASE_HOME}/bin:${PATH};export PATH
LD_LIBRARY_PATH=${ALTIBASE_HOME}/lib:${LD_LIBRARY_PATH};export LD_LIBRARY_PATH
CLASSPATH=${ALTIBASE_HOME}/lib/Altibase.jar:${CLASSPATH};export CLASSPATH

for PORT in "20300" "20301" "20302" "20303" "20304"
do
	is -s localhost -u smssuser -p smssuser -port $PORT -f schema.sql
done



