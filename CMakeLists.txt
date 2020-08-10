# Project
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project("rapidcsv" VERSION 1.0 LANGUAGES CXX)


# Test init
include(CTest)
enable_testing()


add_library("${PROJECT_NAME}" INTERFACE)
add_library("rapidcsv::rapidcsv" ALIAS "${PROJECT_NAME}")


target_include_directories(
	"${PROJECT_NAME}"
	INTERFACE
		"${CMAKE_CURRENT_LIST_DIR}/src"
)


include(CheckIncludeFileCXX)
check_include_file_cxx(codecvt HAS_CODECVT)
if(HAS_CODECVT)
	target_compile_definitions(
		"${PROJECT_NAME}"
		INTERFACE
			"-DHAS_CODECVT"
	)
endif()


add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/examples")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/tests")
