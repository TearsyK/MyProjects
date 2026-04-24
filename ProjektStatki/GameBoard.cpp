#include "GameBoard.h"
#include <ranges> // Do C++20 Ranges
#include "GameUtils.h" // Potrzebne dla CheckShipPlacementValidity

GameBoard::GameBoard() {
    board.resize(BOARD_SIZE, std::vector<Cell>(BOARD_SIZE));
}

void GameBoard::Reset() {
    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            board[y][x] = Cell(); // Resetuj każde pole
        }
    }
    ships.clear(); // Usuń wszystkie statki
}

bool GameBoard::PlaceShip(Ship newShip, int startX, int startY, bool isHorizontal) {
    // Używamy scentralizowanej funkcji walidacji
    // Jeśli statek ma już ID (jest przemieszczany), przekazujemy jego ID do wykluczenia z kolizji.
    // Jeśli statek nie ma ID (jest z palety), przekazujemy 0.
    int shipIdToExclude = (newShip.id != 0) ? newShip.id : 0;

    if (!CheckShipPlacementValidity(*this, newShip.length, startX, startY, isHorizontal, shipIdToExclude)) {
        // Komunikaty o błędach są generowane w CheckShipPlacementValidity (lub mogą być bardziej szczegółowe tutaj)
        // std::cerr << "Blad: Nie mozna umiescic statku w podanej pozycji (kolizja lub poza plansza)." << std::endl;
        return false;
    }

    // Jeśli wszystkie walidacje przeszły, umieść statek
    newShip.isHorizontal = isHorizontal;
    newShip.startX = startX; // Zapisz pozycję początkową statku
    newShip.startY = startY;
    newShip.occupiedCells.clear(); // Wyczyść stare komórki przed dodaniem nowych
    for (int i = 0; i < newShip.length; ++i) {
        int currentX = isHorizontal ? startX + i : startX;
        int currentY = isHorizontal ? startY : startY + i;
        board[currentY][currentX].hasShip = true;
        board[currentY][currentX].shipId = newShip.id;
        newShip.occupiedCells.push_back({(float)currentX, (float)currentY});
    }

    // Sprawdź, czy statek już istnieje na liście (gdy jest przemieszczany/obracany)
    auto it = std::find_if(ships.begin(), ships.end(),
                           [newShipId = newShip.id](const Ship& s) { return s.id == newShipId; });

    if (it != ships.end()) {
        // Statek już istnieje (był przemieszczany/obracany), aktualizujemy go
        *it = newShip; // Zastąp stary obiekt nowym, zaktualizowanym
    } else {
        // Nowy statek, dodaj go do listy
        ships.push_back(newShip);
    }

    std::cout << "Statek o ID " << newShip.id << " umieszczony. Dlugosc: " << newShip.length << std::endl;
    return true;
}

bool GameBoard::ReceiveShot(int x, int y) {
    if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) {
        return false;
    }
    if (board[y][x].isHit) {
        return false;
    }

    board[y][x].isHit = true;

    if (board[y][x].hasShip) {
        for (auto& ship : ships) {
            if (ship.id == board[y][x].shipId) {
                ship.hits++;
                ship.CheckIfSunk();
                std::cout << "Trafiony statek ID: " << ship.id << " na polu (" << x << ", " << y << ")" << std::endl;
                if (ship.isSunk) {
                    std::cout << "Zatopiono statek ID: " << ship.id << "!" << std::endl;
                }
                return true;
            }
        }
    }
    std::cout << "Pudlo na polu (" << x << ", " << y << ")" << std::endl;
    return false;
}

// Użycie std::ranges::all_of do sprawdzenia, czy wszystkie statki są zatopione
bool GameBoard::AreAllShipsSunk() const {
    return std::ranges::all_of(ships, [](const Ship& s) { return s.isSunk; });
}



