//
// Created by Xkacp on 09.10.2024.
//

#ifndef UNTITLED2_XOR_H
#define UNTITLED2_XOR_H

#include <iostream>
#include <string>

template <typename T>
T xorZaszyfrujOdszyfruj(const T& input, char klucz)
{
    T output = input;
    for (size_t i = 0; i < input.size(); i++)
    {
        output[i] ^= klucz;
    }
    return output;
}

//specjalizacja dla char, bo nie ma metody size()
template <>
char xorZaszyfrujOdszyfruj(const char& input, char klucz)
{
    return input ^ klucz;
}
#endif //UNTITLED2_XOR_H
