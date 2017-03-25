#!bin/bash
#set -x
num=0
for file in *."$2"
do
num=$[ num + 1 ]
text="$1""$num"".$2"
mv $file $text
done
#set +x
