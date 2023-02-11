
import paramiko
import uuid
 
# need load paramiko   
# this  demo is upload and load file to ssh 



#执行命令
# # 建立一个sshclient对象
# ssh = paramiko.SSHClient()
# # 允许将信任的主机自动加入到host_allow 列表，此方法必须放在connect方法的前面
# ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())

# # 调用connect方法连接服务器
# ssh.connect(
#     hostname = "192.168.32.130",
#     port = 22,
#     username = "ubuntu64",
#     password = "123456"
# )

# # 执行命令
# stdin,stdout,stderr = ssh.exec_command("df -h")
# print(stdout.read().decode())

# ssh.close()


# # 上传文件
# # 获取Transport实例
# trans = paramiko.Transport(
#     sock=("192.168.32.130",22)
# )
# trans.connect(
#     username="ubuntu64",
#     password="123456"
# )
# # 获取SFTP实例
# sftp = paramiko.SFTPClient.from_transport(trans)
# # 上传文科
# sftp.put("./a.out","/home/ubuntu64/a.out")
# # 下载文件
# sftp.get("/home/ubuntu64/demofile","./demofile")
# trans.close()




# 获取Transport实例
trans = paramiko.Transport(
    sock=("192.168.32.130",22)
)
trans.connect(
    username="ubuntu64",
    password="123456"
)

# 获取ssh对象
ssh = paramiko.SSHClient()
ssh._transport = trans

# 执行命令
stdin,stdout,stderr = ssh.exec_command("df -h")
print(stdout.read().decode())


# 获取sftp对象
sftp = paramiko.SFTPClient.from_transport(trans)
# 上传文科
sftp.put("./a.out","/home/ubuntu64/a.out")
# 下载文件
sftp.get("/home/ubuntu64/demofile","./demofile")
trans.close()



trans.close()
