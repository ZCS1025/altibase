#!/usr/bin/env bash

APPLICATION=$0

if [ $# -ne 1 ]
then
    echo "Usage : ${APPLICATION} configure_file"
    echo ""
    echo "# configure_file sample"
    echo "SHARD_META_SERVER=127.0.0.1"
    echo "SHARD_META_PORT=20300"
    echo "SYS_PASSWORD=MANAGER"
    echo "FROM_NODE_NAME=NODE2"
    echo "TO_NODE_NAME=NODE3"
    echo "SHARD_OBJECT=USER01 T1 P1"
    echo "SHARD_OBJECT=USER02 S1"
    echo "SHARD_OBJECT=USER03 C1"
    echo "REP_NAME_WITH_ALTERNATE_IN_FROM_NODE=REP_WITH_ALTERNATE"
    exit 1
fi

CONFIGURE_FILE=$1

IFS_BACKUP=${IFS}

GET_VALUE()
{
    IFS=$'\n'

    CONF_ARRAY=$( cat ${CONFIGURE_FILE} | grep -v "^#" | grep -e "=" )

    for LINE in ${CONF_ARRAY[@]}
    do
        NAME=$( echo ${LINE} | gawk -F "=" '{print $1}' )
        VALUE=$( echo ${LINE} | gawk -F "=" '{print $2}' )

        if [ ${NAME} = $1 ]
        then
            echo "${VALUE}"
            break
        fi
    done

    IFS=${IFS_BACKUP}
}

GET_ALL_SHARD_OBJECTS()
{
    IFS=$'\n'

    SHARD_OBJECTS_COUNT=0

    CONF_ARRAY=$( cat ${CONFIGURE_FILE} | grep -v "^#" | grep -e "^SHARD_OBJECT=" )

    for LINE in ${CONF_ARRAY[@]}
    do
        VALUE=$( echo ${LINE} | gawk -F "=" '{print $2}' )

        USER_NAME_ARRAY[$SHARD_OBJECTS_COUNT]=$( echo ${VALUE} | gawk '{print $1}' )
        TABLE_NAME_ARRAY[$SHARD_OBJECTS_COUNT]=$( echo ${VALUE} | gawk '{print $2}' )
        PARTITION_NAME_ARRAY[$SHARD_OBJECTS_COUNT]=$( echo ${VALUE} | gawk '{print $3}' )

        let SHARD_OBJECTS_COUNT=$SHARD_OBJECTS_COUNT+1
    done

    IFS=${IFS_BACKUP}
}

SHARD_META_SERVER=$( GET_VALUE "SHARD_META_SERVER" )
SHARD_META_PORT=$( GET_VALUE "SHARD_META_PORT" )
PASSWORD=$( GET_VALUE "SYS_PASSWORD" )
FROM_NODE_NAME=$( GET_VALUE "FROM_NODE_NAME" )
TO_NODE_NAME=$( GET_VALUE "TO_NODE_NAME" )
REP_NAME_WITH_ALTERNATE_IN_FROM_NODE=$( GET_VALUE "REP_NAME_WITH_ALTERNATE_IN_FROM_NODE" )

GET_ALL_SHARD_OBJECTS
