/**
 * @file DirichletModuleExt.cpp
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

#include <DirichletModuleExt.h>

#include <dim1algebra.hpp>

using namespace rur;

//! Replace with your own code
DirichletModuleExt::DirichletModuleExt(): alpha(0.2) {
/*
	int param_dim = 2;
	prior_mean.resize(1,param_dim);
	prior_covar.resize(param_dim,param_dim);
	prior_covar.setZero();
	for (int i = 0; i < param_dim; i++) {
		prior_mean(i) = drand48();
		prior_covar(i,i) = drand48();
	}
	int param_count = 10;
	parameters.resize(param_dim,param_count);

	prior_dist = new Eigen::EigenMultivariateNormal<value_t>(prior_mean,prior_covar);
*/

	stopping_flag = false;
}

//! Replace with your own code
DirichletModuleExt::~DirichletModuleExt() {

}

//! Replace with your own code
void DirichletModuleExt::Tick() {
	std::vector<value_t> assignments; assignments.clear();
	CreateAssignments(100, assignments);
	std::cout << "Assignments: ";
	for (int i = 0; i < assignments.size(); i++) {
		std::cout << assignments[i] << ' ';
	}
	std::cout << std::endl;

	std::vector<value_t> distribution; distribution.clear();

	AssignmentsToDistribution(assignments, distribution);
	std::cout << "Distribution: ";
	for (int i = 0; i < distribution.size(); i++) {
		std::cout << distribution[i] << ' ';
	}
	std::cout << std::endl;

	stopping_flag = true;
}

/**
 * \theta_i | \theta_{-i} ~ 1/(i-1+\alpa) \sum_{j=1}^{i-1} \delta(\theta_j) + \alpha / (i-1+\alpha) G_0
 *
 * Here \theta_{-i} means all \theta, but "not i", ~ means "from distribution".
 */
void DirichletModuleExt::SampleFromPrior() {
	matrix_t theta = prior_dist->samples(parameters.cols());
	for (int i = 1; i < theta.cols(); i++) {
		theta.col(i) = 1/(i-1+alpha) * theta.leftCols(i-1).colwise().sum() + alpha/(i-1+alpha) * theta.col(i);
	}
}

/**
 * Create table assignments following the Chinese Restaurant Process. The first table is table with index "0".
 */
void DirichletModuleExt::CreateAssignments(int count, std::vector<value_t> & assignments) {
	int last_table;
	if (assignments.empty()) {
		assignments.push_back(0);
		last_table = 0;
	} else {
		last_table = *std::max_element(assignments.begin(), assignments.end());
	}
	for (int i = 0; i < count; ++i) {
		if (drand48() < (alpha / (1 + alpha))) {
			last_table++;
			assignments.push_back(last_table);
		} else {
			assignments.push_back(assignments[rand() %  assignments.size()]);
		}
	}
}

void DirichletModuleExt::AssignmentsToDistribution(std::vector<value_t> & assignments,
		std::vector<value_t> & distribution) {
	std::sort(assignments.begin(), assignments.end());
	size_t length = *std::max_element(assignments.begin(), assignments.end());
	distribution.resize(length+1, 0);
	dobots::count(assignments.begin(), assignments.end(), distribution.begin());
}

//! Replace with your own code
bool DirichletModuleExt::Stop() {
	return stopping_flag;
}

