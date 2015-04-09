/*
 * AimSerializationOpenCV.hpp
 *
 *  Created on: Feb 11, 2014
 *      Author: vliedel
 */

#ifndef AIMSERIALIZATIONOPENCV_HPP_
#define AIMSERIALIZATIONOPENCV_HPP_

#include <AimSerialization/AimSerialization.hpp>
#include <string>
#include <iostream>
#include <opencv2/core/core.hpp>

namespace rur {

class AimSerializationOpenCvRgb : public AimSerializationBase {
public:
	AimSerializationOpenCvRgb() {}
	~AimSerializationOpenCvRgb() {}

	void setImage(const cv::Mat img) {
		if (img.empty()) {
			std::cerr << mTag << " WARNING: image is empty" << std::endl;
			throw AimException();
		}
		if (img.channels() != 3) {
			std::cerr << mTag << " WARNING: image should be an RGB (CV_8UC3) image" << std::endl;
			throw AimException();
		}
		mImg = img; // Does not copy data
		setNumDims(3);
		setHeight(img.rows);
		setWidth(img.cols);
		setNumChannels(3);
	}

	const cv::Mat getImage() {
		return mImg;
	}

	template <typename OutputIterator>
	OutputIterator serializeHeader(const OutputIterator first, const OutputIterator last) {
		if (mImg.empty()) {
			std::cerr << mTag << " WARNING: image is empty" << std::endl;
			throw AimException();
		}
		return AimSerializationBase::serializeHeader(first, last);
	}

	template <typename OutputIterator>
	OutputIterator serializeData(const OutputIterator first, const OutputIterator last) {
		serializeDataCheck(first, last);

		OutputIterator it = first;
		cv::Mat_<cv::Vec3b>::const_iterator itImg = mImg.begin<cv::Vec3b>(), itImgEnd = mImg.end<cv::Vec3b>();
		for (; itImg != itImgEnd; ++itImg) {
			*it++ = (*itImg)[2];
			*it++ = (*itImg)[1];
			*it++ = (*itImg)[0];
		}
		return it;
	}

	template <typename InputIterator>
	InputIterator deserializeHeader(const InputIterator first, const InputIterator last) {
		InputIterator it = AimSerializationBase::deserializeHeader(first, last);
		if (mNumDims != 3) {
			std::cerr << mTag << " WARNING: number of dimensions should be 3" << std::endl;
			throw AimException();
		}
		if (getNumChannels() != 3) {
			std::cerr << mTag << " WARNING: number of channels should be 3" << std::endl;
			throw AimException();
		}

		return it;
	}

	template <typename InputIterator>
	InputIterator deserializeData(const InputIterator first, const InputIterator last) {
		deserializeDataCheck(first, last);

		mImg.create(getHeight(), getWidth(), CV_8UC3); // 3 channel 8bit integer
		std::cout << mTag << " Created image of size: " << getHeight() << "x" << getWidth() << " size=" << mImg.total()*3 << std::endl;

		if (mImg.total()*3 != mDataSize) {
			std::cerr << mTag << " WARNING: image size does not match data size: " << mImg.total()*3 << " != " << mDataSize << std::endl;
			throw AimException();
		}

		InputIterator it = first;

		cv::Mat_<cv::Vec3b>::iterator itImg = mImg.begin<cv::Vec3b>(), itImgEnd = mImg.end<cv::Vec3b>();
		for (; itImg != itImgEnd; ++itImg) {
			(*itImg)[2] = *it++;
			(*itImg)[1] = *it++;
			(*itImg)[0] = *it++;
		}

		return it;
	}

private:
	cv::Mat mImg;

	size_t getHeight() {
		return mDimSizes[0];
	}
	size_t getWidth() {
		return mDimSizes[1];
	}
	size_t getNumChannels() {
		return mDimSizes[2];
	}

	void setHeight(size_t height) {
		setDimSize(0, height);
	}
	void setWidth(size_t width) {
		setDimSize(1, width);
	}
	void setNumChannels(size_t channels) {
		setDimSize(2, channels);
	}

};


} // namespace rur


#endif /* AIMSERIALIZATIONOPENCV_HPP_ */
