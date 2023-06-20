# CMakeList.txt : CMake project for 2_month_project, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (cell_world "cell_world.cpp")

target_link_libraries(cell_world PRIVATE imgui::imgui glfw glad::glad OpenMP::OpenMP_CXX creature save_system UI scenario)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET cell_world PROPERTY CXX_STANDARD 20)
endif()

install(TARGETS cell_world RUNTIME)