#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "liblo::liblo" for configuration "Debug"
set_property(TARGET liblo::liblo APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(liblo::liblo PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/liblo.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/liblo.dll"
  )

list(APPEND _cmake_import_check_targets liblo::liblo )
list(APPEND _cmake_import_check_files_for_liblo::liblo "${_IMPORT_PREFIX}/lib/liblo.lib" "${_IMPORT_PREFIX}/bin/liblo.dll" )

# Import target "liblo::oscdump" for configuration "Debug"
set_property(TARGET liblo::oscdump APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(liblo::oscdump PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/oscdump.exe"
  )

list(APPEND _cmake_import_check_targets liblo::oscdump )
list(APPEND _cmake_import_check_files_for_liblo::oscdump "${_IMPORT_PREFIX}/bin/oscdump.exe" )

# Import target "liblo::oscsend" for configuration "Debug"
set_property(TARGET liblo::oscsend APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(liblo::oscsend PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/oscsend.exe"
  )

list(APPEND _cmake_import_check_targets liblo::oscsend )
list(APPEND _cmake_import_check_files_for_liblo::oscsend "${_IMPORT_PREFIX}/bin/oscsend.exe" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
