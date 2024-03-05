#!/bin/bash
cd tests
num=0
for dir in `ls -l | grep ^d | awk '{print $9}'`
do
    if [ $dir -gt $num ]; then
        num=$dir
    fi
done
mkdir `expr $num + 1`
cd `expr $num + 1`
touch in.sy
touch out.md
touch error.txt