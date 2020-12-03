add_executable(example1 example1.c)
add_executable(example2 example2.c)

target_link_libraries(example1 $<TARGET_OBJECTS:objlib> m)
target_link_libraries(example2 $<TARGET_OBJECTS:objlib> m)
