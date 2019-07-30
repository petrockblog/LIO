cmake_minimum_required (VERSION 3.13)
find_package (Threads)
set (CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
file(GLOB coreSRCs
    "./*.h"
    "./*.cpp"
)
list(REMOVE_ITEM coreSRCs "example.cpp")
add_library(LIOlib ${coreSRCs})
if(${Threads_FOUND})
    target_link_libraries(LIOlib Threads::Threads)
endif()
target_include_directories(LIOlib PUBLIC "./")

file(GLOB LinuxSRCs
    "./Linux/*.h"
    "./Linux/*.cpp"
)

add_library(LIOLinuxlib ${LinuxSRCs})
target_link_libraries(LIOLinuxlib LIOlib)
target_include_directories(LIOlib PUBLIC "./Linux")


add_executable(Example "example.cpp")
target_link_libraries(Example LIOLinuxlib)
