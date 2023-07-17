#include <iostream>

#include <cstdio>
#include <cassert>

template <typename T>
class BinaryHeap;

/* Auxilary structure for BinaryHeap
to manage all the logic connected with arrays.*/
template<typename T>
class A_BhBase {
	friend class BinaryHeap<T>;

	// Size of comprised type.
	static constexpr size_t kTypeSize = sizeof(T);
	// Initial capacity of base of heap.
	static constexpr size_t kInitCap  = 1;

 public:
 	A_BhBase() noexcept
 		: sz_(0),
 			cap_(kInitCap),
 			arr_(A_AllocDfltBuf(kInitCap)) {}

 	// Prints all the elements in array.
	void Print() {
		assert(sz_ > 0 &&
			"Binary heap's array is empty!\n");

		for (size_t el_idx = 0; el_idx < sz_ - 1; ++el_idx) {
			std::cout << arr_[el_idx] << ' ';
		}

		std::cout << arr_[sz_ - 1] << '\n';
	}

	// Returns current size of heap.
	size_t Size() {
		return sz_;
	}

	// Inserts new element 'el'.
	void Insert(T el) {
		if (sz_ == cap_) {
			A_ReallocBuf(cap_ << 1);
		}

		arr_[sz_] = el;
		++sz_;
	}

	// Deletes element with index = 'idx'.
	void Delete(size_t idx) {
		assert(idx < cap_ &&
			"'idx' beyound array's capacity!\n"
			"Or negative number passed in the function like 'size_t'\n");

		for (size_t el_idx = idx; el_idx < sz_ - 1; ++el_idx) {
			arr_[el_idx] = arr_[el_idx + 1];
		}

		--sz_;
	}

	// Binary search.
	int64_t Find(T el, size_t cur_idx = 0) {
		assert(cur_idx < cap_ &&
			"'cur_idx' beyound array's capacity!\n"
			"Or negative number passed in the function like 'size_t'\n");

		if (el > arr_[cur_idx]) {
			return -1;
		}

		if (el == arr_[cur_idx]) {
			return cur_idx;
		}

		int64_t result = -1;

		if ((cur_idx << 1) + 1 < sz_) {
			result = Find(el, (cur_idx << 1) + 1);
		}

		if ((cur_idx << 1) + 2 < sz_ && result == -1) {
			result = Find(el, (cur_idx << 1) + 2);
		}

		return result;
	}

	/* All A_functions:
	auxilary functions to decompose system work.*/
 private:
 	/* Allocates new buf
 	creates new buffer with 'n_els' elements in itself.*/
	T *A_AllocDfltBuf(size_t n_els) {
		T *buf = new T[n_els];

		for (size_t el_idx = 0; el_idx < n_els; ++el_idx) {
			buf[el_idx] = T();
		}

		return buf;
	}


	/* Deallocates buff and
	calls destructors for first buf_sz elements.*/
	void A_DeallocBuf(T *b, size_t buf_sz) {
		for (size_t el_idx = 0; el_idx < buf_sz; ++el_idx) {
			(b + el_idx)->~T();
		}

		delete[] b;
	}

	// Reallocates 'arr_' buffer, sets new capacity and size.
	void A_ReallocBuf(size_t new_cap) {
		T *new_arr = A_AllocDfltBuf(new_cap);

		for (size_t el_idx = 0; el_idx < sz_; ++el_idx) {
			new_arr[el_idx] = arr_[el_idx];
		}

		A_DeallocBuf(arr_, sz_);
		arr_ = new_arr;
		cap_ = new_cap;
	}

 private:
 	// Current number of elements in array.
	size_t sz_{0};
	// Current max probable size of array.
	size_t cap_{0};

	T *arr_ = nullptr;
};

// Templated binary heap class.
template<typename T>
class BinaryHeap {
 public:
	BinaryHeap():
			arr_() {}

	~BinaryHeap() = default;

	size_t Size() {
		return arr_.Size();
	}

	void Insert(T el) {
		if (arr_.sz_ == arr_.cap_) {
			arr_.A_ReallocBuf(arr_.cap_ * 2);
		}

		arr_.Insert(el);
		A_SiftUp();
	}

	int64_t Find(T el) {
		return arr_.Find(el);
	}

	void Erase(T el) {
		int64_t idx_of_el = Find(el);

		if (idx_of_el != -1) {
			arr_.Delete(idx_of_el);
		}

		A_Heapify(idx_of_el);
	}

	void Print() {
		std::cout << "bin heap sz: " << arr_.sz_ << '\n';
		arr_.Print();
	}

	/* All A_functions:
	auxilary functions to decompose system work.*/
 private:
 	void A_SiftUp() {
 		int64_t cur_child  =  arr_.sz_  - 1;
 		int64_t cur_parent = (cur_child - 1) >> 1;

 		while (cur_child > 0 && arr_.arr_[cur_child] > arr_.arr_[cur_parent]) {
 			std::swap(arr_.arr_[cur_child], arr_.arr_[cur_parent]);

 			cur_child  =  cur_parent;
 			cur_parent = (cur_child - 1) >> 1;
 		}
 	}

 	void A_Heapify(size_t idx) {
 		size_t largest_child = 0;

 		size_t  left_child = 0;
 		size_t right_child = 0;

 		while (true) {
 			left_child  = (idx << 1) + 1;
 			right_child = (idx << 1) + 1;

 			largest_child = idx;

 			if (left_child < arr_.sz_ && arr_.arr_[largest_child] < arr_.arr_[left_child]) {
 				largest_child = left_child;
 			}

 			if (right_child < arr_.sz_ && arr_.arr_[largest_child] < arr_.arr_[right_child]) {
 				largest_child = right_child;
 			}

 			if (largest_child == idx) {
 				break;
 			}

 			std::swap(arr_.arr_[largest_child], arr_.arr_[idx]);
 			idx = largest_child;
 		}
 	}

 private:
	A_BhBase<T> arr_{};
};

// templated using in c++
template<typename T>
using bh_t = BinaryHeap<T>;

int main() {
	bh_t<int> bh;

	bh.Insert(111);
	bh.Insert(222);
	bh.Insert(333);
	bh.Insert(999);
	bh.Insert(666);
	bh.Insert(555);
	std::cout << bh.Find(222) << '\n';

	// std::cout << "index of el:" << bh.Find(222) << '\n';
	bh.Erase(555);
	bh.Erase(666);
	bh.Erase(999);

	bh.Print();

	return 0;
}