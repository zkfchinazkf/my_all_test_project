# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.16.1)
# 项目信息
project (Demo1)

#选择C++11
set(CMAKE_CXX_STANDARD 11)

if (NOT CHOICE)
    set(CHOICE use)
endif()


set(MYTESTMESSAGE  "message")
message("my sysroot is ${MYTESTMESSAGE}")

include_directories(src/math)
include_directories(src/sharedmemory)
include_directories(src/json)

add_definitions(-DBOOST_LOG_DYN_LINK)

add_library(mymath_lib  
        src/math/mymath.c
        src/math/mymath.h
        )
add_library(sharedmemory_lib  
                src/sharedmemory/semlock.cpp
                src/sharedmemory/semlock.h
                src/sharedmemory/memory.cpp
                src/sharedmemory/memory.h
                )

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++11 -O2  -Wall ${CMAKE_CXX_FLAGS}")
endif(CMAKE_COMPILER_IS_GNUCXX)

link_directories(
        /usr/local/lib/hiredis-vip
)

# IF ( ${CHOICE} STREQUAL test)
#     # 指定生成目标
#     aux_source_directory(srctest/. DIR_SRCS)
# else()
#     aux_source_directory(src DIR_SRCS)
# endif()
aux_source_directory(srctest/. DIR_SRCS)
aux_source_directory(src DIR_SRCS)

add_definitions(-DTESTDATA="this is the test node")


add_executable(Demo         #all demo
        src/youhua.cpp
        )
target_link_libraries(Demo
        mymath_lib
        sharedmemory_lib
        rt
        )

# add_executable(redisdemo    #redis send demo      need hiredis_vip lib
#         src/redisdemo.cpp
#         src/json/json.hpp
#         )
# target_link_libraries(redisdemo
#         hiredis_vip
#         )

add_executable(ptrctltest    #pthread ptrctl fun demo
        src/prctl.cpp
        )
target_link_libraries(ptrctltest
        pthread
        )
        

add_executable(optdemo       #cmd args use opt analysis demo
        src/optdemo.cpp
        )

add_executable(test           #test cmakelist input define  
        srctest/mytest.cpp
        )

add_executable(myexpect       #use to test expect sh
        srctest/expect.cpp
        )

# add_executable(test_serial    #boostlog serial demo 
#         src/serial.cpp
#         )
# target_link_libraries(test_serial
#         boost_serialization
#         pthread
#         )

add_executable(candcplususe     #test c and cplusplus fun demo
        srctest/candcplususe.cpp
        )

add_executable(testjson         #test nlohmann json demo
        src/json/jsontest.cpp
        )

add_executable(chronodemo       #test c++ time fun chrono
        src/chronodemo.cpp
        )

add_executable(staticregister   #test static register demo
        src/staticregister.cpp
        )

add_executable(printfdemo       #all c++ test demo
        src/printfdemo.cpp
        )

add_executable(Cplusplus       #all c++ test demo
        src/Cplus/Cplusplus.cpp
        )

add_executable(stldemo       #all c++ test demo
        src/Cplus/stl.cpp
        )

add_executable(normaldemo       #all c++ test demo
        src/Cplus/normal.cpp
        )

# add_executable(boostlogdemo       #all c++ test demo
#         src/boostlogdemo/boostlogdemo.cpp
#         )
# target_link_libraries(boostlogdemo
#         boost_log
#         boost_log_setup
#         boost_date_time
#         boost_program_options
#         boost_filesystem
#         boost_system
#         boost_serialization
#         pthread
#         )


add_executable(getclock       #get clock demo
        src/getclock.cpp
        )
   
# add_executable(boostcoroutines2       #get clock demo
#         src/boost/boostcoroutines2.cpp
#         )   
# target_link_libraries(boostcoroutines2
#         boost_context
#         )  

# add_executable(boosttest       #get clock demo
#         src/boost/boosttest.cpp
#         )   
        

add_executable(pthreaddemo       #get clock demo
        src/linux/mypthread.cpp
        )   
target_link_libraries(pthreaddemo
        pthread
        )  

# set_target_properties(Demo ${DIR_SRCS})

# add_executable(Demo ${DIR_SRCS})

add_executable(readlinedemo       #get clock demo
        src/C/readline.c
        )   
target_link_libraries(readlinedemo
        readline
        )  
        
add_executable(cdemo       #get clock demo
        src/C/linuxc.c
        )   
target_link_libraries(cdemo
        readline
        )  
        

add_executable(unixclidemo       #unix client demo
        src/filedescriptor/af_unixcli.c
        )   
        
add_executable(unixserdemo       #unix server demo
        src/filedescriptor/af_unixser.c
)   

        
add_executable(bookplusfirstdemo       #cplusbingxingbook first
        src/book/cplusbingxing/first.cpp
)   
target_link_libraries(bookplusfirstdemo
        pthread
        )  
add_executable(bookplusseconddemo       #cplusbingxingbook second
        src/book/cplusbingxing/second.cpp
)   
target_link_libraries(bookplusseconddemo
        pthread
        )  