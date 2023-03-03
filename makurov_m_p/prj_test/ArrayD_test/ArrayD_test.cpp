#include <iostream>
#include <algorithm>
#include <arrayD/arrayD.hpp>


int main()
{

	ArrayD<int> x(10, 15);
	ArrayD<int> y(1);
	ArrayD<int> z(5, 3);
	z[2] = 11;
	z[3] = 81;
	z[4] = 77;
	x.reserve(15);
	std::cout << x.size() << "\n";
	for (int i = 0; i < x.size(); i++) {
		std::cout << x[i] << " ";
	}
	x.insert(3, std::move(z), 2, 5);
	std::cout << "\n";
	std::cout << x.size() << "\n";
	for (int i = 0; i < x.size(); i++) {
		std::cout << x[i] << " ";
	}
	x.insert(4, 3, 8);
	std::cout << "\n";
	std::cout << x.size() << "\n";
	for (int i = 0; i < x.size(); i++) {
		std::cout << x[i] << " ";
	}

	x.insert(10, 2, 2);
	std::cout << "\n";
	std::cout << x.size() << "\n";
	for (int i = 0; i < x.size(); i++) {
		std::cout << x[i] << " ";
	}

	x.resize(10);
	std::cout << "\n";
	std::cout << x.size() << "\n";
	for (int i = 0; i < x.size(); i++) {
		std::cout << x[i] << " ";
	}

	x.resize(16);
	std::cout << "\n";
	std::cout << x.size() << "\n";
	for (int i = 0; i < x.size(); i++) {
		std::cout << x[i] << " ";
	}
	z = x.subArray(4, 12);

	std::cout << "\n";
	std::cout << z.size() << "\n";
	for (int i = 0; i < z.size(); i++) {
		std::cout << z[i] << " ";
	}
	std::cout << x.size() << "\n";
	for (int i = 0; i < x.size(); i++) {
		std::cout << x[i] << " ";
	}

	return 0;
}
