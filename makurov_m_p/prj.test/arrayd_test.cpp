#include <iostream>
#include <algorithm>
#include <arrayd/arrayd.hpp>


int main()
{

	ArrayD x(10, 0.56);
	ArrayD y(1);
	ArrayD z(10, 0.56);
	z[2] = 11;
	z[3] = 81;
	z[4] = 77;
	x.reserve(15);
	std::cout << x.ssize() << "\n";
	for (int i = 0; i < x.ssize(); i++) {
		std::cout << x[i] << " ";
	}
	x.insert(3, std::move(z), 2, 5);
	std::cout << "\n";
	std::cout << x.ssize() << "\n";
	for (int i = 0; i < x.ssize(); i++) {
		std::cout << x[i] << " ";
	}
	x.insert(4, 3, 8);
	std::cout << "\n";
	std::cout << x.ssize() << "\n";
	for (int i = 0; i < x.ssize(); i++) {
		std::cout << x[i] << " ";
	}

	x.erase(5, 9);
	std::cout << "\n";
	std::cout << x.ssize() << "\n";
	for (int i = 0; i < x.ssize(); i++) {
		std::cout << x[i] << " ";
	}

	x.resize(10);
	std::cout << "\n";
	std::cout << x.ssize() << "\n";
	for (int i = 0; i < x.ssize(); i++) {
		std::cout << x[i] << " ";
	}

	x.resize(16);
	std::cout << "\n";
	std::cout << x.ssize() << "\n";
	for (int i = 0; i < x.ssize(); i++) {
		std::cout << x[i] << " ";
	}

	return 0;
}
