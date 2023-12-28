#include <openssl/rand.h>
#include "ellipse.h"

typedef uint256_t pkey_t;

int_t ModInt::_q;
ModInt ECPoint::_a;
ModInt ECPoint::_d;

int_t buffer_to_int(unsigned char* buf, int len) {
    int_t key;
    int_t container = 0;
    for (int i = 0; i < len; ++i) {
        container = buf[i];
        key |= container << i * 8;
    }
    return key;
}

class Ed25519 {
public: 
    ECPoint G;

    pkey_t private_key;
    ECPoint public_key;

    Ed25519() {
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

        ECPoint P = ECPoint();
        P.SetCurve(-ModInt(1), -(ModInt(121665)/ModInt(121666)));
        G = ECPoint((ModInt(4)/5), (int_t)0, true);
    }

    void keygen() {
        unsigned char key[32];
        RAND_bytes(key, 32);
        private_key = buffer_to_int(key, 32);
        public_key = private_key * G;
    }

    
};