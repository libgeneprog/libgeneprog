add_library(geneprog SHARED
	cgp-data.c
	bst-data.c
)

set_target_properties(geneprog PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(geneprog PROPERTIES SOVERSION 0)

target_include_directories(geneprog PRIVATE ../include)

# Need to handle library install here due to subdirectory issue 
# https://stackoverflow.com/questions/34443128/cmake-install-targets-in-subdirectories
include(GNUInstallDirs)
# Install library itself
install(TARGETS geneprog
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
