#include <iostream>
#include <atomic>
#include <memory>

/*
compare_exchange_weak  实现  更新到最新值后将设定值赋值给当前值
当前值与期望值(expect)相等时，修改当前值为设定值(desired)，返回true
当前值与期望值(expect)不等时，将期望值(expect)修改为当前值，返回false
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
            head.store(nullptr);   //如果不设置初值，head将不一定为nullptr
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


int main(int argc,char **argv)
{
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

    return 0;
}