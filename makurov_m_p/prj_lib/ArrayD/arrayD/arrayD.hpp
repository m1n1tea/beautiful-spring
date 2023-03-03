


#include <cstdint>
#include <stdexcept>

template<typename T>
void swap(T& a, T& b) noexcept;
void sizeCheck(const ptrdiff_t& size);



template<typename type>
class ArrayD {
public:
	ArrayD() = default;
	ArrayD(const ArrayD& rhs) noexcept;
	ArrayD(ArrayD&& rhs) noexcept;
	ArrayD(const ArrayD& rhs, const ptrdiff_t& begin, const ptrdiff_t& end);
	ArrayD(ArrayD&& rhs, const ptrdiff_t& begin, const ptrdiff_t& end);
	ArrayD(const ptrdiff_t& size);
	ArrayD(const ptrdiff_t& size, const type& common);
	~ArrayD() noexcept;
	ArrayD& operator=(const ArrayD& rhs) noexcept;
	ArrayD& operator=(ArrayD&& rhs) noexcept;
	type& operator[](const ptrdiff_t& index);
	const type& operator[](const ptrdiff_t& index) const;
	ptrdiff_t size() const noexcept;
	ptrdiff_t capacity() const noexcept;


	void insert(const ptrdiff_t& index, const ptrdiff_t& addsize, const type& common);
	void insert(const ptrdiff_t& index, const ArrayD& rhs, const ptrdiff_t& begin, const ptrdiff_t& end);
	void insert(const ptrdiff_t& index, ArrayD&& rhs, const ptrdiff_t& begin, const ptrdiff_t& end);
	void erase(const ptrdiff_t& index, const ptrdiff_t& removesize);
	void reserve(const ptrdiff_t& size);


	void insert(const ptrdiff_t& index, const ArrayD& rhs);
	void insert(const ptrdiff_t& index, ArrayD&& rhs);
	void resize(const ptrdiff_t& newsize);
	void resize(const ptrdiff_t& newsize, const type& common);
	ArrayD subArray(const ptrdiff_t& begin, const ptrdiff_t& end);
	void insert(const ptrdiff_t& index, const ptrdiff_t& addsize) = delete;
	void insert(const ptrdiff_t& index) = delete;
	void insert(const ptrdiff_t& index, const type& common);
	void push_back(const type& common);
	void erase(const ptrdiff_t& index);
	void pop_back();
	bool empty();



private:
	void indexCheck(const ptrdiff_t& index) const;
	type* arr_ = nullptr;
	ptrdiff_t arraySize_ = 0;
	ptrdiff_t arrayCap_ = 0;
	static const type commonElement = type();
	static const ptrdiff_t capMltpr = 2;

};





template<typename type>
ArrayD<type>::ArrayD(const ptrdiff_t& size) :arraySize_(size), arrayCap_(size) {
	sizeCheck(size);
	arr_ = new type[arraySize_];
	for (ptrdiff_t i = 0; i < arraySize_; ++i) {
		arr_[i] = commonElement;
	}
}

template<typename type>
ArrayD<type>::ArrayD(const ptrdiff_t& size, const type& common) :arraySize_(size), arrayCap_(size) {
	sizeCheck(size);
	arr_ = new type[arraySize_];
	for (ptrdiff_t i = 0; i < arraySize_; ++i) {
		arr_[i] = common;
	}
}


template<typename type>
ArrayD<type>::ArrayD(const ArrayD<type>& rhs)noexcept :arraySize_(rhs.arraySize_), arrayCap_(rhs.arrayCap_) {
	if (arrayCap_ > 0) {
		arr_ = new type[arrayCap_];
		for (ptrdiff_t i = 0; i < arraySize_; ++i) {
			arr_[i] = rhs.arr_[i];
		}
	}
}

template<typename type>
ArrayD<type>::ArrayD(ArrayD<type>&& rhs)noexcept :arraySize_(rhs.arraySize_), arrayCap_(rhs.arrayCap_), arr_(std::move(rhs.arr_)) {
	rhs.arr_ = nullptr;
}


template<typename type>
ArrayD<type>::ArrayD(const ArrayD<type>& rhs, const ptrdiff_t& begin, const ptrdiff_t& end) :arraySize_(end - begin), arrayCap_(end - begin) {
	sizeCheck(arraySize_);
	rhs.indexCheck(begin);
	rhs.indexCheck(end);
	if (arrayCap_ > 0) {
		arr_ = new type[arrayCap_];
		for (ptrdiff_t i = begin; i < end; ++i) {
			arr_[i - begin] = rhs.arr_[i];
		}
	}
}

template<typename type>
ArrayD<type>::ArrayD(ArrayD<type>&& rhs, const ptrdiff_t& begin, const ptrdiff_t& end) :arraySize_(rhs.arraySize_), arrayCap_(rhs.arrayCap_), arr_(std::move(rhs.arr_)) {
	sizeCheck(end - begin);
	rhs.indexCheck(begin);
	rhs.indexCheck(end);
	rhs.arr_ = nullptr;
	erase(end, rhs.size() - end);
	erase(0, begin);
}






template<typename type>
ArrayD<type>::~ArrayD() noexcept {
	delete[] arr_;
};





template<typename type>
ArrayD<type>& ArrayD<type>::operator=(const ArrayD& rhs) noexcept
{
	if (this == &rhs)
		return *this;
	ArrayD tmp(rhs);
	swap(*this, tmp);
	return *this;
}

template<typename type>
ArrayD<type>& ArrayD<type>::operator=(ArrayD&& rhs) noexcept
{
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

void sizeCheck(const ptrdiff_t& size) {
	if (size < 0) {
		throw std::invalid_argument("Negative size or negative change in size of array");
	}
}

template<typename type>
void ArrayD<type>::indexCheck(const ptrdiff_t& index) const {
	if (index < 0 || index >= arraySize_) {
		throw std::out_of_range("Index is out of range of array");
	}
}


template<typename type>
type& ArrayD<type>::operator[](const ptrdiff_t& index)
{
	indexCheck(index);
	return arr_[index];
}

template<typename type>
const type& ArrayD<type>::operator[](const ptrdiff_t& index) const
{
	indexCheck(index);
	return arr_[index];
}


template<typename type>
ptrdiff_t ArrayD<type>::size() const noexcept
{
	return arraySize_;
}

template<typename type>
ptrdiff_t ArrayD<type>::capacity() const noexcept
{
	return arrayCap_;
}

template<typename type>
void ArrayD<type>::insert(const ptrdiff_t& index, const ptrdiff_t& addsize, const type& common)
{
	if (addsize == 0)
		return;

	++arraySize_;
	indexCheck(index);
	--arraySize_;

	sizeCheck(addsize);
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
void ArrayD<type>::insert(const ptrdiff_t& index, const ArrayD<type>& rhs, const ptrdiff_t& begin, const ptrdiff_t& end)
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
void ArrayD<type>::insert(const ptrdiff_t& index, ArrayD<type>&& rhs, const ptrdiff_t& begin, const ptrdiff_t& end)
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
			newArray[i] = std::move(rhs.arr_[i - index + begin]);
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
			arr_[i] = std::move(rhs.arr_[i - index + begin]);
		}

	}
	arraySize_ += addsize;
}

template<typename type>
void ArrayD<type>::erase(const ptrdiff_t& index, const ptrdiff_t& removesize)
{
	if (removesize == 0)
		return;
	sizeCheck(removesize);
	indexCheck(index);
	indexCheck(index + removesize - 1);

	for (ptrdiff_t i = index + removesize; i < arraySize_; ++i) {
		arr_[i - removesize] = std::move(arr_[i]);
	}
	for (ptrdiff_t i = arraySize_ - removesize; i < index + removesize; ++i) {
		arr_[i].~type();
	}
	arraySize_ -= removesize;
}

template<typename type>
void ArrayD<type>::reserve(const ptrdiff_t& size)
{
	sizeCheck(size);
	if (size > arrayCap_) {
		arrayCap_ = size;
		type* newArray = new type[arrayCap_];
		for (ptrdiff_t i = 0; i < arraySize_; ++i) {
			newArray[i] = arr_[i];
		}
		delete[] arr_;
		arr_ = std::move(newArray);
	}

}

template<typename type>
ArrayD<type> ArrayD<type>::subArray(const ptrdiff_t& begin, const ptrdiff_t& end) {
	return ArrayD<type>(*this, begin, end);
}


template<typename type>
void ArrayD<type>::resize(const ptrdiff_t& newSize, const type& common)
{
	sizeCheck(newSize);
	if (newSize == arraySize_)
		return;
	if (newSize > arraySize_) {
		insert(size(), newSize - arraySize_, common);
	}
	else {
		erase(newSize,size()- newSize);
	}


}

template<typename type>
void ArrayD<type>::resize(const ptrdiff_t& newsize) {
	resize(newsize, commonElement);
}


template<typename type>
void ArrayD<type>::insert(const ptrdiff_t& index, const ArrayD& rhs) {
	insert(index, rhs, 0, rhs.size());
}

template<typename type>
void ArrayD<type>::insert(const ptrdiff_t& index, ArrayD&& rhs) {
	insert(index, rhs, 0, rhs.size());
}


template<typename type>
void ArrayD<type>::insert(const ptrdiff_t& index, const type& common) {
	insert(index, 1, common);
}

template<typename type>
void ArrayD<type>::push_back(const type& common) {
	insert(size(), 1, common);
}

template<typename type>
void ArrayD<type>::erase(const ptrdiff_t& index) {
	erase(index, 1);
}

template<typename type>
void ArrayD<type>::pop_back() {
	erase(size() - 1, 1);
}

template<typename type>
bool ArrayD<type>::empty()
{
	return arraySize_ != 0;
}



