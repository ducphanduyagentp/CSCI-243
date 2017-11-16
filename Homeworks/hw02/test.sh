#!/bin/bash
./compile.sh $1
for (( i=$2 ; i<=$3 ; i++ ))
do
    echo $i
    ./$1 <sample$i.csv >out$i.txt
    diff output$i.txt out$i.txt
done

