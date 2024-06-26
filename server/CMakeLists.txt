add_executable(server server.cpp main.cpp)
target_link_libraries(server PRIVATE config_reader)
install(TARGETS server DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)