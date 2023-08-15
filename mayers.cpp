#include <iostream>

class Obj {
public:
  Obj(const std::string& x) : field(x) {}
  Obj(std::string&& x) : field(std::move(x)) {}  // std::move нужен!!
private:
  std::string field;
}

int main()
{
	std::string s = "abc";
	// Obj 

	return 0;
}