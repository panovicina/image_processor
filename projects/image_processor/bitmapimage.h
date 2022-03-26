#pragma once

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace bmpi {

// (0, 0) at the top left corner
template <typename PixelT>
class BitmapImage {
public:
    BitmapImage();

    BitmapImage(size_t width, size_t height);

    inline size_t GetHeight() const;
    inline size_t GetWidth() const;

    PixelT& Get(int col, int row);

    std::vector<std::vector<PixelT>>& Data();
    const std::vector<std::vector<PixelT>>& Data() const;

    bool Empty() const;
    // True if empty image
    bool operator!() const;

    // crops image to [h_begin, h_end), [w_begin, w_end)
    void Crop(size_t w_begin, size_t h_begin, size_t w_end, size_t h_end);
    // == Crop(0, 0, height, width)
    void Crop(size_t w_end, size_t h_end);

    void Resize(size_t width, size_t height);

    void FlipVertical();
    void FlipHorizontal();

    void Clear();

    void Fill(PixelT& filler);

    // Adds padding around img. The colors of the added pixels are equal to the pixel closest to them
    void AddColoredIndent(int32_t top, int32_t bottom, int32_t left, int32_t right);

private:
    std::vector<std::vector<PixelT>> data_;
};

}  // namespace bmpi

#define _BITMAPIMAGE_IMPL_
#include "bitmapimage.cpp"
