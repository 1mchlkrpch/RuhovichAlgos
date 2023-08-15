#include <iostream>
#include <fstream>

static constexpr size_t kDefaullSeed = 2'166'136'261llu;
static constexpr size_t kHashConst1 = 0xcc9e2d51;
static constexpr size_t kHashConst2 = 0x1b873593;
static constexpr size_t kAddConst = 0xe6546b64;
static constexpr size_t combine_shifts[] = {15, 17};
static constexpr size_t hash_shifts[]    = {19, 13};
static constexpr size_t m_constant = 0x5bd1e995;

auto init = []()
{ std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::ofstream out("user.out");
  for (std::string s; getline(std::cin, s); out << '\n') {
    if (s.length() == 2) {
      out << 0;
      continue;
    }

    size_t kSize = 1 << 17;

    int hash_table[kSize] = {0};
    int array_of_nums[100000];
    int nums[100000];
    int n = 0;
    for (int sym_idx = 1; sym_idx < s.length(); ++sym_idx) {
      bool is_neg = false;
      if (s[sym_idx] == '-') {
         ++sym_idx, is_neg = true;
      }
      int cur_number = s[sym_idx++] & 15;
      while ((s[sym_idx] & 15) < 10) {
         cur_number = cur_number * 10 + (s[sym_idx++] & 15);
      }
      if (is_neg) {
        cur_number = -cur_number;
      }

      // printf("num:(%d) ", cur_number);

      auto ui8ptr = (uint8_t*)(&cur_number);
      size_t sz = 4;
      size_t hash = kDefaullSeed ^ sz;
      size_t save_sz = sz;
      hash *= (kHashConst1 ^ ui8ptr[3]);
      hash = (hash << combine_shifts[0]) | (hash >> combine_shifts[1]); 
      hash *= kHashConst2;
      --sz;
      enum {remain3_bytes = 3, remain2_bytes = 2, remain_byte = 1};
      switch (sz) {
        case remain3_bytes:
          hash ^= ui8ptr[2] << 16;
          [[fallthrough]];
        case remain2_bytes:
          hash ^= ui8ptr[1] << 8;
          [[fallthrough]];
        case remain_byte:
          hash ^= ui8ptr[0];
          hash *= m_constant;
      }
      ++cur_number;
      auto ui8ptr2 = (uint8_t*)(&cur_number);
      size_t sz2 = 4;
      size_t hash2 = kDefaullSeed ^ sz2;
      size_t save_sz2 = sz2;
      hash2 *= (kHashConst1 ^ ui8ptr2[3]);
      hash2 = (hash2 << combine_shifts[0]) | (hash2 >> combine_shifts[1]); 
      hash2 *= kHashConst2;
      --sz2;
      switch (sz2) {
        case remain3_bytes:
          hash2 ^= ui8ptr2[2] << 16;
          [[fallthrough]];
        case remain2_bytes:
          hash2 ^= ui8ptr2[1] << 8;
          [[fallthrough]];
        case remain_byte:
          hash2 ^= ui8ptr2[0];
          hash2 *= m_constant;
      }
      printf("num:(%d)\n", cur_number - 1);

      // std::cout << "hash:" << std::hex << (hash & 65'535) << '\n';
      hash_table[hash & (kSize - 1)] = 1;
      array_of_nums[n * 2] = (hash & (kSize - 1));
      array_of_nums[n * 2 + 1] = (hash2 & (kSize - 1));
      nums[n] = cur_number - 1;
      ++n;
    }
    for (size_t i = 0; i < 16; ++i) {
      printf("hash of cur:(%x), hash of next(%x)\n", array_of_nums[i * 2], array_of_nums[i * 2 + 1]);
    }
    int ans = 0;
    int i = 0;
    for (; i < n;) {
        int i0 = i;
        for (++i; i < n; ++i) {
          if (hash_table[array_of_nums[i * 2]] == 1 && hash_table[array_of_nums[i * 2 + 1]] > 1) {
            ans = std::max(ans, i - i0 + hash_table[array_of_nums[i * 2 + 1]]);
            break;
          }

          if (!(hash_table[array_of_nums[i * 2]] == 1 && hash_table[array_of_nums[i * 2 + 1]] == 1)) {
            break;
          }
          // printf("check hash:\n%x and\n%x\n", array_of_nums[i * 2], array_of_nums[i * 2 + 1]);
          printf("num:%d - in table\n", nums[i]);
          printf("c:%d\n", i - i0);
        }
        printf("w\n");
        // printf("i:%d|i0:%d\n", i, i0);
        ans = std::max(ans, i - i0);
        hash_table[array_of_nums[i0 * 2]] = ans;
        printf("we place score at[%d]:%d - in table\n", i0, hash_table[array_of_nums[i0 * 2]]);
    }
    out << ans;
    printf("ans:(%d)\n", ans);
  }
  out.flush();
  exit(0);
  return 'c';
}();

int main() { return 999; }