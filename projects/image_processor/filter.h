#pragma once

#include "bitmapimage.h"
#include "utils.h"

// basic image filter interface
class ImageFilter {
public:
    virtual void Apply(bmpi::BitmapImage<RGBTriple>&) = 0;
};

// mat_h - matrix height
// mat_w - matrix width
// mat_c_h - matrix application point height index
// mat_c_w - matrix application point width index
template <int mat_h, int mat_w, int mat_c_h, int mat_c_w>
class ImageMatrixFilter : public ImageFilter {
public:
    explicit ImageMatrixFilter(std::array<long double, mat_h * mat_w> matr) : filter_matrix(matr){};

    virtual void Apply(bmpi::BitmapImage<RGBTriple>& img) override {
        bmpi::BitmapImage<RGBTriple> prev_img = img;
        prev_img.AddColoredIndent(mat_c_h, mat_h - mat_c_h - 1, mat_c_w, mat_w - mat_c_w - 1);

        for (int row = 0; row < static_cast<int>(img.GetHeight()); ++row) {
            for (int col = 0; col < static_cast<int>(img.GetWidth()); ++col) {
                long double r = 0;
                long double g = 0;
                long double b = 0;
                for (int m_row = 0; m_row < mat_h; ++m_row) {
                    for (int m_col = 0; m_col < mat_w; ++m_col) {
                        auto pix = prev_img.Get(col + m_col, row + m_row);
                        r += pix.r * filter_matrix[m_row * mat_w + m_col];
                        g += pix.g * filter_matrix[m_row * mat_w + m_col];
                        b += pix.b * filter_matrix[m_row * mat_w + m_col];
                    }
                }
                img.Get(col, row).SetColors(r, g, b);
            }
        }
    }

protected:
    std::array<long double, mat_h * mat_w> filter_matrix;
};
