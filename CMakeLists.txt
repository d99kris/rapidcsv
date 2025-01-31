# Project
cmake_minimum_required(VERSION 3.14...3.22 FATAL_ERROR)
project(rapidcsv VERSION 1.0 LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 11)
if(MSVC)
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
  add_compile_options("/utf-8")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Wpedantic -Wshadow -Wpointer-arith \
                       -Wcast-qual -Wno-missing-braces -Wswitch-default -Wcast-align \
                       -Wunreachable-code -Wundef -Wuninitialized -Wold-style-cast \
                       -Wsign-conversion -Weffc++")
endif()

# Build type
set(DEFAULT_BUILD_TYPE "Release")
if((EXISTS "${CMAKE_SOURCE_DIR}/.svn") OR (EXISTS "${CMAKE_SOURCE_DIR}/.git"))
  set(DEFAULT_BUILD_TYPE "Debug")
endif()
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Using build type '${DEFAULT_BUILD_TYPE}' (default).")
  set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}")
else()
  message(STATUS "Using build type '${CMAKE_BUILD_TYPE}'.")
endif()

# Library
add_library(rapidcsv INTERFACE)
target_include_directories(rapidcsv INTERFACE src)

# Tests
option(RAPIDCSV_BUILD_TESTS "Build tests" OFF)
message(STATUS "Build tests: ${RAPIDCSV_BUILD_TESTS}")
if(RAPIDCSV_BUILD_TESTS)

  # Test init
  include(CTest)
  enable_testing()

  # Ccache
  find_program(CCACHE_PROGRAM ccache)
  if(CCACHE_PROGRAM)
    message(STATUS "Found ccache")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
  endif()

  # Enable codecvt tests if supported
  if (CMAKE_CXX_STANDARD VERSION_LESS "17")
    include(CheckIncludeFileCXX)
    check_include_file_cxx(codecvt HAS_CODECVT)
    if(HAS_CODECVT)
      target_compile_definitions(rapidcsv INTERFACE HAS_CODECVT)
    endif()
  else()
    message(STATUS "Skip check for C++ include codecvt - C++${CMAKE_CXX_STANDARD}")
  endif()

  if(CMAKE_BUILD_TYPE MATCHES "Release")
    # perf tests
    FILE(GLOB ptest_sources tests/ptest*.cpp)
    foreach(ptest_source ${ptest_sources})
      get_filename_component(ptest ${ptest_source} NAME_WE)
      add_executable(${ptest} ${ptest_source})
      add_test(NAME ${ptest} COMMAND "${PROJECT_BINARY_DIR}/${ptest}")
      target_link_libraries(${ptest} PUBLIC rapidcsv)
    endforeach(ptest_source ${ptest_sources})
  endif()

  if(CMAKE_BUILD_TYPE MATCHES "Debug")
    # Unit tests
    FILE(GLOB test_sources tests/test*.cpp)
    list(REMOVE_ITEM test_sources "${PROJECT_SOURCE_DIR}/tests/test040b.cpp")
    if(NOT HAS_CODECVT)
      list(REMOVE_ITEM
          test_sources 
           "${PROJECT_SOURCE_DIR}/tests/test057.cpp"
           "${PROJECT_SOURCE_DIR}/tests/test058.cpp"
           "${PROJECT_SOURCE_DIR}/tests/test059.cpp"
           "${PROJECT_SOURCE_DIR}/tests/test060.cpp"
           "${PROJECT_SOURCE_DIR}/tests/test085.cpp")
    endif()
    foreach(test_source ${test_sources})
      get_filename_component(test ${test_source} NAME_WE)
      FILE(GLOB sources tests/${test}*.cpp)
      add_executable(${test} ${sources})
      add_test(NAME ${test} COMMAND "${PROJECT_BINARY_DIR}/${test}")
      target_link_libraries(${test} PUBLIC rapidcsv)
    endforeach(test_source ${test_sources})

    # Examples
    FILE(GLOB example_sources examples/*.cpp)
    foreach(example_source ${example_sources})
      get_filename_component(example ${example_source} NAME_WE)
      add_executable(${example} ${example_source})
      target_link_libraries(${example} PUBLIC rapidcsv)
    endforeach(example_source ${example_sources})
  endif()

endif()

# Install / Uninstall targets for standalone rapidcsv build
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
  message(STATUS "Install/uninstall targets enabled.")

  # Install
  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/rapidcsv.h DESTINATION include)

  # Uninstall
  add_custom_target(uninstall COMMAND "${CMAKE_COMMAND}" -E remove "${CMAKE_INSTALL_PREFIX}/include/rapidcsv.h"
)
else()
  message(STATUS "Install/uninstall targets disabled.")
endif()
