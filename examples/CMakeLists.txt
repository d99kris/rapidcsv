# Examples
option(RAPIDCSV_ENABLE_EXAMPLES "Build rapidcsv examples" ON)


function(rapidcsv_add_example name)
	project("${PROJECT_NAME}-example-${name}")
	add_executable("${PROJECT_NAME}" "${CMAKE_CURRENT_LIST_DIR}/${name}.cpp")

	set_target_properties("${PROJECT_NAME}" PROPERTIES CXX_STANDARD 11)

	target_link_libraries(
		"${PROJECT_NAME}"
		PUBLIC
			rapidcsv::rapidcsv
	)
endfunction()


if (RAPIDCSV_ENABLE_EXAMPLES)
	rapidcsv_add_example("ex001")
	rapidcsv_add_example("ex002")
	rapidcsv_add_example("ex003")
	rapidcsv_add_example("ex004")
	rapidcsv_add_example("ex005")
	rapidcsv_add_example("ex006")
	rapidcsv_add_example("ex007")
	rapidcsv_add_example("ex008")
	rapidcsv_add_example("ex009")
endif()
