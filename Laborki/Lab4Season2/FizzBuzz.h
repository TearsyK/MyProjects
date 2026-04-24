#ifndef LAB4SEASON2_FIZZBUZZ_H
#define LAB4SEASON2_FIZZBUZZ_H
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "Lista.h"

class FizzBuzz
{
public:
static void fizzBuzz(int n) {
    Lista<std::string> result;
    int i = 0;

    while (i < n)
    {
        if (i % 15 == 0) result.add(std::to_string(i) + " FizzBuzz");
        if (i % 3 == 0) result.add(std::to_string(i) + " Fizz");
        if (i % 5 == 0) result.add(std::to_string(i) + " Buzz");
        result.add(std::to_string(i++));
        i++;
    }

    result.print();
}

};


#endif //LAB4SEASON2_FIZZBUZZ_H
