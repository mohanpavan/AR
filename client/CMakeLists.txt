add_executable(client client.cpp main.cpp)
target_link_libraries(client PRIVATE config_reader)
configure_file(${CMAKE_SOURCE_DIR}/config/config.ini ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
install(TARGETS client DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
