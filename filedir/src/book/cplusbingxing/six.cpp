#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <memory>
#include <functional>
#include <list>

template <typename _T>
class threadsafelist
{
    private:
    struct node
    {
        std::mutex  nodemut;
        std::shared_ptr<_T>   data;
        std::unique_ptr<node> next;
        node():next(nullptr){}
        node(_T value):next(nullptr)
        {
            data = std::make_shared<_T>(value);
        }
        ~node()
        {
            std::cout<<"relese node"<<std::endl;
        }
    };

    node head;

    public:
    threadsafelist()
    {}
    ~threadsafelist()
    {}
    
    void push_front(_T value)
    {
        std::unique_ptr<node> newnode(new node(value));
        std::lock_guard<std::mutex> lk(head.nodemut);
        newnode->next = std::move(head.next);
        head.next = std::move(newnode);
    }

    template<typename func>
    void for_each(func f)
    {
        node *pnext = &head;
        std::unique_lock<std::mutex> lk(head.nodemut);
        while(node *next =  (pnext->next).get())
        {
            std::unique_lock<std::mutex> nextlk(next->nodemut);
            lk.unlock();
            f(*(next->data));
            pnext = next;
            lk = std::move(nextlk);
        }
    }

    template<typename Predicate>
    std::shared_ptr<_T> find_first_if(Predicate p)
    {
        node *pnext = &head;
        std::unique_lock<std::mutex> lk(head.nodemut);
        while(node *next = (pnext->next).get())
        {
            std::unique_lock<std::mutex> nextlk(next->nodemut);
            lk.unlock();
            if(p(*(next->data)))
            {
                return next->data;
            }
            pnext = next;
            lk = std::move(nextlk);
        }
        return nullptr;
    }
    
    template<typename Predicate>
    void remove_if(Predicate p)
    {
        node *pnext = &head;
        std::unique_lock<std::mutex> lk(head.nodemut);
        while(node *next = (pnext->next).get())
        {
            std::unique_lock<std::mutex> nextlk(next->nodemut);
            if(p(*(next->data)))
            {
                // std::unique_ptr<node> oldnext = std::move(pnext->next);   //清空空间所有权，当oldnext被释放后原本的空间就被释放  (不添加效果也是一样)
                pnext->next =  std::move(next->next);
            }
            else 
            {
                pnext = next;
                lk = std::move(nextlk);
            }
        }
    }


    void getpri()
    {
        node *uniptr = &head;
        while(uniptr->next)
        {
            uniptr = (uniptr->next).get();
            std::cout<<"list data = "<<*(uniptr->data)<<std::endl;
        }
    }

};


int main(int argc,char **argv)
{
    // std::mutex  mytux;
    // int num=0;
    // std::condition_variable cond_data;
    // std::unique_lock<std::mutex> lk(mytux);
    // std::thread mythread(
    //     [&]{
    //         std::this_thread::sleep_for(std::chrono::milliseconds(500));
    //         num=10;
    //         cond_data.notify_one();
    //     }
    // );
    // // lk.lock();
    // std::cout<<"start lock"<<std::endl;
    // cond_data.wait(lk,[&]{return num;});
    // mythread.join();

    // std::unique_ptr<int> uniptrint(new int(500));
    // int *getuni = uniptrint.get();
    // *getuni = 1000;
    // std::cout<<"*uniptrint=" << *uniptrint<<std::endl;

    // std::hash<std::string> myhash;
    // std::cout<<"hash = "<<myhash("helloworld")<<std::endl;
    

    // std::pair<std::string,int> mymappiar;
    // mymappiar.first = "hello";
    // mymappiar.second = 1000;
    // std::cout<<"mymappiar.first=" << mymappiar.first<<std::endl;
    // std::cout<<"mymappiar.second=" << mymappiar.second<<std::endl;
    // std::list<int>  mytestlist;
    // mytestlist.push_back(30);
    // mytestlist.push_back(40);
    // std::list<int>::iterator listiter = mytestlist.begin();
    // std::cout<<"*listiter=" << *listiter <<std::endl;

    std::thread mythread(
        []{
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    );
    std::mutex myuniloc1;
    std::mutex myuniloc2;
    {
        std::unique_lock<std::mutex> mylk1(myuniloc1);
        std::unique_lock<std::mutex> mylk2 = std::move(mylk1);
        std::unique_lock<std::mutex> mylk3(myuniloc2);
        mylk2 = std::move(mylk3);
        std::unique_lock<std::mutex> mylk4(myuniloc1);
    }


    threadsafelist<int> mylist;
    mylist.push_front(30);
    mylist.push_front(4);
    mylist.push_front(2);
    mylist.push_front(1);
    mylist.for_each(
        [](int num){
            std::cout<<"num="<<num<<std::endl;
        }
    );
    std::shared_ptr<int >ret = mylist.find_first_if(
        [](int num){
            return num>4;
        }
    );
    if(ret)
        std::cout<<"find_first_if ret= "<<*ret<<std::endl;

    mylist.remove_if(
        [](int num){
            return num<3;
        }
    );

    mylist.getpri();
    mythread.join();
    return 0;
}