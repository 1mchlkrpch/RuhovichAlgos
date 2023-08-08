// Except other files in this folder
#include <iostream>
#include <vector>
#include <limits>

int main()
{
	int var = 1;

	// Element in vector:
	std::vector<std::string> nums = {"123", "12", "1", "4"};
	std::string *ptr_to_el = &nums[0];

	ptr_to_el += 2;
	std::cout << *ptr_to_el << " szof:" << sizeof(std::string) << '\n';

	std::ptrdiff_t ptr = -1;
	std::cout << "ptr:" << ptr << '\n';
	std::cout << "max ptrdiff_t:" << PTRDIFF_MAX << '\n';
	std::cout << "max uint64_t:" << std::numeric_limits<uint64_t>::max() << '\n';

	// Memory reuse
	int *ptr = nullptr;
	{
		int local_variable = 1;
		ptr = &local_variable;

		int second_local_variable = 1;
		ptr = &local_variable;
	}

	// for (int iter = 1; iter < 300; ++iter) {
	// 	var *= iter;
	// 	std::cout << var * 10000000000 << std::endl;
	// }

	return 0;
}