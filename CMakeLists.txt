# Project
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(rapidcsv VERSION 1.0 LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 11)
if(MSVC)
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Wpedantic -Wshadow -Wpointer-arith \
                       -Wcast-qual -Wno-missing-braces -Wswitch-default -Wcast-align -Wunreachable-code \
                       -Wundef -Wuninitialized")
endif()

# Tests
enable_testing()
include_directories("src")
set(LIB_HEADERS "src/rapidcsv.h")
set(TEST_HEADERS "tests/unittest.h")

add_executable(test001 tests/test001.cpp)
add_test(test001 "${PROJECT_BINARY_DIR}/test001")

add_executable(test002 tests/test002.cpp)
add_test(test002 "${PROJECT_BINARY_DIR}/test002")

add_executable(test003 tests/test003.cpp)
add_test(test003 "${PROJECT_BINARY_DIR}/test003")

add_executable(test004 tests/test004.cpp)
add_test(test004 "${PROJECT_BINARY_DIR}/test004")

add_executable(test005 tests/test005.cpp)
add_test(test005 "${PROJECT_BINARY_DIR}/test005")

add_executable(test006 tests/test006.cpp)
add_test(test006 "${PROJECT_BINARY_DIR}/test006")

add_executable(test007 tests/test007.cpp)
add_test(test007 "${PROJECT_BINARY_DIR}/test007")

add_executable(test008 tests/test008.cpp)
add_test(test008 "${PROJECT_BINARY_DIR}/test008")

add_executable(test009 tests/test009.cpp)
add_test(test009 "${PROJECT_BINARY_DIR}/test009")

add_executable(test010 tests/test010.cpp)
add_test(test010 "${PROJECT_BINARY_DIR}/test010")

add_executable(test011 tests/test011.cpp)
add_test(test011 "${PROJECT_BINARY_DIR}/test011")

add_executable(test012 tests/test012.cpp)
add_test(test012 "${PROJECT_BINARY_DIR}/test012")

add_executable(test013 tests/test013.cpp)
add_test(test013 "${PROJECT_BINARY_DIR}/test013")

add_executable(test014 tests/test014.cpp)
add_test(test014 "${PROJECT_BINARY_DIR}/test014")

add_executable(test015 tests/test015.cpp)
add_test(test015 "${PROJECT_BINARY_DIR}/test015")

add_executable(test016 tests/test016.cpp)
add_test(test016 "${PROJECT_BINARY_DIR}/test016")

add_executable(test017 tests/test017.cpp)
add_test(test017 "${PROJECT_BINARY_DIR}/test017")

add_executable(test018 tests/test018.cpp)
add_test(test018 "${PROJECT_BINARY_DIR}/test018")

add_executable(test019 tests/test019.cpp)
add_test(test019 "${PROJECT_BINARY_DIR}/test019")

add_executable(test020 tests/test020.cpp)
add_test(test020 "${PROJECT_BINARY_DIR}/test020")

add_executable(test021 tests/test021.cpp)
add_test(test021 "${PROJECT_BINARY_DIR}/test021")

add_executable(test022 tests/test022.cpp)
add_test(test022 "${PROJECT_BINARY_DIR}/test022")

add_executable(test023 tests/test023.cpp)
add_test(test023 "${PROJECT_BINARY_DIR}/test023")

add_executable(test024 tests/test024.cpp)
add_test(test024 "${PROJECT_BINARY_DIR}/test024")

add_executable(test025 tests/test025.cpp)
add_test(test025 "${PROJECT_BINARY_DIR}/test025")

add_executable(test026 tests/test026.cpp)
add_test(test026 "${PROJECT_BINARY_DIR}/test026")

add_executable(test027 tests/test027.cpp)
add_test(test027 "${PROJECT_BINARY_DIR}/test027")

add_executable(test028 tests/test028.cpp)
add_test(test028 "${PROJECT_BINARY_DIR}/test028")

add_executable(test029 tests/test029.cpp)
add_test(test029 "${PROJECT_BINARY_DIR}/test029")

add_executable(test030 tests/test030.cpp)
add_test(test030 "${PROJECT_BINARY_DIR}/test030")

add_executable(test031 tests/test031.cpp)
add_test(test031 "${PROJECT_BINARY_DIR}/test031")

add_executable(test032 tests/test032.cpp)
add_test(test032 "${PROJECT_BINARY_DIR}/test032")

add_executable(test033 tests/test033.cpp)
add_test(test033 "${PROJECT_BINARY_DIR}/test033")

add_executable(test034 tests/test034.cpp)
add_test(test034 "${PROJECT_BINARY_DIR}/test034")

add_executable(test035 tests/test035.cpp)
add_test(test035 "${PROJECT_BINARY_DIR}/test035")

add_executable(test036 tests/test036.cpp)
add_test(test036 "${PROJECT_BINARY_DIR}/test036")

add_executable(test037 tests/test037.cpp)
add_test(test037 "${PROJECT_BINARY_DIR}/test037")

add_executable(test038 tests/test038.cpp)
add_test(test038 "${PROJECT_BINARY_DIR}/test038")

add_executable(test039 tests/test039.cpp)
add_test(test039 "${PROJECT_BINARY_DIR}/test039")

add_executable(test040 tests/test040.cpp tests/test040b.cpp)
add_test(test040 "${PROJECT_BINARY_DIR}/test040")

add_executable(test041 tests/test041.cpp)
add_test(test041 "${PROJECT_BINARY_DIR}/test041")

add_executable(test042 tests/test042.cpp)
add_test(test042 "${PROJECT_BINARY_DIR}/test042")

add_executable(test043 tests/test043.cpp)
add_test(test043 "${PROJECT_BINARY_DIR}/test043")

add_executable(test044 tests/test044.cpp)
add_test(test044 "${PROJECT_BINARY_DIR}/test044")

add_executable(test045 tests/test045.cpp)
add_test(test045 "${PROJECT_BINARY_DIR}/test045")

add_executable(test046 tests/test046.cpp)
add_test(test046 "${PROJECT_BINARY_DIR}/test046")

add_executable(test047 tests/test047.cpp)
add_test(test047 "${PROJECT_BINARY_DIR}/test047")

add_executable(test048 tests/test048.cpp)
add_test(test048 "${PROJECT_BINARY_DIR}/test048")

add_executable(test049 tests/test049.cpp)
add_test(test049 "${PROJECT_BINARY_DIR}/test049")

# Examples
add_executable(example001 examples/example001.cpp)

