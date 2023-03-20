#include "include/matrixs/matrixs.hpp"
MatrixS::MatrixS(const ptrdiff_t& sizeY, const ptrdiff_t& sizeX) : sizeX_(sizeX), sizeY_(sizeY), matrix_(ArrayUni<int>(sizeX* sizeY)), rows_(ArrayUni<ptrdiff_t>(sizeY)) {
	for (ptrdiff_t i = 1; i < sizeY_; ++i) {
		rows_[i] = i;
	}

}


int& MatrixS::at(const ptrdiff_t& indY, const ptrdiff_t& indX) {
	return  matrix_[rows_[indY] * sizeX_ + indX];
}

int MatrixS::at(const ptrdiff_t& indY, const ptrdiff_t& indX) const {
	return  matrix_[rows_[indY] * sizeX_ + indX];
}

void MatrixS::swapRows(const ptrdiff_t& lhs, const ptrdiff_t& rhs) {
	swap(rows_[lhs], rows_[rhs]);
}

void MatrixS::resize(const ptrdiff_t& sizeYNew, const ptrdiff_t& sizeXNew) {
	ptrdiff_t sizeXSmall = (sizeXNew < sizeX_) ? sizeXNew : sizeX_;
	ptrdiff_t sizeYSmall = (sizeYNew < sizeY_) ? sizeYNew : sizeY_;
	ArrayUni<int> newMatrix = ArrayUni<int>(sizeXNew * sizeYNew);


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
