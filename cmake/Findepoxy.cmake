# Config file for the libepoxy package
# It defines the following variables
#  libepoxy_FOUND
#  libepoxy_DEFINITIONS
#  libepoxy_INCLUDE_DIRS
#  libepoxy_LIBRARIE

find_package(PkgConfig QUIET)
pkg_check_modules(PC_epoxy QUIET epoxy)

set(epoxy_VERSION ${PC_epoxy_VERSION})
set(epoxy_DEFINITIONS ${PC_epoxy_CFLAGS})

find_path(epoxy_INCLUDE_DIRS
    NAMES epoxy/gl.h
    HINTS ${PC_epoxy_INCLUDEDIR} ${PC_epoxy_INCLUDE_DIRS}
)

find_library(epoxy_LIBRARIES
    NAMES epoxy
    HINTS ${PC_epoxy_LIBDIR} ${PC_epoxy_LIBRARY_DIRS}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(epoxy
    FOUND_VAR epoxy_FOUND
    REQUIRED_VARS epoxy_LIBRARIES epoxy_INCLUDE_DIRS
    VERSION_VAR epoxy_VERSION
)

if (epoxy_FOUND AND NOT TARGET epoxy::epoxy)
    add_library(epoxy::epoxy UNKNOWN IMPORTED)
    set_target_properties(epoxy::epoxy PROPERTIES
        IMPORTED_LOCATION "${epoxy_LIBRARIES}"
        INTERFACE_COMPILE_OPTIONS "${epoxy_DEFINITIONS}"
        INTERFACE_INCLUDE_DIRECTORIES "${epoxy_INCLUDE_DIRS}"
    )
endif()
