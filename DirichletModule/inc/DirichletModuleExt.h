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
 * The Dirichlet Module contains a mixture model in which a Dirichlet Process is used as underlying distribution of
 * the latent classes. It is an unsupervised learning method.
 */
class DirichletModuleExt: public DirichletModule {
public:
	typedef float value_t;
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

	void Test(int count, bool distribution);
	//void SampleFromPrior();

	//! Create table assignments
	void CreateAssignments(int count, std::vector<value_t> & assignments);

	//! Next assignment
	value_t NextAssignment(std::vector<value_t> & assignments, int & last_table);

	//! Next assignment
	value_t NextTable(std::vector<value_t> & weighted_distribution, int & last_table);

	//! Cast to distribution
	void AssignmentsToDistribution(std::vector<value_t> & assignments, std::vector<value_t> & distribution);

	//! Come up with the next parameter, this would use NextAssignment and if larger than parameters.size()
	// generate a new parameter with the given prior, and if smaller, pick that parameter.
	void NextParameter();

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
	// each parameter is a column-vector
	matrix_t parameters;

	// alpha value for Dirichlet prior
	value_t alpha;

	vector_t prior_mean;
	matrix_t prior_covar;

//	int param_dim = prior_mean.size();
	Eigen::EigenMultivariateNormal<value_t> *prior_dist;

	// TODO: perhaps use Eigen::Ref to put on the heap?
	std::vector< vector_t > observations;
	
	// parameters (theta collects both mean and covariance matrices, and define a normal distribution)
	std::vector<NormalDistribution> thetas;

	bool stopping_flag;
};

}

