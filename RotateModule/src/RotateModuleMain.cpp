/**
 * @file RotateModuleMain.cpp
 * @brief Entry function for RotateModule
 *
 * This file is created at "Distributed Organisms B.V.". It is open-source software and part of "Crownstone". 
 * This software is published under the GNU Lesser General Public License license (LGPLv3).
 *
 * Copyright © 2015 Anne van Rossum <anne@dobots.nl>
 *
 * @author                   Anne van Rossum
 * @date                     Apr  9, 2015
 * @organisation             Distributed Organisms B.V.
 * @project                  Crownstone
 */
 
#include <RotateModuleExt.h>

#include <stdlib.h>
#include <iostream>

using namespace rur;
using namespace std;

/**
 * Every module is a separate binary and hence has its own main method. It is recommended
 * to have a version of your code running without any middleware wrappers, so preferably
 * have this file and the RotateModule header and code in a separate "aim" directory.
 */
int main(int argc, char *argv[])  {
	RotateModuleExt *m = new RotateModuleExt();

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
