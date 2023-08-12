#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <cstdio>

std::vector<std::vector<int>> threeSum(std::vector<int>& nums) {
    std::vector<std::vector<int>> res;

    std::sort(nums.begin(), nums.end());

    for (int i = 0; i < (int)nums.size() - 2; ++i) {
        if (i && nums[i] == nums[i - 1]) {
            continue;
        }

        int j = i + 1;
        int k = (int)nums.size() - 1;

        while (j < k) {
            int sum = nums[i] + nums[j] + nums[k];

            if (sum == 0) {
                res.push_back(std::vector<int>({nums[i], nums[j], nums[k]}));

                while (j < k && nums[j] == nums[j + 1]) {
                    ++j;
                }

                while (j < k && nums[k] == nums[k - 1]) {
                    --k;
                }

                ++k;
                --k;
            } if (sum > 0) {
                --k;
            } else if (sum < 0) {
                ++j;
            }
        }
    }

    return res;
}

int main()
{
	size_t n_nums = 0;
	std::cin >> n_nums;

	std::vector<int> nums;

	int num = 0;
	for (size_t el_idx = 0; el_idx < n_nums; ++el_idx) {
		std::cin >> num;

		nums.push_back(num);
	}

	std::vector<std::vector<int>> ans = threeSum(nums);
	for (size_t el_idx = 0; el_idx < ans.size(); ++el_idx) {
		printf("[%d, %d, %d]",
			ans[el_idx][0],
			ans[el_idx][1],
			ans[el_idx][2]);
	}

	return 0;
}