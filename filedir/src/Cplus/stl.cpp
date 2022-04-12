#include <iostream>
#include <string>

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
    

    return 0;
}