/**
 * 456789------------------------------------------------------------------------------------------------------------120
 *
 * @brief Operator that work on a STL container, such as vector or list
 * @file dim1algebra.hpp
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
 * @date      Jul 24, 2013
 * @project   Replicator
 * @company   Almende B.V.
 * @company   Distributed Organisms B.V.
 * @case      Sensor fusion
 */

#ifndef DIM1ALGEBRA_HPP_
#define DIM1ALGEBRA_HPP_

#define _GLIBCXX_CONCEPT_CHECKS

// General files
#include <cassert>
#include <algorithm>
#include <functional>
#include <numeric>
#include <iostream>
#include <cmath>
#include <iterator>

#include <sstream>
#include <limits>
#include <set>

/***********************************************************************************************************************
 * General README for dim1algebra.hpp
 **********************************************************************************************************************/

/**
 * Some general information for folks that are unaware of the standard STL functions... There are many methods
 * available that should be enough to get you started. Look at <algorithm> and <numeric> for specific implementations.
 * Here you see some examples on use:
 *
 * This multiplies all elements in a vector by a scalar "10":
 *   std::transform(vec.begin(), vec.end(), vec.begin(), std::bind1st(std::multiplies<T>(),10));
 *
 * Or point-wise multiplication of two vectors (same size):
 *   std::transform(v1.begin(), v1.end(), v2.begin(), result.begin(), std::multiplies<T>());
 * It is important to note that a result vector should have allocated enough entries beforehand. This will not be done
 * by the STL methods themselves. It is also easy to see that it is impossible for the function to check if v2 is
 * actually of the same size (or larger) than v1.
 *
 * An other important function does not return a vector of elements, but just a scalar. The one that operates on a
 * single container is called std::accumulator, the one that operates on two containers is std::inner_product, here e.g.
 * used to calculate an Euclidean distance:
 *   std::sqrt(std::inner_product(first1, last1, first2, T(0), std::plus<T>(), euclidean<T>));
 *
 * Just keep these in mind, because you will not always need to create dedicate functions, but might be well off by
 * applying the standard STL ones.
 *
 * Todo:
 *   - perhaps conform to STL standard in two ways:
 *      - have T as final template parameter rather than the first
 *      - use T init so no construction T(0) is needed
 *
 * There will not be a 2D version of this file, for that, please use something like Eigen.
 */

/***********************************************************************************************************************
 * Helper functions for 1 dimensional algebra functions for standard containers.
 **********************************************************************************************************************/

/**
 * We do not define the ASSERT_X functions if already defined before. For example gtest/gtest.h.
 */

namespace dobots {

//! Helper function for printing asserts
#ifndef ASSERT
#define ASSERT(condition) { \
		if(!(condition)){ \
			std::cerr << "ASSERT FAILED: " << #condition << " @ " << __FILE__ << " (" << __LINE__ << ")" << std::endl; \
			assert(condition); \
		} \
}
#endif

//! Helper function for printing asserts
#ifndef ASSERT_EQ
#define ASSERT_EQ(x,y) \
		if (!(x == y)) { \
			std::cerr << "ASSERT FAILED: " << #x << " == " << #y << " fails, specifically: " << x << " == " << y << \
			" fails @ " << __FILE__ << " (" << __LINE__ << ")" << std::endl; \
			assert(x == y); \
		}
#endif

#ifndef ASSERT_NEQ
#define ASSERT_NEQ(x,y) \
		if (!(x != y)) { \
			std::cerr << "ASSERT FAILED: " << #x << " != " << #y << " fails, specifically: " << x << " != " << y << \
			" fails @ " << __FILE__ << " (" << __LINE__ << ")" << std::endl; \
			assert(x != y); \
		}
#endif

#ifndef ASSERT_LT
#define ASSERT_LT(x,y) \
		if (!(x < y)) { \
			std::cerr << "ASSERT FAILED: " << #x << " < " << #y << " fails, specifically: " << x << " < " << y << \
			" fails @ " << __FILE__ << " (" << __LINE__ << ")" << std::endl; \
			assert(x < y); \
		}
#endif

#ifndef ASSERT_LEQ
#define ASSERT_LEQ(x,y) \
		if (!(x <= y)) { \
			std::cerr << "ASSERT FAILED: " << #x << " <= " << #y << " fails, specifically: " << x << " <= " << y << \
			" fails @ " << __FILE__ << " (" << __LINE__ << ")" << std::endl; \
			assert(x <= y); \
		}
#endif

#ifndef ASSERT_GT
#define ASSERT_GT(x,y) \
		if (!(x > y)) { \
			std::cerr << "ASSERT FAILED: " << #x << " > " << #y << " fails, specifically: " << x << " > " << y << \
			" fails @ " << __FILE__ << " (" << __LINE__ << ")" << std::endl; \
			assert(x > y); \
		}
#endif

#ifndef ASSERT_GEQ
#define ASSERT_GEQ(x,y) \
		if (!(x >= y)) { \
			std::cerr << "ASSERT FAILED: " << #x << " >= " << #y << " fails, specifically: " << x << " >= " << y << \
			" fails @ " << __FILE__ << " (" << __LINE__ << ")" << std::endl; \
			assert(x >= y); \
		}
#endif

/**
 * For an explanation of the different metrics, see the "distance" function below. This distance function does really
 * calculate a distance between two containers, say two vectors, and is not the std::distance function that just returns
 * the distance between elements with respect to a given iterator.
 *
 * Only metrics are defined (for now) that do not require additional information. For example, the Mahalanobis distance
 * requires the correlations between the variables as input (either by including standard deviations, or the covariance
 * matrix in the general case).
 */
enum DistanceMetric {
	DM_EUCLIDEAN,
	DM_DOTPRODUCT,
	DM_BHATTACHARYYA,
	DM_HELLINGER,
	DM_MANHATTAN,
	DM_CHEBYSHEV,
	DM_BHATTACHARYYA_COEFFICIENT,
	DM_SQUARED_HELLINGER,
	DM_TYPES
};

enum SetDistanceMetric {
	SDM_INFIMIM,
	SDM_SUPREMUM,
	SDM_HAUSDORFF,
	SDM_SUPINF,
	SDM_TYPES
};

enum Norm {
	N_EUCLIDEAN,
	N_TAXICAB,
	N_MAXIMUM,
	N_TYPES
};

enum PseudoNorm {
	PN_SUM_OF_SQUARES,
	PN_TYPES
};

enum Mean {
	M_ARITHMETIC,
	M_GEOMETRIC,
	M_HARMONIC,
	M_QUADRATIC,
	M_TYPES
};

/**
 * Somehow "std::max" is only given as a normal template function and not as a derivation of a function object derived
 * from binary_function. There does not seem to be a reason for that.
 * Background info: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2001/n1293.html
 * Hence, we need this helper function to be able to use max() as a functor.
 */
template<typename _Tp>
struct max : public std::binary_function<_Tp, _Tp, _Tp> {
	_Tp operator()(const _Tp& __x, const _Tp& __y) const
	{ return std::max(__x, __y); }
};

template<typename _Tp>
struct sqr : public std::unary_function<_Tp, _Tp> {
	_Tp operator()(const _Tp& __x) const
	{ return __x * __x; }
};

/***********************************************************************************************************************
 * Distances and pseudo-distances
 **********************************************************************************************************************/

/**
 * The discrete metric returns 1 accept when x==y (which has distance 0). This is sometimes called the p=0 "norm". It is
 * rather important in sparse representations and compressed sensing.
 */
template<typename T>
T discrete(T x, T y) {
	return ((x == y) ? 0 : 1);
}

/**
 * The p=1 norm, can be used for the Manhattan distance (but also the Chebyshev distance). This norm is a lot "less
 * smooth" than the Euclidean norm. If used for example for regression outliers do have a much higher weight compared
 * to the Euclidean norm (compared to the inliers). It is not symmetric, so not considered a real metric, at times it
 * is called a quasi-metric.
 */
template<typename T>
T taxicab(const T & x, const T & y) {
	return std::abs(x-y);
}

/**
 * The p=2 norm, element-wise squaring the difference. This norm is also called the Euclidean norm and is the most known
 * of all.
 */
template<typename T>
T euclidean(T x, T y) {
	return (x-y)*(x-y);
}

/**
 * See: http://en.wikipedia.org/wiki/Bhattacharyya_distance
 * The Battacharyya distance is related to the Battacharyya coefficient (which measures the overlap between two
 * statistical samples). It is the square root of the product of the entities involved and hence even less robust than
 * the taxicab norm for regression.
 */
template<typename T>
T battacharyya(T x, T y) {
	return std::sqrt(x*y);
}

/**
 * See: http://en.wikipedia.org/wiki/Hellinger_distance
 * Related to the Battacharyya coefficient. It takes the square root of both individual entities, calculates the
 * difference and squares that.
 */
template<typename T>
T hellinger(T x, T y) {
	T tmp = std::sqrt(x)-std::sqrt(y);
	return tmp*tmp;
}

/**
 * The hyperbolic distance is the absolute difference between the log of two variables.
 */
template<typename T>
T hyperbolic(const T & x, const T & y) {
	return std::abs(std::log(x) - std::log(y));
}

/**
 * Here x*log y is convenient for entropies etc.
 */
template<typename T>
T xlogy(const T & x, const T & y) {
	if (y == 0) return 0;
	return std::log(y) * x;
}

/**
 * The non-symmetric "distance" that takes a log of the ratio of x/y times the element itself plays a major role in
 * entropies.
 */
template<typename T>
T xlogx_over_y(const T & x, const T & y) {
	if (y == 0) return 0;
	return std::log(x/y) * x;
}

/**
 * Complicate way to multiply to entities, but convenient for, in the end, Jensen-Shannon divergence.
 */
template<typename T>
T xlogx_avg_y(const T & x, const T & y) {
	T avg = (x + y) / T(2);
	if (avg == 0) return 0;
	return std::log(x/avg) * x;
}

/***********************************************************************************************************************
 * Truncate / cap values
 **********************************************************************************************************************/

/**
 * Cap a value if it is beyond a certain range given by minimum and maximum.
 * @param x                  value to be capped
 * @param min                minimum value
 * @param max                maximum value
 * @return                   value or its minimum or maximum
 */
template<typename T>
void cap_range(T & x, T min, T max) {
	x = (x > max) ? max : ((x < min) ? min : x);
}

//! Same function as a std::unary_function.
template<typename T>
class cap_range_function: std::unary_function<T,T> {
	T min;
	T max;
public:
	cap_range_function(T min, T max): min(min), max(max) {}
	T operator()(T x) const {
		return (x > max) ? max : ((x < min) ? min : x);
	}
};

/**
 * Assume a maximum cap with a center at zero. So, abs(x) < abs(max) and likewise for y. Some examples:
 *   cap_scale<int,double>(0,120,100) results in: (x,y)=(15,100). So you see the value 120 lifted x a bit up to 15
 *   cap_scale<int,double>(-20,120,100) becomes: (0,100). The values exceeded their boundaries (0 and 100) equally.
 *   cap_scale<int,double>(50,120,100) becomes: (50,100). The value 50 is the center, it won't move.
 * @template T               type of parameters and max
 * @template F               floating type to be used for the scaling
 * @param x                  parameter x, will be adapted
 * @param y                  parameter y, will be adapted
 * @param max                maximum value (> 0)
 * @return                   void
 */
template<typename T, typename F>
void cap_scale(T & x, T & y, T max) {
	assert (max > 0);
	if (max == 0) {
		x = y = max;
		return;
	}
	if (abs(x) > max || abs(y) > max) {
		F scale = max / F(abs((abs(x) > abs(y)) ? x : y));
		x = x * scale;
		y = y * scale;
	}
}

/**
 * Caps values at max and if one of them comes above this cap, they are both scaled, such that the maximum value of each
 * of these values is at most max. Some examples:
 *   cap_scale<int,double>(50,-20,0,100) becomes: (50,0). If min=0, it is the same as cap_scale above.
 *   cap_scale<int,double>(0,-200,-100,100) becomes: (0,-100). A symmetric domain is nice for signed values.
 * This function can be used to scale commands to the wheels of a robot for example.
 * @template T               type of parameters and max
 * @template F               floating type to be used for the scaling
 * @param x                  parameter x, will be adapted
 * @param y                  parameter y, will be adapted
 * @param min                minimum value
 * @param max                maximum value (> min), can be smaller than 0 if min is smaller than 0
 */
template<typename T, typename F>
void cap_scale(T & x, T & y, T min, T max) {
	assert (max > min);
	// weird case
	if (max == min) {
		x = y = min;
		return;
	}
	// naturally maximum should be larger than minimum
	assert (max > min);
	// change coordinates from [min,max] with a shift equal to the center, [min+max]/2
	T shift = -(min + max) / 2;
	T sx = x + shift;
	T sy = y + shift;
	// now it is centered around 0, so scale properly with max value
	cap_scale<T,F>(sx, sy, max+shift);
	// now shift back
	x = sx - shift;
	y = sy - shift;
}

/**
 * The same as cap_range, but now for an STL container.
 */
template<typename InputIterator, typename T>
void cap_range(InputIterator first, InputIterator last, T min, T max) {
	std::transform(first, last, first, cap_range_function<T>(min,max));
}

template<typename T, typename U>
U scale(const T x, const T size_before, const U size_after, bool rounding = false) {
	ASSERT(size_before && size_after);
	ASSERT_LEQ(x,size_before);
	if (rounding) {
		return round((x * size_after) / size_before);
	}
	return (x * size_after) / size_before;
}

/**
 * The shrink operator as used in the literature on compressed sensing and in the wavelet literature. It is also called
 * the soft-thresholding operator.
 *   shrink(z,λ)_α = max{|z_α|−λ,0} * (z_α / |z_a|)
 * the norm used here is just absolute value, the object is a scalar anyway, so ||z_a||=|z_a|.
 */
template<typename InputIterator, typename T, typename U>
T shrink(InputIterator first, InputIterator last, const T gamma, const U alpha) {
	ASSERT_LEQ(alpha, last - first);
	T *value = first+alpha;
	return copysign(1.0,*value)*max<T>(abs(*value)-gamma, 0);
}



/***********************************************************************************************************************
 * Printing helpers
 **********************************************************************************************************************/

//! Print stuff with a delimiter
template<typename T>
class print_function: std::unary_function<T,T> {
	std::string delimiter;
public:
	print_function(std::string delimiter): delimiter(delimiter) {}
	void operator()(T x) const {
		std::cout << x << delimiter;
	}
};

template<typename InputIterator, typename T>
void print(InputIterator first, InputIterator last, std::string delimiter) {
	std::for_each(first, last, print_function<T>(delimiter));
}

/**
 * More elaborate function to print to standard out. It will accept any container for which an iterator is defined. The
 * default separator/delimiter is the comma plus a white space. The output is preceded by the length of the container in
 * square brackets. The empty container is denoted by curly brackets. By default there will be an end of line.
 *
 * @param first              First item of the range to be printed
 * @param last               Last item
 * @param delim              Optional: a string of symbols separating the items (default ", ")
 * @param empty              Optional: the string denoting an empty container (default {}"
 * @param endl               Optional: print yes/no a new line at the end (default yes)
 */
template<typename InputIterator>
void print(InputIterator first, InputIterator last, std::string delim=", ", std::string empty="{}", bool endl=true) {
	// concept requirements
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator>);
	__glibcxx_requires_valid_range(first, last);
	typedef typename std::iterator_traits<InputIterator>::value_type ValueType;
	typedef typename std::iterator_traits<InputIterator>::difference_type DistanceType;

	DistanceType dist = std::distance(first, last);
	std::cout << '[' << dist << "] ";
	switch (dist) {
	case 0:
		std::cout << empty;
		break;
	case 1:
		std::cout << *first;
		break;
	default:
		std::ostringstream ss;
		std::copy(first, last - 1, std::ostream_iterator< ValueType >(ss, delim.c_str()));
		ss << *(last - 1);
		std::cout << ss.str();
		break;
	}
	if (endl) std::cout << std::endl;
}


/***********************************************************************************************************************
 * Square, inverse, etc. of individual elements
 **********************************************************************************************************************/

/**
 * Template function for the square of a variable. In the std library only the square root is defined (as std::sqrt).
 */
template<typename T>
T square(T x) { return x * x; }

/**
 * Template function for 1/x.
 */
template<typename T>
T inverse(T x) { return T(1)/x; }

/**
 * Template function for absolute does not exist for every platform. Hence, let's just use std::abs() without template
 * parameters.
 */
template<typename T>
T absolute(T x) { return std::abs(x); }

/**
 * Product of an element with the log of itself.
 */
template<typename T>
T xlogx(T x) { return std::log(x) * x; }

/**
 * Just calculates x*(1-x) or x-x^2, a symmetric curve around 0.5, that intersects with the y-axis at 0 and +1, is
 * positive between 0 and +1, and negative out of that range. The maximum at x=1/2 is y=1/4. The integral from 0 to +1
 * is 1/6.
 */
template<typename T>
T x_times_1_minus_x(T x) { return x * (T(1)-x); }


/***********************************************************************************************************************
 * Unary functions
 **********************************************************************************************************************/

/**
 * Returns exponent x^a. Here "a" is considered to be the same type T as "x".
 */
template<typename T>
class fixed_power: std::unary_function<T,T> {
	T a;
public:
	fixed_power(T a): a(a) {}
	T operator()(T x) const {
		return std::pow(x,a);
	}
};

/***********************************************************************************************************************
 * Binary functions, such as vector multiplication with a scalar
 **********************************************************************************************************************/

/**
 * Create a template function which moves container x from or towards y with a factor "a". A positive "a" will move "x"
 * away, while a negative "a" will move "x" towards "y". This is used in for example incremental learning vector
 * quantization.
 */
template<typename T>
class op_adjust: std::binary_function<T,T,T> {
	T a;
public:
	op_adjust(T a): a(a) {}
	T operator()(T x, T y) const {
		//fancy: std::multiplies<T>( std::minus<T>(x,y), a); (in case + and * are not defined)
		return x + (x-y)*a;
	}
};

/**
 * Mixing of the two elements by using an exponent "a", result becomes x^a * y^(1-a).
 */
template<typename T>
class mixed_exponent: std::binary_function<T,T,T> {
	T a;
public:
	mixed_exponent(T a): a(a) {}
	T operator()(T x, T y) const {
		return std::pow(x,a) * std::pow(y,1-a);
	}
};

/***********************************************************************************************************************
 * Slightly adapted accumulators and max element search
 **********************************************************************************************************************/

/**
 *  @brief Accumulate values in a range with one operation for the accumulation and one operation that has to be
 *  performed on the to be accumulated element.
 *
 *  Accumulates the values in the range [first,last) using the function object @a binary_op. The initial value is
 *  @a init. The values are processed in order.
 *
 *  @param first           Start of range.
 *  @param last            End of range.
 *  @param init            Starting value to add other values to.
 *  @param binary_op       Function object to accumulate with.
 *  @param unary_op        Function object on element before accumulation.
 *  @return                The final sum.
 */
template<typename InputIterator, typename T, typename BinaryOperation, typename UnaryOperation>
inline T accumulate(InputIterator first, InputIterator last, T init,
		BinaryOperation binary_op, UnaryOperation unary_op)
{
	// concept requirements
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator>);
	__glibcxx_requires_valid_range(first, last);

	for (; first != last; ++first)
		init = binary_op(init, unary_op(*first));
	return init;
}

/**
 *  @brief  Return the maximum element in a range given one additional operation that is performed on it beforehand.
 *
 *  This is a slight change with respect to the standard STL max_element procedure. To illustrate the use of this
 *  function, consider for example a unary_op abs<T> which returns the location of the element with the maximum absolute
 *  value.
 *
 *  @ingroup sorting_algorithms
 *  @param first             Start of range
 *  @param last              The end of the range to iterate over
 *  @param unary_op          A unary operation
 *  @return                  Iterator referencing the first instance of the largest value.
 */
template<typename ForwardIterator, typename UnaryOperation>
ForwardIterator
max_element(ForwardIterator first, ForwardIterator last, UnaryOperation unary_op)
{
	// concept requirements
	__glibcxx_function_requires(_ForwardIteratorConcept<ForwardIterator>);
	//		__glibcxx_function_requires(LessThanComparableConcept<
	//				typename iterator_traits<ForwardIterator>::value_type>);
	__glibcxx_requires_valid_range(first, last);

	if (first == last)
		return first;
	ForwardIterator result = first;
	while (++first != last)
		if (unary_op(*result) < unary_op(*first))
			result = first;
	return result;
}

/**
 * The default std::inner_product comes either without any operators, or two at once. The most common way I use the
 * inner product is with default data types, so no need to construct a zero object, and with a binary operator for
 * the way stuff combines from the two vectors, not for how these are constructed to one thing in the end, which is
 * almmost always by summation.
 */
template<typename T, typename InputIterator1, typename InputIterator2, typename BinaryOperation>
inline T
inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryOperation binary_op)
{
	// concept requirements
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator1>);
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator2>);
	__glibcxx_requires_valid_range(first1,last1);;

	T init = T(0); // constructor...
	return std::inner_product(first1, last1, first2, init, std::plus<T>(), binary_op);
}

/**
 * This function tells something about the "size" or "length" of a container, mathematically called "norm".
 * There are currently several norms implemented:
 *   N_EUCLIDEAN:            return sqrt (sum_i { (x_i)^2 } ) (L2 norm)
 *   N_TAXICAB:              return sum_i { abs(x_i) } (L1 norm)
 *   N_MAXIMUM               return max_i (x_i) (maximum norm)
 * @param first              start of the container
 * @param last               end of the container
 * @param norm               a certain type of norm, e.g. N_EUCLIDEAN
 * @return                   the norm
 */
template<typename T, typename InputIterator>
T norm(InputIterator first, InputIterator last, Norm norm) {
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator>);
	__glibcxx_requires_valid_range(first, last);

	switch (norm) {
	case N_EUCLIDEAN:
		return std::sqrt(accumulate(first, last, T(0), std::plus<T>(), square<T> ) );
	case N_TAXICAB:
		return accumulate(first, last, T(0), std::plus<T>(), absolute<T>);
	case N_MAXIMUM:
		if (std::distance(first,last) == 0) return T(0);
		return *max_element(first, last, absolute<T>);
	default:
		std::cerr << "Unknown norm" << std::endl;
		return T(-1);
	}
}

/**
 * A norm can not be a real norm for a multitude of reasons. For example the sum of squares (without the square root)
 * violates the triangle inequality.
 * There is currently one pseudo-norms implemented:
 *   N_SUM_OF_SQUARES:       return sum_i { x_i*x_i } (L2 norm, except for square root at the end)
 * @param first              start of the container
 * @param last               end of the container
 * @param pseudo_norm        a certain type of pseudo-norm
 * @return                   the pseudo-norm
 */
template<typename T, typename InputIterator>
T pseudo_norm(InputIterator first, InputIterator last, PseudoNorm pseudo_norm) {
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator>);
	__glibcxx_requires_valid_range(first, last);

	switch (pseudo_norm) {
	case PN_SUM_OF_SQUARES:
		return accumulate(first, last, T(0), std::plus<T>(), square<T> );
	default:
		std::cerr << "Unknown pseudo_norm" << std::endl;
		return T(-1);
	}
}


/**
 * This function tells something about the static ensemble behaviour in the form of a "mean". There are currently
 * several "means" implemented:
 *   M_ARITHMETIC: return 1/n (sum_i { (x_i) } )
 *   M_GEOMETRIC:  return exp ( 1/n (sum_i { log(x_i) } ) )
 *   M_HARMONIC:   return max_i (x_i) (maximum norm)
 * @param first              Start of the container
 * @param last               End of the container
 * @param mean               A certain type of "mean", e.g. M_GEOMETRIC
 * @return                   The value of the mean
 */
template<typename T, typename InputIterator>
T mean(InputIterator first, InputIterator last, Norm norm) {
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator>);
	__glibcxx_requires_valid_range(first, last);
	typedef typename std::iterator_traits<InputIterator>::difference_type DistanceType1;

	DistanceType1 dist = std::distance(first, last);
	if (!dist) return T(0);

	switch (norm) {
	case M_ARITHMETIC:
		return 1/T(dist)*std::accumulate(first, last, T(0));
	case M_GEOMETRIC:
		return std::exp(1/T(dist)*accumulate(first, last, T(0), std::plus<T>(), std::log<T>));
	case M_HARMONIC:
		return T(dist)/accumulate(first, last, T(0), std::plus<T>(), inverse<T>);
	default:
		std::cerr << "Unknown norm" << std::endl;
		return T(-1);
	}
}

/**
 * Incremental adjustment of a standard container towards a reference container.
 *   d = d + mu ( ref - d)
 * So:
 *   delta_d = mu ( ref - d)
 * If "ref" is smaller than "d", "delta_d" will be negative: it will make "d" smaller.
 * Note that this function does not make use of the different distance metrics that can be defined.
 * It will use std::minus (normal -sign) to judge the distance over all elements of the container
 * and adjust them in the same fashion by the multiplication factor mu.
 * If "mu" is 1, it will set the "tomove" container equal to the "reference" container.
 * @param first1             beginning of the container to-be-moved
 * @param last1              end of the container to-be-moved
 * @param first2             the reference container that functions as a "attractor"
 * @param mu                 the step size (0 < mu <= 1)
 * @return                   void
 */
template<typename T, typename InputIterator1, typename InputIterator2>
void increaseDistance(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T mu) {
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator1>);
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator2>);
	__glibcxx_requires_valid_range(first1, last1);
	assert (mu > T(0)); assert (mu <= T(1));
	std::transform(first1, last1, first2, first1, op_adjust<T>(mu));
}

/**
 * Incremental adjustment of a container back from a reference container.
 *   d = d - mu ( ref - d)
 * @param first1             beginning of the container to-be-moved
 * @param last1              end of the container to-be-moved
 * @param first2             the reference container that functions as a "repeller"
 * @param mu                 the step size (0 < mu <= 1)
 * @return                   void
 */
template<typename T, typename InputIterator1, typename InputIterator2>
void decreaseDistance(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T mu) {
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator1>);
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator2>);
	__glibcxx_requires_valid_range(first1, last1);
	assert (mu > T(0)); assert (mu <= T(1));
	std::transform(first1, last1, first2, first1, op_adjust<T>(-mu));
}

/**
 * This function tells something about the "distance" between containers, in other words the similarity or
 * dissimilarity. There are currently several metrics implemented:
 *   DM_DOTPRODUCT:			return sum_i { x_i*y_i }
 *   DM_EUCLIDEAN:			return sqrt (sum_i { (x_i-y_i)^2 } )
 *   DM_BHATTACHARYYA:			return -ln (sum_i { sqrt (x_i*y_i) } )
 *   DM_HELLINGER:			return sqrt (sum_i { (sqrt(x_i)-sqrt(y_i))^2 } ) * 1/sqrt(2)
 *   DM_CHEBYSHEV:			return max_i abs(x_i-y_i)
 *   DM_MANHATTAN:			return sum_i { abs(x_i-y_i) }
 * And there are some other measures that can be used as metrics. Such as the Bhattacharyya coefficient
 * and the squared Hellinger distance.
 * It is assumed that the containers are of equal size.
 * @param first1			start of the first container
 * @param last1				end of the first container
 * @param first2			start of the second container
 * @param last2				end of the second container
 * @param metric			a certain distance metric
 * @return				the distance between the two containers
 */
template<typename T, typename InputIterator1, typename InputIterator2>
T distance(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, DistanceMetric metric) {
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator1>);
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator2>);
	__glibcxx_requires_valid_range(first1, last1);
	__glibcxx_requires_valid_range(first2, last2);
	if (std::distance(first2,last2) != std::distance(first1,last1)) {
		std::cerr << "Container size unequal: " << std::distance(first1,last1) << " vs " \
				<< std::distance(first2,last2) << std::endl;
		assert (std::distance(first2,last2) == std::distance(first1,last1));
	}
	switch (metric) {
	case DM_DOTPRODUCT:
		return std::inner_product(first1, last1, first2, T(0));
	case DM_EUCLIDEAN:
		return std::sqrt(std::inner_product(first1, last1, first2, T(0), std::plus<T>(), euclidean<T>));
	case DM_BHATTACHARYYA:
		return -std::log(std::inner_product(first1, last1, first2, T(0), std::plus<T>(), battacharyya<T>));
	case DM_HELLINGER:
		return (std::sqrt(std::inner_product(first1, last1, first2, T(0), std::plus<T>(), hellinger<T>))) /
				std::sqrt(2);
	case DM_CHEBYSHEV:
		return std::inner_product(first1, last1, first2, T(0), max<T>(), taxicab<T>);
	case DM_MANHATTAN:
		return std::inner_product(first1, last1, first2, T(0), std::plus<T>(), taxicab<T>);
	case DM_BHATTACHARYYA_COEFFICIENT:
		return std::inner_product(first1, last1, first2, T(0), std::plus<T>(), battacharyya<T>);
	case DM_SQUARED_HELLINGER:
		//return std::inner_product(first1, last1, first2, T(0), std::plus<T>(), hellinger<T>) * T(1)/T(2);
		// same result when using battacharyya instead of hellinger, and faster to calculate:
		return std::sqrt(T(1) - std::inner_product(first1, last1, first2, T(0), std::plus<T>(), battacharyya<T>));
	default:
		std::cerr << "Unknown distance metric" << std::endl;
		return T(-1);
	}
}

/**
 * Functionality of extract_copy is like remove_copy for the vector container. However, in this case the item is still
 * important, so it will be returned in the parameter value. This is a slow implementation, but it might be nice to do
 * your first implementation of Gibbs sampling using this function. The leave-one-out methods are abundant in science.
 * It is for example also called a "cavity". And it resurfaces again in belief propagation. A good implementation would
 * perhaps use a specific cavity_iterator.
 *
 * @param first              begin of container
 * @param last               end of container
 * @param position           position of element to be removed / extracted
 * @param result             begin of container items that are not removed are copied to
 * @param value              item that will be removed / extracted item
 * @return                   container with results
 */
template<typename InputIterator, typename OutputIterator, typename Tp>
OutputIterator
extract_copy(InputIterator first, InputIterator last, InputIterator position,
		OutputIterator result, Tp& value) {
	// concept requirements
	typedef typename std::iterator_traits<InputIterator>::value_type ValueType;

	__glibcxx_function_requires(_InputIteratorConcept<InputIterator>);
	__glibcxx_function_requires(_OutputIteratorConcept<OutputIterator, ValueType>);
	__glibcxx_function_requires(_EqualOpConcept<ValueType, Tp>);
	__glibcxx_requires_valid_range(first, last);

	for (; first != last; ++first) {
		if (!(first == position)) {
			value = *first;
		} else {
			*result = *first;
			++result;
		}
	}
	return result;
}


/***********************************************************************************************************************
 * Expected value, average, and other simple statistic functions
 **********************************************************************************************************************/

/**
 * Calculate the average over a series of elements in a standard container. Note that the container element type does
 * not need to be the same as the type returned by the average, which can for example have a higher accuracy (say it
 * concerns a vector of integers and the resulting value is a double).
 */
template<typename InputIterator, typename T>
T average(InputIterator first, InputIterator last, T init) {
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator>);
	__glibcxx_requires_valid_range(first, last);

	if (first == last) return init;
	return std::accumulate(first, last, init) / T(last-first);
}

/**
 * Expected value is just a weighted average with weights of all probabilities summing up to one, so it is the default
 * inner product.
 *
 *  E_p(x)= \sum_i p(i) x_i
 *
 * @template T               probability type (i.e. float, double)
 * @template InputIterator1  container iterator type
 * @template InputIterator2  container iterator type
 * @param first1             start of container
 * @param last1              end of container
 * @param first2             start of second container
 * @return                   expected value
 */
template<typename T, typename InputIterator1, typename InputIterator2>
T expectation(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2){
	return std::inner_product(first1, last1, first2, T(0));
}


/***********************************************************************************************************************
 * Entropies
 **********************************************************************************************************************/

/**
 * Hartley entropy is a bit silly, it just is the log of the cardinality of the random variable
 *
 *  H(X)=log |X|
 *
 * @template T               probability type (i.e. float, double)
 * @template InputIterator   container iterator type
 * @param first              start of container
 * @param last               end of container
 * @return                   Hartley entropy value
 */
template<typename T, typename InputIterator>
T hartley_entropy(InputIterator first, InputIterator last) {
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator>);
	__glibcxx_requires_valid_range(first, last);
	typedef typename std::iterator_traits<InputIterator>::difference_type DistanceType1;

	DistanceType1 dist = std::distance(first, last);
	return std::log(dist);
}

/**
 * Shannon entropy. If you know all the conditional probabilities between say x and y, you can also use as input vector
 * p(x|y). In that case H will naturally turn out to be \sum_j p(x|y) log p(x|y). Note that the sum is over all p(x|y)
 * without summing in specific over either element. The latter would require a 2-dimensional input namely.
 *
 *  H(p)=\sum_i p(i) log p(i)
 *
 * @template T               probability type (i.e. float, double)
 * @template InputIterator   container iterator type
 * @param first              start of container
 * @param last               end of container
 * @return                   Shannon entropy value
 */
template<typename T, typename InputIterator>
T shannon_entropy(InputIterator first, InputIterator last) {
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator>);
	__glibcxx_requires_valid_range(first, last);
	typedef typename std::iterator_traits<InputIterator>::difference_type DistanceType1;

	DistanceType1 dist = std::distance(first, last);
	if (!dist) return T(0);

	return accumulate(first, last, T(0), std::plus<T>(), xlogx<T>);
}

/**
 * Renyi entropy
 *
 *  H_q(p) = 1/(1-q) * log ( sum_i p_i^q)
 *
 * @template T               probability type (i.e. float, double)
 * @template InputIterator   container iterator type
 * @param first              start of container
 * @param last               end of container
 * @param q                  parameter q as defined for this entropy
 * @return                   Rényi entropy value
 */
template<typename T, typename InputIterator>
T renyi_entropy(InputIterator first, InputIterator last, T q) {
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator>);
	__glibcxx_requires_valid_range(first, last);
	typedef typename std::iterator_traits<InputIterator>::difference_type DistanceType1;

	DistanceType1 dist = std::distance(first, last);
	if (!dist) return T(0);

	// in the limit q->1 it should be equal to Shannon entropy
	if (q == 1) return shannon_entropy(first, last);

	return (T(1)/(1-q)) * std::log( accumulate(first, last, T(0), std::plus<T>(), fixed_power<T>(q)) );
}

//! Collision entropy is just Rényi with q=2.
template<typename T, typename InputIterator>
T collision_entropy(InputIterator first, InputIterator last) {
	return renyi_entropy(first, last, T(2));
}

/**
 * Tsallis entropy
 *
 *  H_q(p) = 1/(1-q) * (1 - sum_i p_i^q)
 *
 * @template T               probability type (i.e. float, double)
 * @template InputIterator   container iterator type
 * @param first              start of container
 * @param last               end of container
 * @param q                  parameter q as defined for this entropy
 * @return                   Tsallis entropy value
 */
template<typename T, typename InputIterator>
T tsallis_entropy(InputIterator first, InputIterator last, T q) {
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator>);
	__glibcxx_requires_valid_range(first, last);
	typedef typename std::iterator_traits<InputIterator>::difference_type DistanceType1;

	DistanceType1 dist = std::distance(first, last);
	if (!dist) return T(0);

	// in the limit q->1 it should be equal to Shannon entropy
	if (q == 1) return shannon_entropy(first, last);

	return (T(1)/(1-q)) * (1 - accumulate(first, last, T(0), std::plus<T>(), fixed_power<T>(q)) );
}

/**
 * See Tsallis entropy, but shifted by q=1-q'. This shifted entropy is proposed by Nelson and Umarov. The new q value
 * leads to distributions with compact support for q>0, to Shannon for q=0, and heavy-tail q-Gaussians for -2<q<0.
 * Verbose, negative values of q are associated with anti-correlation or decoupling and increased decay (rather than
 * exponential as with Shannon it is superexponential and goes to 0 in a finite time). Positive values of q are
 * associated with correlation and coupling, and decrease decay. The solution shows slower than exponential rate, and
 * has a heavy tail.
 */
template<typename T, typename InputIterator>
T coupled_entropy(InputIterator first, InputIterator last, T q) {
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator>);
	__glibcxx_requires_valid_range(first, last);
	typedef typename std::iterator_traits<InputIterator>::difference_type DistanceType1;

	DistanceType1 dist = std::distance(first, last);
	if (!dist) return T(0);

	// in the limit q->1 it should be equal to Shannon entropy
	if (q == 0) return shannon_entropy(first, last);

	return T(1)/q * (-1 + accumulate(first, last, T(0), std::plus<T>(), fixed_power<T>(1-q)) );
}

/**
 * The Gini index or Gini coefficient is not an entropy, but the form is quite similar, hence it is added here. It is also sometimes
 * called "information gain".
 */
template<typename T, typename InputIterator>
T gini_index(InputIterator first, InputIterator last, T q) {
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator>);
	__glibcxx_requires_valid_range(first, last);
	typedef typename std::iterator_traits<InputIterator>::difference_type DistanceType1;

	DistanceType1 dist = std::distance(first, last);
	if (!dist) return T(0);

	return accumulate(first, last, T(0), std::plus<T>(), x_times_1_minus_x<T>);
}


/***********************************************************************************************************************
 * Cross entropy
 **********************************************************************************************************************/

/**
 * Cross entropy
 *
 *  H(p,q)= - \sum_i p(i) log q(i)
 *
 * @template T               probability type (i.e. float, double)
 * @template InputIterator1  container iterator type
 * @template InputIterator2  container iterator type
 * @param first1             start of container
 * @param last1              end of container
 * @param first2             start of second container
 * @return                   Cross entropy
 */
template<typename T, typename InputIterator1, typename InputIterator2>
T cross_entropy(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2){
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator1>);
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator2>);
	__glibcxx_requires_valid_range(first1, last1);
	typedef typename std::iterator_traits<InputIterator1>::difference_type DistanceType1;

	DistanceType1 dist = std::distance(first1, last1);
	if (!dist) return T(0);

	return inner_product(first1, last1, first2, xlogy<T>);
}


/***********************************************************************************************************************
 * Divergences, fancy name for statistical distances.
 *
 * Kullback-Leibler is simple enough to describe in a 1-dimensional fashion. Others, e.g. Fisher information requires
 * even in discrete form marginal calculate over conditional probabilities. They are inherently two dimensional and can
 * better be represented by a matrix library.
 *
 * Fisher: F(v)=sum_x p(x|v) ( 1/dv dln p(x|v) )^2  where d is "delta", "v" is the discrete parameter
 *
 **********************************************************************************************************************/

/**
 * Kullback-Leibler divergence also known as relative entropy, information gain, and information divergence. It belongs
 * to the class of f-divergences, just as the Hellinger distance.
 *
 *  D(p||q)=\sum_i log (p(i)/q(i)) p(i)
 *
 * @template T               probability type (i.e. float, double)
 * @template InputIterator1  container iterator type
 * @template InputIterator2  container iterator type
 * @param first1             start of container
 * @param last1              end of container
 * @param first2             start of second container
 * @return                   Kullback-Leibler divergence
 */
template<typename T, typename InputIterator1, typename InputIterator2>
T kullback_leibler_divergence(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2){
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator1>);
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator2>);
	__glibcxx_requires_valid_range(first1, last1);
	typedef typename std::iterator_traits<InputIterator1>::difference_type DistanceType1;

	DistanceType1 dist = std::distance(first1, last1);
	if (!dist) return T(0);

	return inner_product(first1, last1, first2, xlogx_over_y<T>);
}

/**
 * See taxicab distance, but now with probabilities in the input containers. It also is defined without the division by
 * two, which would make it totally the same as the Manhattan distance function.
 */
template<typename T, typename InputIterator1, typename InputIterator2>
T total_variation_distance(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2){
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator1>);
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator2>);
	__glibcxx_requires_valid_range(first1, last1);
	typedef typename std::iterator_traits<InputIterator1>::difference_type DistanceType1;

	DistanceType1 dist = std::distance(first1, last1);
	if (!dist) return T(0);

	return 1/T(2) * inner_product(first1, last1, first2, taxicab<T>);
}

/**
 * Rényi divergence.
 *
 *  D(p||q)= 1/(a-1) log \sum_i p(i^a) q(i^(1-a))
 *
 * @todo Check if (a-1) is indeed different from (1-a) in Renyi entropy.
 *
 * @template T               probability type (i.e. float, double)
 * @template InputIterator1  container iterator type
 * @template InputIterator2  container iterator type
 * @template OutputIterator  container iterator type
 * @param first1             start of container
 * @param last1              end of container
 * @param first2             start of second container
 * @return                   Rényi divergence
 */
template<typename T, typename InputIterator1, typename InputIterator2>
T renyi_divergence(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T a){
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator1>);
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator2>);
	__glibcxx_requires_valid_range(first1, last1);
	typedef typename std::iterator_traits<InputIterator1>::difference_type DistanceType1;

	DistanceType1 dist = std::distance(first1, last1);
	if (!dist) return T(0);

	// there are more shortcuts, for q==0, q==1/2, q==2, etc. but they will not result in DIV/0, so this one is enough
	if (a == 1) return kullback_leibler_divergence(first1, last1, first2);

	return T(1)/(1-a) *
			std::log ( std::inner_product(first1, last1, first2, T(0), std::plus<T>(), mixed_exponent<T>(a)) );
}

/**
 * Jensen-Shannon divergence.
 *
 * This uses a mixture distribution: m=1/2(p+q),
 *
 *  D(p||q)=1/2 D_KL(p||m) + 1/2 D_KL(q||m)
 *
 * with D_KL the Kullback-Leibler divergence (see above).
 *
 * @template T               probability type (i.e. float, double)
 * @template InputIterator1  container iterator type
 * @template InputIterator2  container iterator type
 * @param first1             start of container
 * @param last1              end of container
 * @param first2             start of second container
 * @return                   Jensen-Shannon divergence
 */
template<typename T, typename InputIterator1, typename InputIterator2>
T jensen_shannon_divergence(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2){
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator1>);
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator2>);
	__glibcxx_requires_valid_range(first1, last1);
	typedef typename std::iterator_traits<InputIterator1>::difference_type DistanceType1;

	DistanceType1 dist = std::distance(first1, last1);
	if (!dist) return T(0);

	T result = T(0);
	result += std::inner_product(first1, last1, first2, T(0), std::plus<T>(), xlogx_avg_y<T>);
	result += std::inner_product(first2, first2+dist, first1, T(0), std::plus<T>(), xlogx_avg_y<T>);
	return result / 2;
}


/***********************************************************************************************************************
 * Points and distances
 *
 * In the following section points are multi-dimensional entities denoted by a container with coordinate indices.
 **********************************************************************************************************************/

/**
 * Provide a similar template function, but now with containers instead of iterators. Be careful that now the
 * typename Point is not checked for having actually "begin() and end()" operators.
 */
template<typename T, typename Point>
T distance_impl(Point point1, Point point2, DistanceMetric metric) {
	return distance(point1.begin(), point1.end(), point2.begin(), point2.end(), metric);
}


/**
 * A wrapper struct for "distance" to be used as function object (it is a binary function) to calculate
 * distances between sets. It is a comparison functor. To do the comparison the distance between a
 * reference point, previously given, and each of the argument is calculated. If the first argument is
 * smaller, the function returns true.
 * @template_param		Point should be a pointer to a container, e.g. std::vector<double>*
 * @template_param		Iterator should be an iterator over the same type of container
 * @template_param		Value should be the value of the elements in the container
 * @param	 		point_metric to be used, e.g. Euclidean
 * @param			first_ref is reference to point
 * @param			last_ref is reference to point
 * @return			true if x is closer to the reference point than y
 * This struct requires the function of above.
 */
template<typename Point, typename PointIterator, typename PointValueType>
struct comp_point_distance: public std::binary_function<Point, Point, bool> {
	comp_point_distance(DistanceMetric point_metric, PointIterator first_ref, PointIterator last_ref):
		point_metric(point_metric), first_ref(first_ref), last_ref(last_ref) {
	}
	bool operator()(Point x, Point y) {
		return distance<PointValueType, PointIterator, PointIterator>(x->begin(), x->end(), first_ref, last_ref, point_metric) <
				distance<PointValueType, PointIterator, PointIterator>(y->begin(), y->end(), first_ref, last_ref, point_metric);
	}
	DistanceMetric point_metric;
	PointIterator first_ref;
	PointIterator last_ref;
};

/*
 * A function calculating the distance of a point to a set.
 * 	SDM_INFIMIM		the minimum distance to this point, for Euclidean/Manhattan in 1D example, d(1,[3,6]) = 2 and d(7,[3,6]) = 1.
 * TODO: make sure that the values of the iterator over the set correspond with the container over which the second iterator
 * runs.
 */
template<typename T, typename SetIterator, typename PointIterator>
T distance_to_point(SetIterator first_set, SetIterator last_set, PointIterator first_point, PointIterator last_point,
		SetDistanceMetric set_metric, DistanceMetric point_metric) {
	__glibcxx_function_requires(_InputIteratorConcept<SetIterator>);
	__glibcxx_function_requires(_InputIteratorConcept<PointIterator>);
	__glibcxx_requires_valid_range(first1, last1);
	__glibcxx_requires_valid_range(first2, last2);
	typedef typename std::iterator_traits<SetIterator>::value_type PointType; // e.g. std::vector<double>*
	typedef typename std::iterator_traits<PointIterator>::value_type PointValueType; // e.g. double

	T result = T(-1);
	PointType tmp;
	switch(set_metric) {
	case SDM_INFIMIM: // the smallest distance between the point and any point in the set
		tmp = *std::min_element(first_set, last_set, comp_point_distance<PointType, PointIterator, PointValueType>(
				point_metric, first_point, last_point));
		return distance<PointValueType, PointIterator, PointIterator>(tmp->begin(), tmp->end(), first_point, last_point, point_metric);
	case SDM_SUPREMUM: // the largest distance between the point and any point in the set
		tmp = *std::max_element(first_set, last_set, comp_point_distance<PointType, PointIterator, PointValueType>(
				point_metric, first_point, last_point));
		return distance<PointValueType, PointIterator, PointIterator>(tmp->begin(), tmp->end(), first_point, last_point, point_metric);
	default:
		std::cerr << "Not yet implemented" << std::endl;
		break;
	}

	return result;
}

/**
 * Same function as above, but using iterators implicitly. Not safe.
 */
template<typename T, typename Point, typename SetContainer>
T distance_impl(SetContainer set, Point point, SetDistanceMetric set_metric, DistanceMetric point_metric) {
	return distance_to_point(set.begin(), set.end(), point.begin(), point.end(), set_metric, point_metric);
}

/**
 * @template_param		Point should be something like std::vector<float> *
 * @template_param		SetIterator should be an iterator over a set
 * @template_param		PointIterator should be an iterator over the point container
 * @template_param		Value should be the type of the values in the point container, e.g. float
 * @return			true if point is closer to
 */
template<typename Point, typename SetIterator, typename PointIterator, typename Value>
struct comp_point_set_distance: public std::binary_function<Point, Point, bool> {
	comp_point_set_distance(SetDistanceMetric set_metric, DistanceMetric point_metric, SetIterator first_set,
			SetIterator last_set):
				set_metric(set_metric), point_metric(point_metric), first_set(first_set), last_set(last_set) {
	}

	bool operator()(const Point & x, const Point & y) const {
		return distance_to_point<Value, SetIterator, PointIterator>(first_set, last_set, x->begin(), x->end(), set_metric, point_metric) <
				distance_to_point<Value, SetIterator, PointIterator>(first_set, last_set, y->begin(), y->end(), set_metric, point_metric);
	}
	SetDistanceMetric set_metric;
	DistanceMetric point_metric;
	SetIterator first_set;
	SetIterator last_set;
};


/**
 * Different metrics that exist between sets of points.
 *   SDM_HAUSDORFF 		longest distance you can be forced to travel by an adversary who chooses a point in one of the two sets,
 *   				from where you then must travel to the other set (wikipedia)
 *   SDM_SUPINF			calculates the smallest distance between a point and any other, then picks the point that is most remote
 *   				of the others (Hausdorff is then just doing this also in the opposite direction in case distances are not
 *   				symmetric)
 * For now only Hausdorff is implemented and the supremum-infimim operator. An example of the Hausdorff metric when the point metric
 * is   d([1,3,6,7], [3,6]) = 2, but d([3,6], [1,3,6,7]) = 0.
 * @param set0			a set of data points (each data point can be a vector or list or matrix)
 * @param set1			another set of data points
 * @param set_metric		the metric to be used between the two sets
 * @param metric		the metric to be used to define distances between points
 *
 * Caution! The SetIterator and the PointIterator do not correspond to the first set of two iterators, respectively the last set of two
 * iterators. All of these iterators should be of the same type SetIterator. However, they should be decomposable into PointIterators.
 * In other words, the set entities should have the PointIterator as valid iterator defined over each of their elements. This definitely
 * requires you to define the template variables (because they cannot be retrieved from the arguments).
 */
template<typename T, typename SetIterator, typename PointIterator>
T distance_to_set(SetIterator first1, SetIterator last1, SetIterator first2, SetIterator last2,
		SetDistanceMetric set_metric, DistanceMetric point_metric) {
	__glibcxx_function_requires(_InputIteratorConcept<SetIterator>);
	__glibcxx_function_requires(_InputIteratorConcept<PointIterator>);
	__glibcxx_requires_valid_range(first1, last1);
	__glibcxx_requires_valid_range(first2, last2);
	typedef typename std::iterator_traits<SetIterator>::value_type PointType; // e.g. std::vector<double>*
	typedef typename std::iterator_traits<PointIterator>::value_type PointValueType; // e.g. double

	SetDistanceMetric overwrite_set_metric;
	PointType tmp;
	switch(set_metric) {
	case SDM_HAUSDORFF: {
		T dist_xy = distance_to_set<T,SetIterator,PointIterator>(first1, last1, first2, last2, SDM_SUPINF, point_metric);
		T dist_yx = distance_to_set<T,SetIterator,PointIterator>(first2, last2, first1, last1, SDM_SUPINF, point_metric);
		return std::max(dist_xy, dist_yx);
	}
	case SDM_SUPINF:
		overwrite_set_metric = SDM_INFIMIM;
		tmp = *std::max_element(first1, last1,
				comp_point_set_distance<PointType, SetIterator, PointIterator, PointValueType>(
						overwrite_set_metric, point_metric, first2, last2));
		return distance_to_point<PointValueType, SetIterator, PointIterator>(
				first2, last2, tmp->begin(), tmp->end(), overwrite_set_metric, point_metric);
		break;
	default:
		std::cerr << "Not yet implemented" << std::endl;
		break;
	}
}

/***********************************************************************************************************************
 * Integrals, rotations, and convolutions
 **********************************************************************************************************************/

/**
 * Generate a "0 value" (depends on the class T what that constitutes).
 */
template <typename T>
T zero_func() { return T(0); }

/**
 * Generate a series of "0 values" (depends on the class T what that constitutes).
 */
template<typename InputIterator1>
void clean(InputIterator1 first1, InputIterator1 last1) {
	typedef typename std::iterator_traits<InputIterator1>::value_type ValueType1;
	std::generate_n(first1, last1 - first1, zero_func<ValueType1>);
}

/**
 * Calculate the integral of a function given a kernel in the discrete case. It multiplies the individual elements from
 * both input vectors and writes the accumulated sum of the product to the result. With x[i]=i, y[i]=i, the result
 * is: r[0]=0, r[1]=0+1*1=1, r[2]=1+2*2=5, r[3]=5+3*3=14, r[4]=14+4*4=30, etc.
 *
 * Functions like remove_copy which return a series of values in the STL C++ library, require the user to allocate the
 * proper number of items beforehand. This seems to me a pain for functions where this is not known beforehand. However,
 * to be consistent with STL, you will need to have the output iterator pointed to a container that is large enough.
 */
template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
OutputIterator
integral(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2,
		OutputIterator result) {

	typedef typename std::iterator_traits<InputIterator1>::value_type ValueType1;

	// concept requirements
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator1>);
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator2>);
	__glibcxx_function_requires(_OutputIteratorConcept<OutputIterator, ValueType1>);
	__glibcxx_requires_valid_range(first1,last1);

	if (first1 == last1)
		return result;
	ValueType1 value = *first1 * *first2;
	*result = value;
	while (++first1 != last1)
	{
		value = value + *first1 * *++first2;
		*++result = value;
	}
	return ++result;
}

/**
 * As the integral above, but the user can define how to combine the input elements (by e.g. a binary operator different
 * from element-wise multiplication) and how to aggregate the result (by e.g. a binary operator different from
 * addition).
 */
template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename BinaryOperation1,
typename BinaryOperation2>
OutputIterator
integral(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2,
		OutputIterator result, BinaryOperation1 binary_op1, BinaryOperation2 binary_op2) {

	typedef typename std::iterator_traits<InputIterator1>::value_type ValueType1;

	// concept requirements
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator1>);
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator2>);
	__glibcxx_function_requires(_OutputIteratorConcept<OutputIterator, ValueType1>);
	__glibcxx_requires_valid_range(first1,last1);

	if (first1 == last1)
		return result;
	ValueType1 value = *first1 * *first2;
	*result = value;
	while (++first1 != last1)
	{
		value = binary_op1(value, binary_op2(*first1, *++first2));
		*++result = value;

	}
	return ++result;
}

/**
 * The Cauchy product:
 *   c_n = \sum_{k=0}^n { a_k * b_{n-k} }
 * This product is very similar to the integral. It is not calculated only for the total size of the vector, but for
 * each element of the vector. And the element-wise multiplication iterates backwards over the second vector as in
 * convolution.
 *
 * @param 			last2 is an iterator to the LAST element of the second vector
 */
template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
OutputIterator
cauchy_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 last2,
		OutputIterator result) {

	typedef typename std::iterator_traits<InputIterator1>::value_type ValueType1;

	// concept requirements
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator1>);
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator2>);
	__glibcxx_function_requires(_OutputIteratorConcept<OutputIterator, ValueType1>);
	__glibcxx_requires_valid_range(first1,last1);

	if (first1 == last1)
		return result;

	ValueType1 value = *first1 * *--last2;
	*result = value;
	while (++first1 != last1)
	{
		value = value + *first1 * *--last2;
		*++result = value;
	}
	return ++result;
}

/**
 * Inner product where we iterate backwards over the second container.
 */
template<typename InputIterator1, typename InputIterator2, typename Tp>
inline Tp
reverse_inner_product(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 last2, Tp init)
{
	// concept requirements
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator1>);
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator2>);
	__glibcxx_requires_valid_range(first1, last1);
	--last2;
	for (; first1 != last1; ++first1, --last2)
		init = init + (*first1 * *last2);
	return init;
}

/**
 * This function calculates the discrete convolution between two functions represented by for example vectors or sets.
 * It calculates the product of x[i] and y[shift-i]. So, with shift of 1, it multiplies x[0] with x[1], x[1] with y[0],
 * x[2] with y[-1], etc. So, a normal convolution would not work for a finite range of values (like a vector). The
 * circular_convolution however, works fine with a limited sequence. It calculates:
 *   conv_n(shift) = \sum_{k=0}^{n-1} { a_k * b_{(shift-k) % n} }
 *
 * In other words, using vector terminology. It calculates the inner product between the two vectors with the second one
 * reversed and each time shifted a bit more with "shift" (default 1) for in total N times, the results which are
 * written into an output container.
 *
 * @param			first1 beginning of container
 * @param			last1 end of container
 * @param			first2 beginning of second container
 * @param 			last2 end of second container (required for rotation)
 * @param			result begin of container for results (needs capacity last1 - first1)
 * @param			(optional) shift with which to calculate the convolution, default 1
 * @return			end of result container
 */
template<typename ForwardIterator1, typename ForwardIterator2, typename OutputIterator>
OutputIterator
circular_convolution(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2,
		ForwardIterator2 last2, OutputIterator result, int shift = 1) {
	typedef typename std::iterator_traits<ForwardIterator1>::value_type ValueType1;

	__glibcxx_function_requires(_ForwardIteratorConcept<ForwardIterator1>);
	__glibcxx_function_requires(_ForwardIteratorConcept<ForwardIterator2>);
	__glibcxx_function_requires(_OutputIteratorConcept<OutputIterator,ValueType1>);
	__glibcxx_requires_valid_range(first1, last1);
	__glibcxx_requires_valid_range(first2, last2);

	typedef typename std::iterator_traits<ForwardIterator1>::value_type ValueType1;
	typedef typename std::iterator_traits<ForwardIterator1>::difference_type DistanceType1;

	DistanceType1 dist = std::distance(first1, last1);
	while (dist--) {
		std::rotate(first2, last2-shift, last2);
		*result++ = reverse_inner_product(first1, last1, last2, ValueType1(0));
	}
	return result;
}

/**
 * Rotating left or right.
 */
enum RotateDirection {
	RD_LEFT,
	RD_RIGHT,
};

/**
 * @brief Rotate the elements of a sequence by one.
 *
 * Rotates the elements of a sequence in either of two directions and just by one. Use the std::rotate if you want
 * something else.
 *
 * @param first   	   A forward iterator.
 * @param last    	   A forward iterator.
 * @param direction    Rotating right or left
 * @return             void: the underlying container is adjusted as a side-effect
 */
template<typename InputIterator>
void rotate(InputIterator first, InputIterator last, RotateDirection direction = RD_RIGHT) {
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator>);
	__glibcxx_requires_valid_range(first, last);
	if (last - first == 1) return;
	switch (direction) {
	case RD_LEFT:
		std::rotate(first, first+1, last);
		break;
	case RD_RIGHT: default:
		std::rotate(first, last-1, last);
		break;
	}
}

/**
 * @brief Rotate the elements of a sequence by N
 *
 * If you use std::valarray, this is the same as cshift (except that cshift on valarray is probably computationally
 * more efficient).
 *
 * @param first   	   A forward iterator.
 * @param last    	   A forward iterator.
 * @param N            Number of elements to shift with
 * @return             void: the underlying container is adjusted as a side-effect
 */
template<typename InputIterator>
void rotate_n(InputIterator first, InputIterator last, size_t N, RotateDirection direction = RD_RIGHT) {
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator>);
	__glibcxx_requires_valid_range(first, last);
	if (last - first == 1) return;
	switch (direction) {
	case RD_LEFT:
		if (first+N > last) return;
		std::rotate(first, first+N, last);
		break;
	case RD_RIGHT: default:
		if (last-N < first) return;
		std::rotate(first, last-N, last);
		break;
	}
}

/**
 * Adds a vector with itself shifted one to the left or right. Shifting to the right is the default.
 */
template<typename InputIterator, typename OutputIterator>
OutputIterator
shiftadd(InputIterator first, InputIterator last, OutputIterator result, RotateDirection direction = RD_RIGHT) {
	typedef typename std::iterator_traits<InputIterator>::value_type ValueType;
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator>);
	__glibcxx_function_requires(_OutputIteratorConcept<OutputIterator, ValueType>);
	__glibcxx_requires_valid_range(first, last);

	if (first == last) return result;

	if (first - last == 1) {
		*result++ = *first + *first;
		return result;
	}

	switch (direction) {
	case RD_RIGHT: default: {
		ValueType prev = *first;
		while (++first != last) {
			ValueType value = *first;
			*++result = value + prev;
			prev = value;
		}
		*++result = prev + *first;
	} break;
	case RD_LEFT: {
		ValueType prev = *first;
		*++result = *(last-1) + prev;
		while (++first != last) {
			ValueType value = *first;
			*++result = value + prev;
			prev = value;
		}
	}
	}
	return result;
}

/**
 * Summation of a window in a vector and write sum to result. The window is centered around the resulting vector
 * elements. So, "half_window" extends left as well as right. A half_window of "2" corresponds to a sliding window
 * of [-2,-1,0,1,2].
 */
template<typename InputIterator, typename OutputIterator>
OutputIterator
window_add(InputIterator first, InputIterator last, OutputIterator result, size_t half_window) {
	typedef typename std::iterator_traits<InputIterator>::value_type ValueType;
	typedef typename std::iterator_traits<InputIterator>::difference_type DistanceType;

	__glibcxx_function_requires(_InputIteratorConcept<InputIterator>);
	__glibcxx_function_requires(_OutputIteratorConcept<OutputIterator, ValueType>);
	__glibcxx_requires_valid_range(first, last);

	DistanceType dist = std::distance(first, last);
	if (!dist) return result;
	if (dist==1) return result;
	if (dist<half_window*2-1) return result;

	DistanceType index = DistanceType(0);
	for (; index != dist; ++index, ++result) {
		ValueType value = ValueType(0);
		for (int i = -half_window; i != half_window+1; ++i) {
			DistanceType d = (i + index + dist) % dist;
			value += *(first+d);
		}
		*result = value;
	}
	return result;
}


/**
 * Pushes an item upon a container and pops off the oldest value. In case the container is build up out of pointers to
 * objects, make sure you deallocate the item yourself. This can perfectly be used for a circular buffer. The
 * implementation actually physically rotates the elements so it will not be efficient, but using just integers in there
 * it might be faster than expected.
 *
 * @param first 		Iterator to the beginning of the container
 * @param last 			Iterator to the end of the container
 * @param item 			The item to be added to the container
 * @param direction 	The direction in which to push and pop (left or right)
 */
template<typename InputIterator, typename T>
void pushpop(InputIterator first, InputIterator last, T item, RotateDirection direction = RD_RIGHT) {
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator>);
	__glibcxx_requires_valid_range(first, last);
	dobots::rotate(first, last, direction);
	// overwrite first item
	*first = item;
}

/**
 * The function argmin calculates unary_op(x_i) and unary_op(x_j) for all i and j in vector X and returns
 * that index that is minimum.
 * 	{x*} = argmin_x f(x)
 * The {} means that it is in principle possible to return a set (multiple minima!), which we will not do however.
 * The * signifies the local minimum of the possible x. The argmin_x will input every possible x in function f. And the
 * function takes only one argument, it is a unary_op.
 *
 * Note, that this function is actually almost the same as std::min_element() except that the latter takes a comparison
 * functor as (optional) argument and this function takes a unary operator.
 *
 * @param first              Beginning of container
 * @param last               End of container
 * @param unary_op           Function to perform
 * @param init               Minimum value (set it to first element in container if you don't want it)
 * @return                   Index to minimum value
 */
template<typename ForwardIterator, typename UnaryOperation>
ForwardIterator
argmin(ForwardIterator first, ForwardIterator last, UnaryOperation unary_op) {

	typedef typename std::iterator_traits<ForwardIterator>::value_type ValueType;

	// concept requirements
	__glibcxx_function_requires(_ForwardIteratorConcept<ForwardIterator>);
	__glibcxx_requires_valid_range(first,last);

	if (first == last)
		return first;
	ForwardIterator result = first;
	while (++first != last)
		if (unary_op(*first) < unary_op(*result))
			result = first;
	return result;
}

/**
 * Counts the number of elements in a given container that exceeds (is strict greater than) a certain threshold.
 * @param first              start of container
 * @param last               end of container
 * @threshold                threshold to compare each element with
 * @return                   number of items above threshold
 */
template<typename InputIterator, typename T>
size_t exceeds(InputIterator first, InputIterator last, T threshold) {
	size_t result = size_t(0);
	while (first != last) {
		result += std::greater<T>(*first, threshold);
		first++;
		result++;
	}
	return result;
}

/**
 * Returns the first index of the element that exceeds (is strict greater than) a certain threshold.
 * @param first              start of container
 * @param last               end of container
 * @threshold                threshold to compare each element with
 * @return                   first item above threshold
 */
template<typename ForwardIterator, typename T>
ForwardIterator exceeds_element(ForwardIterator first, ForwardIterator last, T threshold) {
	while (first != last) {
		if ( *first > threshold ) break;
		first++;
	}
	return first;
}

/**
 * Expects a sorted container. The values are considered indices for the result container.
 */
template<typename InputIterator, typename OutputIterator>
OutputIterator
count(InputIterator first, InputIterator last, OutputIterator result) {
	typedef typename std::iterator_traits<InputIterator>::value_type ValueType1;
	typedef typename std::iterator_traits<OutputIterator>::value_type ValueType2;

	// empty input, do nothing
	if (first == last) return result;

	ValueType2 cnt = 0;
	size_t result_index = 0;
	while (first != last) {
		while ((*first - result_index) > 0) {
			result_index++;
			*result = cnt;
			result++;
			cnt = 0;
		}
		cnt++;
		first++;
	}
	*result = cnt;
	result++;
}


/***********************************************************************************************************************
 * Random
 **********************************************************************************************************************/

/**
 * @brief Returns a random subset from a potentially very large set
 * The function random_subset returns a (small) random subset from a large STL container. It is
 * different from using std::random_shuffle and then picking the first k elements. It is namely
 * of the order O(k) and not of the order O(N) with k the number of elements to pick, and N the
 * total number of elements. It is the so-called Floyd Algorithm.
 * @see http://eyalsch.wordpress.com/2010/04/01/random-sample/
 *
 * @ingroup random_algorithms
 * @param first              start of range
 * @param last               end of range
 * @param int                number of elements
 * @param result             output set with random elements
 * @return Iterator          referencing the first instance of the random subset
 */
template<typename ForwardIterator, typename OutputIterator>
OutputIterator
random_subset(ForwardIterator first, ForwardIterator last, int elements, OutputIterator result)
{
	// concept requirements
	typedef typename std::iterator_traits<ForwardIterator>::value_type ValueType;
	typedef typename std::iterator_traits<ForwardIterator>::difference_type DiffType;

	__glibcxx_function_requires(_ForwardIteratorConcept<ForwardIterator>);
	__glibcxx_requires_valid_range(first, last);
	__glibcxx_function_requires(_OutputIteratorConcept<OutputIterator, ValueType>);

	if (first == last)
		return first;

	std::set<int> indices; indices.clear();
	for (ForwardIterator i = last - elements; i != last; ++i) {
		int pos = std::rand() % (int)((i - first)+1); // pick a random index
		if (indices.find(pos) != indices.end()) {
			indices.insert(i-first);
			*result = *i;
		} else {
			indices.insert(pos);
			*result = *(first+pos);
		}
		++result;
	}
	return result;
}

/***********************************************************************************************************************
 * Subset manipulations
 **********************************************************************************************************************/

/**
 * Subsample a standard container with given factor. It it the callee's responsibility to resize() an output container
 * so that it is large enough. More or less "(last - first) / factor", but different if the container size is not an
 * exact multiple of the factor. For convenience sake, see subsample_size function below, which does simple run the same
 * loop (instead of calculating it at once).
 */
template<typename InputIterator, typename OutputIterator>
OutputIterator
subsample(InputIterator first, InputIterator last, OutputIterator result, int factor) {

	if (first == last) return result;

	while (first < last) {
		*result = *first;
		first += factor;
		++result;
	}
	return ++result;
}

/**
 * Calculate the size of the subsamples container in a loop. Slow but reliable.
 */
template<typename InputIterator>
size_t subsample_size(InputIterator first, InputIterator last, int factor) {
	size_t result = size_t(0);
	while (first < last) {
		first += factor;
		result++;
	}
	return result;
}

/**
 * The compress function is slightly more sophisticated than just subsampling. It averages over a window the size of
 * (last-first)/factor and returns these averages as the elements of the output container.
 *
 * @todo: Check when (last-first) is not a multiple of factor.
 */
template<typename T, typename InputIterator, typename OutputIterator>
OutputIterator
compress(InputIterator first, InputIterator last, OutputIterator result, T init, int factor) {
	typedef typename std::iterator_traits<InputIterator>::value_type ValueType;
	if (first == last) return result;

	while (first < last) {
		ValueType avg = init;
		for (int i = 0; i < factor; ++i, ++first) {
			avg += *first;
		}
		*result = avg / ValueType(factor);
		++result;
	}
	return ++result;
}

/**
 * Slightly adapted version of compress, which can use numeric_limits<ValueType>::quiet_Nan() for not taking every
 * item into account.
 */
template<typename T, typename InputIterator, typename OutputIterator>
OutputIterator
compress_nan(InputIterator first, InputIterator last, OutputIterator result, T init, int factor) {
	typedef typename std::iterator_traits<InputIterator>::value_type ValueType;
	if (first == last) return result;

	while (first < last) {
		ValueType avg = init;
		int count = 0;
		for (int i = 0; i < factor; ++i, ++first) {
			if (!isnan(*first)) {
				avg += *first;
				++count;
			}
		}
		if (!count) {
			*result = avg;
		} else {
			*result = avg / count;
		}
		++result;
	}
	return ++result;
}

/**
 * Set all elements to not-a-number if they are equal to "value"
 *
 * @param first              Start of container (values will be changed in-place)
 * @param last               End of container
 * @param value              The value to be replaced by not-a-number
 */
template<typename T, typename InputIterator>
void
set_nan(InputIterator first, InputIterator last, T value) {
	typedef typename std::iterator_traits<InputIterator>::value_type ValueType;
	for (; first != last; ++first) {
		if (*first == value)
			*first = std::numeric_limits<ValueType>::quiet_Nan();
	}
}

// end of namespace dobots


/**
 * To a stl container filled with references to elements, fill another stl container with the dereferenced elements.
 * This copies all elements.
 *
 * Do not forget to call something like result.resize(...) beforehand.
 */
template<typename InputIterator, typename OutputIterator>
OutputIterator deref(InputIterator first, InputIterator last, OutputIterator result) {
	typedef typename std::iterator_traits<InputIterator>::value_type ValueType;
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator>);
	__glibcxx_function_requires(_OutputIteratorConcept<OutputIterator, ValueType>);
	__glibcxx_requires_valid_range(first1, last1);
	if (first == last) return result;

	*result = *first;
	while (++first != last) {
		*++result = *first;
	}
	return result;
}

/**
 * To a stl container filled with actual elements, fill another stl container with references to these elements.
 * This does not copy the elements, but just creates a reference to them.
 *
 * Do not forget to call something like result.resize(...) beforehand.
 */
template<typename InputIterator, typename OutputIterator>
OutputIterator ref(InputIterator first, InputIterator last, OutputIterator result) {
	typedef typename std::iterator_traits<InputIterator>::value_type ValueType;
	__glibcxx_function_requires(_InputIteratorConcept<InputIterator>);
	__glibcxx_function_requires(_OutputIteratorConcept<OutputIterator, ValueType>);
	__glibcxx_requires_valid_range(first1, last1);
	if (first == last) return result;

	*result = &*first;
	while (++first != last) {
		*++result = &*first;
	}
	return result;
}

/**
 * Often you need to iterate over an array not simply once, as with for_each, but for each element paired with each
 * other element in the sequence. The element does not need be paired or compared with itself, func(a,a), nor is it
 * necessary to compare it associatively: func(a,b) is enough, func(b,a) is unnecessary.
 *
 * For that reason combine_pairwise calls a given function for each of these unique pairs. It performs an iteration
 * over the "strict upper triangular matrix" (without the diagonal) if that makes things clearer.
 *
 * Origin: http://codereview.stackexchange.com/questions/23262/\
 *    using-standard-library-to-simplify-pairwise-iteration-of-container-values
 */
template<typename Iter, typename Func>
void combine_pairwise(Iter first, Iter last, Func func) {
	for(; first != last; ++first) {
		for(Iter next = first+1; next != last; ++next) //std::next(first) is c++11 only
			func(*first, *next);
	}
}

/**
 * Helper function for below function of mem_fun_bind2.
 * Origin: http://www.codeproject.com/Articles/7112/Pointers-to-Member-Functions-and-Functors
 */
template <typename C, typename Arg1, typename Arg2, typename Result>
struct mem_fun_bind2_t : public std::binary_function<Arg1, Arg2, Result> {
	Result (C::*pmf_ )( Arg1, Arg2 ) ;
	C& rC_ ;

	explicit mem_fun_bind2_t ( C& rC,
			Result (C::*pmf)( Arg1, Arg2 )) : rC_ ( rC ), pmf_ ( pmf ) { }

	Result operator () ( Arg1 a1, Arg2 a2 ) {
		return (rC_.*pmf_) ( a1, a2 ) ;
	}
};

/**
 * A function to be used for for example combine_pairwise above, but can also be used for for example a sort function.
 * It makes it easier to use member functions in these contexts. For newer compilers this might now be handled by
 * std::bind, but with std=c99 and before, the functions std::bind1st and std::mem_fun can only take arguments with only
 * one argument.
 *
 * @param    Result (C::*fn)( Arg1, Arg2 )		the member function with two arguments
 * @param    C&c								the instance of the corresponding class
 *
 * Example: combine_pairwise(v.begin(), v.end(), mem_fun_bind2 ( &DetectLineModuleExt::findMatches, *this ) );
 */
template <typename C, typename Arg1, typename Arg2, typename Result>
mem_fun_bind2_t<C, Arg1, Arg2, Result> mem_fun_bind2 ( Result (C::*fn)( Arg1, Arg2 ), C& c) {
	return mem_fun_bind2_t <C, Arg1, Arg2, Result>( c, fn ) ;
}

}


#endif /* DIM1ALGEBRA_HPP_ */
