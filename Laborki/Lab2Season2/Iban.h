#ifndef LAB2SEASON2_IBAN_H
#define LAB2SEASON2_IBAN_H

#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>

class Iban
{
public:
    static int charToValue(char ch) {
        if (std::isdigit(ch)) {
            return ch - '0';
        }
        if (std::isupper(ch)) {
            return ch - 'A' + 10;
        }
        return -1; // Invalid character
    }

// Function to rearrange and convert the IBAN for validation
    static std::string rearrangeIBAN(const std::string& iban) {
        std::string rearranged = iban.substr(4) + iban.substr(0, 4);
        std::string numericIBAN;

        for (char ch : rearranged) {
            int value = charToValue(ch);
            if (value != -1) {
                numericIBAN += std::to_string(value);
            }
        }

        return numericIBAN;
    }

// Function to perform modulo 97 operation on a large number string
    static bool mod97(const std::string& numericIBAN) {
        int remainder = 0;
        for (char ch : numericIBAN) {
            remainder = (remainder * 10 + (ch - '0')) % 97;
        }
        return remainder == 1;
    }

// Function to validate an IBAN
    static bool validateIBAN(const std::string& iban)  {
        // Check IBAN length (varies by country, but must be at least 15 characters)
        if (iban.length() < 15 || iban.length() > 34) {
            return false;
        }

        // Ensure all characters are valid (digits or uppercase letters)
        if (!std::all_of(iban.begin(), iban.end(), [](char ch) {
            return std::isdigit(ch) || std::isupper(ch);
        })) {
            return false;
        }

        // Rearrange and convert the IBAN to numeric form
        std::string numericIBAN = rearrangeIBAN(iban);

        // Validate using modulo 97
        return mod97(numericIBAN);
    }
};


#endif //LAB2SEASON2_IBAN_H
