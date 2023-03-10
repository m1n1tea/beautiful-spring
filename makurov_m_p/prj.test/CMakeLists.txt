add_executable(rational_test rational_test.cpp)
set_property(TARGET rational_test PROPERTY CXX_STANDARD 20)
target_link_libraries(rational_test rational doctest::doctest)
add_test(NAME rational_test COMMAND rational_test)

add_executable(arrayd_test arrayd_test.cpp)
set_property(TARGET arrayd_test PROPERTY CXX_STANDARD 20)
target_link_libraries(arrayd_test arrayd)
add_test(NAME arrayd_test COMMAND arrayd_test)





