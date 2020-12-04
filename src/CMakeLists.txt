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
add_library(${PROJECT_NAME}_shared SHARED $<TARGET_OBJECTS:objlib>)
add_library(${PROJECT_NAME}_static STATIC $<TARGET_OBJECTS:objlib>)

target_link_libraries(${PROJECT_NAME}_shared ${EXTERNAL_LIBS})

# Set same name for static and shared libraries
set_target_properties(${PROJECT_NAME}_shared PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME}_static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME}_static DESTINATION ${CMAKE_BINARY_DIR}/install/lib)
install(TARGETS ${PROJECT_NAME}_shared DESTINATION ${CMAKE_BINARY_DIR}/install/lib)
