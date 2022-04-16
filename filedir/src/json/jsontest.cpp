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
nlohmann::ordered_json demo can use order json data,demo can look ordered_json demo demo 
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
        default:
            break;
    }
    return NULL;
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


    
    /*                  ordered_json demo                        */
    //**********************start********************************
    std::ifstream   cmd_ifs1(FILENAME);
    nlohmann::ordered_json deletetest1;
    cmd_ifs1 >> deletetest1;
    for(auto iter:deletetest1.items())
    {
        if(iter.value().is_object())
        {
            std::cout<<"key is "<<iter.key();
            std::cout<<",value is : {";
            for(auto iterd:iter.value().items())
            {
                std::cout<<iterd.key()<<" : ";
                std::cout<<iterd.value()<<"     ";
            }
            std::cout<<"}"<<std::endl;
        }
        else 
        {
            std::cout<<"key is "<<iter.key();
            std::cout<<",value is : "<<iter.value()<<std::endl;
        }
    }
    //***********************end*********************************

    
    nlohmann::ordered_json dumpdata;
    {
        dumpdata["name"] = "zkf";
        dumpdata["time"] = "120200";
        dumpdata["tick"] = "ccc11";
        dumpdata["content"] = "ccczzz";
    }
    std::cout<<dumpdata.dump()<<std::endl;
    std::cout<<dumpdata.dump(2)<<std::endl;  //dump( " " length,is 2 ,the result will add two ' ')
    std::cout<<dumpdata.dump(10)<<std::endl;



    nlohmann::json pointjsontest = 
    {{
        "home", 
        {{"application",100}}
    }};
    nlohmann::ordered_json pointorderjsontest = 
    {{
        "home", 
        {{"application",100}}
    }};
    std::string pathstr="@";
    pathstr=pathstr.substr(1);
    if(pathstr.empty())
    {
        std::cout<<"pathstr.empty()"<<std::endl;
    }
    else 
    {
        std::cout<<"pathstr no empty()"<<std::endl;
    }
    nlohmann::json::json_pointer mypath(pathstr);
    if(pointjsontest.contains(mypath))
    {
        std::cout<<"pointjsontest.contains ,and data is :"<<pointjsontest.at(mypath)<<std::endl;
    }
    else
    {
        std::cout<<"pointjsontest is no contains "<<std::endl;
    }
    if(pointorderjsontest.contains(mypath))
    {
        std::cout<<"pointorderjsontest.contains ,and data is :"<<pointorderjsontest.at(mypath)<<std::endl;
    }
    else
    {
        std::cout<<"pointorderjsontest is no contains "<<std::endl;
    }

}