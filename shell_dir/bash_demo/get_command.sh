#! /bin/bash

# **********************************************************************************
# $? get last cmd result ,success is zero
# $(cmd)  use cmd,return is cmd return         `` can did but cann't nesting
# **********************************************************************************

# **********************************************************************************
# *******************************----shell pirnt color----**************************
# \033与\e一样，所以二者可以互换
#  \033[配置;配置;配置m   配置可以为多个或一个
#  \033[0m 是 shell 的默认颜色
# 文本属性：0:默认值、1:粗体、22:非粗体、4:下划线、24:非下划线、5:闪烁、25:非闪烁、7:反显、27:非反显
# 颜色对应编号颜色(基础值，需加偏移值)   0:黑 1:红 2:绿 3:黄 4:蓝色 5:紫色 6:深绿 7:白色
# 字体基本颜色号码  在颜色的基础上加30  如黑:30
# 字体高亮颜色号码  在颜色的基础上加90  如黑:90
# 字背景颜色范围    在颜色的基础上加40  如黑:40
# 
# printf 可以直接打印颜色,需要加\n实现转行
# echo 需要加上-e支持转义,会自行转行
# **********************************************************************************

echo -e "\e[42;34;4m ddddd \e[0m"
echo -e "\e[42;94;4m ddddd \e[0m"
printf "\e[42;34;4m ddddd\e[0m \n"
printf "\e[42;94;4m ddddd\e[0m \n"

demofun(){
    if [ -z $3 ];then   #check input is zero
        echo $1
    else 
        echo $2
    fi
}

demofun "no input 3 ,need input" "input 3 ,success"  
demofun "no input 3 ,need input" "input 3 ,success"  "cc"

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