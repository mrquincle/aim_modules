/**
 * @file stddev.hpp
 * @brief Standard deviation, variance, and means in a running fashion
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

/**
 * Calculate standard deviation, mean, etc. from items that are vectors! This is not meant that the items of the vector
 * are averaged. No, series of vectors are averaged. This is done in a running fashion. That means that the complexity
 * will be O(1) of the mean() and variance() functions.
 *
 * See also: http://www.johndcook.com/standard_deviation.html
 */
template<typename T>
class stddev {
	public:
		stddev(int size): k(0) {
			avg.resize(size);
			var.resize(size);
			old_avg.resize(size);
			old_var.resize(size);
			tmp0.resize(size);
			tmp1.resize(size);
		}

		void clear() {
			k = 0;
			std::fill(avg.begin(), avg.end(), T(0));
			std::fill(var.begin(), var.end(), T(0));
			std::fill(old_avg.begin(), old_avg.end(), T(0));
			std::fill(old_var.begin(), old_var.end(), T(0));
			std::fill(tmp0.begin(), tmp0.end(), T(0));
			std::fill(tmp1.begin(), tmp1.end(), T(0));
		}

		void push(std::vector<T> & data) {
			k++;
			if (k == 1) {
				std::copy(data.begin(), data.end(), avg.begin());
				std::copy(data.begin(), data.end(), old_avg.begin());
				std::fill(var.begin(), var.end(), T(0));
			} else {
				// calculate new average
				std::transform(data.begin(), data.end(), old_avg.begin(), tmp0.begin(), std::minus<T>());
				std::transform(tmp0.begin(), tmp0.end(), tmp1.begin(), std::bind1st(std::divides<T>(),k));
				std::transform(old_avg.begin(), old_avg.end(), tmp1.begin(), avg.begin(), std::plus<T>());

				// calculate new variance
				std::transform(data.begin(), data.end(), avg.begin(), tmp1.begin(), std::minus<T>());
				std::transform(tmp0.begin(), tmp0.end(), tmp1.begin(), tmp0.begin(), std::multiplies<T>());
				std::transform(old_var.begin(), old_var.end(), tmp0.begin(), var.begin(), std::plus<T>());

				// set the old average and variance for next push
				std::copy(avg.begin(), avg.end(), old_avg.begin());
				std::copy(var.begin(), var.end(), old_var.begin());
			}	
		}

		std::vector<T> & average() {
			return avg;
		}

		std::vector<T> & mean() {
			return avg;
		}


		std::vector<T> & variance() {
			if (k <= 1) return var; // zero-vector
			std::transform(var.begin(), var.end(), tmp0.begin(), std::bind1st(std::divides<T>(),(k-1)));
			return tmp0;
		}

		std::vector<T> & std_dev() {
			if (k <= 1) return var; // zero-vector
			std::transform(var.begin(), var.end(), tmp0.begin(), std::bind1st(std::divides<T>(),(k-1)));
			std::transform(tmp0.begin(), tmp0.end(), dev.begin(), std::ptr_fun<T,T>(sqrt));
			return dev;
		}

		inline int size() {
			return k;
		}

	private:
		int k;

		std::vector<T> avg;

		std::vector<T> var;

		std::vector<T> dev;

		// we store the old average to calculate a running variance
		std::vector<T> old_avg;
		std::vector<T> old_var;

		// just temporary vectors to store intermediate results
		std::vector<T> tmp0, tmp1;
};
