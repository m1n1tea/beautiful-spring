add_library(matrix "matrix.cpp" "include/matrix/matrix.hpp")
target_link_libraries(matrix arrayd)
set_property(TARGET matrix PROPERTY CXX_STANDARD 20)

target_include_directories(matrix PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
install(TARGETS matrix)
