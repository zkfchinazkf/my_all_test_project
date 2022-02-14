#include <iostream>
#include <chrono>



int main(int argc,char **argv)
{
    auto mindata = std::chrono::steady_clock::time_point::min();
    auto maxdara = std::chrono::steady_clock::time_point::max();
    auto nowdata = std::chrono::steady_clock::now();
    
    std::cout<<mindata.time_since_epoch().count()<<std::endl;  //get min time,-9223372036854775808,can use in init or clear data
    std::cout<<maxdara.time_since_epoch().count()<<std::endl;  //get max time,9223372036854775807,can use in init or clear data
    std::cout<<nowdata.time_since_epoch().count()<<std::endl;
}