#ifndef ProjektStatki_CELL_H
#define ProjektStatki_CELL_H

// Reprezentacja pojedynczego pola na planszy
struct Cell {
    bool hasShip;   // Czy na tym polu znajduje się statek
    bool isHit;     // Czy pole zostało trafione
    int shipId;     // ID statku, jeśli hasShip jest true (0 = brak statku, >0 = ID statku)

    // Domyślny konstruktor
    Cell() : hasShip(false), isHit(false), shipId(0) {}
};

#endif //ProjektStatki_CELL_H
