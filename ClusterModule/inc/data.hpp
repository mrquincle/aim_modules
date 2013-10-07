/**
 * @file data.hpp
 * @brief Data file
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
 * @author  ...
 * @date    ...
 * @company Almende B.V.
 * @case    Artificial Intelligence Framework
 */

#include <string>
#include <locale>
#include <sstream>
#include <vector>
#include <iterator>
#include <iostream>
#include <algorithm>

struct comma_ws: std::ctype<char> {
	static const mask* make_table() {
		static std::vector<mask> v(classic_table(), classic_table() + table_size);
		v[','] |= space;  // comma will be classified as whitespace
		v[' '] |= space;  // space will also be classified as whitespace
		v['.'] &= ~space; // dot is nothing
		return &v[0];
	}
	comma_ws(std::size_t refs = 0) : ctype<char>(make_table(), false, refs) {}
};

/**
 * Data is just a set of vectors.
 */
template <typename T>
class data {
public:
	data() {
		set.clear();
		index = -1;
	}
	virtual ~data() {}

	/*inline void test() {
		if (set.size() == 0) {
			std::cout << "Empty" << std::endl;
			return;
		}
		int i = 0;
		std::cout << "Size of first item is " << set[i].size() << std::endl;

		std::cout << "Content is " << set[i][0];
		for (int j = 1; j < set[i].size(); ++j) {
			std::cout << ", " << set[i][j];
		}
		std::cout << std::endl;
	}*/

	std::vector<T> &pop() {
		return set[(++index) % set.size()];
	}

	inline size_t size() { return set.size(); }

private:
	int index;

	typedef std::vector<T> sample;
	std::vector < sample > set;

	friend std::ostream &operator<<(std::ostream &out, data d) {
		for (int i = 0; i < d.set.size(); ++i) {
			if (d.set[i].size() > 0) {
				out << d.set[i][0];
				for (int j = 1; j < d.set[i].size(); ++j) {
					out << ',' << d.set[i][j];
				}
				out << std::endl;
			}
		}
		return out;
	}

	friend std::istream &operator>>(std::istream &in, data &d) {
		std::string buffer;
		while (std::getline(in, buffer)) {
			std::stringstream ss(buffer);
			std::vector<T> t;
			ss.imbue(std::locale(std::locale(), new comma_ws));
			std::copy(std::istream_iterator<T>(ss), std::istream_iterator<T>(), std::back_inserter(t));
			if (!t.empty())
				d.set.push_back(t);
		}
		return in;
	}
};
