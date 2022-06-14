#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>
#include <memory>

int main(int argc,char **argv)
{
    std::atomic_flag f = ATOMIC_FLAG_INIT;
    f.test_and_set(std::memory_order_acquire);
    std::thread mytestthread(
        [&]{
            std::cout<<"mytestthread run"<<std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            f.clear(std::memory_order_release);   //clear 解锁
            std::cout<<"mytestthread end"<<std::endl;
        }
    );
    while(f.test_and_set(std::memory_order_acquire));   //test_and_set 试图上锁
    std::cout<<"memory_order_acquire success"<<std::endl;
    mytestthread.join();

    std::atomic<bool> myboolatomic;
    std::cout<<"myboolatomic="<<myboolatomic.load()<<std::endl;
    myboolatomic.store(false);
    std::cout<<"myboolatomic="<<myboolatomic.load()<<std::endl;
    bool exchangeret = myboolatomic.exchange(true, std::memory_order_acq_rel); //读旧值 改旧制为新值
    std::cout<<"exchangeret="<<exchangeret<<std::endl;
    std::cout<<"myboolatomic="<<myboolatomic.load()<<std::endl;
    std::cout<<"myboolatomic.is_lock_free="<<myboolatomic.is_lock_free()<<std::endl;

    std::shared_ptr<int>  myptr;
    std::shared_ptr<int>  myptrnew;
    myptr = std::make_shared<int>(100);
    myptrnew = std::atomic_load(&myptr);
    myptr = std::make_shared<int>(30);
    std::atomic_store(&myptrnew,myptr);      //修改值
    std::cout<<"*myptrnew="<<*std::atomic_load(&myptrnew)<<std::endl;  //获取值
    return 0;
}