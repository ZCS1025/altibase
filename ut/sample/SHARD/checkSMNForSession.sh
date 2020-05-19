#!/bin/bash 

DIR_PATH=$( cd `dirname $0` && pwd )
source ${DIR_PATH}/configure.sh

ISQL="${ALTIBASE_HOME}/bin/isql -silent -u sys -p ${PASSWORD} "
################################################################################
function print_smn_and_session()
{
    local ip=$1
    local port=$2
    local NODE_NAME=$3

    ${ISQL} -s ${ip} -port ${port} << EOF
        SET FEED OFF;

        SELECT '[INFO: ${NODE_NAME}]' INFO, SHARD_META_NUMBER
        FROM SYS_SHARD.GLOBAL_META_INFO_
        ORDER BY SHARD_META_NUMBER;

        
        SELECT '[INFO: ${NODE_NAME}]' INFO,  SHARD_META_NUMBER, COUNT(*)
        FROM X\$SESSION 
        GROUP BY SHARD_META_NUMBER
        ORDER BY SHARD_META_NUMBER;

        SELECT '[INFO: ${NODE_NAME}]' INFO, ID, SHARD_META_NUMBER 
        FROM X\$SESSION 
        GROUP BY ID, SHARD_META_NUMBER
        ORDER BY ID, SHARD_META_NUMBER;

        quit;
EOF
}
################################################################################

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

    isPrint=false

    if [ ${FROM_NODE_NAME} ]; then
        if [ ${FROM_NODE_NAME} = ${NODE_NAME} ]; then
            isPrint=true
        fi
    fi

    if [ ${TO_NODE_NAME} ]; then
        if [ ${TO_NODE_NAME} = ${NODE_NAME} ]; then
            isPrint=true
        fi
    fi

    if [ ${isPrint} = true ]; then
        echo '############################################################'
        print_smn_and_session ${NODE_IP} ${NODE_PORT} ${NODE_NAME}

        if [ ${NODE_ALTERNATE_IP} != "N/A" ]; then
            print_smn_and_session ${NODE_ALTERNATE_IP} ${NODE_ALTERNATE_PORT} ${NODE_NAME}'_ALTERNATE'
        fi
        echo '############################################################'
    fi

    IFS=$' '
done

IFS=${IFS_BACKUP}

