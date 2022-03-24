#ifdef _BITMAPIMAGE_IMPL_

namespace bmpi {

template <typename PixelT>
BitmapImage<PixelT>::BitmapImage() {
}

template <typename PixelT>
BitmapImage<PixelT>::BitmapImage(size_t width, size_t height) : data_(height, std::vector<PixelT>(width)) {
}

template <typename PixelT>
inline size_t BitmapImage<PixelT>::GetHeight() const {
    return data_.size();
}

template <typename PixelT>
inline size_t BitmapImage<PixelT>::GetWidth() const {
    if (!data_.empty()) {
        return data_[0].size();
    }
    return 0;
}

template <typename PixelT>
PixelT& BitmapImage<PixelT>::Get(int col, int row) {
    return data_[row][col];
}

template <typename PixelT>
std::vector<std::vector<PixelT>>& BitmapImage<PixelT>::Data() {
    return data_;
}
template <typename PixelT>
const std::vector<std::vector<PixelT>>& BitmapImage<PixelT>::Data() const {
    return data_;
}

template <typename PixelT>
bool BitmapImage<PixelT>::Empty() const {
    return data_.empty();
}
// True if empty image
template <typename PixelT>
bool BitmapImage<PixelT>::operator!() const {
    return Empty();
}

// crops image to [h_begin, h_end), [w_begin, w_end)
template <typename PixelT>
void BitmapImage<PixelT>::Crop(size_t w_begin, size_t h_begin, size_t w_end, size_t h_end) {
    if (h_begin > GetHeight() || w_begin > GetWidth() || h_begin > h_end || w_begin > w_end) {
        Clear();
        return;
    }

    h_end = std::min(h_end, GetHeight());
    w_end = std::min(w_end, GetWidth());

    data_.resize(h_end);
    data_.erase(data_.begin(), data_.begin() + h_begin);

    for (auto& row : data_) {
        row.resize(w_end);
        row.erase(row.begin(), row.begin() + w_begin);
    }
}

// == Crop(0, 0, height, width)
template <typename PixelT>
void BitmapImage<PixelT>::Crop(size_t w_end, size_t h_end) {
    Resize(std::min(w_end, GetWidth()), std::min(h_end, GetHeight()));
}

template <typename PixelT>
void BitmapImage<PixelT>::Resize(size_t width, size_t height) {
    data_.resize(height);
    for (auto& row : data_) {
        row.resize(width);
    }
}

template <typename PixelT>
void BitmapImage<PixelT>::FlipVertical() {
    std::reverse(data_.begin(), data_.end());
}

template <typename PixelT>
void BitmapImage<PixelT>::FlipHorizontal() {
    for (auto& row : data_) {
        std::reverse(row.begin(), row.end());
    }
}

template <typename PixelT>
void BitmapImage<PixelT>::Clear() {
    data_.clear();
}

template <typename PixelT>
void BitmapImage<PixelT>::Fill(PixelT& filler) {
    std::vector<PixelT> filled_row(GetWidth());
    std::fill(filled_row.begin(), filled_row.end(), filler);
    std::fill(data_.begin(), data_.end(), filled_row);
}

template <typename PixelT>
void BitmapImage<PixelT>::AddColoredIndent(int32_t top, int32_t bottom, int32_t left, int32_t right) {
    for (auto& row : data_) {
        row.insert(row.begin(), left, *row.begin());
        row.insert(row.end(), right, *row.rbegin());
    }
    data_.insert(data_.begin(), top, *data_.begin());
    data_.insert(data_.end(), bottom, *data_.rbegin());
}

}  // namespace bmpi

#endif  // _BITMAPIMAGE_IMPL_
