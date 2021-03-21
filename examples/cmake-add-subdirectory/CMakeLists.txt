cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(cmake-addsubdirectory)
set(CMAKE_CXX_STANDARD 11)

add_subdirectory(rapidcsv)

add_executable(exprog2 src/exprog2.cpp)
target_link_libraries(exprog2 PUBLIC rapidcsv)
install(TARGETS exprog2 DESTINATION bin)

add_custom_target(uninstall
  COMMAND "${CMAKE_COMMAND}" -E remove "${CMAKE_INSTALL_PREFIX}/bin/exprog2"
)
