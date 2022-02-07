#include <iostream>
#include <thread>
#include <unistd.h>
#include <sys/prctl.h>

int main(int argc,char **argv)
{
    std::thread([](void)->bool{
        prctl(PR_SET_NAME,"testxc");
        char namebuf[100];
        prctl(PR_GET_NAME,namebuf);
        printf("name=%s\n",namebuf);
        while(1)
        {
            sleep(2);
            char *p=nullptr;
            *p=11;
        }
    }
    ).detach();
    while(1)
    {
        std::cout<<"helloworld"<<std::endl;
        sleep(1);
    }
    return 0;
}