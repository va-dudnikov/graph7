include_directories(unity)

# Unity test-framework
add_library(unity unity/unity.c)
# General functions
add_library(tests utils.c)

list(
    APPEND
    TESTS_LIBS
    $<TARGET_OBJECTS:objlib>
    unity
    tests
    ${EXTERNAL_LIBS}
)

# digraph6 test
add_executable(test_digraph6 test_digraph6.c)
target_link_libraries(test_digraph6 ${TESTS_LIBS})
add_test(NAME digraph6 COMMAND test_digraph6)

# graph6 test
add_executable(test_graph6 test_graph6.c)
target_link_libraries(test_graph6 ${TESTS_LIBS})
add_test(NAME graph6 COMMAND test_graph6)

# sparse6 test
add_executable(test_sparse6 test_sparse6.c)
target_link_libraries(test_sparse6 ${TESTS_LIBS})
add_test(NAME sparse6 COMMAND test_sparse6)

# graph7 test
add_executable(test_graph7 test_graph7.c)
target_link_libraries(test_graph7 ${TESTS_LIBS})
add_test(NAME graph7 COMMAND test_graph7)
