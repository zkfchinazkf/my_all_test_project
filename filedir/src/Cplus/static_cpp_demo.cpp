#include <iostream>
#include "static_cpp_demo.h"
#include <vector>
#include <string.h>
#include <sstream>
#include <string>
#include <iomanip>
#include <math.h>

/*
float精度损失
-32767-32767    保留到小数点后三位，第四位丢失
*/

class myclassos
{
    public:
        myclassos()
        {
        }
        friend inline std::ostream &operator<<(std::ostream & os,int data)
        {
            return os<<std::to_string(data);
        }
};

int main(int argc,char **argv)
{
    democlass::myclassdemo<int>(100).CoutData(50);
    democlass::myclassdemo<float>(100).CoutData(50);

    std::vector<unsigned char>  vecdata;
    vecdata.push_back('a');
    vecdata.push_back('b');
    vecdata.push_back('c');
    std::cout<<std::string((char *)vecdata.data())<<std::endl;

    uint8_t buf[64]="helloworld";
    std::vector<uint8_t> vecsee(buf,buf+10);
    std::cout<<std::string((char *)vecsee.data())<<std::endl;

    std::copy(vecdata.begin(),vecdata.end(),std::back_inserter(vecsee));
    std::cout<<std::string((char *)vecsee.data())<<std::endl;
    std::cout<<std::string((char *)vecdata.data())<<std::endl;


    std::string srcdatalen ="18446744072460667972";
    std::cout << "[zkf]old:"<<srcdatalen<<std::endl;
    long long int length =  18446744069414584320;
    unsigned long long  newdd =  stoull(srcdatalen);
    std::cout << "[zkf]srcdatalen:"<<srcdatalen.c_str()<<std::endl;
    std::cout << "[zkf]newdd:"<<newdd<<std::endl;
    srcdatalen = std::to_string(newdd-length);
    std::cout << "[zkf]new:"<<srcdatalen<<std::endl;


    // myclassos mydata;
    // mydata<<22;
    double a = 32767.923434222f;
    double ad = 111111111234.123434222;
    float  af = 111234.123434222f;
    std::cout<<a<<std::endl;//输出 0.12300
    std::cout<<std::fixed<<std::setprecision(5)<<a<<std::endl;//输出 0.12300
    std::cout<<a<<std::endl;//输出 0.12300
    
    std::ostringstream mystros;
    // printf("%.10lf\n",ad);
    // printf("%.10lf\n",af);
    // mystros<<std::fixed<<std::setprecision(6)<<a<<std::endl;
    mystros<<std::fixed<<std::setprecision(6)<<a<<std::endl;
    std::cout<<std::fixed<<std::setprecision(9)<<a<<std::endl;
    std::cout<<std::fixed<<std::setprecision(8)<<std::string(mystros.str());
    std::istringstream mystris(mystros.str());
    double newdata=0.0001;
    mystris>>newdata;
    std::cout<<newdata<<std::endl;
    std::cout<<std::fixed<<std::setprecision(8)<<newdata<<std::endl;//输出 0.12300

    if(fabs(newdata-1234.1234342f) <0.00001f)
    {
        std::cout<<"new yes"<<std::endl;
    }
    return 0;
}