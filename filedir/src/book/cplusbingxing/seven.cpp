#include <iostream>
#include <atomic>
#include <memory>
#include <thread>
#include <list>
#include <functional>

/*
编译报错   undefined reference to `__atomic_compare_exchange_16'   需要在编译时加入库  libatomic.so


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


#define THREAD_RISK_POINTER_MAX     100
struct risk_pointer_data
{  
    std::atomic<std::thread::id>   thread_id;
    std::atomic<void *>            risk_pointer;
};
risk_pointer_data  risk_pointer_buff[THREAD_RISK_POINTER_MAX];

class thread_risk_pointer
{
    risk_pointer_data  *hp;
    public:
        thread_risk_pointer(thread_risk_pointer const &) = delete;
        thread_risk_pointer operator=(thread_risk_pointer const &) = delete;
        thread_risk_pointer():hp(nullptr)
        {
            int idx=0;
            for(idx = 0;idx<THREAD_RISK_POINTER_MAX;idx++)
            {
                std::thread::id  old_pid; 
                if(risk_pointer_buff[idx].thread_id.compare_exchange_strong(old_pid,std::this_thread::get_id()))
                {
                    hp = &risk_pointer_buff[idx];
                    break;
                }
            }
            if(idx == THREAD_RISK_POINTER_MAX)
            {
                throw "get_thread_risk_pointer fail";
            }
        }
        ~thread_risk_pointer()
        {
            hp->risk_pointer.store(nullptr);
            hp->thread_id.store(std::thread::id());
        }
        std::atomic<void*>& get_point()
        {
            return hp->risk_pointer;
        }
};


std::atomic<void *> &get_thread_risk_pointer()
{
    thread_local static thread_risk_pointer ret_risk_pointer;
    return ret_risk_pointer.get_point();
}

bool get_risk_pointer_usage(void * pointer)
{
    for(int idx=0;idx<THREAD_RISK_POINTER_MAX;idx++)
    {
        if(risk_pointer_buff[idx].risk_pointer.load() == pointer)    //只在清除待删除链表时使用，即只需判断一次，因为其他线程不能在已弹出且弹出后检查无使用该值后再次指向它
        {
            return true;
        }
    }
    return false;
}

template <typename _T>
void to_delete(void *p)
{
    delete static_cast<_T *>(p);
}

struct data_to_reclaim
{
    void*        to_delete_point;
    std::function<void(void *)> deleter_fun;
    data_to_reclaim *next = nullptr;
    template <typename _T>
    data_to_reclaim(_T delete_point,data_to_reclaim *nex):
    deleter_fun(to_delete<_T>),
    to_delete_point(delete_point),
    next(nex)
    {}
    ~data_to_reclaim()
    {
        deleter_fun(to_delete_point);
    }
};

std::atomic<data_to_reclaim *>   nodes_to_reclaim;

template <typename _T>
void add_to_delete_reclaim(_T *p)
{
    data_to_reclaim *new_point = new data_to_reclaim(p,nodes_to_reclaim.load());
    while(!nodes_to_reclaim.compare_exchange_weak(new_point->next,new_point));
}

void clear_all_to_delete_reclaim()
{
    data_to_reclaim *clear_head = nodes_to_reclaim.exchange(nullptr);
    while(clear_head)
    {
        data_to_reclaim *temp_head = clear_head->next;
        if(!get_risk_pointer_usage(clear_head->to_delete_point))
        {
            delete clear_head;
            std::cout<<"get_risk_pointer_usage fail ,clear "<<std::endl;
        }
        clear_head = temp_head;
    }
}

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

        //引用计数机制实现无锁栈
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

        //风险指针法实现无锁栈
        std::shared_ptr<T> risk_pointer_pop()
        {
            std::atomic<void *>& thread_hp = get_thread_risk_pointer();
            node *oldnode = head.load();
            node *temp;
            do
            {
                temp = oldnode;
                thread_hp.store(oldnode);
                oldnode = head.load();
            }while(temp != oldnode);

            while( oldnode && !head.compare_exchange_weak(oldnode,oldnode->next,std::memory_order_release,std::memory_order_relaxed));

            thread_hp.store(nullptr);


            std::shared_ptr<T> res;
            if(oldnode)
            {
                res.swap(oldnode->data);
                if(get_risk_pointer_usage(oldnode))
                {
                    add_to_delete_reclaim(oldnode);
                    std::cout<<"add_to_delete_reclaim "<<std::endl;
                }
                else 
                {
                    delete oldnode;
                }
                clear_all_to_delete_reclaim();
            }
            return res;
        }
};


//检测使用引用计数法实现无锁栈
template <typename _T>
struct ext_counter_stack
{
    struct node;
    struct ext_node
    {
        int  ext_number;
        node *ptr = nullptr;
    };
    struct node
    {
        std::shared_ptr<_T>  m_data;
        std::atomic<int>     int_number;
        ext_node             next;
        node(_T data):m_data(std::make_shared<_T>(data)),int_number(0)
        {}
    };
    std::atomic<ext_node>  head;


    void push(_T data)
    {
        ext_node newnode;
        newnode.ptr = new node(data);
        newnode.ptr->next = head.load();
        newnode.ext_number = 1;
        while(!head.compare_exchange_weak(newnode.ptr->next,newnode,std::memory_order_release,std::memory_order_relaxed));
    }

    void increase_head_count(ext_node &oldhead)
    {
        ext_node temphead;
        do
        {
            temphead = oldhead;
            temphead.ext_number++;
        } while (!head.compare_exchange_weak(oldhead,temphead,std::memory_order_acquire,std::memory_order_relaxed));
        oldhead.ext_number = temphead.ext_number;
    }

    std::shared_ptr<_T>  pop()
    {
        ext_node old_head = head.load();
        while(1)
        {
            increase_head_count(old_head);
            if(!old_head.ptr)
            {
                return nullptr;
            }
            if(head.compare_exchange_strong(old_head,old_head.ptr->next,std::memory_order_relaxed))   //head ！= oldhead  说明要么外部计数改变，要么有新的数据压入，要么有数据弹出
            {
                std::shared_ptr<_T> res;
                res.swap(old_head.ptr->m_data);
                old_head.ext_number-=2;
                if(old_head.ptr->int_number.fetch_add(old_head.ext_number,std::memory_order_release) == -old_head.ext_number)
                {
                    // std::cout<<"fetch_add delete "<<std::endl;
                    delete old_head.ptr;
                }
                return res;
            }
            else if(old_head.ptr->int_number.fetch_sub(1) == 1)
            {
                old_head.ptr->int_number.load(std::memory_order_acquire);//触发该条件时由于使用的是memory_order_relaxed， 
                                                                        //res.swap(old_head.ptr->data) 未必执行完成，故需要该操作，以同步对ptr的操作

                delete old_head.ptr;
                // std::cout<<"fetch_sub delete "<<std::endl;
            }
        }
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

    // mystack<int> mytestlist;
    ext_counter_stack<int> mytestlist;
    
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
                // mytestlist.risk_pointer_pop();
            }
        }
    );
    std::thread mytestlistfun2(
        [&]{
            while(1)
            {
                mytestlist.push(44);
                mytestlist.pop();
                // mytestlist.risk_pointer_pop();
            }
        }
    );
    std::thread mytestlistfun3(
        [&]{
            while(1)
            {
                mytestlist.push(55);
                mytestlist.pop();
                // mytestlist.risk_pointer_pop();
            }
        }
    );
    mytestlistfun1.join();
    mytestlistfun2.join();
    mytestlistfun3.join();


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