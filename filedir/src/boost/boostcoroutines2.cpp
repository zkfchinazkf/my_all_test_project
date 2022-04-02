#include <iostream>
#include <vector>
#include <boost/coroutine2/coroutine.hpp>
#include <functional>
#include <memory>

using namespace std;


/*
协程中，会保留上次返回的位置，在重新调用的时候继续上次的执行，类似线程中的

对于coro_t::pull_type source来说   形参为输出到外部的值，调用 source.get()为获取到该协程最后一次的输出，调用source()为重新执行该协程直到协程中调用sink(xxx)为止  注,会在定义完该变量后自动执行一次协程
对于coro_t::push_type sink来说     形参为输入到内部的值，调用 sink(xxx)为输入到该协程xxx数据，且输入数据后自动进入协程中，在协程中，可以通过source.get()获取到输入的数据，在协程中调用source()则退出等待下一次进入


*/

class mytestcoroutines2
{
    typedef boost::coroutines2::coroutine<int> coro_t;
    int m_mydata;
    public:
        std::shared_ptr<coro_t::pull_type> source;
        std::shared_ptr<coro_t::push_type> sink;
        mytestcoroutines2()
        {
            m_mydata = 0;
            source = std::make_shared<coro_t::pull_type>(
                [&](coro_t::push_type& sink){
                    while(1)
                    {
    cout <<"jjj"<< endl;
                        sink(m_mydata);
                    }
                });
            sink = std::make_shared<coro_t::push_type>(
                [&](coro_t::pull_type& source){
                    while(1)
                    {
    cout <<"xxx"<< endl;
                        m_mydata = source.get();
                        source();
                    }
                });
        };
};



int main()
{
    typedef boost::coroutines2::coroutine<int> coro_t;
    coro_t::pull_type source(
        [&](coro_t::push_type& sink){
            std::cout<<"pull_type 1"<<std::endl;
            sink(1);
            std::cout<<"pull_type 2"<<std::endl;
            sink(2);
            std::cout<<"pull_type 3"<<std::endl;
            sink(3);
            std::cout<<"pull_type 4"<<std::endl;
            sink(4);
        }
    );

    coro_t::push_type sink(
        [&](coro_t::pull_type& source){
            std::cout<< "pushtype1 is "<< source.get() <<std::endl;
            source();
            std::cout<< "pushtype2 is "<< source.get() <<std::endl;
            source();
            std::cout<< "pushtype3 is "<< source.get() <<std::endl;
            source();
            std::cout<< "pushtype4 is "<< source.get() <<std::endl;
            source();
        }
    );
    /*  push_type test  */
    sink(100);
    sink(200);
    sink(300);
    sink(400);
    /*  pull_type test  */
    std::cout<<"start"<<std::endl;
    std::cout<<"test1"<<std::endl;
    std::cout<<"tset1 get is "<<source.get()<<std::endl;
    source();
    std::cout<<"test2"<<std::endl;
    std::cout<<"tset2 get is "<<source.get()<<std::endl;
    source();
    std::cout<<"test3"<<std::endl;
    std::cout<<"tset3 get is "<<source.get()<<std::endl;
    source();
    std::cout<<"test4"<<std::endl;
    std::cout<<"tset4 get is "<<source.get()<<std::endl;
    source();

    std::cout<<"test5"<<std::endl;
    std::cout<<"test5 get is "<<source.get()<<std::endl;
    source();
    std::cout<<"end"<<std::endl;


    mytestcoroutines2 testdata;
    (*testdata.sink)(100);
    (*testdata.source)();
    std::cout << "testdata.sink "<<(*testdata.source).get()<<std::endl;
    (*testdata.sink)(200);
    (*testdata.source)();
    std::cout << "testdata.sink "<<(*testdata.source).get()<<std::endl;

}