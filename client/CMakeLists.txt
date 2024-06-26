cmake_minimum_required(VERSION 3.10)
project(AR_CLIENT)

set(CMAKE_CXX_STANDARD 17)

add_executable(client client.cpp main.cpp)
target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../conf)
configure_file(${CMAKE_SOURCE_DIR}/conf/config.ini config.ini COPYONLY)