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

case $1 in
    "1" )
        is -f tc2_conf_1.sql
	THREADS="3"
        ;;
    "2" )
        is -f tc2_conf_2.sql
	THREADS="7"
        ;;
    "4" )
        is -f tc2_conf_4.sql
	THREADS="11"
        ;;
    "8" )
        is -f tc2_conf_8.sql
	THREADS="20"
        ;;
    "15" )
        is -f tc2_conf_15.sql
	THREADS="32"
        ;;

    * )
        is -f tc2_conf_15.sql
        ;;

esac



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


is -f display.sql
