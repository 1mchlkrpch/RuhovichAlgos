/**
 * @file Contains
 * different hashes which sometimes used in c++ stdlib for example.
 * I found some hashes in bits..
 */
#include <iostream>
#include <limits>

#define inl_f inline __attribute((always_inline))

/*
Simple test for hash functions which is necessary to pass
to call function 'good'.

    1 test - "Chi-squared test".
Hash function should outputs results that are distributed
approximately like uniform distribution.

    2 test - "Avalanche test".
Hash function should outputs different values to nearly placed
values hashed that differ at approximately 50% of bytes.

GOOD NUMBERS:
a0 = 0x5bd1e995
c0 = 0x85ebca6b
c1 = 0xCC9E2D51
c2 = ox1B873593
m  = 0x5
n  = 0xE6546B64
hash = seed
*/

static constexpr uint32_t a0 = 0x5bd1e995;
static constexpr uint32_t c0 = 0x85ebca6b;
static constexpr uint32_t kHashConst1 = 0xcc9e2d51;
static constexpr uint32_t kHashConst2 = 0x1b873593;
// m  = 0x5
// n  = 0xE6546B64

inline std::size_t
unaligned_load(const char* p)
{ std::size_t result;
  __builtin_memcpy(&result, p, sizeof(result));
  return result;
}

inl_f uint32_t scramble(uint32_t x)
{ x *= kHashConst1;
  x  = (x << 15)
     | (x >> 17);
  return  x * kHashConst2;
}

size_t bytehash(const void* ptr, size_t sz, size_t seed = 2'166'136'261llu)
{
  const size_t m = 0x5bd1e995;
  auto ui8ptr = static_cast<const uint8_t*>(ptr);
  size_t hash = seed ^ sz;
  // Contains
  // random number from automatic memory.
  size_t random;

  for (size_t i = sz >> 2; i > 0; --i)
  { __builtin_memcpy(&random, ptr, sizeof(uint32_t));
    ui8ptr += sizeof(uint32_t);
    hash   ^= scramble(random);
    hash   = (hash << 13)
           | (hash >> 19);
    hash = hash * 5 + 0xe6546b64;
  }
  printf("?\n");

  enum { remain_3bytes = 3, remain_2bytes = 2, remain_1byte = 1 };
  switch (sz)
  { case remain_3bytes:
    { random = ui8ptr[2];
      hash ^= random << 16;
      __attribute__((fallthrough));
    }

    case remain_2bytes:
    { random = ui8ptr[1];
      hash ^= random << 8;
      __attribute__((fallthrough));
    }

    case remain_1byte:
    { random = ui8ptr[0];
      hash ^= random;
      hash *= m;
    }
  }

  return hash;
}

size_t _Hash_bytes(const void* ptr, size_t len, size_t seed)
{
  const size_t m = 0x5bd1e995;
  size_t hash = seed ^ len;
  const char* buf = static_cast<const char*>(ptr);
  // Mix 4 bytes at a time into the hash.
  while(len >= 4) {
    size_t k = unaligned_load(buf);
    k *= m;
    k ^= k >> 24;
    k *= m;
    hash *= m;
    hash ^= k;
    buf += 4;
    len -= 4;
  }
  size_t k;
  // Handle the last few bytes of the input array.
  switch(len) {
    case 3:
      k = static_cast<unsigned char>(buf[2]);
      hash ^= k << 16;
      [[gnu::fallthrough]];
    case 2:
      k = static_cast<unsigned char>(buf[1]);
      hash ^= k << 8;
      [[gnu::fallthrough]];
    case 1:
      k = static_cast<unsigned char>(buf[0]);
      hash ^= k;
      hash *= m;
  };
  // Do a few final mixes of the hash.
  hash ^= hash >> 13;
  hash *= m;
  hash ^= hash >> 15;
  return hash;
}

// Simple fnv hash function.
inl_f size_t fnv_hash_bytes(const void* ptr, size_t sz, size_t hash)
{ const char* cptr = static_cast<const char*>(ptr);
  for (; sz; --sz) {
    hash ^= static_cast<size_t>(*cptr++);
    hash *= static_cast<size_t>(16'777'619ull);
  }
  return hash;
}

struct fnv_impl
{ inl_f size_t
      hash(const void *p, size_t n_bytes, size_t seed = 2166136261llu)
    { return bytehash(p, n_bytes, seed); }

  template <typename Tp>
  inl_f size_t
      hash(const Tp& val)
    { return hash(&val, sizeof(val)); }
};

int main() {
  fnv_impl fnv;

  int arr1[] = {8,2,3,4};
  int arr2[] = {9,2,3,4};

  size_t hash1 = fnv.hash(arr1);
  size_t hash2 = fnv.hash(arr2);

  std::cout << std::hex << "hashes:\n" << hash1 << '\n' << hash2 << '\n';

  return 0;
}