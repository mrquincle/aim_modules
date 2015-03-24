#
# @file binding.gyp
# @brief This file provides the configuration and building options for node-gyp
#
# This file is created at "Springfield Power Company". It is open-source software and part of "Project Bootstrap". 
# This software is published under the State license (TR8OR).
#
# Copyright © 2015 Homer J. Simpson <chunkylover53@aol.com>
#
# @author                    Homer J. Simpson
# @date                      Mar 23, 2015
# @organisation              Springfield Power Company
# @project                   Project Bootstrap
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
				"../../src/RecommenderModuleExt.cpp"
			],
		}
	]
}
