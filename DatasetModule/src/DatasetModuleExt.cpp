/**
 * @file DatasetModuleExt.cpp
 * @brief DatasetModule extension
 *
 * This file is created at "Distributed Organisms B.V.". It is open-source software and part of "Robotic Suite". 
 * This software is published under the GNU General Lesser Public license (LGPLv3).
 *
 * Copyright © 2014 Anne C. van Rossum <anne@dobots.nl>
 *
 * @author                   Anne C. van Rossum
 * @date                     Mar 19, 2014
 * @organisation             Distributed Organisms B.V.
 * @project                  Robotic Suite
 */

#include <DatasetModuleExt.h>

#include <unistd.h>

using namespace rur;

/**
 * Solution suggested at:
 *   https://stackoverflow.com/questions/8906545/how-to-initialize-a-vector-in-c
 */
template <typename T>
class make_vector {
public:
	typedef make_vector<T> my_type;
	my_type& operator<< (const T& val) {
		data_.push_back(val);
		return *this;
	}
	operator std::vector<T>() const {
		return data_;
	}
private:
	std::vector<T> data_;
};

//! Replace with your own code
DatasetModuleExt::DatasetModuleExt(): test_index(0), train_index(0) {
}

//! Replace with your own code
DatasetModuleExt::~DatasetModuleExt() {

}

//! Replace with your own code
void DatasetModuleExt::Tick() {
	if (!train_index) {
		// make sure there is time to set up the outgoing connections
		sleep(3);
	}
	//sleep(1);
	if (train_index < 8) {
		Train();
	} else if (test_index < 1) {
		sleep(2);
		Test();
	} else {	
		CheckResult();
	}
}

/**
 * With new compiler extensions things like data = {600, 180, 12} are possible. However, preferably no such compiler
 * options are used to make compiling for other platforms more likely to succeed.
 */
void DatasetModuleExt::Train() {
	std::cout << "Send training sample " << train_index << std::endl;
	std::vector<int> data; int label;
	switch (train_index) {
	case 0:
		data = make_vector<int>() << 600 << 180 << 12;
		label = 0;
		break;
	case 1: 
		data = make_vector<int>() << 592 << 190 << 11;
		label = 0;
		break;
	case 2: 
		data = make_vector<int>() << 558 << 170 << 12;
		label = 0;
		break;
	case 3:
		data = make_vector<int>() << 592 << 165 << 10;
		label = 0;
		break;
	case 4:
		data = make_vector<int>() << 500 << 100 << 6;
		label = 1;
		break;
	case 5: 
		data = make_vector<int>() << 550 << 150 << 8;
		label = 1;
		break;
	case 6:
		data = make_vector<int>() << 542 << 130 << 7;
		label = 1;
		break;
	case 7:
		data = make_vector<int>() << 575 << 150 << 9;
		label = 1;
		break;
	default:
		return;
	}
	int header_size = 5; int label_size = 1;
	std::vector<int> sample(data.size()+label_size+header_size);
	sample[0] = 0;
	sample[1] = 2;
	sample[2] = 0; // scalar
	sample[3] = 1; // vector
	sample[4] = data.size();
	sample[5] = label;
	std::copy(data.begin(), data.end(), sample.begin()+header_size+label_size);

	writeTrainSexClassification(sample);

	train_index++;
}

void DatasetModuleExt::Test() {
	std::cout << "Send test sample " << test_index << std::endl;
	// sample should be classified as female (1)
	std::vector<int> data; 
	switch (test_index) {
	case 0:
		// fill in your data here :-)
		data = make_vector<int>() << 600 << 130 << 8;
		break;
	default:
		return;
	}
	int header_size = 4;
	std::vector<int> sample(data.size() + header_size);
	sample[0] = 0;
	sample[1] = 1;
	sample[2] = 1; // vector
	sample[3] = data.size();
	std::copy(data.begin(), data.end(), sample.begin()+header_size);

	std::cout << "Actually write sample to port" << std::endl;
	writeTestSexClassification(sample);

	test_index++;
}

void DatasetModuleExt::CheckResult() {
	int *label = readExpectedPosterior(true);
	if (!label) {
		std::cerr << "No label object" << std::endl;
	}
	if (*label == 1) {
		std::cout << "Correct classified!!!!!!!!!!!!!!!!!!!!!!!!! Hooray!!!" << std::endl;
	} else {
		std::cout << "Incorrect classified.... Pity....." << std::endl;
	}
	Stop();
}

//! Replace with your own code
bool DatasetModuleExt::Stop() {
	return false;
}

