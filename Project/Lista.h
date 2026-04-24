//
// Created by Xkacp on 07.06.2024.
//

#ifndef PROJECT_LISTA_H
#define PROJECT_LISTA_H

#include <iostream>
#include "ObiektBazowy.h"


class Lista {
public:
    struct Node {
        ObiektBazowy* data;
        Node* next;
        explicit Node(ObiektBazowy* val) : data(val), next(nullptr) {}
    };

    Node* head;

    Lista() : head(nullptr) {}

    void insertAtEnd(ObiektBazowy* val);

    static void modifyListForSurowce(Lista& surowce);

    ~Lista();
};

#endif //PROJECT_LISTA_H
