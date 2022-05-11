#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#define Getdata(num)     \
    ({                   \
        int ret=0;       \
        if((num>10))     \
        {                \
            ret=20;      \
        }                \
        else             \
        {                \
            ret=30;      \
        }                \
        ret;             \
    })

int main(int argc,char **argv)
{
    /*show define return demo*/
    /*-----------start---------*/
    int dd = Getdata(2);
    printf("dd=%d\n",dd);
    /*-----------end---------*/


    struct termios termios_init;
    struct termios termios_now;
    int ret=tcgetattr(fileno(stdin),&termios_init);
    if(ret)
    {
        perror("tcgetattr ");
        return 1;
    }
    termios_now = termios_init;
    termios_now.c_lflag &= ~ICANON;
    termios_now.c_lflag &= ~ECHO;
    termios_now.c_lflag &= ~ISIG;
    ret=tcsetattr(fileno(stdin),TCSANOW,&termios_now);
    printf("now input ,but will no show\r\n");
    sleep(4);
    tcflush(fileno(stdin),TCIOFLUSH);   //if no clear flush,will show in termios
    ret=tcsetattr(fileno(stdin),TCSANOW,&termios_init);
    if(ret)
    {
        perror("tcsetattr ");
        return 1;
    }

    fprintf(stdout,"dadada");
    tcflush(fileno(stdout),TCIOFLUSH);
    fprintf(stdout,"\r\n");
    fflush(stdout);   //将缓冲区的数据输出
    return 0;
}