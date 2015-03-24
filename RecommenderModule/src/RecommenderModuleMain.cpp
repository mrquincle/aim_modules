/**
 * @file RecommenderModuleMain.cpp
 * @brief Entry function for RecommenderModule
 *
 * This file is created at "Springfield Power Company". It is open-source software and part of "Project Bootstrap". 
 * This software is published under the State license (TR8OR).
 *
 * Copyright © 2015 Homer J. Simpson <chunkylover53@aol.com>
 *
 * @author                   Homer J. Simpson
 * @date                     Mar 23, 2015
 * @organisation             Springfield Power Company
 * @project                  Project Bootstrap
 */
 
#include <RecommenderModuleExt.h>

#include <stdlib.h>
#include <iostream>

using namespace rur;
using namespace std;

/**
 * Every module is a separate binary and hence has its own main method. It is recommended
 * to have a version of your code running without any middleware wrappers, so preferably
 * have this file and the RecommenderModule header and code in a separate "aim" directory.
 */
int main(int argc, char *argv[])  {
	RecommenderModuleExt *m = new RecommenderModuleExt();

	if (argc < 2) {
		std::cout << "Use an identifier as argument for this instance" << endl;
		return EXIT_FAILURE;
	}
	std::string identifier = argv[1];
	m->Init(identifier);

	do {
		m->Tick();
	} while (!m->Stop()); 

	delete m;

	return EXIT_SUCCESS;
}
