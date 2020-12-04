include_directories(unity)

list(
	APPEND
	TESTS_SOURCES
	unity/unity.c
	utils.c
)

list(
	APPEND
	TESTS_LIBS
	$<TARGET_OBJECTS:objlib>
	${EXTERNAL_LIBS}
)

# digraph6 tests
add_executable(test_digraph6 test_digraph6.c ${TESTS_SOURCES})
target_link_libraries(test_digraph6 ${TESTS_LIBS})
add_test(NAME digraph6 COMMAND test_digraph6)

# graph6 tests
add_executable(test_graph6 test_graph6.c ${TESTS_SOURCES})
target_link_libraries(test_graph6 ${TESTS_LIBS})
add_test(NAME graph6 COMMAND test_graph6)

# sparse6 tests
add_executable(test_sparse6 test_sparse6.c ${TESTS_SOURCES})
target_link_libraries(test_sparse6 ${TESTS_LIBS})
add_test(NAME sparse6 COMMAND test_sparse6)

# sparse6 tests
add_executable(test_graph7 test_graph7.c ${TESTS_SOURCES})
target_link_libraries(test_graph7 ${TESTS_LIBS})
add_test(NAME graph7 COMMAND test_graph7)