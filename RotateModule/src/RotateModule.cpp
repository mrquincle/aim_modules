/**
 * @file RotateModule.cpp
 * @brief ...
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
 * Copyright © 2012 Anne van Rossum <anne@almende.com>
 *
 * @author  ...
 * @date    ...
 * @company Almende B.V.
 * @case    Artificial Intelligence Framework
 */

#include <RotateModule.h>

#include <random.hpp>
#include <iostream>

using namespace rur;

void RotateModule::Tick() {
	int size = 50;
	int sort = 10;

	std::vector<int> test;
	std::vector<int> result;
	test.clear();
	result.clear();

	for (int i = 0; i < size; ++i) {
		test.push_back(i+1);
		result.push_back(0);
	}

	random_subset(test.begin(), test.end(), sort, result.begin());

	std::cout << "Result: ";
	for (int i = 0; i < size; ++i) {
		std::cout << result[i] << " ";
	}
	std::cout << std::endl;
}

bool RotateModule::Stop() {
	return true;
}

