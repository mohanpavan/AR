
add_executable(server server.cpp main.cpp)
target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/conf)
configure_file(${CMAKE_SOURCE_DIR}/conf/config.ini config.ini COPYONLY)