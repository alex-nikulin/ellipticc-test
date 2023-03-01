#include "integ.h"

class ElPoint {
private:
    ModInt _x;
    ModInt _y;

    static ModInt _a, _d1, _d2;

public:
    ElPoint(ModInt x, ModInt y)
        :_x(x), _y(y)
    {}
    ElPoint()
        :_x(ModInt(0)), _y(ModInt(1))
    {}

    void SetCurve(int_t a, int_t d1, int_t d2 = 1) {
        _a = ModInt(a);
        _d1 = ModInt(d1);
        _d2 = ModInt(d2);
    }

    friend ElPoint operator+(const ElPoint& a, const ElPoint& b);
    friend ElPoint operator-(const ElPoint& a);
    friend ElPoint operator-(const ElPoint& a, const ElPoint& b);
    friend ElPoint operator*(const ModInt& k, const ElPoint& P);

};

ElPoint operator+(const ElPoint& a, const ElPoint& b) {
    ModInt denum = pow((ModInt)1 + ElPoint::_d1 * a._x * a._y * b._x * b._y / ElPoint::_d2, -1);
    ModInt x_out = (a._x*b._y + a._y * b._x) * denum;
    ModInt y_out = (a._y*b._y - ElPoint::_a * a._x * b._x) * denum;

    return ElPoint(x_out, y_out);
}

ElPoint operator-(const ElPoint& a) {
    ElPoint output = ElPoint(-a._x, a._y);
    return output;
}

ElPoint operator-(const ElPoint& a, const ElPoint& b) {
    ElPoint minus_b = -b;
    return a + minus_b;
}

ElPoint operator*(const ModInt& k, const ElPoint& P) {
    std::vector<short> bin_array;
    int_t tmp = k._field;
    bin_array.push_back((short)(tmp % 2));
    tmp /= 2;
    while (tmp > 1) {
        bin_array.push_back((short)(tmp % 2));
        tmp /= 2;
    }
    bin_array.push_back((short)tmp);
    
    ElPoint r = ElPoint();
    ElPoint s = P;

    for (int i = 0; i < bin_array.size(); i++) {
        if (bin_array[i] == 1) {
            r = r + s;
        }
        s = s + s;
    }
    return r;

}