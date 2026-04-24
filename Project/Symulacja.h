//
// Created by Xkacp on 08.06.2024.
//

#ifndef PROJECT_SYMULACJA_H
#define PROJECT_SYMULACJA_H

#include "Lista.h"
#include "random"
#include "fstream"
#include "Surowiec.h"
#include "Produkt.h"

class Symulacja
{
private:

    std::mt19937 generator;
public:
    int CzasSymulacji{};
    using ListaNode = Lista::Node;

    Symulacja();

    void TrybCzasowy(int CzasSymulacji, Lista& Maszyny, Lista& Surowce, Lista& Produkty);

    void TrybSurowcowy(Lista& Maszyny, Lista& Surowce, Lista& Produkty);
};


#endif //PROJECT_SYMULACJA_H
