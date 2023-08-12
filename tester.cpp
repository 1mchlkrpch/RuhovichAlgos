#include <iostream>

#include <chrono>
// #include <chrono_io>

namespace lib
{
class timer
{
	using time_t = std::chrono::_V2::steady_clock::time_point;
	using ms_t = std::chrono::milliseconds;
 public:
	explicit timer()
	{ std::cout << "timer start\n"; }

	auto get_time()
	{
		// printf("?%zu", std::chrono::duration_cast<ms_t>(start_ - std::chrono::steady_clock::now()).count());
		return std::chrono::duration_cast<ms_t>(std::chrono::steady_clock::now() - start_).count();
	}

	void record_now()
	{
		std::cout
			<< std::chrono::duration_cast<ms_t>
				(start_ - std::chrono::steady_clock::now()).count()
			<< "ms passed\n";
	}
 private:
	time_t start_{std::chrono::steady_clock::now()};
};

class average_time_tester
{
	using time_t = std::chrono::_V2::steady_clock::time_point;
public:
	average_time_tester(int k_times, std::string&f, std::string& s,  void (*lambda)(std::string&f, std::string& s)): tm(), k_times_execute(k_times)
	{
		for (size_t el_idx = 0; el_idx < k_times_execute; ++el_idx) {
			lambda(f, s);
		}

		std::cout << "AVERAGE TIME SPEND: " << tm.get_time() /*/ k_times_execute*/ << "ms\n";
	}
private:
	timer tm{};
	size_t k_times_execute{100};
};

}; // end of lib

void solution()
{
	uint64_t a = 0;
	for (size_t i = 0; i < 1'000'0000; ++i) {
		a *= i;
	}
}

bool isAnagram(std::string& s, std::string& t) {
    if(s.size() != t.size())
        return false;
    
    int alph[26] = {0};

    for(size_t i = 0; i < s.size(); i++){
        alph[s[i]-'a']++;
        alph[t[i]-'a']--;
    }

    for(int i=0; i<26; i++){
        if(alph[i] != 0){
            return false;
        }
    }
    return true;
}

bool isAnagram2(std::string s, std::string t) {
    if (s.size() != t.size()) {
        return false;
    }

    uint16_t chars[26] = {0};

    for (const char c : s) {
        ++chars[c - 'a'];
    }

    for (const char c : t) {
        if (chars[c - 'a'] == 0) {
            return false;
        }
        --chars[c - 'a'];
    }

    return true;
}

#include <ranges>

bool isAnagram3(std::string& s, std::string& t) {
    if (s.size() != t.size()) {
        return false;
    }

    uint16_t chars[26] = {0};

    // for (size_t char_idx = 0; char_idx < s.size(); ++char_idx) {
    //     ++chars[s[char_idx] - 'a'];
    //     --chars[t[char_idx] - 'a'];
    // }

    using it_t = std::string::iterator;
    // it_t sit = s.begin();
    // it_t tit = t.begin();
    // for (it_t sit = s.begin() ;sit != s.end() && tit < t.end(); ++sit, ++tit) {
    //     ++chars[*sit - 'a'];
    //     --chars[*tit - 'a'];
    // }

    for (auto& [a, b] : std::zip(s, t)) {
        ++chars[a - 'a'];
        --chars[b - 'a'];
    }

    for (uint16_t& el : chars) {
        if (el != 0) {
            return false;
        }
    }

    return true;
}

int main() {

	std::string f = "";
	std::string s = "";
	std::cin >> f >> s;

	lib::average_time_tester tester(1000, f, s, [](std::string& f, std::string& s){
		isAnagram3(f, s);
	});
	return 0;
}