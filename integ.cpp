#include <boost/multiprecision/cpp_int.hpp>
#include <cmath>
#include <vector>
#include <iostream>

using namespace boost::multiprecision;
typedef int256_t int_t;

class ModInt {
private:
    int_t _field;
    static int_t _q;

public:
    ModInt(int_t value) 
        :_field(value)
    {}

    void SetField (int_t q) {
        this->_q = q;
    }

    friend std::ostream& operator<<(std::ostream& os, const ModInt& obj);
    friend ModInt operator% (const ModInt& a, const ModInt& b);
    friend ModInt operator+ (const ModInt& a, const ModInt& b);
    friend ModInt operator* (const ModInt& a, const ModInt& b);
};

std::ostream& operator<<(std::ostream& os, const ModInt& obj)
{
    return os;
}

ModInt operator% (const ModInt& a, const ModInt& b) {
    int_t result = a._field % b._field;
    if (result < 0) {
        return std::abs(result + b._field);
    }
    return ModInt(result);
}

ModInt operator+ (const ModInt& a, const ModInt& b) {
    int_t x = a._q - b._field;
    if (x > b._field) {
        return ModInt(a._field + b._field);
    }
    return ModInt(b._field - x);
}

ModInt operator* (const ModInt& a, const ModInt& b) {
    std::vector<short> bin_array;
    bin_array.push_back((short)(a._field % 2));
    int_t tmp = a._field;
    while (tmp > 1) {
        bin_array.push_back((short)(tmp % 2));
        tmp /= 2;
    }
    bin_array.push_back((short)(tmp / 2));
    
    ModInt r = ModInt(0);
    ModInt s = ModInt(b._field);

    for (int i = 0; i < bin_array.size(); i++) {
        if (bin_array[i] == 1) {
            r = r + s;
        }
        s = s + s;
    }
    return r;
}
