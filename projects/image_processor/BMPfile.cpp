#include "BMPfile.h"

namespace BMP {

bool BITMAPFILEHEADER::Read(std::ifstream& ifs) {
    bool no_err = true;
    no_err &= SafeRead(reinterpret_cast<char*>(&type), sizeof type, ifs);
    no_err &= SafeRead(reinterpret_cast<char*>(&size), sizeof size, ifs);
    no_err &= SafeRead(reinterpret_cast<char*>(&reserved1), sizeof reserved1, ifs);
    no_err &= SafeRead(reinterpret_cast<char*>(&reserved2), sizeof reserved2, ifs);
    no_err &= SafeRead(reinterpret_cast<char*>(&off_bits), sizeof off_bits, ifs);
    return no_err;
}

bool BITMAPFILEHEADER::Write(std::ofstream& ofs) {
    ofs.write(reinterpret_cast<char*>(&type), sizeof type);
    ofs.write(reinterpret_cast<char*>(&size), sizeof size);
    ofs.write(reinterpret_cast<char*>(&reserved1), sizeof reserved1);
    ofs.write(reinterpret_cast<char*>(&reserved2), sizeof reserved2);
    ofs.write(reinterpret_cast<char*>(&off_bits), sizeof off_bits);
    return true;
}

std::string BITMAPFILEHEADER::ToString() const {
    return "bmfileh:\n\ttype = " + std::to_string(type) + "\n\tsize = " + std::to_string(size) +
           "\n\treserved1 = " + std::to_string(reserved1) + "\n\treserved2 = " + std::to_string(reserved2) +
           "\n\toff_bits = " + std::to_string(off_bits) + '\n';
}

bool BITMAPINFOHEADER::Read(std::ifstream& ifs) {
    bool no_err = true;
    no_err &= SafeRead(reinterpret_cast<char*>(&size), sizeof size, ifs);
    no_err &= SafeRead(reinterpret_cast<char*>(&width), sizeof width, ifs);
    no_err &= SafeRead(reinterpret_cast<char*>(&height), sizeof height, ifs);
    no_err &= SafeRead(reinterpret_cast<char*>(&planes), sizeof planes, ifs);
    no_err &= SafeRead(reinterpret_cast<char*>(&bit_count), sizeof bit_count, ifs);

    no_err &= SafeRead(reinterpret_cast<char*>(&compression), sizeof compression, ifs);
    no_err &= SafeRead(reinterpret_cast<char*>(&size_image), sizeof size_image, ifs);
    no_err &= SafeRead(reinterpret_cast<char*>(&x_pels_per_meter), sizeof x_pels_per_meter, ifs);
    no_err &= SafeRead(reinterpret_cast<char*>(&y_pels_per_meter), sizeof y_pels_per_meter, ifs);
    no_err &= SafeRead(reinterpret_cast<char*>(&clr_used), sizeof clr_used, ifs);
    no_err &= SafeRead(reinterpret_cast<char*>(&clr_important), sizeof clr_important, ifs);
    return no_err;
}

bool BITMAPINFOHEADER::Write(std::ofstream& ofs) {
    ofs.write(reinterpret_cast<char*>(&size), sizeof size);
    ofs.write(reinterpret_cast<char*>(&width), sizeof width);
    ofs.write(reinterpret_cast<char*>(&height), sizeof height);
    ofs.write(reinterpret_cast<char*>(&planes), sizeof planes);
    ofs.write(reinterpret_cast<char*>(&bit_count), sizeof bit_count);

    ofs.write(reinterpret_cast<char*>(&compression), sizeof compression);
    ofs.write(reinterpret_cast<char*>(&size_image), sizeof size_image);
    ofs.write(reinterpret_cast<char*>(&x_pels_per_meter), sizeof x_pels_per_meter);
    ofs.write(reinterpret_cast<char*>(&y_pels_per_meter), sizeof y_pels_per_meter);
    ofs.write(reinterpret_cast<char*>(&clr_used), sizeof clr_used);
    ofs.write(reinterpret_cast<char*>(&clr_important), sizeof clr_important);
    return true;
}

uint16_t BITMAPINFOHEADER::GetBitCount() const {
    return bit_count;
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
    bmpf.bm_info_header = nullptr;
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
    no_err &= SafeRead(reinterpret_cast<char*>(&b_size), sizeof(b_size), input_stream);
    input_stream.seekg(14);

    if (!no_err) {
        std::cout << "err reading BITMAPINFO size" << std::endl;
        return false;
    }

    switch (b_size) {
        case 40:
            bmpf.bm_info_type = BMPFile::BMInfoType::V3;
            bmpf.bm_info_header = new BITMAPINFOHEADER();
            no_err &= bmpf.bm_info_header->Read(input_stream);
            break;
        default:
            // throw std::logic_error("err reading bmp BITMAPINFO of size "+ std::to_string(b_size)+" not implemented");
            std::cout << "err reading bmp BITMAPINFO of size " + std::to_string(b_size) + " not implemented"
                      << std::endl;
            return false;
    }

    if (!no_err) {
        std::cout << "err reading bmp BITMAPINFO" << std::endl;
        return false;
    }

    if (bmpf.bm_info_header->GetBitCount() != 24) {
        // throw std::logic_error("BITMAPINFO bit_count " + std::to_string(bmpf.bm_info_header->GetBitCount())
        // + " not implemented");
        std::cout << "BITMAPINFO bit_count " + std::to_string(bmpf.bm_info_header->GetBitCount()) + " not implemented"
                  << std::endl;
        return false;
    }

    if (bmpf.bm_info_header->GetBitCount() == 24 && bmpf.bm_info_type == BMPFile::BMInfoType::V3) {
        BITMAPINFOHEADER* hdr = dynamic_cast<BITMAPINFOHEADER*>(bmpf.bm_info_header);
        if (hdr->compression != 0) {
            // throw std::logic_error("BITMAPINFO compressed formats not implemented");
            std::cout << "BITMAPINFO compressed formats not implemented" << std::endl;
            return false;
        }

        size_t row_size_pad = (hdr->width * 24 + 31) / 32 * 4;  // row size with padding
        size_t row_size = hdr->width * 3;
        // size_t padding = row_size_pad - row_size;

        bmpf.bm_image.Resize(hdr->width, std::abs(hdr->height));

        size_t pos = bmpf.bm_file_header.off_bits;
        for (auto& row : bmpf.bm_image.GetData()) {
            input_stream.seekg(pos);
            no_err &= SafeRead(reinterpret_cast<char*>(row.data()), row_size, input_stream);
            // input_stream.ignore(padding);
            pos += row_size_pad;
        }
        if (!no_err) {
            std::cout << "err reading pixel array" << std::endl;
            return false;
        }

        if (hdr->height > 0) {
            bmpf.bm_image.FlipVertical();
        }
        return no_err;
    } else {
        // throw std::logic_error("not implemented");
        std::cout << "err not implemented" << std::endl;
        return false;
    }
}

bool WriteToFile(const std::filesystem::path& filename, const BitmapImage& img) {
    if (img.GetPixelBSize() != 3) {
        std::cout << "not implemented" << std::endl;
    }

    std::ofstream output_stream(filename, std::ios::binary);
    if (!output_stream.is_open()) {
        // throw std::runtime_error("BMP write err: Output file [" + filename.string() + "] cannot be opened");
        std::cout << "BMP write err: Output file [" + filename.string() + "] cannot be opened" << std::endl;
        return false;
    }

    BITMAPFILEHEADER bm_file_header;
    bm_file_header.type = 19778;
    bm_file_header.reserved1 = 0;
    bm_file_header.reserved2 = 0;
    bm_file_header.off_bits = 54;

    size_t row_size_pad = (img.GetWidth() * img.GetPixelBSize() * 8 + 31) / 32 * 4;  // row size with padding
    size_t row_size = img.GetWidth() * 3;
    // size_t padding = row_size_pad - row_size;
    size_t pix_arr_size = img.GetHeight() * row_size_pad;
    bm_file_header.size = bm_file_header.off_bits + pix_arr_size;

    if (!bm_file_header.Write(output_stream)) {
        return false;
    }

    BITMAPINFOHEADER bm_info_header;
    bm_info_header.size = 40;
    bm_info_header.planes = 1;
    bm_info_header.bit_count = 24;
    bm_info_header.compression = 0;
    bm_info_header.x_pels_per_meter = 11811;  // TODO ????
    bm_info_header.y_pels_per_meter = 11811;
    bm_info_header.clr_used = 0;
    bm_info_header.clr_important = 0;

    bm_info_header.width = img.GetWidth();
    bm_info_header.height = -img.GetHeight();  // TODO minus shit fix
    bm_info_header.size_image = pix_arr_size;

    if (!bm_info_header.Write(output_stream)) {
        return false;
    }

    size_t pos = bm_file_header.off_bits;
    for (auto& row : img.GetData()) {
        output_stream.seekp(pos);
        output_stream.write(reinterpret_cast<const char*>(row.data()), row_size);
        pos += row_size_pad;
    }

    return true;
}

}  // namespace BMP
