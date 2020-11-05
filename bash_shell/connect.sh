#!/bin/sh

for name in `seq -w 39`
do
if [-f log_$fname];then
    continue
else
    expect expect_demo node$name "c6Civ%Ft" 
    fname=`hostname`
    sh ping.sh > log_$fname
fi
done

mkdir log_node
mv log* log_node