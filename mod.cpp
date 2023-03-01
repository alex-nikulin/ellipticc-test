#include <iostream>
#include "integ.h"

int_t ModInt::_q;
// ModInt ElPoint::_a;
// ModInt ElPoint::_d1;
// ModInt ElPoint::_d2;

int main() {
    ModInt a = ModInt();
    a.SetField(2023);
    a = ModInt(2022);
    ModInt b = ModInt(2021);
    std::cout << pow(a, b) << std::endl;

    // ElPoint P = ElPoint();
    // P.SetCurve(-1, -121665, 121666);
    // int_t field_size = (int_t)2e255-19;
    // a.SetField(field_size);
    
    

    return 0;
}