#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <mutex>
#include <tuple>
#include <map>
#include <memory>




/*
    C++ demo
    1.class copy demo      has  2    content: test copy class will fault  : use of deleted function 
    2.define demo          has  2    content: test define use no exist variable
    3.using typename demo  has  2    content: test using xxx = typename xxx::yyy  
    4.list for demo        has  1    content: test for use list way 
*/

/*---------------define demo start 1--------------*/
#define   mycout   std::cout<<name<<":"
/*---------------define demo end 1--------------*/

/*---------------class copy demo start 1--------------*/
class classcopytest
{
    int m_data;
    // std::mutex    m_mutex;     
    public:
    const classcopytest& operator=(const classcopytest& rhis);
    public:
        classcopytest(int data):m_data(data)
        {
            std::cout<<m_data<<":classcopytest create" <<std::endl;
        }
        ~classcopytest()
        {
            std::cout<<m_data<<":classcopytest delete" <<std::endl;
        }
};
/*---------------class copy demo end 1--------------*/


/*---------------using typename demo start 1--------------*/
class usestr
{
    public:
    class senddata
    {
        public:
            std::string toString()
            {
                return "tostring";
            }
    };

    class recvdata
    {
        private:
            std::string m_usedata;
        public:
            std::string GetRecvData()
            {
                return m_usedata;
            }
            recvdata(std::string usedata):
                m_usedata(usedata)
            {}
    };
};

template <typename _Tp>
class testclass
{
    using senddata = typename _Tp::senddata;
    using recvdata = typename _Tp::recvdata;
    public:
        std::string getsenddata(senddata data)
        {
            return data.toString();
        }
        std::string getrecvdata(recvdata data)
        {
            return data.GetRecvData();
        }
};
/*---------------using typename demo end 1--------------*/

bool retend(int testnum1,int testnum2)
{
    std::cout<<"retend"<<std::endl;
    return testnum1 == testnum2;
}

class mapemplacetest
{
    int m_data1;
    int m_data2;
    public:
        mapemplacetest(int data1,int data2):m_data1(data1),m_data2(data2){};
        std::string GetString()
        {
            return "m_data1=" + std::to_string(m_data1) + ",m_data2=" + std::to_string(m_data2);
        }
};


class baseclass
{
    baseclass*  m_self;
    public:
        std::list<baseclass *> mbaselist;
        int         m_basenum;
        baseclass(int basenum):m_basenum(basenum)
        {
            m_self = this;
            mbaselist.push_back(m_self);
        }
        virtual std::string fcoutdata() = 0;
};

class fclass:public baseclass
{
    std::string m_name;
    public:
        fclass(std::string name,int basenum):baseclass(basenum),m_name(name){};
        std::string fcoutdata()
        {
            return "m_name=" + m_name + ",m_basenum=" + std::to_string(m_basenum);
        }
};
class fclassage:public baseclass
{
    std::string m_age;
    public:
        fclassage(std::string age,int basenum):baseclass(basenum),m_age(age){};
        std::string fcoutdata()
        {
            return "m_age=" + m_age + ",m_basenum=" + std::to_string(m_basenum);
        }
};

int main(int argc,char **argv)
{
/*---------------using typename demo start 2--------------*/
    usestr::senddata  datausesenddata;
    usestr::recvdata  datauserecvdata("helloworld");
    auto dadada = testclass<usestr>();
    std::cout<<dadada.getsenddata(datausesenddata)<<std::endl;
    std::cout<<dadada.getrecvdata(datauserecvdata)<<std::endl;
/*---------------using typename demo end 2--------------*/

/*---------------list for demo start 1--------------*/
    std::list<int> data;
    data.push_back(1);
    data.push_back(2);
    data.push_back(3);
    data.push_back(4);
    //for(auto iter=data.begin();iter!= data.end();iter++) ,data.erase(iter)  will core dump ,because erase iter
    for(auto iter=data.begin();iter!= data.end();)   
    {
        std::cout << *iter << std::endl;
        data.erase(iter++);
    }
/*---------------list for demo end 1--------------*/

    std::string  strdata = "@testdata";
    strdata = strdata.substr(1);
    std::cout<<"strdata = "<<strdata <<std::endl;

/*---------------define demo start 2--------------*/
    std::string name = "zkf";
    mycout<<"hello world"<<std::endl;
/*---------------define demo end 2--------------*/

/*---------------class copy demo start 2--------------*/
    std::vector<classcopytest> classcopytestvec;
    for(int idx=0;idx<2;idx++)
    {
        std::cout<< idx <<":start"<<std::endl;
        classcopytest vectortestself(idx);
        classcopytestvec.push_back(vectortestself);    // like : classcopytestvec[xxx] = vectortestself
        std::cout<< idx <<":end"<<std::endl;
    }
/*---------------class copy demo end 2--------------*/


    if(retend(2,2) && retend(1,2));

    int whilecnt=2;
    do{
        std::cout<<"test do while break start"<<std::endl;
        continue;
        std::cout<<"test do while break end"<<std::endl;
    }while(whilecnt--);

    
    std::map<std::string,mapemplacetest> mymap;
    mymap.insert({"key",mapemplacetest(1,2)});
    std::cout<<mymap.at("key").GetString()<<std::endl;
    mymap.emplace(std::piecewise_construct,std::forward_as_tuple("emp"),std::forward_as_tuple(2,3));
    std::cout<<mymap.at("emp").GetString()<<std::endl;

    //need virtual fun
    std::list<std::shared_ptr<baseclass>> alllist;
    std::shared_ptr<fclass> fdata =std::make_shared<fclass>("zkf",24);
    std::shared_ptr<fclassage> dd =std::make_shared<fclassage>("32",12);
    alllist.push_back(fdata);
    alllist.push_back(dd);
    for(auto &iter:alllist)
    {
        // std::cout<< iter->m_basenum <<std::endl;
        auto ret =  std::dynamic_pointer_cast<fclass>(iter);
        if(ret != nullptr)
        {
            std::cout<< ret->fcoutdata() <<std::endl;
        }
        else 
        {
            std::cout<<"ret == nullptr"<<std::endl;
        }
    }

    return 0;
}