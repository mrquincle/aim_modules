#
# @file binding.gyp
# @brief This file provides the configuration and building options for node-gyp
#
# This file is created at "Distributed Organisms B.V.". It is open-source software and part of "Crownstone". 
# This software is published under the GNU Lesser General Public License license (LGPLv3).
#
# Copyright © 2015 Anne van Rossum <anne@dobots.nl>
#
# @author                    Anne van Rossum
# @date                      Apr  9, 2015
# @organisation              Distributed Organisms B.V.
# @project                   Crownstone
#
{
	"targets": [
		{
			"target_name": "RotateModule",
			
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
				"../../aim-core/src/RotateModule.cpp",
				"RotateModuleNode.cc",
				"../../src/RotateModuleExt.cpp"
			],
		}
	]
}
