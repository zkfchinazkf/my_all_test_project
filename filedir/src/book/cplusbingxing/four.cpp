#include <iostream>
#include <mutex>
#include <chrono>
#include <thread>
#include <condition_variable>
#include <future>
#include <assert.h>

//mutable  关键字，可变的，当调用函数被const修饰时，使用mutable关键字后仍可改动该变量的值，如拷贝函数时对const变量的mutex进行上锁

std::condition_variable cond_data;
std::mutex mytux;
bool myflag=false;
std::promise<int> testpromise;

void thread_fun()
{
    std::cout<<"thread_fun start"<<std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    myflag = true;
    cond_data.notify_one();
    std::cout<<"notify_one wait start"<<std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout<<"notify_one end"<<std::endl;
}

int asyncfun()
{
    std::cout<<"asyncfun start"<<std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout<<"asyncfun end"<<std::endl;
    return 20;
}

void test_promise_fun()
{
    std::cout<<"test_promise_fun start"<<std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout<<"set_value start"<<std::endl;
    testpromise.set_value(30);       //将testpromise输出为30
    // testpromise.set_value_at_thread_exit(30); //在当前线程退出时将testpromise输出为30
    // testpromise.set_exception(std::current_exception());       //抛出异常
    // testpromise.set_exception_at_thread_exit(std::current_exception());       //在当前线程退出时抛出异常
    std::cout<<"set_value end"<<std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout<<"test_promise_fun end"<<std::endl;
}

int main(int argc,char **argv)
{
    // std::thread mythread(thread_fun);
    // std::unique_lock<std::mutex> lk(mytux);
    // cond_data.wait(lk,[]()->bool{    //构建后会执行一次，后续的话则需要notify_one触发,执行函数期间上锁
    //         std::cout<<"wait end"<<std::endl;
    //         return myflag;
    //     }
    // );
    // mythread.join();

    // std::future<int> myfuture = std::async(std::launch::deferred,asyncfun); //std::launch::deferred：调用wait或者get的时候才执行函数    std::launch::async：表明函数必须在其所在的独立线程上执行
    // std::cout<<"deferred start"<<std::endl;
    // std::this_thread::sleep_for(std::chrono::seconds(1));
    // std::cout<<"deferred end"<<std::endl;
    // std::cout<<"myfuture.get = "<<myfuture.get()<<std::endl;


    // std::cout<<"packaged_task start "<<std::endl;
    // std::packaged_task<int()> task(asyncfun);
    // std::this_thread::sleep_for(std::chrono::seconds(1));
    // std::cout<<"task start "<<std::endl;
    // std::shared_future<int> runfu_share =  task.get_future().share();     //只将结果返回给runfu，即runfu可通过get在阻塞情况下获取到返回，但不会主动启动线程
    // std::future<int> runfu =  task.get_future();     //只将结果返回给runfu，即runfu可通过get在阻塞情况下获取到返回，但不会主动启动线程
    // std::thread testrunfuthread(std::move(task));

    // std::cout<<"runfu_share.get = " << runfu_share.get()<<std::endl;    //对于 std::future只能调用一次，二次调用会报错   对于 std::shared_future，可多次调用得到返回值  
    // std::cout<<"runfu_share.get = " << runfu_share.get()<<std::endl;    //如果 runfu 为 std::future ，则此处会抛出异常
    // assert(runfu_share.valid());
    // assert(runfu.valid());
    // runfu.get();
    // // assert(runfu.valid());   //此时将抛出异常
    // testrunfuthread.join();


    std::future<int> promisefuture = testpromise.get_future();
    std::thread  mytest_promise_thread(test_promise_fun);
    std::cout<<"promisefuture.get="<<promisefuture.get()<<std::endl;
    mytest_promise_thread.join();
    return 0;
}