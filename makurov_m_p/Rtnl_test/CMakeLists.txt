add_executable(rational_test rational_test.cpp)
add_test(NAME rational_test COMMAND rational_test)
target_link_libraries(rational_test rational doctest::doctest)


