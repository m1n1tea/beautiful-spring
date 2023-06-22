add_library( scenario "scenario.cpp" "include/scenario/scenario.h")
set_property(TARGET scenario PROPERTY CXX_STANDARD 20)
target_link_libraries(scenario PUBLIC creature)
target_include_directories(scenario PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)