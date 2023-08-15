#include <iostream>
#include <fstream>

#include <algorithm>

#include <cstring>

auto init = [](){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::ofstream out("user.out");
    for (std::string str; getline(std::cin, str); out << '\n') {
        if (str.size() == 2) {
            continue;
        }
        char* __restrict l = str.data() + 1;
        char* __restrict r = str.data() + str.size() - 2;
        uint64_t max_volume = 0;

        register int num1 = 0;
        int* __restrict num1ptr = &num1;
        register int num2 = 0;
        int* __restrict num2ptr = &num2;
        // __builtin_prefetch (&a[i+j], 1, 1);

        bool read_r = true;
        bool read_l = true;

        int len = std::count(str.begin(), str.end(), ',');

        while (l < r) [[likely]] {
            if (read_l) {
                read_l = false;
                *num1ptr = (*l) & 15;
                ++l;
                // printf("1digit:(%d)\n", *num1ptr);
                while (((*l) & 15) < 10) {
                    // printf("load:")
                    *num1ptr = *num1ptr * 10 + ((*l) & 15);
                    ++l;
                }
                // printf("*num1ptr:(%d)\n", *num1ptr);
            }
            if (read_r) {
                read_r = false;
                *num2ptr = (*r) & 15;
                --r;
                int ten_deg = 10;
                while (((*r) & 15) < 10) [[likely]] {
                    *num2ptr = *num2ptr + ((*r) & 15) * ten_deg;
                    ten_deg *= 10;
                    --r;
                }
            }
            // printf("cur res: %d * (%d|%d) = %d\n",
                // len, *num1ptr, *num2ptr, len * std::min(*num1ptr, *num2ptr));
            max_volume = std::max((int)max_volume, len * std::min(*num1ptr, *num2ptr));
            if (*num1ptr < *num2ptr) {
                ++l;
                read_l = true;
            }
            else {
                --r;
                read_r = true;
            }
            --len;
        }
        out << max_volume;
        // printf("ans:(%lu)\n", max_volume);
    }
    out.flush();
    exit(0);
    return 0;
}();

int main() {
	// printf("main\n");
	return 0;
}