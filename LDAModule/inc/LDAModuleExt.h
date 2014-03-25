/**
 * @file LDAModuleExt.h
 * @brief LDAModule extension
 *
 * This file is created at "Distributed Organisms B.V.". It is open-source software and part of "Robotic Suite". 
 * This software is published under the GNU General Lesser Public license (LGPLv3).
 *
 * Copyright © 2014 Anne C. van Rossum <anne@dobots.nl>
 *
 * @author                   Anne C. van Rossum
 * @date                     Mar 24, 2014
 * @organisation             Distributed Organisms B.V.
 * @project                  Robotic Suite
 */

#include <LDAModule.h>
#include <tuple-array.hpp>
#include <nd-array.hpp>

namespace rur {

// AIM protocol version
#define AIM_PROTOCOL_VERSION                     0

// AIM data types
#define AIM_TYPE_SCALAR                          0
#define AIM_TYPE_VECTOR                          1
#define AIM_TYPE_MATRIX                          2

/**
 * Your Description of this module.
 */
class LDAModuleExt: public LDAModule {
public:
	//! The constructor
	LDAModuleExt();

	//! The destructor
	virtual ~LDAModuleExt();

	//! The tick function is the "heart" of your module, it reads and writes to the ports
	void Tick();

	//! As soon as Stop() returns "true", the LDAModuleMain will stop the module
	bool Stop();

	//! Incorporate a new item in frequency statistics
	void Count(long_seq & sample);

	void Gibbs();
private:
	//! Variable size data structure to store counts of (terms, documents)
	tuple_array<int> term_doc_table;

	//! And we have two other tables that store (terms, topics) and (documents, topics)
	tuple_array<int> term_topic_table, doc_topic_table;

	//! And we sum over all terms per topic
	std::vector<int> term_count_per_topic, term_count_per_doc;

	//! Used in sampling step
	std::vector<int> sample_topics;

	//! Number of clusters
	int K;

	//! Number of words
	int W;

	//! Hyperparameters
	double alpha, beta;
};

}

