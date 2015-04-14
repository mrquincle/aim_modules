/**
 * @file ChineseRestaurantProcess.cpp
 * @brief Chinese Restaurant Process
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
 *
 * The following is a generative Dirichlet Process as well as inference using a basic Gibbs sampling procedure.
 */

#include <ChineseRestaurantProcess.h>
#include <dim1algebra.hpp>
#include <log.h>

/**
 * Constructor
 */
ChineseRestaurantProcess::ChineseRestaurantProcess(float alpha) {
	this->alpha = alpha;
}

/**
 * Write assignments and show distribution.
 */
void ChineseRestaurantProcess::Test(int count, bool calculate_distribution) {
	std::vector<value_t> assignments; assignments.clear();
	CreateAssignments(100, assignments);
	dobots::debug << "Assignments: ";
	for (int i = 0; i < assignments.size(); i++) {
		dobots::debug << assignments[i] << ' ';
	}
	dobots::debug << std::endl;

	if (calculate_distribution) {
		std::vector<value_t> distribution; distribution.clear();
		AssignmentsToDistribution(assignments, distribution);
		dobots::debug << "Distribution: ";
		for (auto i : distribution) {
			dobots::debug << i << ' ';
		}
		dobots::debug << std::endl;
	}
}

/**
 * Create table assignments following the Chinese Restaurant Process. The first table is table with index "0". If you
 * use a Dirichlet process for the data points, you can generate all the seat assignments at once.
 */
void ChineseRestaurantProcess::CreateAssignments(int count, std::vector<value_t> & assignments) {
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

/**
 * Pick a new table with probability a/(n+a) and pick an existing table with probability (1 - a/(n+a)) with n being the
 * number of customers already seated. If n==0 it is the first customer, and the chance to get a new table is 1. Note,
 * this assumes observations/assignments starting from index 0: X_0, X_1, etc. The equations on for example
 * http://www.wikiwand.com/en/Dirichlet_process assume them starting from index 1: X_1, X_2, etc. So, they have (n-1)
 * in the equations below.
 *
 * Draw X_0 from the base distribution H
 * For n > 0
 *   1.) Draw X_1 (and up) with probability a/(n+a) from H
 *   2.) Set X_1 (and up) with probability n_{table}/(n+a) to X_{table}
 * We use a shortcut here. Because we have stored an array for all customers, we can perform 2.) by uniformly sampling
 * over these customers (this will automatically be in correspondence with the number of customers at each table).
 */
ChineseRestaurantProcess::value_t ChineseRestaurantProcess::NextAssignment(std::vector<value_t> & assignments, int & last_table) {
	int n = assignments.size();
	if (drand48() < (alpha / (n + alpha))) {
		last_table++;
		return last_table;
	} else {
		return assignments[rand() %  n];
	}
}

/**
 * We can also sample over a distribution. This is more flexible, because it is possible not to just enter the
 * distribution, but assign different weights corresponding to e.g. a likelihood function to each table.
 *
 * TODO: not used
 */
ChineseRestaurantProcess::value_t ChineseRestaurantProcess::NextTable(std::vector<value_t> & weighted_distribution, int & last_table) {
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

/**
 * Just get the vector of customers referring to table indices and calculate the number of customers at each table.
 * Note that this changes the assignments vector (it gets sorted).
 */
void ChineseRestaurantProcess::AssignmentsToDistribution(std::vector<value_t> & assignments,
		std::vector<value_t> & distribution) {
	std::sort(assignments.begin(), assignments.end());
	size_t length = *std::max_element(assignments.begin(), assignments.end());
	distribution.resize(length+1, 0);
	dobots::count(assignments.begin(), assignments.end(), distribution.begin());
}


