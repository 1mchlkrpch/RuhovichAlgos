/**
 * @file
 * Blum, Floyd, Pratt, Rivest, Tarjan's algorithm
 * 
 * Find's k-th statistics ~ O(n).
 */

#include <iostream>

#include <cassert>
#include <random>

#include <vector>

static constexpr std::size_t kArrSize = 30;

static constexpr std::size_t kChankLen = 5;
static constexpr std::size_t kStatisticOfMedianInChank = kChankLen / 2;

int64_t Randv(int64_t low = 0, int64_t high = 100) {
	std::random_device rd;
	std::mt19937 gen(rd());
	
	std::uniform_int_distribution<> dist(low, high);
	return static_cast<int64_t>(dist(gen));
}

void PrintArr(std::vector<int64_t> arr) {
	if (arr.size()) {
		for (size_t idx = 0; idx < arr.size() - 1; ++idx) {
			std::cout << "[" << idx << "]" << arr[idx] << "\n";
		}

		std::cout << "[" << arr.size() - 1 << "]" << arr[arr.size() - 1] << '\n';
	}
}

size_t Partition(std::vector<int64_t>& arr, size_t from, size_t to, int64_t pivot) {
	int left_boundary  = from;
	int right_boundary = to;

	while (left_boundary <= right_boundary) {
		while (arr[left_boundary] < pivot) {
			++left_boundary;
		}

		while (arr[right_boundary] > pivot) {
			--right_boundary;
		}

		if (left_boundary >= right_boundary) {
			break;
		}

		std::swap(arr[left_boundary], arr[right_boundary]);
		++left_boundary;
		--right_boundary;
	}

	return right_boundary;
}

int64_t Qselect(std::vector<int64_t>& arr, size_t from, size_t to, size_t k) {
	assert(from <= to && "wrong boundary indeces!");

	if (from < to) {
		size_t pos = Partition(arr, from, to, arr[(from + to) / 2]);

		if (pos > k + from) {
			return Qselect(arr, from, pos, k);
		} else if (pos == k + from) {
			return arr[pos];
		}

		return Qselect(arr, pos, to, k);
	}

	return arr[from];
}

int64_t BFPRT_algorithm(std::vector<int64_t>& arr, size_t from, size_t to, size_t k) {
	if (arr.size() < k) {
		return std::numeric_limits<int64_t>::max();
	}

	if (from < to && (to - from) > kChankLen * 2) {
		std::vector<int64_t> medians;

		for (size_t idx = 0; idx < to; idx += kChankLen) {
			if (idx + kChankLen < to) {
				medians.push_back(Qselect(arr, idx, idx + kChankLen, kStatisticOfMedianInChank));
			}
			// else {
			// 	medians.push_back(Qselect(arr, idx, to, (arr.size() - idx) / 2));
			// }
		}

		// PrintArr(medians);

			// Find median of medians & make partition with it.
		// size_t pos = Partition(arr, 0, arr.size(), Qselect(medians, 0, medians.size(), arr.size() / 10));

		// if (pos > k) {
		// 	BFPRT_algorithm(arr, from, pos, k);
		// } else if (pos == k) {
		// 	return arr[pos];
		// }

		// return BFPRT_algorithm(arr, pos, to, k);
	} else if (from < to && (to - from) <= kChankLen * 2) {
		return Qselect(arr, from, to, k);
	}

	return std::numeric_limits<int64_t>::max();
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	std::vector<int64_t> arr(kArrSize);
	
	for (size_t idx = 0; idx < kArrSize; ++idx) {
		arr.at(idx) = Randv(100, 1000);
	}

	std::vector<int64_t> arr2(arr);

	PrintArr(arr);
	std::cout << BFPRT_algorithm(arr,  0, arr.size() - 1, 10) << '\n';
	// std::cout <<         Qselect(arr, 0, arr.size() - 1, 10) << '\n';

	return 0;
}