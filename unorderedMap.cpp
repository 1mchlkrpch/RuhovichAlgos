#include <iostream>

#include <unordered_map>
#include <string>

using    NameT = std::string;
using SurNameT = std::string;

typedef std::unordered_map<NameT, SurNameT> um_t;

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	um_t name_table;

	name_table.insert(std::make_pair("Ithan", "Hunt"));

	name_table.erase("Ithan");

	if (name_table.find("Ithan") == name_table.end()) {
		std::cout << "No agents with this name.\n";
	} else {
		std::cout << name_table["Ithan"] << '\n';
	}

	return 0;
}