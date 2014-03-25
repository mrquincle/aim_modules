#
# @file binding.gyp
# @brief This file provides the configuration and building options for node-gyp
#
# This file is created at "Distributed Organisms B.V.". It is open-source software and part of "Robotic Suite". 
# This software is published under the GNU General Lesser Public license (LGPLv3).
#
# Copyright © 2014 Anne C. van Rossum <anne@dobots.nl>
#
# @author                    Anne C. van Rossum
# @date                      Mar 24, 2014
# @organisation              Distributed Organisms B.V.
# @project                   Robotic Suite
#
{
	"targets": [
		{
			"target_name": "LDAModule",
			
			"include_dirs": [
				"../../inc",
				"../../aim-core/inc"
				
			],
			
			"dependencies":[
			],
			
			"cflags": [
			],
			
			"libraries": [
			],
			
			"ldflags": [
				"-pthread",
			],
			
			"sources":[
				"../../aim-core/src/LDAModule.cpp",
				"LDAModuleNode.cc",
				"../../src/LDAModuleExt.cpp"
			],
		}
	]
}
