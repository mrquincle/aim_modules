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

using namespace rur;

void ClusterModuleExt::Init(std::string& name) {
	std::ifstream f;
//	std::string file = "../../data/abalone3.data";
	std::string file = "./gaussian.data";
	f.open(file.c_str());
	if (f.is_open()) {
		f >> d;
	} else {
		std::cerr << "File " << file << " does not exist " << std::endl;
	}
//	d.test();

	index = 0;

	stop = false;

	cluster_method = C_KMEANS;
}

//! Replace with your own functionality
void ClusterModuleExt::Tick() {

	switch(cluster_method) {
	default: case C_KMEANS: {
		std::vector<float> & item = d.pop();
		int D = item.size() - 1; // dimensionality of data samples (length of vector minus field for label)
		std::cout << "Dimensionality is " << D << std::endl;

		// cat ../../data/abalone3.data | cut -f11 -d',' | sort -n | uniq | wc -l
		int K = 2; // # clusters
		KMeans kmeans(K, D);
		int S = d.size()-1; // # samples
		std::cout << "Load all " << S << " samples" << std::endl;
		for (int s = 0; s < S; ++s) {
			kmeans.addSample(item, item[D], D);
			item = d.pop();
		}
		int T = 400; // time span
		for (int t = 0; t < T; ++t) {
//			std::cout << "Tick " << t << std::endl;
			if (!(t%10)) std::cout << '.'; flush(std::cout);
			kmeans.tick();
		}
		std::cout << std::endl;

		kmeans.evaluate();
//		std::cout << "Result k-means (cluster means): " << std::endl;
//		kmeans.print();
	}
	break;
	case C_EM_GMM: {

	}
	break;
	}

	stop = true; // just one iteration
}

//! Replace with your own functionality
bool ClusterModuleExt::Stop() {
	return stop;
}

