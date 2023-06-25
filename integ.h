#include <boost/multiprecision/cpp_int.hpp>
#include <cmath>
#include <vector>
#include <iostream>

using namespace boost::multiprecision;
typedef uint256_t int_t;
class ECPoint;

class ModInt {
private:
    int_t _field;
    static int_t _q;

public:
    ModInt(int_t value) 
        :_field(value)
    {
        Mod();
    }

    ModInt(int value)
        :_field((int_t)value)
    {
        Mod();
    }

    ModInt()
        :_field(0)
    {}

    void SetQ (int_t q) {
        ModInt::_q = q;
    }

    int_t GetQ() {
        return _q;
    }

    int_t GetValue() {
        return _field;
    }

    friend std::ostream& operator<<(std::ostream& os, const ModInt& obj);
    friend ModInt operator% (const ModInt& a, const ModInt& b);
    friend ModInt operator+ (const ModInt& a, const ModInt& b);
    friend ModInt operator* (const ModInt& a, const ModInt& b);
    friend ModInt operator- (const ModInt& a, const ModInt& b);
    friend ModInt operator- (const ModInt& a);
    friend ModInt operator/ (const ModInt& a, const ModInt& b);
    friend ModInt pow(const ModInt& a, const ModInt& b);
    friend ModInt pow(const ModInt& a, const int& b);
    friend ECPoint operator*(const ModInt& k, const ECPoint& P);
    friend ModInt operator& (const ModInt& a, const int_t& b);
    friend ModInt operator| (const ModInt& a, const int_t& b);
    friend bool operator== (const ModInt& a, const ModInt& b);

    explicit operator int_t() {
        return _field;
    }
    
    ModInt& operator&= (const int_t& rhs) {
        this->_field &= rhs;
        return *this;
    }

    ModInt& operator|= (const int_t& rhs) {
        this->_field |= rhs;
        return *this;
    }

    ModInt ExtEuclid() const {
        int512_t old_r = _field;
        int512_t r = ModInt::_q;
        int512_t old_s = 1;
        int512_t s = 0;

        int step = 0;
        std::cout << std::dec;

        while (r != 0) {
            ++step;
            int512_t quot = old_r / r;
            int512_t tmp_r = r;
            int512_t tmp_s = s;
            r = old_r - quot * r;
            s = old_s - quot * s;
            old_r = tmp_r;
            old_s= tmp_s;
        }

        if (old_s < 0) {
            old_s += ModInt::_q;
        }
        
        if (ModInt((int_t)old_s) * ModInt(_field) == ModInt(1)) {
            std::cout << "reverse successful! " << (ModInt((int_t)old_s) * ModInt(_field))._field << "\n\n\n";
        }
        else {
            std::cout << "reverse unsuccessful! " << (ModInt((int_t)old_s) * ModInt(_field))._field << "\n\n\n";
        }

        if (old_r == 1) {
            return ModInt((int_t)old_s);
        }
        else throw "no inverse exists!";
    }

    void Mod() {
        _field %= _q;
        if (_field < 0) {
            _field += _q;
        }
    }

    bool IsEven() {
        if (_field%2==0) {
            return true;
        }
        return false;
    }
};

std::ostream& operator<<(std::ostream& os, const ModInt& obj)
{
    os << obj._field;
    return os;
}

// ModInt operator% (const ModInt& a, const ModInt& b) {
//     int_t result = a._field % b._field;
//     if (result < 0) {
//         return std::abs(result + b._field);
//     }
//     return ModInt(result);
// }

ModInt operator+ (const ModInt& a, const ModInt& b) {
    int_t q = ModInt::_q;
    int_t x = q - a._field;
    if (x > b._field) {
        return ModInt(a._field + b._field);
    }
    return ModInt(b._field - x);
}

ModInt operator-(const ModInt& a) {
    return ModInt(ModInt::_q - a._field);
}

ModInt operator- (const ModInt& a, const ModInt& b) {
    ModInt minus_b = -b;
    return a + minus_b;
}

ModInt operator* (const ModInt& a, const ModInt& b) {
    std::vector<short> bin_array;
    bin_array.push_back((short)(a._field % 2));
    int_t tmp = a._field;
    tmp /= 2;
    while (tmp > 1) {
        bin_array.push_back((short)(tmp % 2));
        tmp /= 2;
    }
    bin_array.push_back((short)tmp);
    
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

ModInt operator/(const ModInt& a, const ModInt& b) {
    return a * pow(b,-1);
}

ModInt pow(const ModInt& a, const ModInt& b) {
    // std::cout << "here\n";
    std::vector<short> bin_array;
    int_t tmp = b._field;
    bin_array.push_back((short)(tmp % 2));
    tmp /= 2;
    while (tmp > 1) {
        bin_array.push_back((short)(tmp % 2));
        tmp /= 2;
    }
    bin_array.push_back((short)tmp);
    
    // for (int i = 0; i < bin_array.size(); i++) {
    //     std::cout << bin_array[i] << " ";
    // }
    // std::cout << std::endl;

    ModInt r = ModInt(1);
    ModInt m = ModInt(a._field);

    for (int i = 0; i < bin_array.size(); i++) {
        if (bin_array[i] == 1) {
            r = r * m;
        }
        m = m * m;
    }
    return r;
}

ModInt pow(const ModInt& a, const int& b) {
    if (b == -1) {
        ModInt inverse = a.ExtEuclid();
        return inverse;
    }
    ModInt power = ModInt(b);
    return pow(a, power);
}

ModInt operator& (const ModInt& a, const int_t& b) {
    return a._field & b;
}

ModInt operator| (const ModInt& a, const int_t& b) {
    return a._field | b;
}

bool operator== (const ModInt& a, const ModInt& b) {
    return a._field == b._field;
}