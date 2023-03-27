#include "include/matrixs/matrixs.hpp"
MatrixS::MatrixS(const ptrdiff_t& sizeY, const ptrdiff_t& sizeX) : sizeX_(sizeX), sizeY_(sizeY), matrix_(ArrayT<int>(sizeX* sizeY)), rows_(ArrayT<ptrdiff_t>(sizeY)) {
    for (ptrdiff_t i = 1; i < sizeY_; ++i) {
        rows_[i] = i;
    }

}

MatrixS::MatrixS(const size_type& size) :sizeX_(size.second), sizeY_(size.first), matrix_(ArrayT<int>(size.second* size.first)), rows_(ArrayT<ptrdiff_t>(size.first))
{
    for (ptrdiff_t i = 1; i < size.first; ++i) {
        rows_[i] = i;
    }
}


int& MatrixS::at(const ptrdiff_t& indY, const ptrdiff_t& indX) {
    indexCheck({ indY,indX });
    return  matrix_[rows_[indY] * sizeX_ + indX];
}

int MatrixS::at(const ptrdiff_t& indY, const ptrdiff_t& indX) const {
    indexCheck({ indY,indX });
    return  matrix_[rows_[indY] * sizeX_ + indX];
}

int& MatrixS::at(const size_type& elem)
{
    indexCheck(elem);
    return matrix_[rows_[elem.first] * sizeX_ + elem.second];
}

int MatrixS::at(const size_type& elem) const
{
    indexCheck(elem);
    return matrix_[rows_[elem.first] * sizeX_ + elem.second];
}

void MatrixS::swapRows(const ptrdiff_t& lhs, const ptrdiff_t& rhs) {
    swap(rows_[lhs], rows_[rhs]);
}

void MatrixS::swapRows(const size_type& elem) {
    swap(rows_[elem.first], rows_[elem.second]);
}

void MatrixS::resize(const size_type& elem)
{
    return resize(elem.first, elem.second);
}

void MatrixS::resize(const ptrdiff_t& sizeYNew, const ptrdiff_t& sizeXNew) {
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


void MatrixS::indexCheck(const size_type& elem) const {
    if (elem.first < 0 || elem.first >= sizeY_) {
        throw std::out_of_range("Index is out of range of array");
    }
    if (elem.second < 0 || elem.second >= sizeX_) {
        throw std::out_of_range("Index is out of range of array");
    }
}
