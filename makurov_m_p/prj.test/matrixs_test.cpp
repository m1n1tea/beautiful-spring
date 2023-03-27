#include <iostream>
#include <matrixs/matrixs.hpp>



int main() {
    MatrixS m( 5,6 );
    m.at( 4,5 ) = 8;
    m.resize( 10,10 );
    m.at( 2,8 ) = 9;
    std::cout << m.nCols();
    std::cout << m.nRows();
    std::cout << "\n";
    for (int i = 0; i < m.nRows(); i++) {
        for (int j = 0; j < m.nCols(); j++) {
            std::cout << m.at( i,j ) << " ";
        }
        std::cout << "\n";
    }

    m.resize( 5,15 );

    std::cout << "\n";
    for (int i = 0; i < m.nRows(); i++) {
        for (int j = 0; j < m.nCols(); j++) {
            std::cout << m.at( i,j ) << " ";
        }
        std::cout << "\n";
    }
    m.at( 2,5 ) = 15;
    m.swapRows( 2, 4);
    
    std::cout << "\n";
    for (int i = 0; i < m.nRows(); i++) {
        for (int j = 0; j < m.nCols(); j++) {
            std::cout << m.at( i,j ) << " ";
        }
        std::cout << "\n";
    }

    MatrixS x = m;
    MatrixS y;
    y = m;
    std::cout << "\n";
    for (int i = 0; i < x.nRows(); i++) {
        for (int j = 0; j < x.nCols(); j++) {
            std::cout << x.at( i,j ) << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
    for (int i = 0; i < y.nRows(); i++) {
        for (int j = 0; j < y.nCols(); j++) {
            std::cout << y.at( i,j ) << " ";
        }
        std::cout << "\n";
    }

    m.resize( 3,8 );
    std::cout << "\n";
    for (int i = 0; i < m.nRows(); i++) {
        for (int j = 0; j < m.nCols(); j++) {
            std::cout << m.at( i,j ) << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
    for (int i = 0; i < x.nRows(); i++) {
        for (int j = 0; j < x.nCols(); j++) {
            std::cout << x.at(i, j) << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
    for (int i = 0; i < y.nRows(); i++) {
        for (int j = 0; j < y.nCols(); j++) {
            std::cout << y.at(i, j) << " ";
        }
        std::cout << "\n";
    }

}