#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>
#include <memory>

/*
memory_order_seq_cst  排序一致模型
memory_order_acquire  获取(用于读)   memory_order_release   释放（用于写）   memory_order_acq_rel 两者兼有的操作（用于读改写）       获取-释放序列   若获取的值不是最后一个值
memory_order_relaxed  自由序列  读取到的值不一定是最后一个值，不建议使用
memory_order_consume  数据相关性   使用该内存模式获取的数据是数据相关的，保证指针指向的值是已同步的，并且不需要对其他任何非独立数据施加任何同步要求
*/

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

    std::atomic<bool> myatomic(false);
    std::thread myatomicthread(
        [&]{
            std::this_thread::sleep_for(std::chrono::seconds(2));
            myatomic.store(true);
        }
    );

    while(myatomic.load() == false)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    std::cout<<"myatomic load == true"<<std::endl;
    myatomicthread.join();


    std::atomic<int> myintatomic(0);

    std::thread myintatomicthread(
        [&]{
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            myintatomic.store(1,std::memory_order_release);
            std::cout<<"myintatomic.store"<<std::endl;
        }
    );
    int expect=1;
    while(myintatomic.compare_exchange_strong(expect,2,std::memory_order_acq_rel) != 0);   //compare_exchange_strong 第一个参数为引用，必须传入变量

    std::cout<<"myintatomic.load="<<myintatomic.load(std::memory_order_acquire)<<std::endl;
    myintatomicthread.join();
    return 0;
}