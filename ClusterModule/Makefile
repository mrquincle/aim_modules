#!/bin/make

# This Makefile is only meant for convenience sake. Use "aimmake" to compile a Module. Do not change this Makefile, 
# because this will make updates and cross compilation more difficult for you. To change things in the build process
# you will have to use the CustomBuild.cmake file in the cmake build system.

include aim-config/config.aim
include aim-core/middleware_default.aim
-include aim-devel/middleware_local.aim

middleware_build=$(build_folder)/$(middleware)

all: 
	@mkdir -p $(middleware_build)
	cd $(middleware_build) && cmake $(CMAKE_FLAGS) ../..
	cd $(middleware_build) && make

clean:
	cd $(middleware_build) && make clean
	rm -f $(middleware_build)/CMakeCache.txt

