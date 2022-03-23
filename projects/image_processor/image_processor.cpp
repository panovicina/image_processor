#include <fstream>
#include <iostream>
#include <string>

#include "argvparser.cpp"
#include "bmpi.h"
#include "filters.cpp"

enum filterCode {
    fWrongFilter,
    fCrop,
    fGrayscale,
    fNegative,
    fSharpening,
    fEdgeDetection,
};

filterCode getFilterCode(std::string const &inString) {
    if (inString == "-crop")
        return fCrop;
    if (inString == "-gs")
        return fGrayscale;
    if (inString == "-neg")
        return fNegative;
    if (inString == "-sharp")
        return fSharpening;
    if (inString == "-edge")
        return fEdgeDetection;
    return fWrongFilter;
}

int main(int argc, char const *argv[]) {
    const int argv_infile_ind = 1;
    const int argv_outfile_ind = 2;

    if (argc < 3) {
        std::cout << "usage: {имя программы} {путь к входному файлу} {путь к выходному файлу}"
                     "[-{имя фильтра 1} [параметр фильтра 1] [параметр фильтра 2] ...]"
                     "[-{имя фильтра 2} [параметр фильтра 1] [параметр фильтра 2] ...] ...\n";
        return 0;
    }

    // read image
    bmpi::BitmapImage<RGBTriple> img;
    bmpi::ReadFromFile(argv[argv_infile_ind], img);
    if (!img) {
        std::cout << "err reading input file" << std::endl;
        return 0;
    }

    // apply filters
    ArgvParser parser(argc - 3, argv + 3);

    char const **option_ptr = nullptr;
    int64_t option_size = 0;
    std::unique_ptr<ImageFilter> filter;
    while (parser.GetNextOption(option_ptr, option_size)) {
        switch (getFilterCode(option_ptr[0])) {
            case fCrop: {
                if (option_size != 3) {
                    std::cout << "wrong usage of -crop filter\nusage: -crop width height" << std::endl;
                    break;
                }
                size_t width = 0;
                size_t height = 0;
                if (!SafeStoll(option_ptr[1], width) || !SafeStoll(option_ptr[2], height)) {
                    std::cout << "wrong crop parameters\nusage: -crop width height" << std::endl;
                    break;
                }
                std::cout << "applying crop " << width << ' ' << height << std::endl;
                filter.reset(new CropFilter(width, height));
                filter->Apply(img);
                break;
            }
            case fGrayscale: {
                if (option_size != 1) {
                    std::cout << "wrong usage of -gs filter\nusage: -gs" << std::endl;
                    break;
                }
                std::cout << "applying grayscale" << std::endl;
                filter.reset(new GrayscaleFilter());
                filter->Apply(img);
                break;
            }
            case fNegative: {
                if (option_size != 1) {
                    std::cout << "wrong usage of -neg filter\nusage: -neg" << std::endl;
                    break;
                }
                std::cout << "applying negative filter" << std::endl;
                filter.reset(new NegativeFilter());
                filter->Apply(img);
                break;
            }
            case fSharpening: {
                if (option_size != 1) {
                    std::cout << "wrong usage of -sharp filter\nusage: -sharp" << std::endl;
                    break;
                }
                std::cout << "applying sharpening" << std::endl;
                filter.reset(new SharpeningFilter());
                filter->Apply(img);
                break;
            }
            case fEdgeDetection: {
                if (option_size != 2) {
                    std::cout << "wrong usage of -edge filter\nusage: -edge threshold" << std::endl;
                    break;
                }
                size_t threshold = 0;
                if (!SafeStoll(option_ptr[1], threshold) || threshold > UCHAR_MAX) {
                    std::cout << "wrong Edge Detection parameters\nusage: -edge threshold" << std::endl;
                    break;
                }
                std::cout << "applying Edge Detection " << threshold << std::endl;
                filter.reset(new EdgeDetectionFilter(threshold));
                filter->Apply(img);
                break;
            }
            default: {
                std::cout << "cant parse option [" << option_ptr[0] << "]" << std::endl;
            }
        }
    }

    // write image
    if (!bmpi::WriteToFile(argv[argv_outfile_ind], img)) {
        std::cout << "err writing file" << std::endl;
    }

    return 0;
}
