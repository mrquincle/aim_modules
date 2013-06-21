/**
 * @file ReadModuleExt.cpp
 * @brief ReadModule extension
 *
 * This file is created at Almende B.V. It is open-source software and part of the Common 
 * Hybrid Agent Platform (CHAP). A toolbox with a lot of open-source tools, ranging from 
 * thread pools and TCP/IP components to control architectures and learning algorithms. 
 * This software is published under the GNU Lesser General Public license (LGPL).
 *
 * It is not possible to add usage restrictions to an open-source license. Nevertheless,
 * we personally strongly object against this software being used by the military, in the
 * bio-industry, for animal experimentation, or anything that violates the Universal
 * Declaration of Human Rights.
 *
 * Copyright © 2013 Your Name <your@email>
 *
 * @author	Your Name
 * @date	Current date
 * @company	Your Company
 * @project	Specific Software Project
 */

#include <ReadModuleExt.h>

#include <string>
#include <iostream>
#include <unistd.h>

using namespace rur;
using namespace std;

static int lifetime = 1000;

/***********************************************************************************************************************
 * Implementation
 **********************************************************************************************************************/

ReadModuleExt::ReadModuleExt() {
}

ReadModuleExt::~ReadModuleExt() {
	std::cout << "Deallocate object and close socket to name server" << endl;
}

void ReadModuleExt::Tick() {
	std::cout << '[' << getpid() << "] Tick " << lifetime << endl;
	ReadModule::Tick();

	int *value = readInput(false);
	if (value != NULL) std::cout << "Got value " << *value << std::endl;

	sleep(1);
}

bool ReadModuleExt::Stop() {
	if (--lifetime) {
		return false;
	}
	return true;
}

