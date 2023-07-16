#include <iostream>
#include <cstring>
#include <set>
#include <map>
#include <vector>
#include <tuple>
#include <list>
#include <memory>

/*
    std::remove_reference  获取去除引用后的类型    typename std::remove_reference<_T>::type  xxx
    std::std::remove_cv 去除const 和 volatile限定  typename std::remove_cv<_T>::type  xxx
    explicit 关键字   禁止隐式类型转换
    __builtin_expect(x,N)  告诉编译器x==N的可能性很大    编译器在编译过程中，会将可能性更大的代码紧跟着起面的代码，从而减少指令跳转带来的性能上的下降
*/
#define GETENUM(x)   (strrchr(#x,':')+1)

#define LINKLY(expr)    __builtin_expect(!!(expr),1)    //使用在if语句中，可能性很大为true的情况
#define UNLINKLY(expr)  __builtin_expect(!!(expr),0)    //使用在if语句中，可能性很大为false的情况


enum class MYENUM
{
    ENUM1,
    ENUM2,
    ENUM3
};

class exception: public std::exception
{
    std::string m_name;
    public:
        exception(std::string name):m_name(name){}
        const char* what() const noexcept override
        {
            return "data false";
        }
        std::string name()
        {
            return m_name;
        }
};

class myexpectclass 
{
    public:
        myexpectclass(int num) 
        {
            throw exception("myfalse");
        }
        myexpectclass()  noexcept      //使得throw出来的错误直接报错，而不会被捕获到 
        {
            throw "false";
        }
};

template<typename _Ty>
class myenabif 
{   
    private:
        _Ty     m_data;
    public:
        myenabif(_Ty data):m_data(data){}
        
        template<typename _T>
        typename std::enable_if<(std::is_same<_T,_Ty>() == true), _T>::type myenabiftest()
        {
            std::cout<<"_T and _Ty is same"<<std::endl;
            return m_data;
        }
        
        // template<typename _T>
        // typename std::enable_if<(std::is_same<_T,_Ty>() == false), _T>::type myenabiftest()
        // {
        //     std::cout<<"_T and _Ty no same"<<std::endl;
        //     return m_data;
        // }
        template<typename _T>
        typename std::enable_if<(true), _T>::type myenabiftest()
        {
            std::cout<<"all is false"<<std::endl;
            return m_data;
        }
};

template <typename _T1,typename _T2>
void changefun(typename std::remove_reference<_T1>::type num1,_T2 num2)  
{
    num1 = num2;
}
template <typename _T1,typename _T2>
void changefun1(_T1 num1,_T2 num2)
{
    num1 = num2;
}

class setdemo
{
    public:
        static std::set<int> classset;
        void insertsetdemo(int x);
};
void setdemo::insertsetdemo(int x)
{
    std::set<int> ininin;
    ininin.insert(x);
    classset.insert(x);
    std::cout<<"ininin:"<<std::endl;
    for(auto iter:ininin)
    {
        std::cout<<iter<<std::endl;
    }
    std::cout<<"classset:"<<std::endl;
    for(auto iter:classset)
    {
        std::cout<<iter<<std::endl;
    }
}
std::set<int> setdemo::classset;

class retselfptr:public std::enable_shared_from_this<retselfptr>
{
    public:
        std::shared_ptr<retselfptr> retself()
        {
            return shared_from_this();
        }
};

struct dd
{
    uint64_t                                timestamp;
    virtual void support_dynamic()=0;
};
struct cc:dd
{
    std::vector<float> 	                    cells_voltage;
    std::vector<float> 	                    sensors_temp;
    float 				                    current;
    float 				                    voltage;
    float                                   soc;
    float                                   remaining_capacity;
    uint8_t			                        user_defined_number;
    float 			                        total_capacity;
    uint16_t 			                    cycle;
    float 			                        design_capacity;
    uint16_t                                mos_status;
    float                                   mos_temp;
    uint16_t                                status_code;
    bool                                    calibration = false;
    void support_dynamic(){};
};
struct bb:dd
{
    std::list<std::tuple<
        int,
        std::string
    >>                                      alarm_list;
    void support_dynamic(){};
};
std::shared_ptr<dd> getdd()   //can not use const dd
{
    return std::make_shared<cc>();
}

int main(int argc,char **argv)
{
    std::cout<<GETENUM(MYENUM::ENUM1)<<std::endl;

    try{
        myexpectclass myexp1(12);
    }
    catch(exception &e)  //== catch(exception &e)
    {
        std::cout<< e.name() <<" = "<<e.what()<<std::endl;
    }
    // try{
    //     myexpectclass myexp2;
    // }
    // catch(...)
    // {
    //     std::cout<<"myexp2"<<std::endl;   //no catch
    // }
    myenabif<char>  data('c');
    std::cout << data.myenabiftest<int>()<<std::endl;

    int num1=100,num2=200;
    changefun<int &,int &>(num1,num2);
    std::cout<<"num1="<<num1<<",num2="<<num2<<std::endl;
    changefun1<int &,int &>(num1,num2);
    std::cout<<"num1="<<num1<<",num2="<<num2<<std::endl;

    std::string strdata;
    uint8_t addbuf[]={'a','b','c'};
    for(int idx=0;idx<3;idx++)
         strdata += addbuf[idx];
    std::cout<<"str data = "<<strdata<<std::endl;

    setdemo *mysetdemo;
    mysetdemo->insertsetdemo(10);

    std::shared_ptr<retselfptr> myptrs=std::make_shared<retselfptr>();

    std::cout<<"myptr use num:"<<myptrs.use_count()<<std::endl;

    std::shared_ptr<retselfptr> myptr1=myptrs;
    std::shared_ptr<retselfptr> myptr2=myptrs->retself();
    
    std::cout<<"myptr use num:"<<myptrs.use_count()<<std::endl;
    std::cout<<"myptr use num:"<<myptr1.use_count()<<std::endl;
    std::cout<<"myptr use num:"<<myptr2.use_count()<<std::endl;

    if(__builtin_constant_p(1))
    {
        printf("ddd\n");
    }
    else 
    {
        printf("ccc\n");
    }
    register int x=1; //将变量放入寄存器
    if(__builtin_constant_p(x))
    {
        printf("ddd\n");
    }
    else 
    {
        printf("ccc\n");
    }

    const double x1=329.9999999999999;
    const double x2=330.0000000000000;
    if(x1>x2)
    {
        std::cout<<"x1:"<<x1<<">x2:"<<x2<<std::endl;
    }
    else if(x1<x2)
    {
        std::cout<<"x1:"<<x1<<"<x2:"<<x2<<std::endl;
    }
    else
    {
        printf("x1==x2\n");
    }

    std::map<int,std::string> demoMap=
    {
        {1,"cc"},
        {2,"dd"}
    };
    for(auto iter:demoMap)
    {
        std::cout<<iter.first<<","<<iter.second<<std::endl;
    }
//     enum ErrorType:uint32_t
//     {
//         E_BATTERY_SHORT_CIRCUIT_PROTECTION                  = 0x50901011, /* 电池短路保护 */
//         E_BATTERY_DISCHARGE_OVERCURRENT_PROTECTIION         = 0x50901012, /* 电池放电过流保护 */
//         E_BATTERY_CHARGE_OVERCURRENT_PROTECTIION            = 0x50901013, /* 电池充电过流保护 */
//         E_BATTERY_TOTAL_PRESSURE_OVERDISCHARGE_PROTECTIION  = 0x50901014, /* 电池总压过放保护 */
//         E_BATTERY_SINGLE_OVERDISCHARGE_PROTECTIION          = 0x50901015, /* 电池单体过充保护 */
//         E_BATTERY_SINGLE_OVERRELEASE_PROTECTIION            = 0x50901016, /* 电池单体过放保护 */
        
//         E_BATTERY_MOS_TEMPERATURE_PROTECTIION               = 0x50901017, /* 电池MOS高温保护 */
//         E_BATTERY_DISCHARGING_LOW_TEMPERATURE_PROTECTIION   = 0x50901018, /* 电池放电低温（电芯）保护 */
//         E_BATTERY_CHARGING_LOW_TEMPERATURE_PROTECTIION      = 0x50901019, /* 电池充电低温（电芯）保护 */
//         E_BATTERY_DISCHARGING_HIGH_TEMPERATURE_PROTECTIION  = 0x5090101a, /* 电池放电高温（电芯）保护 */
//         E_BATTERY_CHARGING_HIGH_TEMPERATURE_PROTECTIION     = 0x5090101b, /* 电池充电高温（电芯）保护 */
        
//         E_BATTERY_DFET_STATUE                               = 0x5090101c, /* 电池DFET 状态 */
//         E_BATTERY_CFET_STATUE                               = 0x5090101d, /* 电池CFET 状态 */

//         E_BATTERY_SAMPLING_FAULT                            = 0x5090101e, /* 电池采样故障 */
//         E_BATTERY_CELL_FAULT                                = 0x5090101f, /* 电池电芯故障 */
//         E_BATTERY_NTC_FAULT                                 = 0x50901020, /* 电池NTC故障 */
//         E_BATTERY_DISCHARGING_MOS_FAULT                     = 0x50901021, /* 电池放电MOS故障 */
//         E_BATTERY_CHARGING_MOS_FAULT                        = 0x50901022, /* 电池充电MOS故障 */
        
//         E_BATTERY_EIGHT_NODAL_EQUILIBRIUM_STATE             = 0x50901023, /* 电池第八节均衡状态 */
//         E_BATTERY_SEVEN_NODAL_EQUILIBRIUM_STATE             = 0x50901024, /* 电池第七节均衡状态 */
//         E_BATTERY_SIX_NODAL_EQUILIBRIUM_STATE               = 0x50901025, /* 电池第六节均衡状态 */
//         E_BATTERY_FIVE_NODAL_EQUILIBRIUM_STATE              = 0x50901026, /* 电池第五节均衡状态 */
//         E_BATTERY_FOUR_NODAL_EQUILIBRIUM_STATE              = 0x50901027, /* 电池第四节均衡状态 */
//         E_BATTERY_THREE_NODAL_EQUILIBRIUM_STATE             = 0x50901028, /* 电池第三节均衡状态 */
//         E_BATTERY_TWO_NODAL_EQUILIBRIUM_STATE               = 0x50901029, /* 电池第二节均衡状态 */
//         E_BATTERY_ONE_NODAL_EQUILIBRIUM_STATE               = 0x5090102a, /* 电池第一节均衡状态 */
        
//         E_BATTERY_SIXTEEN_NODAL_EQUILIBRIUM_STATE           = 0x5090102b, /* 电池第十六节均衡状态 */
//         E_BATTERY_FIFTEEN_NODAL_EQUILIBRIUM_STATE           = 0x5090102c, /* 电池第十五节均衡状态 */
//         E_BATTERY_FOURTEEN_NODAL_EQUILIBRIUM_STATE          = 0x5090102d, /* 电池第十四节均衡状态 */
//         E_BATTERY_THIRTEEN_NODAL_EQUILIBRIUM_STATE          = 0x5090102e, /* 电池第十三节均衡状态 */
//         E_BATTERY_TWELVE_NODAL_EQUILIBRIUM_STATE            = 0x5090102f, /* 电池第十二节均衡状态 */
//         E_BATTERY_ELEVEN_NODAL_EQUILIBRIUM_STATE            = 0x50901030, /* 电池第十一节均衡状态 */
//         E_BATTERY_TEN_NODAL_EQUILIBRIUM_STATE               = 0x50901031, /* 电池第十节均衡状态 */
//         E_BATTERY_NINE_NODAL_EQUILIBRIUM_STATE              = 0x50901032, /* 电池第九节均衡状态 */

//         E_BATTERY_DISCHARGE_OVERCURRENT_ALARM               = 0x50901033, /* 电池放电过流告警 */
//         E_BATTERY_CHARGE_OVERCURRENT_ALARM                  = 0x50901034, /* 电池充电过流告警 */
//         E_BATTERY_TOTAL_PRESSURE_LOW_ALARM                  = 0x50901035, /* 电池总压低压告警 */
//         E_BATTERY_TOTAL_PRESSURE_HIGH_ALARM                 = 0x50901036, /* 电池总压高压告警 */
//         E_BATTERY_SINGLE_PRESSURE_LOW_ALARM                 = 0x50901037, /* 电池单体低压告警 */
//         E_BATTERY_SINGLE_PRESSURE_HIGH_ALARM                = 0x50901038, /* 电池单体高压告警 */

//         E_BATTERY_LOW_POWER_ALARM                           = 0x50901039, /* 电池低电量告警 */
//         E_BATTERY_MOS_ALARM                                 = 0x5090103a, /* 电池MOS告警 */
//         E_BATTERY_AMBIENT_LOW_TEMPERATURE_ALARM             = 0x5090103b, /* 电池环境低温告警 */
//         E_BATTERY_AMBIENT_HIGH_TEMPERATURE_ALARM            = 0x5090103c, /* 电池环境高温告警 */
//         E_BATTERY_DISCHARGING_LOW_TEMPERATURE_ALARM         = 0x5090103d, /* 电池放电低温（电芯）告警 */
//         E_BATTERY_CHARGING_LOW_TEMPERATURE_ALARM            = 0x5090103e, /* 电池充电低温（电芯）告警 */
//         E_BATTERY_DISCHARGING_HIGH_TEMPERATURE_ALARM        = 0x5090103f, /* 电池放电高温（电芯）告警 */
//         E_BATTERY_CHARGING_HIGH_TEMPERATURE_ALARM           = 0x50901040, /* 电池充电高温（电芯）告警 */
//     };
//     uint8_t info[]={0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01};
//     uint32_t inf_ptr=0;
//     std::vector<uint8_t> state_vec;
//     state_vec.push_back(info[inf_ptr++]);
//     state_vec.push_back(info[inf_ptr++]);
//     state_vec.push_back(info[inf_ptr++]);
//     state_vec.push_back(info[inf_ptr++]);
//     state_vec.push_back(info[inf_ptr++]);
//     state_vec.push_back(info[inf_ptr++]);
//     state_vec.push_back(info[inf_ptr++]);
//     state_vec.push_back(info[inf_ptr++]);
//     state_vec.push_back(info[inf_ptr]);

// #define KE_INFO(x,y)  x

//     std::map<int,std::tuple<ErrorType,std::string>> errortypemap = 
//     {
//         {6,std::make_tuple(ErrorType::E_BATTERY_SHORT_CIRCUIT_PROTECTION                   ,KE_INFO("Battery short-circuit protection","电池短路保护"))},
//         {5,std::make_tuple(ErrorType::E_BATTERY_DISCHARGE_OVERCURRENT_PROTECTIION          ,KE_INFO("Battery discharge overcurrent protection","电池放电过流保护"))},
//         {4,std::make_tuple(ErrorType::E_BATTERY_CHARGE_OVERCURRENT_PROTECTIION             ,KE_INFO("Battery charge overcurrent protection","电池充电过流保护"))},
//         {3,std::make_tuple(ErrorType::E_BATTERY_TOTAL_PRESSURE_OVERDISCHARGE_PROTECTIION   ,KE_INFO("Battery overvoltage protection","电池总压过放保护"))},
//         {2,std::make_tuple(ErrorType::E_BATTERY_SINGLE_OVERDISCHARGE_PROTECTIION           ,KE_INFO("Battery overcharge protection","电池单体过充保护"))},
//         {1,std::make_tuple(ErrorType::E_BATTERY_SINGLE_OVERRELEASE_PROTECTIION             ,KE_INFO("Battery overdischarge protection","电池单体过放保护"))},
//         {0,std::make_tuple(ErrorType::E_BATTERY_SINGLE_OVERDISCHARGE_PROTECTIION           ,KE_INFO("Battery overcharge protection","电池单体过充保护"))},
        
//         {12,std::make_tuple(ErrorType::E_BATTERY_MOS_TEMPERATURE_PROTECTIION               ,KE_INFO("Battery MOS high temperature protection","电池MOS高温保护"))},
//         {11,std::make_tuple(ErrorType::E_BATTERY_DISCHARGING_LOW_TEMPERATURE_PROTECTIION   ,KE_INFO("Battery discharge low temperature (cell) protection","电池放电低温（电芯）保护"))},
//         {10,std::make_tuple(ErrorType::E_BATTERY_CHARGING_LOW_TEMPERATURE_PROTECTIION      ,KE_INFO("Low temperature (cell) protection for battery charging","电池充电低温（电芯）保护"))},
//         {9,std::make_tuple(ErrorType::E_BATTERY_DISCHARGING_HIGH_TEMPERATURE_PROTECTIION   ,KE_INFO("Battery discharge high temperature (cell) protection","电池放电高温（电芯）保护"))},
//         {8,std::make_tuple(ErrorType::E_BATTERY_CHARGING_HIGH_TEMPERATURE_PROTECTIION      ,KE_INFO("Battery charging high temperature (cell) protection","电池充电高温（电芯）保护"))},
        
//         {18,std::make_tuple(ErrorType::E_BATTERY_DFET_STATUE                               ,KE_INFO("Battery DFET status","电池DFET 状态"))},
//         {17,std::make_tuple(ErrorType::E_BATTERY_CFET_STATUE                               ,KE_INFO("Battery CFET status","电池CFET 状态"))},

//         {37,std::make_tuple(ErrorType::E_BATTERY_SAMPLING_FAULT                            ,KE_INFO("Battery sampling failure","电池采样故障"))},
//         {36,std::make_tuple(ErrorType::E_BATTERY_CELL_FAULT                                ,KE_INFO("The battery cell is faulty","电池电芯故障"))},
//         {34,std::make_tuple(ErrorType::E_BATTERY_NTC_FAULT                                 ,KE_INFO("The battery NTC is faulty","电池NTC故障"))},
//         {33,std::make_tuple(ErrorType::E_BATTERY_DISCHARGING_MOS_FAULT                     ,KE_INFO("Battery discharging MOS is faulty","电池放电MOS故障"))},
//         {32,std::make_tuple(ErrorType::E_BATTERY_CHARGING_MOS_FAULT                        ,KE_INFO("Battery charging MOS is faulty","电池充电MOS故障"))},
        
//         {47,std::make_tuple(ErrorType::E_BATTERY_EIGHT_NODAL_EQUILIBRIUM_STATE             ,KE_INFO("Battery section 8 equilibrium state","电池第八节均衡状态"))},
//         {46,std::make_tuple(ErrorType::E_BATTERY_SEVEN_NODAL_EQUILIBRIUM_STATE             ,KE_INFO("Battery section 7 equilibrium state","电池第七节均衡状态"))},
//         {45,std::make_tuple(ErrorType::E_BATTERY_SIX_NODAL_EQUILIBRIUM_STATE               ,KE_INFO("Battery section 6 equilibrium state","电池第六节均衡状态"))},
//         {44,std::make_tuple(ErrorType::E_BATTERY_FIVE_NODAL_EQUILIBRIUM_STATE              ,KE_INFO("Battery section 5 equilibrium state","电池第五节均衡状态"))},
//         {43,std::make_tuple(ErrorType::E_BATTERY_FOUR_NODAL_EQUILIBRIUM_STATE              ,KE_INFO("Battery section 4 equilibrium state","电池第四节均衡状态"))},
//         {42,std::make_tuple(ErrorType::E_BATTERY_THREE_NODAL_EQUILIBRIUM_STATE             ,KE_INFO("Battery section 3 equilibrium state","电池第三节均衡状态"))},
//         {41,std::make_tuple(ErrorType::E_BATTERY_TWO_NODAL_EQUILIBRIUM_STATE               ,KE_INFO("Battery section 2 equilibrium state","电池第二节均衡状态"))},
//         {40,std::make_tuple(ErrorType::E_BATTERY_ONE_NODAL_EQUILIBRIUM_STATE               ,KE_INFO("Battery section 1 equilibrium state","电池第一节均衡状态"))},
        
//         {55,std::make_tuple(ErrorType::E_BATTERY_SIXTEEN_NODAL_EQUILIBRIUM_STATE           ,KE_INFO("Battery section 16 equilibrium state","电池第十六节均衡状态"))},
//         {54,std::make_tuple(ErrorType::E_BATTERY_FIFTEEN_NODAL_EQUILIBRIUM_STATE           ,KE_INFO("Battery section 15 equilibrium state","电池第十五节均衡状态"))},
//         {53,std::make_tuple(ErrorType::E_BATTERY_FOURTEEN_NODAL_EQUILIBRIUM_STATE          ,KE_INFO("Battery section 14 equilibrium state","电池第十四节均衡状态"))},
//         {52,std::make_tuple(ErrorType::E_BATTERY_THIRTEEN_NODAL_EQUILIBRIUM_STATE          ,KE_INFO("Battery section 13 equilibrium state","电池第十三节均衡状态"))},
//         {51,std::make_tuple(ErrorType::E_BATTERY_TWELVE_NODAL_EQUILIBRIUM_STATE            ,KE_INFO("Battery section 12 equilibrium state","电池第十二节均衡状态"))},
//         {50,std::make_tuple(ErrorType::E_BATTERY_ELEVEN_NODAL_EQUILIBRIUM_STATE            ,KE_INFO("Battery section 11 equilibrium state","电池第十一节均衡状态"))},
//         {49,std::make_tuple(ErrorType::E_BATTERY_TEN_NODAL_EQUILIBRIUM_STATE               ,KE_INFO("Battery section 10 equilibrium state","电池第十节均衡状态"))},
//         {48,std::make_tuple(ErrorType::E_BATTERY_NINE_NODAL_EQUILIBRIUM_STATE              ,KE_INFO("Battery section 9 equilibrium state","电池第九节均衡状态"))},

//         {61,std::make_tuple(ErrorType::E_BATTERY_DISCHARGE_OVERCURRENT_ALARM               ,KE_INFO("The battery overcurrent alarm is generated","电池放电过流告警"))},
//         {60,std::make_tuple(ErrorType::E_BATTERY_CHARGE_OVERCURRENT_ALARM                  ,KE_INFO("The battery overcharge alarm is generated","电池充电过流告警"))},
//         {59,std::make_tuple(ErrorType::E_BATTERY_TOTAL_PRESSURE_LOW_ALARM                  ,KE_INFO("Total battery voltage low voltage alarm","电池总压低压告警"))},
//         {58,std::make_tuple(ErrorType::E_BATTERY_TOTAL_PRESSURE_HIGH_ALARM                 ,KE_INFO("Total battery voltage High voltage alarm","电池总压高压告警"))},
//         {57,std::make_tuple(ErrorType::E_BATTERY_SINGLE_PRESSURE_LOW_ALARM                 ,KE_INFO("The battery low voltage alarm is generated","电池单体低压告警"))},
//         {56,std::make_tuple(ErrorType::E_BATTERY_SINGLE_PRESSURE_HIGH_ALARM                ,KE_INFO("The battery high voltage alarm is generated","电池单体高压告警"))},

//         {71,std::make_tuple(ErrorType::E_BATTERY_LOW_POWER_ALARM                           ,KE_INFO("The battery is low","电池低电量告警"))},
//         {70,std::make_tuple(ErrorType::E_BATTERY_MOS_ALARM                                 ,KE_INFO("Battery MOS alarm","电池MOS告警"))},
//         {69,std::make_tuple(ErrorType::E_BATTERY_AMBIENT_LOW_TEMPERATURE_ALARM             ,KE_INFO("The battery ambient temperature is low","电池环境低温告警"))},
//         {68,std::make_tuple(ErrorType::E_BATTERY_AMBIENT_HIGH_TEMPERATURE_ALARM            ,KE_INFO("The battery environment is overheated","电池环境高温告警"))},
//         {67,std::make_tuple(ErrorType::E_BATTERY_DISCHARGING_LOW_TEMPERATURE_ALARM         ,KE_INFO("The low-temperature battery discharge (battery) alarm is generated","电池放电低温（电芯）告警"))},
//         {66,std::make_tuple(ErrorType::E_BATTERY_CHARGING_LOW_TEMPERATURE_ALARM            ,KE_INFO("The low-temperature battery charge (battery) alarm is generated","电池充电低温（电芯）告警"))},
//         {65,std::make_tuple(ErrorType::E_BATTERY_DISCHARGING_HIGH_TEMPERATURE_ALARM        ,KE_INFO("The high-temperature battery discharge (battery) alarm is generated","电池放电高温（电芯）告警"))},
//         {64,std::make_tuple(ErrorType::E_BATTERY_CHARGING_HIGH_TEMPERATURE_ALARM           ,KE_INFO("The high-temperature battery charge (battery) alarm is generated","电池充电高温（电芯）告警"))}
//     };
//     for(auto itermap:errortypemap)
//     {
//         if(state_vec.at(itermap.first/8) & (0x1<<(itermap.first%8)))
//         {
//             std::cout<<std::get<1>(itermap.second)<<std::endl;
//         }
//     }

//     std::shared_ptr<dd> newptr = getdd();
//     if(std::dynamic_pointer_cast<cc>(newptr) != nullptr)
//     {
//         std::cout<<"get is cc"<<std::endl;
//     }

    std::list<std::tuple<int,int>> m_list_tuple_demo;
    m_list_tuple_demo.push_back(std::make_pair(11,11));
    m_list_tuple_demo.pop_back();
    std::cout<<std::get<0>(m_list_tuple_demo.back())<<std::endl;

    return 0;
};