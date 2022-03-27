#include <climits>
#include <cmath>
#include <numeric>

#include "filter.h"

class CropFilter : public ImageFilter {
public:
    CropFilter(size_t width, size_t height) : width(width), height(height) {
    }
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

class GaussianBlurFilter : public ImageFilter {
public:
    GaussianBlurFilter(long double sigma, int kernel_radius = 15) : sigma(sigma), kernel_radius(kernel_radius) {
    }

    virtual void Apply(bmpi::BitmapImage<RGBTriple>& img) override {
        int max_coefs = kernel_radius;
        long double threshold = 0.001;
        long double pi = 3.141592653589793238462643383279502884L;
        long double dividor = 1 / (sqrtl(2 * pi) * sigma);

        // compute coefs
        std::vector<long double> gaussian_coefs;

        for (int i = 0; i < max_coefs; ++i) {
            long double res = dividor * expl(-((i * i) / (2 * sigma * sigma)));
            if (res < threshold) {
                break;
            }
            gaussian_coefs.push_back(res);
        }

        int margin = gaussian_coefs.size() - 1;

        // mirroring coefs
        std::reverse(gaussian_coefs.begin(), gaussian_coefs.end());
        gaussian_coefs.resize(margin * 2 + 1);
        std::copy_n(gaussian_coefs.begin(), margin, gaussian_coefs.rbegin());

        // normalize
        long double sum = std::accumulate(gaussian_coefs.begin(), gaussian_coefs.end(), static_cast<long double>(0));
        for (auto& el : gaussian_coefs) {
            el /= sum;
        }

        // adding indent to images
        img.AddColoredIndent(margin, margin, margin, margin);
        bmpi::BitmapImage<RGBTriple> also_img = img;

        // apply to each row
        for (int row = 0; row < static_cast<int>(img.GetHeight()); ++row) {
            for (int col = margin; col < static_cast<int>(img.GetWidth()) - margin; ++col) {
                long double r = 0;
                long double g = 0;
                long double b = 0;
                for (int m_ind = 0; m_ind < static_cast<int>(gaussian_coefs.size()); ++m_ind) {
                    auto pix = img.Get(col + m_ind - margin, row);
                    r += pix.r * gaussian_coefs[m_ind];
                    g += pix.g * gaussian_coefs[m_ind];
                    b += pix.b * gaussian_coefs[m_ind];
                }
                also_img.Get(col, row).SetColors(r, g, b);
            }
        }

        img = also_img;

        // apply to each column
        for (int row = margin; row < static_cast<int>(img.GetHeight()) - margin; ++row) {
            for (int col = margin; col < static_cast<int>(img.GetWidth()) - margin; ++col) {
                long double r = 0;
                long double g = 0;
                long double b = 0;
                for (int m_ind = 0; m_ind < static_cast<int>(gaussian_coefs.size()); ++m_ind) {
                    auto pix = also_img.Get(col, row + m_ind - margin);
                    r += pix.r * gaussian_coefs[m_ind];
                    g += pix.g * gaussian_coefs[m_ind];
                    b += pix.b * gaussian_coefs[m_ind];
                }
                img.Get(col, row).SetColors(r, g, b);
            }
        }
        img.Crop(margin, margin, img.GetWidth() - margin, img.GetHeight() - margin);
    }

    long double sigma;
    int kernel_radius;
};

class MedianFilter : public ImageFilter {
public:
    MedianFilter(int32_t window_radius = 1) : window_radius(window_radius) {
    }

    virtual void Apply(bmpi::BitmapImage<RGBTriple>& img) override {
        // adding indent to images
        img.AddColoredIndent(window_radius, window_radius, window_radius, window_radius);
        bmpi::BitmapImage<RGBTriple> also_img = img;

        // apply to each row
        for (int row = window_radius; row < static_cast<int>(img.GetHeight()) - window_radius; ++row) {
            for (int col = 0; col < static_cast<int>(img.GetWidth()); ++col) {
                std::vector<uint8_t> r;
                std::vector<uint8_t> g;
                std::vector<uint8_t> b;
                for (int m_ind = 0; m_ind < window_radius * 2 + 1; ++m_ind) {
                    auto pix = img.Get(col, row + m_ind - window_radius);
                    r.push_back(pix.r);
                    g.push_back(pix.g);
                    b.push_back(pix.b);
                }
                auto m = r.begin() + window_radius;
                std::nth_element(r.begin(), m, r.end());
                m = g.begin() + window_radius;
                std::nth_element(g.begin(), m, g.end());
                m = b.begin() + window_radius;
                std::nth_element(b.begin(), m, b.end());
                also_img.Get(col, row).SetColors(r[window_radius], g[window_radius], b[window_radius]);
            }
        }

        img = also_img;

        // apply to each row
        for (int row = 0; row < static_cast<int>(img.GetHeight()); ++row) {
            for (int col = window_radius; col < static_cast<int>(img.GetWidth()) - window_radius; ++col) {
                std::vector<uint8_t> r;
                std::vector<uint8_t> g;
                std::vector<uint8_t> b;
                for (int m_ind = 0; m_ind < window_radius * 2 + 1; ++m_ind) {
                    auto pix = also_img.Get(col + m_ind - window_radius, row);
                    r.push_back(pix.r);
                    g.push_back(pix.g);
                    b.push_back(pix.b);
                }
                auto m = r.begin() + window_radius;
                std::nth_element(r.begin(), m, r.end());
                m = g.begin() + window_radius;
                std::nth_element(g.begin(), m, g.end());
                m = b.begin() + window_radius;
                std::nth_element(b.begin(), m, b.end());
                img.Get(col, row).SetColors(r[window_radius], g[window_radius], b[window_radius]);
            }
        }
        img.Crop(window_radius, window_radius, img.GetWidth() - window_radius, img.GetHeight() - window_radius);
    }

    int window_radius;
};

class SharpeningFilter : public ImageMatrixFilter<3, 3, 1, 1> {
public:
    SharpeningFilter() : ImageMatrixFilter({0, -1, 0, -1, 5, -1, 0, -1, 0}) {
    }
};

class EdgeDetectionFilter : public ImageMatrixFilter<3, 3, 1, 1> {
public:
    EdgeDetectionFilter(uint8_t threshold) : ImageMatrixFilter({0, -1, 0, -1, 4, -1, 0, -1, 0}), threshold(threshold) {
    }

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
