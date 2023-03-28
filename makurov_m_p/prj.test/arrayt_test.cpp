#include <iostream>
#include <algorithm>
#include <arrayt/arrayt.hpp>


int main()
{


    ArrayT<ArrayT<double>> x(10, ArrayT<double>(15));
    x.insert(4,5, ArrayT<double>(88));


    x.erase(5, 9);


    x.resize(10);

    x=std::move(x);
    //no memory leak!!

     return 0;
}
