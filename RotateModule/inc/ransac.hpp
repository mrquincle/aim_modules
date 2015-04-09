/**
 * @brief RANdom SAmple Consensus
 * @file ransac.hpp
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
 * @author    Anne C. van Rossum
 * @date      Oct 23, 2012
 * @project   Replicator FP7
 * @company   Almende B.V.
 * @case      Outlier detection
 */

/************************************************************************************
 * Configuration
 ************************************************************************************/

#ifndef RANSAC_HPP_
#define RANSAC_HPP_

#include <vector>
#include <string>

class RansacModel {
public:
	RansacModel() {}

	~RansacModel() {}
private:
	std::vector<double> parameters;
};

class RansacDataPoint {
	int x;
	int y;
	int value;
};

class RansacData {
	std::vector<RansacDataPoint> data;
};


/**
 * RANSAC, RANdom SAmple Consensus is an algorithm to detect outliers so they can be
 * subsequently removed for other image processing algorithms.
 */
class Ransac {
public:
	Ransac(): data(NULL), model(NULL), datapoints_to_fit(0), iterations(0) {}

	~Ransac() {}

	inline void SetDataPointsToFit(int datapoints_to_fit) {
		this->datapoints_to_fit = datapoints_to_fit;
	}

	inline void SetIterations(int iterations) {
		this->iterations = iterations;
	}

	void SetData(RansacData &data) { this->data = *data; }

	void SetModel(RansacModel &model) { this->model = *model; }

	void Tick() {

	}
private:

	RansacData *data;

	RansacModel *model;

	int datapoints_to_fit;

	int iterations;
};


#endif /* RANSAC_HPP_ */
