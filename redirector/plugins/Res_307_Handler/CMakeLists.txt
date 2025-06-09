cmake_minimum_required(VERSION 3.10.0)
project(Res_307_Handler VERSION 0.1.0 LANGUAGES C CXX)


find_package(Boost REQUIRED COMPONENTS url system json)


add_library(Res_307_Handler SHARED Res_307_Handler.cpp)

target_link_libraries(Res_307_Handler PRIVATE ${Boost_LIBRARIES})

include_directories(${Boost_INCLUDE_DIRS})
