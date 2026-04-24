#ifndef ProjektStatki_SHIP_H
#define ProjektStatki_SHIP_H

#include "raylib.h" // Potrzebne dla Vector2
#include <vector>   // Potrzebne dla std::vector

// Reprezentacja statku
struct Ship {
    int id;             // Unikalne ID statku
    int length;         // Długość statku (np. 2, 3, 4, 5)
    int hits;           // Liczba trafień
    bool isSunk;        // Czy statek został zatopiony
    std::vector<Vector2> occupiedCells; // Lista pól (x, y) zajmowanych przez statek
    bool isHorizontal;  // Orientacja statku (true = poziomo, false = pionowo)
    int startX;         // Współrzędna X lewego górnego rogu statku na planszy
    int startY;         // Współrzędna Y lewego górnego rogu statku na planszy

    // Domyślny konstruktor
    Ship(int shipId = 0, int shipLength = 0, int sX = -1, int sY = -1, bool isHoriz = true)
            : id(shipId), length(shipLength), hits(0), isSunk(false), isHorizontal(isHoriz), startX(sX), startY(sY) {}

    // Metoda do sprawdzenia, czy statek jest zatopiony
    void CheckIfSunk() {
        if (hits >= length) {
            isSunk = true;
        }
    }
};

#endif //ProjektStatki_SHIP_H
