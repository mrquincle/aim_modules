/*
 * AimSerialization.hpp
 *
 * This file is created at "DoBots".
 * This software is published under the GNU General Lesser Public Licence license (GPLv3).
 *
 * Copyright © 2014 Bart van Vliet <bart@almende.org>
 *
 * @author                   Bart van Vliet
 * @date                     Feb 14, 2014
 * @organisation             DoBots
 * @project                  AIM
 */

#ifndef AIMSERIALIZATION_HPP_
#define AIMSERIALIZATION_HPP_


#include <string>
#include <iterator>
#include <vector>
#include <numeric>
#include <stdexcept>

namespace rur {

// Ideas:
// For each tensor, work like this:
// If nDims is set, check if data has that amount of dims. Else, set nDims and work with that
// Same for height, width, channels, etc.
// Check size with: http://en.cppreference.com/w/cpp/iterator/distance
// Also see: https://github.com/mrquincle/equids/blob/master/bridles/common/dim1algebra.hpp

// Have AimSerializationHeader and AimSerializationBase (of which specialized classed are derived)
// Let the user call specialized.serializeHeader(), which calls
// serializationHeader.serialize(), Specialized::serializeHeader(), Specialized::serializeData()
// Can't: virtual template functions are impossible..
// Implies each specialized class has an object of type AimSerializationHeader, little overhead.
//
// For multiple tensors, let the user call:
// header.serialize(), specialized_1.serializeHeader(), specialized_2.serializeHeader(),
// specialized_1.serializeData(), specialized_2.serializeData()
//
// Implement serialize as operator<< and deserialize as operator>>


class AimException : public std::runtime_error {
public:
	AimException() : std::runtime_error("AimException") {}
};

class AimSerializationHeader {
public:
	AimSerializationHeader() :
		mDataType(0),
		mNumTensors(0),
		mHeaderSize(2) {
	}
	~AimSerializationHeader() {}

	void init(std::string tag) {
		setTag(tag);
	}

	void setTag(std::string tag) {
		mTag = tag;
	};

	int getTotalSize() {
		return getHeaderSize();
	}

	int getHeaderSize() {
		return mHeaderSize;
	}

	int getNumTensors() {
		return mNumTensors;
	}


	void setNumTensors(int num) {
		if (num < 1) {
			throw AimException();
		}
		mNumTensors = num;
	}

	template <typename OutputIterator>
	OutputIterator serialize(const OutputIterator first, const OutputIterator last) {
		__glibcxx_function_requires(_OutputIteratorConcept<OutputIterator>);
		__glibcxx_requires_valid_range(first, last);
		typename std::iterator_traits<OutputIterator>::difference_type size = std::distance(first, last);

		if (size < mHeaderSize) {
			std::cerr << mTag << "WARNING: container size too small to serialize header" << std::endl;
			throw AimException();
		}

		OutputIterator it = first;
		*it++ = mDataType;
		*it++ = mNumTensors;
		return it;
	}

	template <typename InputIterator>
	InputIterator deserialize(const InputIterator first, const InputIterator last) {
		__glibcxx_function_requires(_InputIteratorConcept<InputIterator>);
		__glibcxx_requires_valid_range(first, last);
//		typedef typename std::iterator_traits<InputIterator>::value_type ValueType;
//		typedef typename std::iterator_traits<InputIterator>::difference_type DistanceType;
		typename std::iterator_traits<InputIterator>::difference_type size = std::distance(first, last);
		if (size < mHeaderSize) {
			std::cerr << mTag << "WARNING: container size too small to deserialize header" << std::endl;
			throw AimException();
		}

		InputIterator it = first;
		mDataType = *it++;
		if (mDataType != 0) {
			std::cerr << mTag << " WARNING: data type should be 0" << std::endl;
			throw AimException();
		}
		int mNumTensors = *it++;
		if (mNumTensors < 1) {
			std::cerr << mTag << " WARNING: number of tensors should be at least 1" << std::endl;
			throw AimException();
		}
		return it;
	}


private:
	int mDataType;
	int mNumTensors;
	int mHeaderSize;
	std::string mTag;

};


class AimSerializationBase {
public:
	AimSerializationBase():
		mNumDims(0),
		mDataSize(0),
		mDimSizes(0),
		mHeaderSize(0),
		mUseHeader(true) {
			mSerializationHeader.setNumTensors(1);
	}
	~AimSerializationBase() {}



	int getTotalSize() {
		if (mUseHeader)
			return getHeaderSize() + getDataSize() + mSerializationHeader.getTotalSize();
		return getHeaderSize() + getDataSize();
	}

	int getHeaderSize() {
		if (mHeaderSize == 0) {
			calcHeaderSize();
		}
		return mHeaderSize;
	}

	int getDataSize() {
		if (mDataSize == 0) {
			calcDataSize();
		}
		return mDataSize;
	}

	void init(std::string tag, bool useHeader=true) {
		setTag(tag);
		setUseHeader(useHeader);
	}

	void setTag(std::string tag) {
		mTag = tag;
		if (mUseHeader)
			mSerializationHeader.setTag(tag);
	};

	void setNumDims(size_t numDims) {
		mNumDims = numDims;
		mDimSizes.resize(numDims);
		mHeaderSize = 0;
		mDataSize = 0;
	}

	void setDimSize(size_t dim, size_t dimSize) {
		if (dim >= mNumDims) {
			throw AimException();
		}
		mDimSizes[dim] = dimSize;
		mDataSize = 0;
	}

	void setUseHeader(bool useHeader) {
		mUseHeader = useHeader;
	}

//	template <typename OutputIterator>
//	OutputIterator serialize(const OutputIterator first, const OutputIterator last) {
//		OutputIterator it = first;
//		if (mUseHeader)
//			it = mSerializationHeader.serialize(it, last);
//		it = serializeHeader(it, last);
//		return serializeData(it, last);
//	}
//
//	template <typename InputIterator>
//	InputIterator deserialize(const InputIterator first, const InputIterator last) {
//		InputIterator it = first;
//		if (mUseHeader)
//			it = mSerializationHeader.deserialize(it, last);
//		it = deserializeHeader(it, last);
//		return deserializeData(it, last);
//	}

	template <typename OutputIterator>
	OutputIterator serializeHeader(const OutputIterator first, const OutputIterator last) {
		__glibcxx_function_requires(_OutputIteratorConcept<OutputIterator>);
		__glibcxx_requires_valid_range(first, last);
		//typedef typename std::iterator_traits<OutputIterator>::difference_type DistanceType;

		OutputIterator it = first;
		if (mUseHeader)
			it = mSerializationHeader.serialize(it, last);

		typename std::iterator_traits<OutputIterator>::difference_type size = std::distance(it, last);

		getHeaderSize();
		if (size < mHeaderSize) {
			std::cerr << mTag << "WARNING: container size too small to serialize header" << std::endl;
			throw AimException();
		}

		std::cout << mTag << " Base serialization of header" << std::endl;

		*it++ = mNumDims;
		for (int i=0; i<mNumDims; ++i) {
			*it++ = mDimSizes[i];
		}
		return it;
	}

	template <typename InputIterator>
	InputIterator deserializeHeader(const InputIterator first, const InputIterator last) {
		__glibcxx_function_requires(_InputIteratorConcept<InputIterator>);
		__glibcxx_requires_valid_range(first, last);
//		typedef typename std::iterator_traits<InputIterator>::value_type ValueType;
//		typedef typename std::iterator_traits<InputIterator>::difference_type DistanceType;

		InputIterator it = first;
		if (mUseHeader)
			it = mSerializationHeader.serialize(it, last);

		typename std::iterator_traits<InputIterator>::difference_type size = std::distance(it, last);
		if (size < 1)
			throw AimException();

		std::cout << mTag << " Base deserialization of header" << std::endl;

		mNumDims = *it++;
		if (mNumDims < 0) {
			std::cerr << mTag << " WARNING: number of dimensions should be >= 0" << std::endl;
			throw AimException();
		}
		mHeaderSize = 1+mNumDims;
		if (size < mHeaderSize) {
			std::cerr << mTag << " WARNING: container size too small to deserialize header" << std::endl;
			throw AimException();
		}
		mDataSize = 1;
		mDimSizes.resize(mNumDims);
		for (int i=0; i<mNumDims; ++i) {
			int dimSize = *it++;
			if (dimSize < 1) {
				std::cerr << mTag << " WARNING: dim size should be >= 1" << std::endl;
				throw AimException();
			}
			mDimSizes[i] = dimSize;
			mDataSize *= dimSize;
		}
		std::cout << mTag << " numdims=" << mNumDims << " datasize=" << mDataSize << std::endl;
		return it;
	}

	template <typename OutputIterator>
	OutputIterator serializeData(const OutputIterator first, const OutputIterator last) {
		serializeDataCheck(first, last);

		std::cout << mTag << " Base serialization of data" << std::endl;

		// Increase the iterator ?
		OutputIterator it = first + mDataSize;
		return it;
	}

	template <typename InputIterator>
	InputIterator deserializeData(const InputIterator first, const InputIterator last) {
		deserializeDataCheck(first, last);

		std::cout << mTag << " Base deserialization of data" << std::endl;

		// Increase the iterator ?
		InputIterator it = first + mDataSize;
		return it;
	}

//	template <typename InputIterator>
//	InputIterator operator>>(const InputIterator first) {
//
//	}

protected:

	std::string mTag;
	int mNumDims;
	std::vector<size_t> mDimSizes;
	int mHeaderSize;
	int mDataSize;
	AimSerializationHeader mSerializationHeader;

	template <typename OutputIterator>
	void serializeDataCheck(const OutputIterator first, const OutputIterator last) {
		__glibcxx_function_requires(_OutputIteratorConcept<OutputIterator>);
		__glibcxx_requires_valid_range(first, last);
//		typedef typename std::iterator_traits<OutputIterator>::difference_type DistanceType;
		typename std::iterator_traits<OutputIterator>::difference_type size = std::distance(first, last);

		getDataSize();
		if (size < mDataSize) {
			std::cerr << mTag << " WARNING: container size too small to serialize data" << std::endl;
			throw AimException();
		}
	}

	template <typename InputIterator>
	void deserializeDataCheck(const InputIterator first, const InputIterator last) {
		__glibcxx_function_requires(_InputIteratorConcept<InputIterator>);
		__glibcxx_requires_valid_range(first, last);
//		typedef typename std::iterator_traits<InputIterator>::difference_type DistanceType;
		typename std::iterator_traits<InputIterator>::difference_type size = std::distance(first, last);
		if (size < mDataSize) {
			std::cerr << mTag << " WARNING: container size too small to deserialize data" << std::endl;
			throw AimException();
		}
	}

private:

	bool mUseHeader;

	void calcHeaderSize() {
		mHeaderSize = 1+mNumDims;
		if (mNumDims != mDimSizes.size()) {
			std::cerr << mTag << " WARNING: number of dimensions does not match" << std::endl;
			throw AimException();
		}
	}

	void calcDataSize() {
		getHeaderSize();
		mDataSize = 1;
		for (int i=0; i<mNumDims; ++i) {
			if (mDimSizes[i] < 1) {
				std::cerr << mTag << " WARNING: dim size should be >= 1" << std::endl;
				throw AimException();
			}
			mDataSize *= mDimSizes[i];
		}
	}
};

/*
 * Idea:
 * Have 1 class that can load several serialization classes, one for each tensor
 * virtual template functions are impossible..

class AimSerialization {
public:
	AimSerialization(): mDataType(0), mNumTensors(0), mSerializedSize(0), mSerializations(0), mBaseSerializations(0) {}
	~AimSerialization() {}

	void setTag(std::string tag) {
		mTag = tag;
		for (size_t i=0; i<mSerializations.size(); ++i) {
			mSerializations[i]->setTag(tag);
		}
	};

	void add(AimSerializationBase* serialization) {
		serialization->setTag(mTag);
		mSerializations.push_back(serialization);
	}

	size_t getRequiredSize() {
		mNumTensors = mSerializations.size();
		mSerializedSize=2;
		for (size_t i=0; i<mNumTensors; ++i) {
			mSerializedSize += mSerializations[i]->getHeaderSize();
			mSerializedSize += mSerializations[i]->getDataSize();
		}
		return mSerializedSize;
	}

	template <typename OutputIterator>
	OutputIterator serialize(const OutputIterator first, const OutputIterator last) {
		__glibcxx_function_requires(_OutputIteratorConcept<OutputIterator>);
		__glibcxx_requires_valid_range(first, last);
		typename std::iterator_traits<OutputIterator>::difference_type size = std::distance(first, last);

		// Check size
		getRequiredSize(); // shouldn't be necessary, since it should be called before serializing
		if (size < mSerializedSize) {
			std::cerr << mTag << " WARNING: container size too small to serialize data" << std::endl;
			throw AimException();
		}

		OutputIterator it = first;

		// Write headers
		*it++ = mDataType;
		*it++ = mNumTensors;
		for (size_t i=0; i<mNumTensors; ++i) {
			it = mSerializations[i]->serializeHeader(it, last);
		}

		// Write data
		for (size_t i=0; i<mNumTensors; ++i) {
			it = mSerializations[i]->serializeData(it, last);
		}

		return it;
	}

	template <typename InputIterator>
	InputIterator deserialize(const InputIterator first, const InputIterator last) {
		__glibcxx_function_requires(_InputIteratorConcept<InputIterator>)
		__glibcxx_requires_valid_range(first, last);
		typename std::iterator_traits<InputIterator>::difference_type size = std::distance(first, last);

		if (size < 2)
			throw AimException();

		InputIterator it(first);

		int mDataType = *it++;
		if (mDataType != 0) {
			std::cerr << mTag << " WARNING: data type should be 0" << std::endl;
			throw AimException();
		}
		int mNumTensors = *it++;
		if (mNumTensors < mSerializations.size()) {
			std::cerr << mTag << " WARNING: number of tensors should be at least " << mSerializations.size() << std::endl;
			throw AimException();
		}

		std::cout << mTag << " deserializing headers of " << mNumTensors << " tensors" << std::endl;

		// Read header of tensors
		for (size_t i=0; i<mSerializations.size(); ++i) {
			it = mSerializations[i]->deserializeHeader(it, last);
		}

		// Read header of remaining tensors
		mBaseSerializations.resize(mNumTensors - mSerializations.size());
		for (size_t i=0; i<mBaseSerializations.size(); ++i) {
			it = mBaseSerializations[i].deserializeHeader(it, last);
		}

		std::cout << mTag << " deserializing data" << std::endl;

		// Read data of tensors
		for (size_t i=0; i<mSerializations.size(); ++i) {
			it = mSerializations[i]->deserializeData(it, last);
		}

		// Read data of remaining tensors
		for (size_t i=0; i<mBaseSerializations.size(); ++i) {
			it = mBaseSerializations[i].deserializeHeader(it, last);
		}

		return it;
	}

private:
	std::string mTag;
	int mDataType;
	size_t mNumTensors;
	size_t mSerializedSize;
	std::vector<AimSerializationBase*> mSerializations;
	std::vector<AimSerializationBase> mBaseSerializations;
};
*/

} // namespace rur

#endif /* AIMSERIALIZATION_HPP_ */
