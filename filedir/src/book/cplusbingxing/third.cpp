#include <iostream>
#include <thread>
#include <mutex>


void testfun()
{
    std::mutex mylock;
    std::lock_guard<std::mutex> mytestguard(mylock);
}

int main(int agrc,char **argv)
{
    std::mutex mylock,mylock2;
    std::thread  testpth(testfun);
    testpth.join();
    {
        // std::lock_guard<std::mutex> myguardlock(mylock); //创建的时候自动上锁，不允许解锁再上锁
        std::unique_lock<std::mutex> myguardlock(mylock);   //创建的时候自动上锁，允许解锁再上锁
        myguardlock.unlock();
        myguardlock.lock();
        myguardlock.unlock();
        std::unique_lock<std::mutex> myguardlockc(mylock);
        std::cout<<"myguardlock"<<std::endl;
    }
    {
        std::lock_guard<std::mutex> myguardlock1(mylock);
        std::cout<<"myguardlock1"<<std::endl;
    }
        // std::lock(mylock,mylock2);  //同时锁两个锁


    return 0;
}