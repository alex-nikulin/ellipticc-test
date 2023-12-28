#include <iostream>
#include "ed25519.h"


int main() {
    Ed25519 key_pair;
    key_pair.keygen();

    std::cout << key_pair.public_key << "\n" << key_pair.private_key << "\n";

    return 0;
}