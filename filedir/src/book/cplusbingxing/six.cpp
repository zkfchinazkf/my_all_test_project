#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <memory>

int main(int argc,char **argv)
{
    std::mutex  mytux;
    int num=0;
    std::condition_variable cond_data;
    std::unique_lock<std::mutex> lk(mytux);
    std::thread mythread(
        [&]{
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            num=10;
            cond_data.notify_one();
        }
    );
    // lk.lock();
    std::cout<<"start lock"<<std::endl;
    cond_data.wait(lk,[&]{return num;});
    mythread.join();

    std::unique_ptr<int> uniptrint(new int(500));
    std::cout<<"uniptrint.get=" << uniptrint.get()<<std::endl;
    return 0;
}