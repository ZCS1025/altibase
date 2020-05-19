#!/bin/bash 

APPLICATION=$0

if [ $# -ne 4 ]
then
    echo $#
    echo "Usage : ${APPLICATION} shard_meta_server shard_meta_port sys_password old_node_name"
    exit 1
fi

SHARD_META_SERVER=$1
SHARD_META_PORT=$2
PASSWORD=$3
OLD_NODE_NAME=$4

IFS_BACKUP=${IFS}

ISQL="${ALTIBASE_HOME}/bin/isql -silent -u sys -p ${PASSWORD} "

REP_NAME_ADD_NODE="REP_ADD_NODE"
REP_NAME_ADD_NODE_ALTERNATE="REP_ADD_NODE_ALTERNATE"

echo ""
echo "[Step-0. Check Node, Property, Partiton, Partition Method, Node Split, Replication]"

check_failure=0
failiure=0

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
            exec utl_shard_online_rebuild.CHECK_EXIST_NODE( '${OLD_NODE_NAME}', FALSE );
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
                exec utl_shard_online_rebuild.CHECK_EXIST_NODE( '${OLD_NODE_NAME}', FALSE );
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
    echo '# MAIN Success'
    echo '############################################################'
    exit 0
else
    echo '############################################################'
    echo '# MAIN Failure'
    echo '############################################################'
    exit 1
fi

