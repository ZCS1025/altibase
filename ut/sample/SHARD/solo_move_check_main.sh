#!/bin/bash 

DIR_PATH=$( cd `dirname $0` && pwd )
source ${DIR_PATH}/configure.sh

REP_NAME_REBUILD="REP_REBUILD"
REP_NAME_ALTERNATE="REP_ALTERNATE"

IFS_BACKUP=${IFS}

ISQL="${ALTIBASE_HOME}/bin/isql -silent -u sys -p ${PASSWORD}"

################################################################################

function check_table()
{
    local sNode_ip=${1}
    local sNode_port=${2}

    local i=0
    local sFailiure=0


    for (( i = 0; i < ${SHARD_OBJECTS_COUNT}; i++ ));
    do
        check=$(
        ${ISQL} -s ${sNode_ip} -port ${sNode_port} << EOF
            SET HEADING OFF;
            SET FEED OFF;

            exec utl_shard_online_rebuild.CHECK_EXIST_TABLE( '${USER_NAME_ARRAY[$i]}',
                                                             '${TABLE_NAME_ARRAY[$i]}',
                                                             TRUE );

            exec utl_shard_online_rebuild.CHECK_EXIST_SOLO( '${USER_NAME_ARRAY[$i]}',
                                                            '${TABLE_NAME_ARRAY[$i]}',
                                                            TRUE );

            exec utl_shard_online_rebuild.CHECK_SOLO( '${USER_NAME_ARRAY[$i]}',
                                                      '${TABLE_NAME_ARRAY[$i]}',
                                                      '${FROM_NODE_NAME}',
                                                      FALSE );

            exec utl_shard_online_rebuild.CHECK_SOLO( '${USER_NAME_ARRAY[$i]}',
                                                      '${TABLE_NAME_ARRAY[$i]}',
                                                      '${TO_NODE_NAME}',
                                                      TRUE );

EOF
)
        echo "${check}" | grep -v 'Execute success'

        check_failure=$( echo "${check}" | grep "Failure" | wc -l )

        if [ "${check_failure}" -ne 0 ]; then
            sFailiure=1
        fi
    done


    return ${sFailiure}

}

################################################################################

echo ""
echo "[Step-0. Check Node, Property, Partiton, Partition Method, Node Split, Replication]"

NODE_NAME_ARRAY=()
NODE_IP_ARRAY=()
NODE_PORT_ARRAY=()
NODE_ALTERNATE_IP_ARRAY=()
NODE_ALTERNATE_PORT_ARRAY=()

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

    NODE_NAME_ARRAY=( ${NODE_NAME_ARRAY[@]}  ${NODE_NAME} )
    NODE_IP_ARRAY=( ${NODE_IP_ARRAY[@]}  ${NODE_IP} )
    NODE_PORT_ARRAY=( ${NODE_PORT_ARRAY[@]}  ${NODE_PORT} )
    NODE_ALTERNATE_IP_ARRAY=( ${NODE_ALTERNATE_IP_ARRAY[@]}  ${NODE_ALTERNATE_IP} )
    NODE_ALTERNATE_PORT_ARRAY=( ${NODE_ALTERNATE_PORT_ARRAY[@]}  ${NODE_ALTERNATE_PORT} )

    echo "Node Info. : ${NODE_NAME}, ${NODE_IP}, ${NODE_PORT}"

    if [ ${NODE_ALTERNATE_IP} != "N/A" ]; then
        echo "Alternate Node Info. : ${NODE_NAME}, ${NODE_ALTERNATE_IP}, ${NODE_ALTERNATE_PORT}"
    fi

    IFS=$' '
done

NODE_COUNT=${#NODE_NAME_ARRAY[@]}

if [ ${NODE_COUNT} -eq 0 ]; then
    echo 'Node does not exist'

    echo '############################################################'
    echo '# PREPARATION Failure'
    echo '############################################################'
    exit 1
fi

IFS=${IFS_BACKUP}

check_failure=0
failiure=0

for (( i = 0; i < ${NODE_COUNT}; i++ ));
do 
    if [ ${FROM_NODE_NAME} = ${NODE_NAME_ARRAY[${i}]} ]; then
        echo '############################################################'
        echo '# Check' ${NODE_NAME_ARRAY[${i}]}
        echo '############################################################'

        check=$(
        ${ISQL} -s ${NODE_IP_ARRAY[$i]} -port ${NODE_PORT_ARRAY[$i]} << EOF
            SET HEADING OFF;
            SET FEED OFF;

            exec utl_shard_online_rebuild.CHECK_ALL_PROPERTY();
            exec utl_shard_online_rebuild.CHECK_EXIST_NODE( '${FROM_NODE_NAME}', TRUE );
            exec utl_shard_online_rebuild.CHECK_EXIST_NODE( '${TO_NODE_NAME}', TRUE );
            exec utl_shard_online_rebuild.CHECK_EXIST_REPLICATION( '${REP_NAME_REBUILD}', TRUE );
            exec utl_shard_online_rebuild.CHECK_REPLICATION_ROLE( '${REP_NAME_REBUILD}', 0 );
            exec utl_shard_online_rebuild.CHECK_REPLICATION_GAP( '${REP_NAME_REBUILD}' );
EOF
)
        echo "${check}" | grep -v 'Execute success'
        check_failure=$( echo "${check}" | grep "Failure" | wc -l )
        if [ "${check_failure}" -ne 0 ]; then
            failiure=1
        fi

        check_table ${NODE_IP_ARRAY[$i]} ${NODE_PORT_ARRAY[$i]}
        result=$?

        if [ "${result}" -ne 0 ]; then
            failiure=1
        fi
    fi 

    if [ ${TO_NODE_NAME} = ${NODE_NAME_ARRAY[${i}]} ]; then
        echo '############################################################'
        echo '# Check' ${NODE_NAME_ARRAY[${i}]}
        echo '############################################################'

        if [ ${NODE_ALTERNATE_IP_ARRAY[${i}]} != 'N/A' ]; then
            check=$(
            ${ISQL} -s ${NODE_IP_ARRAY[$i]} -port ${NODE_PORT_ARRAY[$i]} << EOF
                SET HEADING OFF;
                SET FEED OFF;

                exec utl_shard_online_rebuild.CHECK_ALL_PROPERTY();
                exec utl_shard_online_rebuild.CHECK_EXIST_NODE( '${FROM_NODE_NAME}', TRUE );
                exec utl_shard_online_rebuild.CHECK_EXIST_NODE( '${TO_NODE_NAME}', TRUE );
                exec utl_shard_online_rebuild.CHECK_EXIST_REPLICATION( '${REP_NAME_REBUILD}', TRUE );
                exec utl_shard_online_rebuild.CHECK_REPLICATION_ROLE( '${REP_NAME_REBUILD}', 2 );
                exec utl_shard_online_rebuild.CHECK_EXIST_REPLICATION( '${REP_NAME_ALTERNATE}', TRUE );
                exec utl_shard_online_rebuild.CHECK_REPLICATION_ROLE( '${REP_NAME_ALTERNATE}', 3 );
                exec utl_shard_online_rebuild.CHECK_REPLICATION_GAP( '${REP_NAME_ALTERNATE}' );
EOF
)
            echo "${check}" | grep -v 'Execute success'
            check_failure=$( echo "${check}" | grep "Failure" | wc -l )
            if [ "${check_failure}" -ne 0 ]; then
                failiure=1
            fi

            check_table ${NODE_IP_ARRAY[$i]} ${NODE_PORT_ARRAY[$i]}
            result=$?

            if [ "${result}" -ne 0 ]; then
                failiure=1
            fi

            echo '############################################################'
            echo '# Check' ${NODE_NAME_ARRAY[${i}]} alternate
            echo '############################################################'

            check=$(
            ${ISQL} -s ${NODE_ALTERNATE_IP_ARRAY[$i]} -port ${NODE_ALTERNATE_PORT_ARRAY[$i]} << EOF
                SET HEADING OFF;
                SET FEED OFF;
                exec utl_shard_online_rebuild.CHECK_ALL_PROPERTY();
                exec utl_shard_online_rebuild.CHECK_EXIST_NODE( '${FROM_NODE_NAME}', TRUE );
                exec utl_shard_online_rebuild.CHECK_EXIST_NODE( '${TO_NODE_NAME}', TRUE );
                exec utl_shard_online_rebuild.CHECK_EXIST_REPLICATION( '${REP_NAME_ALTERNATE}', TRUE );
                exec utl_shard_online_rebuild.CHECK_REPLICATION_ROLE( '${REP_NAME_ALTERNATE}', 0 );
EOF
)
            echo "${check}" | grep -v 'Execute success'
            check_failure=$( echo "${check}" | grep "Failure" | wc -l )
            if [ "${check_failure}" -ne 0 ]; then
                failiure=1
            fi

            check_table ${NODE_ALTERNATE_IP_ARRAY[$i]} ${NODE_ALTERNATE_PORT_ARRAY[$i]}
            result=$?

            if [ "${result}" -ne 0 ]; then
                failiure=1
            fi
        else
            check=$(
            ${ISQL} -s ${NODE_IP_ARRAY[$i]} -port ${NODE_PORT_ARRAY[$i]} << EOF
                SET HEADING OFF;
                SET FEED OFF;

                exec utl_shard_online_rebuild.CHECK_ALL_PROPERTY();
                exec utl_shard_online_rebuild.CHECK_EXIST_NODE( '${FROM_NODE_NAME}', TRUE );
                exec utl_shard_online_rebuild.CHECK_EXIST_NODE( '${TO_NODE_NAME}', TRUE );
                exec utl_shard_online_rebuild.CHECK_EXIST_REPLICATION( '${REP_NAME_REBUILD}', TRUE );
                exec utl_shard_online_rebuild.CHECK_REPLICATION_ROLE( '${REP_NAME_REBUILD}', 2 );
EOF
)
            echo "${check}" | grep -v 'Execute success'
            check_failure=$( echo "${check}" | grep "Failure" | wc -l )
            if [ "${check_failure}" -ne 0 ]; then
                failiure=1
            fi

            check_table ${NODE_IP_ARRAY[$i]} ${NODE_PORT_ARRAY[$i]}
            result=$?
            if [ "${result}" -ne 0 ]; then
                failiure=1
            fi
        fi
    fi
done

DIR_PATH=$(cd `dirname $0` && pwd)
/usr/bin/env bash ${DIR_PATH}/checkSMNForSession.sh ${1}

if [ ${failiure} -eq 0 ] ; then
    echo '############################################################'
    echo '# MAIN Success'
    echo '############################################################'
    exit 0
else
    echo '############################################################'
    echo '# MAIN Failure'
    echo '############################################################'
    exit 1
fi

