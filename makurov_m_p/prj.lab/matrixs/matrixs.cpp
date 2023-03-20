#include "include/matrixs/matrixs.hpp"
MatrixS::MatrixS(const size_type& size) : sizeX_(size.second), sizeY_(size.first), matrix_(ArrayUni<int>(size.first* size.second)), rows_(ArrayUni<ptrdiff_t>(size.first)) {
	for (ptrdiff_t i = 1; i < sizeY_; ++i) {
		rows_[i] = i;
	}

}


int& MatrixS::at(const size_type elem) {
	return  *(&matrix_[0] + rows_[elem.first] * sizeX_ + elem.second);
}

int MatrixS::at(const size_type elem) const {
	return  *(&matrix_[0] + rows_[elem.first] * sizeX_ + elem.second);
}

void MatrixS::swapRows(const size_type& indexes) {
	swap(rows_[indexes.first], rows_[indexes.second]);
}

void MatrixS::resize(const size_type new_size) {
	const ptrdiff_t& sizeXNew = new_size.second;
	const ptrdiff_t& sizeYNew = new_size.first;
	ptrdiff_t sizeXSmall = (sizeXNew < sizeX_) ? sizeXNew : sizeX_;
	ptrdiff_t sizeYSmall = (sizeYNew < sizeY_) ? sizeYNew : sizeY_;
	ArrayUni<int> newMatrix = ArrayUni<int>(sizeXNew * sizeYNew);
	rows_.resize(sizeYNew);
	for (ptrdiff_t i = sizeY_; i < sizeYNew; i++)
		if (rows_[i] == 0)
			rows_[i] = i;


	for (ptrdiff_t y = 0; y < sizeYSmall; ++y) {
		for (ptrdiff_t x = 0; x < sizeXSmall; ++x) {
			*(&newMatrix[0] + rows_[y] * sizeXNew + x) = *(&matrix_[0] + rows_[y] * sizeX_ + x);
		}
	}
	swap(newMatrix, matrix_);
	sizeX_ = sizeXNew;
	sizeY_ = sizeYNew;
}
