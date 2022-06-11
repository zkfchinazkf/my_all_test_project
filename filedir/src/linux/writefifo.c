#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc,char **argv)
{
    int fifofd = open("./myddd",O_RDWR|O_CREAT,0777);
    if(fifofd<0)
    {
        perror("fifofd open");
        return 1;
    }
    char buff[1024]="helloworld";
    int cnt = write(fifofd,buff,11);
    if(cnt<0)
    {
        perror("fifofd read");
    }
    while(1);
    close(fifofd);
    return 0;
}