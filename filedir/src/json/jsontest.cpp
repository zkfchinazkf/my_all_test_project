#include <iostream>
#include "json.hpp"
#include <fstream>
#include <string>

#define FILENAME "../src/json/testfile.txt"


enum class INTYPE
{
    TYPESTRING = 0,
    TYPEINT,
    TYPEENUM,
    TYPEFLOAT
};

class GetJsonData
{
    public:

};


char *typefindus(INTYPE intype)
{
    switch(intype)
    {
        case INTYPE::TYPESTRING:
            break;
    }
}

int main(int argc,char **argv)
{
    std::ifstream   cmd_ifs(FILENAME);
    std::string testdata;
    nlohmann::json readfile;
    cmd_ifs >> readfile;
    std::cout<<readfile.size()<<std::endl;
    for(auto &jsoniter:readfile.items())
    {
        if(jsoniter.value().contains("value"))
        {
            std::cout<<"key="<<jsoniter.key()<<std::endl;
            std::cout<<"value="<<jsoniter.value().at("value")<<std::endl;
            std::cout<<"type="<<jsoniter.value().at("type")<<std::endl;
        }
        else 
        {
            for(auto &valueiter:jsoniter.value().items())
            {
                if(valueiter.value().contains("value"))
                {
                    std::cout<<"key="<<valueiter.key()<<std::endl;
                    std::cout<<"value="<<valueiter.value().at("value")<<std::endl;
                    std::cout<<"type="<<valueiter.value().at("type")<<std::endl;
                }
            }
        }
    }

}