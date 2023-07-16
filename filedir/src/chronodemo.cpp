#include <iostream>
#include <chrono>
#include <unistd.h>


int main(int argc,char **argv)
{
    auto mindata = std::chrono::steady_clock::time_point::min();
    auto maxdara = std::chrono::steady_clock::time_point::max();
    auto nowdata = std::chrono::steady_clock::now();
    
    std::cout<<mindata.time_since_epoch().count()<<std::endl;  //get min time,-9223372036854775808,can use in init or clear data
    std::cout<<maxdara.time_since_epoch().count()<<std::endl;  //get max time,9223372036854775807,can use in init or clear data
    std::cout<<nowdata.time_since_epoch().count()<<std::endl;

    
    auto nowtime = std::chrono::steady_clock::now();
    
    std::cout<<nowtime.time_since_epoch().count()<<std::endl;
    usleep(1000000);
    std::cout<<"delta time is "<<(std::chrono::steady_clock::now().time_since_epoch().count()-nowtime.time_since_epoch().count())/1000000000.0f<<std::endl;

}


// #include <stdio.h>
// #include <sys/sysinfo.h>
// #include <time.h>
// #include <errno.h>
// #include <stdlib.h>
// #include <string.h>
// int main(void)
// {
//     struct sysinfo info;
//     char run_time[128];
 
//     if (sysinfo(&info)) 
//     {
//         printf("Failed to get sysinfo, errno:%u, reason:%s\n",errno, strerror(errno));
//         return -1;
//     }
 
//     long timenum=info.uptime;
//     int runday=timenum/86400;
//     int runhour=(timenum%86400)/3600;
//     int runmin=(timenum%3600)/60;
//     int runsec=timenum%60;
//     bzero(run_time, 128);
 
//     sprintf(run_time,"系统开机已运行：%d天%d时%d分%d秒",runday,runhour,runmin,runsec);
//     printf("--->%s\n",run_time);
//     return 0; 
// }