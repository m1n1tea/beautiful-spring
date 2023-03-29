#include "include/matrixs/matrixs.hpp"
MatrixS::MatrixS(const ptrdiff_t sizeY, const ptrdiff_t sizeX) : sizeX_(sizeX), sizeY_(sizeY), matrix_(ArrayT<int>(sizeX* sizeY)), rows_(ArrayT<ptrdiff_t>(sizeY)) {
    for (ptrdiff_t i = 1; i < sizeY_; ++i) {
        rows_[i] = i;
    }

}

MatrixS::MatrixS(const SizeType& size) :sizeX_(get<1>(size)), sizeY_(get<0>(size)), matrix_(ArrayT<int>(get<0>(size)* get<1>(size))), rows_(ArrayT<ptrdiff_t>(get<0>(size)))
{
    for (ptrdiff_t i = 1; i < get<0>(size); ++i) {
        rows_[i] = i;
    }
}


int& MatrixS::at(const ptrdiff_t indY, const ptrdiff_t indX) {
    indexCheck({ indY,indX });
    return  matrix_[rows_[indY] * sizeX_ + indX];
}

const int& MatrixS::at(const ptrdiff_t indY, const ptrdiff_t indX) const {
    indexCheck({ indY,indX });
    return  matrix_[rows_[indY] * sizeX_ + indX];
}

int& MatrixS::at(const SizeType& elem)
{
    indexCheck(elem);
    return matrix_[rows_[get<0>(elem)] * sizeX_ + get<1>(elem)];
}

const int& MatrixS::at(const SizeType& elem) const
{
    indexCheck(elem);
    return matrix_[rows_[get<0>(elem)] * sizeX_ + get<1>(elem)];
}

void MatrixS::swapRows(const ptrdiff_t lhs, const ptrdiff_t rhs) {
    swap(rows_[lhs], rows_[rhs]);
}

void MatrixS::swapRows(const SizeType& elem) {
    swap(rows_[get<0>(elem)], rows_[get<1>(elem)]);
}

void MatrixS::resize(const SizeType& elem)
{
    return resize(get<0>(elem), get<1>(elem));
}

void MatrixS::resize(const ptrdiff_t sizeYNew, const ptrdiff_t sizeXNew) {
    sizeCheck(sizeYNew);
    sizeCheck(sizeXNew);
    ptrdiff_t sizeXSmall = (sizeXNew < sizeX_) ? sizeXNew : sizeX_;
    ptrdiff_t sizeYSmall = (sizeYNew < sizeY_) ? sizeYNew : sizeY_;
    ArrayT<int> newMatrix = ArrayT<int>(sizeXNew * sizeYNew);


    for (ptrdiff_t y = 0; y < sizeYSmall; ++y) {
        for (ptrdiff_t x = 0; x < sizeXSmall; ++x) {
            newMatrix[y * sizeXNew + x]= matrix_[rows_[y] * sizeX_ + x];
        }
    }
    rows_.resize(sizeYNew);
    for (ptrdiff_t i = 0; i < sizeYNew; i++)
            rows_[i] = i;
    swap(newMatrix, matrix_);
    sizeX_ = sizeXNew;
    sizeY_ = sizeYNew;
}


void MatrixS::indexCheck(const SizeType& elem) const {
    if (get<0>(elem) < 0 || get<0>(elem) >= sizeY_) {
        throw std::out_of_range("Index is out of range of array");
    }
    if (get<1>(elem) < 0 || get<1>(elem) >= sizeX_) {
        throw std::out_of_range("Index is out of range of array");
    }
}
