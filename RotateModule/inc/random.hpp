/**
 * @brief
 * @file random.hpp
 * 
 * This file is created at Almende B.V. It is open-source software and part of the Common
 * Hybrid Agent Platform (CHAP). A toolbox with a lot of open-source tools, ranging from
 * thread pools and TCP/IP components to control architectures and learning algorithms.
 * This software is published under the GNU Lesser General Public license (LGPL).
 *
 * It is not possible to add usage restrictions to an open-source license. Nevertheless,
 * we personally strongly object against this software being used by the military, in the
 * bio-industry, for animal experimentation, or anything that violates the Universal
 * Declaration of Human Rights.
 *
 * Copyright © 2012 Anne van Rossum <anne@almende.com>
 *
 * @author    Anne C. van Rossum
 * @date      Oct 23, 2012
 * @project   Replicator FP7
 * @company   Almende B.V.
 * @case      Cognitive Sensor Fusion
 */

/************************************************************************************
 * Configuration
 ************************************************************************************/

#ifndef RANDOM_HPP_
#define RANDOM_HPP_

#include <iterator>
#include <algorithm>
#include <set>
//#include <iostream>

/**
 * @brief Returns a random subset from a potentially very large set
 * The function random_subset returns a (small) random subset from a large STL container. It is
 * different from using std::random_shuffle and then picking the first k elements. It is namely
 * of the order O(k) and not of the order O(N) with k the number of elements to pick, and N the
 * total number of elements. It is the so-called Floyd Algorithm.
 * @see http://eyalsch.wordpress.com/2010/04/01/random-sample/
 *
 * @ingroup random_algorithms
 * @param first   start of range
 * @param last    end of range
 * @param int     elements
 * @param result  output of random elements
 * @return Iterator referencing the first instance of the random subset
 */
template<typename ForwardIterator, typename OutputIterator>
OutputIterator
random_subset(ForwardIterator first, ForwardIterator last, int elements, OutputIterator result)
{
	// concept requirements
	typedef typename std::iterator_traits<ForwardIterator>::value_type ValueType;
	typedef typename std::iterator_traits<ForwardIterator>::difference_type DiffType;

	__glibcxx_function_requires(ForwardIteratorConcept<ForwardIterator>);
	__glibcxx_requires_valid_range(first, last);
	__glibcxx_function_requires(_OutputIteratorConcept<OutputIterator, ValueType>);

	if (first == last)
		return first;

	std::set<int> indices; indices.clear();
	for (ForwardIterator i = last - elements; i != last; ++i) {
		int pos = std::rand() % (int)((i - first)+1); // pick a random index
//		std::cout << "Position: " << pos << std::endl;
		if (indices.find(pos) != indices.end()) {
			indices.insert(i-first);
//			std::cout << "Insert i: " << (i - first) << std::endl;
			*result = *i;
		} else {
//			std::cout << "Insert pos: " << (pos) << std::endl;
			indices.insert(pos);
			*result = *(first+pos);
		}
		++result;
	}
	return result;
}



//public static <T> Set<T> randomSample4(List<T> items, int m){
//    HashSet<T> res = new HashSet<T>(m);
//    int n = items.size();
//    for(int i=n-m;i<n;i++){
//        int pos = rnd.nextInt(i+1);
//        T item = items.get(pos);
//        if (res.contains(item))
//            res.add(items.get(i));
//        else
//            res.add(item);
//    }
//    return res;
//}

#endif /* RANDOM_HPP_ */
