#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <errno.h>
#include <sys/un.h>
#include <string.h>
#include <sys/mman.h>

/*
    AF_UNIX use one by one mode,like tcp ,but only use in local,speed is fast
*/

#define HAVE_MSGHDR_MSG_CONTROL


int send_fd(int fd, int *fd_buf ,int fd_num)
{
    struct msghdr msg;
    // 有些系统使用的是旧的msg_accrights域来传递描述符，Linux下是新的msg_control字段
#ifdef HAVE_MSGHDR_MSG_CONTROL
    // union { // 前面说过，保证cmsghdr和msg_control的对齐
    //     struct cmsghdr cm;
    //     char control[CMSG_SPACE(sizeof(int) * fd_num)];
    // }control_un;
    // msg.msg_control = control_un.control; 
    // msg.msg_controllen = CMSG_SPACE(sizeof(int) * fd_num);
    // 设置辅助缓冲区和长度
    char control[CMSG_SPACE(sizeof(int) * fd_num)];
    msg.msg_control = control; 
    msg.msg_controllen = sizeof(control);
    // 只需要一组附属数据就够了，直接通过CMSG_FIRSTHDR取得
    struct cmsghdr *cmptr; 
    cmptr = CMSG_FIRSTHDR(&msg);
    // 设置必要的字段，数据和长度
    cmptr->cmsg_len = CMSG_LEN(sizeof(int) * fd_num); // fd类型是int，设置长度
    cmptr->cmsg_level = SOL_SOCKET; 
    cmptr->cmsg_type = SCM_RIGHTS;  // 指明发送的是描述符
    memcpy(CMSG_DATA(cmptr),fd_buf,sizeof(int) * fd_num); // 把fd写入辅助数据中

#else

    msg.msg_accrights = (caddr_t)&sendfd; // 这个旧的更方便啊
    msg.msg_accrightslen = sizeof(int);
#endif
    // UDP才需要，无视
    msg.msg_name = NULL;
    msg.msg_namelen = 0;
    // 别忘了设置数据缓冲区，实际上1个字节就够了
    struct iovec iov[1];
    char ptr;
    iov[0].iov_base = &ptr;
    iov[0].iov_len = 1;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;
    return sendmsg(fd, &msg, 0);
}


int main(int argc,char **argv)
{
    int ret = socket(AF_UNIX,SOCK_STREAM,0);
    if(ret<0)
    {
        perror("socket");
        return 1;
    }
    int fd =ret;
    struct sockaddr_un myaddrun;
    myaddrun.sun_family = AF_UNIX;
    strcpy(myaddrun.sun_path,"mytestfile");
    if(connect(fd,&myaddrun,sizeof(myaddrun)))
    {
        perror("connect");
        return 2;
    }
    
    int shmmemfd = memfd_create("shmfa1",0);
    ftruncate(shmmemfd,0x4000);
    char *datmmap = (char *)mmap(NULL,0x4000, PROT_READ|PROT_WRITE, MAP_SHARED,shmmemfd,0);
    memset(datmmap,0x30,0x4000);

    int shmmemfdb = memfd_create("shmfa2",0);
    ftruncate(shmmemfdb,0x4000);
    char *datmmapb = (char *)mmap(NULL,0x4000, PROT_READ|PROT_WRITE, MAP_SHARED,shmmemfdb,0);
    memset(datmmapb,0x30,0x4000);

    int fdbuf[2];
    fdbuf[0] = shmmemfd;
    fdbuf[1] = shmmemfdb;
    char ptr;
    send_fd(fd, fdbuf,2);
    sleep(1);
    printf("1:data[0]=%c\n",datmmap[0]);
    printf("2:data[0]=%c\n",datmmapb[0]);

    close(fd);
    close(shmmemfd);
    close(shmmemfdb);
    munmap(datmmap, 0x4000);
    munmap(datmmapb, 0x4000);
    return 0;
}