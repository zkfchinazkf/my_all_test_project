#include <iostream>
#include <cstring>

/*
    std::remove_reference  获取去除引用后的类型    typename std::remove_reference<_T>::type  xxx
    std::std::remove_cv 去除const 和 volatile限定  typename std::remove_cv<_T>::type  xxx
    explicit 关键字   禁止隐式类型转换
    __builtin_expect(x,N)  告诉编译器x==N的可能性很大    编译器在编译过程中，会将可能性更大的代码紧跟着起面的代码，从而减少指令跳转带来的性能上的下降
*/
#define GETENUM(x)   (strrchr(#x,':')+1)

#define LINKLY(expr)    __builtin_expect(!!(expr),1)    //使用在if语句中，可能性很大为true的情况
#define UNLINKLY(expr)  __builtin_expect(!!(expr),0)    //使用在if语句中，可能性很大为false的情况


enum class MYENUM
{
    ENUM1,
    ENUM2,
    ENUM3
};

class exception: public std::exception
{
    std::string m_name;
    public:
        exception(std::string name):m_name(name){}
        const char* what() const noexcept override
        {
            return "data false";
        }
        std::string name()
        {
            return m_name;
        }
};

class myexpectclass 
{
    public:
        myexpectclass(int num) 
        {
            throw exception("myfalse");
        }
        myexpectclass()  noexcept      //使得throw出来的错误直接报错，而不会被捕获到 
        {
            int num=0;
            throw "false";
        }
};

template<typename _Ty>
class myenabif 
{   
    private:
        _Ty     m_data;
    public:
        myenabif(_Ty data):m_data(data){}
        
        template<typename _T>
        typename std::enable_if<(std::is_same<_T,_Ty>() == true), _T>::type myenabiftest()
        {
            std::cout<<"_T and _Ty is same"<<std::endl;
            return m_data;
        }
        
        template<typename _T>
        typename std::enable_if<(std::is_same<_T,_Ty>() == false), _T>::type myenabiftest()
        {
            std::cout<<"_T and _Ty no same"<<std::endl;
            return m_data;
        }

};

template <typename _T1,typename _T2>
void changefun(typename std::remove_reference<_T1>::type num1,_T2 num2)  
{
    num1 = num2;
}
template <typename _T1,typename _T2>
void changefun1(_T1 num1,_T2 num2)
{
    num1 = num2;
}



int main(int argc,char **argv)
{
    std::cout<<GETENUM(MYENUM::ENUM1)<<std::endl;

    try{
        myexpectclass myexp1(12);
    }
    catch(exception &e)  //== catch(exception &e)
    {
        std::cout<< e.name() <<" = "<<e.what()<<std::endl;
    }
    // try{
    //     myexpectclass myexp2;
    // }
    // catch(...)
    // {
    //     std::cout<<"myexp2"<<std::endl;   //no catch
    // }
    myenabif<char>  data('c');
    std::cout << data.myenabiftest<char>()<<std::endl;


    int num1=100,num2=200;
    changefun<int &,int &>(num1,num2);
    std::cout<<"num1="<<num1<<",num2="<<num2<<std::endl;
    changefun1<int &,int &>(num1,num2);
    std::cout<<"num1="<<num1<<",num2="<<num2<<std::endl;



    return 0;
};