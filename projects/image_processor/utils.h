#pragma once

#include <fstream>
#include <iostream>

// 24 bit rgb cell
struct RGBTriple {
    uint8_t b{0};
    uint8_t g{0};
    uint8_t r{0};
};

inline bool SafeRead(char* buffer, std::streamsize count, std::ifstream& ifs) {
    return ifs.read(buffer, count) && ifs.gcount() == count;
}

template <typename T>
inline bool SafeRead(T* buffer, std::ifstream& ifs) {
    return SafeRead(reinterpret_cast<char*>(buffer), sizeof(T), ifs);
}

template <typename T>
inline void SafeWrite(T* buffer, std::ofstream& ifs) {
    ifs.write(reinterpret_cast<char*>(buffer), sizeof(T));
}
