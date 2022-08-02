#include <iostream>
#include <list>
#include <future>
#include <thread>
#include <chrono>

 

int main(int argc,char **argv)
{
    std::promise<int>  myprodata; 

    std::thread  mythead([](std::promise<int> &myprodatauses){
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        myprodatauses.set_value(1000);
        std::cout<<"mythead success end"<<std::endl;
    },std::ref(myprodata));   //模板传参的时候使用ref，否则传参失败
    std::future<int> myfuture = myprodata.get_future();

    while(myfuture.get() != 1000);
    std::cout<<"myfuture.get() == 1000"<<std::endl;

    mythead.join();

    std::future<int>  newfuture = std::async(std::launch::deferred,[&]()->int{
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout<<"async end"<<std::endl;
        return 100;
    });   //std::launch::async    异步执行    不需要等到调用get就已执行
          //std::launch::deferred 同步执行    等到调用get再执行
        std::cout<<"newfuture start"<<std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    newfuture.get();
    std::cout<<"newfuture end"<<std::endl;

    return 0;
}