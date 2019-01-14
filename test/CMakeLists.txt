# https://stackoverflow.com/a/14447765/978509
find_package(PkgConfig REQUIRED)
pkg_search_module(PKG_GENEPROG REQUIRED geneprog)
pkg_search_module(PKG_CUNIT REQUIRED cunit)

include_directories(${PKG_CUNIT_INCLUDE_DIRS}
                    ${PKG_GENEPROG_INCLUDE_DIRS})
file(GLOB_RECURSE _TEST_SOURCES "*.c")
#add_executable (Test test_runner.c ${_TEST_SOURCES})
add_executable (Test ${_TEST_SOURCES})
target_link_libraries (Test ${PKG_GENEPROG_LDFLAGS} ${PKG_CUNIT_LDFLAGS})

add_test (NAME GeneprogTest COMMAND Test)
