#! /bin/bash

# cd filedir/build 
# cmake .. 
# make -j10 

# /usr/bin/expect <<EOF
# spawn scp bin/* zkf@192.168.32.130:/home/zkf/testfile
# expect "zkf@192.168.32.130's password:" 
# send "123456\r"
# interact
# expect eof
# EOF


# # /usr/bin/expect <<EOF
# # spawn scp -r build/bin/ kubot@172.20.24.77:/home/zkf/mytestfun
# # expect \
# #     "(yes/no)?" { send "yes"}  \
# #     "password:" {send HairouKubot_@2018!\r}

# # expect eof
# EOF

# echo $1
# data="123.123.123.123"

# if [[ $data =~ ^(((25[0-5])|(2[0-4][0-9])|([01]?[0-9][0-9]?)).){3}((25[0-5])|(2[0-4][0-9])|([01]?[0-9][0-9]?))$ ]]
# then
#     echo "data1 yes"
# else 
#     echo "data1 no"
# fi

# if [[ ! $1 =~ ^(((25[0-5])|(2[0-4][0-9])|([01]?[0-9][0-9]?)).){3}((25[0-5])|(2[0-4][0-9])|([01]?[0-9][0-9]?))$ ]]
# then 
#     echo "$1 is no IPaddr"
#     exit  1
# fi
# echo "$1 is IPaddr"


# if [[ ! $1 =~ ^ubuntu1[68]$ ]]
# then 
#     echo "$1 is no System version"
#     exit  1
# fi
# echo "$1 is System version"


# cd myaddr
# liblist=$(ls )
# echo ${liblist}
# for newlib in ${liblist}
# do 
#     echo ${newlib}
# done

# echo ${ZZlist}
# if [[ $1 == "172.20.24.211" ]]
# then 
#     echo "num $1==124"
# else 
#     echo "num $1!=124"
# fi


# if [[ $1 == "172.20.24.211" ]]
# then 
#     echo "str $1==124"
# else 
#     echo "str $1!=124"
# fi

# exit




# echo $#    #获取传入参数总数

# for ((index=0 ; idx<=$# ;idx++))
# do
#     echo ${!idx}                # {}里不能使用$号，故使用!号
# done

data=$[1+2]
echo ${data}