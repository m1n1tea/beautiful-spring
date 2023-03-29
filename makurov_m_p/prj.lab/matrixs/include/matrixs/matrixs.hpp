




#ifndef ARRAY_DYNAMIC_HPP_2023
#define ARRAY_DYNAMIC_HPP_2023

#include <cstdint>
#include <stdexcept>
#include<malloc.h>
template<typename T>
void swap(T& a, T& b) noexcept;
inline void sizeCheck(const ptrdiff_t& size);




template<typename type>
class ArrayT {
public:
    ArrayT() = default;
    ArrayT(const ArrayT& rhs) noexcept;
    ArrayT(ArrayT&& rhs) noexcept;
    explicit ArrayT(const ptrdiff_t& size);
    ArrayT(const ptrdiff_t& size, const type& common);
    ~ArrayT() noexcept;
    ArrayT& operator=(const ArrayT& rhs) noexcept;
    ArrayT& operator=(ArrayT&& rhs) noexcept;
    [[nodiscard]] type& operator[](const ptrdiff_t& index);
    [[nodiscard]] const type& operator[](const ptrdiff_t& index) const;
    [[nodiscard]] ptrdiff_t ssize() const noexcept;
    [[nodiscard]] ptrdiff_t capacity() const noexcept;


    void insert(const ptrdiff_t& index, const ptrdiff_t& addsize, const type& common);
    void insert(const ptrdiff_t& index, const ArrayT& rhs, const ptrdiff_t& begin, const ptrdiff_t& end);
    void erase(const ptrdiff_t& index, const ptrdiff_t& removesize);
    void reserve(const ptrdiff_t& size);


    void insert(const ptrdiff_t& index, const ArrayT& rhs);
    void resize(const ptrdiff_t& newsize);
    void resize(const ptrdiff_t& newsize, const type& common);
    void insert(const ptrdiff_t& index, const type& common);
    void pushBack(const type& common);
    void remove(const ptrdiff_t& index);
    void popBack();
    bool empty();



private:
    void indexCheck(const ptrdiff_t& index) const;
    type* arr_ = nullptr;
    ptrdiff_t arraySize_ = 0;
    ptrdiff_t arrayCap_ = 0;
    inline static const type commonElement = type();
    inline static type trash_can = type();
    static const ptrdiff_t capMltpr = 2;
};





template<typename type>
ArrayT<type>::ArrayT(const ptrdiff_t& size) :arraySize_(size), arrayCap_(size) {
    sizeCheck(size + 1);
    arr_ = new type[arraySize_];
    for (ptrdiff_t i = 0; i < arraySize_; ++i) {
        arr_[i] = commonElement;
    }
}

template<typename type>
ArrayT<type>::ArrayT(const ptrdiff_t& size, const type& common) :arraySize_(size), arrayCap_(size) {
    sizeCheck(size + 1);
    arr_ = new type[arraySize_];
    for (ptrdiff_t i = 0; i < arraySize_; ++i) {
        arr_[i] = common;
    }
}


template<typename type>
ArrayT<type>::ArrayT(const ArrayT<type>& rhs)noexcept :arraySize_(rhs.arraySize_), arrayCap_(rhs.arrayCap_) {
    if (arrayCap_ > 0) {
        arr_ = new type[arrayCap_];
        for (ptrdiff_t i = 0; i < arraySize_; ++i) {
            arr_[i] = rhs.arr_[i];
        }
    }
}

template<typename type>
ArrayT<type>::ArrayT(ArrayT<type>&& rhs)noexcept :arraySize_(rhs.arraySize_), arrayCap_(rhs.arrayCap_), arr_(std::move(rhs.arr_)) {
    rhs.arr_ = nullptr;
}









template<typename type>
ArrayT<type>::~ArrayT() noexcept {
    delete[] arr_;
    arr_ = nullptr;
};





template<typename type>
ArrayT<type>& ArrayT<type>::operator=(const ArrayT& rhs) noexcept
{
    if (arr_ == rhs.arr_)
        return *this;
    ArrayT tmp(rhs);
    swap(*this, tmp);
    return *this;
}

template<typename type>
ArrayT<type>& ArrayT<type>::operator=(ArrayT&& rhs) noexcept
{
    if (arr_ == rhs.arr_)
        return *this;
    delete[] arr_;
    arr_ = std::move(rhs.arr_);
    rhs.arr_ = nullptr;
    arrayCap_ = rhs.arrayCap_;
    arraySize_ = rhs.arraySize_;
    return *this;
}











template<typename T>
void swap(T& a, T& b) noexcept {
    T tmp(std::move(a));
    a = std::move(b);
    b = std::move(tmp);
}

inline void sizeCheck(const ptrdiff_t& size) {
    if (size <= 0) {
        throw std::invalid_argument("Array size or change in size is not positive");
    }
}

template<typename type>
void ArrayT<type>::indexCheck(const ptrdiff_t& index) const {
    if (index < 0 || index >= arraySize_) {
        throw std::out_of_range("Index is out of range of array");
    }
}


template<typename type>
type& ArrayT<type>::operator[](const ptrdiff_t& index)
{
    indexCheck(index);
    return arr_[index];
}

template<typename type>
const type& ArrayT<type>::operator[](const ptrdiff_t& index) const
{
    indexCheck(index);
    return arr_[index];
}


template<typename type>
ptrdiff_t ArrayT<type>::ssize() const noexcept
{
    return arraySize_;
}

template<typename type>
ptrdiff_t ArrayT<type>::capacity() const noexcept
{
    return arrayCap_;
}

template<typename type>
void ArrayT<type>::insert(const ptrdiff_t& index, const ptrdiff_t& addsize, const type& common)
{
    if (addsize == 0)
        return;

    ++arraySize_;
    indexCheck(index);
    --arraySize_;

    sizeCheck(addsize + 1);
    if (addsize + arraySize_ > arrayCap_) {
        arrayCap_ = addsize + arraySize_;
        arrayCap_ *= capMltpr;
        type* newArray = new type[arrayCap_];
        for (ptrdiff_t i = 0; i < index; ++i) {
            newArray[i] = std::move(arr_[i]);
        }
        for (ptrdiff_t i = index; i < index + addsize; ++i) {
            newArray[i] = common;
        }

        for (ptrdiff_t i = index + addsize; i < addsize + arraySize_; ++i) {
            newArray[i] = std::move(arr_[i - addsize]);
        }
        delete[] arr_;
        arr_ = std::move(newArray);
    }
    else {

        for (ptrdiff_t i = index + addsize; i < addsize + arraySize_; ++i) {
            arr_[i] = std::move(arr_[i - addsize]);
        }
        for (ptrdiff_t i = index; i < index + addsize; ++i) {
            arr_[i] = common;
        }

    }
    arraySize_ += addsize;
}


template<typename type>
void ArrayT<type>::insert(const ptrdiff_t& index, const ArrayT<type>& rhs, const ptrdiff_t& begin, const ptrdiff_t& end)
{
    ++arraySize_;
    indexCheck(index);
    --arraySize_;
    rhs.indexCheck(begin);
    rhs.indexCheck(end - 1);

    const ptrdiff_t addsize = end - begin;
    if (addsize == 0)
        return;

    if (addsize + arraySize_ > arrayCap_) {
        arrayCap_ = addsize + arraySize_;
        arrayCap_ *= capMltpr;
        type* newArray = new type[arrayCap_];
        for (ptrdiff_t i = 0; i < index; ++i) {
            newArray[i] = std::move(arr_[i]);
        }
        for (ptrdiff_t i = index; i < index + addsize; ++i) {
            newArray[i] = (rhs.arr_[i - index + begin]);
        }

        for (ptrdiff_t i = index + addsize; i < addsize + arraySize_; ++i) {
            newArray[i] = std::move(arr_[i - addsize]);
        }
        delete[] arr_;
        arr_ = std::move(newArray);
    }
    else {

        for (ptrdiff_t i = index + addsize; i < addsize + arraySize_; ++i) {
            arr_[i] = std::move(arr_[i - addsize]);
        }
        for (ptrdiff_t i = index; i < index + addsize; ++i) {
            arr_[i] = rhs.arr_[i - index + begin];
        }

    }
    arraySize_ += addsize;
}



template<typename type>
void ArrayT<type>::erase(const ptrdiff_t& index, const ptrdiff_t& removesize)
{
    if (removesize == 0)
        return;
    sizeCheck(removesize);
    sizeCheck(arraySize_ - removesize);
    indexCheck(index);
    indexCheck(index + removesize - 1);

    for (ptrdiff_t i = index + removesize; i < arraySize_; ++i) {
        arr_[i - removesize] = std::move(arr_[i]);
    }
    for (ptrdiff_t i = arraySize_ - removesize; i < index + removesize; ++i) {
        trash_can = std::move(arr_[i]);
    }
    arraySize_ -= removesize;
}

template<typename type>
void ArrayT<type>::reserve(const ptrdiff_t& size)
{
    if (size <= arrayCap_)
        return;
    arrayCap_ = size;
    type* newArray = new type[arrayCap_];
    for (ptrdiff_t i = 0; i < arraySize_; ++i) {
        newArray[i] = arr_[i];
    }
    delete[] arr_;
    arr_ = std::move(newArray);

}



template<typename type>
void ArrayT<type>::resize(const ptrdiff_t& newSize, const type& common)
{
    sizeCheck(newSize);
    if (newSize == arraySize_)
        return;
    if (newSize > arraySize_) {
        insert(ssize(), newSize - arraySize_, common);
    }
    else {
        erase(newSize, ssize() - newSize);
    }


}

template<typename type>
void ArrayT<type>::resize(const ptrdiff_t& newsize) {
    resize(newsize, commonElement);
}


template<typename type>
void ArrayT<type>::insert(const ptrdiff_t& index, const ArrayT& rhs) {
    insert(index, rhs, 0, rhs.ssize());
}



template<typename type>
void ArrayT<type>::insert(const ptrdiff_t& index, const type& common) {
    insert(index, 1, common);
}

template<typename type>
void ArrayT<type>::pushBack(const type& common) {
    insert(ssize(), 1, common);
}

template<typename type>
void ArrayT<type>::remove(const ptrdiff_t& index) {
    erase(index, 1);
}

template<typename type>
void ArrayT<type>::popBack() {
    erase(ssize() - 1, 1);
}

template<typename type>
bool ArrayT<type>::empty()
{
    return arraySize_ != 0;
}


#endif


#ifndef MATRIX_HPP_2023
#define MATRIX_HPP_2023

class MatrixS {

    using SizeType = std::tuple<std::ptrdiff_t, std::ptrdiff_t>;

public:
    MatrixS() = default;
    MatrixS(const MatrixS& rhs) = default;
    MatrixS(MatrixS&& rhs) = default;
    MatrixS(const ptrdiff_t, const ptrdiff_t);
    explicit MatrixS(const SizeType&);
    ~MatrixS() = default;

    MatrixS& operator=(const MatrixS& rhs) = default;
    MatrixS& operator=(MatrixS&& rhs) = default;

    [[nodiscard]] int& at(const ptrdiff_t, const ptrdiff_t);
    [[nodiscard]] const int& at(const ptrdiff_t, const ptrdiff_t) const;

    [[nodiscard]] int& at(const SizeType&);
    [[nodiscard]] const int& at(const SizeType&) const;


    ptrdiff_t nCols() noexcept { return sizeX_; }
    ptrdiff_t nRows() noexcept { return sizeY_; }
    const SizeType& ssize() const noexcept  {return { sizeY_ ,sizeX_ };}

    void resize(const ptrdiff_t, const ptrdiff_t);
    void resize(const SizeType&);

    void swapRows(const ptrdiff_t, const ptrdiff_t);
    void swapRows(const SizeType&);

    

    
    


private:
    void indexCheck(const SizeType&) const;


    ArrayT<int> matrix_;
    ArrayT<ptrdiff_t> rows_;
    
    ptrdiff_t sizeX_ = 0;
    ptrdiff_t sizeY_ = 0;

};









#endif