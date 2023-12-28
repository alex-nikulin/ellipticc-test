#include "integ.h"
#include <iostream>
#include <bitset>

class ECPoint {
public:
    ModInt _x;
    ModInt _y;

    static ModInt _a, _d;

public:
    ECPoint(ModInt x, ModInt y)
        :_x(x), _y(y)
    {}
    ECPoint()
        :_x(ModInt(0)), _y(ModInt(1))
    {}
    ECPoint(ModInt y) {
        int_t mask = 1;
        mask <<= 255;

        int_t y_bit = (y & mask).GetValue();

        y &= ~mask;
        ECPoint(y, y_bit, false);
    }
    ECPoint(ModInt y, int_t sig_bit, bool offset = true) {
        ModInt x;
        int_t mask = 1;
        mask <<= 255;

        if (offset) {
            sig_bit <<= 255;
        }

        y &= ~mask;

        ModInt u = y*y - 1;
        ModInt v = _d*y*y + 1;
        ModInt z = u * pow(v,3) * pow(u*pow(v,7),(u.GetQ()-5)/8);
        ModInt vz2 = v*z*z;

        if (vz2 == u) {
            x = z;
        }
        else if (vz2 == -u) {
            x = z * pow(2, (z.GetQ()-1)/4);
        }

        int_t x_bit = x.GetValue() & mask;

        if (x_bit == sig_bit) {
            x = -x;
        }
        
        _x = x;
        _y = y;
    }

    int_t GetX() {
        return (int_t)_x;
    }
    int_t GetY() {
        return (int_t)_y;
    }

    void SetCurve(int_t a, int_t d) {
        _a = ModInt(a);
        _d = ModInt(d);
    }

    void SetCurve(ModInt a, ModInt d) {
        _a = a;
        _d = d;
    }

    ModInt compress() {
        ModInt compressed = _y;
        int_t mask = 1;
        mask >>= 1;
        if (_x.IsEven()) {
            compressed &= ~mask;
        }
        else {
            compressed |= mask;
        }
        return compressed;
    }

    friend ECPoint operator+(const ECPoint& a, const ECPoint& b);
    friend ECPoint operator-(const ECPoint& a);
    friend ECPoint operator-(const ECPoint& a, const ECPoint& b);
    friend ECPoint operator*(const ModInt& k, const ECPoint& P);
    friend std::ostream& operator<<(std::ostream& out, const ECPoint& P);
};

ECPoint operator+(const ECPoint& a, const ECPoint& b) {
    ModInt denum = pow((ModInt)1 + ECPoint::_d * a._x * a._y * b._x * b._y, -1);
    ModInt x_out = (a._x*b._y + a._y * b._x) * denum;
    ModInt y_out = (a._y*b._y - ECPoint::_a * a._x * b._x) * denum;

    return ECPoint(x_out, y_out);
}

ECPoint operator-(const ECPoint& a) {
    ECPoint output = ECPoint(-a._x, a._y);
    return output;
}

ECPoint operator-(const ECPoint& a, const ECPoint& b) {
    ECPoint minus_b = -b;
    return a + minus_b;
}

ECPoint operator*(const ModInt& k, const ECPoint& P) {
    std::vector<short> bin_array;
    int_t tmp = k._field;
    bin_array.push_back((short)(tmp % 2));
    tmp /= 2;
    while (tmp > 1) {
        bin_array.push_back((short)(tmp % 2));
        tmp /= 2;
    }
    bin_array.push_back((short)tmp);
    
    ECPoint r = ECPoint();
    ECPoint s = P;

    for (int i = 0; i < bin_array.size(); i++) {
        if (bin_array[i] == 1) {
            r = r + s;
        }
        s = s + s;
    }
    return r;

}

std::ostream& operator<<(std::ostream& out, const ECPoint& P) {
    return out << "X :" << P._x << "\n" << "Y :" << P._y << "\n";
}
