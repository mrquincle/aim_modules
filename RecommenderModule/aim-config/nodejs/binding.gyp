#
# @file binding.gyp
# @brief This file provides the configuration and building options for node-gyp
#
# This file is created at "Distributed Organisms B.V.". It is open-source software and part of "Commit P3". 
# This software is published under the LGPLv3.
#
# Copyright © 2015 Anne van Rossum <anne@dobos.nl>
#
# @author                    Anne van Rossum
# @date                      Mar 23, 2015
# @organisation              Distributed Organisms B.V.
# @project                   Commit P3 / Sensei
#
{
	"targets": [
		{
			"target_name": "RecommenderModule",
			
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
				"../../aim-core/src/RecommenderModule.cpp",
				"RecommenderModuleNode.cc",
				"../../src/RecommenderModuleExt.cpp",
				"../../src/Document.cpp",
				"../../src/Library.cpp"
			],
		}
	]
}
