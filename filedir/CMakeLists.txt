# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.16.1)
# 项目信息
project (Demo1)


if (NOT CHOICE)
    set(CHOICE use)
endif()


set(MYTESTMESSAGE  "message")
message("my sysroot is ${MYTESTMESSAGE}")

include_directories(src/math)
include_directories(src/sharedmemory)
include_directories(src/json)


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


add_executable(Demo
        src/youhua.cpp
        )
target_link_libraries(Demo
        mymath_lib
        sharedmemory_lib
        rt
        )

add_executable(redisdemo
        src/redisdemo.cpp
        src/json/json.hpp
        )
target_link_libraries(redisdemo
        hiredis_vip
        )

add_executable(ptrctltest
        src/prctl.cpp
        )
target_link_libraries(ptrctltest
        pthread
        )
        

add_executable(optdemo
        src/optdemo.cpp
        )

add_executable(test
        srctest/mytest.cpp
        )

add_executable(myexpect
        srctest/expect.cpp
        )

add_executable(test_serial
        src/serial.cpp
        )
target_link_libraries(test_serial
        boost_serialization
        pthread
        )

add_executable(candcplususe
        srctest/candcplususe.cpp
        )

add_executable(testjson
        src/json/jsontest.cpp
        )

add_executable(chronodemo
        src/chronodemo.cpp
        )
# set_target_properties(Demo ${DIR_SRCS})

# add_executable(Demo ${DIR_SRCS})

