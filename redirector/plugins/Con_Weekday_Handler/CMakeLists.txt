cmake_minimum_required(VERSION 3.10.0)
project(Con_Weekday_Handler VERSION 0.1.0 LANGUAGES C CXX)


find_package(Boost REQUIRED COMPONENTS url system json)


add_library(Con_Weekday_Handler SHARED Con_Weekday_Handler.cpp)

target_link_libraries(Con_Weekday_Handler PRIVATE ${Boost_LIBRARIES})

include_directories(${Boost_INCLUDE_DIRS})
