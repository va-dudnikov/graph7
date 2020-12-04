cmake_minimum_required(VERSION 3.9)
include(CheckLibraryExists)

project(graph7 VERSION 0.2.0 LANGUAGES C)

# We try to find sqrt in libm (in gcc and mingw), if we find it, we add a dependency.
# In MSVC, sqrt appears to be somewhere else. For now, I ignore this problem, but
# I will definitely figure it out!
set(EXTERNAL_LIBS "") # For libm and other (if necessary in the future)

# Check sqrt in libm
check_library_exists(m sqrt "" LIBM)

# If found then add into list
if(LIBM)
    list(APPEND EXTERNAL_LIBS "m")
endif()

enable_testing()

include_directories(include)
add_subdirectory(src)
add_subdirectory(tests)

# If need compile examples use -DEXAMPLES=1
if (EXAMPLES)
    add_subdirectory(examples)
endif()

install(
    FILES
    include/graph7/defs.h
    include/graph7/errno.h
    include/graph7/graph6.h
    include/graph7/graph7.h
    DESTINATION
    ${CMAKE_BINARY_DIR}/install/include/graph7
)
