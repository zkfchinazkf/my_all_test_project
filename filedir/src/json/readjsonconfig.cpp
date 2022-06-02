#include <iostream>
#include "json.hpp"
#include <fstream>
#include <array>
#include <list>
#include <tuple>
#include <map>
#include <memory>

class plc_status
{
    public:
        enum PLC_STATUS_VALUE
        {
            VALUE_ERRR=0,
            VALUE_FLASE,
            VALUE_TRUE
        };
    private:
        PLC_STATUS_VALUE m_value;
    public:
        plc_status(bool value)
        {
            m_value = value ? VALUE_TRUE:VALUE_FLASE;
        }
        plc_status()
        {
            m_value = VALUE_ERRR;
        }
        PLC_STATUS_VALUE get_value()
        {
            return m_value;
        }
        bool operator==(const plc_status& plc_input) const
        {
            return this->m_value == plc_input.m_value;
        }
        std::string ToString()
        {
            std::string retstr;
            switch (m_value)
            {
                case VALUE_ERRR:
                    retstr = "VALUE_ERRR";
                    break;
                case VALUE_FLASE:
                    retstr = "VALUE_FLASE";
                    break;
                case VALUE_TRUE:
                    retstr = "VALUE_TRUE";
                    break;
                default:
                    break;
            }
            return retstr;
        }
};

int main(int argc,char **argv)
{
    std::map<std::string,plc_status>    datamap;
    datamap.emplace("heart_fault",plc_status());
    datamap.emplace("fault_0_triggered",plc_status());
    datamap.emplace("fault_1_triggered",plc_status());
    datamap.emplace("fault_2_triggered",plc_status());
    datamap.emplace("allow_stm32_enb",plc_status());
    datamap.emplace("safety_door_mode",plc_status());
    datamap.emplace("driver_circuit_err",plc_status());
    datamap.emplace("area_mian",plc_status());
    datamap.emplace("area_tunnel",plc_status());
    datamap.emplace("area_workstation",plc_status());
    datamap.emplace("area_charging",plc_status());
    datamap.emplace("mode_normal",plc_status());
    datamap.emplace("mode_recovery",plc_status());
    datamap.emplace("mode_debug",plc_status());
    datamap.emplace("safety_door_mode",plc_status());
    datamap.emplace("mode_error",plc_status());
    datamap.emplace("dddd",plc_status());

    std::ifstream inputdata("../src/json/ce.json");
    nlohmann::json recjson;
    inputdata >> recjson;
    std::cout<<recjson<<std::endl;
    if(!recjson.contains("regisister"))
    {
        std::cout<<"no has regisister"<<std::endl;
        return 1;
    }
            //reg         bit     name
    std::map<int,std::map<int,std::string>>  regist;
    uint16_t mydata[5000] ;
    memset(mydata,0,sizeof(uint16_t) * 5000);

    for(auto iter:recjson.at("regisister").items())
    {
        try
        {
            std::map<int,std::string>  powerlist;
            std::cout<<iter.value().at("addr").get<int>()<<std::endl;
            for(auto iterer:iter.value().at("power").items())
            {
                // auto ddd = iterer.value().items();
                // std::cout<<ddd.begin().key()<<std::endl;
                // std::cout<<ddd.begin().value()<<std::endl;
                powerlist.emplace(iterer.value().items().begin().value().get<int>(),iterer.value().items().begin().key());
            }
            
            regist.emplace(iter.value().at("addr").get<int>(),powerlist);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    mydata[1024] = 0xff0f;
    mydata[3342] = 0x00ff;
    for(auto iter:regist)
    {
        std::cout<<"tuple id = "<<iter.first;
        uint16_t usedata = mydata[iter.first];
        for(auto itermap:iter.second)
        {
            auto ret = datamap.find(itermap.second);
            if(ret == datamap.end())
            {
                std::cout<<itermap.second<<"is no exist in datamap"<<std::endl;
                continue;
            }
            datamap[itermap.second] = plc_status(!!(usedata & (1<<itermap.first)));
            // std::cout<<"map id = "<<itermap.first<<std::endl;
            // std::cout<<"data id = "<<itermap.second<<std::endl;
        }
    }

    for(auto ddd:datamap)
    {
        std::cout<<ddd.first<<":status:"<<ddd.second.ToString()<<std::endl;
    }
    if(datamap["area_tunnel"].get_value() == plc_status::VALUE_TRUE)
    {
        std::cout<<"area_tunnel is true"<<std::endl;
    }
    else 
    if(datamap["area_tunnel"].get_value() == plc_status::VALUE_FLASE)
    {
        std::cout<<"area_tunnel is false"<<std::endl;
    }
    else 
    {
        std::cout<<"area_tunnel is no has config"<<std::endl;
    }

    datamap["area_tunnel"] = plc_status(false);
    auto maptwo = datamap;
    if(maptwo == datamap)
    {
        std::cout<<"maptwo == datamap"<<std::endl;
    }
    else 
    {
        std::cout<<"maptwo != datamap"<<std::endl;
    }
    maptwo["area_tunnel"] = plc_status(true);
    if(maptwo == datamap)
    {
        std::cout<<"maptwo == datamap"<<std::endl;
    }
    else 
    {
        std::cout<<"maptwo != datamap"<<std::endl;
    }
    return 0;
}
