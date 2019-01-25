add_library(geneprog SHARED
	bst-data.c
	cgp-data.c
	genepool.c
)

set_target_properties(geneprog PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(geneprog PROPERTIES SOVERSION 0)

target_include_directories(geneprog PRIVATE ../include)

# Need to handle library install here due to subdirectory issue 
# https://stackoverflow.com/questions/34443128/cmake-install-targets-in-subdirectories
include(GNUInstallDirs)
# Install library itself
install(TARGETS geneprog
	LIBRARY 
	DESTINATION ${CMAKE_INSTALL_LIBDIR}
	#COMPONENT library
)

# Since this builds the library, it should also build the library package
# The end result should be 2 packages, similar to how most *nix stuff is:
# libgeneprog - the shared library for linking
# libgeneprog-dev - the headers for developing

SET(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "libgeneprog")
set(CPACK_PACKAGE_VENDOR "libgeneprog")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Richard Wardin")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Genetic programming library")
#set(CPACK_COMPONENTS_ALL library)

include(CPack)
