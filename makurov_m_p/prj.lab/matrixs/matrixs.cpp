#include "include/matrixs/matrixs.hpp"
MatrixS::MatrixS(const std::ptrdiff_t size_y, const std::ptrdiff_t size_x) : size_x_(size_x), size_y_(size_y), matrix_(ArrayT<int>(size_x* size_y)), rows_(ArrayT<std::ptrdiff_t>(size_y)) {
    for (std::ptrdiff_t i = 1; i < size_y_; ++i) {
        rows_[i] = i;
    }

}

MatrixS::MatrixS(const SizeType& size) :size_x_(get<1>(size)), size_y_(get<0>(size)), matrix_(ArrayT<int>(get<0>(size)* get<1>(size))), rows_(ArrayT<std::ptrdiff_t>(get<0>(size)))
{
    for (std::ptrdiff_t i = 1; i < get<0>(size); ++i) {
        rows_[i] = i;
    }
}


int& MatrixS::at(const std::ptrdiff_t ind_y, const std::ptrdiff_t ind_x) {
    indexCheck({ ind_y,ind_x });
    return  matrix_[rows_[ind_y] * size_x_ + ind_x];
}

const int& MatrixS::at(const std::ptrdiff_t ind_y, const std::ptrdiff_t ind_x) const {
    indexCheck({ ind_y,ind_x });
    return  matrix_[rows_[ind_y] * size_x_ + ind_x];
}

int& MatrixS::at(const SizeType& elem)
{
    indexCheck(elem);
    return matrix_[rows_[get<0>(elem)] * size_x_ + get<1>(elem)];
}

const int& MatrixS::at(const SizeType& elem) const
{
    indexCheck(elem);
    return matrix_[rows_[get<0>(elem)] * size_x_ + get<1>(elem)];
}

void MatrixS::swapRows(const std::ptrdiff_t lhs, const std::ptrdiff_t rhs) {
    swap(rows_[lhs], rows_[rhs]);
}

void MatrixS::swapRows(const SizeType& elem) {
    swap(rows_[get<0>(elem)], rows_[get<1>(elem)]);
}

void MatrixS::resize(const SizeType& elem)
{
    return resize(get<0>(elem), get<1>(elem));
}


void MatrixS::resize(const std::ptrdiff_t size_y_new, const std::ptrdiff_t size_x_new) {
    sizeCheck(size_y_new);
    sizeCheck(size_x_new);
    std::ptrdiff_t sizeXSmall = (size_x_new < size_x_) ? size_x_new : size_x_;
    std::ptrdiff_t sizeYSmall = (size_y_new < size_y_) ? size_y_new : size_y_;
    ArrayT<int> newMatrix = ArrayT<int>(size_x_new * size_y_new);


    for (std::ptrdiff_t y = 0; y < sizeYSmall; ++y) {
        for (std::ptrdiff_t x = 0; x < sizeXSmall; ++x) {
            newMatrix[y * size_x_new + x]= matrix_[rows_[y] * size_x_ + x];
        }
    }
    rows_.resize(size_y_new);
    for (std::ptrdiff_t i = 0; i < size_y_new; i++)
            rows_[i] = i;
    swap(newMatrix, matrix_);
    size_x_ = size_x_new;
    size_y_ = size_y_new;
}


void MatrixS::indexCheck(const SizeType& elem) const {
    if (get<0>(elem) < 0 || get<0>(elem) >= size_y_) {
        throw std::out_of_range("Index is out of range of array");
    }
    if (get<1>(elem) < 0 || get<1>(elem) >= size_x_) {
        throw std::out_of_range("Index is out of range of array");
    }
}
