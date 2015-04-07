/**
 * 456789------------------------------------------------------------------------------------------------------------120
 *
 * @brief Multi-variate normal distribution using the Eigen library.
 * @file eigenmultivariatenormal.hpp
 * 
 * From: http://lost-found-wandering.blogspot.nl/2011/05/sampling-from-multivariate-normal-in-c.html
 * and:  http://stackoverflow.com/questions/16361226/error-while-creating-object-from-templated-class
 *
 * Adapted by Anne van Rossum to account for dynamic arrays.
 *
 * @author    Joseph (JCooper)
 * @author    Anne van Rossum
 * @date      Mar 27, 2011
 */

#ifndef EIGENMULTIVARIATENORMAL_HPP
#define EIGENMULTIVARIATENORMAL_HPP

#include <Eigen/Dense>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>

/*
  We need a functor that can pretend it's const,
  but to be a good random number generator
  it needs mutable state.  The standard Eigen function
  Random() just calls rand(), which changes a global
  variable.
 */
namespace Eigen {

namespace internal {

template<typename V>
struct V_normal_dist_op
{
	static boost::mt19937 rng;                        // The uniform pseudo-random algorithm
	mutable boost::normal_distribution<V> norm;  // The gaussian combinator

	EIGEN_EMPTY_STRUCT_CTOR(V_normal_dist_op);

	template<typename Index>
	inline const V operator() (Index, Index = 0) const { return norm(rng); }
};

template<typename V>
boost::mt19937 V_normal_dist_op<V>::rng;

template<typename V>
struct functor_traits<V_normal_dist_op<V> >
{ enum { Cost = 50 * NumTraits<V>::MulCost, PacketAccess = false, IsRepeatable = false }; };

} // end namespace internal

/**
    Find the eigen-decomposition of the covariance matrix
    and then store it for sampling from a multi-variate normal
 */
template<typename V>
class EigenMultivariateNormal
{
	typedef Eigen::Matrix<V,Eigen::Dynamic,Eigen::Dynamic> matrix_t;
	typedef Eigen::Matrix<V,Eigen::Dynamic,1> vector_t;

	matrix_t covar;
	matrix_t transform;
	vector_t mean;
	internal::V_normal_dist_op<V> randN; // Gaussian functor
	size_t size;

public:
	EigenMultivariateNormal(const vector_t& mean, const matrix_t& covar) {
		setMean(mean);
		setCovar(covar);
	}

	void setMean(const vector_t & mean) {
		//std::cout << "Set mean to " << mean << std::endl;
		this->mean = mean;
		size = mean.rows();
	}

	void setCovar(const matrix_t & covar)	{
		//std::cout << "Set covariance to " << covar << std::endl;
		this->covar = covar;
		SelfAdjointEigenSolver<matrix_t > eigenSolver(covar);
		transform = eigenSolver.eigenvectors() * 
			eigenSolver.eigenvalues().cwiseMax(0).cwiseSqrt().asDiagonal();
	}

	/// Draw nn samples from the gaussian and return them as columns in a mean.cols() x n matrix
	vector_t samples(int n) {
		//std::cout << "Get " << n << " samples " << std::endl;
		vector_t result = (transform
				//* vector_t::NullaryExpr(Eigen::Dynamic,n,randN)).colwise() 
				* vector_t::NullaryExpr(size,n,randN)).colwise() 
				+ mean;
		//std::cout << "Result: " << result << std::endl;
		return result;
	}

	// Todo: draw single sample

}; // end class EigenMultivariateNormal

} // end namespace Eigen

#endif
