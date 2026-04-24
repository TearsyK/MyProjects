#ifndef UNTITLED4_LISTATODO_H
#define UNTITLED4_LISTATODO_H

#include "Lista.h"

class ListaToDo
{
public:
    static void todoList()
    {
        Lista<std::string> todo;

        // Dodawanie zadań
        todo.push_back("Kupic mleko");
        todo.push_back("Ukonczyc projekt z C++");
        todo.push_back("Wyslac e-mail do klienta");
        todo.push_back("Zapisac sie na silownie");

        // Wyświetlenie listy To-Do
        std::cout << "Lista To-Do:" << std::endl;
        for (auto it = todo.begin(); it != todo.end(); ++it)
        {
            std::cout << "- " << *it << std::endl;
        }

        // Usunięcie wszystkich zadań
        todo.clear();
        std::cout << "\nWszystkie zadania zostały usunięte." << std::endl;
    }
};


#endif //UNTITLED4_LISTATODO_H
