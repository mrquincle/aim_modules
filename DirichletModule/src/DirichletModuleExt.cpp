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
DirichletModuleExt::DirichletModuleExt(): alpha(1.2) {
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
	long int seed = time(NULL);
	std::cout << "Use seed: " << seed << std::endl;
	srand(seed);
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
 *
 * Wrong interpretation. To
 */
void DirichletModuleExt::SampleFromPrior() {
	matrix_t theta = prior_dist->samples(parameters.cols());
	for (int i = 1; i < theta.cols(); i++) {
		theta.col(i) = 1/(i-1+alpha) * theta.leftCols(i-1).colwise().sum() + alpha/(i-1+alpha) * theta.col(i);
	}
}

/**
 * Create table assignments following the Chinese Restaurant Process. The first table is table with index "0". If you
 * use a Dirichlet process for data points, you can in a batch process, generated all the seat assignments at once.
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
		assignments.push_back(NextAssignment(assignments, last_table));
	}
}

DirichletModuleExt::value_t DirichletModuleExt::NextAssignment(std::vector<value_t> & assignments, int & last_table) {
	int i = assignments.size();
	if (drand48() < (alpha / (i + alpha))) {
		last_table++;
		return last_table;
	} else {
		return assignments[rand() %  i];
	}
}

/**
 * We can also sample over a distribution. This is more flexible, because it is possible not to just enter the
 * distribution, but assign different weights corresponding to e.g. a likelihood function to each table.
 */
DirichletModuleExt::value_t DirichletModuleExt::NextTable(std::vector<value_t> & weighted_distribution, int & last_table) {
	size_t len = weighted_distribution.size();
	assert (len != 0);
	std::vector<value_t> part_sum;
	part_sum.resize(len, 0);
	std::partial_sum(weighted_distribution.begin(), weighted_distribution.end(), part_sum.begin());
	double total_sum = part_sum[len - 1];
	if (drand48() < (alpha / (total_sum + alpha))) {
		last_table++;
		return last_table;
	} else {
		std::transform(part_sum.begin(), part_sum.end(), part_sum.begin(), std::bind1st(std::divides<double>(),total_sum));
		size_t index = dobots::exceeds_element(part_sum.begin(), part_sum.end(), (float)drand48()) - part_sum.begin();
		assert (index < weighted_distribution.size());
		return weighted_distribution[index];
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

