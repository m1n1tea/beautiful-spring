add_library(arrayd  INTERFACE)
set_property(TARGET arrayd PROPERTY CXX_STANDARD 20)
target_link_libraries(arrayd INTERFACE arrayt)

target_include_directories(arrayd INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
install(TARGETS arrayd)

