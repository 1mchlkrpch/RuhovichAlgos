#include <iostream>

int f(int a) {

	std::cout << "inside function\n";

	return a;
}

int f = 5;

int main() {

	std::cout << 4 + f
		(4 + 1); 
	
	std::cout << f + 1;

	return 0;
}