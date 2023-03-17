#ifndef MATRIX_HPP_2023
#define MATRIX_HPP_2023

#include<arrayd/arrayd.hpp>

template<typename type>
class Matrix;

template<typename type>
class MatrixRow;


template<typename type>
class MatrixRow {
public:
	friend class Matrix<type>;
	MatrixRow() = default;
	MatrixRow(const ArrayUni<type>& rhs) : row_(rhs) {}
	MatrixRow(ArrayUni<type>&& rhs) : row_(rhs) {}
	~MatrixRow()=default;
	type& operator[](const ptrdiff_t& index) {	return row_[index];}
	const type& operator[](const ptrdiff_t& index) const { return row_[index]; }

	
	
private:
	ArrayUni<type> row_;
	

};






template<typename type>
class Matrix {
public:
	friend class MatrixRow<type>;
	Matrix() = default;
	Matrix(const Matrix<type>& rhs) = default;
	Matrix(Matrix<type>&& rhs) = default;
	Matrix(const ptrdiff_t& sizeX) : matrix_(1, ArrayUni<type>(sizeX)), sizeX_(sizeX), sizeY_(1) {}
	Matrix(const ptrdiff_t& sizeX, const ptrdiff_t& sizeY) : matrix_(sizeY, ArrayUni<type>(sizeX)), sizeX_(sizeX), sizeY_(sizeY){}
	Matrix(const ptrdiff_t& sizeX, const ptrdiff_t& sizeY, const type& common) : matrix_(sizeY, ArrayUni<type>(sizeX,common)), sizeX_(sizeX), sizeY_(sizeY) {}
	~Matrix() = default;

	Matrix& operator=(const Matrix& rhs) = default;
	Matrix& operator=(Matrix&& rhs) = default;



	MatrixRow<type>& operator[](const ptrdiff_t& index) { return matrix_[index]; }
	const MatrixRow<type>& operator[](const ptrdiff_t& index) const { return matrix_[index]; }
	ptrdiff_t sizeX() { return sizeX_; }
	ptrdiff_t sizeY() { return sizeY_; }
	ArrayUni<type> getRow(const ptrdiff_t& index) {  return matrix_[index].row_;}
	ArrayUni<type> getColumn(const ptrdiff_t& index);

	void resizeX(const ptrdiff_t& newsize);
	void resizeX(const ptrdiff_t& newsize, const type& common);
	void resizeY(const ptrdiff_t& newsize) { matrix_.resize(newsize); sizeY_ = newsize;}
	void resizeY(const ptrdiff_t& newsize, const type& common) { matrix_.resize(newsize, ArrayUni<type>(sizeX_,common)); sizeY_ = newsize;}
	void resize(const ptrdiff_t& newsizeX, const ptrdiff_t& newsizeY, const type& common) { resizeY(newsizeY, common); resizeX(newsizeX, common);}
	void resize(const ptrdiff_t& newsizeX, const ptrdiff_t& newsizeY) { resizeY(newsizeY); resizeX(newsizeX); }

	
	

private:
	ArrayUni<MatrixRow<type>> matrix_= ArrayUni<MatrixRow<type>>(1);
	ptrdiff_t sizeX_ = 0;
	ptrdiff_t sizeY_ = 1;
};


template<typename type>
void Matrix<type>::resizeX(const ptrdiff_t& newsize, const type& common) {
	for (ptrdiff_t i = 0; i < sizeY_; ++i) {
		matrix_[i].row_.resize(newsize, common);
	}
	sizeX_ = newsize;
}

template<typename type>
void Matrix<type>::resizeX(const ptrdiff_t& newsize) {
	for (ptrdiff_t i = 0; i < sizeY_; ++i) {
		matrix_[i].row_.resize(newsize);
	}
	sizeX_ = newsize;
}

template<typename type>
ArrayUni<type> Matrix<type>::getColumn(const ptrdiff_t& index) {
	ArrayUni<type> res(sizeY_);
	for (ptrdiff_t i = 0; i < sizeY_; ++i) {
		res[i] = matrix_[i][index];
	}
	return res;
}



#endif