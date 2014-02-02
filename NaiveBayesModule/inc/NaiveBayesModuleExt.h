/**
 * @file NaiveBayesModuleExt.h
 * @brief NaiveBayesModule extension
 *
 * This file is created at "Distributed Organisms B.V.". It is open-source software and part of "Robotic Suite". 
 * This software is published under the GNU General Lesser Public license (LGPLv3).
 *
 * Copyright © 2014 Anne C. van Rossum <anne@dobots.nl>
 *
 * @author                   Anne C. van Rossum
 * @date                     Jan 31, 2014
 * @organisation             Distributed Organisms B.V.
 * @project                  Robotic Suite
 */

#include <NaiveBayesModule.h>
#include <vector>
#include <nd-array.hpp>
#include <stddev.hpp>
#include <map>

namespace rur {

#define AIM_TYPE_SCALAR      0
#define AIM_TYPE_VECTOR      1
#define AIM_TYPE_MATRIX      2

/**
 * A Naive Bayes classifier can naturally use different underlying models. It is possible to use a multinomial model,
 * or, alternatively, a multi-Bernouilli approach. With the former the number of words in a document is taken into 
 * account. With the latter, the absence or presence of a term is counted, not its frequency.
 */
class NaiveBayesModuleExt: public NaiveBayesModule {
public:
	// The constructor
	NaiveBayesModuleExt();

	//! The destructor
	virtual ~NaiveBayesModuleExt();

	//! The tick function is the "heart" of your module, it reads and writes to the ports
	void Tick();

	//! Function to train
	void Train(std::vector<int> & sample);

	//! Test incoming data
	void Test(std::vector<int> & sample);

	//! As soon as Stop returns "true", the NaiveBayesModuleMain will stop the module
	bool Stop();

private:
	//! Length of an individual data item
	int dataitem_length;

	//! Build up full joint probability (be careful, this might be big!)
	nd_array<double> probability_table;

	//! Sample averages and variances.
	std::map<int, stddev<double>*> stats;
};

}

