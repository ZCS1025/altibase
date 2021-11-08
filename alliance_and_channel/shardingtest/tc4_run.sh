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


case $1 in
    "h" )
        is -port $PORT -f tc4_conf_h.sql
        ;;
    "r" )
	is -port $PORT -f tc4_conf_r.sql
        ;;
    "l" )
        is -port $PORT -f tc4_conf_l.sql
        ;;
    "c" )
        is -port $PORT -f tc4_conf_c.sql
        ;;
      * )
	is -port $PORT -f tc4_conf_h.sql
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
