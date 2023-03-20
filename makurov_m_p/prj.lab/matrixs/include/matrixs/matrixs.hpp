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









#endif