

#include<arrayt/arrayt.hpp>


#ifndef MATRIX_HPP_2023
#define MATRIX_HPP_2023

class MatrixS {

    using SizeType = std::tuple<std::ptrdiff_t, std::ptrdiff_t>;

public:
    MatrixS() = default;
    MatrixS(const MatrixS& rhs) = default;
    MatrixS(MatrixS&& rhs) = default;
    MatrixS(const std::ptrdiff_t, const std::ptrdiff_t);
    explicit MatrixS(const SizeType&);
    ~MatrixS() = default;

    MatrixS& operator=(const MatrixS& rhs) = default;
    MatrixS& operator=(MatrixS&& rhs) = default;

    [[nodiscard]] int& at(const std::ptrdiff_t, const std::ptrdiff_t);
    [[nodiscard]] const int& at(const std::ptrdiff_t, const std::ptrdiff_t) const;

    [[nodiscard]] int& at(const SizeType&);
    [[nodiscard]] const int& at(const SizeType&) const;


    [[nodiscard]]  std::ptrdiff_t nCols() noexcept { return size_x_; }
    [[nodiscard]]  std::ptrdiff_t nRows() noexcept { return size_y_; }
    [[nodiscard]]  const SizeType& ssize() const noexcept  {return { size_y_ ,size_x_ };}

    void resize(const std::ptrdiff_t, const std::ptrdiff_t);
    void resize(const SizeType&);

    void swapRows(const std::ptrdiff_t, const std::ptrdiff_t);
    void swapRows(const SizeType&);

    

    
    


private:
    void indexCheck(const SizeType&) const;


    ArrayT<int> matrix_;
    ArrayT<std::ptrdiff_t> rows_;
    
    std::ptrdiff_t size_x_ = 0;
    std::ptrdiff_t size_y_ = 0;

};














#endif