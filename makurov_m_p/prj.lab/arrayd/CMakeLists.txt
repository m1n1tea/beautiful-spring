add_library(arrayd  "include/arrayd/arrayd.hpp" "arrayd.cpp")
set_property(TARGET arrayd PROPERTY CXX_STANDARD 20)

target_include_directories(arrayd PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
install(TARGETS arrayd)
