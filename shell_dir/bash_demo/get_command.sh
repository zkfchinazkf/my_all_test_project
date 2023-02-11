#! /bin/bash

# **********************************************************************************
# $? get last cmd result ,success is zero
# $(cmd)  use cmd,return is cmd return         `` can did but cann't nesting
# **********************************************************************************



# data=`demo`
# echo ${data}
sh -c "gcc 1.c"
ret=$?

if [ ${ret} != 0  ];then
    echo ${ret}!=0
# elif [ ${ret} == 0  ];then  
else
    echo ${ret}==0
fi

sh -c "./a.out 1>/dev/null 2>/dev/null"
ret=$?
if [ ${ret} != 0  ];then
    echo ${ret}!=0
fi

sh -c "gcc 2.c 1>/dev/null 2>/dev/null"
ret=$?
if [ ${ret} != 0  ];then
    echo ${ret}!=0
fi


data2=$(pwd)
ret=$?
echo ${data2}  ${ret}

data2=$(rm helloworld 1>/dev/null 2>/dev/null)
ret=$?
echo ${data2}  ${ret}