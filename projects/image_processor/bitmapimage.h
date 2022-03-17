#pragma once

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace BMP {

// (0, 0) at the top left corner
class BitmapImage {
public:
    BitmapImage(){};

    BitmapImage(size_t width, size_t height)
        : _width(width), _height(height), _pixel_bsize(3), _data(height, std::vector<uint8_t>(width * _pixel_bsize)) {
    }

    size_t GetWidth() const {
        return _width;
    }
    size_t GetHeight() const {
        return _height;
    }

    uint16_t GetPixelBSize() const {
        return _pixel_bsize;
    }

    void SetPixelBSize(uint16_t new_bsize) {
        _pixel_bsize = new_bsize;
        Resize(_width, _height);
    }

    // individual pixel acess operator
    uint8_t* operator()(size_t col, size_t row) {
        return &_data[row][col * _pixel_bsize];
    }

    template <typename PixelT>
    PixelT& Get(size_t col, size_t row) {
        return *reinterpret_cast<PixelT*>(&_data[row][col * _pixel_bsize]);
    }

    uint8_t* GetRowPtr(size_t row) {
        return _data[row].data();
    }

    std::vector<std::vector<uint8_t>>& GetData() {
        return _data;
    }

    const std::vector<std::vector<uint8_t>>& GetData() const {
        return _data;
    }

    // return True if invalid image
    bool operator!() const {
        return _width == 0 || _height == 0 || _data.size() == 0;
    }

    void Crop(size_t w_begin, size_t h_begin, size_t w_end, size_t h_end) {
        if (h_begin > _height || w_begin > _width) {
            Clear();
            return;
        }
        h_end = std::min(h_end, _height);
        w_end = std::min(w_end, _width);

        _data.resize(h_end);
        _data.erase(_data.begin(), _data.begin() + h_begin);

        for (auto& row : _data) {
            row.resize(w_end);
            row.erase(row.begin(), row.begin() + w_begin * _pixel_bsize);
        }

        _width = w_end - w_begin;
        _height = h_end - h_begin;
    }

    // == Crop(0, 0, height, width)
    void Resize(size_t width, size_t height) {
        _width = width;
        _height = height;
        _data.resize(_height);
        for (auto& row : _data) {
            row.resize(_width * _pixel_bsize);
        }
    }

    void FlipVertical() {
        std::reverse(_data.begin(), _data.end());
    }
    void FlipHorizontal() {
        for (auto& row : _data) {
            std::reverse(row.begin(), row.end());
        }
    }

    void Clear() {
        _data.clear();
        _height = 0;
        _width = 0;
    }

    template <typename PixelT>
    bool Fill(PixelT& filler) {
        if (sizeof(PixelT) != _pixel_bsize) {
            return false;
        }

        std::vector<uint8_t> filled_row(_width * _pixel_bsize);
        PixelT* beg = reinterpret_cast<PixelT*>(filled_row.data());
        std::fill(beg, beg + _width, filler);

        std::fill(_data.begin(), _data.end(), filled_row);
        return true;
    }

private:
    size_t _width{0};
    size_t _height{0};
    uint16_t _pixel_bsize{3};  // bytes per pixel;
    std::vector<std::vector<uint8_t>> _data;
};

}  // namespace BMP
