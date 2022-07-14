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

对写入施加 release 语义（store），在代码中这条语句前面的所有读写操作都无法被重排到这个操作之后   unlock
对读取施加 acquire 语义（load），在代码中这条语句后面所有读写操作都无法重排到这个操作之前        lock

release 和 acquire 是为了确保前后变量的先行关系   本身该原子变量的操作依然为原子的


实现  更新到最新值后将设定值赋值给当前值
bool compare_exchange_weak( T& expected, T desired,
                        std::memory_order success,
                        std::memory_order failure );
bool compare_exchange_strong( T& expected, T desired,
                            std::memory_order success,
                            std::memory_order failure );
当前值与期望值(expect)相等时，修改当前值为设定值(desired)，返回true
当前值与期望值(expect)不等时，将期望值(expect)修改为当前值，返回false


如果值的计算很容易存储，那么使用compare_exchange_weak()能更好的避免一个双重循环的执行，即使compare_exchange_weak()可能会“伪失败”(因此compare_exchange_strong()包含一个循环)。
另一方面，如果值计算的存储本身是耗时的，那么当期望值不变时，使用compare_exchange_strong()可以避免对值的重复计算


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
    while(myintatomic.compare_exchange_strong(expect,3,std::memory_order_acq_rel) != 0);   //compare_exchange_strong 第一个参数为引用，必须传入变量

    std::cout<<"expect="<<expect<<std::endl;
    std::cout<<"myintatomic.load="<<myintatomic.load(std::memory_order_acquire)<<std::endl;
    myintatomicthread.join();
    return 0;
}