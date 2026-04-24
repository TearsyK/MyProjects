#ifndef LAB3SEASON2_XOR_H
#define LAB3SEASON2_XOR_H

#include <iostream>
#include <string>

class XOR
{
public:

static std::string szyfrXOR(const std::string& tekst, char klucz) {
    std::string wynik = tekst;
    for (char& znak : wynik) {
        znak ^= klucz; // Operacja XOR na każdym znaku
    }
    return wynik;
}
};


#endif //LAB3SEASON2_XOR_H
