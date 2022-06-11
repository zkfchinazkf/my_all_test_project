#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

/*
对文件描述符的操作必须先使用int flags=fcntl(fd,F_GETFL,0)获取当前描述符的属性，然后在此基础上添加或者删除，否则会删除之前的属性（本来可能有多个属性）

*/

int fifofd;
int inputmode=0;
void *testfun(void)
{
    printf("get SIGIO\n");
    char buff[1024];
    int cnt ;
    // if(inputmode)
    // {
    //     inputmode = 0;
    //     return ;
    // }
    // else 
    // {
    //     inputmode = 1;
    // }
    cnt = read(fifofd,buff,1024);
    if(cnt<0)
    {
        perror("fifofd read");
        printf("err=%d\n",errno);
    }
    else 
    {
        for(int idx=0;idx<cnt;idx++)
        {
            printf("%c",buff[idx]);
        }
        printf("\n");
    }
}

int main(int argc,char **argv)
{
    unlink( "./myddd" );
    mkfifo("./myddd",0777);
    fifofd = open("./myddd",O_RDWR|O_CREAT,0777);
    if(fifofd<0)
    {
        perror("fifofd open");
        return 1;
    }

    int f1 = fcntl(fifofd, F_GETFL, 0);
    fcntl(fifofd, F_SETFL, f1 | O_NONBLOCK | O_ASYNC);
    fcntl(fifofd, F_SETOWN, getpid());

    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags   = 0;
    act.sa_handler = testfun;
    int ret = sigaction(SIGIO, &act,NULL);
    // signal(SIGIO,NULL);
    while(1)
    {
        sleep(1);
    }
//     int flags=fcntl(fifofd,F_GETFL,0); 
//     flags |= O_NONBLOCK;
//     fcntl(fifofd,F_SETFL,flags);
    
//     char buff[1024];
//     int cnt ;
// again:
//     cnt = read(fifofd,buff,1024);
//     if(cnt<0)
//     {
//         if(errno == EAGAIN)
//         {
//             sleep(2);
//             printf("try again\n");
//             goto again;
//         }
//         perror("fifofd read");
//         printf("err=%d\n",errno);
//     }
//     else 
//     {
//         for(int idx=0;idx<cnt;idx++)
//         {
//             printf("%c",buff[idx]);
//         }
//         printf("\n");
//     }
    close(fifofd);
    return 0;
}