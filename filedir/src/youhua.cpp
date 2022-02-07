#include <cstdio>
#include <iostream>
#include <time.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "mymath.h"
#include "semlock.h"
#include "memory.h"
#include <vector>

int main(int argc,char **argv)
{
    struct timeval oldtime,nowtime;
    gettimeofday(&oldtime,NULL);
    
    std::cout<<GetMax(100,120)<<std::endl;

    SEMAPHORE semdata("/home",33,1);
    semdata.init();
    // SHMEMORY  shmemory("mytestfilename");
    SHMEMORY  shmemory("aaa");
    shmemory.init();
    std::vector<uint8_t> vecdata;
    vecdata.resize(1024);
    sprintf((char *)(vecdata.data()),"helloworld man\n");
    std::vector<uint8_t>recdata = shmemory.get();
    printf("%s",recdata.data());
    semdata.lock();
    shmemory.set(vecdata);
    semdata.unlock();
    semdata.lock();
    semdata.lock(51);

    gettimeofday(&nowtime,NULL);
    double dectime = (nowtime.tv_sec-oldtime.tv_sec)*1.0f + (nowtime.tv_usec-oldtime.tv_usec)/1000000.0f;
    std::cout<<"usetime is "<<dectime<< std::endl;

    return 0;
}