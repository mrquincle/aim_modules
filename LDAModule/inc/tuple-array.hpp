#include <vector>
#include <nd-array.hpp>
#include <iostream>
#include <cassert>

/**
 * A tuple array is not a key-value map. Both values can be non-unique. It can be seen as a specific way to represent 
 * a matrix. 
 */
template <typename T>
struct tuple {
	// Default constructor
	tuple(T item0, T item1): elem0(item0), elem1(item1) {}
	// Copy constructor
	tuple(T other): elem0(other.elem0), elem1(other.elem1) {}
	// Relational equality operator
	bool operator ==(const tuple &other) {
		return ((other.elem0 == elem0) && (other.elem1 == elem1));
	}
	// Data fields
	T elem0, elem1;	
};

/**
 * A tuple array can be seen as a matrix.
 */
template <typename T>
class tuple_array {
public:
	typedef std::vector<tuple<T> > tuple_vector;
	typedef typename tuple_vector::iterator tuple_vector_iterator;

	tuple_array() {
		content.clear();
	}

	void push(tuple<T> & t) {
		content.push_back(t);
	}

	void push(T item0, T item1) {
		content.push_back(tuple<T>(item0, item1));
	}

	inline size_t size() {
		return content.size();
	}

	int count(T item0, T item1) {
		return count(tuple<T>(item0, item1));
	}

	/**
	 * Count all the tuples that are identical to the given parameter.
	 * 
	 * @param t
	 *   tuple (combination of objects) to compare with
	 */
	int count(tuple<T> & t) {
		int sum = 0;
		tuple_vector_iterator iter;
		for (iter = content.begin(); iter != content.end(); ++iter) {
			sum += (int)((*iter) == t);
		}
		return sum;
	}

	/**
	 * Count the number of objects that have this item at the first tuple location
	 */
	int countFirst(T item0) {
		int sum = 0;
		tuple_vector_iterator iter;
		for (iter = content.begin(); iter != content.end(); ++iter) {
			sum += (int)(iter->elem0 == item0);
		}
		return sum;
	}

	/**
	 * Count the number of objects that have this item at the second tuple location.
	 */
	int countSecond(T item1) {
		int sum = 0;
		tuple_vector_iterator iter;
		for (iter = content.begin(); iter != content.end(); ++iter) {
			sum += (int)(iter->elem1 == item1);
		}
		return sum;
	}

	/**
	 * Returns max of first and second element. Note that this does not need to correspond to a "real" occuring
	 * entity. The maximum of the first, and the maximum of the second element can occur in a different tuple.
	 */
	tuple<T> & max() {
		if (content.size() < 1) assert(false);
		tuple<T> & m(content.front());
		tuple_vector_iterator iter;
		for (iter = content.begin()+1; iter != content.end(); ++iter) {
			if (iter->elem0 > m.elem0) m.elem0 = iter->elem0;
			if (iter->elem1 > m.elem1) m.elem1 = iter->elem1;
		}
		return m;

	}

	/**
	 * The order in which content is pushed onto the internal data structure is preserved. Hence, it is easy to 
	 * retrieve an item after it has been pushed. 
	 */
	tuple<T> &operator[] (size_t index) {
		return content[index];
	}

	/**
	 * Return the array of tuples as an nd-array (actually, a matrix). In case a lot of counting is required a 
	 * matrix structure is much more convenient. The reason why a matrix cannot be used directly from the start is
	 * that the dimensions are not known necessarily. Note, that we do use the values directly. In case some kind
	 * of conversion is required to get a smaller matrix, this is up to the user.
	 */
	nd_array<T> & matrix() {
		tuple<T> m = max();
		if ((m.elem0 > 1024) || (m.elem1 > 1024)) {
			std::cerr << "Warning: are you sure you want to create a matrix of size: " << m.elem0 \
				<< " x " << m.elem1 << std::endl;
		}
		std::vector<size_t> dim(2);
		dim.push_back(m.elem0); 
		dim.push_back(m.elem1); 
		nd_array<T> & mat = *new nd_array<T>(dim);

		tuple_vector_iterator iter;
		for (iter = content.begin(); iter != content.end(); ++iter) {
			mat.add(iter->elem0, iter->elem1, 1);
		}
		return mat;
	}

private:
	//! Internal data structure is a (dynamic) STL vector of tuples
	tuple_vector content;


};
