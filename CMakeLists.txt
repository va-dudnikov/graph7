cmake_minimum_required(VERSION 3.1)

project(graph7 VERSION 0.2.0 LANGUAGES C CXX)

include_directories(include)
add_subdirectory(src)

if (EXAMPLES)
    add_subdirectory(examples)
endif()

if (TESTS)
    add_subdirectory(tests)
endif()

install(TARGETS ${PROJECT_NAME}_static DESTINATION /lib)
install(TARGETS ${PROJECT_NAME}_shared DESTINATION /lib)
install(
    FILES
    include/graph7/defs.h
    include/graph7/errno.h
    include/graph7/graph6.h
    include/graph7/graph7.h
    DESTINATION
    ${CMAKE_BINARY_DIR}/include/graph7
)
