#include <iostream>

#include <cassert>
#include <random>

#include <vector>

static constexpr std::size_t kArrSize = 30;

	// To get ~~random element from arr.
static constexpr std::size_t kKey = 38379;

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

void Qsort(std::vector<int64_t>& arr, size_t from, size_t to) {
	/*
			Split all elements on two groups:
		first:  x \in array <= pivot,
		second: x \in array > pivot
	*/
	if (from < to) {
		int64_t pivot = arr[(from + to) / 2];

		size_t pos = Partition(arr, from, to, pivot);

		Qsort(arr, from, pos);
		Qsort(arr, pos + 1, to);
	}
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	std::vector<int64_t> arr(kArrSize);
	
	for (size_t idx = 0; idx < kArrSize; ++idx) {
		arr.at(idx) = Randv(100, 1000);
	}

	PrintArr(arr);
	Qsort(arr, 0, arr.size());
	std::cout << "===============" << '\n';
	PrintArr(arr);

	return 0;
}