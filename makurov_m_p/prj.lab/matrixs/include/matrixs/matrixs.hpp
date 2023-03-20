#ifndef MATRIX_HPP_2023
#define MATRIX_HPP_2023

#include<arrayd/arrayd.hpp>






class MatrixS {

	using size_type = std::pair<std::ptrdiff_t, std::ptrdiff_t>;

public:
	MatrixS() = default;
	MatrixS(const MatrixS& rhs) = default;
	MatrixS(MatrixS&& rhs) = default;
	MatrixS(const ptrdiff_t&, const ptrdiff_t&);
	~MatrixS() = default;

	MatrixS& operator=(const MatrixS& rhs) = default;
	MatrixS& operator=(MatrixS&& rhs) = default;
	[[nodiscard]] int& at(const ptrdiff_t&, const ptrdiff_t&);
	[[nodiscard]] int at(const ptrdiff_t&, const ptrdiff_t&) const;


	ptrdiff_t sizeX() noexcept { return sizeX_; }
	ptrdiff_t sizeY() noexcept { return sizeY_; }

	void resize(const ptrdiff_t&, const ptrdiff_t&);
	void swapRows(const ptrdiff_t&, const ptrdiff_t&);

	
	


private:
	ArrayUni<int> matrix_;
	ArrayUni<ptrdiff_t> rows_;
	
	ptrdiff_t sizeX_ = 0;
	ptrdiff_t sizeY_ = 0;

};









#endif