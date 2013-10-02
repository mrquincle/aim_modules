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

/**
 * Let's use expectation-maximization for a Gaussian mixture model: P(x|\theta_i,p_k)=\sum_1^K w_k p(x|\theta_k). So,
 * the latter term is Gaussian p(x|\theta_k) = p(x|\mu_k,\Sigma_k), in the usual exponential form.
 */
class ExpectationMaximization {
public:
	//! Define a matrix as a dense Eigen matrix of floats.
	typedef float value_t;
	typedef Eigen::Matrix<value_t,Eigen::Dynamic,Eigen::Dynamic> matrix_t;
	typedef Eigen::Matrix<value_t,Eigen::Dynamic,Eigen::Dynamic> vector_t;

	// The hidden variables are these that define a Gaussian, we also add a weight for the mixture model
	struct Gaussian {
		matrix_t covariance;
		vector_t mean;
		value_t weight;
	};

	ExpectationMaximization() {
		srand(time(NULL));
	}

	~ExpectationMaximization() {}


	void run(int K) {
		for (int k = 0; k < K; ++k) {
			init(k);
		}
		int T = 1; // timespan
		for (int t = 0; t < T || converged(); ++t) {
			for (int k = 0; k < K; ++k) {
				calculate(k);
			}
		}
	}

	// e.g. http://www.hindawi.com/journals/mpe/2013/757240/
	void init(int k) {
		int pick = rand() % data_set.size();
		mixture_model[k].mean = data_set[pick];
		mixture_model[k].covariance = matrix_t::Identity(data_set[pick].size(), data_set[pick].size());
		assert (mixture_model.size() != 0);
		mixture_model[k].weight = 1.0/mixture_model.size();
	}

	/**
	 * Calculate Gaussian kernel between x and \mu (the mean) given the covariance \Sigma.
	 */
	value_t gaussian_kernel(const vector_t & x, const vector_t & mean, const matrix_t & covariance) {
		return value_t(1)/(std::sqrt( std::pow(2*M_PI,mean.size()) * covariance.determinant() ) ) *
				std::exp( (-(x-mean).adjoint() * covariance.inverse() * (x-mean))  / 2 );
	}

	/**
	 * This is the expectation step. It calculates the probability that data item "i" is indeed generated by kernel
	 * "k". Use it in lock-step with the maximization step.
	 */
	value_t generated_by(int i, int k) {
		assert (i < data_set.size());
		assert (k < mixture_model.size());

		// calculate the contribution to "i" for every model
		value_t sum = 0;
		for (int m = 0; m < mixture_model.size(); ++m) {
			sum += mixture_model[m].weight *
					gaussian_kernel(data_set[i], mixture_model[m].mean, mixture_model[m].covariance);
		}

		// and compare that with model "k"
		return (mixture_model[k].weight *
				gaussian_kernel(data_set[i], mixture_model[k].mean, mixture_model[k].covariance)) / sum;
	}

	void calculate(int k) {
		value_t sum_w = 0;
		// check if default construction goes well for vector/matrix
		vector_t sum_mu;
		matrix_t sum_sigma;

		std::vector<value_t> p;
		p.reserve(data_set.size());

		for (int i = 0; i < data_set.size(); ++i) {
			p[i] = generated_by(i,k);
			sum_w += p[i];
			sum_mu += p[i]*data_set[i];
		}
		assert (data_set.size() != 0);
		mixture_model[k].weight = sum_w / data_set.size();
		assert (sum_w != 0);
		mixture_model[k].mean = sum_mu / sum_w;

		for (int i = 0; i < data_set.size(); ++i) {
			sum_sigma += p[i]*(data_set[i] - mixture_model[k].mean)*(data_set[i] - mixture_model[k].mean).transpose();
		}

		mixture_model[k].covariance = sum_sigma / sum_w;
	}


	bool converged() {
	// check difference between mean and next mean for example, if it becomes small, quit
		return false;
	}

private:
	std::vector<Gaussian> mixture_model;

	std::vector<vector_t> data_set;

};


#endif /* EXPECTATIONMAXIMIZATION_H_ */