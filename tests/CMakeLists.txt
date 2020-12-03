include_directories(unity)

# Object library for tests
add_library(test_objlib OBJECT unity/unity.c utils.c)

list(
    APPEND
    TESTS_LIBS
    $<TARGET_OBJECTS:objlib>
    test_objlib
    ${EXTERNAL_LIBS}
)

add_executable(test_digraph6 test_digraph6.c)
add_executable(test_graph6 test_graph6.c)
add_executable(test_graph7 test_graph7.c)
add_executable(test_sparse6 test_sparse6.c)

target_link_libraries(test_digraph6 ${TESTS_LIBS})
target_link_libraries(test_graph6 ${TESTS_LIBS})
target_link_libraries(test_graph7 ${TESTS_LIBS})
target_link_libraries(test_sparse6 ${TESTS_LIBS})

add_test(NAME digraph6 COMMAND test_digraph6)
add_test(NAME graph6 COMMAND test_graph6)
add_test(NAME graph7 COMMAND test_graph7)
add_test(NAME sparse6 COMMAND test_sparse6)
