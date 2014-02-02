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
 * @template T               type (e.g. float, int)
 * @param x                  given data point
 * @param m                  mean 
 * @param s                  variance (should not be 0)
 * 
 */
template <typename T>
T normal_pdf(T x, T m, T s)
{
    static const T M_1_SQRT2PI = 0.39894228040143267793994605993438186847585863116493;
    T a = (x - m) / s;

    return M_1_SQRT2PI / s * std::exp(-T(0.5) * a * a);
}
