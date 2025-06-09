cmake_minimum_required(VERSION 3.10.0)
project(agent VERSION 0.1.0 LANGUAGES C CXX)

find_package(Boost COMPONENTS json)

include_directories(${Boost_INCLUDE_DIRS})

# Настраиваем флаги для покрытия
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")

add_executable(agent main.cpp)

target_link_libraries(agent PRIVATE ${Boost_LIBRARIES})

include_directories(${Boost_INCLUDE_DIRS})