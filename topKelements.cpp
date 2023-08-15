#include <iostream>

#include <vector>
#include <unordered_map>

std::vector<int> topKFrequent(std::vector<int>& nums, int k) {
    std::unordered_map<int,int> um;
    for (auto& num : nums) {
        ++um[num];
    }

    std::vector<std::vector<int>> frequency(nums.size() + 1);
    for (size_t num_idx = 0; num_idx < nums.size(); ++num_idx) {
        if (um.find(nums[num_idx]) != um.end()) {
            frequency[um[nums[num_idx]]].emplace_back(nums[num_idx]);
        }
    }

    std::vector<int> ans;
    for (int ans_idx = (int)frequency.size() - 1; ans_idx >= 0; --ans_idx) {
    	if (k == 0) {
    		break;
    	}

        if (frequency[ans_idx].size() != 0) {
        	size_t i = 0;
        	while (k > 0 && i < frequency[ans_idx].size()) {
            	ans.push_back(frequency[ans_idx][i]);
            	--k;
            	++i;
        	}
        }
    }

    return ans;
}

int main() {
	int n_nums = 0;
	std::cin >> n_nums;

	std::vector<int> nums;
	int num;
	for (int cur_n = 0; cur_n < n_nums; ++cur_n) {
		std::cin >> num;
		nums.push_back(num);
	}

	int k = 0;
	std::cin >> k;

	std::vector<int> ans = topKFrequent(nums, k);
	for (size_t el_idx = 0; el_idx < ans.size(); ++el_idx) {
		std::cout << ans[el_idx] << ' ';
	}
	std::cout << '\n';

	return 0;
}