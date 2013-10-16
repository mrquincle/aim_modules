/**
 * 456789------------------------------------------------------------------------------------------------------------120
 *
 * @brief ...
 * @file kMeans.h
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
 * @date      Oct 2, 2013
 * @project   Replicator 
 * @company   Almende B.V.
 * @company   Distributed Organisms B.V.
 * @case      Clustering
 */

#ifndef KMEANS_H_
#define KMEANS_H_

#include <Eigen/LU>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <vector>
#include <set>
#include <cmath>
#include <cstddef>
#include <assert.hpp>

template<typename _Tp>
struct sqr : public std::unary_function<_Tp, _Tp> {
	_Tp operator()(const _Tp& __x) const
	{ return __x * __x; }
};

/**
 * The k-means algorithm, also called Lloyd's algorithm. A generalization is the Linde-Buzo-Gray algorithm for vector
 * quantization, see https://en.wikipedia.org/wiki/Linde%E2%80%93Buzo%E2%80%93Gray_algorithm
 */
class KMeans {
public:
	typedef float value_t;
	typedef Eigen::Matrix<value_t,1,Eigen::Dynamic> vector_t; // row_vector
	typedef Eigen::Matrix<value_t,Eigen::Dynamic,Eigen::Dynamic> matrix_t; // matrix
	typedef Eigen::Matrix<value_t,Eigen::Dynamic,1> column_vector_t;

	typedef Eigen::Matrix<size_t,1,Eigen::Dynamic> index_vector_t; // row

	struct Pair {
		Pair(size_t ground_truth) {
			this->prediction = 0;
			this->ground_truth = ground_truth;
		}
		size_t prediction;
		size_t ground_truth;
	};

	// Cluster has a mean and references a subset of vectors in the data set
	struct Cluster {
		vector_t mean;
		std::vector<size_t> r_data;
//		index_vector_t r_data;
	};

	void test() {
		// in Eigen broadcasting can be used, see
		// http://stackoverflow.com/questions/18403478/how-can-i-apply-bsxfun-like-functionality-at-eigen
		// however, a pairwise distance between elements in two matrices, as in
		// http://www.mathworks.nl/help/stats/pdist2.html I cannot find

		matrix_t data(2,4);
		matrix_t means(2,2);

		// data points
		data <<	1, 23, 6, 9,
				3, 11, 7, 2;

		// means
		means << 2, 20,
				 3, 10;

		std::cout << "Data: " << std::endl;
		std::cout << data.replicate(2,1) << std::endl;

		column_vector_t temp1(4);
		temp1 = Eigen::Map<column_vector_t>(means.data(),4);

		std::cout << "Means: " << std::endl;
		std::cout << temp1.replicate(1,4) << std::endl;

		matrix_t temp2(4,4);
		temp2 = (data.replicate(2,1) - temp1.replicate(1,4));
		std::cout << "Differences: " << std::endl;
		std::cout << temp2 << std::endl; // .minCoeff(&index);

		matrix_t temp3(2,8);
		temp3 = Eigen::Map<matrix_t>(temp2.data(),2,8);
		std::cout << "Remap to 2xF: " << std::endl;
		std::cout << temp3 << std::endl;

		matrix_t temp4(1,8);
		temp4 = temp3.colwise().squaredNorm();
		std::cout << "Squared norm: " << std::endl;
		std::cout << temp4 << std::endl;//.minCoeff(&index);

		matrix_t temp5(2,4);
		temp5 = Eigen::Map<matrix_t>(temp4.data(),2,4);
		std::cout << "Squared norm result: " << std::endl;
		std::cout << temp5.transpose() << std::endl;

//		matrix_t::Index x, y;
		std::cout << "Result: " << std::endl;
		std::cout << temp5.transpose().colwise().minCoeff() << std::endl;


	}

	unsigned long long rdtsc(){
		unsigned int lo,hi;
		__asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
		return ((unsigned long long)hi << 32) | lo;
	}

	KMeans(int K, int D) {
		long int seed = rdtsc();
		// seed = 58564383378988; gives 0.8737
		std::cout << "Use seed " << seed << std::endl;
		srand(seed);
		clusters.resize(K);
		distances.resize(K);
		for (int k = 0; k < K; ++k) {
			init(k, D);
		}
	}

	~KMeans() {}

	void tick() {
		for (int k = 0; k < clusters.size(); ++k) {
			clear(k);
		}
		for (int i = 0; i < data_set.size(); ++i) {
			assign(i);
		}
		for (int k = 0; k < clusters.size(); ++k) {
			update(k);
		}
	}

	/**
	 * Add a sample, size is given as separate parameter, so it is possible to take only the first N items from the
	 * sample rather than the entire vector.
	 */
	void addSample(std::vector<value_t> & x, size_t label, size_t size = 0) {
		if (!size) size = x.size();
		ASSERT_LEQ (size, x.size());
		vector_t v(size); v.setZero();
		v = vector_t::Map(x.data(), 1, size); // create row-vector
		data_set.push_back(v);
		labels.push_back(Pair(label));
	}

	void init() {
		size_t D = data_set.size();
	}

	std::vector<Pair> & result() {
		return labels;
	}

	void evaluate() {
		for (int k = 0; k < clusters.size(); ++k) {
			for (int i = 0; i < clusters[k].r_data.size(); ++i) {
				labels[clusters[k].r_data[i]].prediction = k;
				//				std::cout << "Cluster " << k << ", data item " << clusters[k].r_data[i] << std::endl;
			}
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
		//std::cout << "Rand index is (" << a << "+" << b << ") /" << a+b+c+d << " = " << quality << std::endl;
		std::cout << "Rand index is " << quality << std::endl;
	}

	/**
	 * Call evaluate before this function
	 */
	void print() {
		// print clusters
		for (int k = 0; k < clusters.size(); ++k) {
			std::set<size_t> l;
			l.clear();
			for (int i = 0; i < clusters[k].r_data.size(); ++i) {
				l.insert(labels[clusters[k].r_data[i]].ground_truth);
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
		for (int k = 0; k < clusters.size(); ++k) {
			std::cout << clusters[k].mean << std::endl;
		}
	}
protected:
	void init(int k, int d) {
		clusters[k].r_data.clear();
		clusters[k].mean.setRandom(d);
		clusters[k].mean = (clusters[k].mean.array() + 1) / 2;
	}

	void assign(int i) {
		ASSERT (clusters.size());

//#define CALCULATE_MIN_IN_LOOP

#ifdef CALCULATE_MIN_IN_LOOP
		struct NearestCluster {
			value_t dist;
			size_t k;
			void swap(NearestCluster &other) {
				NearestCluster temp;
				temp.dist = dist; temp.k = k;
				dist = other.dist; k = other.k;
				other.dist = temp.dist; other.k = temp.k;
			}
		} nearest, next;
		int k = 0;
		nearest.dist = (data_set[i] - clusters[k].mean).squaredNorm();
		nearest.k = k;
		for (k = 1; k < clusters.size(); ++k) {
			next.dist = (data_set[i] - clusters[k].mean).squaredNorm();
			next.k = k;
			if (next.dist < nearest.dist) next.swap(nearest);
		}
		ASSERT ((nearest.k >= 0) && (nearest.k < clusters.size()));
		clusters[nearest.k].r_data.push_back(i);
#else // is shorter, but actually slower
		for (int k = 0; k < clusters.size(); ++k) {
			distances[k] = (data_set[i] - clusters[k].mean).squaredNorm();
		}
		int nearest_k = std::min_element(distances.begin(), distances.end()) - distances.begin();
		ASSERT ((nearest_k >= 0) && (nearest_k < clusters.size()));
		clusters[nearest_k].r_data.push_back(i);
#endif
	}

	/**
	 * Calculate centroid of observations in cluster "k". Currently we keep adding till we summed all items belonging
	 * to this cluster. The division over the number of clusters is at the end.
	 */
	void update(int k) {
		ASSERT ((k >= 0) && (k < clusters.size()));
		if (clusters[k].r_data.size() == 0) return;
		clusters[k].mean.setZero(); // set to 0 for all values
		for (int i = 0; i < clusters[k].r_data.size(); ++i) {
			clusters[k].mean += data_set[clusters[k].r_data[i]];
		}
		clusters[k].mean /= clusters[k].r_data.size();
	}

	// delete all data references in clusters
	void clear(int k) {
		clusters[k].r_data.clear();
	}
private:
	std::vector<Cluster> clusters;

	std::vector<vector_t> data_set;

	std::vector<value_t> distances;

	std::vector<Pair> labels;
};


#endif /* KMEANS_H_ */
