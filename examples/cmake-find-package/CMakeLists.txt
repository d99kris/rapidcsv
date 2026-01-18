cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(cmake-find-package)
set(CMAKE_CXX_STANDARD 11)

find_package(rapidcsv REQUIRED)
add_executable(exprog3 src/exprog3.cpp)
target_link_libraries(exprog3 PRIVATE rapidcsv::rapidcsv)

install(TARGETS exprog3 DESTINATION bin)
add_custom_target(uninstall
  COMMAND "${CMAKE_COMMAND}" -E remove "${CMAKE_INSTALL_PREFIX}/bin/exprog2"
)
