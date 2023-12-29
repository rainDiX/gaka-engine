# Config file for the libepoxy package
# It defines the following variables
#  libepoxy_FOUND
#  libepoxy_DEFINITIONS
#  libepoxy_INCLUDE_DIRS
#  libepoxy_LIBRARIE

find_package(PkgConfig)
pkg_check_modules(PC_LIBEPOXY QUIET libepoxy)

if (PC_LIBEPOXY_FOUND)
    set(libepoxy_DEFINITIONS ${PC_LIBEPOXY_CFLAGS_OTHER})
endif ()

find_path(libepoxy_INCLUDE_DIRS
    NAMES epoxy/gl.h
    HINTS ${PC_LIBEPOXY_INCLUDEDIR} ${PC_LIBEPOXY_INCLUDE_DIRS}
    PATH_SUFFIXES libepoxy)

find_library(libepoxy_LIBRARY NAMES ${PC_LIBEPOXY_LIBRARIES}
    HINTS ${PC_LIBEPOXY_LIBDIR} ${PC_LIBEPOXY_LIBRARY_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(libepoxy DEFAULT_MSG
    libepoxy_LIBRARY libepoxy_INCLUDE_DIRS)

mark_as_advanced(libepoxy_INCLUDE_DIRS libepoxy_LIBRARY)

set(libepoxy_LIBRARIES libepoxy)

add_library(libepoxy SHARED IMPORTED)
set_property(TARGET libepoxy APPEND PROPERTY IMPORTED_LOCATION ${libepoxy_LIBRARY})
set_property(TARGET libepoxy APPEND PROPERTY IMPORTED_IMPLIB ${libepoxy_LIBRARY})
set_property(TARGET libepoxy APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${libepoxy_INCLUDE_DIRS})
