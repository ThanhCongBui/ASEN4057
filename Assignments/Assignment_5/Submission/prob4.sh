#!/bin/bash 
#set -x
counter=0
score=0
while read line || [ -n "$line" ]
do
temp=$( echo "$line" |cut -d';' -f2 )
score=$(( score + temp ))
((counter++))
done < $1

echo "scale=2;$score/$counter" | bc
