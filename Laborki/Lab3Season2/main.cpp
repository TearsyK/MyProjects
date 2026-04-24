#include <iostream>
#include "XOR.h"
#include "Pascal.h"
#include "Armstrong.h"
int main()
{

    std::string tekst = "PrzykladowyTekst";
    char klucz = 'X';

    std::string zaszyfrowane = XOR::szyfrXOR(tekst, klucz);
    std::cout << "Zaszyfrowane: " << zaszyfrowane << std::endl;

    std::string odszyfrowane = XOR::szyfrXOR(zaszyfrowane, klucz);
    std::cout << "Odszyfrowane: " << odszyfrowane << std::endl;

    Pascal::trojkatPascala(5);
    Armstrong::znajdzLiczbyArmstronga(1,1000);
    return 0;
}

