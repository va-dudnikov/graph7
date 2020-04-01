cmake_minimum_required(VERSION 3.0)

project(graph7 VERSION 0.2.0 LANGUAGES C)
include_directories(include)
add_subdirectory(src)

if (EXAMPLES)
	add_subdirectory(examples)
endif()

if (TESTS)
	add_subdirectory(tests)
endif()

if (DOCS)
	add_subdirectory(docs)
endif()

install(TARGETS ${PROJECT_NAME}_static DESTINATION lib)
install(TARGETS ${PROJECT_NAME}_shared DESTINATION lib)
install(
	FILES
	include/graph7/def.h
	include/graph7/errno.h
	include/graph7/graph6.h
	include/graph7/graph7.h
	DESTINATION
	include/graph7
)
