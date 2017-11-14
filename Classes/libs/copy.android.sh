#!/bin/sh
path=$(dirname $0)
if [ ${path:0:1} == \. ]
then
path=${path/\./$(pwd)}
fi

#
cp -f $path/libSingleEye.android.a $path/libSingleEye.a