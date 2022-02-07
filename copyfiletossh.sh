#!/bin/bash


if [[ ! $1 =~ ^(((25[0-5])|(2[0-4][0-9])|([01]?[0-9][0-9]?)).){3}((25[0-5])|(2[0-4][0-9])|([01]?[0-9][0-9]?))$ ]]
then 
    echo "$1 is no IPaddr"
    exit  1
fi
echo "$1 is IPaddr"

if [[ ! $2 =~ ^ubuntu1[68]$ ]]
then 
    echo "$2 is no System version"
    exit  1
fi
echo "$2 is System version"

rsync_path="kubot@$1:/home/zkf/mytestfun"

echo $rsync_path
sshpass -p "HairouKubot_@2018!" rsync -avzP exp.sh  $rsync_path