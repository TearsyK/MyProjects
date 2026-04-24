//
// Created by Xkacp on 17.04.2024.
//

#ifndef PROJECT_GAME_H
#define PROJECT_GAME_H

#include <iostream>
#include <sstream>
#include <fstream>
#include "Maszyna.h"
#include "Surowiec.h"
#include "Produkt.h"
#include "Lista.h"
#include "Symulacja.h"

class Game
{
public:
    int trybPracy;
    Lista Maszyny;
    Lista Surowce;
    Lista Produkty;
    Symulacja symulacja;

    Game();

    static void BaseUstawienia(Lista &Maszyny, Lista &Surowce, Lista &Produkty);

    void TrybCzasowy(Lista &Maszyny, Lista &Surowce, Lista &Produkty);

    void TrybSurowcowy(Lista &Maszyny, Lista &Surowce, Lista &Produkty);

    void start(Lista &Maszyny, Lista &Surowce, Lista &Produkty);

    void saveConfig();

    void loadConfig();
};

#endif //PROJECT_GAME_H
