#include <iostream>

namespace democlass
{
    template <typename ValueT>
    class myclassdemo
    {
        ValueT data;
        public:
            myclassdemo(ValueT newdata):data(newdata){}
            static void CoutData(ValueT mydata);//{std::cout<<"helloworld:src !"<<std::endl;};
    };

    template<> void myclassdemo<int>::CoutData(int mydata)
    {
        std::cout<<"helloworld:int "<<mydata<<"!"<<std::endl;
    }
    template<> void myclassdemo<float>::CoutData(float mydata)
    {
        std::cout<<"helloworld:float "<<mydata<<"!"<<std::endl;
    }
}

