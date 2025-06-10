cmake_minimum_required(VERSION 3.10.0)
project(Con_age_Handler VERSION 0.1.0 LANGUAGES C CXX)


find_package(Boost REQUIRED COMPONENTS url system json)



include_directories(../../)

add_library(Con_age_Handler SHARED Con_age_Handler.cpp)

target_link_libraries(Con_age_Handler PRIVATE ${Boost_LIBRARIES})

include_directories(${Boost_INCLUDE_DIRS})
