#include "palindrome.h"
#include <string>

bool IsPalindrome(const std::string& str) {
    int i = 0;
    int j = size(str) - 1;
    while (i <= j) {
        if (str[i] == ' ') {
            ++i;
            continue;
        }
        if (str[j] == ' ') {
            --j;
            continue;
        }
        if (str[i] != str[j]) {
            return false;
        }
        ++i;
        --j;
    }
    return true;
}
