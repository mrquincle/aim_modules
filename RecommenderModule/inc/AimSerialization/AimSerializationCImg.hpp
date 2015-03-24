/*
 * AimSerializationCImg.hpp
 *
 *  Created on: Apr 24, 2014
 *      Author: vliedel
 */

#ifndef AIMSERIALIZATIONCIMG_HPP_
#define AIMSERIALIZATIONCIMG_HPP_

#include <AimSerialization.hpp>
#include <string>
#include <iostream>
#include <CImg.h>

namespace rur {

class AimSerializationCImgRgb : public AimSerializationBase {
public:
	AimSerializationCImgRgb() {}
	~AimSerializationCImgRgb() {}

	void setImage(cimg_library::CImg<unsigned char>* img) {

		if (img == NULL) {
			std::cerr << mTag << " WARNING: image is NULL" << std::endl;
			throw AimException();
		}
//		if (img->size() < 1) {
//			std::cerr << mTag << " WARNING: image is empty" << std::endl;
//			throw AimException();
//		}
//		if (mImg->spectrum() != 3) {
//			std::cerr << mTag << " WARNING: image should be an RGB (spectrum=3) image" << std::endl;
//			throw AimException();
//		}

		mImg = img;
		setNumDims(3);
		setHeight(img->height());
		setWidth(img->width());
		setNumChannels(3);
	}

//	const cimg_library::CImg<unsigned char>* getImage() {
//		return mImg;
//	}

	template <typename OutputIterator>
	OutputIterator serializeHeader(const OutputIterator first, const OutputIterator last) {
		if (mImg == NULL || mImg->size() < 1) {
			std::cerr << mTag << " WARNING: image is empty" << std::endl;
			throw AimException();
		}
		if (mImg->spectrum() != 3) {
			std::cerr << mTag << " WARNING: image should be an RGB (spectrum=3) image" << std::endl;
			throw AimException();
		}

		return AimSerializationBase::serializeHeader(first, last);
	}

	template <typename OutputIterator>
	OutputIterator serializeData(const OutputIterator first, const OutputIterator last) {
		serializeDataCheck(first, last);

		OutputIterator it = first;

		// Not an efficient loop (CImg prefers for(c) for(y) for(x))
		for (int y=0; y<mImg->height(); ++y) {
			for (int x=0; x<mImg->width(); ++x) {
//				for (int c=0; c<mImg->spectrum(); ++c) {
//					*it++ = mImg->operator()(x,y,c);
//				}
				*it++ = mImg->operator()(x,y,0);
				*it++ = mImg->operator()(x,y,1);
				*it++ = mImg->operator()(x,y,2);
			}
		}

		return it;
	}

	template <typename InputIterator>
	InputIterator deserializeHeader(const InputIterator first, const InputIterator last) {
		InputIterator it = AimSerializationBase::deserializeHeader(first, last);
		if (mImg == NULL) {
			std::cerr << mTag << " WARNING: image is not set" << std::endl;
			throw AimException();
		}
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

		if (mImg == NULL) {
			std::cerr << mTag << " WARNING: image is not set" << std::endl;
			throw AimException();
		}

		int height = getHeight();
		int width = getWidth();
		mImg->assign(width, height, 1, 3);
		std::cout << mTag << " Created image of size: " << getHeight() << "x" << getWidth() << " size=" << mImg->size() << std::endl;

		if (mImg->size() != mDataSize) {
			std::cerr << mTag << " WARNING: image size does not match data size: " << mImg->size() << " != " << mDataSize << std::endl;
			throw AimException();
		}

		InputIterator it = first;

		// Not an efficient loop (CImg prefers for(c) for(y) for(x))
		for (int y=0; y<height; ++y) {
			for (int x=0; x<width; ++x) {
//				for (int c=0; c<channels; ++c) {
//					mImg->operator()(x,y,c) = *it++;
//				}
				mImg->operator()(x,y,0) = *it++;
				mImg->operator()(x,y,1) = *it++;
				mImg->operator()(x,y,2) = *it++;
			}
		}

		return it;
	}

private:
	cimg_library::CImg<unsigned char>* mImg;

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


#endif /* AIMSERIALIZATIONCIMG_HPP_ */
