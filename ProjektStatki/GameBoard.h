#ifndef ProjektStatki_GAMEBOARD_H
#define ProjektStatki_GAMEBOARD_H


#include "raylib.h"     // Potrzebne dla Color
#include "Cell.h"       // Dołączamy definicję Cell
#include "Ship.h"       // Dołączamy definicję Ship
#include <vector>       // Potrzebne dla std::vector
#include <iostream>     // Potrzebne dla std::cerr, std::cout
#include <algorithm>    // Potrzebne dla std::all_of (do Ranges)
#include "GameUtils.h"  // Dołączamy GameUtils.h, aby uzyskać dostęp do BOARD_SIZE i CELL_SIZE


class GameBoard {
public:
    // Dwuwymiarowa tablica pól reprezentująca planszę
    std::vector<std::vector<Cell>> board;
    // Lista statków na tej planszy
    std::vector<Ship> ships;

    GameBoard(); // Konstruktor

    void Reset(); // Resetuje stan planszy i statków

    // Metoda do umieszczania statku na planszy z walidacją
    // Zwraca true jeśli umieszczenie udane, false w przeciwnym wypadku
    bool PlaceShip(Ship newShip, int startX, int startY, bool isHorizontal);

    // Metoda do obsługi strzału na planszy
    // Zwraca true jeśli trafiono statek, false jeśli pudło
    bool ReceiveShot(int x, int y);

    // Metoda do sprawdzenia, czy wszystkie statki na planszy zostały zatopione
    // Wykorzystuje std::ranges (C++20)
    bool AreAllShipsSunk() const;

    // Funkcja do rysowania planszy
    // To jest funkcja pomocnicza, która może być częścią GameBoard lub osobną funkcją
    // Dla prostoty, na razie zostanie w GameUtils.h, ale tutaj deklarujemy jej zależność
    // od GameBoard.
    // void Draw(int offsetX, int offsetY, bool showShips) const;
};



#endif //ProjektStatki_GAMEBOARD_H
