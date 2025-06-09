cmake_minimum_required(VERSION 3.10.0)
project(http_server VERSION 0.1.0 LANGUAGES C CXX)

find_package(Boost REQUIRED COMPONENTS url system json)

find_package(OpenSSL REQUIRED)

find_package(jwt-cpp CONFIG REQUIRED)

add_executable(http_server main.cpp)

target_link_libraries(http_server PRIVATE ${Boost_LIBRARIES} ${OpenSSL_LIBRARIES} jwt-cpp::jwt-cpp)

include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(plugins/Res_401_Handler)
add_subdirectory(plugins/Res_405_Handler)
add_subdirectory(plugins/Res_307_Handler)
add_subdirectory(plugins/Con_age_Handler)
add_subdirectory(plugins/Con_weekday_Handler)
add_subdirectory(plugins/Con_daytime_Handler)