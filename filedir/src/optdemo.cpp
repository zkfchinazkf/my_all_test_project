#include <iostream>
#include <getopt.h>


int main(int argc,char **argv)
{
    int pret ;
    struct option opts[] = {
        {"version",required_argument,NULL,'v'},
        {"help",no_argument,NULL,'h'},
        {0,0,0,0}
    };
    int ret=0;
    std::cout<<"opterr="<<opterr<<std::endl;
    while((ret = getopt_long(argc,argv,"v:h",opts,NULL)) != -1)
    {
        std::cout<<"ret = "<<(char )ret<<std::endl;
        switch (ret)
        {
            case 'v':
                std::cout<<"get version,"<<"optarg = "<<(char *)optarg<<std::endl;
                break;
            case 'h':
                std::cout<<"get help"<<std::endl;
                break;
            case '?':
                std::cout<<"unknow input"<<std::endl;
                break;
            default:
                std::cout<<"input data is unknow"<<std::endl;
                break;
        }
    }
//      std::string inpu;
//     std::cout<<"please input x:";
//     std::cin>>inpu;
    
//     int data =0;
//     if("datastr"==inpu)
//     {
// #define XXX   100
//         data = XXX;
//     }
//     else 
//     {
// #define XXX   200 
//         data = XXX;
//     }

    // std::cout <<data<<std::endl;
    // std::cout <<XXX<<std::endl;

    return 0;
}