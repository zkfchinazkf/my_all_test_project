#include <hiredis-vip/hircluster.h>
#include <iostream>
#include <chrono>
#include <json.hpp>
#include <unistd.h>
#include <list>


int main(int argc,char **argv)
{
    redisClusterContext  *predis = redisClusterConnect("127.0.0.1:7001,127.0.0.1:7002,127.0.0.1:7003,127.0.0.1:7004,127.0.0.1:7005,127.0.0.1:7006",HIRCLUSTER_FLAG_ROUTE_USE_SLOTS);
     if(predis == NULL || predis->err)
    {
        printf("connect error : %s\n", predis == NULL ? "NULL" : predis->errstr);
        return -1;
    }
    std::list<std::string> usekeylist;
    usekeylist.push_back("hello1");
    usekeylist.push_back("hello2");
    usekeylist.push_back("hello3");
    usekeylist.push_back("hello4");
    usekeylist.push_back("hello5");
    usekeylist.push_back("hello6");
    while(1)
    {
        nlohmann::json report_info = nlohmann::json
        {
            { "robotState",     0 },
            { "errorState",     0 },
            { "mappingVersion", 0},
            { "mapPosition",    {{ "x",          NAN },
                                { "y",          NAN },
                                { "theta",      NAN }}},
            { "motionSpeed",    {{ "x",          NAN },
                                { "y",          NAN },
                                { "theta",      NAN }}},
            { "forkRotate",     {{ "position",   0 },
                                { "speed",      0 }}},
            { "forkStretch",    {{ "position",   0 },
                                { "speed",      0 }}},
            { "forkFinger",     {{{ "id",         0 },
                                { "status",     0 }},
                                {{ "id",         1 },
                                { "status",     0 }}}},
            { "lift",           {{ "height",     0 },
                                { "speed",      0 }}},
            {"trayQuantity",    0},
            {"trays",           0},
            {"obstacles",       0},
            {"batteryInfo",     {{"voltage", 0 },
                                {"current", 0 },
                                {"powerLevel",  0 },
                                {"cycle", 0 },
                                {"state", 0 }}},
            {"networkInfo",     {{"channel", 0 },
                                {"rssi", 0},
                                {"snr", 0 }}},
            {"lastUpdate",      std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() }
        };

        for(auto iter:usekeylist)
        {
            redisReply *reply = (redisReply*)redisClusterCommand(predis, "set %s %s",iter.c_str(),report_info.dump().c_str());
            if(reply == nullptr)
            {
                printf(" reply is NULL, error info: %s\n", predis->errstr);
                redisClusterFree(predis);
                return -1;
            }
            printf("set %s reply:%s\n",iter.c_str(),reply->str);
            freeReplyObject(reply);
            usleep(200000);
        }
    }
    
    
    
    return 0;
}