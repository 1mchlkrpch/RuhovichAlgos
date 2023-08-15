#include <iostream>
#include <fstream>

#include <cstring>

int init = [](){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    return 0;
}();

int main() {
    std::string s;
    getline(std::cin, s);

    std::cout << s << '\n';

    char ANSI_ALPH[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\0";
    int ans = 0;
    size_t left_cursor = 0;
    size_t right_cursor = 0;
    for (;left_cursor + right_cursor < s.size();) {
        if (s[left_cursor] != s[s.size() - 1 - right_cursor] && std::abs((int)(s[left_cursor] - 'A') - (int)(s[s.size() - 1 - right_cursor] - 'A')) != 32) {
            // printf("dif:%d - %d\n", (int)(s[left_cursor] - 'A'), (int)(s[s.size() - 1 - right_cursor] - 'A'));

            // printf("DIFS idx:%zu[%c|%c]\n", left_cursor, s[left_cursor], s[s.size() - 1 - right_cursor]);
            std::cout << "false\n";
            return 0;
            break;
        } else {
            // printf("SAME idx:%zu[%c|%c]\n", left_cursor, s[left_cursor], s[s.size() - 1 - right_cursor]);
            ++left_cursor;
            ++right_cursor;
        }
        while (!strchr(ANSI_ALPH, s[left_cursor]) && left_cursor + right_cursor < s.size()) {
            ++left_cursor;
        }
        while (!strchr(ANSI_ALPH, s[s.size() - 1 - right_cursor]) && left_cursor + right_cursor < s.size()) {
            ++right_cursor;
        }
    }

    std::cout << "true\n";

    return 0;
}