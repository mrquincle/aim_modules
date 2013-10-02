/**
 * @file ClusterModuleExt.cpp
 * @brief ClusterModule extension
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

#include <ClusterModuleExt.h>

#include <iostream>
#include <fstream>

using namespace rur;

void ClusterModuleExt::Init(std::string& name) {
	std::ifstream f;
	std::string file = "../../data/abalone2.data";
	f.open(file.c_str());
	if (f.is_open()) {
		f >> d;
	} else {
		std::cerr << "File " << file << " does not exist " << std::endl;
	}
	d.test();

	index = 0;
}

//! Replace with your own functionality
void ClusterModuleExt::Tick() {
	std::vector<float> & item = d.pop();

}

//! Replace with your own functionality
bool ClusterModuleExt::Stop() {
	return false;
}

