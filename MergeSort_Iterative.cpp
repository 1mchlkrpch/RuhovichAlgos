#include <iostream>

#include <cassert>
#include <random>

#include <vector>
#include <queue>

static constexpr std::size_t kArrSize = 30;

int64_t Randv(int64_t low = 0, int64_t high = 100) {
	std::random_device rd;
	std::mt19937 gen(rd());
	
	std::uniform_int_distribution<> dist(low, high);
	return static_cast<int64_t>(dist(gen));
}

void PrintArr(std::vector<int64_t> arr) {
	for (size_t idx = 0; idx < arr.size() - 1; ++idx) {
		std::cout << "[" << idx << "]" << arr[idx] << "\n";
	}

	std::cout << "[" << arr.size() - 1 << "]" << arr[arr.size() - 1] << '\n';
}

void Merge(
			std::vector<int64_t>& a,
			std::vector<int64_t>& b,
			std::vector<int64_t>& res) {
	size_t cursor_a = 0;
	size_t cursor_b = 0;

	/*
			If in sorted arrays a,b fulfilled that
		for particular a_idx, b_idx:
				a[a_idx] < b[b_idx],
		
		we can conclude that:
				a[0, ... a_idx - 1] < b[b_idx]
		so we should placed these elements before b[b_idx]
		in res-array.
	*/
	while (cursor_a < a.size() && cursor_b < b.size()) {
		if (a[cursor_a] <= b[cursor_b]) {
			res[cursor_a + cursor_b] = a[cursor_a];
			++cursor_a;
		} else {
			res[cursor_a + cursor_b] = b[cursor_b];
			++cursor_b;
		}
	}

	/*
			Fulfilled that:
				a_idx = a.size() or b_idx = b.size()
		we add remnants of array x:
				x_idx < x.size()
		after all elements in res-array.
	*/
	while (cursor_a < a.size()) {
		res[cursor_a + cursor_b] = a[cursor_a];
		++cursor_a;
	}

	while (cursor_b < b.size()) {
		res[cursor_a + cursor_b] = b[cursor_b];
		++cursor_b;
	}
}

void CopyPart(
			std::vector<int64_t>& to,
			std::vector<int64_t>& from,
			size_t start_idx,
			size_t n_elements_to_copy) {
	for (size_t idx = 0; idx < n_elements_to_copy; ++idx) {
		to[idx] = from[start_idx + idx];
	}
}

/*
		If we dont have enough memory in pc-stack
	should place all data in heap, we need stackless algorithm.
*/
void MergeSort_Iterative(
			std::vector<int64_t>& arr) {
	std::queue<std::vector<int64_t>> q;

	for (size_t el_idx = 0; el_idx < arr.size(); ++el_idx) {
		q.push({arr[el_idx]});
	}

	/*
			Push parts of len 2^k,
		merge them and push into queue
		in cycle with 'start_idx'-index we pop all elements with length 2^k
		and push all new elements with length 2^{k + 1}.
	*/
	for (size_t chank_len = 1; chank_len < arr.size(); chank_len *= 2) {
		size_t secon_chank_len = 0;

		size_t n_chanks = arr.size() / chank_len;
		n_chanks += (arr.size() % chank_len == 0)? 0 : 1;

		for (size_t start_idx = 0; start_idx < arr.size() - chank_len; start_idx += 2 * chank_len) {
			std::vector<int64_t> a;
			std::vector<int64_t> b;

			if (!q.empty()) {			
				a = q.front();
				q.pop();
			}

			if (!q.empty()) {
				b = q.front();
				q.pop();
			}

			std::vector<int64_t> cur_res(a.size() + b.size());

			Merge(a, b, cur_res);
			q.push(cur_res);
		}
	}

	
		// After all iterations 'q' contains one element with len arr.size().
	std::cout << "size:" << q.size() << '\n';
	arr = q.front();
	q.pop();
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	std::vector<int64_t> arr(kArrSize);
	
	for (size_t idx = 0; idx < kArrSize; ++idx) {
		arr.at(idx) = Randv(100, 1000);
	}

	PrintArr(arr);
	std::cout << "===============" << '\n';
	MergeSort_Iterative(arr);
	PrintArr(arr);

	return 0;
}
