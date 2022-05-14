#include <iostream>
#include <thread>

void thread_fun(void)
{
    std::cout<<"helloworld"<<std::endl;
}

int main(int argc,char **argv)
{
    std::thread t(thread_fun);
    t.join();
    return 0;
}