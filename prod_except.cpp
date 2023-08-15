#include <iostream>
#include <vector>

auto init = [](){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    return 'c';
}();


std::vector<int> productExceptSelf(std::vector<int>& nums) {
    int64_t prod = 1;
    int64_t prod_with_zero = 1;
    uint32_t nzeros = 0;

    for (const auto& num : nums) {
        if (num == 0) {
            ++nzeros;
        } else {
            prod *= num;
        }
        prod_with_zero *= num;
    }

    std::vector<int> ans(nums.size(), 0);
    if (nzeros <= 1) {
        for (size_t el_idx = 0; el_idx < nums.size(); ++el_idx) {
            if (nums[el_idx] == 0) {
                ans[el_idx] = prod;
            } else {
                ans[el_idx] = prod_with_zero / nums[el_idx];
            }
        }
    }

    return ans;
}

int main()
{
	int n_nums = 0;
	std::cin >> n_nums;

	int cur_num = 0;
	std::vector<int> nums;
	for (int i = 0; i < n_nums; ++i) {
		std::cin >> cur_num;
		nums.push_back(cur_num);
	}

	// for (auto& num : nums) {
	// 	printf("num:(%d)\n", num);
	// }

	std::vector<int> ans = productExceptSelf(nums);

	for (auto& num : ans) {
		std::cout << num << ", ";
	}

	std::cout << '\n';
	return 0;
}