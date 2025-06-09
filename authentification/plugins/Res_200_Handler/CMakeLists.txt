cmake_minimum_required(VERSION 3.10.0)
project(Res_200_Handler VERSION 0.1.0 LANGUAGES C CXX)

find_package(Boost REQUIRED COMPONENTS url system json)

find_package(OpenSSL REQUIRED)

find_package(jwt-cpp CONFIG REQUIRED)

add_library(Res_200_Handler SHARED Res_200_Handler.cpp)
#add_executable(Res_200_Handler  Res_200_Handler.cpp)

target_link_libraries(Res_200_Handler PRIVATE ${Boost_LIBRARIES} ${OpenSSL_LIBRARIES} jwt-cpp::jwt-cpp)

include_directories(${Boost_INCLUDE_DIRS})




