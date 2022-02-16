#include <iostream>
#include <string>

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

int main(int argc,char **argv)
{
    usestr::senddata  datausesenddata;
    usestr::recvdata  datauserecvdata("helloworld");
    auto dadada = testclass<usestr>();
    std::cout<<dadada.getsenddata(datausesenddata)<<std::endl;
    std::cout<<dadada.getrecvdata(datauserecvdata)<<std::endl;
    return 0;
}