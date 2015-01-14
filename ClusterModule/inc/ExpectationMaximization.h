/**
 * 456789------------------------------------------------------------------------------------------------------------120
 *
 * @brief An Expectation-Maximization algorithm
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
#include <Print.hpp>

#include <map>

#define LINE_DETECTION
//#define VERBOSE
#define ASSIGNMENT

#define TEST_START_FROM_GROUND_TRUTH


/**
 * Let's use expectation-maximization for a Gaussian mixture model: P(x|\theta_i,p_k)=\sum_1^K w_k p(x|\theta_k). So,
 * the latter term is Gaussian p(x|\theta_k) = p(x|\mu_k,\Sigma_k), in the usual exponential form.
 */
class ExpectationMaximization {
	public:
		//! Define a matrix as a dense Eigen matrix of floats.
		typedef double value_t;
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
			vector_t beta;
			value_t weight;
			std::vector<size_t> r_data;
		};

		/*	struct Shape {
			value_t weight;
		// value_t scale; // later on, we can make the line larger...
		//	value_t pnt0; // end point of line
		//      value_t pnt1; // end point of line
		std::vector<size_t> r_data;
		};
		*/
		// To generate a random seed
		unsigned long long rdtsc(){
			unsigned int lo,hi;
			__asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
			return ((unsigned long long)hi << 32) | lo;
		}

		/**
		 * The constructor requires a fixed number of clusters "K" and a fixed number of data items "D". It is a batch
		 * process.
		 */
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
				std::cout << "Init beta of model " << k << ": " << mixture_model[k].beta.transpose() << std::endl;
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
		void addSample(std::vector<float> & x, size_t label, size_t size = 0) {
			if (!size) size = x.size();
			ASSERT_LEQ(size, x.size());
			vector_t v(size); 
			v.setZero();
			std::vector<double> x_double(x.begin(), x.end());
			//v = vector_t::Map(x.data(), size, 1);
			v = vector_t::Map(x_double.data(), size, 1); // create column-vector
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
#ifdef ASSIGNMENT
			assignments.clear();
			assignments.resize(S);
			for (int i = 0; i < S; ++i) {
				assignments[i].clear();
				assignments[i].resize(mixture_model.size());
			}
#endif
			initialized = true;
			//testClosest();
		}

		void testClosest() {
			vector_t beta(2);
			beta[0] = 2; 
			beta[1] = 1;
			vector_t x(2);
			x[0] = 1;
			x[1] = 2.5;
			vector_t cl(2);
			closest(beta, x, cl);
			std::cout << "Closest point: (" << cl[0] << "," << cl[1] << ")" << std::endl;
			std::cout << "  should be (0.75, 2.75)" << std::endl;
		}

		/**
		 * Main function, each time one additional cycle calculates new probabilities per data point, and subsequently
		 * new update for each cluster "k".
		 */
		void tick() {
			assert (mixture_model.size());

			if (!initialized) {
				std::cerr << "Not yet initialized, skip clustering step." << std::endl;
				return;
			}
			static int once = 1;
			if (!once) {	
				for (int i = 0; i < data_set.size(); i++) {
					std::cout << "Sample[i=" << i << "]: " << data_set[i][0] << "," << data_set[i][1] << std::endl;
					std::cout << "Label: " << labels[i].ground_truth << std::endl;
				}
				once++;
			}
#ifdef VERBOSE
			std::cout << "Expectation step" << std::endl;
#endif
			calculate_probabilities();
#ifdef VERBOSE
			std::cout << "Maximization step" << std::endl;
#endif

#ifdef TEST_START_FROM_GROUND_TRUTH
			// for testing, if we start with right probabilities, will it work? it assumes first half of dataset is k=0, other half is k=1
			if (once == 1) {
				std::cout << "Set probabilities beforehand, to test if solution diverges from the ground truth" << std::endl;
				for (int k = 0; k < mixture_model.size(); ++k) {
					for (int i = 0; i < data_set.size()/2; ++i) {
						probabilities[i][k] = (1-k)*0.70 + 0.15;
					}
					for (int i = data_set.size()/2; i < data_set.size(); ++i) {
						probabilities[i][k] = k*0.70 + 0.15;
					}
				}
				//
				once++;
			}
#endif

#ifdef ASSIGNMENT
			for (int i = 0; i < data_set.size(); ++i) {
				int k = 0, k_max = 0;
				value_t max = probabilities[i][k];
				for (k = 1; k < probabilities[i].size(); ++k) {
					if (probabilities[i][k] > max) {
						max = probabilities[i][k];
						k_max = k;
					}
				}

				for (k = 0; k < probabilities[i].size(); ++k) {
					if (k == k_max) {
						assignments[i][k] = 1;
					} else {
						assignments[i][k] = 0;
					}
				}
			}
			std::cout << "Assignments: " ;
			for (int i = 0; i < data_set.size(); ++i) {
				dobots::print(assignments[i].begin(), assignments[i].end());
			}
#endif 

			for (int k = 0; k < mixture_model.size(); ++k) {
				calculate(k);
			}
		}

		/**
		 * After enough ticks feel free to evaluate the algorithm. In this case the Rand index is implemented. This 
		 * takes into account false positives, false negatives, etc.
		 */
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

		// small test function with 1000 data points
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
			//		std::cout << "Initialize k=" << k << " and d=" << d << std::endl;
			assert (mixture_model.size());
			mixture_model[k].r_data.clear();
			mixture_model[k].mean.setRandom(d);
			mixture_model[k].mean = (mixture_model[k].mean.array() + 1) / 2;
			mixture_model[k].beta.setRandom(d);

#ifdef TEST_START_FROM_GROUND_TRUTH
			std::cout << "Set beta to ground truth" << std::endl;
			if (k == 0) {
				mixture_model[k].beta[0] = 2;
				mixture_model[k].beta[1] = 1;
			} else {
				mixture_model[k].beta[0] = -1;
				mixture_model[k].beta[1] = -1;
			}
#endif
			mixture_model[k].covariance = matrix_t::Identity(d, d);
			//		mixture_model[k].covariance *= 0.1;
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
		 *
		 * Add an option to only use diagonal covariance matrices
		 */
		value_t gaussian_kernel(const vector_t & x, const vector_t & mean, const matrix_t & covariance) {
			value_t det = fabs(covariance.determinant());
			assert (det >= 0.0);
			assert (mean.size());
			if (det < 0.0001) {
				std::cout << "Determinant is zero, means that the matrix is rank deficient" << std::endl;
				std::cout << "This means that the random variables are perfectly correlated" << std::endl;
			}
			value_t sq = std::pow(2.0*M_PI,mean.size()) * det ;
			assert (sq >= 0.0);
			value_t factor = std::sqrt( sq );
#ifdef VERBOSE
			//	std::cout << "Multiply by " << (value_t(1)/factor) << " which is 1/sqrt(" << std::pow(2.0*M_PI,mean.size())
			//	<< "*" << det << ")" << std::endl;
#endif

			value_t result = (value_t(1)/factor) *
				std::exp( (value_t)((x-mean).transpose() * covariance.inverse() * (x-mean)) * value_t(-0.5) );

#ifdef VERBOSE
			std::cout << "Gaussian distance between (" << mean.transpose() << ") and data (" << x.transpose() <<
				") is " << result << std::endl;
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
		 * G((\tau+1)/2)/( G(\tau/2) sqrt(pi*\tau) )
		 */
		value_t t_distribution_Gamma_factor(const value_t tau) {
			value_t result = sqrt(tau);
			int itau = (int)tau;
			if (itau % 2 == 0) {
				// even
				//value_t nominator = 1, denominator = sqrt(tau)*2;
				result *= 2;
				for (int i = 0; i < tau/2-2; i++) {
					result *= ((tau - (2*i + 1)) / ( tau - (2*i+2)));
				}
				//for (int j = 2; j < tau; j+=2) {
				//	demoninator *= j;
				//}
				//for (int j = 3; j < tau; j+=2) {
				//	nominator *= j;
				//}
			} else {
				// odd
				result *= M_PI;
				for (int i = 0; i < (tau-1)/2-1; i++) {
					result *= ((tau - (2*i + 1)) / ( tau - (2*i+2)));
				}
				//value_t nominator = 1, denominator = sqrt(tau)*M_PI;
				//for (int j = 3; j < tau; j+=2) {
				//	demoninator *= j;
				//}
				//for (int j = 2; j < tau; j+=2) {
				//	nominator *= j;
				//}

			}
			return result;
		}

		/**
		 * The non-standardized Student's t-distribution has a location / scale parameter (as with a Gaussian) plus a degrees
		 * of freedom parameter, \tau. It's form is quite different though (although it converges to a Gaussian with 
		 * dof=\infty.
		 *
		 *   f(x_1,...x_d) = G((\tau+1)/2)/( G(\tau/2) sqrt(pi*\tau) * |\Sigma| )  * ( 1 -1/\tau * (x-\mu)' \Sigma^-1 (x-\mu ) ) ^ ((-\tau+1)/2)
		 *
		 * G is the Gamma function.
		 */	
		value_t t_distribution(const vector_t & x, const vector_t &mean, const matrix_t covariance, const value_t dof) {
			value_t det = fabs(covariance.determinant());
			assert (det >= 0.0);
			assert (mean.size());
			if (det < 0.0001) {
				std::cout << "Determinant is zero, means that the matrix is rank deficient" << std::endl;
				std::cout << "This means that the random variables are perfectly correlated" << std::endl;
			}
			value_t sq = std::pow(2.0*M_PI,mean.size()) * det ;
			assert (sq >= 0.0);
			value_t factor = std::sqrt( sq );
#ifdef VERBOSE
			//	std::cout << "Multiply by " << (value_t(1)/factor) << " which is 1/sqrt(" << std::pow(2.0*M_PI,mean.size())
			//	<< "*" << det << ")" << std::endl;
#endif
			value_t result = (value_t(1)/factor) * t_distribution_Gamma_factor(dof) *
				std::pow(1 + 1/dof * (value_t)((x-mean).transpose() * covariance.inverse() * (x-mean)) , -(dof+1)/2);

#ifdef VERBOSE
			std::cout << "Student-t distance between (" << mean.transpose() << ") and data (" << x.transpose() <<
				") is " << result << std::endl;
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

#ifndef LINE_DETECTION

		/**
		 * This is part of the expectation step. It calculates the probability that data item "i" is indeed generated 
		 * by kernel "k". Use it in lock-step with the maximization step.
		 *
		 * For one item "i" it iterates over all clusters k, and calculates:
		 *   prob_cluster[k] = cluster_weight * gaussian_distance(i, k)
		 * 
		 * This function needs to be called for all items "i".
		 */
		void generated_by(int i, std::vector<value_t> &clusters) {
			assert (i < data_set.size());

			// calculate the contribution to "i" for every model
			for (int m = 0; m < mixture_model.size(); ++m) {
				//			std::cout << "Covariance: " << std::endl << mixture_model[m].covariance << std::endl;
				clusters[m] = mixture_model[m].weight *
					t_distribution(data_set[i], mixture_model[m].mean, mixture_model[m].covariance, 4);
					//gaussian_kernel(data_set[i], mixture_model[m].mean, mixture_model[m].covariance);
#ifdef VERBOSE
				//			std::cout << "Generated " << i << " by cluster " << m << " as " << clusters[m] <<
				//					" from weight " << mixture_model[m].weight <<
				//					std::endl;
#endif
			}
		}
#endif

		/**
		 * If a line passes through two points (b[0],b[1]) and (e[0],e[1]) and we write D0=e[0]-b[0], D1=e[1]-b[1], 
		 * then the distance to point x:
		 *
		 *   d = 1/sqrt(D0^2+D1^2) * | D1*x[0] - D1*x[1] - b[0]*e[1] + b[1]*e[0] |
		 *
		 * .... later: This function returns std::exp(-d*d), related to the inverse of the distance.
		 */
		value_t distance(const vector_t & x, const vector_t & b, const vector_t & e) {
			value_t D0 = e[0] - b[0];
			value_t D1 = e[1] - b[1];
			value_t diff;
			if (!D0) { // horizontal line
				diff = x[0] - b[0];
				return diff*diff;
			} else if (!D1) { // vertical line
				diff = x[1] - b[1];
				return diff*diff;
			}
			diff = D1*x[0] - D1*x[1] - b[0]*e[1] + b[1]*e[0];
			diff *= diff;
			diff = diff/(D0*D0+D1*D1);
			//return std::exp(-diff);
			return diff; // perhaps later return error (+/-), not distance (+ only)
		}

		/**
		 * Assume y = beta[0] + x*beta[1], then in notation ax+by+c=0, b=-1, so ax+c=y, thus a=beta[1], and c=beta[0]. 
		 * It is assumed that x[0] contains the x-coordinate, while x[1] contains the y-coordinate of the point. Likewise
		 * cl[0] will contain the x-coordinate of the closest point, cl[1] the y-coordinate.
		 *
		 * Function is checked. 
		 *
		 * @todo Vertical and horizontal lines
		 */
		void closest(const vector_t & beta, const vector_t &x, vector_t & cl) {
			assert (beta.size() == 2);
			assert(cl.size() == 2);
			value_t a = beta[1];
			value_t b = -1;
			value_t c = beta[0];
			value_t z = a*a + b*b;
			//std::cout << "z=" << z << std::endl;
			cl[0] = (b*(b*x[0]-a*x[1]) - a*c) / z;
			//std::cout << "B=" << b*(b*x[0]-a*x[1]) << std::endl;
			cl[1] = (a*(-b*x[0]+a*x[1]) - b*c) / z;
			//std::cout << "C=" << a*(-b*x[0]+a*x[1]) << std::endl;
			//#ifdef VERBOSE
			//		std::cout << "For line y=" << c << "+" << a << "*x the projection of x=(" << x[0] << "," << x[1] << ") is (" << cl[0] << "," << cl[1] << ")" << std::endl;
			//#endif 

		}

#ifdef LINE_DETECTION
		/**
		 * Calculate the probability that a data point is generated by each of the clusters. Hence, returns a vector 
		 * of length K, for each cluster a probability.
		 */
		void generated_by(int i, std::vector<value_t> &clusters) {
			assert(i < data_set.size());

			for (int m = 0; m < mixture_model.size(); ++m) {
				// error residual for i vs m:
				//mixture_model[m].mean = distance(data_set[i], mixture_model[m].pnt0, mixture_model[m].pnt1);
				vector_t cl(2);
				closest(mixture_model[m].beta, data_set[i], cl);
				// now use the error residual as input for the Gaussian model
				clusters[m] = mixture_model[m].weight *
					t_distribution(data_set[i], cl, mixture_model[m].covariance, 4);
//					gaussian_kernel(data_set[i], cl, mixture_model[m].covariance);
			}
		}
#endif

		/**
		 * Calculate probability for each item "i" to be generated by each of the clusters "k". This uses the Gaussian 
		 * distance function through "generated_by".
		 *
		 * The posterior probability is calculated as:
		 *
		 *   h_s(t) = w_s p_s(x; \mu_s, \Sigma_s) / { sum_i w_i p_i(x; \mu_i, \Sigma_i) }
		 *
		 * To calculate this we first calculate "w_i p_i(x; \mu_i, \Sigma_i)" for all "i" and then uses these values to 
		 * calculate the fraction.
		 *
		 * This is called the expectation step in Expectation-Maximization. 
		 *
		 * It is called here "calculate_probabilities" because the result is a NxK matrix of membership weights, where 
		 * each row sums to one.
		 */
		void calculate_probabilities() {
#ifdef VERBOSE
			std::cout << "---------------------------------------------------------------------------------------------------" << std::endl;
			for (int k = 0; k < mixture_model.size(); ++k) {
				std::cout << "Calculate for cluster " << k << std::endl;
				std::cout << "Weight " << mixture_model[k].weight << std::endl;
				std::cout << "Mean " << mixture_model[k].mean.transpose() << std::endl;
				std::cout << "Covariance " << std::endl << mixture_model[k].covariance << std::endl;
				std::cout << " with determinant " << mixture_model[k].covariance.determinant() << std::endl;
			}
#endif
			const int data_set_size = probabilities.size();
			//std::cout << "Data size: " << data_set_size << std::endl;

			// get probabilities that item "i" is generated by all of the k clusters 
			// probabilities[i] is a vector of size "k"
			// probabilities is the same size as the entire dataset of points
			//std::cout << "Get for each data point, probability that it is generated by all of the clusters" << std::endl;
			for (int i = 0; i < data_set_size; ++i) {
				generated_by(i, probabilities[i]);
			}

			// sum all "probabilities" and divide the individual "probabilities" so the result is an actual 
			// normalized probability
			//std::cout << "Sum all probabilities (per cluster) to get means" << std::endl;
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
					// for testing, check that the total sum is 1, each cluster has a part of this cake
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

		/**
		 * Get the cluster "k" by which item "i" is most likely generated. This is a simple iteration over all 
		 * probabilities and returning the kernel for which the probability is at its maximum value.
		 */
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
		 * This is the "maximization" step in the Expectation-Maximization algorithm. It assigns for each cluster k its
		 * new weight, mean, and covariance. The mixture weight is the support a specific mixture "k" has along all the
		 * data points.
		 *
		 * This does not work for line estimation. The outliers from other lines destroy the line estimation problem in
		 * such way that it is impossible in such way that the EM algorithm even diverges from the ground truth, given
		 * only a few steps. This can not be solved by using a stronger form for the Gaussian. For example a quadratic
		 * interpretation of the weights, moves indeed outliers closer to the origin. 
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
#ifdef TWEAK1
			sum_sigma.setIdentity();
			sum_sigma *= 0.1;
#endif

#ifdef ASSIGNMENT
			// introduce an assignment step, probabilities[i][k] will be set to 0 and 1
			// or perhaps 0 versus a normalized probability in a multi-component setting

#endif		
			// sum probabilities for cluster "k" for the entire dataset, and calculate its support over the entire
			// dataset
			for (int i = 0; i < data_set.size(); ++i) {
				value_t p = probabilities[i][k];
#ifdef ASSIGNMENT
				p *= assignments[i][k];
				p = assignments[i][k];
#endif
				sum_w += p;
				// incorporate (2D) point i with weight probabilities[i][k] in mean \mu
				sum_mu += (p*data_set[i]);
			}

			// mixture weight is all probabilities per data point divided by the size of the dataset
			mixture_model[k].weight = sum_w / (value_t)data_set.size();
#ifdef TWEAK2
			// tweak in case the weights drop below a certain threshold
			value_t threshold = 1.0/(mixture_model.size() * 10);
			if (mixture_model[k].weight < threshold) {
				mixture_model[k].weight = threshold;
			}
#endif

			// calculate new mean
			if (sum_w != 0) {
				mixture_model[k].mean = sum_mu / sum_w;
			} else {
				mixture_model[k].mean = sum_mu;
			}


#ifdef LINE_DETECTION
			// shift to origin
			for (int i = 0; i < data_set.size(); ++i) {
				data_set[i] -= mixture_model[k].mean;
			}

#ifdef VERBOSE
			vector_t tavg(d); tavg.setZero();
			for (int i = 0; i < data_set.size(); ++i) {
				value_t p = probabilities[i][k];
#ifdef ASSIGNMENT
				p *= assignments[i][k];
				p = assignments[i][k];
#endif
				tavg += data_set[i]*p;
			}
			std::cout << "New average should be around origin, so sum should be zero: " << tavg.transpose() << std::endl;
#endif

			// calculate covariance matrix
			value_t sxx = 0, sxy = 0, syy = 0;
			for (int i = 0; i < data_set.size(); ++i) {
				//vector_t dd = (probabilities[i][k]*data_set[i] - mixture_model[k].mean);
				value_t p = probabilities[i][k];
#ifdef ASSIGNMENT
				p *= assignments[i][k];
				p = assignments[i][k];
#endif
				vector_t dd = p*data_set[i];
				//std::cout << "diff [k=" << k << "][i=" << i << "]: " << dd.transpose() << std::endl;
				sxx += dd[0]*dd[0];
				syy += dd[1]*dd[1];
				sxy += dd[0]*dd[1];
			}
			// normalize as second-order moment
			sxx = sxx / (value_t)(data_set.size() - 1);
			syy = syy / (value_t)(data_set.size() - 1);
			sxy = sxy / (value_t)(data_set.size() - 1);

#endif


#ifdef VERBOSE
			std::cout << "weight [k=" << k << "]: " << sum_w << " / " << data_set.size() << std::endl;
			std::cout << "mean [k=" << k << "]: (" << sum_mu.transpose() << ") / " << sum_w << std::endl;
#endif

			//#define TWEAK

#ifdef LINE_DETECTION
			// calculate line coordinates
			value_t lambda = 1, sign = 1;
			value_t b = (syy - lambda*sxx); value_t a = sxy; value_t c = sxy;
			std::cout << "sxx=" << std::setprecision(9) << sxx << std::endl;
			std::cout << "syy=" << syy << std::endl;
			std::cout << "sxy=a=c=" << a << std::endl;
			std::cout << "b=" << b << std::endl;
			value_t test = b*b + 4*lambda*a*c;
			std::cout << "test=" << test << std::endl;
			// beta[1] is scale
			if (c != 0) {
				mixture_model[k].beta[1] = (b+sign*sqrt( b*b + 4*lambda*a*c)) / (2*c);
			} else {
				std::cerr << "Huh, c = 0" << std::endl;
				mixture_model[k].beta[1] = 0;
			}
			// beta[0] is the y-intersection
			mixture_model[k].beta[0] = mixture_model[k].mean[1] - mixture_model[k].beta[1] * mixture_model[k].mean[0];

			std::cout << "beta (D,s) for " << k << ": " << mixture_model[k].beta[0] << " " << mixture_model[k].beta[1] << std::endl;

			std::cout << "mean (x,y) for " << k << ": " << mixture_model[k].mean[0] << " " << mixture_model[k].mean[1] << std::endl;
#endif

			// calculate new covariance matrix
			for (int i = 0; i < data_set.size(); ++i) {
				value_t p = probabilities[i][k];
				ASSERT_LEQ(p, 1.0);
				ASSERT_GEQ(p, 0.0);
#ifdef ASSIGNMENT
				p *= assignments[i][k];
				p = assignments[i][k];
#endif
#ifdef LINE_DETECTION
				vector_t cl(2);
				closest(mixture_model[k].beta, data_set[i], cl);
				matrix_t m = (data_set[i] - cl) * (data_set[i] - cl).transpose();
#else
				matrix_t m = (data_set[i] - mixture_model[k].mean) * (data_set[i] - mixture_model[k].mean).transpose();
#endif
				sum_sigma += (m * p);
			}

			if (sum_w != 0) {
				mixture_model[k].covariance = sum_sigma / sum_w;
			} else {
				mixture_model[k].covariance = sum_sigma;
			}

#define DIAGONAL_COVARIANCE_MATRIX
#ifdef DIAGONAL_COVARIANCE_MATRIX
			//		std::cout << "Diagonalize covariance matrix (of size " << mixture_model[k].covariance.rows() << "x" 
			//			<< mixture_model[k].covariance.cols() << ")" << std::endl;
			for (int m = 0; m < mixture_model[k].covariance.rows(); ++m) {
				for (int n = 0; n < mixture_model[k].covariance.cols(); ++n) {
					if (m != n) {
						mixture_model[k].covariance(m,n) = 0;
					}
				}
			}
#endif


#ifdef LINE_DETECTION
			// shift back
			for (int i = 0; i < data_set.size(); ++i) {
				data_set[i] += mixture_model[k].mean;
			}
#endif

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

#ifdef ASSIGNMENT
		//! Per data item set "1" to cluster that it is most probably member off, "0" to other clusters
		std::vector<std::vector<value_t> > assignments;
#endif
		bool initialized;
};


#endif /* EXPECTATIONMAXIMIZATION_H_ */
