add_executable(ArrayD_test ArrayD_test.cpp)
set_property(TARGET ArrayD_test PROPERTY CXX_STANDARD 20)
target_link_libraries(ArrayD_test arrayD)
add_test(NAME ArrayD_test COMMAND ArrayD_test)


