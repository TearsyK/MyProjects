#include <iostream>
#include "Iban.h"
#include "Pascal.h"
#include "XOR.h"
int main()
{
        std::string iban;
        std::cout << "Enter IBAN: ";
        std::cin >> iban;

        // Convert input IBAN to uppercase for uniformity
        std::transform(iban.begin(), iban.end(), iban.begin(), ::toupper);

        if (Iban::validateIBAN(iban)) {
            std::cout << "The IBAN is valid." << std::endl;
        } else {
            std::cout << "The IBAN is invalid." << std::endl;
        }

        Pascal::pascal();

        XOR::Xor("Lo cie SKIBIDI",'F');

        return 0;
}