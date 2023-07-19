#include <iostream>

#include <cstdint>

#include <cstddef>
#include <cassert>
#include <vector>

#include <fstream>

template<typename T, size_t cap, int (*Cmp)(T&, T&)>
class DHeap;

/** 
 * @brief Auxilary structure for BinaryHeap
 * to manage all the logic connected with arrays.
 *  
 * @param Tp  type of comprised data.
 */
template<typename Tp>
class A_ArrayBase {
	template<typename T, size_t cap, int (*C)(T&, T&)>
	friend class DHeap;

	// Size of comprised type.
	static constexpr size_t kTypeSize = sizeof(Tp);
	// Initial capacity of base of heap.
	static constexpr size_t kInitCap  = 1;

 public:
 	A_ArrayBase() noexcept
 		: sz_(0), cap_(kInitCap), arr_(A_AllocDfltBuf(kInitCap)) {}

 	A_ArrayBase(std::vector<Tp>& v)
 			: sz_(v.size()), cap_(v.size()), arr_(new Tp[v.size()]) {
 		
 		for (size_t el_idx = 0; el_idx < sz_; ++el_idx) {
 			arr_[el_idx] = v[el_idx];
 		}
 	}

 	size_t Cap() {
 		return cap_;
 	}

 	Tp at(size_t idx) {
 		return (idx < cap_)? arr_[idx] : Tp();
 	}

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
 * @brief Max in root D-Heap.
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
template<typename Tp, size_t dCap, int (*Cmp)(Tp&, Tp&)>
class DHeap {
	static constexpr size_t kInitCap = 1;

 public:
	DHeap():
			arr_() {}

	DHeap(std::vector<Tp> v):
			arr_(v) {
		Heapify();
	}

	~DHeap() = default;

	// Inserts new element in D-Heap.
	void Insert(Tp el) {
		if (Find(el) != -1) {
			return;
		}

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

		A_SiftDown(idx_of_el);
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

		for (size_t cur_child = 1; cur_child <= dCap; ++cur_child) {
			size_t idx_of_cur_child = (cur_idx * dCap) + cur_child;

			if (idx_of_cur_child >= arr_.sz_) {
				break;
			}
			
			result = Find(el, idx_of_cur_child);

			if (result != -1) {
				break;
			}
		}

		return result;
	}

	void ExtractFirst() {
		std::swap(arr_.arr_[0], arr_.arr_[arr_.sz_ - 1]);
		--arr_.sz_;

		A_SiftDown(0);
	}


	// O(n) procedure.
	void Heapify() {
		for (size_t el_idx = 0; el_idx < arr_.sz_; ++el_idx) {
			A_SiftUp(el_idx);
		}
	}

	// Make .dot graph from heap.
 	void Debug() {
 		std::ofstream f("graph.dot");

 		f << "digraph G {\n";

		for (size_t cur_v = 0; cur_v < arr_.sz_; ++cur_v) {
			for (size_t cur_child = 1; cur_child <= dCap; ++cur_child) {
				if (cur_v * dCap + cur_child < arr_.sz_) {
					f << '\t' << arr_.arr_[cur_v] << " -> " << arr_.arr_[cur_v * dCap + cur_child] << '\n';
				}
			}
		}

		f << "}\n";
		f.close();

		system("dot -Tpng graph.dot > graph.png");
 	}

 	A_ArrayBase<Tp>& GetArr() {
 		return arr_;
 	}

 	/* All A_functions:
 	auxilary functions to decompose system work.*/
 private:
 	// Sifts up element with index = 'idx'.
 	void A_SiftUp(int64_t idx = -1) {
 		int64_t cur_child = (idx == -1)? arr_.sz_ - 1 : idx;

 		int64_t cur_parent = (cur_child - 1) / dCap;

 		while (cur_child > 0 && arr_.arr_[cur_child] > arr_.arr_[cur_parent]) {
 			std::swap(arr_.arr_[cur_child], arr_.arr_[cur_parent]);

 			cur_child  =  cur_parent;
 			cur_parent = (cur_child - 1) / dCap;
 		}
 	}

 	// Sifts down element with index = 'idx'.
	void A_SiftDown(size_t idx) {
		size_t largest_child = 0;

		while (true) {
			largest_child = idx;

			for (size_t cur_child = 1; cur_child <= dCap; ++cur_child) {
				size_t idx_of_cur_child = (idx * dCap) + cur_child;
			
				if (idx_of_cur_child < arr_.sz_
							&& arr_.arr_[largest_child] < arr_.arr_[idx_of_cur_child]) {
					largest_child = idx_of_cur_child;
				}
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

 private:
 	A_ArrayBase<Tp> arr_{};
};

static constexpr size_t kCapacity = 17;

// int main() {
// 	std::ios_base::sync_with_stdio(false);
// 	std::cin.tie(nullptr);

// 	DHeap<int, 3, &SimpleCmp> d_heap;

// 	for (size_t el = 0; el < kCapacity; ++el) {
// 		int added_el = el * 3237 % kCapacity;

// 		d_heap.Insert(added_el);
// 	}

// 	d_heap.Heapify();

// 	d_heap.Print();

// 	std::cout << "idx:" << d_heap.Find(2) << '\n';

// 	d_heap.Debug();

// 	return 0;
// }


int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	std::vector<int> v;
	for (size_t i = 1; i < kCapacity; ++i) {
		v.push_back(i * 3237 % kCapacity);
	}

	DHeap<int, 3, &SimpleCmp> d_heap(v);

	for (size_t el = 0; el < kCapacity - 1; ++el) {
		d_heap.ExtractFirst();
	}

	A_ArrayBase<int> arr = d_heap.GetArr();
	for (size_t el = 0; el < arr.Cap(); ++el) {
		std::cout << arr.at(el) << '\n';
	}

	return 0;
}