#include <iostream>
#include <matrix/matrix.hpp>

int main() {
	Matrix<int> m(5, 5,8);
	std::cout << m.sizeX() << "\n";
	std::cout << m.sizeY() << "\n";
	for (int i = 0; i < m.sizeY(); i++) {
		for (int j = 0; j < m.sizeX(); j++) {
			m[i][j] = i * 5 + j;
		}
	}
	m.resizeX(8, 4);
	m.resizeY(11, 2);
	for (int i = 0; i < m.sizeY(); i++) {
		for (int j = 0; j < m.sizeX(); j++) {
			std::cout << m[i][j] << "\t";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
	m.resize(2, 7);
	m.resize(8, 12);
	for (int i = 0; i < m.sizeY(); i++) {
		for (int j = 0; j < m.sizeX(); j++) {
			std::cout << m[i][j] << "\t";
		}
		std::cout << "\n";
	}
}
