#define _GNU_SOURCE

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

/*
对文件描述符的操作必须先使用int flags=fcntl(fd,F_GETFL,0)获取当前描述符的属性，然后在此基础上添加或者删除，否则会删除之前的属性（本来可能有多个属性）

设置信号IO驱动，需要设置文件描述符打开O_ASYNC标志使能信号驱动IO
fcntl(fifofd, F_SETFL, f1 | O_NONBLOCK | O_ASYNC);
设定文件符的属主，也就是当文件描述符上可执行IO时会接收通知信号的进程或进程组
fcntl(fifofd, F_SETOWN, getpid());

*/


/*      wait find 
1.sigaction  and  signal

*/
int fifofd;
int inputmode=0;
void *testfun(int sign)
{
    printf("get signal =%d\n",sign);
    char buff[1024];
    int cnt ;
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


void sa_sigaction_fun (int sign, siginfo_t *si, void *ucontext)
{
    printf("recv sign=%d\n",sign);
    printf("recv int =%d\n",(int)(si->si_int));

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
    act.sa_flags   = SA_SIGINFO;//0;
    act.sa_handler = testfun;
    act.sa_sigaction = sa_sigaction_fun;
    int ret = sigaction(SIGRTMIN, &act,NULL);
    // signal(SIGIO,NULL);

    if(fcntl(fifofd, F_SETSIG,SIGRTMIN)==-1)
    {
        printf("set sig fail\n");
    }

    int sig = fcntl(fifofd, F_GETSIG);
    // printf("get sig=%d\n",sig);

    while(1)
    {
        // write(fifofd,"hello",6);
        // write(fifofd,"hello",6);
        // raise(SIGRTMIN);
        union sigval val;
        val.sival_int=100;
        sigqueue(getpid(),SIGRTMIN,val);
        sleep(5);
    }
    close(fifofd);
    return 0;
}