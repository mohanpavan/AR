add_library(config_reader INTERFACE)
target_include_directories(config_reader INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
install(FILES config.ini DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)