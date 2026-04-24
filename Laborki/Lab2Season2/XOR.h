#ifndef LAB2SEASON2_XOR_H
#define LAB2SEASON2_XOR_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

class XOR
{
public:

    static int Xor(std::string text, char key) {
        std::vector<char> encrypted(text.size());
        std::transform(text.begin(), text.end(), encrypted.begin(), [key](char c) {
            return c ^ key;
        });

        std::cout << "Zaszyfrowany tekst: ";
        for (char c : encrypted) {
            std::cout << c;
        }
        std::cout << "\n";

        std::string decrypted(text.size(), ' ');

        std::transform(encrypted.begin(), encrypted.end(), decrypted.begin(), [key](char c) {
            return c ^ key;
        });

        std::cout << "Odszyfrowany tekst: " << decrypted << "\n";

        return 0;
    }
};


#endif //LAB2SEASON2_XOR_H
