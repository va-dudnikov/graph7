# Source files
list(
	APPEND
	LIB_SOURCES
    graph6.c
    graph7.c
)

# Object library for once compile
add_library(objlib OBJECT ${LIB_SOURCES})

# Shared libraries need PIC
set_property(TARGET objlib PROPERTY POSITION_INDEPENDENT_CODE  1)

# Build shared and static libraries
add_library(graph7_shared SHARED $<TARGET_OBJECTS:objlib>)
add_library(graph7_static STATIC $<TARGET_OBJECTS:objlib>)

# Set same name for static and shared libraries
set_target_properties(graph7_shared PROPERTIES OUTPUT_NAME graph7)
set_target_properties(graph7_static PROPERTIES OUTPUT_NAME graph7)
