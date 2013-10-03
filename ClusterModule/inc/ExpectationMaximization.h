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

	ExpectationMaximization(int K, int D) {
		srand(time(NULL));

		mixture_model.resize(K);
		for (int k = 0; k < K; ++k) {
			init(k, D);
		}
	}

	~ExpectationMaximization() {}

	/**
	 * Add a sample, size is given as separate parameter, so it is possible to take only the first N items from the
	 * sample rather than the entire vector.
	 */
	void addSample(std::vector<value_t> & x, size_t label, size_t size = 0) {
		if (!size) size = x.size();
		assert (size <= x.size());
		vector_t v(size); v.setZero();
		v = vector_t::Map(x.data(), size, 1); // create column-vector
		data_set.push_back(v);
		labels.push_back(Pair(label));
	}

	void tick() {
		assert (mixture_model.size());

		calculate_probabilities();
		for (int k = 0; k < mixture_model.size(); ++k) {
			calculate(k);
		}
	}

	void evaluate() {
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
		for (int k = 0; k < mixture_model.size(); ++k) {
			std::set<size_t> l;
			l.clear();
			for (int i = 0; i < mixture_model[k].r_data.size(); ++i) {
				l.insert(labels[mixture_model[k].r_data[i]].ground_truth);
			}
			std::cout << "Cluster " << k << " corresponds with ";
			if (l.size()) {
				std::set<size_t>::iterator iter;
				for (iter = l.begin(); iter != l.end(); ++iter) {
					std::cout << *iter << " ";
				}
			} else {
				std::cout << "nothing";
			}
			std::cout << std::endl;
		}

		// print means
		for (int k = 0; k < mixture_model.size(); ++k) {
			std::cout << "Mean of model " << k << ": " << mixture_model[k].mean.transpose() << std::endl;
		}
	}


protected:

	// e.g. http://www.hindawi.com/journals/mpe/2013/757240/
	void init(int k, int d) {
		assert (mixture_model.size());
		mixture_model[k].r_data.clear();

//		int pick = rand() % data_set.size();
//		mixture_model[k].mean = data_set[pick];
		mixture_model[k].mean.setRandom(d);
		mixture_model[k].mean = (mixture_model[k].mean.array() + 1) / 2;

		mixture_model[k].covariance = matrix_t::Identity(d, d);
		assert (mixture_model.size() != 0);
		mixture_model[k].weight = 1.0/mixture_model.size();
	}

	/**
	 * Calculate Gaussian kernel between x and \mu (the mean) given the covariance \Sigma.
	 *
	 *   f(x_1,...x_d) = 1/(sqrt((2*PI)^d |\Sigma| )) * exp( -1/2 * (x-\mu)' \Sigma^-1 (x-\mu ) )
	 *
	 * Returns a scalar
	 */
	value_t gaussian_kernel(const vector_t & x, const vector_t & mean, const matrix_t & covariance) {
		value_t det = covariance.determinant();
		if (!(det >= 0)) {
			std::cout << "Determinant of a covariance matrix should be positive " << det << std::endl;
			det = fabs(det);
		}
		value_t sq = std::pow(2*M_PI,mean.size()) * det ;
		assert (sq >= 0);
		value_t factor = std::sqrt( sq );
		assert (factor != 0);

		return (value_t(1)/factor) *
				std::exp( (value_t)((x-mean).transpose() * covariance.inverse() * (x-mean)) * value_t(-0.5) );
	}

	/**
	 * This is the expectation step. It calculates the probability that data item "i" is indeed generated by kernel
	 * "k". Use it in lock-step with the maximization step.
	 */
	void generated_by(int i, std::vector<value_t> &clusters) {
		assert (i < data_set.size());
		clusters.clear();
		clusters.resize(mixture_model.size());

		// calculate the contribution to "i" for every model
		for (int m = 0; m < mixture_model.size(); ++m) {
			clusters[m] = mixture_model[m].weight *
					gaussian_kernel(data_set[i], mixture_model[m].mean, mixture_model[m].covariance);
		}

		// and compare that with model "k"
//		value_t result = (mixture_model[k].weight *
//				gaussian_kernel(data_set[i], mixture_model[k].mean, mixture_model[k].covariance)) / sum;
//		std::cout << "Generated " << i << " by cluster " << k << " with prob " << result << std::endl;
//		return result;
	}

	void calculate_probabilities() {
		probabilities.clear();
		probabilities.resize(data_set.size());
		for (int i = 0; i < probabilities.size(); ++i) {
			generated_by(i, probabilities[i]);
		}

		for (int i = 0; i < probabilities.size(); ++i) {
			value_t sum(0);
			for (int k = 0; k < probabilities[i].size(); ++k) {
				sum += probabilities[i][k];
			}
			if (sum != 0) {
				for (int k = 0; k < probabilities[i].size(); ++k) {
					probabilities[i][k] = probabilities[i][k] / sum;
					assert (probabilities[i][k] <= 1.0);
					assert (probabilities[i][k] >= 0.0);
				}
			}
		}

	}

	int generated_by(int i) {
		int k = 0, k_min = 0;
		value_t min = probabilities[i][k];
		for (k = 1; k < probabilities[i].size(); ++k) {
			if (probabilities[i][k] < min) {
				min = probabilities[i][k];
				k_min = k;
			}
		}
		mixture_model[k_min].r_data.push_back(i);
		return k_min;
	}

	void calculate(int k) {
//		std::cout << "Calculate cluster " << k << std::endl;
		value_t sum_w = 0;
		// check if default construction goes well for vector/matrix
		size_t d = data_set[0].size();
		vector_t sum_mu(d);
		matrix_t sum_sigma(d,d);

		sum_mu.setZero();
		sum_sigma.setZero();

		for (int i = 0; i < data_set.size(); ++i) {
			sum_w += probabilities[i][k];
			sum_mu += probabilities[i][k]*data_set[i];
		}
		assert (data_set.size() != 0);
		mixture_model[k].weight = sum_w / data_set.size();
		assert (sum_w != 0);
		mixture_model[k].mean = sum_mu / sum_w;

		for (int i = 0; i < data_set.size(); ++i) {
			assert (probabilities[i][k] >= 0);
			matrix_t m = (data_set[i] - mixture_model[k].mean)*(data_set[i] - mixture_model[k].mean).transpose();
			assert (m.determinant() == 0);
			sum_sigma += m * probabilities[i][k];
		}

		std::cout << "Cluster " << k << " weight: " << mixture_model[k].weight << ", sum of prob " << sum_w << std::endl;
	}


	bool converged() {
	// check difference between mean and next mean for example, if it becomes small, quit
		return false;
	}

private:
	std::vector<Gaussian> mixture_model;

	std::vector<vector_t> data_set;

	std::vector<Pair> labels;

	std::vector<std::vector<value_t> > probabilities;

};


#endif /* EXPECTATIONMAXIMIZATION_H_ */
