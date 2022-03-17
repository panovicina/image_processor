#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "BMPutils.h"
#include "bitmapimage.h"

namespace BMP {

class BITMAPFILEHEADER {
public:
    uint16_t type;  // identify field. should be 0x42 0x4D
    uint32_t size;  // file size in bytes
    uint16_t reserved1{0};
    uint16_t reserved2{0};
    uint32_t off_bits;  // bitmap image data offset

    bool Read(std::ifstream&);
    bool Write(std::ofstream&);
    std::string ToString() const;
};

class BITMAPINFO {
public:
    virtual ~BITMAPINFO(){};
    virtual uint16_t GetBitCount() const = 0;
    virtual bool Read(std::ifstream&) = 0;
    virtual bool Write(std::ofstream&) = 0;
    virtual std::string ToString() const = 0;
};

class BITMAPINFOHEADER : public BITMAPINFO {
public:
    uint32_t size;       // size of header, in bytes
    int32_t width;       // abs width in pixels
    int32_t height;      // height in pixels (signed)
    uint16_t planes;     // should be 1
    uint16_t bit_count;  // bits per pixel 1 - 64

    uint32_t compression;      // compression type 0 - 6
    uint32_t size_image;       // image size in bytes. can be 0
    int32_t x_pels_per_meter;  // horizontal resolution. pixels per meter
    int32_t y_pels_per_meter;  // vertical resolution. pixels per meter
    uint32_t clr_used;         // number of colors in the color palette
    uint32_t clr_important;    // the number of important colors used, or 0 when every color is important

    uint16_t GetBitCount() const override;
    bool Read(std::ifstream&) override;
    bool Write(std::ofstream&) override;
    std::string ToString() const override;
};

struct BMPFile {
public:
    BITMAPFILEHEADER bm_file_header;

    enum class BMInfoType { CORE = 12, V3 = 40, V4 = 108, V5 = 124 };
    BMInfoType bm_info_type{BMInfoType::V3};
    BITMAPINFO* bm_info_header{nullptr};

    BitmapImage bm_image;

    ~BMPFile() {
        delete bm_info_header;
    }

    bool operator!() {
        return !bm_image || bm_info_header == nullptr;
    }
};

bool ReadFromFile(const std::filesystem::path& filename, BMPFile& bmpf);
bool WriteToFile(const std::filesystem::path& filename, const BitmapImage& img);

}  // namespace BMP
