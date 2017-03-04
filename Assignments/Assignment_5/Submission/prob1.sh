#!/bin/bash -x
counter=$1
until [ $counter -gt $2 ]
do
echo $counter
((counter+=1))
done 
