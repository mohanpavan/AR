add_executable(server server.cpp main.cpp)
target_link_libraries(server PRIVATE config_reader)
configure_file(${CMAKE_SOURCE_DIR}/config/config.ini ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
install(TARGETS server DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)