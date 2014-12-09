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
 *
 * The following is a generative Dirichlet Process as well as inference using a basic Gibbs sampling procedure.
 * The literature used here is:
 *
 * Escobar1994               Estimating Normal Means with a Dirichlet Process Prior (1994) Escobar
 * Neal2000                  Markov Chain Sampling Methods for Dirichlet Process Mixture Models (2000) Neal
 * Murphy2007                Conjugate Bayesian analysis of the Gaussian distribution (2007) Murphy
 */

#include <DirichletModuleExt.h>

#include <dim1algebra.hpp>

#include <algorithm>
#include <cmath>
#include <random>

using namespace rur;
using namespace Eigen;

/**
 * Constructor initializes random generators and the dispersion factor for the Dirichlet Process.
 */
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

/**
 * We tick only once and stop then our module. There is currently no input from outside this module used.
 *
 * The function CreateAssignments generates samples according to a Chinese Restaurant Process with alpha defined in 
 * the constructor. Then the representation in the form of a table index per customer, is changed to a representation 
 * in the form of only the number of customers per table. 
 */
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
 * use a Dirichlet process for the data points, you can generate all the seat assignments at once.
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
DirichletModuleExt::value_t DirichletModuleExt::NextAssignment(std::vector<value_t> & assignments, int & last_table) {
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

/**
 * Just get the vector of customers referring to table indices and calculate the number of customers at each table.
 * Note that this changes the assignments vector (it gets sorted).
 */
void DirichletModuleExt::AssignmentsToDistribution(std::vector<value_t> & assignments,
		std::vector<value_t> & distribution) {
	std::sort(assignments.begin(), assignments.end());
	size_t length = *std::max_element(assignments.begin(), assignments.end());
	distribution.resize(length+1, 0);
	dobots::count(assignments.begin(), assignments.end(), distribution.begin());
}

/**
 * The Stop function checks when its time to stop. We use a simple flag that we set in the Tick function.
 */
bool DirichletModuleExt::Stop() {
	return stopping_flag;
}

/**
 * Update the sufficient statistics (the hyperparameters) that describe the Normal Inverse Wishart distribution. A 
 * single observation changes these parameters in a closed form manner.
 *
 * Do not use the same sufficient statistics object as input as well as output.
 */
void DirichletModuleExt::UpdateSufficientStatistics(const SufficientStatistics & ss_in, vector_t observation,
		SufficientStatistics & ss_out) {
	ss_out.kappa = ss_in.kappa + 1;
	ss_out.nu = ss_in.nu + 1;
	ss_out.mu = (observation + ss_in.kappa*ss_in.mu)/ss_out.kappa;
	ss_out.lambda = ss_in.lambda + (ss_in.kappa/ss_out.kappa) * 
		(observation-ss_in.mu)*(observation-ss_in.mu).transpose();
}

/**
 * This is the integral that benefits from conjugacy between the multivariate normal distribution and the 
 * normal-inverse-wishart prior distribution.
 *
 * B(y_i) = \integral F(y_i,theta_i) dG_0(theta_i)
 *
 * p(x|D) = t-distribution_{nu_n,d+1}(mu_n, Lambda_n(kappa_n + 1)/kappa_n( nu_n -d + 1 ))
 *
 * Notations used in the literature:
 *    p(x|D)
 *
 * The equation in Murphy2007 with respect to S and nu and pi to the power of -1/2 is made slightly simpler by the
 * identity: |cA|=c^d|A|, so |S|*nu^d*pi^d=|S*nu*pi| (with of course S a matrix of dxd).
 *
 * The multivariate t-distribution can be found in Murphy2007. 
 */
void DirichletModuleExt::PosteriorPredictive(const SufficientStatistics & ss, const vector_t & observation,
		value_t & posterior_predictive) {
	value_t d = observation.size();
	// parameters for the t-distribution
	matrix_t S = ss.lambda * (ss.kappa + 1) / ( ss.kappa * ( ss.nu - d + 1) );
	value_t nu = ss.nu - d + 1;
	vector_t mu = ss.mu;
	// calculate the t-distribution
	// set up Cholesky decomposition to get S^{-1}
	/*
	LLT<matrix_t> llt;
	llt.compute(S);
	if (llt.info() != Success) {
		std::cout << "Error in LLT decomposition. Is covariance matrix positive semi-definite?" << std::endl;
		return;
	}*/
	value_t Snupi = (S * nu * M_PI).determinant();
	value_t c = std::exp(lgamma((nu+d)/2)-lgamma(nu/2)) * std::pow(Snupi, -0.5);
	vector_t diff = (observation - mu);
	value_t term = (diff.transpose() * S.inverse() * diff); 
	value_t scatter = std::pow(1+term/nu, -(nu+d)/2);
       	posterior_predictive = scatter * c * Snupi;
}

/**
 * Calculate the likelihood of a data point given a multivariate normal distribution (with mean and covariance matrix).
 * The multivariate normal distribution requires an inversion of the covariance matrix. 
 *
 * Notations used in the literature: 
 *    p(D|mu,Sigma)
 *    p(x|mu,sigma^2)
 *
 * @param                    nd [in], the parameters defining the multivariate normal distribution
 * @param                    observation [in], the data point to be described by the distribution
 * @return                   probability [out], probability that this data point stems from this distribution
 */
DirichletModuleExt::value_t DirichletModuleExt::Likelihood(const NormalDistribution &nd, const vector_t & observation) {
	/*
	LLT<matrix_t> llt(nd.covar);
	//llt.compute(nd.covar);
	if (llt.info() != Success) {
		std::cout << "Error in LLT decomposition. Is covariance matrix positive semi-definite?" << std::endl;
		return -1;
	}
	*/
	const vector_t diff = (nd.mean - observation);
	value_t dim = observation.size();
	// return normal distribution
	matrix_t inverse = nd.covar.inverse();
	value_t exponent = -0.5 * diff.transpose() * inverse * diff;
	//value_t det = llt.matrixL().diagonal().product();
	value_t det = nd.covar.determinant();
	value_t normalization = std::pow( 2*M_PI * det, -0.5*dim ) ; // check how to get det out of llt
	return normalization * std::exp(exponent);
}

/**
 * Get new normal distribution given observation and sufficient statistics (from before that observation).
 */
void DirichletModuleExt::PosteriorDensity(const SufficientStatistics & ss, const vector_t & observation, 
	       NormalDistribution & nd) {
	SufficientStatistics ss_temp;
	UpdateSufficientStatistics(ss, observation, ss_temp);
	SampleNormalInverseWishart(ss_temp, nd);	
}

/**
 * Sample nd.S from an inverse Wishart distribution 
 * Sample nd.mean from a normal distribution N(ss.mu0, S/ss.kappa);
 *
 * TODO: Study articles referenced from
 *       http://dahtah.wordpress.com/2012/03/07/why-an-inverse-wishart-prior-may-not-be-such-a-good-idea/
 *       and adjust prior accordingly if necessary.
 */
void DirichletModuleExt::SampleNormalInverseWishart(const SufficientStatistics & ss, NormalDistribution &nd) {
	SampleInverseWishart(ss.nu, nd.covar);
	SampleMultivariateNormal(ss.mu, nd.covar/ss.kappa, nd.mean);
}

/**
 * Generate a vector (e.g. a mean) using a multivariate normal distribution.
 */
void DirichletModuleExt::SampleMultivariateNormal(const vector_t & mean, const matrix_t & S, vector_t sample) {
	EigenMultivariateNormal<value_t> normX_solver(mean, S);
	sample = normX_solver.samples(1);// might need transpose
}

/**
 * Generate a matrix (e.g. a covariance matrix) using the hyperparameters given by the Inverse Wishart.
 */
void DirichletModuleExt::SampleInverseWishart(const value_t & degrees_of_freedom, matrix_t & S) {
	// zero-mean normal
	vector_t mean = vector_t::Zero(S.cols());
	EigenMultivariateNormal<value_t> normX_solver(mean, S);
	vector_t sample = normX_solver.samples(1);
	matrix_t iS = sample*sample.transpose();
	/*
	LLT<matrix_t> llt;
	llt.compute(iS);
	if (llt.info() != Success) {
		std::cout << "Error in LLT decomposition. How did we not create a covariance matrix?" << std::endl;
		return;
	}
	S = llt().solve();
	*/
	S = iS.inverse();
}

/**
 * Helper function which generates likelihoods with respect to all parameters theta given an observation. This is 
 * convenient because of the sum in equation 2 in Neal (where we need all these terms).
 */
void DirichletModuleExt::Likelihoods(const std::vector<NormalDistribution> & thetas, const vector_t & observation,
		std::vector<value_t> & likelihoods) {
	likelihoods.clear();
	likelihoods.resize(thetas.size());
	for (auto && i : thetas) {
		value_t ll = Likelihood(i, observation);
		likelihoods.push_back(ll);
	}
}

/**
 *
 * Notations used in the literature
 *     dispersion_factor     alpha (Neal2000), A_0 (Escobar1994)
 *
 * @param                    ss [in], sufficient statistics for normal-inverse wishart distribution (4 parameters)
 * @param                    thetas_without_k [in], all other normal distributions (each 2 parameters)
 * @param                    dispersion_factor [in], often alpha (or A_0), governs the Dirichlet Process
 * @param                    observation [in], the new observation
 * @param                    theta_k [out], the updated normal distribution as a result of the Gibbs sampling step
 */
void DirichletModuleExt::GibbsStep(const SufficientStatistics & ss, const std::vector<NormalDistribution> & thetas_without_k, 
		const value_t dispersion_factor, const vector_t & observation, 
		NormalDistribution & theta_k) {
	// 1. calculate likelihoods	
	std::vector<value_t> likelihoods;
	Likelihoods(thetas_without_k, observation, likelihoods);

	// 2. calculate posterior predictive of observation given a gaussian distribution
	value_t posterior_predictive;
	PosteriorPredictive(ss, observation, posterior_predictive);

	// 3. calculate denominator
	value_t sum_likelihoods;
	for (auto & i : likelihoods) {
		sum_likelihoods += i;
	}
	value_t Z = sum_likelihoods + dispersion_factor * posterior_predictive;

	// 4. calculate probability of a new "table", a new gaussian distribution
	value_t prob_new = (dispersion_factor * posterior_predictive) / Z;

	// 5. pick a uniform number between 0 and 1
	value_t u = rand(); // <!- todo, pick proper random generator

	// 6. assign from new table 
	if (u < prob_new) {
		PosteriorDensity(ss, observation, theta_k);
	}
	// 7. assign from old table
	else {
		// 8. inverse transform sample to use a uniform number to pick item from normal distribution set
		value_t its = u - prob_new;
		std::vector<value_t> cumsum_likelihoods;
		cumsum_likelihoods.resize(likelihoods.size());
		std::partial_sum(likelihoods.begin(), likelihoods.end(), cumsum_likelihoods.begin());
		for (auto it = cumsum_likelihoods.begin(); it != cumsum_likelihoods.end(); ++it) {
			// 9. pick the item when uniform number exceeds value in the cumulative density function
			if (*it >= its) {
				// likelihoods in step 1 have to be ordered exactly the same as the thetas_without_k
				value_t index = std::distance(it, cumsum_likelihoods.begin());
				theta_k = thetas_without_k[index];
			}
		}
	}
}
