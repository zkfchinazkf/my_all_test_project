#! /bin/bash

# -o StrictHostKeyChecking=no  去掉公钥确认

sshpass -p "123456" ssh ubuntu64@192.168.32.130  "echo hello"
echo $?

sshpass -p "HairouKubot_@2018!" ssh -o StrictHostKeyChecking=no kubot@172.18.118.78   "echo hello" 
echo $?

for index in {100..150};do
    ping 192.168.32.${index} -w 1 >/dev/null 2>/dev/null
    ret=$?
    if [ ${ret} -eq 0 ] ;then
        echo "192.168.32.${index} is can connect,ret=${ret}"
    fi
done