#include <iostream>
#include <boost/algorithm/string.hpp>
#include <string>
#include <vector>
#include <boost/format.hpp>


int main(int argc,char **argv)
{
    std::vector<std::string> arg_tokens;
    std::string   usestring = "111,222,,,333,4444";
    /* use  token_compress_on will ignore sequential delimiters*/
    std::cout << "use token_compress_on "<<std::endl;
    boost::split(arg_tokens,usestring,boost::is_any_of(","),boost::token_compress_on);
    for(auto iter:arg_tokens)
    {
        std::cout << "iter = "<<iter<<std::endl;
    }
    /* use  token_compress_of will identify sequential delimiters*/
    std::cout << "use token_compress_off "<<std::endl;
    boost::split(arg_tokens,usestring,boost::is_any_of(","),boost::token_compress_off);   
    for(auto iter:arg_tokens)
    {
        std::cout << "iter = "<<iter<<std::endl;
    }


    std::cout << "coutnum 07.2f  is " << boost::str(boost::format("%07.2f") % 1000.321) << std::endl;   
    std::cout << "coutnum 08.2f  is " << boost::str(boost::format("%08.2f") % 1000.321) << std::endl;   
    std::cout << "coutnum 010d   is " << boost::str(boost::format("%010d") % 1000) << std::endl;    


    return 0;
}