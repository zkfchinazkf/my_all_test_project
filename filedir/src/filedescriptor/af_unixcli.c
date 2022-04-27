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
    char buff[1024];
    int cnt = read(fd,buff,1024);
    printf("cnt=%d\n",cnt);
    close(fd);
    return 0;
}