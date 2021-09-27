#!/bin/bash

OS_NAME="`uname -s`"
NCURSES5_LINKPATH=../lib/libncurses.so.5
TINFO5_LINKPATH=../lib/libtinfo.so.5

if [[ "$OS_NAME" = "Linux"* ]];
then
    VER5=`ldconfig -p | grep libncurses.so.5 | grep "64" -m 1 | awk '{print $NF}'`
    VER6=`ldconfig -p | grep libncurses.so.6 | grep "64" -m 1 | awk '{print $NF}'`

    if [[ -z $VER5 ]] && [[ -n $VER6 ]]; then
        ln -sf $VER6 $NCURSES5_LINKPATH    
    fi

    VER5=`ldconfig -p | grep libtinfo.so.5 | grep "64" -m 1 | awk '{print $NF}'`
    VER6=`ldconfig -p | grep libtinfo.so.6 | grep "64" -m 1 | awk '{print $NF}'`

    if [[ -z $VER5 ]] && [[ -n $VER6 ]]; then
        ln -sf $VER6 $TINFO5_LINKPATH
    fi
fi
