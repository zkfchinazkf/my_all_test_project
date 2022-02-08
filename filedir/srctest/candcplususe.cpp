#include <iostream>
#include <functional>
typedef void (*mytestfunuse) (int);

void ctestfun(mytestfunuse testfun)
{
    return;
}


void demofun2(int dat)
{
    printf("dat=%d\n",dat);
}
class testclass
{
    public:
        void demofun(int dat);
        std::function<void(int dat)>  myinputfun;
        int classusefun()
        {
            // ctestfun(this->demofun);     //fault
            // ctestfun(myinputfun);        //fault
            ctestfun(demofun2);             //true
        }
};

void testclass::demofun(int dat)
{
    printf("dat=%d\n",dat);
}

int main(int argc,char **argv)
{
    return 0;
}

