add_library( UI "UI.cpp" "include/UI/UI.h")
set_property(TARGET UI PROPERTY CXX_STANDARD 20)
target_link_libraries(UI PUBLIC  creature save_system scenario glfw imgui::imgui)
target_include_directories(UI PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)