#!/bin/bash
cd $1
ls -X > ~/list.txt
while read line1
do
while read line2
do
~/mat_multiply $line1 $line2
done < ~/list.txt 
done < ~/list.txt
rm ~/list.txt
cd
