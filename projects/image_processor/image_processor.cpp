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
    fGaussianBlur,
    fMedian,
};

filterCode GetFilterCode(std::string const &in_string) {
    if (in_string == "-crop") {
        return fCrop;
    }
    if (in_string == "-gs") {
        return fGrayscale;
    }
    if (in_string == "-neg") {
        return fNegative;
    }
    if (in_string == "-sharp") {
        return fSharpening;
    }
    if (in_string == "-edge") {
        return fEdgeDetection;
    }
    if (in_string == "-blur") {
        return fGaussianBlur;
    }
    if (in_string == "-median") {
        return fMedian;
    }
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
        switch (GetFilterCode(option_ptr[0])) {
            case fCrop: {
                std::string usage = "\tusage: -crop width height";
                if (option_size != 3) {
                    std::cout << "wrong usage of -crop filter\n" << usage << std::endl;
                    break;
                }
                size_t width = 0;
                size_t height = 0;
                if (!SafeStoull(option_ptr[1], width) || !SafeStoull(option_ptr[2], height)) {
                    std::cout << "wrong crop parameters\n" << usage << std::endl;
                    break;
                }
                std::cout << "applying crop " << width << ' ' << height << std::endl;
                filter.reset(new CropFilter(width, height));
                filter->Apply(img);
                break;
            }
            case fGrayscale: {
                if (option_size != 1) {
                    std::cout << "wrong usage of -gs filter\n\tusage: -gs" << std::endl;
                    break;
                }
                std::cout << "applying grayscale" << std::endl;
                filter.reset(new GrayscaleFilter());
                filter->Apply(img);
                break;
            }
            case fNegative: {
                if (option_size != 1) {
                    std::cout << "wrong usage of -neg filter\n\tusage: -neg" << std::endl;
                    break;
                }
                std::cout << "applying negative filter" << std::endl;
                filter.reset(new NegativeFilter());
                filter->Apply(img);
                break;
            }
            case fSharpening: {
                if (option_size != 1) {
                    std::cout << "wrong usage of -sharp filter\n\tusage: -sharp" << std::endl;
                    break;
                }
                std::cout << "applying sharpening" << std::endl;
                filter.reset(new SharpeningFilter());
                filter->Apply(img);
                break;
            }
            case fEdgeDetection: {
                std::string usage =
                    "\tusage: -edge threshold\n"
                    "\tthreshold in range [0, 255]";
                if (option_size != 2) {
                    std::cout << "wrong usage of -edge filter\n" << usage << std::endl;
                    break;
                }
                size_t threshold = 0;
                if (!SafeStoull(option_ptr[1], threshold) || threshold > UCHAR_MAX) {
                    std::cout << "wrong Edge Detection parameters\n" << usage << std::endl;
                    break;
                }
                std::cout << "applying Edge Detection " << threshold << std::endl;
                filter.reset(new EdgeDetectionFilter(threshold));
                filter->Apply(img);
                break;
            }
            case fGaussianBlur: {
                std::string usage =
                    "\tusage: -blur sigma [kernel_radius]\n"
                    "\tsigma in range [0, 100]\n"
                    "\tkernel_radius in range [1, 100] optional";
                if (option_size != 2 && option_size != 3) {
                    std::cout << "wrong usage of -blur filter\n" << usage << std::endl;
                    break;
                }
                long double sigma = 0;
                size_t kernel_radius = 15;
                if (!SafeStold(option_ptr[1], sigma) || sigma > 100 || sigma < 0) {
                    std::cout << "wrong Gaussian Blur parameters\n" << usage << std::endl;
                    break;
                }
                if (option_size == 3) {
                    if (!SafeStoull(option_ptr[2], kernel_radius) || kernel_radius > 100 || kernel_radius < 1) {
                        std::cout << "wrong Gaussian Blur parameters\n" << usage << std::endl;
                        break;
                    }
                    std::cout << "applying Gaussian Blur " << sigma << ' ' << kernel_radius << std::endl;
                } else {
                    std::cout << "applying Gaussian Blur " << sigma << std::endl;
                }
                filter.reset(new GaussianBlurFilter(sigma, kernel_radius));
                filter->Apply(img);
                break;
            }
            case fMedian: {
                std::string usage =
                    "\tusage: -median kernel_radius\n"
                    "\tkernel_radius in range [1, 5]";
                if (option_size != 2) {
                    std::cout << "wrong usage of -median filter\n" << usage << std::endl;
                    break;
                }
                size_t kernel_radius = 1;
                if (!SafeStoull(option_ptr[1], kernel_radius) || kernel_radius > 5 || kernel_radius < 1) {
                    std::cout << "wrong Median Filter parameters\n" << usage << std::endl;
                    break;
                }
                std::cout << "applying Median Filter " << kernel_radius << std::endl;

                filter.reset(new MedianFilter(kernel_radius));
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
