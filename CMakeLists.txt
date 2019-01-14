cmake_minimum_required(VERSION 3.9)
project(geneprog VERSION 0.0.1 DESCRIPTION "Genetic programming library")
add_library(geneprog SHARED
	src/cgp-data.c
	src/bst-data.c
)

set_target_properties(geneprog PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(geneprog PROPERTIES SOVERSION 0)

# We don't have a *single* header. We have multiple
#set_target_properties(geneprog PROPERTIES PUBLIC_HEADER "include/cgp-data.h include/gene.h")
file(GLOB_RECURSE _HEADERS "include/*.h")

# Set up the pkg-config file:
configure_file(geneprog.pc.in geneprog.pc @ONLY)

target_include_directories(geneprog PRIVATE include)

include(GNUInstallDirs)
install(TARGETS geneprog
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
#	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/geneprog)
install(FILES ${_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/geneprog)

#Install the pkg-config file
install(FILES ${CMAKE_BINARY_DIR}/geneprog.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

enable_testing ()
add_subdirectory (test)
