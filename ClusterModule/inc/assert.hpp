/**
 * 456789------------------------------------------------------------------------------------------------------------120
 *
 * @brief ...
 * @file assert.hpp
 * 
 * This file is created at Almende B.V. and Distributed Organisms B.V. It is open-source software and belongs to a
 * larger suite of software that is meant for research on self-organization principles and multi-agent systems where
 * learning algorithms are an important aspect.
 *
 * This software is published under the GNU Lesser General Public license (LGPL).
 *
 * It is not possible to add usage restrictions to an open-source license. Nevertheless, we personally strongly object
 * against this software being used for military purposes, factory farming, animal experimentation, and "Universal
 * Declaration of Human Rights" violations.
 *
 * Copyright (c) 2013 Anne C. van Rossum <anne@almende.org>
 *
 * @author    Anne C. van Rossum
 * @date      Oct 4, 2013
 * @project   Replicator 
 * @company   Almende B.V.
 * @company   Distributed Organisms B.V.
 * @case      Clustering
 */

#ifndef ASSERT_HPP_
#define ASSERT_HPP_

#define ENABLE_ASSERTS 1

#if ENABLE_ASSERTS == 1

//! Helper function for printing asserts
#define ASSERT(condition) { \
	if(!(condition)){ \
		std::cerr << "ASSERT FAILED: " << #condition << " @ " << __FILE__ << " (" << __LINE__ << ")" << std::endl; \
		assert(condition); \
	} \
	}

//! Helper function for printing asserts
#define ASSERT_EQ(x,y) \
	if (!(x == y)) { \
		std::cerr << "ASSERT FAILED: " << #x << " == " << #y << " fails, specifically: " << x << " == " << y << \
		" fails @ " << __FILE__ << " (" << __LINE__ << ")" << std::endl; \
		assert(x == y); \
	}

//! Helper function for printing asserts
#define ASSERT_ALMOST_EQ(x,y,margin) \
	if (!(abs(x - y) < margin)) { \
		std::cerr << "ASSERT FAILED: " << #x << " == " << #y << " fails, specifically: " << x << " == " << y << \
		" fails @ " << __FILE__ << " (" << __LINE__ << ")" << std::endl; \
		assert(abs(x - y) < margin); \
	}

#define ASSERT_NEQ(x,y) \
	if (!(x != y)) { \
		std::cerr << "ASSERT FAILED: " << #x << " != " << #y << " fails, specifically: " << x << " != " << y << \
		" fails @ " << __FILE__ << " (" << __LINE__ << ")" << std::endl; \
		assert(x != y); \
	}

#define ASSERT_LT(x,y) \
	if (!(x < y)) { \
		std::cerr << "ASSERT FAILED: " << #x << " < " << #y << " fails, specifically: " << x << " < " << y << \
		" fails @ " << __FILE__ << " (" << __LINE__ << ")" << std::endl; \
		assert(x < y); \
	}

#define ASSERT_LEQ(x,y) \
	if (!(x <= y)) { \
		std::cerr << "ASSERT FAILED: " << #x << " <= " << #y << " fails, specifically: " << x << " <= " << y << \
		" fails @ " << __FILE__ << " (" << __LINE__ << ")" << std::endl; \
		assert(x <= y); \
	}

#define ASSERT_GT(x,y) \
	if (!(x > y)) { \
		std::cerr << "ASSERT FAILED: " << #x << " > " << #y << " fails, specifically: " << x << " > " << y << \
		" fails @ " << __FILE__ << " (" << __LINE__ << ")" << std::endl; \
		assert(x > y); \
	}

#define ASSERT_GEQ(x,y) \
	if (!(x >= y)) { \
		std::cerr << "ASSERT FAILED: " << #x << " >= " << #y << " fails, specifically: " << x << " >= " << y << \
		" fails @ " << __FILE__ << " (" << __LINE__ << ")" << std::endl; \
		assert(x >= y); \
	}

#else

#define ASSERT(condition)
#define ASSERT_EQ(x,y)
#define ASSERT_ALMOST_EQ(x,y,margin)
#define ASSERT_NEQ(x,y)
#define ASSERT_LT(x,y)
#define ASSERT_LEQ(x,y)
#define ASSERT_GT(x,y)
#define ASSERT_GEQ(x,y)

#endif


#endif /* ASSERT_HPP_ */
