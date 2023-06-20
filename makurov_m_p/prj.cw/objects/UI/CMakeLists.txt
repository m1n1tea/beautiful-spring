add_library( UI "UI.cpp" "include/UI/UI.h")
set_property(TARGET UI PROPERTY CXX_STANDARD 20)
target_link_libraries(UI PRIVATE imgui::imgui creature save_system scenario)
target_include_directories(UI PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)