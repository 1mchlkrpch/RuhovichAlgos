#include <iostream>
// Default
// inline function keywords for compiler.
#define inl_f inline __attribute__((always_inline))

// Some constants for good hashing:
static constexpr size_t kDefaullSeed = 2'166'136'261llu;

static constexpr size_t kHashConst1 = 0xcc9e2d51;
static constexpr size_t kHashConst2 = 0x1b873593;

static constexpr size_t kAddConst = 0xe6546b64;

static constexpr size_t combine_shifts[] = {15, 17};
static constexpr size_t hash_shifts[]    = {19, 13};

static constexpr size_t m_constant = 0x5bd1e995;


inl_f size_t scramble(size_t x)
{ x *= kHashConst1;
  x  = x << combine_shifts[0] | x >> combine_shifts[1];
  return x * kHashConst2;
}

// That is default murmurhash3 hash function.
inl_f size_t hash_bytes(const void *ptr, size_t sz, size_t seed)
{ size_t hash = seed ^ sz;
  size_t random;
  auto ui8ptr = static_cast<const uint8_t*>(ptr);
  while (sz > 0) {
  	hash   ^= scramble(random);
  	hash    = hash << hash_shifts[0] | hash >> hash_shifts[1];
  	hash    = hash * 5 + kAddConst;
  	ui8ptr += sizeof(uint32_t);
  	    sz -= sizeof(uint32_t);
  }

  enum { remain_3bytes = 3, remain_2bytes = 2, remain_1byte = 1 }_;

  switch (sz) {
    case remain_3bytes:
      hash ^= ui8ptr[2] << 16;
      [[fallthrough]];

    case remain_2bytes:
      hash ^= ui8ptr[1] << 8;
      [[fallthrough]];
    
    case remain_1byte:
      hash ^= ui8ptr[0];
      hash *= m_constant;
  }

  return hash;
}

struct hasher
{ inl_f size_t hash(const void *ptr, size_t sz, size_t seed = kDefaullSeed)
  { return hash_bytes(ptr, sz, seed); }

  template <typename Tp>
  inl_f size_t hash(Tp& val)
  { return hash(&val, sizeof(Tp)); }
};

int main()
{ int arr1[] = {9, 1, 2};
  int arr2[] = {9, 1, 3};
  hasher hasher;
  std::cout << std::hex
            << hasher.hash(arr1) << '\n'
            << hasher.hash(arr2) << '\n';
  return 0;
}