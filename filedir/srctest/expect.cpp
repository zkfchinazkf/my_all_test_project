#include <iostream>
#include <string>


int main(int argc,char **argv)
{
    int i=0;
    while(1)
    {
        std::string inpu;
        std::cout<<"please input:";
        std::cin>>inpu;
        if(inpu != "end")
        {
            std::cout<<"input is "<<inpu <<std::endl;
            i++;
        }
        else 
        {
            std::cout<<"myexpect end"<<std::endl;
            break;
        }
    }


    
    return 0;
}
