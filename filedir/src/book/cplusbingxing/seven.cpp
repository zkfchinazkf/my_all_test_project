#include <iostream>
#include <atomic>
#include <memory>
#include <thread>

/*
compare_exchange_weak  实现  更新到最新值后将设定值赋值给当前值
当前值与期望值(expect)相等时，修改当前值为设定值(desired)，返回true
当前值与期望值(expect)不等时，将期望值(expect)修改为当前值，返回false



分离引用计数:
    分为内部和外部计数   
    外部计数：获取到当前对象的时候加1，消除该对象的时候减一，若成功移除链表，则减二
    内部计数：用以在外部计数对象被挪动时使用
    外部计数加内部计数为0时，则消除对象
*/

template<typename T>
class mylist
{
    struct node
    {
        std::shared_ptr<T> data;  // 1 指针获取数据
        node* next;

        node(T const& data_):
            data(std::make_shared<T>(data_))  // 2 让std::shared_ptr指向新分配出来的T
        {
            next = nullptr;
        }
    };

    std::atomic<node*> head;
    public:
        mylist()
        {
            std::cout<<"create id:"<<std::this_thread::get_id()<<std::endl;
            head.store(nullptr);   //如果不设置初值，head将不一定为nullptr
        }
        ~mylist()
        {
            std::cout<<"~id:"<<std::this_thread::get_id()<<std::endl;
        }
        void push(T const new_data)
        {
            node *newnode = new node(new_data);
            newnode->next = head.load();
            while(!head.compare_exchange_weak(newnode->next,newnode));
        }

        std::shared_ptr<T> pop()
        {
            node *oldnode = head.load();
            while( oldnode && !head.compare_exchange_weak(oldnode,oldnode->next));
            return oldnode ? oldnode->data:nullptr;
        }
};


void test_thread_local_fun()
{
    thread_local static int mytestlocaldata = 100;
    static int mytestdata = 100;
    std::cout<<"mytestlocaldata = "<<mytestlocaldata<<std::endl;
    std::cout<<"mytestdata = "<<mytestdata<<std::endl;
    mytestlocaldata = 200;
    mytestdata = 200;
}

void myfuntion()
{
    std::cout<<"funclass"<<std::endl;
}

class funclass
{
    std::function<void(void)>      m_func;

    public:
        funclass():m_func(myfuntion)
        {}
        void use_fun()
        {
            m_func();
        }

};


int main(int argc,char **argv)
{
    std::shared_ptr<bool> some_shared_ptr;
    std::cout<<"shared_ptr bool free lock is "<<std::atomic_is_lock_free(&some_shared_ptr)<<std::endl;
    std::atomic<int> bool_atomic_ptr;
    std::cout<<"bool_atomic_ptr bool free lock is "<<std::atomic_is_lock_free(&bool_atomic_ptr)<<std::endl;
    std::atomic_flag ato_flag;
    ato_flag.test_and_set(std::memory_order_acquire);
    ato_flag.clear(std::memory_order_release);

    std::atomic<int> myint(3);
    int expect=2;
    while(!myint.compare_exchange_weak(expect,4)){}
    std::cout<<"expect="<<expect<<std::endl;
    std::cout<<"myint.load="<<myint.load()<<std::endl;

    mylist<int> mytestlist;
    mytestlist.push(100);
    mytestlist.push(200);
    std::shared_ptr<int> ret;
    if(ret = mytestlist.pop())
    {
        std::cout<<"1.mytestlist.pop="<<*ret<<std::endl;
    }
    if(ret = mytestlist.pop())
    {
        std::cout<<"2.mytestlist.pop="<<*ret<<std::endl;
    }
    if(ret = mytestlist.pop())
    {
        std::cout<<"3.mytestlist.pop="<<*ret<<std::endl;
    }

    std::shared_ptr<int> data = nullptr;
    std::shared_ptr<int> newdata = std::make_shared<int>(100);
    if(data==nullptr)
    {
        std::cout<<"data==nullptr"<<std::endl;
    }
    else
    {
        std::cout<<"data="<<*data<<std::endl;
    }
    if(newdata==nullptr)
    {
        std::cout<<"newdata==nullptr"<<std::endl;
    }
    else
    {
        std::cout<<"newdata="<<*newdata<<std::endl;
    }

    data.swap(newdata);
    std::cout<<"swap :"<<std::endl;
    if(data==nullptr)
    {
        std::cout<<"data==nullptr"<<std::endl;
    }
    else
    {
        std::cout<<"data="<<*data<<std::endl;
    }
    if(newdata==nullptr)
    {
        std::cout<<"newdata==nullptr"<<std::endl;
    }
    else
    {
        std::cout<<"newdata="<<*newdata<<std::endl;
    }

    std::thread  mythread1(test_thread_local_fun);
    std::thread  mythread2(test_thread_local_fun);
    mythread1.join();
    mythread2.join();

    std::thread::id normalid;
    std::cout<<"id="<<std::thread::id()<<",this id "<<std::this_thread::get_id()<<",normal is ="<<normalid<<std::endl;


    funclass mytestfun;
    mytestfun.use_fun();

    return 0;
}