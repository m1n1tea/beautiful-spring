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

#if (WIN32)
#    # /ENTRY:mainCRTStartup keeps the same "main" function instead of requiring "WinMain"
#    set(SUBSYSTEM_LINKER_OPTIONS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
#else()
#    set(SUBSYSTEM_LINKER_OPTIONS "-mwindows")
#endif()

target_link_options(cell_world PRIVATE ${SUBSYSTEM_LINKER_OPTIONS})