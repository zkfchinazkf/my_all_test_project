#include <iostream>
#include <thread>
#include <memory>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <functional>

#define  question_1   

#ifdef question_1
int mydatause = 100;
#endif

class backgroud_task
{
    int &m_data;
    public:
        backgroud_task(int &data):m_data(data){}
        void operator()() const          //only use this way can success,use other fun will fault,like use new fun name dddd and f.dddd
        {
            int ddd=100;
            while(1)
            {
                if(m_data != 100)
                {
                    std::cout<<"backgroud_task data="<<m_data<<std::endl;
                    return;
                }
                if(ddd == 100)
                {
                    ddd = 101;
#ifdef question_1
                    mydatause = ddd;
#endif
                }
                else 
                {
                    ddd = 100;
#ifdef question_1
                    mydatause = ddd;
#endif
                }
                // std::cout<<"backgroud_task data="<<m_data<<std::endl;
            }
        }
};


void update_data_for_widget(int w,int& data) // 1
{
    data = 100;
}
void oops_again(int w)
{
    int data;
    std::thread t(update_data_for_widget,w,std::ref(data)); // 2 //std::ref(data) 将参数转换为引用的形式 若直接传递data，则编译报错
    t.join();
    std::cout<<"oops_again = "<<data<<std::endl; // 3
}

class X
{
    public:
        int m_data;
        X(int data):m_data(data){}
        void getclassfun()
        {
            std::cout<<"getclassfun data = "<<m_data<<std::endl;
        }
};


void threadmovefun()
{
    std::cout<<"threadmovefun"<<std::endl;
}


void do_work1()
{
    std::cout<<"do_work1 start"<<std::endl;
    sleep(1);
    std::cout<<"do_work1 end"  <<std::endl;
}
void do_work2()
{
    std::cout<<"do_work2 start"<<std::endl;
    sleep(2);
    std::cout<<"do_work2 end"  <<std::endl;
}
void f()
{
    // std::thread thread1(do_work1);
    // std::thread thread2(do_work2);
    // std::cout<<"f cc"  <<std::endl;
    // sleep(3);
    // std::cout<<"f ff"  <<std::endl;
    // thread1.join();
    // std::cout<<"f dd"  <<std::endl;
    // thread2.join();
    // std::cout<<"f end"  <<std::endl;

    std::vector<std::thread> threads;
    threads.push_back(std::thread(do_work1)); // 产生线程
    threads.push_back(std::thread(do_work2)); // 产生线程
    // std::for_each(threads.begin(),threads.end(),
    //             std::mem_fn(&std::thread::join)); // 对每个线程调用join()
    std::cout<<"f cc"  <<std::endl;
    sleep(3);
    std::cout<<"f ff"  <<std::endl;
    threads[0].join();
    std::cout<<"f dd"  <<std::endl;
    threads[1].join();
    std::cout<<"f end"  <<std::endl;

}

int main(int argc,char **argv)
{
    int data=100;
    std::shared_ptr<backgroud_task> f_point;
    std::cout<<"f_point use_count is ="<<f_point.use_count()<<std::endl;
    f_point = std::make_shared<backgroud_task>(data);
    std::cout<<"f_point use_count is ="<<f_point.use_count()<<std::endl;
    std::thread mythread(*f_point); //将函数对象拷贝到了新线程的存储空间，意味着后续对f_point的操作都不会直接影响到线程数据
    std::cout<<"f_point use_count is ="<<f_point.use_count()<<std::endl;
    std::cout<<"mythread.joinable() ="<<mythread.joinable()<<std::endl;  
    mythread.detach();    //will disable joinable
    std::cout<<"mythread.joinable() ="<<mythread.joinable()<<std::endl;  
    
    sleep(1);
    std::ref(data);   //将参数转换为引用的形式
    data = 200;
    sleep(1);
    oops_again(2);

    X myx(20);
    std::thread classthread1(&X::getclassfun,&myx);
    std::thread classthread2(&X::getclassfun,myx);
    myx.m_data=30;
    classthread1.join();
    classthread2.join();

    std::thread mymovethread1(threadmovefun);
    std::cout<<"mymovethread1 joinable = "<<mymovethread1.joinable()<<std::endl;
    std::thread mymovethread2 = std::move(mymovethread1);    //可移动性
    std::cout<<"mymovethread1 joinable = "<<mymovethread1.joinable()<<std::endl;
    std::cout<<"mymovethread2 joinable = "<<mymovethread2.joinable()<<std::endl;
    
    mymovethread2.join();

    f();

    std::cout<<std::this_thread::get_id()<<std::endl;

    return 0;
}