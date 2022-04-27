#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <errno.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

#define _GNU_SOURCE 
/*
    AF_UNIX use one by one mode,like tcp ,but only use in local,speed is fast
*/

int main(int argc,char **argv)
{
    unlink("mytestfile");
    int ret=socket(AF_UNIX,SOCK_STREAM,0);
    if(ret<0)
    {
        perror("socket:");
        return 1;
    }
    int fd=ret;
    struct sockaddr_un myaddrun;
    myaddrun.sun_family = AF_UNIX;
    strcpy(myaddrun.sun_path,"mytestfile");
    bind(fd,&myaddrun,sizeof(myaddrun));
    listen(fd,2);
    struct sockaddr_un clientaddr;
    int addrlen = sizeof(clientaddr);
    int clifd = accept(fd,&clientaddr,&addrlen);
    if( clifd < 0)
    {
        perror("accept:");
        return 2;
    }
    int shmmemfd = memfd_create("shmfa",0);
    ftruncate(shmmemfd,0x4000);
    char datmmap = (char *)mmap(NULL,0x4000, PROT_READ|PROT_WRITE, MAP_SHARED,shmmemfd,0);
    memset(datmmap,30,0x4000);
    munmap(datmmap, 0x4000);
    // send_fd(clifd,&shmmemfd,1);
    close(clifd);
    close(fd);
    unlink("mytestfile");
    return 0;
}