#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/from_stream.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>
#include <boost/log/utility/setup/filter_parser.hpp>
#include <boost/log/sources/severity_channel_logger.hpp> 
#include <boost/format.hpp>
#include <condition_variable>
#include <boost/phoenix.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>


// #include <boost/smart_ptr/shared_ptr.hpp>
// #include <boost/smart_ptr/make_shared_object.hpp>
// #include <boost/log/core.hpp>
// #include <boost/log/expressions.hpp>
// #include <boost/log/attributes/attribute_name.hpp>
// #include <boost/log/attributes/scoped_attribute.hpp>
// #include <boost/log/sources/logger.hpp>
// #include <boost/log/sources/record_ostream.hpp>
// #include <boost/log/support/date_time.hpp>
// #include <boost/log/utility/setup.hpp>
// #include <boost/log/utility/value_ref.hpp>
// #include <boost/log/utility/formatting_ostream.hpp>
// #include <boost/log/utility/manipulators/add_value.hpp>
// #include <boost/log/utility/setup/console.hpp>


#define   BOOSTLOG_FILE_NAME   "../src/boostlogdemo/log/"
#define   BOOSTLOG_CONFIG_NAME "../src/boostlogdemo/myboostlog.config"

#define   OSTREAM_FILE_NAME    "../src/boostlogdemo/log/myostreamfile"

#define FILENAME(path) (strrchr(path,'/') ? (strrchr(path,'/')+1) : path)


namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = logging::sinks;
namespace expr = logging::expressions;
using namespace logging::trivial;


#define ANSI_CLEAR      "\x1B[0;00m"
#define ANSI_RED        "\x1B[1;31m"
#define ANSI_GREEN      "\x1B[1;32m"
#define ANSI_YELLOW     "\x1B[1;33m"
#define ANSI_BLUE       "\x1B[1;34m"
#define ANSI_MAGENTA    "\x1B[1;35m"
#define ANSI_CYAN       "\x1B[1;36m"


class kubot_severity_level_formatter
{
public:
    typedef void result_type;

public:
    explicit kubot_severity_level_formatter(std::string const &fmt) :
            useColor_(fmt == "color") {/*std::cout<<"fmt="<<fmt<<std::endl;*/};

    void operator()(logging::formatting_ostream &strm, logging::value_ref<severity_level> const &value) const
    {
        if (!value)
            return;

        severity_level const &level = value.get();
        if (useColor_) {
            switch (level) {
                case trace:
                    strm << ANSI_CYAN << "[TRAC]" << ANSI_CLEAR;
                    break;
                case debug:
                    strm << ANSI_GREEN << "[DBUG]" << ANSI_CLEAR;
                    break;
                case info:
                    strm << ANSI_BLUE << "[INFO]" << ANSI_CLEAR;
                    std::cout << ANSI_BLUE << "[INFO]" << ANSI_CLEAR;
                    break;
                case warning:
                    strm << ANSI_YELLOW << "[WARN]" << ANSI_CLEAR;
                    break;
                case error:
                    strm << ANSI_MAGENTA << "[ERRR]" << ANSI_CLEAR;
                    break;
                case fatal:
                    strm << ANSI_RED << "[FATL]" << ANSI_CLEAR;
                    break;
                default:
                    strm << (int) level;
            }
        } else {
            switch (level) {
                case trace:
                    strm << "[TRAC]";
                    break;
                case debug:
                    strm << "[DBUG]";
                    break;
                case info:
                    strm << "[INFO]";
                    break;
                case warning:
                    strm << "[WARN]";
                    break;
                case error:
                    strm << "[ERRR]";
                    break;
                case fatal:
                    strm << "[FATL]";
                    break;
                default:
                    strm << (int) level;
            }
        }
    }

private:
    bool useColor_;
};

class kubot_severity_level_formatter_factory : public logging::basic_formatter_factory<char, severity_level>
{
public:
    formatter_type  create_formatter(logging::attribute_name const &name, args_map const &args)
    {
		auto test=args.find("data");
		for(auto testiter:args)
		{
			std::cout<<"first="<<testiter.first<<std::endl;
			std::cout<<"second="<<testiter.second<<std::endl;
		}
		std::cout<<"name="<<name.string()<<std::endl;


        args_map::const_iterator it = args.find("format");
        if (it != args.end())
            return boost::phoenix::bind(kubot_severity_level_formatter(it->second), expr::stream,
                                        expr::attr<severity_level>(name));
        else
            return boost::phoenix::bind(kubot_severity_level_formatter("default"), expr::stream,
                                        expr::attr<severity_level>(name));
    }
};



int main(int argc,char **argv)
{
    if (!boost::filesystem::exists(BOOSTLOG_FILE_NAME))
	{
		boost::filesystem::create_directory(BOOSTLOG_FILE_NAME);
	}

	logging::add_common_attributes();

	// logging::register_simple_formatter_factory<severity_level, char>("Severity");
    logging::register_formatter_factory("Severity", boost::make_shared<kubot_severity_level_formatter_factory>());
	logging::register_simple_filter_factory<severity_level, char>("Severity");
	

	std::ifstream configstream(BOOSTLOG_CONFIG_NAME);
	logging::init_from_stream(configstream);


	src::severity_logger<severity_level> lg;
	
	src::severity_channel_logger<severity_level,std::string> moduleOneLogger(logging::keywords::channel = "moduleone");
	src::severity_channel_logger<severity_level,std::string> moduleTwoLogger(logging::keywords::channel = "moduletwo");


	BOOST_LOG_TRIVIAL(info)<<"[" <<FILENAME(__FILE__)<<":"<<__LINE__<<"]"<<"testfile";
	BOOST_LOG_TRIVIAL(error)<<"[" <<FILENAME(__FILE__)<<":"<<__LINE__<<"]"<<"testfile";
	BOOST_LOG_SEV(lg,info)<<"[" <<FILENAME(__FILE__)<<":"<<__LINE__<<"]"<<"testfile";

	BOOST_LOG_SEV(moduleOneLogger,info) << boost::log::add_value("zkf", "zcd")<<"[" <<FILENAME(__FILE__)<<":"<<__LINE__<<"]"<<"testfile";
	BOOST_LOG_SEV(moduleTwoLogger,info) <<"[" <<FILENAME(__FILE__)<<":"<<__LINE__<<"]"<<"testfile";

	int cnt;

	// int cnt=2000000;
	// while(cnt--)
	// {
	// 	BOOST_LOG_SEV(moduleOneLogger,info)<<"[" <<FILENAME(__FILE__)<<":"<<__LINE__<<"]"<<"testfile";
	// 	BOOST_LOG_SEV(moduleTwoLogger,info)<<"[" <<FILENAME(__FILE__)<<":"<<__LINE__<<"]"<<"testfile";
	// 	usleep(10000);
	// }


	std::ofstream ofile(OSTREAM_FILE_NAME);

	cnt=3;
	while(cnt--)
	{
		ofile <<"[" <<FILENAME(__FILE__)<<":"<<__LINE__<<"]"<<"testfile\n";
		ofile.flush();     //同步写入文件
	}
	
    return 0;
}