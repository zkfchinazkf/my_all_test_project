#include <iostream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <string>
#include <sstream>

class testB
{
    public:
        int b;
    testB(int bb):b(bb)
    {};

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & b;
    }
};


class testA :public testB
{
    public:
        int a;
    testA(int aa):testB(aa),a(aa)
    {};


    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<testB>(*this);
        ar & a;
    }
};


class myserial:public testA
{
    // 为了能让串行化类库能够访问私有成员，所以要声明一个友元类
    friend class boost::serialization::access;
    // 串行化的函数，这一个函数完成对象的保存与恢复
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<testA>(*this);
        ar & data1;   //就是这么简单，也可以使用 ar<<a 这样的语法
        ar & data2;
        ar & data3;
        ar & data4;
        ar & data5;
    }
    public:
        int data1;
        int data2;
        int data3;
        int data4;
        int data5;
        myserial( ):testA(11),data1(1),data2(2),data3(5),data4(7),data5(9)
        {};
        myserial(int aa,int d1,int d2):testA(aa),data1(d1),data2(d2),data3(5),data4(7),data5(9)
        {};
};

class NoContain
{
    public:
        int aaa;
        int ccc;
    NoContain():aaa(10),ccc(12)
    {}
    NoContain(int a,int c):aaa(a),ccc(c)
    {}
};

namespace boost{
    namespace serialization{
        template<class Archive>
        void serialize(Archive & ar, NoContain & cla, const unsigned int version)
        {
            ar & cla.aaa;
            ar & cla.ccc;
        }
    }
}


int main(int argc,char **argv)
{
    std::ostringstream oss;
    boost::archive::text_oarchive   toa(oss);
    myserial da1(3,13,133);
    myserial da2(2,12,122);
    myserial da3(1,11,111);
    NoContain noc(1,2);
    toa<<da1;
    toa<<da2;
    toa<<da3;
    toa<<noc;
    
    std::istringstream istr(oss.str());
    boost::archive::text_iarchive   tia(istr);

    myserial  ddd1;
    myserial  ddd2;
    myserial  ddd3;
    NoContain  noo;
    tia>>ddd1;
    tia>>ddd2;
    tia>>ddd3;
    tia>>noo;
    std::cout<<"1:b="<<ddd1.b<<",a="<<ddd1.a<<",1="<<ddd1.data1<<",2="<<ddd1.data2<<std::endl;
    std::cout<<"2:b="<<ddd2.b<<",a="<<ddd2.a<<",1="<<ddd2.data1<<",2="<<ddd2.data2<<std::endl;
    std::cout<<"3:b="<<ddd3.b<<",a="<<ddd3.a<<",1="<<ddd3.data1<<",2="<<ddd3.data2<<std::endl;
    std::cout<<"4:aaa="<<noo.aaa<<",ccc="<<noo.ccc<<std::endl;
    return 0;
}