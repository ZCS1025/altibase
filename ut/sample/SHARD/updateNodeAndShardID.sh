#!/bin/bash 

ip=${1}
port=${2}
mode=${3}       # 0 : to_node_name update 
                # 1 : max_value update for all node
to_ip=${4}
to_port=${5}

temp=temp

ISQL="${ALTIBASE_HOME}/bin/isql -silent -u sys -p MANAGER "
################################################################################
function get_next_shard_id()
{
    local ip=$1
    local port=$2

current_value=$(
    ${ISQL} -s ${ip} -port ${port} << EOF
        SET HEADING OFF;
        SET FEED OFF;

        SELECT S.CURRENT_SEQ 
        FROM SYSTEM_.SYS_TABLES_ T,
             V\$SEQ              S 
        WHERE T.TABLE_OID = S.SEQ_OID AND
              T.TABLE_NAME = 'NEXT_SHARD_ID';
        quit;
EOF
)

    eval "$3='${current_value}'"
}

function get_next_node_id()
{
    local ip=$1
    local port=$2

current_value=$(
    ${ISQL} -s ${ip} -port ${port} << EOF
        SET HEADING OFF;
        SET FEED OFF;

        SELECT S.CURRENT_SEQ
        FROM SYSTEM_.SYS_TABLES_ T,
             V\$SEQ              S
        WHERE T.TABLE_OID = S.SEQ_OID AND
              T.TABLE_NAME = 'NEXT_NODE_ID';
        quit;
EOF
)

    eval "$3='${current_value}'"
}

function set_next_shard_id()
{
    local ip=$1
    local port=$2
    local next_shard_id=$3

    ${ISQL} -s ${ip} -port ${port} << EOF 
    exec utl_shard_online_rebuild.SET_NEXT_SHARD_ID( ${next_shard_id} );
    quit;
EOF
}

function set_next_node_id()
{
    local ip=$1
    local port=$2
    local next_node_id=$3

    ${ISQL} -s ${ip} -port ${port} << EOF 
    exec utl_shard_online_rebuild.SET_NEXT_NODE_ID( ${next_node_id} );
    quit;
EOF
}

function get_max_next_shard_id()
{
    local node_ip_list=( ${!1} )
    local node_port_list=( ${!2} )
    local node_count=$3

    get_next_shard_id ${node_ip_list[0]} ${node_port_list[0]} max_next_shard_id

    for (( i = 1; i < ${node_count}; i++ ));
    do 
        get_next_shard_id ${node_ip_list[$i]} ${node_port_list[$i]} next_shard_id

        if [ ${max_next_shard_id} ]; then
            if [ ${next_shard_id} ]; then
                if [ ${next_shard_id} -gt ${max_next_shard_id} ]; then
                    max_next_shard_id=${next_shard_id}
                fi
            fi
        else
            max_next_shard_id=${next_shard_id}
        fi
    done
    
    eval "$4='${max_next_shard_id}'"
}

function get_max_next_node_id()
{
    local node_ip_list=( ${!1} )
    local node_port_list=( ${!2} )
    local node_count=$3
    
    get_next_node_id ${node_ip_list[0]} ${node_port_list[0]} max_next_node_id

    for (( i = 1; i < ${node_count}; i++ ));
    do 
        get_next_node_id ${node_ip_list[$i]} ${node_port_list[$i]} next_node_id

        if [ ${max_next_node_id} ]; then
            if [ ${next_node_id} ]; then
                if [ ${next_node_id} -gt ${max_next_node_id} ]; then
                    max_next_node_id=${next_node_id}
                fi
            fi
        else
            max_next_node_id=${next_node_id}
        fi
    done

    eval "$4='${max_next_node_id}'"
}

################################################################################

if [ -e ${temp} ]; then
    echo ${temp} 'directory exists'
#    exit 1
else
    mkdir ${temp}
fi

${ISQL} -s ${ip} -port ${port} << EOF > ${temp}/node.info
    exec UTL_SHARD_ONLINE_REBUILD.GET_NODE_INFO;
    quit;
EOF

sed -i '1d' ${temp}/node.info
sed -i '$d' ${temp}/node.info

node_name_list=()
node_ip_list=()
node_port_list=()
node_alternate_ip_list=()
node_alternate_port_list=()

while read node_name node_ip node_port node_alternate_ip node_alternate_port smn
do 
    node_name_list=( ${node_name_list[@]}  ${node_name} )
    node_ip_list=( ${node_ip_list[@]}  ${node_ip} )
    node_port_list=( ${node_port_list[@]}  ${node_port} )
    node_alternate_ip_list=( ${node_alternate_ip_list[@]}  ${node_alternate_ip} )
    node_alternate_port_list=( ${node_alternate_port_list[@]}  ${node_alternate_port} )
done < ${temp}/node.info

if [ ${node_name_list[0]} = "Nothing" ]; then 
    echo 'Node does not exist'
    exit 1
fi

node_count=${#node_name_list[@]}

case ${mode} in
    0)
        get_next_shard_id ${ip} ${port} next_shard_id
        get_next_node_id ${ip} ${port} next_node_id
        ;;

    1)
        get_max_next_shard_id node_ip_list[@] node_port_list[@] ${node_count} next_shard_id
        get_max_next_node_id node_ip_list[@] node_port_list[@] ${node_count} next_node_id
        ;;

esac

case ${mode} in

    0)
        if [ ${next_shard_id} ]; then
            set_next_shard_id ${to_ip} ${to_port} ${next_shard_id}
        fi
        
        if [ ${next_node_id} ]; then
            set_next_node_id ${to_ip} ${to_port} ${next_node_id}
        fi
        ;;
    1)
        for (( i = 0; i < ${node_count}; i++ ))
        do 
        
            echo '############################################################'
            echo '# Update Shard and Node ID : ' ${node_name_list[${i}]}
            echo '############################################################'
        
            if [ ${next_shard_id} ]; then
                set_next_shard_id ${node_ip_list[$i]} ${node_port_list[$i]} ${next_shard_id}
            fi
        
            if [ ${next_node_id} ]; then
                set_next_node_id ${node_ip_list[$i]} ${node_port_list[$i]} ${next_node_id}
            fi
        
            if [ ${node_alternate_ip_list[$i]} ]; then
        
                echo '############################################################'
                echo '# Update Shard snd Node ID : ' ${node_name_list[${i}]} ' alternate'
                echo '############################################################'
        
                if [ ${next_shard_id} ]; then
                    set_next_shard_id ${node_alternate_ip_list[$i]} ${node_alternate_port_list[$i]} ${next_shard_id}
                fi
        
                if [ ${next_node_id} ]; then
                    set_next_node_id ${node_alternate_ip_list[$i]} ${node_alternate_port_list[$i]} ${next_node_id}
                fi
            fi 
        done
        ;;
esac

rm -rf ${temp}

