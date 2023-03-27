#ifndef RATIONAL_RATIONAL_HPP_20230215
#define RATIONAL_RATIONAL_HPP_20230215

#include <iosfwd>
#include <cstdint>

int64_t gcd(int64_t a, int64_t b) noexcept;

class Rational {
public:
    Rational() = default;
    Rational(Rational&&) = default;
    Rational(const Rational& rhs) = default;
    Rational(const int64_t& num, const int64_t& denum);
    Rational(const int64_t& num) noexcept;
    ~Rational() = default;
    Rational& operator=(const Rational& rhs) = default;
    Rational& operator=(Rational&& rhs) = default;

    explicit operator bool() const noexcept;

    Rational& operator+=(const Rational& rhs) noexcept;
    Rational& operator-=(const Rational& rhs) noexcept;
    Rational& operator*=(const Rational& rhs) noexcept;
    Rational& operator/=(const Rational& rhs);

    Rational& operator++() noexcept;
    Rational& operator--() noexcept;
    Rational operator++(int pref) noexcept;
    Rational operator--(int pref) noexcept;
    std::istream& readIn(std::istream& istrm) noexcept;
    std::ostream& writeOut(std::ostream& ostrm) const noexcept;
    int64_t num() const noexcept;
    int64_t denom() const noexcept;

private:
    int64_t num_ = 0;
    int64_t denum_ = 1;
    Rational& normalize() noexcept;
    static const char separator = '/';


};



bool operator> (const Rational& lhs, const Rational& rhs)noexcept;
bool operator== (const Rational& lhs, const Rational& rhs) noexcept;
bool operator!= (const Rational& lhs, const Rational& rhs) noexcept;
bool operator>= (const Rational& lhs, const Rational& rhs) noexcept;
bool operator< (const Rational& lhs, const Rational& rhs) noexcept;
bool operator<= (const Rational& lhs, const Rational& rhs) noexcept;


std::ostream& operator<<(std::ostream& ostrm, const Rational& rhs) noexcept;
std::istream& operator>>(std::istream& istrm, Rational& rhs) noexcept;

Rational operator-(const Rational& lhs) noexcept;
Rational operator+ (const Rational& lhs, const Rational& rhs) noexcept;
Rational operator- (const Rational& lhs, const Rational& rhs) noexcept;
Rational operator* (const Rational& lhs, const Rational& rhs) noexcept;
Rational operator/ (const Rational& lhs, const Rational& rhs);

#endif
