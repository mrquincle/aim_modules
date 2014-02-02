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
# @date                      Jan 31, 2014
# @organisation              Distributed Organisms B.V.
# @project                   Robotic Suite
#
{
	"targets": [
		{
			"target_name": "NaiveBayesModule",
			
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
				"../../aim-core/src/NaiveBayesModule.cpp",
				"NaiveBayesModuleNode.cc",
				"../../src/NaiveBayesModuleExt.cpp"
			],
		}
	]
}
