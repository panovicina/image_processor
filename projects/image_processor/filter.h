#pragma once

#include <climits>

#include "bitmapimage.h"
#include "utils.h"

class ImageFilter {
public:
    virtual void Apply(bmpi::BitmapImage<RGBTriple>&) = 0;
};

template <int mat_h, int mat_w, int mat_c_h, int mat_c_w>
class ImageMatrixFilter : public ImageFilter {
public:
    explicit ImageMatrixFilter(std::array<int32_t, mat_h * mat_w> matr) : filter_matrix(matr){};

    virtual void Apply(bmpi::BitmapImage<RGBTriple>& img) override {
        bmpi::BitmapImage<RGBTriple> new_img(img.GetWidth(), img.GetHeight());

        for (int row = 0; row < img.GetHeight(); ++row) {
            for (int col = 0; col < img.GetWidth(); ++col) {
                int32_t r = 0;
                int32_t g = 0;
                int32_t b = 0;
                for (int m_r = 0; m_r < mat_h; ++m_r) {
                    for (int m_c = 0; m_c < mat_w; ++m_c) {
                        auto pix = img.GetNearest(col + m_c - mat_c_w, row + m_r - mat_c_h);
                        r += pix.r * filter_matrix[m_r * mat_w + m_c];
                        g += pix.g * filter_matrix[m_r * mat_w + m_c];
                        b += pix.b * filter_matrix[m_r * mat_w + m_c];
                    }
                }
                new_img.Get(col, row).SetColors(r, g, b);
            }
        }

        img = new_img;
    }

protected:
    std::array<int32_t, mat_h * mat_w> filter_matrix;
};
