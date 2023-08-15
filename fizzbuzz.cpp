#include <iostream>
#include <fstream>
#include <cstring>

auto boost = []()
{ std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  return 0;
}();

#define inl_f static inline __attribute((always_inline))

static const std::size_t kChunkSize = 119;
static const std::size_t kLimit     = 1'000;

inl_f void debugbuf(const char *const p)
{ printf(".\n");
	for (size_t sym_idx = 0; sym_idx < kChunkSize; ++sym_idx) {
  	printf("%c", p[sym_idx]);
  }
  printf(".\n");
}

inl_f void writeFizzBuzz(char **cptr)
{ memcpy(*cptr - 9, "\nFizzBuzz\n", 10);
  (*cptr) -= 10;
}

inl_f void writeFizz(char **cptr)
{ memcpy(*cptr - 5, "\nFizz\n", 6);
  (*cptr) -= 6;
}

inl_f void writeFuzz(char **cptr)
{ memcpy(*cptr - 5, "\nFizz\n", 6);
  (*cptr) -= 6;
}

static const char nums[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

inl_f void myitoa(int x, char **cptr)
{	do {
    **cptr = nums[x % 10];
    x /= 10;
    (*cptr)--;
  } while (x > 0);
}

inl_f void print(FILE *f, int num)
{ static char buf15nums[kChunkSize] = {0};
  char *cptr = buf15nums + kChunkSize - 1;
  writeFizzBuzz(&cptr);    // 15
  myitoa(num + 13, &cptr); // 14
  myitoa(num + 12, &cptr); // 13
  writeFizz(&cptr);        // 12
  myitoa(num + 11, &cptr); // 11
  myitoa(num + 10, &cptr); // 10
  writeFuzz(&cptr);        // 9
  myitoa(num + 7, &cptr);  // 8
  myitoa(num + 6, &cptr);  // 7 
  myitoa(num + 5, &cptr);  // 6
  writeFizz(&cptr);        // 5
  myitoa(num + 3, &cptr);  // 4
  myitoa(num + 2, &cptr);  // 3
  myitoa(num + 1, &cptr);  // 2
  myitoa(num + 0, &cptr);  // num(=1)
  fprintf(f, cptr + 1, cptr - buf15nums, sizeof(char));
}

int main() {
	FILE *f = fopen("file", "w");
	int i = 1;
	for (;i < kLimit - 10; i += 15) {
		print(f, i);
	}
	while (i <= kLimit) {
    if (i % 3 == 0) {
      printf("Fizz\n");
    } else if (i % 5 == 0) {
      printf("Buzz\n");
    } else {
      printf("%d\n", i);
    }
    i++;
  }
	return 0;
}
