#!/bin/bash
name=$( echo "$1" | cut -d'.' -f1 )
mkdir -p ~/$name
tar -zvxf $1 -C ~/$name
cd ~/$name
ls -X > ~/list.txt
#set -x
while read line
do 
foldername=$( echo "$line" | cut -d'.' -f2 )
if [[ -d $foldername ]]; then
mv $line ~/$name/$foldername
else
mkdir -p $foldername
mv $line ~/$name/$foldername
fi
done < ~/list.txt 
cd ~
tar -zcvf $name"_clean".tar.gz $name
rm ~/list.txt
rm -r ~/$name
