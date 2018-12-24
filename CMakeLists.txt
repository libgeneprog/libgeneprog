cmake_minimum_required(VERSION 3.9)
project(geneprog VERSION 0.0.1 DESCRIPTION "Genetic programming library")
add_library(geneprog SHARED
	src/cgp-data.c
)

set_target_properties(geneprog PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(geneprog PROPERTIES SOVERSION 0)
set_target_properties(geneprog PROPERTIES PUBLIC_HEADER 
	include/cgp-data.h
)

target_include_directories(geneprog PRIVATE include)

include(GNUInstallDirs)
install(TARGETS geneprog
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/geneprog)
