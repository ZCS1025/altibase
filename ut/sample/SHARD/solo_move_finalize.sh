#!/usr/bin/env bash

# 아래의 상황을 가정한다.
#     - Target Node : Node2, Node3
#     - All Active(Standby) Node : Node1(NodeA), Node2(NodeB), Node3(NodeC)
#     - Solo 샤드 객체 : T3(Node2, NodeB)
#
# T3를 Node3(NodeC)로 이동한다.
#     T3(Node2, NodeB)
#     ->
#     T3(Node3, NodeC)

DIR_PATH=$( cd `dirname $0` && pwd )
source ${DIR_PATH}/configure.sh

IFS_BACKUP=${IFS}

ISQL="${ALTIBASE_HOME}/bin/isql -u SYS -p ${PASSWORD} -silent"
REP_NAME_REBUILD="REP_REBUILD"
REP_NAME_ALTERNATE="REP_ALTERNATE"

DIR_PATH=$(cd `dirname $0` && pwd)
/usr/bin/env bash ${DIR_PATH}/solo_move_check_finalize.sh ${1}
if [ $? = 1 ]; then
    echo "${APPLICATION} can not be executed"
    exit 1
fi

# 모든 Node의 접속 정보를 얻는다.
META_SQL_RESULT=$(
${ISQL} -s ${SHARD_META_SERVER} -port ${SHARD_META_PORT} << EOF
    SET HEADING OFF;
    EXEC utl_shard_online_rebuild.get_connection_info();
    QUIT
EOF
)

IFS=$'\n'
RESULT_ARRAY=$( echo "${META_SQL_RESULT}" | grep -v "^\[NODE_NAME\]" | grep -v "^Execute success." )
for VALUE in ${RESULT_ARRAY[@]}
do
    NODE_NAME=$( echo ${VALUE} | gawk '{print $1}' )
    NODE_IP=$( echo ${VALUE} | gawk '{print $2}' )
    NODE_PORT=$( echo ${VALUE} | gawk '{print $3}' )
    NODE_ALTERNATE_IP=$( echo ${VALUE} | gawk '{print $4}' )
    NODE_ALTERNATE_PORT=$( echo ${VALUE} | gawk '{print $5}' )

    if [ ${NODE_NAME} = ${FROM_NODE_NAME} ]
    then
        FROM_NODE_IP=${NODE_IP}
        FROM_NODE_PORT=${NODE_PORT}
    fi

    if [ ${NODE_NAME} = ${TO_NODE_NAME} ]
    then
        TO_NODE_IP=${NODE_IP}
        TO_NODE_PORT=${NODE_PORT}
        if [ ${NODE_ALTERNATE_IP} != "N/A" ]
        then
            TO_NODE_ALTERNATE_IP=${NODE_ALTERNATE_IP}
            TO_NODE_ALTERNATE_PORT=${NODE_ALTERNATE_PORT}
        fi
    fi
done

IFS=${IFS_BACKUP}

ISQL_FROM_NODE="${ISQL} -s ${FROM_NODE_IP} -port ${FROM_NODE_PORT}"
ISQL_TO_NODE="${ISQL} -s ${TO_NODE_IP} -port ${TO_NODE_PORT}"
if [ ${TO_NODE_ALTERNATE_IP} ]
then
    ISQL_TO_NODE_ALTERNATE="${ISQL} -s ${TO_NODE_ALTERNATE_IP} -port ${TO_NODE_ALTERNATE_PORT}"
fi

# Node2에서 수행한 작업을 정리한다.
#   (Node2 -> Node3) Shard Data Replication을 정지하고 제거한다. (T3 -> T3_REBUILD)
echo ""
echo "[Step-1. (Target nodes) stop/drop replication for rebuild]"
echo "Node Info. : ${FROM_NODE_NAME}, ${FROM_NODE_IP}, ${FROM_NODE_PORT}"

${ISQL_FROM_NODE} << EOF
    EXEC utl_shard_online_rebuild.stop_replication( '${REP_NAME_REBUILD}' );
    EXEC utl_shard_online_rebuild.drop_replication( '${REP_NAME_REBUILD}' );
    QUIT
EOF

echo "Node Info. : ${TO_NODE_NAME}, ${TO_NODE_IP}, ${TO_NODE_PORT}"

${ISQL_TO_NODE} << EOF
    EXEC utl_shard_online_rebuild.drop_replication( '${REP_NAME_REBUILD}' );
    QUIT
EOF

# Node3(NodeC)에서 T3_REBUILD를 제거한다.
#   (Node3 -> NodeC) Shard Data Replication을 정지하고 제거한다. (T3_REBUILD -> T3_REBUILD)
#   Node3(NodeC)에서 T3_REBUILD를 제거한다.
echo ""
echo "[Step-2. (Target nodes) Stop/drop replication for alternate and drop rebuild table]"

if [ ${TO_NODE_ALTERNATE_IP} ]
then
    echo "Node Info. : ${TO_NODE_NAME}, ${TO_NODE_IP}, ${TO_NODE_PORT}"

    ${ISQL_TO_NODE} << EOF
        EXEC utl_shard_online_rebuild.stop_replication( '${REP_NAME_ALTERNATE}' );
        EXEC utl_shard_online_rebuild.drop_replication( '${REP_NAME_ALTERNATE}' );
        QUIT
EOF

    echo "Alternate Node Info. : ${TO_NODE_NAME}, ${TO_NODE_ALTERNATE_IP}, ${TO_NODE_ALTERNATE_PORT}"

    ${ISQL_TO_NODE_ALTERNATE} << EOF
        EXEC utl_shard_online_rebuild.drop_replication( '${REP_NAME_ALTERNATE}' );
        QUIT
EOF
fi

echo "Node Info. : ${TO_NODE_NAME}, ${TO_NODE_IP}, ${TO_NODE_PORT}"

for (( i = 0 ; i < $SHARD_OBJECTS_COUNT ; i++ ))
do
    DROP_TABLE_SQL="DROP TABLE \"${USER_NAME_ARRAY[$i]}\".\"${TABLE_NAME_ARRAY[$i]}_REBUILD\""
    echo "DDL : ${DROP_TABLE_SQL}"

    ${ISQL_TO_NODE} << EOF
        ${DROP_TABLE_SQL};
        QUIT
EOF
done

if [ ${TO_NODE_ALTERNATE_IP} ]
then
    echo "Alternate Node Info. : ${TO_NODE_NAME}, ${TO_NODE_ALTERNATE_IP}, ${TO_NODE_ALTERNATE_PORT}"

    for (( i = 0 ; i < $SHARD_OBJECTS_COUNT ; i++ ))
    do
        DROP_TABLE_SQL="DROP TABLE \"${USER_NAME_ARRAY[$i]}\".\"${TABLE_NAME_ARRAY[$i]}_REBUILD\""
        echo "DDL : ${DROP_TABLE_SQL}"

        ${ISQL_TO_NODE_ALTERNATE} << EOF
            ${DROP_TABLE_SQL};
            QUIT
EOF
    done
fi

echo "Done."
