#!/usr/bin/env bash

# 아래의 상황을 가정한다.
#     - Target Node : Node3
#     - All Active(Standby) Node : Node1(NodeA), Node2(NodeB), Node3(NodeC)
#     - Clone 샤드 객체 : T2(Node1, NodeA, Node2, NodeB, Node3, NodeC)
#
# T2를 Node3(NodeC)에서 제거한다.
#     T2(Node1, NodeA, Node2, NodeB, Node3, NodeC)
#     ->
#     T2(Node1, NodeA, Node2, NodeB)

DIR_PATH=$( cd `dirname $0` && pwd )
source ${DIR_PATH}/configure.sh

IFS_BACKUP=${IFS}

ISQL="${ALTIBASE_HOME}/bin/isql -u SYS -p ${PASSWORD} -silent"

# FROM_NODE_NAME을 지정했는지 검사한다.
if [ ! ${FROM_NODE_NAME} ]
then
    echo "FROM_NODE_NAME is needed."
    exit 1
fi

DIR_PATH=$(cd `dirname $0` && pwd)
/usr/bin/env bash ${DIR_PATH}/clone_remove_check_prepare.sh ${1}
if [ $? = 1 ]; then
    echo "${APPLICATION} can not be executed"
    exit 1
fi

# Reset Shard Resident Node SQL을 구성한다.
SHARD_META_SQL=""

for (( i = 0 ; i < $SHARD_OBJECTS_COUNT ; i++ ))
do
    SHARD_META_SQL="${SHARD_META_SQL}
        EXEC DBMS_SHARD.RESET_SHARD_RESIDENT_NODE( '${USER_NAME_ARRAY[$i]}',
                                                   '${TABLE_NAME_ARRAY[$i]}',
                                                   '${FROM_NODE_NAME}',
                                                   NULL );"
done

echo ""
echo "Shard Meta SQL : ${SHARD_META_SQL}"

# 모든 Node의 접속 정보를 얻는다.
META_SQL_RESULT=$(
${ISQL} -s ${SHARD_META_SERVER} -port ${SHARD_META_PORT} << EOF
    SET HEADING OFF;
    EXEC utl_shard_online_rebuild.get_connection_info();
    QUIT
EOF
)

# 모든 Node의 Shard Meta에서 Clone 샤드 객체의 Node를 제거한다.
echo ""
echo "[Step-1. (All nodes) Unset Shard Clone]"

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

    # T2의 Node에서 Node3(NodeC)를 제거한다.
    echo "Node Info. : ${NODE_NAME}, ${NODE_IP}, ${NODE_PORT}"

    ${ISQL} -s ${NODE_IP} -port ${NODE_PORT} << EOF
        ALTER SESSION SET AUTOCOMMIT = FALSE;
        ${SHARD_META_SQL}
        COMMIT;
        QUIT
EOF

    if [ ${NODE_ALTERNATE_IP} != "N/A" ]
    then
        echo "Alternate Node Info. : ${NODE_NAME}, ${NODE_ALTERNATE_IP}, ${NODE_ALTERNATE_PORT}"

        ${ISQL} -s ${NODE_ALTERNATE_IP} -port ${NODE_ALTERNATE_PORT} << EOF
            ALTER SESSION SET AUTOCOMMIT = FALSE;
            ${SHARD_META_SQL}
            COMMIT;
            QUIT
EOF
    fi
done

IFS=${IFS_BACKUP}

echo "Done."
