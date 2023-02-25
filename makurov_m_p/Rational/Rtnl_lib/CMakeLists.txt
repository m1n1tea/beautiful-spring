add_library(rational "Rational.cpp" "Rational.hpp")
set_property(TARGET rational PROPERTY CXX_STANDARD 20)

target_include_directories(rational PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
