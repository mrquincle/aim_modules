/**
 * @file ModelSelectionModuleExt.cpp
 * @brief ModelSelectionModule extension
 *
 * This file is created at "Distributed Organisms B.V.". It is open-source software and part of "Robotic Suite". 
 * This software is published under the GNU General Lesser Public license (LGPLv3).
 *
 * Copyright © 2013 Anne C. van Rossum <anne@dobots.nl>
 *
 * @author                   Anne C. van Rossum
 * @date                     Oct 18, 2013
 * @organisation             Distributed Organisms B.V.
 * @project                  Robotic Suite
 */

#include <ModelSelectionModuleExt.h>

#include <dim1algebra.hpp>

using namespace rur;

//! Replace with your own code
ModelSelectionModuleExt::ModelSelectionModuleExt(): RSS(0), k(0), n(0), criterion(AIC) {

}

//! Replace with your own code
ModelSelectionModuleExt::~ModelSelectionModuleExt() {

}

float ModelSelectionModuleExt::ModelQuality(QualityCriterion criterion) {
	float value(0);
	switch(criterion) {
	default: case AIC:
		value = n * log(RSS/(double)n) + 2*k;
		break;
	case BIC:
		value = n + n * log(2*M_PI) + n * log(RSS/(double)n) + (log((double)n) / (k + 1));
		break;
	}
	return value;
}

//! Replace with your own code
void ModelSelectionModuleExt::Tick() {
	long_seq *vec = readResiduals(true);
	if (!vec || vec->empty()) return;
	n = vec->size();

	// update k if necessary
	int *l_k = readModelParameterCount();
	if (l_k) k = *l_k;
	if (!k) return; // k hasn't been set yet, skip

	// update criterion if necessary
	int *l_m = readMethod();
	if (l_m) setMethod(*l_m); // if not set, just continue and use default

	// residual sum of squares
	RSS = dobots::pseudo_norm<int>(vec->begin(), vec->end(), dobots::PN_SUM_OF_SQUARES);

	double value = ModelQuality(criterion);
	writeModelQuality(value);
}

//! Replace with your own code
bool ModelSelectionModuleExt::Stop() {
	return false;
}

