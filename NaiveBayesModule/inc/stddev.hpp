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
 * Calculate standard deviation, mean, etc. from items that are vectors! Be careful, this does not mean that the items 
 * of the vector are averaged. No, subsequent(!) vectors are averaged. This is done in an online (running, incremental)
 * fashion. Consequently, the complexity will be O(1) of the mean() and variance() functions, because they are already
 * calculated on push().
 *
 * Note that the data items are not actually stored, they are thrown away and cannot be recovered. This class is only
 * to calculate means and variances. It is not a dataset object.
 *
 * ATTENTION. The variable for the variance stores the "variance times k" and becomes really large for large k. If this
 * is your use case, adjust the code! One solution is to have both a multiplication at the beginning, and a division 
 * step at the end in push(). A better solution might think of subdividing k in k0 and k1, as containers with different 
 * weights. In case there is an efficient implementation available for that... A nice precursor is a +operator function
 * that can add the fields (average, variance, k) in a smart way. A function mode() might even switch the mode from
 * favoring a fast push() function versus a fast average() or variance() function.
 *
 * See also: http://www.johndcook.com/standard_deviation.html
 */
template<typename T>
class stddev {
	public:
		//! Set the dimension of the data items to be expected
		stddev(int dim): k(0) {
			avg.resize(dim);
			var.resize(dim);
			dev.resize(dim);
			old_avg.resize(dim);
			old_var.resize(dim);
			tmp0.resize(dim);
			tmp1.resize(dim);
		}

		//! Reset all counters, average and variances becomes zero
		void clear() {
			k = 0;
			std::fill(avg.begin(), avg.end(), T(0));
			std::fill(var.begin(), var.end(), T(0));
			std::fill(dev.begin(), dev.end(), T(0));
			std::fill(old_avg.begin(), old_avg.end(), T(0));
			std::fill(old_var.begin(), old_var.end(), T(0));
			std::fill(tmp0.begin(), tmp0.end(), T(0));
			std::fill(tmp1.begin(), tmp1.end(), T(0));
		}

		/**
		 *  Push a new data item into the dataset. This follows Knuth.
		 *  
		 *  k++;
		 *  if (k == 1) {
		 *    oldM = newM = x;
		 *    oldS = 0;
		 *  } else {
		 *    newM = oldM + (x - oldM)/k;
		 *    newS = oldS + (x - oldM) * (x - newM);
		 *    oldM = newM;
		 *    oldS = newS;
		 *  }
		 *
		 *  @param data
		 *    A data point of dimensionality as indicate by constructor parameter "dim"
		 */
		void push(std::vector<T> & data) {
			k++;
			if (k == 1) {
				// just set data
				std::copy(data.begin(), data.end(), avg.begin());
				std::copy(data.begin(), data.end(), old_avg.begin());
				std::fill(old_var.begin(), old_var.end(), T(0));
			} else {
				double factor = T(1)/T(k);
				
				// calculate new average, tmp0 = (x - oldM)
				std::transform(data.begin(), data.end(), old_avg.begin(), tmp0.begin(), std::minus<T>());
				// tmp1 = tmp0/k = (x - oldM)/k
				std::transform(tmp0.begin(), tmp0.end(), tmp1.begin(), std::bind1st(std::multiplies<T>(),factor));
				// avg = oldM + tmp1 = oldM + (x - oldM) / k
				std::transform(old_avg.begin(), old_avg.end(), tmp1.begin(), avg.begin(), std::plus<T>());
				
				// calculate new variance, tmp1 = (x - newM)
				std::transform(data.begin(), data.end(), avg.begin(), tmp1.begin(), std::minus<T>());
				// tmp0 = tmp0 * tmp1 = (x - oldM) * (x - newM)
				std::transform(tmp0.begin(), tmp0.end(), tmp1.begin(), tmp0.begin(), std::multiplies<T>());
				// var = oldS + tmp0 = oldS + (x - oldM) * (x - newM)
				std::transform(old_var.begin(), old_var.end(), tmp0.begin(), var.begin(), std::plus<T>());

				// set the old average and variance for next push, oldM = newM and oldS = newS.
				std::copy(avg.begin(), avg.end(), old_avg.begin());
				std::copy(var.begin(), var.end(), old_var.begin());
			}	
		}

		//! Return the mean: same as mean()
		std::vector<T> & average() {
			return avg;
		}

		//! Return the mean
		std::vector<T> & mean() {
			return avg;
		}

		//! Return the variance
		std::vector<T> & variance() {
			if (k <= 1) return var; // zero-vector
			double factor = T(1)/T(k-1);
			std::transform(var.begin(), var.end(), tmp0.begin(), std::bind1st(std::multiplies<T>(),factor));
			return tmp0;
		}

		//! Return the standard deviation, one additional square root operation compared to the variance
		std::vector<T> & std_dev() {
			if (k <= 1) return var; // zero-vector
			double factor = T(1)/T(k-1);
			std::transform(var.begin(), var.end(), tmp0.begin(), std::bind1st(std::multiplies<T>(),factor));
			std::transform(tmp0.begin(), tmp0.end(), dev.begin(), std::ptr_fun<T,T>(sqrt));
			return dev;
		}

		//! Number of data items hitherto received
		inline int size() {
			return k;
		}

		//! Dimension of data to be expected, pick avg.size() as typical
		inline int dim() {
			return avg.size();
		}

	private:
		//! Number of items received
		int k;

		//! Variable to store the average, is updated every push()
		std::vector<T> avg;

		//! Variable to store the variance times (k-1), is updated every push()
		std::vector<T> var;

		//! Temporary variable for standard deviation, only updated on call to std_dev()
		std::vector<T> dev;

		//! Store the old average and variance to calculate a running variance, used in push()
		std::vector<T> old_avg, old_var;

		//! Some additional temporary vectors to store intermediate results, used in push()
		std::vector<T> tmp0, tmp1;
};
