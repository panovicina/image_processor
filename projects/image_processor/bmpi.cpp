#include "bmpi.h"

namespace bmpi {

bool BITMAPFILEHEADER::Read(std::ifstream& ifs) {
    bool no_err = true;
    no_err &= SafeRead(&type, ifs);
    no_err &= SafeRead(&size, ifs);
    no_err &= SafeRead(&reserved1, ifs);
    no_err &= SafeRead(&reserved2, ifs);
    no_err &= SafeRead(&off_bits, ifs);
    return no_err;
}

bool BITMAPFILEHEADER::Write(std::ofstream& ofs) {
    SafeWrite(&type, ofs);
    SafeWrite(&size, ofs);
    SafeWrite(&reserved1, ofs);
    SafeWrite(&reserved2, ofs);
    SafeWrite(&off_bits, ofs);
    return true;
}

std::string BITMAPFILEHEADER::ToString() const {
    return "bmfileh:\n\ttype = " + std::to_string(type) + "\n\tsize = " + std::to_string(size) +
           "\n\treserved1 = " + std::to_string(reserved1) + "\n\treserved2 = " + std::to_string(reserved2) +
           "\n\toff_bits = " + std::to_string(off_bits) + '\n';
}

bool BITMAPINFOHEADER::Read(std::ifstream& ifs) {
    bool no_err = true;
    no_err &= SafeRead(&size, ifs);
    no_err &= SafeRead(&width, ifs);
    no_err &= SafeRead(&height, ifs);
    no_err &= SafeRead(&planes, ifs);
    no_err &= SafeRead(&bit_count, ifs);

    no_err &= SafeRead(&compression, ifs);
    no_err &= SafeRead(&size_image, ifs);
    no_err &= SafeRead(&x_pels_per_meter, ifs);
    no_err &= SafeRead(&y_pels_per_meter, ifs);
    no_err &= SafeRead(&clr_used, ifs);
    no_err &= SafeRead(&clr_important, ifs);
    return no_err;
}

bool BITMAPINFOHEADER::Write(std::ofstream& ofs) {
    SafeWrite(&size, ofs);
    SafeWrite(&width, ofs);
    SafeWrite(&height, ofs);
    SafeWrite(&planes, ofs);
    SafeWrite(&bit_count, ofs);
    SafeWrite(&compression, ofs);
    SafeWrite(&size_image, ofs);
    SafeWrite(&x_pels_per_meter, ofs);
    SafeWrite(&y_pels_per_meter, ofs);
    SafeWrite(&clr_used, ofs);
    SafeWrite(&clr_important, ofs);
    return true;
}

std::string BITMAPINFOHEADER::ToString() const {
    return "BITMAPINFOHEADER:\n\tsize = " + std::to_string(size) + "\n\twidth = " + std::to_string(width) +
           "\n\theight = " + std::to_string(height) + "\n\tplanes = " + std::to_string(planes) +
           "\n\tbit_count = " + std::to_string(bit_count) + "\n\tcompression = " + std::to_string(compression) +
           "\n\tsize_image = " + std::to_string(size_image) +
           "\n\tx_pels_per_meter = " + std::to_string(x_pels_per_meter) +
           "\n\ty_pels_per_meter = " + std::to_string(y_pels_per_meter) + "\n\tclr_used = " + std::to_string(clr_used) +
           "\n\tclr_important = " + std::to_string(clr_important) + '\n';
}

bool ReadFromFile(const std::filesystem::path& filename, BMPFile& bmpf) {
    bmpf.bm_image.Clear();

    std::ifstream input_stream(filename, std::ios::binary);
    if (!input_stream.is_open()) {
        // throw std::runtime_error("BMP read err: Input file [" + filename.string() + "] cannot be opened");
        std::cout << "BMP read err: Input file [" + filename.string() + "] cannot be opened" << std::endl;
        return false;
    }

    bool no_err = true;
    no_err &= bmpf.bm_file_header.Read(input_stream);
    if (!no_err) {
        std::cout << "err reading bmp BITMAPFILEHEADER" << std::endl;
        return false;
    }

    if (bmpf.bm_file_header.type != 0x4D42) {
        // throw std::runtime_error("err wrong bmp BITMAPFILEHEADER bfType. Should be 0x4D42");
        std::cout << "err wrong bmp BITMAPFILEHEADER bfType. Should be 0x4D42" << std::endl;
        return false;
    }

    uint32_t b_size;
    no_err &= SafeRead(&b_size, input_stream);
    input_stream.seekg(14);

    if (!no_err) {
        std::cout << "err reading BITMAPINFO size" << std::endl;
        return false;
    }

    if (b_size != 40) {
        std::cout << "err reading bmp BITMAPINFO of size " + std::to_string(b_size) + " not implemented" << std::endl;
        return false;
    }

    no_err &= bmpf.bm_info_header.Read(input_stream);

    if (!no_err) {
        std::cout << "err reading bmp BITMAPINFO" << std::endl;
        return false;
    }

    if (bmpf.bm_info_header.bit_count != 24) {
        // throw std::logic_error("BITMAPINFO bit_count " + std::to_string(bmpf.bm_info_header->GetBitCount())
        // + " not implemented");
        std::cout << "BITMAPINFO bit_count " + std::to_string(bmpf.bm_info_header.bit_count) + " not implemented"
                  << std::endl;
        return false;
    }

    if (bmpf.bm_info_header.compression != 0) {
        // throw std::logic_error("BITMAPINFO compressed formats not implemented");
        std::cout << "BMP compressed formats not implemented" << std::endl;
        return false;
    }

    size_t row_size_pad = (bmpf.bm_info_header.width * 24 + 31) / 32 * 4;  // row size with padding
    size_t row_size = bmpf.bm_info_header.width * 3;
    // size_t padding = row_size_pad - row_size;

    bmpf.bm_image.Resize(bmpf.bm_info_header.width, std::abs(bmpf.bm_info_header.height));

    size_t pos = bmpf.bm_file_header.off_bits;
    for (auto& row : bmpf.bm_image.Data()) {
        input_stream.seekg(pos);
        no_err &= SafeRead(reinterpret_cast<char*>(row.data()), row_size, input_stream);
        // input_stream.ignore(padding);
        pos += row_size_pad;
    }
    if (!no_err) {
        std::cout << "err reading pixel array" << std::endl;
        return false;
    }

    if (bmpf.bm_info_header.height > 0) {
        bmpf.bm_image.FlipVertical();
    }
    return no_err;
}

bool WriteToFile(const std::filesystem::path& filename, const BMPFile& bmpf) {
    auto& img = bmpf.bm_image;
    if (img.Empty()) {
        std::cout << "BMP write err: Empty image" << std::endl;
        return false;
    }

    std::ofstream output_stream(filename, std::ios::binary);
    if (!output_stream.is_open()) {
        // throw std::runtime_error("BMP write err: Output file [" + filename.string() + "] cannot be opened");
        std::cout << "BMP write err: Output file [" + filename.string() + "] cannot be opened" << std::endl;
        return false;
    }

    size_t row_size = img.GetWidth() * 3;
    size_t row_size_pad = (row_size * 8 + 31) / 32 * 4;  // row size with padding
    size_t pix_arr_size = img.GetHeight() * row_size_pad;

    BITMAPFILEHEADER bm_file_header = bmpf.bm_file_header;
    bm_file_header.size = bm_file_header.off_bits + pix_arr_size;

    if (!bm_file_header.Write(output_stream)) {
        return false;
    }

    BITMAPINFOHEADER bm_info_header = bmpf.bm_info_header;
    bm_info_header.width = img.GetWidth();
    bm_info_header.height = -img.GetHeight();
    bm_info_header.size_image = pix_arr_size;

    if (!bm_info_header.Write(output_stream)) {
        return false;
    }

    size_t pos = bm_file_header.off_bits;
    for (auto& row : img.Data()) {
        output_stream.seekp(pos);
        output_stream.write(reinterpret_cast<const char*>(row.data()), row_size);
        pos += row_size_pad;
    }

    return true;
}

bool ReadFromFile(const std::filesystem::path& filename, BitmapImage<RGBTriple>& img) {
    BMPFile bmpf;
    bool no_err = ReadFromFile(filename, bmpf);
    img = bmpf.bm_image;
    return no_err;
}

bool WriteToFile(const std::filesystem::path& filename, const BitmapImage<RGBTriple>& img) {
    BMPFile bmpf;
    bmpf.bm_image = img;
    return WriteToFile(filename, bmpf);
}

}  // namespace bmpi
