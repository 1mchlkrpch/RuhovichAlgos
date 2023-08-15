#include <iostream>

#include <vector>

std::vector<int> twoSum(std::vector<int>& nums, int target) {
    size_t l = nums.size() - 1;
    size_t r = 0;

    int n_max_loops = 16;
    while (nums[pivot] + nums[idx] != target && n_max_loops > 0) {
        size_t m = (l + r) / 2;

        if (nums[l] + nums[m] > )

        --n_max_loops;
        rangesz /= 2;
    }

    printf("id1(%d) id2(%d)\n", pivot, idx);

    return std::vector<int>();
}

int main() {
	int nnums = 0;
	int target = 0;
	std::cin >> nnums >> target;

	int num = 0;
	std::vector<int> nums;
	for (size_t i = 0; i < nnums; ++i) {
		std::cin >> num;
		nums.emplace_back(num);
	}

	// printf("pivot init:(%d)\n", nums.size());

	std::vector<int> ans = twoSum(nums, target);
	for (auto& num : ans) {
		printf("num:(%d)\n", num);
	}

	return 0;
}