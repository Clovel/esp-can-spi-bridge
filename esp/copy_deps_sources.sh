#!/bin/bash

MYDIR=$(dirname $(readlink -f $0))

cp -v $MYDIR/submodules/MCP_CAN_lib/*.c $MYDIR/src/
cp -v $MYDIR/submodules/MCP_CAN_lib/*.cpp $MYDIR/src/

cp -v $MYDIR/submodules/MCP_CAN_lib/*.h $MYDIR/inc/
cp -v $MYDIR/submodules/MCP_CAN_lib/*.hpp $MYDIR/inc/
