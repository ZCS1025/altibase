#!/usr/bin/env bash

# 아래의 상황을 가정한다.
#     - All Active(Standby) Node : Node1(NodeA), Node2(NodeB), Node3(NodeC)
#     - Hash, Range, List 샤드 객체 : T1 (Partition : P1, P2, P3)
#     - Clone 샤드 객체 : T2
#     - Solo 샤드 객체 : T3
#     - Data Rebuild를 수행하여 분산 정보에서 Node3(NodeC)의 샤드 객체를 제거한 상태이다.
#
# Node3(NodeC)를 제거한다.

APPLICATION=$0

if [ $# -ne 4 ]
then
    echo "Usage : ${APPLICATION} shard_meta_server shard_meta_port sys_password old_node_name"
    exit 1
fi

SHARD_META_SERVER=$1
SHARD_META_PORT=$2
PASSWORD=$3
OLD_NODE_NAME=$4

IFS_BACKUP=${IFS}

ISQL="${ALTIBASE_HOME}/bin/isql -u SYS -p ${PASSWORD} -silent"

# 모든 Node의 접속 정보를 얻는다. (추가 Node 제외)
META_SQL_RESULT=$(
${ISQL} -s ${SHARD_META_SERVER} -port ${SHARD_META_PORT} << EOF
    SET HEADING OFF;
    EXEC utl_shard_online_rebuild.get_connection_info();
    QUIT
EOF
)

DIR_PATH=$(cd `dirname $0` && pwd)
/usr/bin/env bash ${DIR_PATH}/node_remove_check_prepare.sh ${SHARD_META_SERVER} ${SHARD_META_PORT} ${PASSWORD} ${OLD_NODE_NAME}
if [ $? = 1 ]; then
    echo "${APPLICATION} can not be executed"
    exit 1
fi

# 모든 Node의 Shard Meta에서 Node3(NodeC)를 제거한다. (DBMS_SHARD.UNSET_NODE())
echo "[Step-1. (All nodes) Unset Node]"

IFS=$'\n'
RESULT_ARRAY=$( echo "${META_SQL_RESULT}" | grep -v "^\[NODE_NAME\]" | grep -v "^Execute success." )
for VALUE in ${RESULT_ARRAY[@]}
do
    NODE_NAME=$( echo ${VALUE} | gawk '{print $1}' )
    NODE_IP=$( echo ${VALUE} | gawk '{print $2}' )
    NODE_PORT=$( echo ${VALUE} | gawk '{print $3}' )
    NODE_ALTERNATE_IP=$( echo ${VALUE} | gawk '{print $4}' )
    NODE_ALTERNATE_PORT=$( echo ${VALUE} | gawk '{print $5}' )

    IFS=$' '

    echo "Node Info. : ${NODE_NAME}, ${NODE_IP}, ${NODE_PORT}"

    ${ISQL} -s ${NODE_IP} -port ${NODE_PORT} << EOF
        EXEC DBMS_SHARD.UNSET_NODE( '${OLD_NODE_NAME}' );
        QUIT
EOF

    if [ ${NODE_ALTERNATE_IP} != "N/A" ]
    then
        echo "Alternate Node Info. : ${NODE_NAME}, ${NODE_ALTERNATE_IP}, ${NODE_ALTERNATE_PORT}"

        ${ISQL} -s ${NODE_ALTERNATE_IP} -port ${NODE_ALTERNATE_PORT} << EOF
            EXEC DBMS_SHARD.UNSET_NODE( '${OLD_NODE_NAME}' );
            QUIT
EOF
    fi
done

IFS=${IFS_BACKUP}

echo "Done."
