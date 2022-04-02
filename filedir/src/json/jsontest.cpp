#include <iostream>
#include "json.hpp"
#include <fstream>
#include <string>
#include <stdio.h>
#include <unistd.h>

#define FILENAME "../src/json/testfile.txt"


/*
{{ "aa",1}}  == {"aa":1}
{"aa",1}  == ["aa",1]

nlohmann::json::json_pointer use to find know path file ,demo can look json_pointer demo
*/

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


    nlohmann::json deletetest = nlohmann::json 
    {
        {"hello",1},
        {"use",2}
    };
    deletetest.erase("hello");
    std::cout <<deletetest<< std::endl;

    nlohmann::json jsontype = nlohmann::json 
    {
        {
            "cc",{"aa","ssc"}   //array
            // "cc","aa"         //string
            // "cc",
            // {{"aa","ssc"}}    //object  
        }
    };
    std::cout<<jsontype<<std::endl;  
    jsontype = jsontype.at("cc");
    if(jsontype.is_string())
    {
            std::cout<<" is_string"<<std::endl;   
        std::cout<<jsontype.at("aa").is_string()<<std::endl;    
    } 
    else if(jsontype.is_array())
    {
            std::cout<<" is_array"<<std::endl;   
        for(auto iter:jsontype )
        {
            std::cout<<"iter  = " <<iter <<std::endl; 
            std::cout<<iter.is_string()<<std::endl;   
        }
    }
    else if(jsontype.is_object())
    {
        for(auto iter:jsontype.items())
        {
            std::cout<<"iter value = " <<iter.value()<<std::endl; 
            std::cout<<iter.value().is_string()<<std::endl;   
        }
    }
    else 
    {
        std::cout<<"jsontype is unknow type " <<std::endl;
    }
    // std::cout<<jsontype.at("cc").at("aa").is_string()<<std::endl;

    /*                  json_pointer demo                        */
    //**********************start********************************
    nlohmann::json pointjson = 
    {{
        "home", 
        {{"application",100}}
    }};
    std::string path = "/home/application";
    // std::string path = "/home/cc";
    nlohmann::json::json_pointer    pointer(path);
    std::cout <<pointjson<<std::endl;
    if(pointjson.contains(pointer))
    {
        std::cout << pointjson.contains(pointer)  <<std::endl;
        std::cout << pointjson.at(pointer).get<int>()  <<std::endl;
    }
    else 
    {
        std::cout << pointjson.contains(pointer)  <<std::endl;
    }
    //**********************end********************************

    
}