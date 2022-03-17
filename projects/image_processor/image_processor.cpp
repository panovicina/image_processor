#include <fstream>
#include <iostream>
#include <string>

#include "BMPfile.h"

int main(int argc, char const *argv[]) {
    const int argv_infile_ind = 1;
    const int argv_outfile_ind = 2;

    if (argc < 3) {
        std::cout << "usage: {имя программы} {путь к входному файлу} {путь к выходному файлу}"
                     "[-{имя фильтра 1} [параметр фильтра 1] [параметр фильтра 2] ...]"
                     "[-{имя фильтра 2} [параметр фильтра 1] [параметр фильтра 2] ...] ...\n";
        return 0;
    }

    BMP::BMPFile bmpf;
    BMP::ReadFromFile(argv[argv_infile_ind], bmpf);
    if (!bmpf) {
        std::cout << "err reading input file" << std::endl;
        return 0;
    }
    std::cout << bmpf.bm_file_header.ToString() << '\n' << bmpf.bm_info_header->ToString();

    BMP::BitmapImage img = bmpf.bm_image;

    img.Crop(0, 0, 1023, 1023);

    if (!BMP::WriteToFile(argv[argv_outfile_ind], img)) {
        std::cout << "err writing file" << std::endl;
    }

    return 0;
}
