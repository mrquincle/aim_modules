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
#include <vector>
#include <set>
#include <cmath>
#include <cstddef>

/**
 * The k-means algorithm, also called Lloyd's algorithm. A generalization is the Linde-Buzo-Gray algorithm for vector
 * quantization, see https://en.wikipedia.org/wiki/Linde%E2%80%93Buzo%E2%80%93Gray_algorithm
 */
class KMeans {
public:
	typedef float value_t;
	typedef Eigen::Matrix<value_t,1,Eigen::Dynamic> vector_t; // row_vector

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
	};

	KMeans(int K, int D) {
		srand(time(NULL));

		clusters.resize(K);
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
		assert (size <= x.size());
		vector_t v(size); v.setZero();
		v = vector_t::Map(x.data(), 1, size); // create row-vector
		data_set.push_back(v);
		labels.push_back(Pair(label));
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
		assert (clusters.size());
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
		assert ((nearest.k >= 0) && (nearest.k < clusters.size()));
		clusters[nearest.k].r_data.push_back(i);
	}

	/**
	 * Calculate centroid of observations in cluster "k". Currently we keep adding till we summed all items belonging
	 * to this cluster. The division over the number of clusters is at the end.
	 */
	void update(int k) {
		assert ((k >= 0) && (k < clusters.size()));
		if (clusters[k].r_data.size() == 0) return;
		clusters[k].mean.setZero(); // set to 0 for all values
		for (int i = 0; i < clusters[k].r_data.size(); ++i) {
			vector_t t = data_set[clusters[k].r_data[i]];
			clusters[k].mean += t;
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

	std::vector<Pair> labels;
};


#endif /* KMEANS_H_ */
