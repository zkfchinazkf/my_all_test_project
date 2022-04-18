#include <iostream>
#include <cstring>

#define GETENUM(x)   (strrchr(#x,':')+1)

enum class MYENUM
{
    ENUM1,
    ENUM2,
    ENUM3
};

int main(int argc,char **argv)
{
    std::cout<<GETENUM(MYENUM::ENUM1)<<std::endl;
    return 0;
};