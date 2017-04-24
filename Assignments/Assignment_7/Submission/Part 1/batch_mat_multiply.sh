#!/bin/bash
cd $1
ls -X > ~/list.txt
while read line1
do
while read line2
do
while read line3
do
~/mat_ops $line1 $line2 $line3
done < ~/list.txt 
done < ~/list.txt
done < ~/list.txt
rm ~/list.txt
cd
