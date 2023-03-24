#include <rational/rational.hpp>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
TEST_CASE("[rational] - Rational ctor") {
	REQUIRE((Rational() == Rational(0, 1)));
	REQUIRE((Rational(3) == Rational(3, 1)));
	REQUIRE((Rational(4, 8) == Rational(1, 2)));
	REQUIRE((Rational(-4, 8) == Rational(-1, 2)));
	REQUIRE((Rational(8, 4) == Rational(2, 1)));
}
TEST_CASE("[rational] - ariphmetic") {
	REQUIRE((Rational(1, 2) + Rational(1, 4) == Rational(3, 4)));
	REQUIRE((Rational(1, -2) + Rational(-1, 4) == Rational(-3, 4)));
	REQUIRE((Rational(4, 2) + Rational(1, 4) == Rational(9, 4)));
	REQUIRE((Rational(1, 2) += Rational(1, 4) == Rational(3, 4)));
	REQUIRE((Rational(1, 2) - Rational(1, 4) == Rational(1, 4)));
	REQUIRE((Rational(1, 2) -= Rational(1, 4) == Rational(1, 4)));
	REQUIRE((Rational(1, 2) * Rational(1, 4) == Rational(1, 8)));
	REQUIRE(((Rational(1, 3) *= Rational(1, 4)) == Rational(1, 12)));
	REQUIRE((Rational(1, 2) / Rational(1, 4) == Rational(2, 1)));
	REQUIRE(((Rational(1, 2) /= Rational(1, 4)) == Rational(2, 1)));
	Rational a(1, 2);
	a++;
	REQUIRE((a == Rational(3, 2)));
	a--;
	REQUIRE((a == Rational(1, 2)));
	REQUIRE((++Rational(1, 2) == Rational(3, 2)));
	REQUIRE((--Rational(1, 2) == Rational(-1, 2)));
}
TEST_CASE("[rational] - compare")
{
	REQUIRE((Rational(1, 2) > Rational(1, 4) == true));
	REQUIRE((Rational(1, 2) < Rational(1, 4) == false));
	REQUIRE((Rational(1, 2) > Rational(8, 4) == false));
	REQUIRE((Rational(1, 2) < Rational(8, 4) == true));
	REQUIRE((Rational(1, 2) >= Rational(1, 4) == true));
	REQUIRE((Rational(1, 2) <= Rational(1, 4) == false));
	REQUIRE((Rational(1, 2) >= Rational(4, 8) == true));
	REQUIRE_FALSE((Rational(1, 2) <= Rational(4, 8) == false));
	REQUIRE((Rational(1, 2) == Rational(4, 8) == true));
	REQUIRE((Rational(1, 2) != Rational(4, 8) == false));
	REQUIRE((Rational(1, 2) == Rational(6, 8) == false));
	REQUIRE((Rational(1, 2) != Rational(6, 8) == true));
}
/*
TEST_CASE("[rational] - input_and_output") {
Rational a;
std::stringstream is("-7/3");
is >> a ;
std::stringstream os;
os << a;
CHECK(os.str() == "-7/3");
std::stringstream is1("-7 /3");
is1 >> a ;
std::stringstream os1;
os1 << a ;
CHECK_FALSE(os1.str() == "-7/3");
std::stringstream is2("4%6");
is2 >> a ;
std::stringstream os2;
os2 << a ;
CHECK_FALSE(os2.str() == "2/3");
std::stringstream is5("o/-4");
is5 >> a;
std::ostringstream os5;
os5 << a;
CHECK_FALSE(os5.str() == "-2/1");
std::stringstream is6(" 5/-4");
is6 >> a;
std::ostringstream os6;
os6 << a;
CHECK_FALSE(os6.str() == "-5/4");
std::stringstream is7("5//-4");
is7 >> a;
std::ostringstream os7;
os7 << a;
CHECK_FALSE(os7.str() == "-5/4");
}*/
TEST_CASE("std::ostream &operator>>(std::ostream &ostrm, Rational &f) FALSE") {
	std::vector<std::string> v = {
	"abc",
	"5//6",
	"5/\n6",
	"5\t/\t6",
	"5\n/6",
	"-5a/r6",
	"-5/--6",
	"5/-6",
	"5 /6"
	};
	for (auto& i : v) {
		std::stringstream strm(i);
		Rational x;
		strm >> x;
		REQUIRE_FALSE(x == Rational(5, 6));
	}
}
TEST_CASE("std::istream &operator>>(std::istream &istrm, Rational &f) TRUE") {
	std::vector<std::string> v = {
	"6\n/7",
	"5/7",
	"-5/7"
	};
	std::vector<std::string> ans = {
	"0/1",
	"5/7",
	"-5/7"
	};
	for (int i = 0; i < v.size(); i++) {
		std::stringstream strm(v[i]);
		std::stringstream s;
		Rational a;
		std::string str;
		strm >> a;
		s << a;
		str = s.str();
		CHECK(str == ans[i]);
	}
}