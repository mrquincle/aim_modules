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

#pragma once

#include <DirichletModule.h>
#include <eigenmultivariatenormal.hpp>
#include <vector>
#include <ChineseRestaurantProcess.h>

namespace rur {

/**
 * The Dirichlet Module contains a mixture model in which a Dirichlet Process is used as underlying distribution of
 * the latent classes. It is an unsupervised learning method.
 */
class DirichletModuleExt: public DirichletModule {
public:
	typedef float value_t;
	typedef int index_t;

	typedef Eigen::Matrix<value_t,Eigen::Dynamic,Eigen::Dynamic> matrix_t;
	typedef Eigen::Matrix<value_t,Eigen::Dynamic,1> vector_t; // column_vector

	/**
	 * Normal distribution is represented by mean (vector) and covariance (matrix).
	 */
	class NormalDistribution {
		public:
			vector_t mean;
			matrix_t covar;

			NormalDistribution & operator=(const NormalDistribution & other) {
				if (&other == this) return *this;
				mean = other.mean;
				covar = other.covar;
				return *this;
			}

			bool operator<(const NormalDistribution &other) const {
				for (int i=0; i < other.mean.size(); i++) {
					if (mean[i] < other.mean[i]) return true;
					if (mean[i] > other.mean[i]) return false;
				}
				return false;
			}

			bool operator==(const NormalDistribution &other) const {
				return (mean == other.mean && covar == other.covar);
			}
	};

	/**
	 * The sufficient statistics of the NIW distribution are represented by four parameters.
	 */
	struct SufficientStatistics {
		size_t dim;
		vector_t mu;
		value_t kappa;
		value_t nu;
		matrix_t lambda;
	};

	//! The constructor
	DirichletModuleExt();

	//! The destructor
	virtual ~DirichletModuleExt();

	//! The tick function is the "heart" of your module, it reads and writes to the ports
	void Tick();

	//! As soon as Stop() returns "true", the DirichletModuleMain will stop the module
	bool Stop();

	void Initialization(const SufficientStatistics & ss);

	void Run(const SufficientStatistics & ss, size_t iterations);

	void MetropolisHastingsStep(const std::vector<index_t> & assignments, 
		std::vector<NormalDistribution> & tables, const NormalDistribution &current_distribution, 
		const index_t current_table_index, const SufficientStatistics & ss, const vector_t & observation, 
		bool accept_all, index_t & assignment);
	bool Acceptance(const NormalDistribution &nd_proposed, const NormalDistribution &nd_old, 
		const vector_t observation);

	void UpdateSufficientStatistics(const SufficientStatistics & ss_in, vector_t observation,
			SufficientStatistics & ss_out);
	void PosteriorPredictive(const SufficientStatistics & ss, const vector_t & observation,
			value_t & posterior_predictive);
	value_t Likelihood(const NormalDistribution &nd, const vector_t & observation);
	void PosteriorDensity(const SufficientStatistics & ss, const vector_t & observation, 
			NormalDistribution & nd);
	void SampleNormalInverseWishart(const SufficientStatistics & ss, NormalDistribution &nd);
	void SampleMultivariateNormal(const vector_t & mean, const matrix_t & S, vector_t & sample);
	void SampleInverseWishart(const SufficientStatistics & ss, matrix_t & S);
	void Likelihoods(const std::vector<NormalDistribution> & thetas, const vector_t & observation,
			std::vector<value_t> & likelihoods);
	void GibbsStep(const SufficientStatistics & ss, const std::vector<NormalDistribution> & thetas_without_k, 
			const value_t dispersion_factor, const vector_t & observation, 
			NormalDistribution & theta_k);

private:
	// alpha value for Dirichlet prior
	value_t alpha;

	ChineseRestaurantProcess chinese_restaurant_process;

	Eigen::EigenMultivariateNormal<value_t> *prior_dist;

	// TODO: perhaps use Eigen::Ref to put on the heap?
	std::vector< vector_t > observations;
	
	// parameters (theta collects both mean and covariance matrices, and define a normal distribution)
	std::vector<NormalDistribution> thetas;

	// stops the module when set to true
	bool stopping_flag;

	// 
	std::vector< index_t > tables;
};

}

