#include <iostream>
#include "ellipse.h"

int_t ModInt::_q;
ModInt ECPoint::_a;
ModInt ECPoint::_d;

int main() {
    ModInt a = ModInt();
    int_t field_size = 0x7fff;
    int move = 256-16;
    field_size <<= move;
    for (int i = 0; i < 14; ++i) {
        move -= 16;
        int_t put = 0xffff;
        put <<= move;
        field_size |= put;
    }
    field_size |= 0xffed;    
    a.SetQ(field_size);

    std::cout << std::hex << "Q: " << a.GetQ() << "\n";

    ECPoint P = ECPoint();
    P.SetCurve(-ModInt(1), -(ModInt(121665)/ModInt(121666)));
    std::cout << std::hex << "a: " << ECPoint::_a << "\n";
    std::cout << std::hex << "d: " << ECPoint::_d << "\n";

    auto g = ECPoint((ModInt(4)/5), (int_t)0);

    std::cout << std::hex << "hex:\n";
    std::cout << "X: " << g.GetX() << "\n";
    std::cout << "Y: " << g.GetY() << "\n";
    std::cout << std::dec << "dec:\n";
    std::cout << "X: " << g.GetX() << "\n";
    std::cout << "Y: " << g.GetY() << "\n";

    return 0;
}