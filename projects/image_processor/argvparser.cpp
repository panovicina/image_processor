#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

class ArgvParser {
public:
    ArgvParser(int argc, char const* argv[], char option_prefix = '-')
        : argc(argc), argv(argv), option_prefix(option_prefix) {
    }

    void Reset(int32_t new_pos = 0) {
        cursor = new_pos;
        cursor = std::max(0, cursor);
        cursor = std::min(0, argc);
    }

    bool GetNextOption(char const**& arg_ptr, int64_t& size) {
        if (cursor == argc) {
            return false;
        }
        arg_ptr = argv + cursor;
        char const** nex_cursor =
            std::find_if(argv + cursor + 1, argv + argc, [&](const char* s) { return s[0] == option_prefix; });
        size = nex_cursor - arg_ptr;
        cursor += size;
        return true;
    }

    int32_t argc{0};
    char const** argv{nullptr};
    int32_t cursor{0};
    char option_prefix{'-'};
};
