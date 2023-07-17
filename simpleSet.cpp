#include <iostream>

#include <cassert>

#include <set>
#include <string>



int main() {
	std::set<std::string> names;

	std::string cur_name;
	std::cin >> cur_name;

	names.insert(cur_name);

	if (names.find("igor") != names.end()) {
		std::cout << "yes\n";
	}

	return 0;
}