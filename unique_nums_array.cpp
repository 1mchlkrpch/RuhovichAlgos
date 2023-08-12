#include <iostream>
#include <vector>
#include <bitset>

auto init = [](){
	std::ios::sync_with_stdio(false);

	std::cin.tie(0);
	std::cout.tie(0);
	
	return 'c';
}();

unsigned hash(unsigned a) {
	a = (a + 0x7ed55d16) + (a << 12);
	a = (a ^ 0xc761c23c) ^ (a >> 19);
	a = (a + 0x165667b1) + (a << 5);
	a = (a + 0xd3a2646c) ^ (a << 9);
	a = (a + 0xfd7046c5) + (a << 3);
	a = (a ^ 0xb55a4f09) ^ (a >> 16);

	return a;
}

unsigned int nextPO2(unsigned int v)
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;

	return v + 1;
}

bool containsDuplicate(std::vector<int>& nums) {
	// Give neares power of two to nums.size().
	const int hashPO2Size = nextPO2(nums.size());

	enum { eEmpty = 0 };
	int hshtbl[131072] = {};

	// Restricted pointer to last cell in hash table.
	const int* __restrict end = &hshtbl[hashPO2Size];

	// printf("hshtbl:%d\n", hshtbl[2]);

	// Running out the words.
	for (const int& num : nums) {
		// replace num with non zero value.
		int j = (num)? num : (uint32_t)0 - 1; // 0 is reserved

		// Restrict size by setting first log_2(hashPO2Size) position of address hash(num).
		int* __restrict probe = &hshtbl[hash(num) & (hashPO2Size - 1)];
		
		while (*probe != j && *probe != eEmpty) {
			if (++probe == end) {
				probe = hshtbl;
			}
		}

		if (*probe != 0) {
			return true;
		} else {
			*probe = j;
		}
	}

	return false;
}

int main()
{
	// int a = 10;
	// int hshtbl[131072] = {};

	// std::cout << sizeof(hshtbl) << '\n';
	// std::cout << hash(a) << '\n';

	int n_nums = 0;
	std::cin >> n_nums;
	std::vector<int> nums;
	int cur_num = 0;
	for (int el_idx = 0; el_idx < n_nums; ++el_idx) {
		std::cin >> cur_num;
		nums.push_back(cur_num);
	}
	std::cout << containsDuplicate(nums) << '\n';

	// std::bitset<1 << 24> set;
}