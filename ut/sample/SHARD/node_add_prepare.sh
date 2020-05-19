#!/usr/bin/env bash

# 아래의 상황을 가정한다.
#     - All Active(Standby) Node : Node1(NodeA), Node2(NodeB)
#     - Hash, Range, List 샤드 객체 : T1 (Partition : P1, P2, P3)
#     - Clone 샤드 객체 : T2
#     - Solo 샤드 객체 : T3
#
# Node3(NodeC)를 추가한다.

APPLICATION=$0

if [ $# -ne 7 -a $# -ne 11 ]
then
    echo "Usage : ${APPLICATION} shard_meta_server shard_meta_port sys_password new_node_name meta_node_id host_ip port_no [alternate_meta_node_id alternate_host_ip alternate_port_no connection_type]"
    exit 1
fi

SHARD_META_SERVER=$1
SHARD_META_PORT=$2
PASSWORD=$3
NEW_NODE_NAME=$4
META_NODE_ID=$5
NEW_NODE_IP=$6
NEW_NODE_PORT=$7
META_NODE_ALTERNATE_ID=$8
NEW_NODE_ALTERNATE_IP=$9
NEW_NODE_ALTERNATE_PORT=${10}
NEW_NODE_CONN_TYPE=${11}

IFS_BACKUP=${IFS}

ISQL="${ALTIBASE_HOME}/bin/isql -u SYS -p ${PASSWORD} -silent"
ISQL_NEW_NODE="${ISQL} -s ${NEW_NODE_IP} -port ${NEW_NODE_PORT}"
if [ ${NEW_NODE_ALTERNATE_IP} ]
then
    ISQL_NEW_NODE_ALTERNATE="${ISQL} -s ${NEW_NODE_ALTERNATE_IP} -port ${NEW_NODE_ALTERNATE_PORT}"
fi
REP_NAME_ADD_NODE="REP_ADD_NODE"
REP_NAME_ADD_NODE_ALTERNATE="REP_ADD_NODE_ALTERNATE"


DIR_PATH=$(cd `dirname $0` && pwd)
/usr/bin/env bash ${DIR_PATH}/node_add_check_prepare.sh ${SHARD_META_SERVER} ${SHARD_META_PORT} ${PASSWORD} ${NEW_NODE_NAME} ${NEW_NODE_IP} ${NEW_NODE_PORT} ${NEW_NODE_ALTERNATE_IP} ${NEW_NODE_ALTERNATE_PORT}
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

# 첫 번째 Node를 검색한다.
echo "[Step-1. Find the first node]"

IFS=$'\n'
RESULT_ARRAY=$( echo "${META_SQL_RESULT}" | grep -v "^\[NODE_NAME\]" | grep -v "^Execute success." )
for VALUE in ${RESULT_ARRAY[@]}
do
    NODE_NAME=$( echo ${VALUE} | gawk '{print $1}' )
    NODE_IP=$( echo ${VALUE} | gawk '{print $2}' )
    NODE_PORT=$( echo ${VALUE} | gawk '{print $3}' )

    if [ ! ${OLD_NODE_NAME} ]
    then
        echo "Node Info. : ${NODE_NAME}, ${NODE_IP}, ${NODE_PORT}"

        OLD_NODE_NAME=${NODE_NAME}
        OLD_NODE_IP=${NODE_IP}
        OLD_NODE_PORT=${NODE_PORT}
    fi

    if [ ${NODE_NAME} = ${NEW_NODE_NAME} ]
    then
        echo "New node already exists!"
        echo "Node Info. : ${NODE_NAME}, ${NODE_IP}, ${NODE_PORT}"
        exit 2
    fi
done

IFS=${IFS_BACKUP}

if [ ! ${OLD_NODE_NAME} ]
then
    echo "Old node does not exist!"
    exit 3
fi

ISQL_OLD_NODE="${ISQL} -s ${OLD_NODE_IP} -port ${OLD_NODE_PORT}"

# Node1, Node3(NodeC)에서 Replication Port No를 얻는다.
echo ""
echo "[Step-2. (Target nodes) Get replication port number]"
echo "Node Info. : ${OLD_NODE_NAME}, ${OLD_NODE_IP}, ${OLD_NODE_PORT}"

NODE_SQL_RESULT=$(
${ISQL_OLD_NODE} << EOF
    SET HEADING OFF;
    SELECT MEMORY_VALUE1 FROM X\$PROPERTY WHERE NAME = 'REPLICATION_PORT_NO';
    QUIT
EOF
)
OLD_NODE_REPL_PORT_NO=$( echo "${NODE_SQL_RESULT}" | grep -v "^1 row selected." | grep -o '[0-9]*' | gawk '{print $1}' )
echo "Replication port number : ${OLD_NODE_REPL_PORT_NO}"

echo "Node Info. : ${NEW_NODE_NAME}, ${NEW_NODE_IP}, ${NEW_NODE_PORT}"

NODE_SQL_RESULT=$(
${ISQL_NEW_NODE} << EOF
    SET HEADING OFF;
    SELECT MEMORY_VALUE1 FROM X\$PROPERTY WHERE NAME = 'REPLICATION_PORT_NO';
    QUIT
EOF
)
NEW_NODE_REPL_PORT_NO=$( echo "${NODE_SQL_RESULT}" | grep -v "^1 row selected." | grep -o '[0-9]*' | gawk '{print $1}' )
echo "Replication port number : ${NEW_NODE_REPL_PORT_NO}"

if [ ${NEW_NODE_ALTERNATE_IP} ]
then
    echo "Alternate Node Info. : ${NEW_NODE_NAME}, ${NEW_NODE_ALTERNATE_IP}, ${NEW_NODE_ALTERNATE_PORT}"

    NODE_SQL_RESULT=$(
    ${ISQL_NEW_NODE_ALTERNATE} << EOF
        SET HEADING OFF;
        SELECT MEMORY_VALUE1 FROM X\$PROPERTY WHERE NAME = 'REPLICATION_PORT_NO';
        QUIT
EOF
)
    NEW_NODE_ALTERNATE_REPL_PORT_NO=$( echo "${NODE_SQL_RESULT}" | grep -v "^1 row selected." | grep -o '[0-9]*' | gawk '{print $1}' )
    echo "Replication port number : ${NEW_NODE_ALTERNATE_REPL_PORT_NO}"
fi

# Node3(NodeC)에 기존 Shard Meta를 복제한다.
#   Node3(NodeC)에 DBMS_SHARD.CREATE_META()로 Shard Meta를 생성한다.
#       그리고 SYS_SHARD.GLOBAL_META_INFO_에 DELETE를 수행하여 Meta SMN을 제거한다.
echo ""
echo "[Step-3. (Target nodes) Create Shard Meta]"
echo "Node Info. : ${NEW_NODE_NAME}, ${NEW_NODE_IP}, ${NEW_NODE_PORT}"

${ISQL_NEW_NODE} << EOF
    EXEC DBMS_SHARD.CREATE_META( ${META_NODE_ID} );
    DELETE FROM SYS_SHARD.GLOBAL_META_INFO_;
    QUIT
EOF

if [ ${NEW_NODE_ALTERNATE_IP} ]
then
    echo "Alternate Node Info. : ${NEW_NODE_NAME}, ${NEW_NODE_ALTERNATE_IP}, ${NEW_NODE_ALTERNATE_PORT}"

    ${ISQL_NEW_NODE_ALTERNATE} << EOF
        EXEC DBMS_SHARD.CREATE_META( ${META_NODE_ALTERNATE_ID} );
        DELETE FROM SYS_SHARD.GLOBAL_META_INFO_;
        QUIT
EOF
fi

#   기존 Node의 Shard Meta를 Node3(NodeC)에 복제한다.
#       (Node1 -> (Node3, NodeC)) Shard Meta Replication을 생성한다.
#       Shard Meta Replication을 완료한다. (Replication Sync & Stop & Drop)
echo ""
echo "[Step-4. (Target nodes) create/sync/stop/drop replication for shard meta tables]"
echo "Node Info. : ${NEW_NODE_NAME}, ${NEW_NODE_IP}, ${NEW_NODE_PORT}"

CREATE_REPLICATION_SQL="CREATE REPLICATION \"${REP_NAME_ADD_NODE}\"
    WITH '${OLD_NODE_IP}', ${OLD_NODE_REPL_PORT_NO}
    FROM SYS_SHARD.GLOBAL_META_INFO_ TO SYS_SHARD.GLOBAL_META_INFO_,
    FROM SYS_SHARD.NODES_            TO SYS_SHARD.NODES_,
    FROM SYS_SHARD.OBJECTS_          TO SYS_SHARD.OBJECTS_,
    FROM SYS_SHARD.RANGES_           TO SYS_SHARD.RANGES_,
    FROM SYS_SHARD.CLONES_           TO SYS_SHARD.CLONES_,
    FROM SYS_SHARD.SOLOS_            TO SYS_SHARD.SOLOS_"
echo "DDL : "${CREATE_REPLICATION_SQL}""

${ISQL_NEW_NODE} << EOF
    ${CREATE_REPLICATION_SQL};
    QUIT
EOF

if [ ${NEW_NODE_ALTERNATE_IP} ]
then
    echo "Alternate Node Info. : ${NEW_NODE_NAME}, ${NEW_NODE_ALTERNATE_IP}, ${NEW_NODE_ALTERNATE_PORT}"

    CREATE_REPLICATION_SQL="CREATE REPLICATION \"${REP_NAME_ADD_NODE_ALTERNATE}\"
        WITH '${OLD_NODE_IP}', ${OLD_NODE_REPL_PORT_NO}
        FROM SYS_SHARD.GLOBAL_META_INFO_ TO SYS_SHARD.GLOBAL_META_INFO_,
        FROM SYS_SHARD.NODES_            TO SYS_SHARD.NODES_,
        FROM SYS_SHARD.OBJECTS_          TO SYS_SHARD.OBJECTS_,
        FROM SYS_SHARD.RANGES_           TO SYS_SHARD.RANGES_,
        FROM SYS_SHARD.CLONES_           TO SYS_SHARD.CLONES_,
        FROM SYS_SHARD.SOLOS_            TO SYS_SHARD.SOLOS_"
    echo "DDL : "${CREATE_REPLICATION_SQL}""

    ${ISQL_NEW_NODE_ALTERNATE} << EOF
        ${CREATE_REPLICATION_SQL};
        QUIT
EOF
fi

echo "Node Info. : ${OLD_NODE_NAME}, ${OLD_NODE_IP}, ${OLD_NODE_PORT}"

CREATE_REPLICATION_SQL="CREATE REPLICATION \"${REP_NAME_ADD_NODE}\"
    WITH '${NEW_NODE_IP}', ${NEW_NODE_REPL_PORT_NO}
    FROM SYS_SHARD.GLOBAL_META_INFO_ TO SYS_SHARD.GLOBAL_META_INFO_,
    FROM SYS_SHARD.NODES_            TO SYS_SHARD.NODES_,
    FROM SYS_SHARD.OBJECTS_          TO SYS_SHARD.OBJECTS_,
    FROM SYS_SHARD.RANGES_           TO SYS_SHARD.RANGES_,
    FROM SYS_SHARD.CLONES_           TO SYS_SHARD.CLONES_,
    FROM SYS_SHARD.SOLOS_            TO SYS_SHARD.SOLOS_"
echo "DDL : "${CREATE_REPLICATION_SQL}""

${ISQL_OLD_NODE} << EOF
    ${CREATE_REPLICATION_SQL};
    EXEC utl_shard_online_rebuild.sync_replication( '${REP_NAME_ADD_NODE}',
                                                    1 );
    EXEC utl_shard_online_rebuild.stop_replication( '${REP_NAME_ADD_NODE}' );
    EXEC utl_shard_online_rebuild.drop_replication( '${REP_NAME_ADD_NODE}' );
    QUIT
EOF

if [ ${NEW_NODE_ALTERNATE_IP} ]
then
    CREATE_REPLICATION_SQL="CREATE REPLICATION \"${REP_NAME_ADD_NODE_ALTERNATE}\"
        WITH '${NEW_NODE_ALTERNATE_IP}', ${NEW_NODE_ALTERNATE_REPL_PORT_NO}
        FROM SYS_SHARD.GLOBAL_META_INFO_ TO SYS_SHARD.GLOBAL_META_INFO_,
        FROM SYS_SHARD.NODES_            TO SYS_SHARD.NODES_,
        FROM SYS_SHARD.OBJECTS_          TO SYS_SHARD.OBJECTS_,
        FROM SYS_SHARD.RANGES_           TO SYS_SHARD.RANGES_,
        FROM SYS_SHARD.CLONES_           TO SYS_SHARD.CLONES_,
        FROM SYS_SHARD.SOLOS_            TO SYS_SHARD.SOLOS_"
    echo "DDL : "${CREATE_REPLICATION_SQL}""

    ${ISQL_OLD_NODE} << EOF
        ${CREATE_REPLICATION_SQL};
        EXEC utl_shard_online_rebuild.sync_replication( '${REP_NAME_ADD_NODE_ALTERNATE}',
                                                        1 );
        EXEC utl_shard_online_rebuild.stop_replication( '${REP_NAME_ADD_NODE_ALTERNATE}' );
        EXEC utl_shard_online_rebuild.drop_replication( '${REP_NAME_ADD_NODE_ALTERNATE}' );
        QUIT
EOF
fi

echo "Node Info. : ${NEW_NODE_NAME}, ${NEW_NODE_IP}, ${NEW_NODE_PORT}"

${ISQL_NEW_NODE} << EOF
    EXEC utl_shard_online_rebuild.drop_replication( '${REP_NAME_ADD_NODE}' );
    QUIT
EOF

if [ ${NEW_NODE_ALTERNATE_IP} ]
then
    echo "Alternate Node Info. : ${NEW_NODE_NAME}, ${NEW_NODE_ALTERNATE_IP}, ${NEW_NODE_ALTERNATE_PORT}"

    ${ISQL_NEW_NODE_ALTERNATE} << EOF
        EXEC utl_shard_online_rebuild.drop_replication( '${REP_NAME_ADD_NODE_ALTERNATE}' );
        QUIT
EOF
fi

#   기존 Node의 Shard Sequence 정보를 Node3(NodeC)에 동기화한다.
echo ""
echo "[Step-5. (Target nodes) Update Shard Sequence]"
echo "Node Info. : ${NEW_NODE_NAME}, ${NEW_NODE_IP}, ${NEW_NODE_PORT}"

cd $(dirname "$0")

/usr/bin/env bash ${DIR_PATH}/updateNodeAndShardID.sh ${OLD_NODE_IP} ${OLD_NODE_PORT} 0 ${NEW_NODE_IP} ${NEW_NODE_PORT}

if [ ${NEW_NODE_ALTERNATE_IP} ]
then
    echo "Alternate Node Info. : ${NEW_NODE_NAME}, ${NEW_NODE_ALTERNATE_IP}, ${NEW_NODE_ALTERNATE_PORT}"

    /usr/bin/env bash ${DIR_PATH}/updateNodeAndShardID.sh ${OLD_NODE_IP} ${OLD_NODE_PORT} 0 ${NEW_NODE_ALTERNATE_IP} ${NEW_NODE_ALTERNATE_PORT}
fi

# Node3(NodeC)를 포함한 모든 Node의 Shard Meta에 Node3(NodeC)를 추가한다. (DBMS_SHARD.SET_NODE())
echo ""
echo "[Step-6. (Target nodes) Set Node to the new one]"
echo "Node Info. : ${NEW_NODE_NAME}, ${NEW_NODE_IP}, ${NEW_NODE_PORT}"

if [ ! ${NEW_NODE_ALTERNATE_IP} ]
then
    SET_NODE_SQL="EXEC DBMS_SHARD.SET_NODE( '${NEW_NODE_NAME}',
                                            '${NEW_NODE_IP}',
                                            ${NEW_NODE_PORT} )"

    ${ISQL_NEW_NODE} << EOF
        ${SET_NODE_SQL};
        QUIT
EOF
else
    SET_NODE_SQL="EXEC DBMS_SHARD.SET_NODE( '${NEW_NODE_NAME}',
                                            '${NEW_NODE_IP}',
                                            ${NEW_NODE_PORT},
                                            '${NEW_NODE_ALTERNATE_IP}',
                                            ${NEW_NODE_ALTERNATE_PORT},
                                            ${NEW_NODE_CONN_TYPE} )"
    ${ISQL_NEW_NODE} << EOF
        ${SET_NODE_SQL};
        QUIT
EOF

    echo "Alternate Node Info. : ${NEW_NODE_NAME}, ${NEW_NODE_ALTERNATE_IP}, ${NEW_NODE_ALTERNATE_PORT}"

    ${ISQL_NEW_NODE_ALTERNATE} << EOF
        ${SET_NODE_SQL};
        QUIT
EOF
fi

# 모든 Node의 접속 정보를 얻는다. (추가 Node 제외)
META_SQL_RESULT=$(
${ISQL} -s ${SHARD_META_SERVER} -port ${SHARD_META_PORT} << EOF
    SET HEADING OFF;
    EXEC utl_shard_online_rebuild.get_connection_info();
    QUIT
EOF
)

echo ""
echo "[Step-7. (All nodes) Set Node to the old ones]"

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
        ${SET_NODE_SQL};
        QUIT
EOF

    if [ ${NODE_ALTERNATE_IP} != "N/A" ]
    then
        echo "Alternate Node Info. : ${NODE_NAME}, ${NODE_ALTERNATE_IP}, ${NODE_ALTERNATE_PORT}"

        ${ISQL} -s ${NODE_ALTERNATE_IP} -port ${NODE_ALTERNATE_PORT} << EOF
            ${SET_NODE_SQL};
            QUIT
EOF
    fi
done

IFS=${IFS_BACKUP}

echo "Done."
