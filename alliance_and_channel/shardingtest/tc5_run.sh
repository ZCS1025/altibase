HOST="localhost"
PORT="20301"
CONNTYPE="1"

THREADS="32"
RECORDS="1000000"
UNIT="1000"
THRESHOLD="100000"
CAP="0"
USER="SYS"
PASSWORD="MANAGER"
OPTION="DSN=$HOST;PORT_NO=$PORT;CONNTYPE=$CONNTYPE"

./configure.sh
is -f tc5_conf.sql

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
echo

START="0"
for THREAD in $THREADS
do
./bmt_insert $THREAD $START $RECORDS $UNIT $THRESHOLD $CAP $USER $PASSWORD $OPTION
START=`expr $START + $RECORDS`
done


echo '--------------------------------------------------------------------------------'

is -f tc5_set.sql



echo '--------------------------------------------------------------------------------'

is -f tc5_rebuild.sql


echo '--------------------------------------------------------------------------------'

is -f tc5_set_2.sql

is -f tc5_rebuild.sql
