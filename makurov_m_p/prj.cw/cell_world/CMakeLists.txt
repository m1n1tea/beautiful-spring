# CMakeList.txt : CMake project for 2_month_project, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (cell_world "cell_world.cpp")

target_link_libraries(cell_world PRIVATE UI )
if(OpenMP_CXX_FOUND)
    target_link_libraries(cell_world PUBLIC OpenMP::OpenMP_CXX)
endif()
set_property(TARGET cell_world PROPERTY CXX_STANDARD 20)

if (WIN32)
    set(SUBSYSTEM_LINKER_OPTIONS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup>")
else()
    set(SUBSYSTEM_LINKER_OPTIONS "-mwindows")
endif()

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${SUBSYSTEM_LINKER_OPTIONS}")