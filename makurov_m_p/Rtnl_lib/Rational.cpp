﻿#include "Rational.hpp"
#include<iostream>

int64_t gcd(int64_t a, int64_t b) noexcept {
	int64_t c;
	while (b != 0) {
		a = a % b;
		c = a;
		a = b;
		b = c;
	}
	return a;
}


Rational& Rational::normalize()noexcept {
	int64_t a = gcd(denum_, num_);
	denum_ /= a;
	num_ /= a;
	if ((denum_) < 0) {
		denum_ *= -1;
		num_ *= -1;
	}
	return *this;
}

const int64_t& Rational::getNum() const noexcept {
	return num_;
}

const int64_t& Rational::getDenum() const noexcept {
	return denum_;
}


Rational::Rational(const int64_t& num, const int64_t& denum)
	: num_(num)
	, denum_(denum)
{
	if (denum_ == 0)
		throw "Division by zero";
	normalize();
}
Rational::Rational(const int64_t& num) noexcept
	: num_(num)
	, denum_(1)
{
}

Rational::operator bool() const noexcept{
	return num_!=0;
}


bool operator== (const Rational& lhs, const Rational& rhs) noexcept {
	return lhs.getNum()==rhs.getNum() && lhs.getDenum() == rhs.getDenum();
}
bool operator> (const Rational& lhs, const Rational& rhs) noexcept {
	return  lhs.getNum()*rhs.getDenum() > rhs.getNum()*lhs.getDenum();
}


bool operator!= (const Rational& lhs, const Rational& rhs)noexcept {
	return !(lhs==rhs);
}
bool operator>=(const Rational& lhs, const Rational& rhs) noexcept {
	return lhs==rhs || lhs>rhs;
}
bool operator<(const Rational& lhs, const Rational& rhs) noexcept {
	return !(lhs>=rhs);
}
bool operator<=(const Rational& lhs, const Rational& rhs) noexcept {
	return !(lhs>rhs);
}


Rational& Rational::operator+=(const Rational& rhs) noexcept {
	if (&rhs == this) {
		if (denum_ % 2 == 1)
			num_ *= 2;
		else
			denum_ /= 2;
		return *this;
	}
	num_ *= rhs.denum_;
	num_ += rhs.num_ * denum_;
	denum_ *= rhs.denum_;
	normalize();
	return *this;
}
Rational& Rational::operator-=(const Rational& rhs) noexcept {
	if (&rhs == this) {
		num_ = 0;
		denum_ = 1;
		return *this;
	}
	num_ *= rhs.denum_;
	num_ -= rhs.num_ * denum_;
	denum_ *= rhs.denum_;
	normalize();
	return *this;
}


Rational& Rational::operator/=(const Rational& rhs) {

	if (rhs == 0)
		throw ("Division by zero");
	if (&rhs == this) {
		num_ = 1;
		denum_ = 1;
		return *this;
	}
	num_ *= rhs.denum_;
	denum_ *= rhs.num_;
	normalize();
	return *this;
}
Rational& Rational::operator*=(const Rational& rhs) noexcept {
	num_ *= rhs.num_;
	denum_ *= rhs.denum_;
	normalize();
	return *this;
}


Rational operator-(const Rational& lhs) noexcept {
	return { -lhs.getNum(),lhs.getDenum() };
}

Rational operator+ (const Rational& lhs, const Rational& rhs) noexcept {
	Rational res(lhs);
	res += rhs;
	return res;
}
Rational operator- (const Rational& lhs, const Rational& rhs) noexcept {
	Rational res(lhs);
	res -= rhs;
	return res;
}

Rational operator* (const Rational& lhs, const Rational& rhs)noexcept {
	Rational res(lhs);
	res *= rhs;
	return res;
}
Rational operator/ (const Rational& lhs, const Rational& rhs) {
	Rational res(lhs);
	res /= rhs;
	return res;
}





std::ostream& Rational::writeOut(std::ostream& ostrm) const noexcept {
	ostrm << num_ << separator << denum_;
	return ostrm;
}
std::istream& Rational::readIn(std::istream& istrm) noexcept {
	int64_t num = 0, denum = 0;
	char div = ' ';
	istrm >> num >> div >> denum;
	if (istrm.good()) {
		if (div == Rational::separator && denum>0) {
			denum_ = denum;
			num_ = num;
			normalize();
		}
		else {
			istrm.setstate(std::ios_base::failbit);
		}
	}

	return istrm;
}

Rational& Rational::operator++() noexcept {
	*this += 1;
	return(*this);
}
Rational& Rational::operator--() noexcept {
	*this -= 1;
	return(*this);
}


Rational Rational::operator++(int pref) noexcept {
	Rational res = *this;
	++(*this);
	return res;
}
Rational Rational::operator--(int pref) noexcept {
	Rational res = *this;
	--(*this);
	return res;
}


std::ostream& operator<<(std::ostream& ostrm, const Rational& rhs) noexcept {
	return rhs.writeOut(ostrm);
}

std::istream& operator>>(std::istream& istrm, Rational& rhs) noexcept {
	return rhs.readIn(istrm);
}