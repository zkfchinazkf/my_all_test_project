#include <iostream>
#include <string>
#include <list>

class key
{
    friend class room;
    std::string  m_usename;
    public:
        key(std::string username):m_usename(username){};

};

class room
{   
    std::string roomkeyname;
    public:
        room(key &A):roomkeyname(A.m_usename){};
        std::string get_roomkeyname()
        {
            return "roomkeyname is " + roomkeyname;
        } 

};

class faultclass
{
    public:
    int  m_faulttype;
    std::string m_faultinfo;
        faultclass(int faulttype,std::string faultinfo):m_faulttype(faulttype),m_faultinfo(faultinfo){}
        std::string getfaultdata()
        {
            return "fault type is " + std::to_string(m_faulttype) + ",fault info is " + m_faultinfo;
        }
};

class baseclass
{
    std::string  m_name;
    public:
        static std::list<baseclass *>  m_all_list;
    public:
        baseclass(std::string name):m_name(name)
        {
            m_all_list.push_back(this);
        }
        virtual std::string run(void) = 0;
};

class useclass : public baseclass
{
    std::string   m_data;
    public:
        useclass(std::string data,std::string name):baseclass(name),m_data(data){};
        std::string run(void)
        {
            std::cout<<"useclass is now class"<<std::endl;
            return m_data;
        }

};

std::list<baseclass *>  baseclass::m_all_list;


int main(int argc,char **argv)
{
    key newkey("myhouse");
    room myroom(newkey);
    std::cout<< myroom.get_roomkeyname()<<std::endl;

    try
    {
        throw faultclass(100,"testthrow");
    }
    catch(faultclass f)
    {
        std::cout <<"get fault is  "<< f.getfaultdata() <<std::endl;
    }
    
    useclass myuse("mydatacc","zkf");
    std::string getdata = baseclass::m_all_list.front()->run();
    std::cout<<getdata<<std::endl;
    return 0;
}