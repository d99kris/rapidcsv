option(RAPIDCSV_ENABLE_TESTS "Build rapidcsv tests" ON)


#if(MSVC)
#	if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
#		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
#	else()
#		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
#	endif()
#else()
#	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Wpedantic -Wshadow -Wpointer-arith \
#                       -Wcast-qual -Wno-missing-braces -Wswitch-default -Wcast-align -Wunreachable-code \
#                       -Wundef -Wuninitialized")
#endif()



## Ccache
#find_program(CCACHE_PROGRAM ccache)
#if(CCACHE_PROGRAM)
#	message(STATUS "Found ccache")
#	set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
#endif()


# Test function rapidcsv_add_unit_test
function(rapidcsv_add_unit_test name)
	project("${PROJECT_NAME}-unit-test-${name}")
	add_executable("${PROJECT_NAME}" "${CMAKE_CURRENT_LIST_DIR}/${name}.cpp")

	set_target_properties("${PROJECT_NAME}" PROPERTIES CXX_STANDARD 11)

	target_link_libraries(
		"${PROJECT_NAME}"
		PUBLIC
			rapidcsv::rapidcsv
	)

	add_test(NAME "${name}" CONFIGURATIONS unit Debug COMMAND "${PROJECT_NAME}")
endfunction()


# Test function rapidcsv_add_perf_test
function(rapidcsv_add_perf_test name)
	project("${PROJECT_NAME}-perf-test-${name}")
	add_executable("${PROJECT_NAME}" "${CMAKE_CURRENT_LIST_DIR}/${name}.cpp")

	set_target_properties("${PROJECT_NAME}" PROPERTIES CXX_STANDARD 11)

	target_link_libraries(
		"${PROJECT_NAME}"
		PUBLIC
			rapidcsv::rapidcsv
	)

	configure_file(
		"${CMAKE_CURRENT_LIST_DIR}/msft.csv"
		"${CMAKE_CURRENT_BINARY_DIR}/msft.csv"
		COPYONLY
	)


	add_test(NAME "${name}" CONFIGURATIONS perf Release COMMAND "${PROJECT_NAME}")
endfunction()


# Unit tests
if(RAPIDCSV_ENABLE_TESTS)
	rapidcsv_add_unit_test(test001)
	rapidcsv_add_unit_test(test002)
	rapidcsv_add_unit_test(test003)
	rapidcsv_add_unit_test(test004)
	rapidcsv_add_unit_test(test005)
	rapidcsv_add_unit_test(test006)
	rapidcsv_add_unit_test(test007)
	rapidcsv_add_unit_test(test008)
	rapidcsv_add_unit_test(test009)
	rapidcsv_add_unit_test(test010)
	rapidcsv_add_unit_test(test011)
	rapidcsv_add_unit_test(test012)
	rapidcsv_add_unit_test(test013)
	rapidcsv_add_unit_test(test014)
	rapidcsv_add_unit_test(test015)
	rapidcsv_add_unit_test(test016)
	rapidcsv_add_unit_test(test017)
	rapidcsv_add_unit_test(test018)
	rapidcsv_add_unit_test(test019)
	rapidcsv_add_unit_test(test020)
	rapidcsv_add_unit_test(test021)
	rapidcsv_add_unit_test(test022)
	rapidcsv_add_unit_test(test023)
	rapidcsv_add_unit_test(test024)
	rapidcsv_add_unit_test(test025)
	rapidcsv_add_unit_test(test026)
	rapidcsv_add_unit_test(test027)
	rapidcsv_add_unit_test(test028)
	rapidcsv_add_unit_test(test029)
	rapidcsv_add_unit_test(test030)
	rapidcsv_add_unit_test(test031)
	rapidcsv_add_unit_test(test032)
	rapidcsv_add_unit_test(test033)
	rapidcsv_add_unit_test(test034)
	rapidcsv_add_unit_test(test035)
	rapidcsv_add_unit_test(test036)
	rapidcsv_add_unit_test(test037)
	rapidcsv_add_unit_test(test038)
	rapidcsv_add_unit_test(test039)
# TODO	add_executable(test040 tests/test040.cpp tests/test040b.cpp)
# TODO	add_test(NAME text040 CONFIGURATIONS unit COMMAND "${PROJECT_BINARY_DIR}/test040")
	rapidcsv_add_unit_test(test041)
	rapidcsv_add_unit_test(test042)
	rapidcsv_add_unit_test(test043)
	rapidcsv_add_unit_test(test044)
	rapidcsv_add_unit_test(test045)
	rapidcsv_add_unit_test(test046)
	rapidcsv_add_unit_test(test047)
	rapidcsv_add_unit_test(test048)
	rapidcsv_add_unit_test(test049)
	rapidcsv_add_unit_test(test050)
	rapidcsv_add_unit_test(test051)
	rapidcsv_add_unit_test(test052)
	rapidcsv_add_unit_test(test053)
	rapidcsv_add_unit_test(test054)
	rapidcsv_add_unit_test(test055)
	rapidcsv_add_unit_test(test056)
	if (HAS_CODECVT)
		rapidcsv_add_unit_test(test057)
		rapidcsv_add_unit_test(test058)
		rapidcsv_add_unit_test(test059)
		rapidcsv_add_unit_test(test060)
	endif()
	rapidcsv_add_unit_test(test061)
	rapidcsv_add_unit_test(test062)
	rapidcsv_add_unit_test(test063)
	rapidcsv_add_unit_test(test064)
	rapidcsv_add_unit_test(test065)
	rapidcsv_add_unit_test(test066)
	rapidcsv_add_unit_test(test067)
	rapidcsv_add_unit_test(test068)
	rapidcsv_add_unit_test(test069)
	rapidcsv_add_unit_test(test070)
	rapidcsv_add_unit_test(test071)
	rapidcsv_add_unit_test(test072)
	rapidcsv_add_unit_test(test073)
endif()


# perf tests
rapidcsv_add_perf_test(ptest001)
rapidcsv_add_perf_test(ptest002)