#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "bitmapimage.h"
#include "utils.h"

namespace bmpi {

class BITMAPFILEHEADER {
public:
    uint16_t type{19778};  // identify field. should be 0x42 0x4D
    uint32_t size;         // file size in bytes
    uint16_t reserved1{0};
    uint16_t reserved2{0};
    uint32_t off_bits{54};  // bitmap image data offset

    bool Read(std::ifstream&);
    bool Write(std::ofstream&);
    std::string ToString() const;
};

class BITMAPINFOHEADER {
public:
    uint32_t size{40};       // size of header, in bytes
    int32_t width{0};        // abs width in pixels
    int32_t height{0};       // height in pixels (signed)
    uint16_t planes{1};      // should be 1
    uint16_t bit_count{24};  // bits per pixel 1 - 64

    uint32_t compression{0};          // compression type 0 - 6
    uint32_t size_image{0};           // image size in bytes. can be 0
    int32_t x_pels_per_meter{11811};  // horizontal resolution. pixels per meter
    int32_t y_pels_per_meter{11811};  // vertical resolution. pixels per meter
    uint32_t clr_used{0};             // number of colors in the color palette
    uint32_t clr_important{0};        // the number of important colors used, or 0 when every color is important

    bool Read(std::ifstream&);
    bool Write(std::ofstream&);
    std::string ToString() const;
};

struct BMPFile {
public:
    BITMAPFILEHEADER bm_file_header;
    BITMAPINFOHEADER bm_info_header;

    BitmapImage<RGBTriple> bm_image;

    bool operator!() {
        return !bm_image;
    }
};

bool ReadFromFile(const std::filesystem::path& filename, BMPFile& bmpf);
bool WriteToFile(const std::filesystem::path& filename, const BMPFile& bmpf);

bool ReadFromFile(const std::filesystem::path& filename, BitmapImage<RGBTriple>& img);
bool WriteToFile(const std::filesystem::path& filename, const BitmapImage<RGBTriple>& img);

}  // namespace bmpi
