add_library(arrayD  "arrayD/arrayD.hpp" "arrayD.cpp")
set_property(TARGET arrayD PROPERTY CXX_STANDARD 20)

target_include_directories(arrayD PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
