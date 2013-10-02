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
#include <cmath>
#include <cstddef>

/**
 * The k-means algorithm, also called Lloyd's algorithm. A generalization is the Linde-Buzo-Gray algorithm for vector
 * quantization, see https://en.wikipedia.org/wiki/Linde%E2%80%93Buzo%E2%80%93Gray_algorithm
 */
class KMeans {
public:
	typedef float value_t;
	typedef Eigen::Matrix<value_t,Eigen::Dynamic,Eigen::Dynamic> vector_t;

	// Cluster has a mean and references a subset of vectors in the data set
	struct Cluster {
		vector_t mean;
		std::vector<vector_t*> data_set;
	};

	KMeans(int K) {
		srand(time(NULL));

		clusters.resize(K);
		for (int k = 0; k < K; ++k) {
			init(k);
		}

	}

	~KMeans() {}


	void init(int k) {
		clusters[k].data_set.clear();
		// clusters[k].mean =
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
		for (int k = 1; k < clusters.size(); ++k) {
			next.dist = (data_set[i] - clusters[k].mean).squaredNorm();
			next.k = k;
			if (next.dist < nearest.dist) next.swap(nearest);
		}
		assert ((nearest.k >= 0) && (nearest.k < clusters.size()));
		clusters[nearest.k].data_set.push_back(&data_set[i]);
	}

	/**
	 * Calculate centroid of observations in cluster "k". Currently we keep adding till we summed all items belonging
	 * to this cluster. The division over the number of clusters is at the end.
	 */
	void update(int k) {
		assert ((k >= 0) && (k < clusters.size()));
		if (clusters[k].data_set.size() == 0) return; // perhaps, pick another random mean for k...
		clusters[k].mean = 0; // set to 0 for all values
		for (int i = 0; i < data_set.size(); ++i) {
			clusters[k].mean += *clusters[k].data_set[i];
		}
		clusters[k].mean /= clusters[k].data_set.size();
	}

	void clear(int k) {
		assert ((k >= 0) && (k < clusters.size()));
		// delete all data references in clusters
		clusters[k].data_set.clear();
	}
private:
	std::vector<Cluster> clusters;

	std::vector<vector_t> data_set;
};


#endif /* KMEANS_H_ */
