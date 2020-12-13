cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(cmake-fetchcontent)
set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
  rapidcsv
  GIT_REPOSITORY https://github.com/d99kris/rapidcsv.git
  GIT_TAG        v8.43
)
FetchContent_MakeAvailable(rapidcsv)

add_executable(exprog src/exprog.cpp)
target_link_libraries(exprog PUBLIC rapidcsv)
