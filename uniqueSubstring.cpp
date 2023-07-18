#include <iostream>

#include <string>
#include <vector>

#include <cassert>

static constexpr size_t kAlphabetCapacity = 256;
static constexpr char kMinChar            = 'a';

static inline __attribute__((always_inline))
int64_t Idx(const char c)
  { return c - kMinChar; }

static inline __attribute__((always_inline))
bool NewSymUnique(const char c, std::vector<uint64_t>& letters) {
  assert(static_cast<uint64_t>(Idx(c)) < letters.size() && "Beyound array's capacity!");
  return letters[Idx(c)] == 0;
}

static inline __attribute__((always_inline))
void updateChampion(
  uint64_t& cur_len,
  uint64_t& max_len,
  uint64_t  cur_left,
  uint64_t& max_left,
  uint64_t  cur_righ,
  uint64_t& max_righ) {
  if (cur_len > max_len) {
    max_len = cur_len;

    max_righ = cur_righ;
    max_left = cur_left;
  }
}

static inline __attribute__((always_inline))
void MoveLeftBorder(std::string& s, uint64_t& cur_left, uint64_t& cur_len) {
  while (s[cur_left] == s[cur_left + 1]) {
    ++cur_left;
    --cur_len;
  }

  ++cur_left;
  --cur_len;
}

std::pair<uint64_t, uint64_t> Algo(std::string& s, uint64_t max_k) {
  uint64_t left_cursor = 0;

  uint64_t cur_len = 1;
  uint64_t max_len = 0;
  uint64_t num_unique = 0;

  uint64_t max_left = 0;
  uint64_t max_righ = 0;

  std::vector<uint64_t> letters(kAlphabetCapacity, 0);

for (size_t sym_idx = 0; sym_idx < s.size(); ++sym_idx) {
  char cur_sym = s[sym_idx];

  if (NewSymUnique(cur_sym, letters)) {
    // ... add new sym logic.
    if (num_unique == max_k) {
      // uint64_t cur_righ = sym_idx - 1;
      updateChampion(cur_len, max_len, left_cursor, max_left, sym_idx - 1, max_righ);

      letters.at(Idx(s[left_cursor])) = 0;
      std::cout << "deleted\n";
      MoveLeftBorder(s, left_cursor, cur_len);
      --num_unique;
    } else {
      std::cout << "num unique:" << num_unique << '\n';
      ++num_unique;
      ++cur_len;

      ++letters.at(Idx(cur_sym));
    }

  } else {
    ++cur_len;
    std::cout << "curlen:" << cur_len << '\n';
  }
}

  uint64_t sz = static_cast<uint64_t>(s.size()) - 1;
  updateChampion(cur_len, max_len, left_cursor, max_left, sz, max_righ);

  return (max_righ == 0)? std::make_pair(static_cast<size_t>(0), s.size()) : std::make_pair(max_left, max_righ);
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::string s = "";
  uint64_t k    = 0;

  std::cin >> s >> k;

  std::pair<uint64_t, uint64_t> ans = Algo(s, k);

  std::cout << "The greatest string from: " << ans.first << " to: " << ans.second << '\n';

  return 0;
}