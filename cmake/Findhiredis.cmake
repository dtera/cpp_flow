# Try to find hiredis
# Once done, this will define
#
# Hiredis_FOUND        - system has hiredis
# Hiredis_INCLUDE_DIRS - hiredis include directories
# Hiredis_LIBRARIES    - libraries need to use hiredis
set(Hiredis_ROOT_DIR /usr/local/opt/hiredis /opt/homebrew/opt/hiredis)

if(Hiredis_INCLUDE_DIRS AND Hiredis_LIBRARIES)
    set(Hiredis_FIND_QUIETLY TRUE)
else()
    find_path(
            Hiredis_INCLUDE_DIR
            NAMES Hiredis
            HINTS ${Hiredis_ROOT_DIR}
            PATH_SUFFIXES include)

    find_library(
            Hiredis_LIBRARY
            NAMES Hiredis
            HINTS ${Hiredis_ROOT_DIR}
            PATH_SUFFIXES ${CMAKE_INSTALL_LIBDIR})

    set(Hiredis_INCLUDE_DIRS ${Hiredis_INCLUDE_DIR})
    set(Hiredis_LIBRARIES ${Hiredis_LIBRARY})

    include (FindPackageHandleStandardArgs)
    find_package_handle_standard_args(Hiredis DEFAULT_MSG Hiredis_LIBRARY Hiredis_INCLUDE_DIR)

    mark_as_advanced(Hiredis_LIBRARY Hiredis_INCLUDE_DIR)
endif()