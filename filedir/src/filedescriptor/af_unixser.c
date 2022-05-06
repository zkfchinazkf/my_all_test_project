#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <errno.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>

#define _GNU_SOURCE 
/*
    AF_UNIX use one by one mode,like tcp ,but only use in local,speed is fast
*/

#define HAVE_MSGHDR_MSG_CONTROL


int read_fd(int fd, int *fd_buf,int fd_num)
{
    struct msghdr msg;
    int n;
    int newfd;
#ifdef HAVE_MSGHDR_MSG_CONTROL
    union{ // 对齐
	struct cmsghdr cm;
    char control[CMSG_SPACE(sizeof(int) * fd_num)];
    }control_un;
    struct cmsghdr *cmptr;
    // 设置辅助数据缓冲区和长度
    msg.msg_control = control_un.control;
    msg.msg_controllen = sizeof(control_un.control);
#else
    msg.msg_accrights = (caddr_t) &newfd; // 这个简单
    msg.msg_accrightslen = sizeof(int);
#endif 
    
    // TCP无视
    msg.msg_name = NULL;
    msg.msg_namelen = 0;
    // 设置数据缓冲区
    char ptr;
    struct iovec iov={.iov_base=&ptr,.iov_len=1};
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    // 设置结束，准备接收
    if((n = recvmsg(fd, &msg, 0)) <= 0)
    {
        return n;
    }
#ifdef HAVE_MSGHDR_MSG_CONTROL
    // 检查是否收到了辅助数据，以及长度，回忆上一节的CMSG宏
    cmptr = CMSG_FIRSTHDR(&msg);
    
    if((cmptr != NULL) && (cmptr->cmsg_len == CMSG_LEN(sizeof(int)* fd_num)))
    {
	// 还是必要的检查
        if(cmptr->cmsg_level != SOL_SOCKET)
        {
            printf("control level != SOL_SOCKET/n");
            exit(-1);
        }
        if(cmptr->cmsg_type != SCM_RIGHTS)
        {
            printf("control type != SCM_RIGHTS/n");
            exit(-1);
        }
	// 好了，描述符在这
        memcpy(fd_buf,CMSG_DATA(cmptr),sizeof(int) * fd_num);
    }
    else
    {
        if(cmptr == NULL) 
        {
            printf("null cmptr, fd not passed./n");
        }
        else 
        {
            printf("message len[%ld] if incorrect./n", cmptr->cmsg_len);
        }
        *fd_buf = -1; // descriptor was not passed
    }
#else
    if(msg.msg_accrightslen == sizeof(int)) *recvfd = newfd; 
    else *recvfd = -1;
#endif
    return n;
}



int main(int argc,char **argv)
{
    unlink("/tmp/mytestfile");
    int ret=socket(AF_UNIX,SOCK_STREAM,0);
    if(ret<0)
    {
        perror("socket:");
        return 1;
    }
    int fd=ret;
    struct sockaddr_un myaddrun;
    myaddrun.sun_family = AF_UNIX;
    strcpy(myaddrun.sun_path,"/tmp/mytestfile");
    bind(fd,(struct sockaddr *)&myaddrun,sizeof(myaddrun));
    listen(fd,2);
    struct sockaddr_un clientaddr;
    int addrlen = sizeof(clientaddr);
    int clifd = accept(fd,(struct sockaddr *)&clientaddr,&addrlen);
    if( clifd < 0)
    {
        perror("accept:");
        return 2;
    }
    
    int recfd[2];
    char ptr;
    read_fd(clifd, recfd,2);
    
    char *datmmap = (char *)mmap(NULL,0x4000, PROT_READ|PROT_WRITE, MAP_SHARED,recfd[0],0);
    memset(datmmap,'s',0x4000);
    munmap(datmmap,0x4000);

    char *datmmap2 = (char *)mmap(NULL,0x4000, PROT_READ|PROT_WRITE, MAP_SHARED,recfd[1],0);
    memset(datmmap2,'d',0x4000);
    munmap(datmmap2,0x4000);
    
    close(recfd[0]);
    close(recfd[1]);
    close(clifd);
    close(fd);
    unlink("/tmp/mytestfile");
    return 0;
}