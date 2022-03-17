#pragma once

#include <fstream>

namespace BMP {

// 24 bit rgb cell
struct RGBTriple {
    uint8_t red{0};
    uint8_t green{0};
    uint8_t blue{0};
};

inline bool SafeRead(char* buffer, std::streamsize count, std::ifstream& ifs) {
    return ifs.read(buffer, count) && ifs.gcount() == count;
}

}  // namespace BMP
