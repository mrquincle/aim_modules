/**
 * @file ILVQModuleExt.cpp
 * @brief ILVQModule extension
 *
 * This file is created at "Distributed Organisms B.V.". It is open-source software and part of "Robotic Suite". 
 * This software is published under the GNU General Lesser Public license (LGPLv3).
 *
 * Copyright © 2013 Anne C. van Rossum <anne@dobots.nl>
 *
 * @author                   Anne C. van Rossum
 * @date                     Oct 29, 2013
 * @organisation             Distributed Organisms B.V.
 * @project                  Robotic Suite
 */

#include <ILVQModuleExt.h>

#include <dim1algebra.hpp>

using namespace rur;

//! Replace with your own code
ILVQModuleExt::ILVQModuleExt() {
	ilvq = new dobots::ILVQ_XSZ();
	sample_dimension = 0;
}

//! Replace with your own code
ILVQModuleExt::~ILVQModuleExt() {
	if (ilvq) delete ilvq;
}

//! Replace with your own code
void ILVQModuleExt::Tick() {
	long_seq *train_samples = readTraining(false);
	if (train_samples) {
		std::vector<float> values;
		if (sample_dimension) {
			if (sample_dimension != train_samples->size() - 1) {
				std::cerr << "New sample arrived with deviating size!" << std::endl;
			}
		} else {
			sample_dimension = train_samples->size()-1;
		}
		values.resize(sample_dimension,0);
		int label;
		dobots::extract_copy(train_samples->begin(), train_samples->end(),
				train_samples->begin()+sample_dimension, values.begin(), label);
		ilvq->add(values, label);
	}
	long_seq *test_samples = readTesting(false);
	if (test_samples) {
		if (test_samples->size() != sample_dimension - 1) {
			std::cerr << "Test sample should be training sample size without the label element, so one less!" << std::endl;
			return;
		}
		std::vector<float> values;
		values.resize(sample_dimension,0);
		std::copy(test_samples->begin(), test_samples->end(), values.begin());
		int label = ilvq->classify(values);

		// write to output
		writeResult(label);
	}
}

//! Replace with your own code
bool ILVQModuleExt::Stop() {
	return false;
}

