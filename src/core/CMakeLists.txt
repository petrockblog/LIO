cmake_minimum_required (VERSION 3.13)
find_package (Threads)
set (CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
file(GLOB coreSRCs
    "./*.h"
    "./*.cpp"
)
add_library(LIOlib ${coreSRCs})
if(${Threads_FOUND})
    target_link_libraries(LIOlib Threads::Threads)
endif()
target_include_directories(LIOlib PUBLIC "./")
