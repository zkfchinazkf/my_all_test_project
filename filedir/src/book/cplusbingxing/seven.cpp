#include <iostream>
#include <atomic>
#include <memory>
#include <thread>
#include <list>

/*
compare_exchange_weak  实现  更新到最新值后将设定值赋值给当前值
当前值与期望值(expect)相等时，修改当前值为设定值(desired)，返回true
当前值与期望值(expect)不等时，将期望值(expect)修改为当前值，返回false
第三第四参数为可选参数，分别为成功时使用的内存序和失败时使用的内存序

compare_exchange_strong 同compare_exchange_weak相同


release 和 acquire 是为了确保前后变量的先行关系   本身该原子变量的操作依然为原子的

分离引用计数:
    分为内部和外部计数   
    外部计数：获取到当前对象的时候加1，消除该对象的时候减一，若成功移除链表，则减二
    内部计数：用以在外部计数对象被挪动时使用
    外部计数加内部计数为0时，则消除对象
*/

template<typename T>
class mystack
{
    struct node
    {
        std::shared_ptr<T> data;  // 1 指针获取数据
        node* next = nullptr;

        node(T const& data_):
            data(std::make_shared<T>(data_))  // 2 让std::shared_ptr指向新分配出来的T
        {
            next = nullptr;
        }
    };

    std::atomic<int>    threads_in_pop;
    std::atomic<node*>  head;

    std::atomic<node*>  need_clear_list;
    
    void clear_all_need_list(node *clear_node)
    {
        while(clear_node)
        {
            node *temp_node = clear_node ->next;
            delete clear_node;
            clear_node = temp_node;
        }
    }

    void chain_pending_nodes(node *nodes)
    {
        if(!nodes)
        {
            return;
        }
        node *last = nodes;
        while(node *next = last->next)
        {
            std::cout<<"find next"<<std::endl;
            last = next;
        }
        chain_pending_nodes(nodes,last);
    }

    void chain_pending_nodes(node *first,node *last)
    {
        last->next = need_clear_list.load();
        while(!need_clear_list.compare_exchange_weak(last->next,first));
    }

    void chain_pending_node(node *nodes)
    {
        chain_pending_nodes(nodes,nodes);
    }

    void clear_pop_threads(node *clear_data)
    {
        if(threads_in_pop == 1)
        {
            node *old_head = need_clear_list.exchange(nullptr);
            if(threads_in_pop.fetch_sub(1) == 1)
            {
                clear_all_need_list(old_head);
            }
            else 
            {
                chain_pending_nodes(old_head);
            }
            delete clear_data;
        }
        else 
        {
            // clear_data ->next = nullptr;
            // chain_pending_nodes(clear_data);            //理论上与上方的一致
            chain_pending_node(clear_data);

            threads_in_pop.fetch_sub(1);
            std::cout<<"threads_in_pop != 1"<<std::endl;
        }
    }

    public:
        mystack()
        {
            threads_in_pop = 0;
            need_clear_list.store(nullptr);
            std::cout<<"create id:"<<std::this_thread::get_id()<<std::endl;
            head.store(nullptr);   //如果不设置初值，head将不一定为nullptr
        }
        ~mystack()
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
            threads_in_pop.fetch_add(1);
            node *oldnode = head.load();
            while( oldnode && !head.compare_exchange_weak(oldnode,oldnode->next,std::memory_order_release,std::memory_order_relaxed));

            std::shared_ptr<T> res;
            if(oldnode)
            {
                res.swap(oldnode->data);
                // delete oldnode;   //将会导致coredump
            }
            
            clear_pop_threads(oldnode);

            return res;
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


struct testdemo
{
    public:
        int data ;
        testdemo& operator=(testdemo &input_data)
        {
            std::cout<<"use operator "<<std::endl;
            return *this;
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

    testdemo dd;
    dd.data = 100;
    testdemo aa ;
    aa = dd;

    mystack<int> mytestlist;
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
    std::thread mytestlistfun1(
        [&]{
            while(1)
            {
                mytestlist.push(32);
                mytestlist.pop();
            }
        }
    );
    std::thread mytestlistfun2(
        [&]{
            while(1)
            {
                mytestlist.push(44);
                mytestlist.pop();
            }
        }
    );
    mytestlistfun1.join();
    mytestlistfun2.join();
    // std::shared_ptr<int> data = nullptr;
    // std::shared_ptr<int> newdata = std::make_shared<int>(100);
    // if(data==nullptr)
    // {
    //     std::cout<<"data==nullptr"<<std::endl;
    // }
    // else
    // {
    //     std::cout<<"data="<<*data<<std::endl;
    // }
    // if(newdata==nullptr)
    // {
    //     std::cout<<"newdata==nullptr"<<std::endl;
    // }
    // else
    // {
    //     std::cout<<"newdata="<<*newdata<<std::endl;
    // }

    // data.swap(newdata);
    // std::cout<<"swap :"<<std::endl;
    // if(data==nullptr)
    // {
    //     std::cout<<"data==nullptr"<<std::endl;
    // }
    // else
    // {
    //     std::cout<<"data="<<*data<<std::endl;
    // }
    // if(newdata==nullptr)
    // {
    //     std::cout<<"newdata==nullptr"<<std::endl;
    // }
    // else
    // {
    //     std::cout<<"newdata="<<*newdata<<std::endl;
    // }

    // std::thread  mythread1(test_thread_local_fun);
    // std::thread  mythread2(test_thread_local_fun);
    // mythread1.join();
    // mythread2.join();

    // std::thread::id normalid;
    // std::cout<<"id="<<std::thread::id()<<",this id "<<std::this_thread::get_id()<<",normal is ="<<normalid<<std::endl;


    // funclass mytestfun;
    // mytestfun.use_fun();

    return 0;
}