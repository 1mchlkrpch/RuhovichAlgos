#include <iostream>

bool isPalindrome(int x) {
	if (x < 0) {
		return false;
	}
	
	int *nums = new int[10];
	
	int digit_idx = 0;
	while (x > 0) {
		nums[digit_idx] = x % 10;
		++digit_idx;
		x /= 10;
	}
	printf("digit_idx:(%d)\n", digit_idx);

	for (size_t el_idx = 0; el_idx < 10; ++el_idx) {
		printf("%d ", nums[el_idx]);
	}

	int half = digit_idx / 2;
	--digit_idx;

	for (int i = 0; i < half; ++i, --digit_idx) {
		// printf("nums:(%d|%d)\n", nums[i] != nums[digit_idx]);
		printf("nums:(%d|%d)\n", i, digit_idx);
		if (nums[i] != nums[digit_idx]) {
			return false;
		}
	}

	return true;
}

int main() {
	int num = 0;
	std::cin >> num;

	if (isPalindrome(num)) {
		printf("True\n");
	}

	return 0;
}