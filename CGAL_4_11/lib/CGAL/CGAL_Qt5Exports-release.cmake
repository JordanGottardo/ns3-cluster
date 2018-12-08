#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "CGAL::CGAL_Qt5" for configuration "Release"
set_property(TARGET CGAL::CGAL_Qt5 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(CGAL::CGAL_Qt5 PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "/usr/lib/x86_64-linux-gnu/libmpfr.so;/usr/lib/x86_64-linux-gnu/libgmp.so;Qt5::OpenGL;Qt5::Svg;CGAL::CGAL;/usr/lib/x86_64-linux-gnu/libboost_thread.so;/usr/lib/x86_64-linux-gnu/libboost_system.so;/usr/lib/x86_64-linux-gnu/libboost_chrono.so;/usr/lib/x86_64-linux-gnu/libboost_date_time.so;/usr/lib/x86_64-linux-gnu/libboost_atomic.so;/usr/lib/x86_64-linux-gnu/libpthread.so;/usr/lib/x86_64-linux-gnu/libGLU.so;/usr/lib/x86_64-linux-gnu/libGL.so"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libCGAL_Qt5.so.13.0.1"
  IMPORTED_SONAME_RELEASE "libCGAL_Qt5.so.13"
  )

list(APPEND _IMPORT_CHECK_TARGETS CGAL::CGAL_Qt5 )
list(APPEND _IMPORT_CHECK_FILES_FOR_CGAL::CGAL_Qt5 "${_IMPORT_PREFIX}/lib/libCGAL_Qt5.so.13.0.1" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
