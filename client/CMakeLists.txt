
add_executable(client client.cpp main.cpp)
target_include_directories(client PRIVATE ${CMAKE_SOURCE_DIR}/conf)
configure_file(${CMAKE_SOURCE_DIR}/conf/config.ini config.ini COPYONLY)