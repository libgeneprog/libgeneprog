cmake_minimum_required(VERSION 3.9)

project(geneprog VERSION 0.1.1 DESCRIPTION "Genetic programming library")

# ____________________________
#    /src for building
# ____________________________
add_subdirectory(src)

# ____________________________
#    /test for testing
# ____________________________
enable_testing ()
add_subdirectory (test)

# ____________________________
#    stuff for installing
# ____________________________
include(GNUInstallDirs)
# Install headers
file(GLOB_RECURSE _HEADERS "include/*.h")
install(FILES ${_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/geneprog)
# Install the pkg-config file
# Set up the pkg-config file:
configure_file(geneprog.pc.in geneprog.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/geneprog.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

