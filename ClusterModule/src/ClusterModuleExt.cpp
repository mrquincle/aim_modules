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

#include <kMeans.h>
#include <ExpectationMaximization.h>

using namespace rur;

enum DataSet { D_ABALONE, D_GAUSSIAN, D_IRIS, NUMBER_OF_DATASETS };

void ClusterModuleExt::Init(std::string& name) {
	std::ifstream f;
	DataSet dataset;
	dataset = D_ABALONE;
//	dataset = D_GAUSSIAN;
	std::string file = "";
	switch (dataset) {
	case D_ABALONE: {
		file = "../../data/abalone3.data";
		// obtained # of clusters by $(cat ../../data/abalone3.data | cut -f11 -d',' | sort -n | uniq | wc -l)
		predefined_clusters = 28;
	}
	break;
	case D_GAUSSIAN: default: {
		file = "../../data/gaussian3d1.data";
		predefined_clusters = 3;
//		file = "../../data/gaussian1.data";
//		predefined_clusters = 2;
	}
	break;
	}

	f.open(file.c_str());
	if (f.is_open()) {
		f >> d;
	} else {
		std::cerr << "File " << file << " does not exist " << std::endl;
	}

//	d.test();

	index = 0;

	stop = false;

	cluster_method = C_EM_GMM;
}

//! Replace with your own functionality
void ClusterModuleExt::Tick() {

	switch(cluster_method) {
	default: case C_KMEANS: {
		std::vector<value_t> & item = d.pop();
		int D = item.size() - 1; // dimensionality of data samples (length of vector minus field for label)
		std::cout << "Dimensionality is " << D << std::endl;

		int K = predefined_clusters;
		KMeans kmeans(K, D);
		int S = d.size(); // # samples
		std::cout << "Load all " << S << " samples" << std::endl;
		for (int s = 0; s < S; ++s) {
			kmeans.addSample(item, item[D], D);
			item = d.pop();
		}
		int T = 400; // time span
		for (int t = 0; t < T; ++t) {
			if (!(t%10)) std::cout << '.'; flush(std::cout);
			kmeans.tick();
		}
		std::cout << std::endl;

		kmeans.evaluate();
	}
	break;
	case C_EM_GMM: {
		std::vector<value_t> & item = d.pop();
		int D = item.size() - 1; // dimensionality of data samples (length of vector minus field for label)
		std::cout << "Dimensionality is " << D << std::endl;
		std::cout << "Item is ";
		for (int i = 0; i < item.size(); ++i)
			std::cout << item[i] << ' ';
		std::cout << std::endl;

		int K = predefined_clusters;
		ExpectationMaximization expmax(K, D);

		int S = d.size(); // # samples
		std::cout << "Load all " << S << " samples" << std::endl;
		for (int s = 0; s < S; ++s) {
			expmax.addSample(item, item[D], D);
			item = d.pop();
		}

//		expmax.test();

		int T = 30; // time span
		std::cout << "We will run for " << T << " time steps (progress shown by dots)" << std::endl;
		for (int t = 0; t < T; ++t) {
			expmax.tick();
//			if (!(t%10)) {
//				std::cout << std::endl;
				expmax.evaluate();
//			}
//			std::cout << '.'; flush(std::cout);
		}
		std::cout << std::endl;

		expmax.evaluate();

		expmax.print();

	}
	break;
	}

	stop = true; // just one iteration
}

//! Replace with your own functionality
bool ClusterModuleExt::Stop() {
	return stop;
}

