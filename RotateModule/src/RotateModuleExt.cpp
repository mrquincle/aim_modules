/**
 * @file RotateModuleExt.cpp
 * @brief RotateModule extension
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

#include <random.hpp>
#include <iostream>
#include <stdlib.h>

using namespace rur;

//! Replace with your own code
RotateModuleExt::RotateModuleExt() {
	int seed = time(NULL);
	cout << "Use seed for random generator: " << seed << endl;
	srand(seed);
}

//! Replace with your own code
RotateModuleExt::~RotateModuleExt() {

}

//! Replace with your own code
void RotateModuleExt::Tick() {
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

//! Replace with your own code
bool RotateModuleExt::Stop() {
	return true;
}

