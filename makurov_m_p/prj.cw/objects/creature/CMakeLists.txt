add_library(creature "creature.cpp" "include/creature/creature.h")
set_property(TARGET creature PROPERTY CXX_STANDARD 20)
target_link_libraries(creature PUBLIC imgui::imgui glfw glad::glad)

if(OpenMP_CXX_FOUND)
    target_link_libraries(creature PUBLIC OpenMP::OpenMP_CXX)
endif()

target_include_directories(creature PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)