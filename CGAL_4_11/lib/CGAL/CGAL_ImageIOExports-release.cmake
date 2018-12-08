#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "CGAL::CGAL_ImageIO" for configuration "Release"
set_property(TARGET CGAL::CGAL_ImageIO APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(CGAL::CGAL_ImageIO PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "/usr/lib/x86_64-linux-gnu/libmpfr.so;/usr/lib/x86_64-linux-gnu/libgmp.so;CGAL::CGAL;/usr/lib/x86_64-linux-gnu/libboost_thread.so;/usr/lib/x86_64-linux-gnu/libboost_system.so;/usr/lib/x86_64-linux-gnu/libboost_chrono.so;/usr/lib/x86_64-linux-gnu/libboost_date_time.so;/usr/lib/x86_64-linux-gnu/libboost_atomic.so;/usr/lib/x86_64-linux-gnu/libpthread.so;/usr/lib/x86_64-linux-gnu/libz.so"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libCGAL_ImageIO.so.13.0.1"
  IMPORTED_SONAME_RELEASE "libCGAL_ImageIO.so.13"
  )

list(APPEND _IMPORT_CHECK_TARGETS CGAL::CGAL_ImageIO )
list(APPEND _IMPORT_CHECK_FILES_FOR_CGAL::CGAL_ImageIO "${_IMPORT_PREFIX}/lib/libCGAL_ImageIO.so.13.0.1" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
