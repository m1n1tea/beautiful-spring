add_library(arial_rus INTERFACE)

set_property(TARGET arial_rus PROPERTY CXX_STANDARD 20)

target_include_directories(arial_rus INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)