include_directories(unity)
add_executable(runner test_digraph6.c test_graph6.c test_graph7.c test_sparse6.c utils.c unity/unity.c runner.c)
target_link_libraries(runner $<TARGET_OBJECTS:objlib> m)
