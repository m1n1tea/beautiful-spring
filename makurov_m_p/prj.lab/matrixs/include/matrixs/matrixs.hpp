#ifndef MATRIX_HPP_2023
#define MATRIX_HPP_2023

#include<arrayd/arrayd.hpp>






class MatrixS {

	using size_type = std::pair<std::ptrdiff_t, std::ptrdiff_t>;

public:
	MatrixS() = default;
	MatrixS(const MatrixS& rhs) = default;
	MatrixS(MatrixS&& rhs) = default;
	MatrixS(const size_type& size);
	~MatrixS() = default;

	MatrixS& operator=(const MatrixS& rhs) = default;
	MatrixS& operator=(MatrixS&& rhs) = default;
	[[nodiscard]] int& at(const size_type elem);
	[[nodiscard]] int at(const size_type elem) const;


	ptrdiff_t sizeX() { return sizeX_; }
	ptrdiff_t sizeY() { return sizeY_; }

	void resize(const size_type new_size);
	void swapRows(const size_type& indexes);

	
	


private:
	ArrayUni<int> matrix_;
	ArrayUni<ptrdiff_t> rows_;
	
	ptrdiff_t sizeX_ = 0;
	ptrdiff_t sizeY_ = 0;

};

MatrixS::MatrixS(const size_type& size) : sizeX_(size.second), sizeY_(size.first), matrix_(ArrayUni<int>(size.first*size.second)), rows_(ArrayUni<ptrdiff_t>(size.first)){
	for (ptrdiff_t i = 1; i < sizeY_; ++i) {
		rows_[i] = i;
	}

}


int& MatrixS:: at(const size_type elem) {
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
			*(&newMatrix[0] + rows_[y] * sizeXNew + x)=*(&matrix_[0] + rows_[y] * sizeX_ + x);
		}
	}
	swap(newMatrix, matrix_);
	sizeX_ = sizeXNew;
	sizeY_ = sizeYNew;
}








#endif