// Based (on comparison) sort with O(nlogn) asymptotic
#include <iostream>
#include <cassert>

#include <random>
#include <vector>

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

void CopyPart(
			std::vector<int64_t>& to,
			std::vector<int64_t>& from,
			size_t start_idx) {
	for (size_t idx = 0; idx < to.size(); ++idx) {
		to[idx] = from[start_idx + idx];
	}
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

void MergeSort(std::vector<int64_t>& arr) {
	if (arr.size() == 1) {
		return;
	}

	std::vector<int64_t> first_half(arr.size() / 2);
	CopyPart(first_half, arr, 0);
	std::vector<int64_t> secon_half((arr.size() + 1) / 2);
	CopyPart(secon_half, arr, arr.size() / 2);

	
		// Sort two halfs recursively.
	MergeSort(first_half);
	MergeSort(secon_half);

	
		// Merge to halfs via O(n)-function.
	Merge(first_half, secon_half, arr);
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
	MergeSort(arr);
	PrintArr(arr);

	return 0;
}
