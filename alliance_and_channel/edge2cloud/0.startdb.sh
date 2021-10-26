#!/bin/bash

for DB in "DB1" "DB2" "DB3" "DB4" "DB0"
do
	ALTIBASE_HOME=/home/niceguy/AB71/$DB;export ALTIBASE_HOME
	echo $ALTIBASE_HOME
	PATH=${ALTIBASE_HOME}/bin:${PATH};export PATH
	LD_LIBRARY_PATH=${ALTIBASE_HOME}/lib:${LD_LIBRARY_PATH};export LD_LIBRARY_PATH
	CLASSPATH=${ALTIBASE_HOME}/lib/Altibase.jar:${CLASSPATH};export CLASSPATH
	server restart &
done



