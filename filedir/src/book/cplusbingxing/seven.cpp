#include <iostream>
#include <atomic>


int main(int argc,char **argv)
{
    std::atomic_flag ato_flag;
    ato_flag.test_and_set(std::memory_order_acquire);
    ato_flag.clear(std::memory_order_release);
    return 0;
}