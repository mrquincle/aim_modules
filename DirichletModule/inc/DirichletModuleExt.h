/**
 * @file DirichletModuleExt.h
 * @brief DirichletModule extension
 *
 * This file is created at "Distributed Organisms B.V.". It is open-source software and part of "Robotic Suite". 
 * This software is published under the GNU General Lesser Public license (LGPLv3).
 *
 * Copyright © 2013 Anne C. van Rossum <anne@dobots.nl>
 *
 * @author                   Anne C. van Rossum
 * @date                     Oct 22, 2013
 * @organisation             Distributed Organisms B.V.
 * @project                  Robotic Suite
 */

#include <DirichletModule.h>

#include <eigenmultivariatenormal.hpp>

#include <vector>

namespace rur {

/**
 * Your Description of this module.
 */
class DirichletModuleExt: public DirichletModule {
public:
	typedef float value_t;
	typedef Eigen::Matrix<value_t,Eigen::Dynamic,Eigen::Dynamic> matrix_t;
	typedef Eigen::Matrix<value_t,Eigen::Dynamic,1> vector_t; // column_vector

	//! The constructor
	DirichletModuleExt();

	//! The destructor
	virtual ~DirichletModuleExt();

	//! The tick function is the "heart" of your module, it reads and writes to the ports
	void Tick();

	//! As soon as Stop() returns "true", the DirichletModuleMain will stop the module
	bool Stop();

	void SampleFromPrior();

	//! Create table assignments
	void CreateAssignments(int count, std::vector<value_t> & assignments);

	void AssignmentsToDistribution(std::vector<value_t> & assignments, std::vector<value_t> & distribution);
private:
	// each parameter is a column-vector
	matrix_t parameters;

	// alpha value for Dirichlet prior
	value_t alpha;

	vector_t prior_mean;
	matrix_t prior_covar;

//	int param_dim = prior_mean.size();
	Eigen::EigenMultivariateNormal<value_t> *prior_dist;

	bool stopping_flag;
};

}

