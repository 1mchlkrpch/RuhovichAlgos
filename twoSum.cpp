#include <iostream>

#include <vector>
#include <map>

// Input: nums = [2,7,11,15], target = 9
// Output: [0,1]
// Explanation: Because nums[0] + nums[1] == 9, we return [0, 1].

std::vector<int> TwoSum(std::vector<int>& nums, int target)
{
	std::map<int, size_t> map;
    for (size_t el_idx = 0; el_idx < nums.size(); ++el_idx) {
	    if (map.find(target - nums[el_idx]) != map.end()) {
		    return std::vector<int>({static_cast<int>(map[target - nums[el_idx]]), static_cast<int>(el_idx)});
	    }

	    map.insert(std::make_pair(nums[el_idx], el_idx));
    }

    return std::vector<int>();
}


int main()
{
	std::vector<int> nums;
	size_t n_nums = 0;

	int target = 0;

	std::cin >> n_nums;
	std::cin >> target;

	int c_num = 0;
	for (size_t el_idx = 0; el_idx < n_nums; ++el_idx) {
		std::cin >> c_num;
		nums.push_back(c_num);
	}

	std::vector<int>ans = TwoSum(nums, target);

	std::cout << ans[0] << ' ' << ans[1] << '\n';

	return 0;
}