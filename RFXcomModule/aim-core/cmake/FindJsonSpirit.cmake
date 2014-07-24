# - Try to find JSON spirit library based on boost
# - On Ubuntu it's part of libjson-spirit-dev, feel free to adjust this file to cover
#   a wider range of operating systems
# Once done, this will define
#
#  JSON_SPIRIT_FOUND - system has JSON spirit library
#  JSON_SPIRIT_INCLUDE_DIRS - the JSON spirit include directories
#  JSON_SPIRIT_LIBRARIES - link these to use the JSON spirit library 

#include(LibFindMacros)

if (JSON_SPIRIT_LIBRARIES AND JSON_SPIRIT_INCLUDE_DIRS)
	# in cache already
	set(JSON_SPIRIT_FOUND TRUE)
else (JSON_SPIRIT_LIBRARIES AND JSON_SPIRIT_INCLUDE_DIRS)
	# Include dir
	find_path(JSON_SPIRIT_INCLUDE_DIR
		NAMES json_spirit.h
		PATHS 
		${JSON_SPIRIT_PKGCONF_INCLUDE_DIRS}
		~/Library/Frameworks
		/Library/Frameworks
		/usr/local
		/usr
		/sw # Fink
		/opt/local # DarwinPorts
		/opt/csw # Blastwave
		/opt
		/usr/freeware/include
		PATH_SUFFIXES
		include
		)

	# Finally the library itself
	find_library(JSON_SPIRIT_LIBRARY
		NAMES json_spirit
		PATHS 
		${JSON_SPIRIT_PKGCONF_LIBRARY_DIRS}
		/usr/local
		/usr
		PATH_SUFFIXES
		Build/lib
		Debug/lib
		Release/lib
		lib  
		lib64
		)

	# NOTE: Singular variables for this library, plural for libraries this lib depends on.
	set(JSON_SPIRIT_INCLUDE_DIRS ${JSON_SPIRIT_INCLUDE_DIR})
	set(JSON_SPIRIT_LIBRARIES ${JSON_SPIRIT_LIBRARY})
  
	include(FindPackageHandleStandardArgs)
	find_package_handle_standard_args(JSON_SPIRIT DEFAULT_MSG JSON_SPIRIT_LIBRARIES JSON_SPIRIT_INCLUDE_DIRS)

	# mark as advanced in cmake-gui or ccmake
	mark_as_advanced(JSON_SPIRIT_INCLUDE_DIR JSON_SPIRIT_LIBRARY)
	mark_as_advanced(JSON_SPIRIT_INCLUDE_DIRS JSON_SPIRIT_LIBRARIES)

	#libfind_process(JSON_SPIRIT)
endif (JSON_SPIRIT_LIBRARIES AND JSON_SPIRIT_INCLUDE_DIRS)

