#!/bin/sh
path=$(dirname $0)
if [ ${path:0:1} == \. ]
then
path=${path/\./$(pwd)}
fi

#
cp -f $path/libSingleEye.ios.a $path/libSingleEye.a