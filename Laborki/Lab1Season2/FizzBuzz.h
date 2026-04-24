#ifndef UNTITLED2_FIZZBUZZ_H
#define UNTITLED2_FIZZBUZZ_H

#include <iostream>
#include "Lista.h"
class FizzBuzz
{
private:
    int n;
public:
    FizzBuzz(int num) : n(num) {}

    void uruchom(Lista<int> &lista) const
    {
        for (auto i : )
            if (i % 3 == 0 && i % 5 == 0)
                std::cout<<"FizzBuzz"<< std::endl;
            else if (i % 3 == 0)
                std::cout<<"Fizz"<< std::endl;
            else if (i % 3 == 0)
                std::cout<<"Buzz"<< std::endl;
            else
                std::cout<< i << std::endl;
    }
};


#endif //UNTITLED2_FIZZBUZZ_H
