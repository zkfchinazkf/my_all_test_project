#include <iostream>
#include <memory>
#include <string>
#include <list>
#include <functional>
#include <map>

std::map<std::string,std::function<void *(std::string)>> testlist;    //接受注册函数的表

class testaddregister    //中转类，用来将静态注册的驱动申请函数添加到注册表中
{
    public:
        testaddregister(std::string device_name,std::function<void *(std::string)> classfun)
        {
            std::cout<<"device_name="<<device_name<<std::endl;
            testlist.insert(make_pair(device_name,classfun));
        }
};

//静态注册函数
#define DRIVER_REGISTER(class_name) \
    class_name* object_register##class_name(std::string device_name)\
    {\
        return new class_name(device_name);\
    }\
    testaddregister add_create_register##class_name( \
        #class_name,object_register##class_name);

//静态声明函数
#define DRIVER_DECLARE(class_name)\
    class_name* object_declare##class_name(std::string device_id){ \
        return new class_name(device_id); \
    }


class teststr
{
    std::string dada="100";
    public:
        teststr()
        {
            std::cout<<"teststr create"<<std::endl;
        }
        teststr(std::string device_id)
        {
            dada = device_id;
        }
        ~teststr()
        {
            std::cout<<"teststr delete"<<std::endl;
        }
        std::string tostring()
        {
            return "helloworld="+dada;
        }
};

DRIVER_REGISTER(teststr)  //静态注册
DRIVER_DECLARE(teststr)   //静态声明

int main(int argc,char **argv)
{
    //智能指针 reset测试代码
    /*******************start******************************/
    std::shared_ptr<teststr> testptr;
    std::shared_ptr<teststr> testptr1;
    teststr* lookptr = new(teststr);
    testptr.reset(lookptr);
    std::cout<<"use_count="<<testptr.use_count()<<std::endl;
    // testptr.reset();    //计数减一，若计数为0则调用析构函数
    // std::cout<<"use_count="<<testptr.use_count()<<std::endl;
    std::cout<<testptr->tostring()<<std::endl;
    /*******************end******************************/


    
    //静态注册表中取出注册函数过程  
    /*******************start******************************/
    std::cout<<"testlift count="<<testlist.size()<<std::endl;
    auto iter = testlist.find("teststr");
    teststr *ret =  (teststr *)iter->second("helloworld");
    std::cout<<ret->tostring()<<std::endl;
    /*******************end******************************/
        
    return 0;
}