#include <iostream>
#include <functional>
#include <string>
#include <unistd.h>


enum class testenum  
{
    D1 = 150,
    D2,
    D3,
    D4
};
enum class testenum2 :long
{
    D1=99999999999,
    D2,
    D3,
    D4
};

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
    private:
        std::string data;
    public:
        char *getdata();
        void demofun(int dat);
        std::function<void(int dat)>  myinputfun;
        int classusefun()
        {
            // ctestfun(this->demofun);     //fault
            // ctestfun(myinputfun);        //fault
            ctestfun(demofun2);             //true
            return 0;
        }
};

void testclass::demofun(int dat)
{
    printf("dat=%d\n",dat);
}


struct _teststr
{
    char *pt;
};
struct _teststr teststr;

char * testclass::getdata()
{
    data = "helloman";
    return (char *)data.c_str();
}

int main(int argc,char **argv)
{
    // testclass dad;
    // teststr.pt=dad.getdata();
    // sleep(1);
    // printf("ddd=%s\n",teststr.pt);
    // return 0;
    
    // std::cout<<testenum::D1<<std::endl;
    // std::cout<<testenum2::D1<<std::endl;
}

