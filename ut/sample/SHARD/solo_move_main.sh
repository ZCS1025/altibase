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
REBUILD_DATA_STEP_FAILURE=-1
REBUILD_DATA_STEP_SUCCESS=0
REBUILD_DATA_STEP_LOCKED=1
LOCK_WAIT_SECONDS_CHECK=5
LOCK_WAIT_SECONDS_FROM_NODE=5
LOCK_WAIT_SECONDS_TO_NODE=5

DIR_PATH=$(cd `dirname $0` && pwd)
/usr/bin/env bash ${DIR_PATH}/solo_move_check_main.sh ${1}
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

# From/To Node 접속 정보를 추출한다.
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
        if [ ${NODE_ALTERNATE_IP} != "N/A" ]
        then
            FROM_NODE_ALTERNATE_IP=${NODE_ALTERNATE_IP}
            FROM_NODE_ALTERNATE_PORT=${NODE_ALTERNATE_PORT}
        fi
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
if [ ${FROM_NODE_ALTERNATE_IP} ]
then
    ISQL_FROM_NODE_ALTERNATE="${ISQL} -s ${FROM_NODE_ALTERNATE_IP} -port ${FROM_NODE_ALTERNATE_PORT}"
fi
ISQL_TO_NODE="${ISQL} -s ${TO_NODE_IP} -port ${TO_NODE_PORT}"

LOCK_AND_TRUNCATE_TABLE()
{
    # Node2의 T3에 X Lock을 잡는다. (Autocommit Off)
    # Node2에서 Replication Sync를 완료한다.
    #   (Node2 -> Node3) Shard Data Replication Flush
    # Rebuild Data Step이 성공/실패로 변경될 때까지 대기한다.
    # Node2(NodeB)에서 T3의 데이터를 제거한다.
    #   Node2의 T3에 Truncate를 수행한다. (단계 4에서 잡은 X Lock 해제)
    echo "Node Info. : ${FROM_NODE_NAME}, ${FROM_NODE_IP}, ${FROM_NODE_PORT}"

    REBUILD_DATA_STEP_LOCKED=$1
    USER_NAME=$2
    TABLE_NAME=$3

    ${ISQL_FROM_NODE} << EOF
        ALTER SESSION SET AUTOCOMMIT = FALSE;
        EXEC utl_shard_online_rebuild.lock_and_truncate( '${USER_NAME}',
                                                         '${TABLE_NAME}',
                                                         NULL,
                                                         '${REP_NAME_REBUILD}',
                                                         ${LOCK_WAIT_SECONDS_FROM_NODE},
                                                         ${REBUILD_DATA_STEP_LOCKED},
                                                         ${REBUILD_DATA_STEP_SUCCESS},
                                                         ${REBUILD_DATA_STEP_FAILURE} );
        COMMIT;
        QUIT
EOF

    #   [Check] (Node2 -> NodeB) Shard Data Replication Flush
    #   NodeB의 T3에 Truncate를 수행한다.
    if [ ${FROM_NODE_ALTERNATE_IP} ]
    then
        NODE_SQL_RESULT=$(
            ${ISQL_FROM_NODE} << EOF
                SET HEADING OFF;
                EXEC utl_shard_online_rebuild.print_rebuild_data_step();
                QUIT
EOF
)
        REBUILD_DATA_STEP=$( echo "${NODE_SQL_RESULT}" | grep -v "^Execute success." | grep -o '[0-9]*' | gawk '{print $1}' )

        if [ ${REBUILD_DATA_STEP} -eq ${REBUILD_DATA_STEP_SUCCESS} ]
        then
            if [ ${REP_NAME_WITH_ALTERNATE_IN_FROM_NODE} ]
            then
                ${ISQL_FROM_NODE} << EOF
                    EXEC utl_shard_online_rebuild.flush_replication( '${REP_NAME_WITH_ALTERNATE_IN_FROM_NODE}' );
                    QUIT
EOF
            fi

            echo "Alternate Node Info. : ${FROM_NODE_NAME}, ${FROM_NODE_ALTERNATE_IP}, ${FROM_NODE_ALTERNATE_PORT}"

            TRUNCATE_TABLE_SQL="TRUNCATE TABLE \"${USER_NAME}\".\"${TABLE_NAME}\""
            echo "DDL : ${TRUNCATE_TABLE_SQL}"

            ${ISQL_FROM_NODE_ALTERNATE} << EOF
                ${TRUNCATE_TABLE_SQL};
                QUIT
EOF
        fi
    fi
}

LOCK_AND_SWAP_TABLE()
{
    # Node3에서 T3에 X Lock을 잡는다. (Autocommit Off)
    # Rebuild Data Step이 성공/실패로 변경될 때까지 대기한다.
    # Node3에서 T3와 T3_REBUILD를 Swap한다. (Commit)
    # Replication이 Standby Node에서 T3와 T3_REBUILD를 Swap한다.
    #   (Node3 -> NodeC) Shard Data Replication Flush
    echo "Node Info. : ${TO_NODE_NAME}, ${TO_NODE_IP}, ${TO_NODE_PORT}"

    REBUILD_DATA_STEP_LOCKED=$1
    USER_NAME=$2
    TABLE_NAME=$3
    REBUILD_TABLE_NAME="${TABLE_NAME}_REBUILD"

    if [ ! ${TO_NODE_ALTERNATE_IP} ]
    then
        REP_NAME_ALTERNATE=""
    fi

    ${ISQL_TO_NODE} << EOF
        ALTER SESSION SET AUTOCOMMIT = FALSE;
        EXEC utl_shard_online_rebuild.lock_and_swap( '${USER_NAME}',
                                                     '${TABLE_NAME}',
                                                     '${REBUILD_TABLE_NAME}',
                                                     NULL,
                                                     '${REP_NAME_ALTERNATE}',
                                                     ${LOCK_WAIT_SECONDS_TO_NODE},
                                                     ${REBUILD_DATA_STEP_LOCKED},
                                                     ${REBUILD_DATA_STEP_SUCCESS},
                                                     ${REBUILD_DATA_STEP_FAILURE} );
        COMMIT;
        QUIT
EOF

    # 성공한 경우 LOCK_AND_TRUNCATE_TABLE()에서 대기 중인 작업을 진행한다.
    NODE_SQL_RESULT=$(
        ${ISQL_TO_NODE} << EOF
            SET HEADING OFF;
            EXEC utl_shard_online_rebuild.print_rebuild_data_step();
            QUIT
EOF
)
    REBUILD_DATA_STEP=$( echo "${NODE_SQL_RESULT}" | grep -v "^Execute success." | grep -o '[0-9]*' | gawk '{print $1}' )

    if [ ${REBUILD_DATA_STEP} -eq ${REBUILD_DATA_STEP_SUCCESS} ]
    then
        echo "Node Info. : ${FROM_NODE_NAME}, ${FROM_NODE_IP}, ${FROM_NODE_PORT}"

        ${ISQL_FROM_NODE} << EOF
            EXEC utl_shard_online_rebuild.insert_rebuild_state( ${REBUILD_DATA_STEP_LOCKED},
                                                                ${REBUILD_DATA_STEP_FAILURE} );
            QUIT
EOF
    else
        echo "Node Info. : ${FROM_NODE_NAME}, ${FROM_NODE_IP}, ${FROM_NODE_PORT}"

        ${ISQL_FROM_NODE} << EOF
            EXEC utl_shard_online_rebuild.set_rebuild_data_step( ${REBUILD_DATA_STEP_FAILURE} );
            QUIT
EOF
    fi
}

echo "[Step-1. (Target nodes) Check SHARD_REBUILD_DATA_STEP property]"
echo "Node Info. : ${TO_NODE_NAME}, ${TO_NODE_IP}, ${TO_NODE_PORT}"

NODE_SQL_RESULT=$(
    ${ISQL_TO_NODE} << EOF
        SET HEADING OFF;
        EXEC utl_shard_online_rebuild.print_rebuild_data_step();
        QUIT
EOF
)
REBUILD_DATA_STEP=$( echo "${NODE_SQL_RESULT}" | grep -v "^Execute success." | grep -o '[0-9]*' | gawk '{print $1}' )

if [ ${REBUILD_DATA_STEP} -ne ${REBUILD_DATA_STEP_SUCCESS} ]
then
    ${ISQL_TO_NODE} << EOF
        EXEC utl_shard_online_rebuild.set_rebuild_data_step( ${REBUILD_DATA_STEP_FAILURE} );
        QUIT
EOF

    echo "SHARD_REBUILD_DATA_STEP property is not ${REBUILD_DATA_STEP_SUCCESS}. Wait for ${LOCK_WAIT_SECONDS_CHECK} seconds."
    sleep ${LOCK_WAIT_SECONDS_CHECK}
fi

echo "Node Info. : ${FROM_NODE_NAME}, ${FROM_NODE_IP}, ${FROM_NODE_PORT}"

NODE_SQL_RESULT=$(
    ${ISQL_FROM_NODE} << EOF
        SET HEADING OFF;
        EXEC utl_shard_online_rebuild.print_rebuild_data_step();
        QUIT
EOF
)
REBUILD_DATA_STEP=$( echo "${NODE_SQL_RESULT}" | grep -v "^Execute success." | grep -o '[0-9]*' | gawk '{print $1}' )

if [ ${REBUILD_DATA_STEP} -ne ${REBUILD_DATA_STEP_SUCCESS} ]
then
    ${ISQL_FROM_NODE} << EOF
        EXEC utl_shard_online_rebuild.set_rebuild_data_step( ${REBUILD_DATA_STEP_FAILURE} );
        QUIT
EOF

    echo "SHARD_REBUILD_DATA_STEP property is not ${REBUILD_DATA_STEP_SUCCESS}. Wait for ${LOCK_WAIT_SECONDS_CHECK} seconds."
    sleep ${LOCK_WAIT_SECONDS_CHECK}
fi

echo ""
echo "[Step-2. (Target nodes) Preparation]"
echo "Node Info. : ${FROM_NODE_NAME}, ${FROM_NODE_IP}, ${FROM_NODE_PORT}"

NODE_SQL_RESULT=$(
${ISQL_FROM_NODE} << EOF
    DELETE FROM SYS_SHARD.REBUILD_STATE_;
    QUIT
EOF
)

${ISQL_FROM_NODE} << EOF
    EXEC utl_shard_online_rebuild.flush_replication( '${REP_NAME_REBUILD}' );
    QUIT
EOF

if [ ${TO_NODE_ALTERNATE_IP} ]
then
    echo "Node Info. : ${TO_NODE_NAME}, ${TO_NODE_IP}, ${TO_NODE_PORT}"

    ${ISQL_TO_NODE} << EOF
        EXEC utl_shard_online_rebuild.flush_replication( '${REP_NAME_ALTERNATE}' );
        QUIT
EOF
fi

echo ""
echo "[Step-3. (Target nodes) Lock table & flush replication & wait]"
echo "Node Info. : ${FROM_NODE_NAME}, ${FROM_NODE_IP}, ${FROM_NODE_PORT}"

${ISQL_FROM_NODE} << EOF
    EXEC utl_shard_online_rebuild.set_rebuild_data_step( ${REBUILD_DATA_STEP_SUCCESS} );
    QUIT
EOF

for (( i = 0 ; i < $SHARD_OBJECTS_COUNT ; i++ ))
do
    let REBUILD_DATA_STEP_LOCKED=$i+1

    LOCK_AND_TRUNCATE_TABLE ${REBUILD_DATA_STEP_LOCKED} ${USER_NAME_ARRAY[$i]} ${TABLE_NAME_ARRAY[$i]} &

    ${ISQL_FROM_NODE} << EOF
        EXEC utl_shard_online_rebuild.wait_rebuild_data_step( ${REBUILD_DATA_STEP_LOCKED} );
        QUIT
EOF

    NODE_SQL_RESULT=$(
        ${ISQL_FROM_NODE} << EOF
            SET HEADING OFF;
            EXEC utl_shard_online_rebuild.print_rebuild_data_step();
            QUIT
EOF
)
    REBUILD_DATA_STEP=$( echo "${NODE_SQL_RESULT}" | grep -v "^Execute success." | grep -o '[0-9]*' | gawk '{print $1}' )

    if [ ${REBUILD_DATA_STEP} -ne ${REBUILD_DATA_STEP_LOCKED} ]
    then
        ${ISQL_FROM_NODE} << EOF
            EXEC utl_shard_online_rebuild.set_rebuild_data_step( ${REBUILD_DATA_STEP_FAILURE} );
            QUIT
EOF

        echo "Canceled."
        exit 2
    fi
done

echo ""
echo "[Step-4. (Target nodes) Lock table & wait]"
echo "Node Info. : ${TO_NODE_NAME}, ${TO_NODE_IP}, ${TO_NODE_PORT}"

${ISQL_TO_NODE} << EOF
    EXEC utl_shard_online_rebuild.set_rebuild_data_step( ${REBUILD_DATA_STEP_SUCCESS} );
    QUIT
EOF

for (( i = 0 ; i < $SHARD_OBJECTS_COUNT ; i++ ))
do
    let REBUILD_DATA_STEP_LOCKED=$i+1

    LOCK_AND_SWAP_TABLE ${REBUILD_DATA_STEP_LOCKED} ${USER_NAME_ARRAY[$i]} ${TABLE_NAME_ARRAY[$i]} &

    ${ISQL_TO_NODE} << EOF
        EXEC utl_shard_online_rebuild.wait_rebuild_data_step( ${REBUILD_DATA_STEP_LOCKED} );
        QUIT
EOF

    NODE_SQL_RESULT=$(
        ${ISQL_TO_NODE} << EOF
            SET HEADING OFF;
            EXEC utl_shard_online_rebuild.print_rebuild_data_step();
            QUIT
EOF
)
    REBUILD_DATA_STEP=$( echo "${NODE_SQL_RESULT}" | grep -v "^Execute success." | grep -o '[0-9]*' | gawk '{print $1}' )

    if [ ${REBUILD_DATA_STEP} -ne ${REBUILD_DATA_STEP_LOCKED} ]
    then
        ${ISQL_TO_NODE} << EOF
            EXEC utl_shard_online_rebuild.set_rebuild_data_step( ${REBUILD_DATA_STEP_FAILURE} );
            QUIT
EOF

        echo "Canceled."
        exit 3
    fi
done

# 모든 Node의 접속 정보를 얻는다.
META_SQL_RESULT=$(
${ISQL} -s ${SHARD_META_SERVER} -port ${SHARD_META_PORT} << EOF
    SET HEADING OFF;
    EXEC utl_shard_online_rebuild.get_connection_info();
    QUIT
EOF
)

# 모든 Node에 증가한 Data SMN을 설정한다. (각 Node에 수행)
echo ""
echo "[Step-5. (All nodes) Set new data SMN]"

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
        ALTER SYSTEM RELOAD SHARD META NUMBER LOCAL;
        QUIT
EOF

    if [ ${NODE_ALTERNATE_IP} != "N/A" ]
    then
        echo "Alternate Node Info. : ${NODE_NAME}, ${NODE_ALTERNATE_IP}, ${NODE_ALTERNATE_PORT}"

        ${ISQL} -s ${NODE_ALTERNATE_IP} -port ${NODE_ALTERNATE_PORT} << EOF
            ALTER SYSTEM RELOAD SHARD META NUMBER LOCAL;
            QUIT
EOF
    fi
done

IFS=${IFS_BACKUP}

echo ""
echo "[Step-6. (Target nodes) Swap table & flush replication]"
echo "[Step-7. (Target nodes) Truncate Table]"
echo "Node Info. : ${FROM_NODE_NAME}, ${FROM_NODE_IP}, ${FROM_NODE_PORT}"

${ISQL_FROM_NODE} << EOF
    EXEC utl_shard_online_rebuild.set_rebuild_data_step( ${REBUILD_DATA_STEP_SUCCESS} );
    QUIT
EOF

echo ""
echo "Node Info. : ${TO_NODE_NAME}, ${TO_NODE_IP}, ${TO_NODE_PORT}"

${ISQL_TO_NODE} << EOF
    EXEC utl_shard_online_rebuild.set_rebuild_data_step( ${REBUILD_DATA_STEP_SUCCESS} );
    QUIT
EOF

# Background로 실행한 작업이 끝나기를 기다린다.
wait

echo ""
echo "[Step-8. (Target nodes) Finalization]"
echo "Node Info. : ${FROM_NODE_NAME}, ${FROM_NODE_IP}, ${FROM_NODE_PORT}"

DELETE_SQL="DELETE FROM SYS_SHARD.REBUILD_STATE_"
echo "DML : "${DELETE_SQL}""

${ISQL_FROM_NODE} << EOF
    ${DELETE_SQL};
    QUIT
EOF

echo "Done."
