#include <iostream>

#include <map>
#include <string>
// #include <pair>

int main() {
	std::map<std::string, std::pair<std::string, std::string>> agents;

	std::string gg = "boomer";

	agents[gg] = std::make_pair("Ithan", "Hunt");

	agents.insert(std::make_pair("Lark", std::make_pair("Genry", "Cavil")));

	agents.erase("Lark");

	// std::cout << agents["Lark"].first << agents[gg].second << '\n';

	if (agents.find("Lark") != agents.end()) {
		std::cout << "Lark in map\n";
	}

	return 0;
}