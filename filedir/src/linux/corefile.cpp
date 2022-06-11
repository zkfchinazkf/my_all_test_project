#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <execinfo.h>
#include <list>
#include <vector>

#define BACKTRACE_SIZE 100


class myclass
{
public:
    int data=100;
    std::vector<int> dd;
};




void dump(int sig)
{
        int i = 0, nptrs = 0;
        void *buf[BACKTRACE_SIZE];	//定义用于存放获取到堆栈信息的指针数组
        char **strings;
        nptrs = backtrace(buf, BACKTRACE_SIZE);	//返回获取到的实际堆栈信息指针数

        printf("backtrace() returned %d addresses\n", nptrs);
        strings = backtrace_symbols(buf, nptrs);	//将buf中存放的信息转换为可打印的字符串信息
        if (strings == NULL)
        {
                perror("backtrace_symbols");
                exit(EXIT_FAILURE);
        }
        for (i = 0; i < nptrs; i++)
        {
                printf(" [%02d] %s\n", i, strings[i]);
        }
        free(strings);	//释放一整块存放信息的字符串内存
        abort();
}

void myfun()
{
    myclass *classtest;
    classtest->dd.resize(4);
    classtest->dd.push_back(2);
    classtest->dd.push_back(2);


//     std::list<int>  corelist;
//     corelist.push_back(1);
//     corelist.push_back(3);
//     corelist.push_back(5);
//     for(auto iter = corelist.begin();iter!=corelist.end();iter++)
//     {
//         corelist.erase(iter);
//     }
}

int main(int argc,char **argv)
{
        struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;
    act.sa_handler = dump;
    sigaction(SIGSEGV, &act, NULL);

    char *p=NULL;
    printf("hellowworld\n");

//     myclass *mp = nullptr;
//     mp->data = 200;
//     printf("data=%d\n",mp->data);
    myfun();
    return 1;
}