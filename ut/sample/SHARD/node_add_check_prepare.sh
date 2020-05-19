#!/bin/bash 

APPLICATION=$0

if [ $# -ne 6 -a $# -ne 8 ]
then
    echo "Usage : ${APPLICATION} shard_meta_server shard_meta_port sys_password new_node_name host_ip port_no [alternate_host_ip alternate_port_no]"
    exit 1
fi

SHARD_META_SERVER=$1
SHARD_META_PORT=$2
PASSWORD=$3
NEW_NODE_NAME=$4
NEW_NODE_IP=$5
NEW_NODE_PORT=$6
NEW_NODE_ALTERNATE_IP=$7
NEW_NODE_ALTERNATE_PORT=${8}

IFS_BACKUP=${IFS}

ISQL="${ALTIBASE_HOME}/bin/isql -silent -u sys -p ${PASSWORD} "

REP_NAME_ADD_NODE="REP_ADD_NODE"
REP_NAME_ADD_NODE_ALTERNATE="REP_ADD_NODE_ALTERNATE"

echo ""
echo "[Step-0. Check Node, Property, Partiton, Partition Method, Node Split, Replication]"

check_failure=0
failiure=0

echo "New Node: ${NEW_NODE_NAME}"
check=$(
    ${ISQL} -s ${NEW_NODE_IP} -port ${NEW_NODE_PORT} << EOF
        SET HEADING OFF;
        SET FEED OFF;
        SELECT COUNT(*) 
        FROM SYSTEM_.SYS_USERS_
        WHERE USER_NAME = 'SYS_SHARD';
        quit
EOF
)

if [ "${check}" -ne 0 ]; then
    echo "There are data for shard in this node -> Failure"
    failiure=1
else
    echo "This node is ready to add shard -> Success"
fi

if [ ${NEW_NODE_ALTERNATE_IP} ]; then
    echo "New Node: ${NEW_NODE_NAME} ALTERNATE"

    check=$(
        ${ISQL} -s ${NEW_NODE_ALTERNATE_IP} -port ${NEW_NODE_ALTERNATE_PORT} << EOF
            SET HEADING OFF;
            SET FEED OFF;
            SELECT COUNT(*) 
            FROM SYSTEM_.SYS_USERS_
            WHERE USER_NAME = 'SYS_SHARD';
            quit
EOF
)
    if [ "${check}" -ne 0 ]; then
        echo "There are data for shard in this node -> Failure"
        failiure=1
    else
        echo "This node is ready to add shard -> Success"
    fi
fi

if [ "${check}" -ne 0 ]; then
    echo "There are data for shard in this node -> Failure"
    failiure=1
else
    echo "This node is ready to add shard -> Success"
fi

META_SQL_RESULT=$(
${ISQL} -s ${SHARD_META_SERVER} -port ${SHARD_META_PORT} << EOF
    SET HEADING OFF;
    SET FEED OFF;
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

    echo "Node : ${NODE_NAME}"

    IFS=$' '

    check=$( 
        ${ISQL} -s ${NODE_IP} -port ${NODE_PORT} << EOF
            SET HEADING OFF;
            SET FEED OFF;
            exec utl_shard_online_rebuild.CHECK_EXIST_NODE( '${NEW_NODE_NAME}', FALSE );
            exec utl_shard_online_rebuild.CHECK_EXIST_REPLICATION( '${REP_NAME_ADD_NODE}', FALSE );
            exec utl_shard_online_rebuild.CHECK_EXIST_REPLICATION( '${REP_NAME_ADD_NODE_ALTERNATE}', FALSE );
            quit
EOF
)
    echo "${check}" | grep -v 'Execute success'
    
    check_failure=$( echo "${check}" | grep "Failure" | wc -l )
    if [ "${check_failure}" -ne 0 ]; then
        failiure=1
    fi

    if [ ${NODE_ALTERNATE_IP} != "N/A" ]; then
        echo "Node : ${NODE_NAME} ALTERNATE"

        check=$( 
            ${ISQL} -s ${NODE_ALTERNATE_IP} -port ${NODE_ALTERNATE_PORT} << EOF
                SET HEADING OFF;
                SET FEED OFF;
                exec utl_shard_online_rebuild.CHECK_EXIST_NODE( '${NEW_NODE_NAME}', FALSE );
                exec utl_shard_online_rebuild.CHECK_EXIST_REPLICATION( '${REP_NAME_ADD_NODE}', FALSE );
                exec utl_shard_online_rebuild.CHECK_EXIST_REPLICATION( '${REP_NAME_ADD_NODE_ALTERNATE}', FALSE );
                quit
EOF
)
        echo "${check}" | grep -v 'Execute success'
        
        check_failure=$( echo "${check}" | grep "Failure" | wc -l )
        if [ "${check_failure}" -ne 0 ]; then
            failiure=1
        fi
    fi
done

IFS=${IFS_BACKUP}

if [ ${failiure} -eq 0 ] ; then
    echo '############################################################'
    echo '# PREPARATION Success'
    echo '############################################################'
    exit 0
else
    echo '############################################################'
    echo '# PREPARATION Failure'
    echo '############################################################'
    exit 1
fi

