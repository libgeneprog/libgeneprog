# https://stackoverflow.com/a/14447765/978509
find_package(PkgConfig REQUIRED)
#pkg_search_module(PKG_GENEPROG REQUIRED geneprog)
pkg_search_module(PKG_CUNIT REQUIRED cunit)
#
#include_directories(${PKG_CUNIT_INCLUDE_DIRS}
#                    ${PKG_GENEPROG_INCLUDE_DIRS})
include_directories(${PKG_CUNIT_INCLUDE_DIRS})

file(GLOB_RECURSE _TEST_SOURCES "*.c")
# Need to include library sources/headers:
file(GLOB_RECURSE _LIB_SOURCES "../src/*.c")
#add_executable (Test test_runner.c ${_TEST_SOURCES})
#add_executable (Test test_runner.c ${_TEST_SOURCES} ${_LIB_SOURCES})
string(CONCAT _ALL_SOURCES ${_TEST_SOURCES} ${_LIB_SOURCES})
#add_executable (Test ${_TEST_SOURCES})
#add_executable (geneprogtest ${_ALL_SOURCES})
add_executable (geneprogtest ${_TEST_SOURCES} ${_LIB_SOURCES})
message( ${_ALL_SOURCES} )
#target_link_libraries (Test ${PKG_GENEPROG_LDFLAGS} ${PKG_CUNIT_LDFLAGS})
target_link_libraries (geneprogtest ${PKG_CUNIT_LDFLAGS})
target_include_directories(geneprogtest PRIVATE ../include)

add_test (NAME GeneprogTest COMMAND geneprogtest)


