/**
 * 456789------------------------------------------------------------------------------------------------------------120
 *
 * @brief ...
 * @file ExpectationMaximization.h
 * 
 * This file is created at Almende B.V. and Distributed Organisms B.V. It is open-source software and belongs to a
 * larger suite of software that is meant for research on self-organization principles and multi-agent systems where
 * learning algorithms are an important aspect.
 *
 * This software is published under the GNU Lesser General Public license (LGPL).
 *
 * It is not possible to add usage restrictions to an open-source license. Nevertheless, we personally strongly object
 * against this software being used for military purposes, factory farming, animal experimentation, and "Universal
 * Declaration of Human Rights" violations.
 *
 * Copyright (c) 2013 Anne C. van Rossum <anne@almende.org>
 *
 * @author    Anne C. van Rossum
 * @date      Oct 1, 2013
 * @project   Replicator 
 * @company   Almende B.V.
 * @company   Distributed Organisms B.V.
 * @case      Clustering
 */

#ifndef EXPECTATIONMAXIMIZATION_H_
#define EXPECTATIONMAXIMIZATION_H_

#include <Eigen/LU>
#include <cmath>
#include <cassert>
#include <cstdlib>
#include <assert.hpp>
#include <iomanip>

#include <map>

/**
 * Let's use expectation-maximization for a Gaussian mixture model: P(x|\theta_i,p_k)=\sum_1^K w_k p(x|\theta_k). So,
 * the latter term is Gaussian p(x|\theta_k) = p(x|\mu_k,\Sigma_k), in the usual exponential form.
 */
class ExpectationMaximization {
public:
	//! Define a matrix as a dense Eigen matrix of floats.
	typedef float value_t;
	typedef Eigen::Matrix<value_t,Eigen::Dynamic,Eigen::Dynamic> matrix_t;
	typedef Eigen::Matrix<value_t,Eigen::Dynamic,1> vector_t; // column_vector

	struct Pair {
		Pair(size_t ground_truth) {
			this->prediction = 0;
			this->ground_truth = ground_truth;
		}
		size_t prediction;
		size_t ground_truth;
	};

	// The hidden variables are these that define a Gaussian, we also add a weight for the mixture model
	struct Gaussian {
		matrix_t covariance;
		vector_t mean;
		value_t weight;
		std::vector<size_t> r_data;
	};

	unsigned long long rdtsc(){
	    unsigned int lo,hi;
	    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
	    return ((unsigned long long)hi << 32) | lo;
	}

	ExpectationMaximization(int K, int D) {
		long int seed = rdtsc();
//		seed = 51196996379962;
		std::cout << "Use seed " << seed << std::endl;
		srand(seed);

		mixture_model.resize(K);
		for (int k = 0; k < K; ++k) {
			init(k, D);
		}
	
		for (int k = 0; k < mixture_model.size(); ++k) {
			std::cout << "Init mean of model " << k << ": " << mixture_model[k].mean.transpose() << std::endl;
		}
		for (int k = 0; k < mixture_model.size(); ++k) {
			std::cout << "Init covariance of model " << k << ": " << std::endl << mixture_model[k].covariance << std::endl;
		}
		initialized = false;
	}

	~ExpectationMaximization() {}

	/**
	 * Add a sample, size is given as separate parameter, so it is possible to take only the first N items from the
	 * sample rather than the entire vector.
	 */
	void addSample(std::vector<value_t> & x, size_t label, size_t size = 0) {
		if (!size) size = x.size();
		ASSERT_LEQ(size, x.size());
		vector_t v(size); v.setZero();
		v = vector_t::Map(x.data(), size, 1); // create column-vector
		data_set.push_back(v);
		labels.push_back(Pair(label));
	}

	void init() {
		if (initialized) {
			std::cerr << "Already initialized. Not doing it again." << std::endl;
			return;
		}
		size_t S = data_set.size();
		probabilities.clear();
		probabilities.resize(S);
		for (int i = 0; i < S; ++i) {
			probabilities[i].clear();
			probabilities[i].resize(mixture_model.size());
		}
		initialized = true;
	}

	void tick() {
		assert (mixture_model.size());

		if (!initialized) {
			std::cerr << "Not yet initialized, skip clustering step." << std::endl;
			return;
		}

		calculate_probabilities();
		for (int k = 0; k < mixture_model.size(); ++k) {
			calculate(k);
		}
	}

	void evaluate() {
		for (int k = 0; k < mixture_model.size(); ++k) {
			mixture_model[k].r_data.clear();
		}

		for (int i = 0; i < data_set.size(); ++i) {
			labels[i].prediction = generated_by(i);
		}

		size_t a, b, c, d; a = b = c = d = 0;
		for (int i = 0; i < data_set.size(); ++i) {
			for (int j = i+1; j < data_set.size(); ++j) {
				if (labels[i].ground_truth == labels[j].ground_truth) {
					if (labels[i].prediction == labels[j].prediction) {
						a++;
					} else {
						c++;
					}
				} else {
					if (labels[i].prediction == labels[j].prediction) {
						d++;
					} else {
						b++;
					}
				}
			}
		}

		value_t quality = (a+b) / value_t(a+b+c+d);
		std::cout << "Rand index is " << quality << std::endl;
	}

	void print() {
		// print clusters
		std::vector< std::vector <size_t> > lab;
		lab.resize(mixture_model.size());
		for (int i = 0; i < mixture_model.size(); ++i) {
			lab[i].resize(mixture_model.size());
		}

		for (int l = 0; l < labels.size(); ++l) {
			assert (labels[l].ground_truth-1 >= 0);
			assert (labels[l].ground_truth-1 < mixture_model.size());
			lab[labels[l].ground_truth-1][labels[l].prediction]++;
		}

		std::cout << "Create label matrix of ground truth x prediction (" << mixture_model.size() << "x" << mixture_model.size()
				<< ")" << std::endl;
		for (int i = 0; i < lab.size(); ++i) {
			std::cout << "label " << i << ": ";
			for (int j = 0; j < lab[i].size(); ++j) {
				std::cout << std::setw(3) << std::fixed << lab[i][j] << ' ';
			}
			std::cout << std::endl;
		}

		std::cout << "Final weights of mixture models: ";
		for (int k = 0; k < mixture_model.size(); ++k) {
			std::cout << mixture_model[k].weight << ' ';
		}
		std::cout << std::endl;

		for (int k = 0; k < mixture_model.size(); ++k) {
			std::cout << "Mean of model " << k << ": " << mixture_model[k].mean.transpose() << std::endl;
		}

		for (int k = 0; k < mixture_model.size(); ++k) {
			std::cout << "Final covariance of model " << k << ": " << std::endl << mixture_model[k].covariance << std::endl;
		}

	}

	void test() {
		int dim = 2;
		vector_t mean(dim);
		for (int j = 0; j < dim; j++) {
			mean[j] = 0;
		}
		for (int i = 0; i < 1000; i++) {
			vector_t x(dim);
			for (int j = 0; j < dim; j++) {
				x[j] = drand48() * 4 -2;
			}
			matrix_t covariance = matrix_t::Identity(dim,dim);
			value_t result = gaussian_kernel(x, mean, covariance);
			for (int j = 0; j < dim; j++) {
				std::cout << x[j] << ',';
			}
			std::cout << result << std::endl;
		}
	}

protected:

	// e.g. http://www.hindawi.com/journals/mpe/2013/757240/
	void init(int k, int d) {
		assert (mixture_model.size());
		mixture_model[k].r_data.clear();
		mixture_model[k].mean.setRandom(d);
		mixture_model[k].mean = (mixture_model[k].mean.array() + 1) / 2;

//		for (int i = 0; i < 3; i++) {
//			mixture_model[k].mean[i] = 0;
//		}
//		mixture_model[k].mean[k%3] = 1;

		mixture_model[k].covariance = matrix_t::Identity(d, d);
		mixture_model[k].covariance *= 0.1;
		assert (mixture_model.size() != 0);
		mixture_model[k].weight = 1.0/mixture_model.size();
	}

	/**
	 * Calculate Gaussian kernel between x and \mu (the mean) given the covariance \Sigma.
	 *
	 *   f(x_1,...x_d) = 1/(sqrt((2*PI)^d |\Sigma| )) * exp( -1/2 * (x-\mu)' \Sigma^-1 (x-\mu ) )
	 *
	 * Returns a scalar
	 *
	 * @todo: Test this function for a covariance matrix with a determinant of zero. And for a vector x that is exactly equal to the
	 * mean.
	 */
	value_t gaussian_kernel(const vector_t & x, const vector_t & mean, const matrix_t & covariance) {
		value_t det = fabs(covariance.determinant());
		assert (det >= 0.0);
		assert (mean.size());
		value_t sq = std::pow(2.0*M_PI,mean.size()) * det ;
		assert (sq >= 0.0);
		value_t factor = std::sqrt( sq );
#ifdef VERBOSE
		std::cout << "Multiply by " << (value_t(1)/factor) << " which is 1/sqrt(" << std::pow(2.0*M_PI,mean.size())
		<< "*" << det << ")" << std::endl;
#endif

		value_t result = (value_t(1)/factor) *
				std::exp( (value_t)((x-mean).transpose() * covariance.inverse() * (x-mean)) * value_t(-0.5) );

#ifdef VERBOSE
		std::cout << "Gaussian distance between" << mean.transpose() << " and data " << x.transpose() <<
				" is " << result << std::endl;
#endif

		int large_number = 1000;
		if ((result != result) || (result > large_number)) {
#ifdef VERBOSE
			std::cout << "Return " << large_number << " as large number" << std::endl;
#endif
			return 1000;
		}
		return result;
	}

	/**
	 * This is the expectation step. It calculates the probability that data item "i" is indeed generated by kernel
	 * "k". Use it in lock-step with the maximization step.
	 */
	void generated_by(int i, std::vector<value_t> &clusters) {
		assert (i < data_set.size());

		// calculate the contribution to "i" for every model
		for (int m = 0; m < mixture_model.size(); ++m) {
//			std::cout << "Covariance: " << std::endl << mixture_model[m].covariance << std::endl;
			clusters[m] = mixture_model[m].weight *
					gaussian_kernel(data_set[i], mixture_model[m].mean, mixture_model[m].covariance);

//			std::cout << "Generated " << i << " by cluster " << m << " as " << clusters[m] <<
//					" from weight " << mixture_model[m].weight <<
//					std::endl;
		}
	}


	void calculate_probabilities() {
#ifdef VERBOSE
		std::cout << "---------------------------------------------------" << std::endl;
		for (int k = 0; k < mixture_model.size(); ++k) {
			std::cout << "Calculate for cluster " << k << std::endl;
			std::cout << "Weight " << mixture_model[k].weight << std::endl;
			std::cout << "Mean " << mixture_model[k].mean.transpose() << std::endl;
			std::cout << "Covariance " << std::endl << mixture_model[k].covariance << std::endl;
			std::cout << " with determinant " << mixture_model[k].covariance.determinant() << std::endl;
		}
#endif
		for (int i = 0; i < probabilities.size(); ++i) {
			generated_by(i, probabilities[i]);
		}

		for (int i = 0; i < probabilities.size(); ++i) {
			value_t sum = 0;
			ASSERT_EQ(mixture_model.size(), probabilities[i].size());
			for (int k = 0; k < probabilities[i].size(); ++k) {
				sum += probabilities[i][k];
			}
			if (sum != 0) {
				for (int k = 0; k < probabilities[i].size(); ++k) {
					probabilities[i][k] = probabilities[i][k] / sum;
					ASSERT_LEQ(probabilities[i][k], 1.0);
					ASSERT_GEQ(probabilities[i][k], 0.0);
				}
#ifdef EXCESSIVE_TESTS
				sum = 0;
				for (int k = 0; k < probabilities[i].size(); ++k) {
					sum += probabilities[i][k];
				}
				ASSERT_ALMOST_EQ(sum, 1, 0.01);
#endif
			}

#ifdef VERBOSE
			std::cout << "Calculated probability of " << data_set[i].transpose() << " (for cluster k=[0..." << probabilities[i].size()-1 << "]): [ ";
			for (int k = 0; k < probabilities[i].size(); ++k) {
				std::cout << std::setw(4) << std::fixed << std::setprecision(3) << probabilities[i][k] << ' ';
			}
			std::cout << ']' << std::endl;
#endif
		}
	}

	int generated_by(int i) {
		int k = 0, k_max = 0;
		value_t max = probabilities[i][k];
		for (k = 1; k < probabilities[i].size(); ++k) {
			if (probabilities[i][k] > max) {
				max = probabilities[i][k];
				k_max = k;
			}
		}
		mixture_model[k_max].r_data.push_back(i);
		return k_max;
	}

	/**
	 * Calculate for cluster k the new weight, mean, and covariance.
	 */
	void calculate(int k) {
		assert (data_set.size() != 0);

		value_t sum_w = 0;
		size_t d = data_set[0].size();
//		size_t d = probabilities.size();
		vector_t sum_mu(d);
		matrix_t sum_sigma(d,d);

		sum_mu.setZero();
		sum_sigma.setZero();
#ifdef TWEAK
		sum_sigma.setIdentity();
		sum_sigma *= 0.1;
#endif

		for (int i = 0; i < data_set.size(); ++i) {
			sum_w += probabilities[i][k];
			sum_mu += (probabilities[i][k]*data_set[i]);
		}

#ifdef VERBOSE
		std::cout << "Weight becomes: " << sum_w << " / " << data_set.size() << std::endl;
#endif
		mixture_model[k].weight = sum_w / (value_t)data_set.size();

#ifdef TWEAK
		value_t threshold = 1.0/(mixture_model.size() * 10);
		if (mixture_model[k].weight < threshold) {
			mixture_model[k].weight = threshold;
		}
#endif

		if (sum_w != 0) {
			mixture_model[k].mean = sum_mu / sum_w;
		} else {
			mixture_model[k].mean = sum_mu;
		}

		for (int i = 0; i < data_set.size(); ++i) {
			ASSERT_LEQ(probabilities[i][k], 1.0);
			ASSERT_GEQ(probabilities[i][k], 0.0);
			matrix_t m = (data_set[i] - mixture_model[k].mean) * (data_set[i] - mixture_model[k].mean).transpose();
			sum_sigma += (m * probabilities[i][k]);
		}

		if (sum_w != 0) {
			mixture_model[k].covariance = sum_sigma / sum_w;
		} else {
			mixture_model[k].covariance = sum_sigma;
		}

	}


	bool converged() {
	// check difference between mean and next mean for example, if it becomes small, quit
		return false;
	}

private:
	// there are K clusters, in this case a "mixture of k models"
	std::vector<Gaussian> mixture_model;

	// the data set
	std::vector<vector_t> data_set;

	// storing the labels to check later what matches
	std::vector<Pair> labels;

	//! Per data item store the probability that it belongs to a given cluster k
	std::vector<std::vector<value_t> > probabilities;

	bool initialized;
};


#endif /* EXPECTATIONMAXIMIZATION_H_ */
