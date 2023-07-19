/* Дана строка s и число k
в строке s требуется найти подстроку максимальной длины,
в которой все различные символы встречатся не менее k раз.

Examples:
in
5 3
aaabb

out
3
*/

#include <iostream>

#include <string>
#include <vector>

#include <cstring>

static constexpr size_t kAlphabetCapacity = 26;
static constexpr char kFirstSymInAlphabet = 'a';

/* Calculates index in array or vector of particular symbol
in the alphabet (for small continuous alphabet).*/
static inline __attribute__((always_inline))
uint64_t Ind(const char c)
{ return c - kFirstSymInAlphabet; }

// Calculates number of unique symbols in string 's'.
uint64_t CalcUniqueSymbols(std::string& s, std::vector<uint64_t>& cnt)
{
	uint64_t n_unique = 0;

	for (size_t sym_idx = 0; sym_idx < s.size(); ++sym_idx) {
		if (cnt[Ind(s[sym_idx])] == 0) {
			++n_unique;
		}

		++cnt[Ind(s[sym_idx])];
	}

	return n_unique;
}

/* Tries to search longest suitable substring with

  1 unique symbols in itself,
  2 unique symbols in itself,
  ...
  n-unique symbols in itself

where n - number of unique symbols in the whole 's'-string.

WHEN WE CAN MEET SUITABLE SUBSTRING?
when we move right cursor at right untill we meet k + 1 unique symbol
	so we will check every time we meet new symbol in 's'-string
	if the quantity of every unique symbol (maximum k unique symbols) we met at_least_number-times

	if we met k + 1-new unuque symbol we will move left border until the the amount
	of unique symbols decreases.

Time-complexity: O(n), but actually O(CalcUniqueSymbols(s, cnt) * n)
memory-complexity: O(1) with small alphabet
but we can use unordered map if we work with big not continuous alphabet*/
uint64_t kEachSymInSubstring(std::string& s, uint64_t at_least_number)
{
	std::vector<uint64_t> cnt(kAlphabetCapacity, 0);

	uint64_t N = CalcUniqueSymbols(s, cnt);
	uint64_t ans = 0;

	// Uses 'sliding window' technique.
	for (uint64_t K = 1; K <= N; ++K) {
		std::fill(cnt.begin(), cnt.end(), 0);

		size_t left_cursor = 0;
		size_t righ_cursor = 0;

		/* Number of unique characters in substring with
		maximum 'K'-unique symbols at particular symbol in 's'.*/
		uint64_t cur_unique = 0;
		uint64_t nSymsOccuringAtLeastKTimes = 0;

		while (righ_cursor < s.size()) {
			uint64_t idx = 0;
			if (cur_unique <= K) {
				idx = Ind(s[righ_cursor]);
			} else {
				idx = Ind(s[left_cursor]);
			}

			uint64_t delta = (cur_unique <= K)? 1 : -1;

			if (cnt[idx] == 0) {
				cur_unique += delta;
			}
			
			cnt[idx] += delta;

			if (cnt[idx] == at_least_number) {
				nSymsOccuringAtLeastKTimes += delta;
			}

			/* If we met k+1-new unuque symbol we will move left border in while
			until we have k+1 unique symbol in current substring.*/ 
			if (cur_unique <= K) {
				++righ_cursor;
			} else {
				++left_cursor;
			}

			// Update champion.
			if (cur_unique == K && nSymsOccuringAtLeastKTimes == K) {
				ans = std::max(ans, righ_cursor - left_cursor);
			}
		}
	}

	return ans;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	uint64_t k = 0;
	uint64_t n = 0;
	std::string s;
	std::cin >> n >> k >> s;

	s.resize(n);

	std::cout << kEachSymInSubstring(s, k) << std::endl;

	return 0;
}