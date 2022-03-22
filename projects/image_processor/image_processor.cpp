#include <fstream>
#include <iostream>
#include <string>

#include "bmpi.h"

int main(int argc, char const *argv[]) {
    const int argv_infile_ind = 1;
    const int argv_outfile_ind = 2;

    if (argc < 3) {
        std::cout << "usage: {имя программы} {путь к входному файлу} {путь к выходному файлу}"
                     "[-{имя фильтра 1} [параметр фильтра 1] [параметр фильтра 2] ...]"
                     "[-{имя фильтра 2} [параметр фильтра 1] [параметр фильтра 2] ...] ...\n";
        return 0;
    }

    bmpi::BitmapImage<RGBTriple> img;
    bmpi::ReadFromFile(argv[argv_infile_ind], img);
    if (!img) {
        std::cout << "err reading input file" << std::endl;
        return 0;
    }

    img.Crop(0, 500, 1000, 1000);

    if (!bmpi::WriteToFile(argv[argv_outfile_ind], img)) {
        std::cout << "err writing file" << std::endl;
    }

    return 0;
}
