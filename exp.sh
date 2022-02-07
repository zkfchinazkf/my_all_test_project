#!/usr/bin/expect  

set user [lindex $argv 0]  
spawn ./filedir/build/myexpect
expect "please input:"  
send "ddd1\r"   
expect "please input:"  
send "ddd2\r"   
expect "please input:"  
send "ddd3\r"  
expect "please input:"  
# send "end\r"  

# interact

expect eof;#表示结束 与spawn成对应关系

spawn ./filedir/build/myexpect
expect "please input:"  
send "ddd1\r"   
expect "please input:"  
send "ddd2\r"   
expect "please input:"  
send "ddd3\r"  
expect "please input:"  
send "end\r"  

interact

expect eof;#表示结束 与spawn成对应关系


# sleep 5
# spawn scp ./filedir/build/Demo zkf@192.168.32.130:/home/zkf/testfile
# expect "zkf@192.168.32.130's password:" 
# send "123456\n"




# spawn ./filedir/build/myexpect
# 以下的形式为自动读取回复式
# expect {
# "input:" {send "ddd1\r";exp_continue}
# }
# expect eof;

# exit 



# /usr/bin/expect <<EOF
# spawn scp bin/* zkf@192.168.32.130:/home/zkf/testfile
# expect "zkf@192.168.32.130's password:" 
# send "123456\r"
# interact
# expect eof
# EOF