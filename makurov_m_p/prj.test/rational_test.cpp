#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <iostream>
#include<sstream>
#include <rational/rational.hpp>
#include <doctest/doctest.h>





TEST_CASE("comparison operators") {
	CHECK(Rational ( 2, 3 ) == Rational( 2,3 ));
	CHECK(Rational( 2, 3 ) == Rational( 4,6 ));
	CHECK(Rational( -2, 3 ) == Rational( -4,6 ));
	CHECK(Rational( -2, 3 ) == Rational( 4,-6 ));
	CHECK(Rational( 2, 3 ) == Rational( -4,-6 ));
	CHECK(!(Rational( 2, 3 ) == Rational( 2,2 )));
	CHECK(Rational( 3, 3 ) > Rational( 2,3 ));
	CHECK(Rational( 2, 3 ) > Rational( 2,4 ));
	CHECK(Rational( 15,16 ) > Rational( 14,15 ));
	CHECK(!(Rational( 15,16 ) > Rational( 15,16 )));
	CHECK(!(Rational( 15,16 ) > Rational( 16,17 )));
	// �� �������� ������ ���������, ��������� ��� ������������ ����� ������

}

TEST_CASE("constructor") {
  CHECK(Rational() == Rational(0, 1));
  CHECK(Rational(3) == Rational(3, 1));
  CHECK(Rational(-3) == Rational(-3, 1));
  CHECK_THROWS(Rational(1, 0));
}

TEST_CASE("bool") {
  CHECK(!Rational(0,17));
  CHECK(Rational(1,1));
}

Rational x(3,8);

TEST_CASE("arithmetic operators") {
	CHECK(x + x == 2 * x);
	CHECK(x - x == 0);
	CHECK(x / x == 1);
	CHECK(x * x == Rational( 9, 64 ));

	CHECK(0 == 0*x);
	CHECK(x * 1 == x);
	CHECK(x + 0 == x - 0);

	CHECK(Rational( 1, 5 ) + Rational( 1, 6 ) == Rational( 11,30 ));
	CHECK(Rational( 1, 3 ) + Rational( 1, 6 ) == Rational( 1,2 ));
	CHECK(Rational(1, 3) + 2== 2 + Rational(1,3));
	CHECK(Rational(1, 3) + 2 ==Rational(7, 3));

	CHECK(Rational( 1, 3 ) - Rational( 1, 6 ) == Rational( 1,6 ));
	CHECK(Rational( 1, 6 ) - Rational( 1, 3 ) == Rational( -1,6 ));

	CHECK(Rational( 1, 3 ) *Rational( 1, 5 ) == Rational( 1,15 ));
	CHECK(Rational( 1, 3 ) * Rational( 12, 5 ) == Rational( 4,5 ));
	CHECK(Rational( 1, 3 ) *Rational( -1, 5 ) == Rational( -1,15 ));
	CHECK(Rational( -1, 3 ) *Rational( -1, 5 ) == Rational( 1,15 ));

	CHECK(Rational( 1, 3 ) /Rational( 5, 1 ) == Rational( 1,15 ));
	CHECK(Rational( 1, 3 ) /Rational( 5, 12 ) == Rational( 4,5 ));
	CHECK(Rational( 1, 3 ) /Rational( 5, -1 ) == Rational( -1,15 ));
	CHECK(Rational( -1, 3 ) /Rational( 5, -1 ) == Rational( 1,15 ));
	CHECK_THROWS(Rational(1, 5)/Rational(0, 13));
	CHECK(Rational(1, 5)+5);


}

TEST_CASE("inc and dec") {
	CHECK((x++) + 1 == x);
	CHECK(Rational( x ) + 1 == ++x);
	CHECK(!(x + 1 == ++x));

	CHECK((x--) - 1 == x);
	CHECK(Rational( x ) - 1 == --x);
	CHECK(!(x - 1 == --x));
}

bool testIn(std::string s) {
	std::istringstream istrm(s);
	Rational x;
	istrm >> x;
	if (istrm.good())
		std::cout << "Read success: " << s << " -> " << x << std::endl;
	else
		std::cout << "Read error: " << s << " -> " << x << std::endl;
	return (istrm.good() || istrm.eof()) && !(istrm.fail()) && !(istrm.bad());
}



TEST_CASE("input check") {
	CHECK(testIn("1/2"));
	CHECK(!(testIn("1/-2")));
	CHECK(testIn("-1/2"));
	CHECK(!(testIn("-1/-2")));
	CHECK(testIn("5/10"));
	CHECK(testIn("5/10ab"));
	CHECK(!testIn("  1/ 2  "));
	CHECK(!testIn("  1 /2  "));
	CHECK(!testIn("  1 / 2  "));
	CHECK(!(testIn("1/0 ")));
	CHECK(!(testIn("123 ")));
	CHECK(!(testIn("1 2 3 ")));
	CHECK(!(testIn("a ")));
	CHECK(!(testIn("a b c ")));
}