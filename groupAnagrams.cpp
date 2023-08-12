#include <iostream>

#include <vector>
#include <array>
#include <map>

using alph_t = std::array<int,26>;

std::vector<std::vector<std::string>> groupAnagrams(std::vector<std::string>& strs)
{ std::map<alph_t, std::vector<std::string>> map;

  for (size_t word_idx = 0; word_idx < strs.size(); ++word_idx) {
    
    alph_t chars{};
    // printf("default value of chars:(%d)\n", chars[word_idx]);

    for (size_t sym_idx = 0; sym_idx < strs[word_idx].size(); ++sym_idx) {
      ++chars[strs[word_idx][sym_idx] - 'a'];
    }

    for (size_t sym_idx = 0; sym_idx < 26; ++sym_idx) {
    	printf("%d ", chars[sym_idx]);
    }
    printf("\n");

    std::string s = strs[word_idx];
    auto it = map.find(chars);
    if (it != map.end()) {
    	(*it).second.push_back(s);
    }
  }

  return std::vector<std::vector<std::string>>();
}

int main()
{
  int n_words = 0;
  std::cin >> n_words;

  std::vector<std::string> strs;
  std::string cur_str;
  for (size_t word_idx = 0; word_idx < n_words; ++word_idx) {
  	std::cin >> cur_str;
  	strs.push_back(cur_str);
  }

  groupAnagrams(strs);

  return 0;
}