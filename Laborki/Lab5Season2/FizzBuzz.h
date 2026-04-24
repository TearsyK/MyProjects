#ifndef UNTITLED4_FIZZBUZZ_H
#define UNTITLED4_FIZZBUZZ_H
#include <iostream>
#include "Lista.h"

class FizzBuzz
{
public:

static void fizzBuzz() {
    Lista<int> lista;

    // Dodawanie liczb od 1 do 100
    for (int i = 1; i <= 100; ++i) {
        lista.push_back(i);
    }

    // Iterowanie przez listę i wypisywanie wyników FizzBuzz
    for (auto h = lista.begin(); h != lista.end(); ++h) {
        if (*h % 3 == 0 && *h % 5 == 0) {
            std::cout << "FizzBuzz ";
        } else if (*h % 3 == 0) {
            std::cout << "Fizz ";
        } else if (*h % 5 == 0) {
            std::cout << "Buzz ";
        } else {
            std::cout << *h << " ";
        }
    }

    std::cout << std::endl;
}
};


#endif //UNTITLED4_FIZZBUZZ_H
