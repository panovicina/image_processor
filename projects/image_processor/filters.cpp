#include "filter.h"

class CropFilter : public ImageFilter {
public:
    CropFilter(size_t width, size_t height) : width(width), height(height) { }
    virtual void Apply(bmpi::BitmapImage<RGBTriple>& img) override {
        img.Crop(width, height);
    }

    size_t width;
    size_t height;
};

class GrayscaleFilter : public ImageFilter {
public:
    virtual void Apply(bmpi::BitmapImage<RGBTriple>& img) override {
        for (auto& row : img.Data()) {
            for (RGBTriple& pix : row) {
                uint8_t gray = 0.299 * pix.r + 0.587 * pix.g + 0.114 * pix.b;
                pix.r = gray;
                pix.g = gray;
                pix.b = gray;
            }
        }
    }
};

class NegativeFilter : public ImageFilter {
public:
    virtual void Apply(bmpi::BitmapImage<RGBTriple>& img) override {
        for (auto& row : img.Data()) {
            for (RGBTriple& pix : row) {
                pix.r = UCHAR_MAX - pix.r;
                pix.g = UCHAR_MAX - pix.g;
                pix.b = UCHAR_MAX - pix.b;
            }
        }
    }
};

class SharpeningFilter : public ImageMatrixFilter<3, 3, 1, 1> {
public:
    SharpeningFilter() : ImageMatrixFilter({0, -1, 0, -1, 5, -1, 0, -1, 0}) {
    }
};

class EdgeDetectionFilter : public ImageMatrixFilter<3, 3, 1, 1> {
public:
    EdgeDetectionFilter(uint8_t threshold) : ImageMatrixFilter({0, -1, 0, -1, 4, -1, 0, -1, 0}), threshold(threshold) { }
// {0, -1, 0, -1, 4, -1, 0, -1, 0}
    virtual void Apply(bmpi::BitmapImage<RGBTriple>& img) override {
        GrayscaleFilter().Apply(img);

        ImageMatrixFilter::Apply(img);

        RGBTriple white{UCHAR_MAX, UCHAR_MAX, UCHAR_MAX};
        RGBTriple black{0, 0, 0};

        for (auto& row : img.Data()) {
            for (RGBTriple& pix : row) {
                if (pix.r > threshold) {
                    pix = white;
                } else {
                    pix = black;
                }
            }
        }
    }

    uint8_t threshold{127};
};

// class NegativeFilter : public ImageMatrixFilter<3> {
// public:
//     virtual void Apply(bmpi::BitmapImage<RGBTriple>& img) override {
//         for (auto& row : img.Data()) {
//             for (RGBTriple& pix : row) {
//                 pix.r = UCHAR_MAX - pix.r;
//                 pix.g = UCHAR_MAX - pix.g;
//                 pix.b = UCHAR_MAX - pix.b;
//             }
//         }
//     }
// };
