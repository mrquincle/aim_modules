/**
 * @file gaussian.hpp
 * @brief A Gaussian function (or normal function). Note, this is not a random generator, just the function.
 *
 * This file is created at "Distributed Organisms B.V.". It is open-source software and part of "Robotic Suite". 
 * This software is published under the GNU General Lesser Public license (LGPLv3).
 *
 * Copyright © 2014 Anne C. van Rossum <anne@dobots.nl>
 *
 * @author                   Anne C. van Rossum
 * @date                     Jan 31, 2014
 * @organisation             Distributed Organisms B.V.
 * @project                  Statistics Suite
 */

#pragma once

#include <math.h>

/**
 * The normal probability density function calculates a distance in "exponential" space between the two points "x" and 
 * "m". This is scaled through the variance "s". Usually the function is used to define a distance between a fixed 
 * point at "m" (the mean), and a given diffusion rate "s" around it (the variance).
 *
 * A Gaussian is known to many, but do you actually know how for a fixed distance between x and m, the function varies
 * with s, the variance? Check it on http://www.wolframalpha.com/input/?i=1%2Fx+exp%28-6%2Fx%5E2%29
 *
 * @template T               type (e.g. float, int)
 * @param x                  given data point
 * @param m                  mean 
 * @param s                  variance (should not be 0)
 * 
 */
template <typename T>
T normal_pdf(T x, T m, T s)
{
	if (!s) return T(0);
	static const T M_1_SQRT2PI = 0.39894228040143267793994605993438186847585863116493;
	T a = (x - m);
	std::cout << "var = " << s << std::endl;
	std::cout << "x - m = " << a << std::endl;
	std::cout << "1/(sqrt(2*pi) * s = " << M_1_SQRT2PI / std::sqrt(s) << std::endl;
	return M_1_SQRT2PI / std::sqrt(s) * std::exp(-T(0.5) * a * a / s);
}

//! Similar to normal_pdf, but using standard deviation, d = sqrt(s)
template <typename T>
T normal_pdf_stddev(T x, T m, T d) {
	if (!d) return T(0);
	static const T M_1_SQRT2PI = 0.39894228040143267793994605993438186847585863116493;
	T a = (x - m) / d;
	return M_1_SQRT2PI / d * std::exp(-T(0.5) * a * a);
}
