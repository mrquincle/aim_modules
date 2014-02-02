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
	if (ss) {
		Train(*ss);
	}
	// delete ss?

	ss = readTest();
	if (ss) {
		Test(*ss);
	}
}

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

	// fill conditional probability table 
	probability_table.clear();
	std::vector<size_t> dim; 
	dim.push_back(stats.size()); // number of classes
	dim.push_back(length);       // number of variables
	probability_table.init(dim);
	std::map<int, stddev<double>*>::iterator iter;
	int j = 0;
	for (iter = stats.begin(); iter != stats.end(); ++iter, j++) {
		stddev<double> *s = iter->second;
		for (int i = 0; i < s->size(); ++i) {
			double cond_prob = normal_pdf<double>((*d)[i],s->mean()[i],s->std_dev()[i]);
			probability_table.set(j,i,cond_prob);
		}
	}
}

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
	} 
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
