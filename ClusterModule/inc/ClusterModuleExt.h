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
 * Copyright © 2012 Anne van Rossum <anne@almende.com>
 *
 * @author  ...
 * @date    ...
 * @company Almende B.V.
 * @case    Artificial Intelligence Framework
 */

#include <ClusterModule.h>

#include <data.hpp>

namespace rur {

enum ClusterMethod { C_KMEANS, C_EM_GMM, NUMBER_OF_CLUSTER_METHODS };

class ClusterModuleExt: public ClusterModule {
public:
	typedef float value_t;

	void Init(std::string& name);

	// The tick function will be called from the ClusterModuleMain file
	void Tick();

	// As soon as Stop() returns "true", the ClusterModuleMain will stop the module
	bool Stop();

private:
	data<value_t> d;

	int index;

	ClusterMethod cluster_method;

	bool stop;

	int predefined_clusters;
};

}

