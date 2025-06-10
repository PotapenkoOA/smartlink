cmake_minimum_required(VERSION 3.10.0)
project(Res_405_Handler VERSION 0.1.0 LANGUAGES C CXX)


find_package(Boost REQUIRED COMPONENTS url system json)

# Определяем путь к директории с заголовками
include_directories(../../)

add_library(Res_405_Handler SHARED Res_405_Handler.cpp)

target_link_libraries(Res_405_Handler PRIVATE ${Boost_LIBRARIES})

include_directories(${Boost_INCLUDE_DIRS})
