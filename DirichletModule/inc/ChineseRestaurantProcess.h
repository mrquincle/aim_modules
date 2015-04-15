/**
 * @file ChineseRestaurantProcess.h
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

#pragma once

#include <vector>

/**
 * The data structure used in the Chinese Restaurant Process (CRP) is one in which each customer is an index in an
 * assignment vector, of which each value refers to a cluster index.
 */
class ChineseRestaurantProcess {
public:
	//! Typedef for index_t
	typedef int index_t;

	//! Typedef for value_t
	typedef float value_t;

	//! Constructor
	ChineseRestaurantProcess(float alpha);

	//! Test the CRP output 
	void Test(int count, bool distribution);

	//! Create table assignments
	void CreateAssignments(int count, std::vector<index_t> & assignments);

	//! Next assignment
	void NextAssignment(const std::vector<index_t> & assignments, index_t & last_table, index_t & assigned_table, 
			bool &is_table_new);

	//! Next assignment
	value_t NextTable(std::vector<value_t> & weighted_distribution, index_t & last_table);

	//! Cast to distribution
	void AssignmentsToDistribution(std::vector<index_t> & assignments, std::vector<index_t> & distribution);

	//! Come up with the next parameter, this would use NextAssignment and if larger than parameters.size()
	// generate a new parameter with the given prior, and if smaller, pick that parameter.
	void NextParameter();
private:
	// dispersion factor
	float alpha;
};

