#
# @file binding.gyp
# @brief This file provides the configuration and building options for node-gyp
#
# This file is created at Almende B.V. It is open-source software and part of the Common
# Hybrid Agent Platform (CHAP). A toolbox with a lot of open-source tools, ranging from
# thread pools and TCP/IP components to control architectures and learning algorithms.
# This software is published under the GNU Lesser General Public license (LGPL).
#
# It is not possible to add usage restrictions to an open-source license. Nevertheless,
# we personally strongly object against this software being used by the military, in the
# bio-industry, for animal experimentation, or anything that violates the Universal
# Declaration of Human Rights.
#
# Copyright © 2012 Your Name <your@email.address>
#
# @author  ...
# @date    ...
# @company Almende B.V. (or your university, institute or company)
# @case    Artificial Intelligence Module
#
{
	"targets": [
		{
			"target_name": "ClusterModule",
			
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
				"../../aim-core/src/ClusterModule.cpp",
				"ClusterModuleNode.cc",
				"../../src/ClusterModuleExt.cpp"
			],
		}
	]
}
