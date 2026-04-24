#include <iostream>
#include "FizzBuzz.h"
#include "ListaToDo.h"
#include <vector>

int main()
{
        std::string jebackwiatka;
        std::cout << "Zadanie 1: FizzBuzz" << std::endl;
        FizzBuzz::fizzBuzz();

        std::cout << "\nZadanie 2: Lista To-Do" << std::endl;
        ListaToDo::todoList();
        std::cin>>jebackwiatka;
        std::vector<int> lol;
    return 0;
}