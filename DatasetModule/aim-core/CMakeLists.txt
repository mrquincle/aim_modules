##########################################################################################
#               _/\/\/\/\/\____/\/\/\/\__/\/\/\/\/\____/\/\_______                       #
#               _/\/\____/\/\____/\/\____/\/\____/\/\__/\/\_______                       #
#               _/\/\/\/\/\______/\/\____/\/\____/\/\__/\/\_______                       #
#               _/\/\__/\/\______/\/\____/\/\____/\/\__/\/\_______                       #
#               _/\/\____/\/\__/\/\/\/\__/\/\/\/\/\____/\/\/\/\/\_                       #
#               __________________________________________________                       #
##########################################################################################
# Part of RIDL, the RUR IDL builder which generates modules for middlewares, including
# lightweight (robotic) middleware like YARP.
# This makefile is part of a cmake build system. The automatically generated code falls 
# under the Lesser General Public License (LGPL GNU v3).

# Author:	 Anne C. van Rossum (Distributed Organisms B.V.)
# Date: 	 Mar 19, 2014
#
# ASCII Art generated by: http://www.network-science.de/ascii/ with font "ticks".
#
# Copyright © 2014 Anne C. van Rossum <anne@dobots.nl>
##########################################################################################
# Check "man cmake" for very detailed information on this build system
##########################################################################################

IF(WIN32)
	cmake_minimum_required(VERSION 2.6)
ELSE(WIN32)
	cmake_minimum_required(VERSION 2.4)
ENDIF(WIN32)

# The directory with all the FindXXX modules
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/aim-core/cmake/")

MESSAGE(STATUS "Search for FindX files in ${CMAKE_SOURCE_DIR}/aim-core/cmake")

# The macro below forces the build directory to be different from source directory:
INCLUDE( MacroEnsureOutOfSourceBuild )
MACRO_ENSURE_OUT_OF_SOURCE_BUILD ( "AUTHOR_WARNING Requires an out of source build." )

# Build options
OPTION(BUILD_YARP      "Build module with YARP" OFF)
OPTION(BUILD_ROS       "Build module with ROS" OFF)
OPTION(BUILD_ZEROMQ    "Build module with ZeroMQ" OFF)
OPTION(BUILD_NODEJS    "Build module with NodeJS" OFF)
OPTION(BUILD_STANDARD  "Default build option" ON)

# Check again if one of the options has turned on from one of the .cmake files
IF (BUILD_YARP OR BUILD_ROS OR BUILD_ZEROMQ OR BUILD_NODEJS)
	SET(BUILD_STANDARD OFF)
ENDIF()

# Load config file from user if it exists
IF (EXISTS "${CMAKE_SOURCE_DIR}/aim-devel/middleware_local.aim")
	SET(CONFIG_FILE "${CMAKE_SOURCE_DIR}/aim-devel/middleware_local.aim")
ELSE()
	SET(CONFIG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/middleware_default.aim")
ENDIF()

FILE(STRINGS ${CONFIG_FILE} ConfigContents)
FOREACH(Item ${ConfigContents})
	STRING(REGEX REPLACE "^[ ]+" "" UnindentedItem ${Item})
	# only keep everything before # sign
	STRING(REGEX MATCH "^[^#]+" NameAndValue ${UnindentedItem})
	IF (NOT "${NameAndValue}" STREQUAL "")
		# name is thing before = sign, value comes after
		STRING(REGEX MATCH "^[^=]+" Name ${NameAndValue})
		STRING(REPLACE "${Name}=" "" Value ${NameAndValue})
		IF (NOT "${Name}" STREQUAL "" AND NOT "${Value}" STREQUAL "")
			MESSAGE(STATUS "Set ${Name}=${Value}")
#			SET(${Name} "${Value}")
			IF ("${Name}" STREQUAL "middleware")
				SET(MIDDLEWARE_NAME "${Value}")
			ENDIF()				
		ENDIF()
	ENDIF()
ENDFOREACH()

IF (NOT "${MIDDLEWARE_NAME}" STREQUAL "")
	SET(MIDDLEWARE "${MIDDLEWARE_NAME}")
	SET(BUILD_STANDARD OFF)
ELSE()
	MESSAGE(STATUS "Can not determine middleware to run")
ENDIF()

MESSAGE(STATUS "Read \"/etc/rur/backends.cmake\" configuration file")
INCLUDE(/etc/rur/backends.cmake)
MESSAGE(STATUS "Read (optionally) \"~/.rur/backends_path.cmake\" configuration file")
INCLUDE(~/.rur/backends_path.cmake OPTIONAL)
IF(NOT (IS_DIRECTORY ${BACKENDS_PATH}))
	MESSAGE(FATAL_ERROR "No backends path found. Check the contents of the aforementioned configuration files.")
ENDIF()

# Set commands for header generation
IF (BUILD_YARP)
	SET(MIDDLEWARE "yarp")
ENDIF ()

IF (BUILD_ROS)
	SET(MIDDLEWARE "ros")
ENDIF ()

IF (BUILD_ZEROMQ)
	SET(MIDDLEWARE "zeromq")
ENDIF ()

IF (BUILD_NODEJS)
	SET(MIDDLEWARE "nodejs")
ENDIF ()

IF (BUILD_STANDARD)
	SET(MIDDLEWARE "standard")
ENDIF ()

# Set files to be used or generated
SET(IDL_FILE "${CMAKE_SOURCE_DIR}/aim-config/${PROJECT_NAME}.idl")
SET(OUT_NAME "${PROJECT_NAME}")
SET(OUT_PATH "${CMAKE_SOURCE_DIR}/aim-core")

# Actually generate the source files
FIND_PROGRAM(RUR_BUILDER rur-builder)

IF (RUR_BUILDER)
	file(MAKE_DIRECTORY "${OUT_PATH}/src")
	file(MAKE_DIRECTORY "${OUT_PATH}/inc")

	MESSAGE(STATUS "Run the rur-builder with arguments: -i ${IDL_FILE} -p ${BACKENDS_PATH} -o \"${OUT_PATH}/inc/${OUT_NAME}.h\" -b \"rur_main_head\" -m ${MIDDLEWARE}")
	EXECUTE_PROCESS(COMMAND rur-builder -i ${IDL_FILE} -p ${BACKENDS_PATH} -o "${OUT_PATH}/inc/${OUT_NAME}.h" -b "rur_main_head" -m ${MIDDLEWARE} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} RESULT_VARIABLE RUR_RESULT )
	
	MESSAGE(STATUS "Run the rur-builder with arguments: -i ${IDL_FILE} -p ${BACKENDS_PATH} -o \"${OUT_PATH}/src/${OUT_NAME}.cpp\"  -b \"rur_main_impl\" -m ${MIDDLEWARE}")
	EXECUTE_PROCESS(COMMAND rur-builder -i ${IDL_FILE} -p ${BACKENDS_PATH} -o "${OUT_PATH}/src/${OUT_NAME}.cpp"  -b "rur_main_impl" -m ${MIDDLEWARE} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} RESULT_VARIABLE RUR_RESULT )

	IF(EXISTS "${OUT_PATH}/.relay")
		MESSAGE(STATUS "Run the rur-builder with arguments: -i ${IDL_FILE} -p ${BACKENDS_PATH} -o \"${CMAKE_SOURCE_DIR}/src/${OUT_NAME}Ext.cpp\" -b \"rur_main_relay\" -m ${MIDDLEWARE}")
		EXECUTE_PROCESS(COMMAND rur-builder -i ${IDL_FILE} -p ${BACKENDS_PATH} -o "${CMAKE_SOURCE_DIR}/src/${OUT_NAME}Ext.cpp" -b "rur_main_relay" -m ${MIDDLEWARE} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} RESULT_VARIABLE RUR_RESULT )
	ENDIF()	

# TODO: should be checked 3 times
	IF (NOT "${RUR_RESULT}" STREQUAL "0")
		MESSAGE(FATAL_ERROR "The rur-builder did not run correctly! ${RUR_RESULT}\n "
			"The rur-builder did probably run, so it might be an error in this specific backend for ${MIDDLEWARE}. "
			"Please, submit an issue at https://github.com/dobots/aimtools. "
			"And, if you don't mind, attach the output of aimdebug to that issue. "
			"Usage: aimdebug ${PROJECT_NAME}. ")
	ENDIF ()

ENDIF ()


# Write the (backend) configuration
#MESSAGE(STATUS "Generate aim/Config.h file, that you can use in the code to know which middleware is in use")
#CONFIGURE_FILE( ${CMAKE_SOURCE_DIR}/aim/config.h.cmake ${CMAKE_SOURCE_DIR}/aim/inc/Config.h )

# Find all cmake packages
IF (${MIDDLEWARE} STREQUAL "yarp")
	MESSAGE(STATUS "YARP build, useful for YARP middleware")
	FIND_PACKAGE(YARP REQUIRED)
	INCLUDE_DIRECTORIES(${YARP_INCLUDE_DIRS} PARENT_SCOPE)
	SET(LIBS ${LIBS} ${YARP_LIBRARIES} -lrt -lpthread)
ENDIF()

IF (${MIDDLEWARE} STREQUAL "ros")
	MESSAGE(STATUS "The project source dir: ${PROJECT_SOURCE_DIR}")
	IF ("$ENV{ROS_ROOT}" STREQUAL "")
		MESSAGE(FATAL_ERROR "Requires env. variable $ROS_ROOT. Run for example: \"source /opt/ros/groovy/setup.bash\".")
	ENDIF()
	MESSAGE(STATUS "ROS build, useful for ROS middleware")
	
	# See http://www.ros.org/wiki/catkin/CMakeLists.txt
	
	find_package(catkin REQUIRED COMPONENTS roscpp message_generation std_msgs)

	add_message_files(DIRECTORY ../aim-config/ros/msg)
	
	generate_messages(DEPENDENCIES std_msgs)
	
#	catkin_package(CATKIN_DEPENDS message_runtime std_msgs)
	
#	ADD_DEPENDENCIES(${PROJECT_NAME} ${PROJECT_NAME}_gencpp)
	
	SET(INCLUDE_DIRS ${catkin_INCLUDE_DIRS})
	MESSAGE(STATUS "catkin includes: ${catkin_INCLUDE_DIRS}")
	SET(INCLUDE_DIRS "${INCLUDE_DIRS};${CATKIN_DEVEL_PREFIX}/include")
	MESSAGE(STATUS "catkin includes: ${CATKIN_DEVEL_PREFIX}/include")

	SET(LIBS ${LIBS} ${catkin_LIBRARIES})
	MESSAGE(STATUS "catkin libs: ${catkin_LIBRARIES}")

	SET(PARENT_INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)
ENDIF()

IF (${MIDDLEWARE} STREQUAL "zeromq")
	MESSAGE(STATUS "ZeroMQ build, useful for ZeroMQ message queues")
	FIND_PACKAGE(ZMQ REQUIRED)
	INCLUDE_DIRECTORIES(${ZMQ_INCLUDE_DIRS})
	SET(LIBS ${LIBS} ${ZMQ_LIBRARIES})

	FIND_PACKAGE(JsonSpirit REQUIRED)
	INCLUDE_DIRECTORIES(${JSON_SPIRIT_INCLUDE_DIRS})
	SET(LIBS ${LIBS} ${JSON_SPIRIT_LIBRARIES})

	# Currently tacitly assuming rt is always there...
	SET(LIBS ${LIBS} rt)
ENDIF()

IF (${MIDDLEWARE} STREQUAL "nodejs")
	MESSAGE("[*] NodeJS build, compiles a NodeJS addon")
	MESSAGE("[*] For now: use node-gyp to build")
ENDIF()

# Source directory
SET(SOURCE_DIR "src")
SET(INCLUDE_DIR "inc")

# Set LIBS to PARENT_SCOPE OR LIBS here will not be "seen" by the parent CMakeLists.txt file
SET(LIBS ${LIBS} PARENT_SCOPE)

# Warning for user when standard build
IF (BUILD_STANDARD)
	MESSAGE(WARNING "This is the \"standard\" build option. This means that there is no information coming in or over the defined ports. The functions generated from the interface description file are dummies. You will need to adjust it to actually use your code functionally, but you can use this standard build already to make sure that everything compiles fine.")
ENDIF (BUILD_STANDARD)

# Search for functional source code
FILE(GLOB aim_source ${SOURCE_DIR}/*.cpp ${SOURCE_DIR}/*.cc ${SOURCE_DIR}/*.c)
FILE(GLOB aim_header ${INCLUDE_DIR}/*.hpp ${INCLUDE_DIR}/*.hh ${INCLUDE_DIR}/*.h)

SET(AIM_HEADERS ${aim_header} PARENT_SCOPE)
SET(AIM_SOURCES ${aim_source} PARENT_SCOPE)

