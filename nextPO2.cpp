#include <iostream>

#define inl_f_ static inline __attribute__((__always_inline__))

auto init = []() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);

	return 'c';
};

inl_f_ void debug_bin(uint32_t x)
{
	int *digits = new int[32]();

	int i = 0;
	while (x > 0) {
		digits[32 - i] = x % 2;
		x /= 2;
		++i;
	}
	--i;

	for (; i >= 0; --i) {
		printf("%d", digits[32 - i]);
	}
	printf("\n");

	delete[] digits;
}

inl_f_ uint32_t int32nextPO2(uint32_t x)
{
	--x;
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;

	return x + 1;
}

int main() {
	int num = 17;
	// std::cin >> num;

	std::cout << int32nextPO2(num) << '\n';

	return 0;
}