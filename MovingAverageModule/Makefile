#!/bin/make

# This Makefile is only meant for convenience sake. Use "aimmake" to compile a Module. Do not change this Makefile, 
# because this will make updates and cross compilation more difficult for you. To change things in the build process
# you will have to use the CustomBuild.cmake file in the cmake build system.

include aim-config/config.aim

all: 
	@mkdir -p $(build_folder)
	cd $(build_folder) && cmake $(CMAKE_FLAGS) ../..
	cd $(build_folder) && make

clean:
	cd $(build_folder) && make clean
	rm -f $(build_folder)/CMakeCache.txt
