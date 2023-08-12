#include <iostream>
#include <vector>
#include <map>
#include <cstring>
#include <limits>

// std::vector<int> twoSum(std::vector<int>& nums, int target)
// { std::map<int,int> um;
//   for (int el_idx = 0; el_idx < nums.size(); ++el_idx) {
//       // printf("cur check:(%d)\n", target - nums[el_idx]);

//       // printf("in that place:(%d)\n", um[target - nums[el_idx]]);

//       if (um.find(target - nums[el_idx]) != um.end()) {
//         return std::vector<int>({um[target - nums[el_idx]], el_idx});
//       }

//       um[nums[el_idx]] = el_idx;
//       printf("pushed:(%d)\n\n", um[nums[el_idx]]);
//   }

//   return std::vector<int>({0,0});
// }

#include <cstdlib>

static constexpr int kNone = 10'001u;

#define inl_f static inline __attribute__((always_inline))
inl_f uint32_t simpleHash(uint32_t x)
{
  x = ((x >> 16) ^ x) * 0x45f3b;
  x = ((x >> 16) ^ x) * 0x45f3b;
  x = (x >> 16)  ^ x;
  return x;
}

inl_f uint32_t nextPO2(uint32_t x)
{ --x;
  x |= x >>  1;
  x |= x >> (1 << 1);
  x |= x >> (1 << 2);
  x |= x >> (1 << 3);
  return x + 1;
}

std::vector<int> twoSum(std::vector<int>& nums, int target)
{
  uint32_t sz = nextPO2(nums.size());
  int *hash_table = (int*)calloc(sz + 1, sizeof(int));

  for (size_t i = 0; i < sz; ++i) {
    hash_table[i] = kNone;
  }
  int* __restrict end = &hash_table[sz];
  
  for (uint32_t num_idx = 0; num_idx < static_cast<uint32_t>(nums.size()); ++num_idx) {
    int* __restrict try_pos = &hash_table[simpleHash(target - nums[num_idx]) & (sz - 1)];

    while (true) {
      printf("hash:(%u)\n", simpleHash(target - nums[num_idx]) & (sz - 1));
      if ((*try_pos < kNone && nums[*try_pos] + nums[num_idx] == target) || (*try_pos == kNone)) {
        break;
      }

      if (++try_pos == end) {
        try_pos = hash_table;
      }
    }

    if (*try_pos != kNone && nums[*try_pos] + nums[num_idx] == target) {
      printf("?\n");
      return std::vector<int>({(int)*try_pos, (int)num_idx});
    } else {
      printf("on:(%u):%u\n", simpleHash(nums[num_idx]) & (sz - 1), num_idx);
      hash_table[simpleHash(nums[num_idx]) & (sz - 1)] = num_idx;
    }
  }
  
  return std::vector<int>({0,0});
}

int main()
{ int n_nums = 0;
  std::cin >> n_nums;

  std::vector<int> nums;
  int num = 0;
  for (size_t i = 0; i < n_nums; ++i) {
    std::cin >> num;
    nums.push_back(num);
  }

  // for (size_t i = 0; i < nums.size(); ++i) {
  //   printf("%d ", nums[i]);
  // }
  // printf("\n");

  int target = 0;
  std::cin >> target;

  std::vector<int> ans = twoSum(nums, target);
  std::cout << ans[0] << ' ' << ans[1] << '\n';

  return 0;
}