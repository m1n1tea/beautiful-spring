add_library(save_system "save_system.cpp" "include/save_system/save_system.h")
set_property(TARGET save_system PROPERTY CXX_STANDARD 20)
target_include_directories(save_system PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)