#include <iostream>
#include <stdio.h>
#include <unistd.h>


int main(int argc,char **argv)
{
    std::cout<<"datetest1\rcc2"<<std::endl;
    printf("datetest1\rcc2\n");
    std::cout<<"datetest1\b3"<<std::endl;
    printf("datetest1\b3\n"); 
    std::cout<<"hello\n\033[1Awo"<<std::endl;
    printf("hello\n\033[1Awo\n");
    return 0;
}