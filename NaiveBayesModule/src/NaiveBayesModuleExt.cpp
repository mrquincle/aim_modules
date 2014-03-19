/**
 * @file NaiveBayesModuleExt.cpp
 * @brief NaiveBayesModule extension
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

#include <NaiveBayesModuleExt.h>
#include <stddev.hpp>
#include <gaussian.hpp>

using namespace rur;

NaiveBayesModuleExt::NaiveBayesModuleExt() {

}

NaiveBayesModuleExt::~NaiveBayesModuleExt() {

}

/**
 * The label to be expected can be seen as the "class" or the "type" of the entity involved. The entity involved in a
 * task like spam detection is very likely a "document". The document has to be represented as a series of integers
 * in this case. Each word (or stem, or emoticon) is indexed beforehand. Hence, the data following the label will be
 * a series of indices of which each index refers to a unique item (in this case "word"). Note, that because we use a
 * generative approach, it might also be possible to send in a "type" and get a typical document returned. The order
 * of the words is irrelevant: bag of words approach.
 */
void NaiveBayesModuleExt::Tick() {

	std::vector<int> * ss = readTrain();
	if (!ss->empty()) {
		std::cout << "Received training sample: ";
		std::copy(ss->begin(), ss->end(), std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		Train(*ss);
	}
	ss->clear();

	ss = readTest();
	if (!ss->empty()) {
		std::cout << "Received test sample: ";
		std::copy(ss->begin(), ss->end(), std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		Test(*ss);
	}
	ss->clear();
}

/**
 * Now, we test the classifier. We build a nd-array (a probability array) with the sample mean and variances 
 * calculated during training. A normal probability density function is assumed for the conditional probabilities.
 *
 * @param sample
 *   Data sample, without label
 *   sample[0]: data protocol version, should be 0
 *   sample[1]: number of elements, should be 1, only data
 *   sample[2]: data dimensions, should be AIM_TYPE_VECTOR
 *   sample[3]: length of data, variable, but should be same as during training
 *   sample[4-x]: data itself 
 */
void NaiveBayesModuleExt::Test(std::vector<int> & sample) {
	if (sample.size() < 5) return;
	if (sample[0] != 0) return; // only type=0 is understood
	if (sample[1] != 1) return; // expect one element, data
	if (sample[2] != AIM_TYPE_VECTOR) return; // expect the data element to be a vector
	int length = sample[3];
	if (dataitem_length && (length != dataitem_length)) {
		std::cerr << "All data items should be of the same length (" << length << " is not " << \
			dataitem_length << ")" << std::endl;
	} 

	const int data_start = 4;
	if (sample.size() != length+data_start) {
		std::cerr << "The length of the sample should be " << length << " + " << data_start << \
			", while it is " << sample.size() << std::endl;
		return;
	}
	std::vector<double> *d = new std::vector<double>(length);
	std::copy(sample.begin()+data_start,sample.begin()+data_start+length, d->begin() );

	std::cout << "Fill conditional probability table" << std::endl;
	// fill conditional probability table 
	probability_table.clear();
	std::vector<size_t> dim; 
	dim.push_back(stats.size()); // number of classes
	dim.push_back(length);       // number of variables
	std::cout << "Initialize table with dimensions: ";
	std::copy(dim.begin(), dim.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << std::endl;

	int data_dimension = d->size();
	int label_count = stats.size();

	probability_table.init(dim);
	std::map<int, stddev<double>*>::iterator iter;
	int j = 0;
	// iterate through (label, stats) pairs in std::map
	for (iter = stats.begin(); iter != stats.end(); ++iter, j++) {
		std::cout << "Inspect label: " << iter->first << std::endl;
		stddev<double> *s = iter->second;
		if (!s) {
			std::cerr << "Object for label " << iter->first << " is not allocated!" << std::endl;
			continue;
		} else {
			std::cout << "Mean: " << std::endl;
			std::copy(s->mean().begin(), s->mean().end(), std::ostream_iterator<double>(std::cout, " "));
			std::cout << std::endl;
			std::cout << "Variance: " << std::endl;
			std::copy(s->variance().begin(), s->variance().end(), std::ostream_iterator<double>(std::cout, " "));
			std::cout << std::endl;
			std::cout << "Standard deviation: " << std::endl;
			std::copy(s->std_dev().begin(), s->std_dev().end(), std::ostream_iterator<double>(std::cout, " "));
			std::cout << std::endl;
		}
		assert (data_dimension == s->dim());
		assert (data_dimension == s->std_dev().size());
		for (int i = 0; i < s->dim(); ++i) {
			//std::cout << "Calculate distance between " << (*d)[i] << " and " << s->mean()[i] << std::endl;
			double cond_prob = normal_pdf_stddev<double>((*d)[i],s->mean()[i],s->std_dev()[i]);
			//std::cout << "Set conditional probability for " << j << "," << i << " to " << cond_prob << std::endl;
			probability_table.set(j,i,cond_prob);
		}
	}
	std::cout << "Print entire probability table " << std::endl;
	std::cout << probability_table << std::endl;

	std::vector<double> posterior(stats.size());

	// naive implementation, multiplication (rather than addition in log-space)
	for (j=0; j != label_count; j++) {
		// set prior to 1/k (with k the number of classes)
		posterior[j] = 1/(double)label_count; 
		for (int i = 0; i < data_dimension; ++i) {
			double cond_prob = probability_table.get(j, i);
			std::cout << "Posterior for " << j << " becomes with " << cond_prob << ": " << posterior[j] * cond_prob << std::endl;
			posterior[j] *= cond_prob; 
		}
	}

	std::cout << "Posterior: " << std::endl;
	std::copy(posterior.begin(), posterior.end(), std::ostream_iterator<double>(std::cout, " "));
	std::cout << std::endl;

	std::vector<double>::iterator max = std::max_element(posterior.begin(), posterior.end());
	//std::cout << "Maximum element is: " << *max << std::endl;
	iter = stats.begin();
	size_t max_index = std::distance(posterior.begin(), max);
	std::advance(iter, max_index);
	int label = iter->first;
	std::cout << "Write most likely label " << label << " to port" << std::endl;
	writeClass(label);

	int header_size = 5, label_size = 1;
	std::vector<int> result(data_dimension + header_size + label_size);
	result[0] = 0;
	result[1] = 2;
	result[2] = AIM_TYPE_SCALAR;
	result[3] = AIM_TYPE_VECTOR;
	result[4] = data_dimension;
	result[5] = label;
	std::copy(d->begin(), d->end(), result.begin() + header_size + label_size);
	writeTestResult(result);
}

/**
 * First train the Bayes classifier. It expects its data in a specific form (see below). The data itself is thrown 
 * away, only the statistics are saved. That is, for each unique label, the average and variance are calculated.
 *
 * @param sample
 *    Data sample, plus label, and with size information included 
 *    sample[0]: data protocol version, should be 0
 *    sample[1]: number of elements, should be 2, a label and a data element
 *    sample[2]: label dimensions, should be a scalar (AIM_TYPE_SCALAR)
 *    sample[3]: data dimensions, should be a vector (AIM_TYPE_VECTOR)
 *    sample[4]: data size, length of the data vector, can be anything but should be the same over testing/training
 *    sample[5]: label itself
 *    sample[6-x]: the data itself
 */
void NaiveBayesModuleExt::Train(std::vector<int> & sample) {
	if (sample.size() < 6) return;
	if (sample[0] != 0) return; // only type=0 is understood
	if (sample[1] != 2) return; // expect two elements, label and data element
	if (sample[2] != AIM_TYPE_SCALAR) return; // expect the label to be a scalar (order-0 tensor)
	if (sample[3] != AIM_TYPE_VECTOR) return; // expect the data element to be a vector
	int length = sample[4];
	if (dataitem_length && (length != dataitem_length)) {
		std::cerr << "All data items should be of the same length (" << length << " is not " << \
			dataitem_length << ")" << std::endl;
	} 
	int label = sample[5];

	const int data_start = 6;
	if (sample.size() != length+data_start) {
		std::cerr << "The length of the sample should be " << length << " + " << data_start << \
			", while it is " << sample.size() << std::endl;
		return;
	}
	std::vector<double> *d = new std::vector<double>(length);
	std::copy(sample.begin()+data_start,sample.begin()+data_start+length, d->begin() );

	// calculate the sample averages and variances
	if (stats.find(label) == stats.end()) {
		stats[label] = new stddev<double>(length);
		std::cout << "Created new stat object of length " << stats[label]->dim() << std::endl;
	} 
	// add statistics to (hash) map with (labels, data) pairs
	stats[label]->push(*d);
}

/**
 * Calculate cumulants (average, variance) from samples.
 */
//void NaiveBayesModuleExt::SampleCumulants() {
//	std::vector<int> mLabels;
//	std::copy(labels.begin(), labels.end(), mLabels.begin());
//	std::sort(mLabels.begin(), mLabels.end());
//	std::vector<int>::iterator last, it0, it1;
//	last = std::unique(mLabels.begin(), mLabels.end());
//
//	std::vector<std::vector<double>* > averages;
//	std::vector<int> counts;
//
//	for (it0 = mLabels.begin(); it0 != last; ++it0) {
//		std::vector<double> & average = *new std::vector<double>(dataitem_length);
//		averages.push_back(&average);
//		counts.push_back(0);
//		for (int i = 0; i != labels.size(); ++i) {
//			if (*it0 == labels[i]) {
//				std::transform(dataset[i]->begin(), dataset[i]->end(), average.begin(), 
//						average.begin(), std::plus<double>() ); 
//				counts[i]++;
//			}
//		}
//	}
//
//	// calculate average by dividing it through no. of occurences
//	for (int i = 0; i < averages.size(); ++i) {
//		std::transform(averages[i]->begin(), averages[i]->end(), averages[i]->begin(), std::bind1st(std::multiplies<double>(), 1.0/counts[i]));
//	} 
//
//	// calculate variances by comparing every item with averages
//	for (int i = 0; i < last - mLabels.begin(); ++i) {
//		std::vector<double> & variance = *new std::vector<double>(dataitem_length);
//		variances.push_back(&variance);
//		counts.push_back(0);
//		std::vector<double> v(data_item_length);
//		for (int i = 0; i != labels.size(); ++i) {
//			if (*it0 == labels[i]) {
//				
//				std::transform(dataset[i]->begin(), dataset[i]->end(), variance.begin(), 
//						variance.begin(), std::plus<double>() ); 
//			}
//		}
//
//	} 
//}

bool NaiveBayesModuleExt::Stop() {
	return false;
}
