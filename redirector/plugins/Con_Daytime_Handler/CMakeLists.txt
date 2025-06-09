cmake_minimum_required(VERSION 3.10.0)
project(Con_Daytime_Handler VERSION 0.1.0 LANGUAGES C CXX)


find_package(Boost REQUIRED COMPONENTS url system json)


add_library(Con_Daytime_Handler SHARED Con_Daytime_Handler.cpp)
#add_executable(Con_Daytime_Handler  Con_Daytime_Handler.cpp)

target_link_libraries(Con_Daytime_Handler PRIVATE ${Boost_LIBRARIES})

include_directories(${Boost_INCLUDE_DIRS})
