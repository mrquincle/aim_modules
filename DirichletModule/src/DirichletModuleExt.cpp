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
#include <fstream>
#include <log.h>

using namespace rur;
using namespace Eigen;
	
//const static IOFormat VectorFormat(StreamPrecision, DontAlignCols, " ", " | ");
const static IOFormat VectorFormat(StreamPrecision, DontAlignCols, " ", " ");

#define UPDATE_CLUSTERS

// Comment following to enable cluster updates, do this only after you're sure the inference method is correct
#undef UPDATE_CLUSTERS

/**
 * Constructor initializes random generators and the dispersion factor for the Dirichlet Process.
 */
DirichletModuleExt::DirichletModuleExt(): alpha(1.2), chinese_restaurant_process(alpha) {
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
	dobots::debug << "Use seed: " << seed << std::endl;
	srand48(seed);
	stopping_flag = false;
	Eigen::internal::V_normal_dist_op<value_t>::rng.seed(seed);
}

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

//#define TESTING0
#define TESTING1

	SufficientStatistics ss;
	ss.dim = 2; // data dimension, for now fix it, but should be obtained from actual received data dimension
	ss.kappa = 1;
	ss.mu = vector_t(ss.dim);
	ss.mu << 0, 0;
	ss.nu = 4;
	ss.lambda = matrix_t::Identity(ss.dim, ss.dim);

	stopping_flag = true;
	dobots::debug << "Read observations from file" << std::endl;	
	std::ifstream input;
	input.open("../../data/clusters.txt");
	if (input && input.is_open()) {
		value_t x, y;
		int trunc = 500, i = 0;
		while (input >> x >> y) {
			//dobots::debug << "Read observation: " << x << "," << y << std::endl;	
			vector_t v(2); v << x, y;
			observations.push_back(v);
			if (++i >= trunc) break;
		}
		dobots::info << "Load " << observations.size() << " observations" << std::endl;
		input.close();
	}

#ifdef TESTING0

#ifdef UNIT_TEST_POST_PRED	
	dobots::debug << "kappa0: " << ss.kappa << std::endl;
	dobots::debug << "mu0: " << ss.mu.transpose() << std::endl;
	dobots::debug << "nu0: " << ss.nu << std::endl;
	dobots::debug << "lambda0: " << ss.lambda << std::endl;

	// check posterior predictive
	value_t post;
	PosteriorPredictive(ss, observations[0], post);
	dobots::debug << "Observation " << observations[0].transpose() << " leads to post.pred " << post << std::endl;

	dobots::debug << "posterior " << post << " should be 0.0353623" << std::endl;

	dobots::debug << "Create hyper parameters with dimension for data of " << ss.mu.rows() << std::endl;
#endif

	Initialization(ss);
	int steps = 2000;
	Run(ss, steps);

#elif defined(TESTING1)

	// creation of table assignments
	index_t last_table = 0;

	// assignments are kept separate from the distribution vector and store only an index to an entry into the latter
	std::vector<index_t> assignments; 
	assignments.clear();

	// the distributions
	std::vector<NormalDistribution> tables; 
	tables.clear();

	// initialize
	index_t current_table_index;
	NormalDistribution tmp_table;
	for (int i = 0; i < observations.size(); i++) {
		index_t assignment;
		MetropolisHastingsStep(assignments, tables, tmp_table, current_table_index, ss, observations[i], true,
				assignment);
		assignments.push_back(assignment);
	}
		
	dobots::print(assignments.begin(), assignments.end());
	
	dobots::debug << "Number of assignments is " << assignments.size() << " (and should be " << 
		observations.size() << ")" << std::endl;

	int steps = 200;
	int R = 3;
	for (int t = 0; t < steps; t++) {
		//std::debug << "Step " << t << std::endl;
		// going over table assignments and reassign
		size_t M = assignments.size()-1;
		current_table_index = assignments.front();
		assignments.erase(assignments.begin());
		bool is_new_table = false;
		for (int i = 0; i < M; i++) {
			index_t assignment;
			for (int j = 0; j < R; j++) {
				MetropolisHastingsStep(assignments, tables, tables[current_table_index], current_table_index, ss, 
						observations[i], false, assignment);
				current_table_index = assignment;
			}
			//dobots::debug << i << " @" <<  current_table_index << " -> " << assignment << std::endl;
			assignments[i] = assignment;
			current_table_index = assignments[i+1];
		}
		// for last table	
		index_t assignment;
		MetropolisHastingsStep(assignments, tables, tables[current_table_index], current_table_index, ss, 
				observations[M], false, assignment);
		assignments.push_back(assignment);
		//dobots::print(assignments.begin(), assignments.end());
		
		std::set<index_t> cluster_indices(assignments.begin(), assignments.end());

		std::vector<NormalDistribution> clusters;
		clusters.clear();
		for (auto && i: cluster_indices) {
			dobots::debug << "Add table " << i << std::endl;
			clusters.push_back(tables[i]);
		}

#ifdef UPDATE_CLUSTERS
		// update clusters given observations
		std::vector<size_t> tmp(assignments.size());
		std::iota(tmp.begin(), tmp.end(), 0);
		for (auto && index: cluster_indices) {
			std::vector<size_t> cl_assignments;
			cl_assignments.clear();
			std::copy_if(tmp.begin(), tmp.end(), 
				std::back_inserter(cl_assignments),
				[=](size_t i) { return assignments[i] == index; });
			//dobots::print(cl_assignments.begin(), cl_assignments.end());

			// create Eigen matrix with corresponding observations
			matrix_t mat(cl_assignments.size(), ss.dim);
			for (int i = 0; i < cl_assignments.size(); ++i) {
				mat.row(i) = vector_t::Map(observations[cl_assignments[i]].data(), ss.dim) ;
			}
			// std::cout << mat << std::endl;

			// calculate maximum likelihood (for Gaussian), sample mean and variance
			vector_t mean = mat.colwise().mean().transpose();

			dobots::error << "Following is incorrect. Use sample mean and variance for Gibbs sampling instead!" << std::endl;
			tables[index].mean = mean;

			//std::cout << "Mean: " << mean << std::endl;

			// update tables
			//std::cout << "Currently: " << tables[index].mean.transpose() << std::endl;
			if (mat.rows() > 1) {
				matrix_t centered = mat.rowwise() - mat.colwise().mean();
				matrix_t cov = (centered.adjoint() * centered) / value_t(mat.rows() - 1);
				// determinant might be very small, if accidently there are many points on the same location
				// this will lead to blow ups, so don't update the covar in that case
				if (cov.determinant() > 0.001) {
					tables[index].covar = cov;
				}
			}
		}
#endif
		dobots::debug << "Number of tables: " << tables.size() << std::endl;
		for (auto && i : tables) {
			dobots::debug << "Parameters (mean): " << i.mean.transpose() << std::endl;
		}

		dobots::info << "Number of clusters: " << clusters.size() << std::endl;
		for (auto && i : clusters) {
			dobots::info << "Parameters (mean): " << t << " " << i.mean.transpose() << " " << i.covar.format(VectorFormat) << std::endl;
		}
	}



#else
	int *count;
	count = readGenerate();
	if (count && *count) {
		std::vector<value_t> assignments; assignments.clear();
		ChineseRestaurantProcess CRP(alpha);
		CRP.CreateAssignments(*count, assignments);
		std::vector<int> crp; crp.clear();
		for (int i = 0; i < assignments.size(); i++) {
			crp.push_back((int)assignments[i]);
		}
		writeCRP(crp);
	}

	std::vector<value_t> *observation;
	observation = readObservation();
	if (observation) {
		vector_t v(observation->size());
		v = vector_t::Map(observation->data(), observation->size());
		//vector_t v = vector_t::Map(observation->data(), observation->size());
		observations.push_back(v);
	}

	int *train;
	train = readDoTrain();
	if (train && *train) {
		dobots::debug << "Start training" << std::endl;
		SufficientStatistics ss;
		ss.dim = 2;
		ss.kappa = 1;
		ss.mu = vector_t(ss.dim);
		ss.mu << 0, 0;
		ss.nu = 4;
		ss.lambda = matrix_t::Identity(ss.dim,ss.dim);
	
		Initialization(ss);
		int steps = 10;
		Run(ss, steps);

	}
#endif
}

/*
 * Metropolis Hasting step calculate an acceptance probability. If the probability 
 */
void DirichletModuleExt::MetropolisHastingsStep(const std::vector<index_t> & assignments, 
		std::vector<NormalDistribution> & tables, const NormalDistribution &current_distribution, 
		const index_t current_table_index, const SufficientStatistics & ss, const vector_t & observation, 
		bool accept_all, index_t & assignment) {
	if (tables.empty()) {
		dobots::debug << "Might be fine" << std::endl;
	}
	bool is_new_table;
	index_t table;
	index_t last_table = tables.size()-1;
	chinese_restaurant_process.NextAssignment(assignments, last_table, table, is_new_table);
	if (is_new_table) {
		// sample from G_0
		NormalDistribution new_distribution;
		SampleNormalInverseWishart(ss, new_distribution);
		if (tables.size() != table) {
			dobots::error << "Table " << table << " should be last table in vector" << std::endl;
		}
		dobots::debug << "Calculate acceptance" << std::endl;
		if (accept_all || Acceptance(new_distribution, current_distribution, observation)) {
			dobots::debug << "Add " << new_distribution.mean.transpose() << " to table with index " << table << std::endl;
			tables.push_back(new_distribution);
			assignment = table;
		} else {
			dobots::debug << "Forget new table" << std::endl;
			last_table--;
			assignment = current_table_index;
		}
	} else {
		// move to another, existing, table?
		if (accept_all || Acceptance(tables[table], current_distribution, observation)) {
			assignment = table;
		} else {
			assignment = current_table_index;
		}
	}
}

/**
 * Calculate the acceptance probability in the Metropolis-Hastings step. 
 */
bool DirichletModuleExt::Acceptance(const NormalDistribution &nd_proposed, 
		const NormalDistribution &nd_old, const vector_t observation) {
	value_t nom = Likelihood(nd_proposed, observation);
	value_t denom = Likelihood(nd_old, observation);
	value_t a = std::min((value_t)1, nom/denom);
	value_t random = drand48();
	bool accept = a > random;
	return (a > random);
}

/**
 * The Stop function checks when its time to stop. We use a simple flag that we set in the Tick function.
 */
bool DirichletModuleExt::Stop() {
	return stopping_flag;
}

/**
 * Initialization of the "tables". The first table is equipped with a prior from the normal inverse Wishart
 * distribution. For the next tables the mean and covariance matrix are sampled using the parameters of the preceding
 * tables.
 */
void DirichletModuleExt::Initialization(const SufficientStatistics & ss) {
	dobots::debug << "Initialization" << std::endl;
	NormalDistribution nd;
	PosteriorDensity(ss, observations[0], nd);
	thetas.push_back(nd);
	dobots::debug << "Initialization of further observations" << std::endl;
	for (int i = 1; i < observations.size(); i++) {
		NormalDistribution theta;
		GibbsStep(ss, thetas, alpha, observations[i], theta);
		if (!theta.mean.rows() )
			std::cerr << "Gibbs step to calculate the mean is incorrect! Input:"
				<< " ss.mu " << ss.mu 
				<< " thetas.size " << thetas.size() << std::endl;
		thetas.push_back(theta);
	}
}

/**
 * This function is called after initialization and performs the Gibbs iterations. At the moment also debug information
 * will be output (if the verbosity level is high enough) about the clusters every Gibbs step.
 *
 * For the data structures we use just as many parameters (thetas) as we have observations. A lot of observation refer
 * to the same parameter, but we account for that with having duplicates.
 */
void DirichletModuleExt::Run(const SufficientStatistics & ss, size_t iterations) {
	dobots::debug << "====================================================================" << std::endl;
	dobots::debug << "================================ Run ===============================" << std::endl;
	dobots::debug << "====================================================================" << std::endl;
	if (thetas.empty()) {
		std::cerr << "Theta array shouldn't be empty" << std::endl;
		return;
	}
	size_t M = observations.size()-1;
	for (int t = 1; t < iterations; t++) {
		dobots::debug << "Number of thetas is " << thetas.size() << " (and should be " << 
			observations.size() << ")" << std::endl;
		// the following is a nice design pattern, let's call it the "cavity pattern"
		// remove first theta, so we iterate through [1, 2, 3, ..] -> [0, 2, 3, ..] -> [0, 1, 3, ..], etc.
		thetas.erase(thetas.begin());
		for (int i = 0; i < M; i++) {
			// note that thetas[i] writes the result to location i 
			GibbsStep(ss, thetas, alpha, observations[i], thetas[i]);
		}	
		// last observation, needs to be added to the vector
		NormalDistribution theta;
		GibbsStep(ss, thetas, alpha, observations[M], theta);
		thetas.push_back(theta);

		// Plot or print current partition, copy vector it into a set to make parameters unique
		std::set<NormalDistribution> clusters(thetas.begin(), thetas.end());

		dobots::debug << "Number of thetas: " << thetas.size() << std::endl;
		for (auto && i : thetas) {
			dobots::debug << "Parameters (mean): " << i.mean.transpose() << std::endl;
		}

		dobots::info << "Number of clusters: " << clusters.size() << std::endl;
		for (auto && i : clusters) {
			dobots::info << "Parameters (mean): " << t << " " << i.mean.transpose() << " " << i.covar.format(VectorFormat) << std::endl;
		//	dobots::info << "Parameters (mean): " << t << " " << i.mean.transpose() << std::endl;
		}
}
}

/**
 * Update the sufficient statistics (the hyperparameters) that describe the Normal Inverse Wishart distribution. A 
 * single observation changes these parameters in a closed form manner.
 *
 * Do not use the same sufficient statistics object as input as well as output.
 */
void DirichletModuleExt::UpdateSufficientStatistics(const SufficientStatistics & ss_in, vector_t observation,
		SufficientStatistics & ss_out) {
	ss_out.dim = ss_in.dim;
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

	// parameters for the t-distribution
	matrix_t S = ss.lambda * (ss.kappa + 1) / ( ss.kappa * ( ss.nu - ss.dim + 1) );
	value_t nu = ss.nu - ss.dim + 1;
	vector_t mu = ss.mu;
	value_t Snupi = (S * nu * ss.dim).determinant();
	value_t c = std::exp(lgamma((nu+ss.dim)/2)-lgamma(nu/2)) * std::pow(Snupi, -0.5);
	vector_t diff = (observation - mu);
	value_t term = (diff.transpose() * S.inverse() * diff); 
	value_t scatter = std::pow(1+term/nu, -(nu+ss.dim)/2);
	posterior_predictive = scatter * c;
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
	if (!nd.mean.rows()) {
		std::cerr << "Mean should have values" << std::endl;
		return 1;
	}
	const vector_t diff = (nd.mean - observation);
	value_t dim = observation.size();
	// return normal distribution
	matrix_t inverse = nd.covar.inverse();
	value_t exponent = -0.5 * diff.transpose() * inverse * diff;
	value_t det = nd.covar.determinant();
	value_t normalization = std::pow( 2*M_PI * det, -0.5*dim ) ; // check how to get det out of llt
	return normalization * std::exp(exponent);
}

/**
 * Get new normal distribution given observation and sufficient statistics (from before that observation).
 */
void DirichletModuleExt::PosteriorDensity(const SufficientStatistics & ss, const vector_t & observation, 
	       NormalDistribution & nd) {
	dobots::debug << "Posterior Density" << std::endl;
	SufficientStatistics ss_out;
	UpdateSufficientStatistics(ss, observation, ss_out);
	dobots::debug << "Hyperparameter mu updated from " << ss.mu.transpose() << " to " << ss_out.mu.transpose() << std::endl;
	SampleNormalInverseWishart(ss_out, nd);
	dobots::debug << "Theta mean becomes " << nd.mean.transpose() << std::endl;
	if (!nd.mean.rows() )
		std::cerr << "Sampling of the mean is incorrect! Input:"
			<< " ss.mu " << ss.mu 
			<< " nd.covar / ss.kappa" << nd.covar / ss.kappa << std::endl;
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
	SampleInverseWishart(ss, nd.covar);
	SampleMultivariateNormal(ss.mu, nd.covar/ss.kappa, nd.mean);
}

/**
 * Generate a vector (e.g. a mean) using a multivariate normal distribution.
 */
void DirichletModuleExt::SampleMultivariateNormal(const vector_t & mean, const matrix_t & S, vector_t & sample) {
	EigenMultivariateNormal<value_t> normX_solver(mean, S);
	sample = normX_solver.samples(1);
	//dobots::debug << "Sample from " << mean.transpose() << " with covar " << S.format(VectorFormat) << " becomes: " 
	//	<< sample.transpose() << std::endl;
}

/**
 * Generate a matrix (e.g. a covariance matrix) using the hyperparameters given by the Inverse Wishart.
 */
void DirichletModuleExt::SampleInverseWishart(const SufficientStatistics & ss, matrix_t & S) {
	// zero-mean normal
	vector_t zeromean = vector_t::Zero(ss.dim);
	// huh, we take ss.lambda.inverse() every time? then who not ss.lambda_inv stored instead?
	EigenMultivariateNormal<value_t> normX_solver(zeromean, ss.lambda.inverse());
	matrix_t samples = normX_solver.samples(ss.nu);
	matrix_t iS = samples*samples.transpose();
	dobots::debug << "New precision matrix: " << iS.format(VectorFormat) << std::endl;
	S = iS.inverse();
}

/**
 * Helper function which generates likelihoods with respect to all parameters theta given an observation. This is 
 * convenient because of the sum in equation 2 in Neal (where we need all these terms).
 */
void DirichletModuleExt::Likelihoods(const std::vector<NormalDistribution> & thetas, const vector_t & observation,
		std::vector<value_t> & likelihoods) {
//	dobots::debug << "Likelihoods" << std::endl;
	likelihoods.clear();
	//likelihoods.resize(thetas.size());
	for (auto && i : thetas) {
		//dobots::debug << "Calculate likelihood for theta " << i.mean.transpose() << std::endl;
		value_t ll = Likelihood(i, observation);
		likelihoods.push_back(ll);
	}
}

/**
 * Perform a single Gibbs step. 
 *
 * Notations used in the literature
 *     dispersion_factor     alpha (Neal2000), A_0 (Escobar1994)
 *
 * Calculates:
 *  1.) likelihoods F(y_i,\theta_j)
 *  2.) posterior predictives \int F(y_i,\theta) dG_0(\theta)
 *  3-5.) calculate denominator (or ratio considering sum of likelihoods and alpha factor)
 *  6.) if new table, calculate posterior density H_i 
 *  7.) if old table, sample from one of the old densities \sum{i\neq j} q_{i,j} \delta(\theta_j)
 *
 * @param                    ss [in], sufficient statistics for normal-inverse wishart distribution (4 parameters)
 * @param                    thetas_without_k [in], all other normal distributions (each 2 parameters)
 * @param                    dispersion_factor [in], often alpha (or A_0), governs the Dirichlet Process
 * @param                    observation [in], the new observation
 * @param                    theta_k [out], the updated normal distribution as a result of the Gibbs sampling step
 */
void DirichletModuleExt::GibbsStep(const SufficientStatistics & ss, 
		const std::vector<NormalDistribution> & thetas_without_k, 
		const value_t dispersion_factor, const vector_t & observation, 
		NormalDistribution & theta_k) {
	
	dobots::debug << "Gibbs step" << std::endl;
	// 1. calculate likelihoods	
	std::vector<value_t> likelihoods;
	Likelihoods(thetas_without_k, observation, likelihoods);
	//dobots::debug << "Likelihoods: " << std::endl;
	//dobots::print(likelihoods.begin(), likelihoods.end());

	// 2. calculate posterior predictive of observation given a gaussian distribution
	value_t posterior_predictive;
	PosteriorPredictive(ss, observation, posterior_predictive);
	dobots::debug << "Unnormalized posterior predictive is: " << posterior_predictive << std::endl;

	// 3. calculate denominator
	value_t sum_likelihoods = value_t(0);
	for (auto & i : likelihoods) {
		sum_likelihoods += i;
	}
	value_t Z = sum_likelihoods + dispersion_factor * posterior_predictive;
	dobots::debug << "Sum of all likelihoods is: " << sum_likelihoods << std::endl;

	// 4. calculate probability of a new "table", a new gaussian distribution
	value_t prob_new = (dispersion_factor * posterior_predictive) / Z;

	// 5. pick a uniform number between 0 and 1
	value_t u = drand48(); // <!- todo, pick proper random generator

	dobots::debug << "Compare " << prob_new << " with " << u << std::endl;
	// 6. assign from new table 
	if (u < prob_new) {
		PosteriorDensity(ss, observation, theta_k);
		dobots::debug << "Create new table with i.e. param " << theta_k.mean.transpose() << std::endl;
	}
	// 7. assign from old table
	else {
		// 8. inverse transform sample to use a uniform number to pick item from normal distribution set
		value_t its = u - prob_new;
		dobots::debug << "Check for probability: " << its << std::endl;
		std::vector<value_t> cumsum_likelihoods;
		cumsum_likelihoods.resize(likelihoods.size());
		std::partial_sum(likelihoods.begin(), likelihoods.end(), cumsum_likelihoods.begin());
		// divide each item (we can also compare with a multiplication factor)
		dobots::debug << "Mult factor: " << Z << std::endl;
		std::transform(cumsum_likelihoods.begin(), cumsum_likelihoods.end(), 
				cumsum_likelihoods.begin(), std::bind1st(std::multiplies<value_t>(), value_t(1)/Z));
		value_t end = cumsum_likelihoods.back();
		//dobots::debug << "Probability " << prob_new << " should be 1-" << end << " = " << (1-end) << std::endl;
		//dobots::debug << "Cumulative likelihoods: " << std::endl;
		//dobots::print(cumsum_likelihoods.begin(), cumsum_likelihoods.end());
		value_t index = -1;
		for (auto it = cumsum_likelihoods.begin(); it != cumsum_likelihoods.end(); ++it) {
			// 9. pick the item when uniform number exceeds value in the cumulative density function
			if (*it >= its) {
				// likelihoods in step 1 have to be ordered exactly the same as the thetas_without_k
				index = std::distance(cumsum_likelihoods.begin(), it);
				dobots::debug << "Item " << index << " is first item with cumulative prob above " << its << std::endl;
				dobots::debug << "Thetas, size: " << thetas_without_k.size() << std::endl;
				theta_k = thetas_without_k[index];
				break;
			}
		}
		if (index >= 0) {
			dobots::debug << "Picked table: " << index << std::endl;
		} else {
			std::cerr << "Error! Cumulative sum not 1?" << std::endl;
		}
	}
}
