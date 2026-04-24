#ifndef LAB3SEASON2_ARMSTRONG_H
#define LAB3SEASON2_ARMSTRONG_H

#include <iostream>
#include <cmath>

class Armstrong
{
public:

static bool czyArmstrong(int liczba) {
    int suma = 0, temp = liczba;
    int liczbaCyfr = std::to_string(liczba).length();

    while (temp > 0) {
        int cyfra = temp % 10;
        suma += std::pow(cyfra, liczbaCyfr);
        temp /= 10;
    }
    return suma == liczba;
}

static void znajdzLiczbyArmstronga(int zakresDolny, int zakresGorny) {
    std::cout << "Liczby Armstronga w zakresie od " << zakresDolny << " do " << zakresGorny << ":" << std::endl;
    for (int i = zakresDolny; i <= zakresGorny; ++i) {
        if (czyArmstrong(i)) {
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
}
};


#endif //LAB3SEASON2_ARMSTRONG_H
