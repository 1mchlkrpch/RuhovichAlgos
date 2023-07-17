#include <iostream>

#include <cstdint>

#include <cstddef>
#include <cassert>

static constexpr size_t kMaxAlignSize = alignof(std::max_align_t);

template<typename T, size_t cap, int (*Cmp)(T&, T&)>
class A_DHeapBase;

/** 
 * @brief Auxilary structure for BinaryHeap
 * to manage all the logic connected with arrays.
 * 
 * @param Tp  type of comprised data.
 */
template<typename Tp>
class A_ArrayBase {
	template<typename T, size_t cap, int (*C)(T&, T&)>
	friend class A_DHeapBase;

	// Size of comprised type.
	static constexpr size_t kTypeSize = sizeof(Tp);
	// Initial capacity of base of heap.
	static constexpr size_t kInitCap  = 1;

 public:
 	A_ArrayBase() noexcept
 		: sz_(0), cap_(kInitCap), arr_(A_AllocDfltBuf(kInitCap)) {}

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
	void PushBack(Tp el) {
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

		if (sz_ == cap_ / 4) {
			A_ReallocBuf(cap_ >> 1);
		}

		for (size_t el_idx = idx; el_idx < sz_ - 1; ++el_idx) {
			arr_[el_idx] = arr_[el_idx + 1];
		}

		--sz_;
	}

	/* All A_functions:
	auxilary functions to decompose system work.*/
 private:
 	/* Allocates new buf
 	creates new buffer with 'n_els' elements in itself.*/
	Tp *A_AllocDfltBuf(size_t n_els) {
		Tp *buf = new Tp[n_els];

		for (size_t el_idx = 0; el_idx < n_els; ++el_idx) {
			buf[el_idx] = Tp();
		}

		return buf;
	}


	/* Deallocates buff and
	calls destructors for first buf_sz elements.*/
	void A_DeallocBuf(Tp *b, size_t buf_sz) {
		for (size_t el_idx = 0; el_idx < buf_sz; ++el_idx) {
			(b + el_idx)->~Tp();
		}

		delete[] b;
	}

	// Reallocates 'arr_' buffer, sets new capacity and size.
	void A_ReallocBuf(size_t new_cap) {
		Tp *new_arr = A_AllocDfltBuf(new_cap);

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

	Tp *arr_ = nullptr;
};

int SimpleCmp(int& a, int& b) {
	if (a > b) {
		return 1;
	}

	return (a == b)? 0 : -1;
}

/**
 * @brief Auxilary class to manage all the work
 * with memory in itself for DHeap.
 * 
 * @param Tp   type of contained objects.
 * @param dCap quantity of children in knot.
 * @param Cmp  comparator for type
 * 
 * Cmp should returns value in {-1, 0, 1}-format
 * @retval -1 less.
 * @retval 0  equal.
 * @retval 1  greater.
 */
template<
	typename Tp,
	size_t dCap,
	int (*Cmp)(Tp&, Tp&)
	>
class A_DHeapBase {
	static constexpr size_t kInitCap = 1;

 public:
	A_DHeapBase():
			arr_() {}

	~A_DHeapBase() = default;

	// Inserts new element in D-Heap.
	void Insert(Tp el) {
		if (arr_.sz_ == arr_.cap_) {
			arr_.A_ReallocBuf(arr_.cap_ << 1);
		}

		arr_.PushBack(el);
		A_SiftUp();
	}

	void Erase(Tp el) {
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

	// Binary search.
	int64_t Find(Tp el, size_t cur_idx = 0) {
		assert(cur_idx < arr_.cap_ &&
			"'cur_idx' beyound array's capacity!\n"
			"Or negative number passed in the function like 'size_t'\n");

		if (Gr(el, arr_.arr_[cur_idx])) {
			return -1;
		}

		if (Eq(el, arr_.arr_[cur_idx])) {
			return cur_idx;
		}

		int64_t result = -1;

		if ((cur_idx << 1) + 1 < arr_.sz_) {
			result = Find(el, (cur_idx << 1) + 1);
		}

		if ((cur_idx << 1) + 2 < arr_.sz_ && result == -1) {
			result = Find(el, (cur_idx << 1) + 2);
		}

		return result;
	}

 	/* All A_functions:
 	auxilary functions to decompose system work.*/
 private:
 	void A_SiftUp() {
 		int64_t cur_child  =  arr_.sz_  - 1;
 		int64_t cur_parent = (cur_child - 1) / dCap;

 		while (cur_child > 0 && arr_.arr_[cur_child] > arr_.arr_[cur_parent]) {
 			std::swap(arr_.arr_[cur_child], arr_.arr_[cur_parent]);

 			cur_child  =  cur_parent;
 			cur_parent = (cur_child - 1) / dCap;
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

 	// Returns true if a > b.
 	bool Gr(Tp& a, Tp& b) {
 		return Cmp(a, b) == 1;
 	}

 	// Returns true if a == b.
 	bool Eq(Tp& a, Tp& b) {
 		return Cmp(a, b) == 0;
 	}

 	// Returns true if a < b.
 	bool Ls(Tp& a, Tp& b) {
 		return Cmp(a, b) == -1;
 	}

 	// Make .dot graph from heap.
 	void Debug() {

 	}

 private:
 	A_ArrayBase<Tp> arr_{};
};

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	A_DHeapBase<int, 3, &SimpleCmp> d_heap;

	d_heap.Insert(111);
	d_heap.Insert(222);
	d_heap.Insert(333);
	d_heap.Insert(444);

	return 0;
}