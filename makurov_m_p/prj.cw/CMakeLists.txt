find_package(imgui REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glad REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)

find_package(OpenMP)

if(OpenMP_CXX_FOUND)
    add_compile_definitions(OPEN_MP_FOUND=1)
else()
    add_compile_definitions(OPEN_MP_FOUND=0)
endif()

find_package(Doxygen)

if(DOXYGEN_FOUND)
set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
set(DOXYGEN_CONFIGURED_INPUT ${DOXYGEN_IN})

add_custom_target(doc ALL
COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONFIGURED_INPUT}
WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
COMMENT "Documentation with doxygen"
VERBATIM )
endif()

if(DOXYGEN_FOUND)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/documentation
DESTINATION .)
endif()

add_subdirectory ("include")
add_subdirectory ("cell_world")

install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/ DESTINATION bin FILES_MATCHING PATTERN "*test*" EXCLUDE PATTERN "*" )


