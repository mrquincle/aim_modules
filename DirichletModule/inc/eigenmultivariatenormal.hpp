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
	Matrix<V,Eigen::Dynamic,Eigen::Dynamic> covar;
	Matrix<V,Eigen::Dynamic,Eigen::Dynamic> transform;
	Matrix<V,Eigen::Dynamic,1> mean;
	internal::V_normal_dist_op<V> randN; // Gaussian functor

public:
	EigenMultivariateNormal(const Matrix<V,Eigen::Dynamic,1>& mean,
			const Matrix<V,Eigen::Dynamic,Eigen::Dynamic>& covar) {
		setMean(mean);
		setCovar(covar);
	}

	void setMean(const Matrix<V,Eigen::Dynamic,1>& mean) {
		this->mean = mean;
	}

	void setCovar(const Matrix<V,Eigen::Dynamic,Eigen::Dynamic>& covar)	{
		this->covar = covar;
		SelfAdjointEigenSolver<Matrix<V,Eigen::Dynamic,Eigen::Dynamic> > eigenSolver(covar);
		transform = eigenSolver.eigenvectors()*eigenSolver.eigenvalues().cwiseMax(0).cwiseSqrt().asDiagonal();
	}

	/// Draw nn samples from the gaussian and return them as columns in a mean.cols() x n matrix
	Matrix<V,Eigen::Dynamic,-1> samples(int n) {
		return (transform * Matrix<V,Eigen::Dynamic,-1>::NullaryExpr(Eigen::Dynamic,n,randN)).colwise() + mean;
	}

}; // end class EigenMultivariateNormal

} // end namespace Eigen

#endif
