# CMakeList.txt : CMake project for 2_month_project, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (cell_world "cell_world.cpp")

target_link_libraries(cell_world PRIVATE creature glfw imgui::imgui  save_system UI scenario)
if(OpenMP_CXX_FOUND)
    target_link_libraries(cell_world PUBLIC OpenMP::OpenMP_CXX)
endif()
set_property(TARGET cell_world PROPERTY CXX_STANDARD 17)

install(TARGETS cell_world RUNTIME)