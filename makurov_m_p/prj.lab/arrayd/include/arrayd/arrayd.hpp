
#ifndef ARRAY_DYNAMIC_HPP_2023
#define ARRAY_DYNAMIC_HPP_2023

#include <cstdint>
#include <stdexcept>
#include<malloc.h>
template<typename T>
void swap(T& a, T& b) noexcept;
inline void sizeCheck(const std::ptrdiff_t& size);




template<typename type>
class ArrayT {
public:
    ArrayT() = default;
    ArrayT(const ArrayT& rhs) noexcept;
    ArrayT(ArrayT&& rhs) noexcept;
    explicit ArrayT(const std::ptrdiff_t& size);
    ArrayT(const std::ptrdiff_t& size, const type& common);
    ~ArrayT() noexcept;
    ArrayT& operator=(const ArrayT& rhs) noexcept;
    ArrayT& operator=(ArrayT&& rhs) noexcept;
    [[nodiscard]] type& operator[](const std::ptrdiff_t& index);
    [[nodiscard]] const type& operator[](const std::ptrdiff_t& index) const;
    [[nodiscard]] std::ptrdiff_t ssize() const noexcept;
    [[nodiscard]] std::ptrdiff_t capacity() const noexcept;


    void insert(const std::ptrdiff_t& index, const std::ptrdiff_t& add_size, const type& common);
    void insert(const std::ptrdiff_t& index, const ArrayT& rhs, const std::ptrdiff_t& begin, const std::ptrdiff_t& end);
    void erase(const std::ptrdiff_t& index, const std::ptrdiff_t& remove_size);
    void reserve(const std::ptrdiff_t& size);


    void insert(const std::ptrdiff_t& index, const ArrayT& rhs);
    void resize(const std::ptrdiff_t& new_size);
    void resize(const std::ptrdiff_t& new_size, const type& common);
    void insert(const std::ptrdiff_t& index, const type& common);
    void pushBack(const type& common);
    void remove(const std::ptrdiff_t& index);
    void popBack();
    bool empty();



private:
    void indexCheck(const std::ptrdiff_t& index) const;
    type* arr_ = nullptr;
    std::ptrdiff_t array_size_ = 0;
    std::ptrdiff_t array_cap_ = 0;
    inline static const type common_element = type();
    inline static type trash_can = type();
    static const std::ptrdiff_t cap_mltpr = 2;
};





template<typename type>
ArrayT<type>::ArrayT(const std::ptrdiff_t& size) :array_size_(size), array_cap_(size) {
    sizeCheck(size + 1);
    arr_ = new type[array_size_];
    for (std::ptrdiff_t i = 0; i < array_size_; ++i) {
        arr_[i] = common_element;
    }
}

template<typename type>
ArrayT<type>::ArrayT(const std::ptrdiff_t& size, const type& common) :array_size_(size), array_cap_(size) {
    sizeCheck(size + 1);
    arr_ = new type[array_size_];
    for (std::ptrdiff_t i = 0; i < array_size_; ++i) {
        arr_[i] = common;
    }
}


template<typename type>
ArrayT<type>::ArrayT(const ArrayT<type>& rhs)noexcept :array_size_(rhs.array_size_), array_cap_(rhs.array_cap_) {
    if (array_cap_ > 0) {
        arr_ = new type[array_cap_];
        for (std::ptrdiff_t i = 0; i < array_size_; ++i) {
            arr_[i] = rhs.arr_[i];
        }
    }
}

template<typename type>
ArrayT<type>::ArrayT(ArrayT<type>&& rhs)noexcept :array_size_(rhs.array_size_), array_cap_(rhs.array_cap_), arr_(std::move(rhs.arr_)) {
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
    arr_ = rhs.arr_;
    rhs.arr_ = nullptr;
    array_cap_ = rhs.array_cap_;
    array_size_ = rhs.array_size_;
    return *this;
}











template<typename T>
void swap(T& a, T& b) noexcept {
    T tmp(std::move(a));
    a = std::move(b);
    b = std::move(tmp);
}

inline void sizeCheck(const std::ptrdiff_t& size) {
    if (size <= 0) {
        throw std::invalid_argument("Array size or change in size is not positive");
    }
}

template<typename type>
void ArrayT<type>::indexCheck(const std::ptrdiff_t& index) const {
    if (index < 0 || index >= array_size_) {
        throw std::out_of_range("Index is out of range of array");
    }
}


template<typename type>
type& ArrayT<type>::operator[](const std::ptrdiff_t& index)
{
    indexCheck(index);
    return arr_[index];
}

template<typename type>
const type& ArrayT<type>::operator[](const std::ptrdiff_t& index) const
{
    indexCheck(index);
    return arr_[index];
}


template<typename type>
std::ptrdiff_t ArrayT<type>::ssize() const noexcept
{
    return array_size_;
}

template<typename type>
std::ptrdiff_t ArrayT<type>::capacity() const noexcept
{
    return array_cap_;
}

template<typename type>
void ArrayT<type>::insert(const std::ptrdiff_t& index, const std::ptrdiff_t& add_size, const type& common)
{
    if (add_size == 0)
        return;

    ++array_size_;
    indexCheck(index);
    --array_size_;

    sizeCheck(add_size + 1);
    if (add_size + array_size_ > array_cap_) {
        array_cap_ = add_size + array_size_;
        array_cap_ *= cap_mltpr;
        type* newArray = new type[array_cap_];
        for (std::ptrdiff_t i = 0; i < index; ++i) {
            newArray[i] = std::move(arr_[i]);
        }
        for (std::ptrdiff_t i = index; i < index + add_size; ++i) {
            newArray[i] = common;
        }

        for (std::ptrdiff_t i = index + add_size; i < add_size + array_size_; ++i) {
            newArray[i] = std::move(arr_[i - add_size]);
        }
        delete[] arr_;
        arr_ = std::move(newArray);
    }
    else {

        for (std::ptrdiff_t i = index + add_size; i < add_size + array_size_; ++i) {
            arr_[i] = std::move(arr_[i - add_size]);
        }
        for (std::ptrdiff_t i = index; i < index + add_size; ++i) {
            arr_[i] = common;
        }

    }
    array_size_ += add_size;
}


template<typename type>
void ArrayT<type>::insert(const std::ptrdiff_t& index, const ArrayT<type>& rhs, const std::ptrdiff_t& begin, const std::ptrdiff_t& end)
{
    ++array_size_;
    indexCheck(index);
    --array_size_;
    rhs.indexCheck(begin);
    rhs.indexCheck(end - 1);

    const std::ptrdiff_t add_size = end - begin;
    if (add_size == 0)
        return;

    if (add_size + array_size_ > array_cap_) {
        array_cap_ = add_size + array_size_;
        array_cap_ *= cap_mltpr;
        type* newArray = new type[array_cap_];
        for (std::ptrdiff_t i = 0; i < index; ++i) {
            newArray[i] = std::move(arr_[i]);
        }
        for (std::ptrdiff_t i = index; i < index + add_size; ++i) {
            newArray[i] = (rhs.arr_[i - index + begin]);
        }

        for (std::ptrdiff_t i = index + add_size; i < add_size + array_size_; ++i) {
            newArray[i] = std::move(arr_[i - add_size]);
        }
        delete[] arr_;
        arr_ = std::move(newArray);
    }
    else {

        for (std::ptrdiff_t i = index + add_size; i < add_size + array_size_; ++i) {
            arr_[i] = std::move(arr_[i - add_size]);
        }
        for (std::ptrdiff_t i = index; i < index + add_size; ++i) {
            arr_[i] = rhs.arr_[i - index + begin];
        }

    }
    array_size_ += add_size;
}



template<typename type>
void ArrayT<type>::erase(const std::ptrdiff_t& index, const std::ptrdiff_t& remove_size)
{
    if (remove_size == 0)
        return;
    sizeCheck(remove_size);
    sizeCheck(array_size_ - remove_size);
    indexCheck(index);
    indexCheck(index + remove_size - 1);

    for (std::ptrdiff_t i = index + remove_size; i < array_size_; ++i) {
        arr_[i - remove_size] = std::move(arr_[i]);
    }
    for (std::ptrdiff_t i = array_size_ - remove_size; i < index + remove_size; ++i) {
        trash_can = std::move(arr_[i]);
    }
    array_size_ -= remove_size;
}

template<typename type>
void ArrayT<type>::reserve(const std::ptrdiff_t& size)
{
    if (size <= array_cap_)
        return;
    array_cap_ = size;
    type* newArray = new type[array_cap_];
    for (std::ptrdiff_t i = 0; i < array_size_; ++i) {
        newArray[i] = arr_[i];
    }
    delete[] arr_;
    arr_ = std::move(newArray);

}



template<typename type>
void ArrayT<type>::resize(const std::ptrdiff_t& new_size, const type& common)
{
    sizeCheck(new_size);
    if (new_size == array_size_)
        return;
    if (new_size > array_size_) {
        insert(ssize(), new_size - array_size_, common);
    }
    else {
        erase(new_size, ssize() - new_size);
    }


}

template<typename type>
void ArrayT<type>::resize(const std::ptrdiff_t& new_size) {
    resize(new_size, common_element);
}


template<typename type>
void ArrayT<type>::insert(const std::ptrdiff_t& index, const ArrayT& rhs) {
    insert(index, rhs, 0, rhs.ssize());
}



template<typename type>
void ArrayT<type>::insert(const std::ptrdiff_t& index, const type& common) {
    insert(index, 1, common);
}

template<typename type>
void ArrayT<type>::pushBack(const type& common) {
    insert(ssize(), 1, common);
}

template<typename type>
void ArrayT<type>::remove(const std::ptrdiff_t& index) {
    erase(index, 1);
}

template<typename type>
void ArrayT<type>::popBack() {
    erase(ssize() - 1, 1);
}

template<typename type>
bool ArrayT<type>::empty()
{
    return array_size_ != 0;
}


#endif

#ifndef ARRAY_DOUBLE_HPP_2023
#define ARRAY_DOUBLE_HPP_2023

typedef ArrayT<double> ArrayD;


#endif