#include "GameUtils.h"
#include "GameBoard.h"
#include <iostream>    // Potrzebne dla std::cerr, std::cout
#include <algorithm>   // Potrzebne dla std::find_if, std::any_of (do Ranges)
#include <string>      // Potrzebne do string, getline
#include <regex>       // Potrzebne dla std::regex

// Implementacja funkcji rysującej planszę bitewną
void DrawBattleshipBoard(const GameBoard& boardData, int offsetX, int offsetY, bool showShips) {
    DrawRectangle(offsetX, offsetY, BOARD_SIZE * CELL_SIZE, BOARD_SIZE * CELL_SIZE, WATER_COLOR);

    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            Rectangle cellRect = { (float)(offsetX + x * CELL_SIZE), (float)(offsetY + y * CELL_SIZE), (float)CELL_SIZE, (float)CELL_SIZE };

            if (boardData.board[y][x].isHit) {
                if (boardData.board[y][x].hasShip) {
                    DrawRectangleRec(cellRect, HIT_COLOR);
                    // Rysowanie X o grubości 3.0f
                    DrawLineEx({cellRect.x, cellRect.y}, {cellRect.x + cellRect.width, cellRect.y + cellRect.height}, 3.0f, BLACK);
                    DrawLineEx({cellRect.x, cellRect.y + cellRect.height}, {cellRect.x + cellRect.width, cellRect.y}, 3.0f, BLACK);
                } else {
                    DrawRectangleRec(cellRect, MISS_COLOR);
                    // Rysowanie kółka
                    DrawCircleLines(cellRect.x + CELL_SIZE / 2, cellRect.y + CELL_SIZE / 2, CELL_SIZE / 4, BLACK);
                }
            } else if (showShips && boardData.board[y][x].hasShip) {
                bool isSunk = false;
                for(const auto& ship : boardData.ships) {
                    if (ship.id == boardData.board[y][x].shipId && ship.isSunk) {
                        isSunk = true;
                        break;
                    }
                }
                DrawRectangleRec(cellRect, isSunk ? SUNK_SHIP_COLOR : SHIP_COLOR);
            }

            DrawRectangleLinesEx(cellRect, 1, GRID_LINE_COLOR);
        }
    }
}

// Implementacja funkcji sprawdzającej poprawność umieszczenia statku
bool CheckShipPlacementValidity(const GameBoard& board, int shipLength, int startX, int startY, bool isHorizontal, int excludedShipId) {
    // 1. Sprawdzenie, czy statek mieści się na planszy
    if (isHorizontal) {
        if (startX < 0 || startX + shipLength > BOARD_SIZE || startY < 0 || startY >= BOARD_SIZE) {
            // std::cerr << "Blad walidacji: Statek poza plansza (poziomo)." << std::endl;
            return false;
        }
    } else {
        if (startX < 0 || startX >= BOARD_SIZE || startY < 0 || startY + shipLength > BOARD_SIZE) {
            // std::cerr << "Blad walidacji: Statek poza plansza (pionowo)." << std::endl;
            return false;
        }
    }

    // 2. Sprawdzenie kolizji z innymi statkami lub sąsiednimi polami
    for (int i = 0; i < shipLength; ++i) {
        int currentX = isHorizontal ? startX + i : startX;
        int currentY = isHorizontal ? startY : startY + i;

        // Sprawdź same pola, które statek zajmie
        if (board.board[currentY][currentX].hasShip && board.board[currentY][currentX].shipId != excludedShipId) {
            // std::cerr << "Blad walidacji: Kolizja z istiejacym statkiem na polu (" << currentX << ", " << currentY << ")." << std::endl;
            return false;
        }

        // Sprawdź okoliczne pola (włącznie z ukośnymi)
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                // Nie sprawdzamy samego pola, które chcemy zająć, tylko sąsiednie
                if (dx == 0 && dy == 0) continue;

                int checkX = currentX + dx;
                int checkY = currentY + dy;

                // Upewnij się, że sprawdzane pole jest w granicach planszy
                if (checkX >= 0 && checkX < BOARD_SIZE && checkY >= 0 && checkY < BOARD_SIZE) {
                    if (board.board[checkY][checkX].hasShip && board.board[checkY][checkX].shipId != excludedShipId) {
                        // std::cerr << "Blad walidacji: Statek dotyka innego statku na polu (" << checkX << ", " << checkY << ")." << std::endl;
                        return false; // Statek dotyka innego statku
                    }
                }
            }
        }
    }
    return true; // Jeśli wszystkie walidacje przeszły
}


// Implementacja nowej funkcji rysowania pojedynczego statku wizualnie
void DrawShipVisual(int length, float drawX, float drawY, Color color, bool isHorizontal, int cellSize, bool drawOutline) {
    float width = isHorizontal ? (float)length * cellSize : (float)cellSize;
    float height = isHorizontal ? (float)cellSize : (float)length * cellSize;

    DrawRectangle(drawX, drawY, width, height, color);
    if (drawOutline) {
        DrawRectangleLinesEx({drawX, drawY, width, height}, 2, BLACK); // Gruby obrys
    } else {
        DrawRectangleLinesEx({drawX, drawY, width, height}, 1, DARKGRAY); // Cienki obrys
    }
}

// Implementacja funkcji rysowania pojedynczego statku jako prostokąta (teraz używa DrawShipVisual)
void DrawShipRect(const Ship& ship, float drawX, float drawY, Color color, bool isHorizontal, int cellSize, bool drawOutline) {
    DrawShipVisual(ship.length, drawX, drawY, color, isHorizontal, cellSize, drawOutline);
}


// Implementacja funkcji rysowania przycisku
bool DrawButton(Rectangle bounds, const char* text, Color color, Color hoverColor, bool enabled) {
    bool clicked = false;
    Vector2 mousePoint = GetMousePosition();
    Color currentColor = color;

    if (!enabled) {
        currentColor = { (unsigned char)(color.r / 2), (unsigned char)(color.g / 2), (unsigned char)(color.b / 2), color.a }; // Przyciemniony kolor
    } else {
        if (CheckCollisionPointRec(mousePoint, bounds)) {
            currentColor = hoverColor;
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                clicked = true;
            }
        }
    }

    DrawRectangleRec(bounds, currentColor);
    DrawRectangleLinesEx(bounds, 2, DARKGRAY); // Ramka przycisku
    // Wyśrodkowanie tekstu
    int textWidth = MeasureText(text, 20);
    DrawText(text, bounds.x + bounds.width / 2 - textWidth / 2, bounds.y + bounds.height / 2 - 10, 20, enabled ? TEXT_COLOR : GRAY);

    return clicked && enabled; // Zwróć kliknięcie tylko jeśli przycisk jest aktywny
}

// --- Implementacja funkcji zapisu i wczytywania gry (FileSystem) ---

bool CheckSaveFileExists() {
    return std::filesystem::exists(SAVE_FILE_PATH);
}

void SaveGame(int currentGameState, const GameBoard& playerBoard, const GameBoard& aiBoard) {
    std::ofstream outFile(SAVE_FILE_PATH);
    if (outFile.is_open()) {
        outFile << currentGameState << "\n"; // Zapisz numer stanu

        // Zapis planszy gracza (liczba statków, dane statków, stan trafień pól)
        outFile << playerBoard.ships.size() << "\n";
        for (const auto& ship : playerBoard.ships) {
            outFile << ship.id << " "
                    << ship.length << " "
                    << ship.hits << " "
                    << (ship.isSunk ? 1 : 0) << " " // Zapisz bool jako int (0 lub 1)
                    << (ship.isHorizontal ? 1 : 0) << " " // Zapisz bool jako int (0 lub 1)
                    << ship.startX << " "
                    << ship.startY << "\n";
        }
        for (int y = 0; y < BOARD_SIZE; ++y) {
            for (int x = 0; x < BOARD_SIZE; ++x) {
                outFile << (playerBoard.board[y][x].isHit ? 'H' : 'N');
            }
        }
        outFile << "\n";

        // Zapis planszy AI (liczba statków, dane statków, stan trafień pól)
        outFile << aiBoard.ships.size() << "\n";
        for (const auto& ship : aiBoard.ships) {
            outFile << ship.id << " "
                    << ship.length << " "
                    << ship.hits << " "
                    << (ship.isSunk ? 1 : 0) << " "
                    << (ship.isHorizontal ? 1 : 0) << " "
                    << ship.startX << " "
                    << ship.startY << "\n";
        }
        for (int y = 0; y < BOARD_SIZE; ++y) {
            for (int x = 0; x < BOARD_SIZE; ++x) {
                outFile << (aiBoard.board[y][x].isHit ? 'H' : 'N');
            }
        }
        outFile << "\n";

        outFile.close();
        std::cout << "Gra zapisana do: " << std::filesystem::absolute(SAVE_FILE_PATH) << std::endl;
    } else {
        std::cerr << "Blad: Nie mozna otworzyc pliku do zapisu: " << SAVE_FILE_PATH << std::endl;
    }
}

bool LoadGame(int& currentGameState, GameBoard& playerBoard, GameBoard& aiBoard) {
    if (!std::filesystem::exists(SAVE_FILE_PATH)) {
        std::cout << "Brak pliku zapisu: " << SAVE_FILE_PATH << std::endl;
        return false;
    }

    std::ifstream inFile(SAVE_FILE_PATH);
    if (inFile.is_open()) {
        std::string line;

        // Wczytaj stan gry
        std::getline(inFile, line);
        try {
            currentGameState = std::stoi(line);
        } catch (const std::exception& e) {
            std::cerr << "Blad parsowania stanu gry: " << e.what() << std::endl;
            inFile.close();
            return false;
        }

        // --- Funkcja pomocnicza do wczytywania danych planszy ---
        auto loadBoardData = [&](GameBoard& boardToLoad) {
            boardToLoad.Reset(); // Wyczyść planszę przed wczytaniem

            std::getline(inFile, line); // Wczytaj linię z liczbą statków
            size_t numShips;
            try {
                numShips = std::stoul(line);
            } catch (const std::exception& e) {
                std::cerr << "Blad parsowania liczby statkow: " << e.what() << std::endl;
                return false;
            }

            // Regex do parsowania linii statku: id, length, hits, isSunk, isHorizontal, startX, startY
            // (\d+)\s+(\d+)\s+(\d+)\s+(0|1)\s+(0|1)\s+(-?\d+)\s+(-?\d+)
            // Group 1: id, Group 2: length, Group 3: hits, Group 4: isSunk, Group 5: isHorizontal, Group 6: startX, Group 7: startY
            std::regex shipRegex(R"((\d+)\s+(\d+)\s+(\d+)\s+(0|1)\s+(0|1)\s+(-?\d+)\s+(-?\d+))"); // Użycie raw string literal R"(...)"

            for (size_t i = 0; i < numShips; ++i) {
                std::getline(inFile, line); // Wczytaj linię danych statku
                std::smatch matches;
                if (std::regex_match(line, matches, shipRegex)) {
                    try {
                        int id = std::stoi(matches[1].str());
                        int length = std::stoi(matches[2].str());
                        int hits = std::stoi(matches[3].str());
                        bool isSunk = (std::stoi(matches[4].str()) == 1);
                        bool isHorizontal = (std::stoi(matches[5].str()) == 1);
                        int startX = std::stoi(matches[6].str());
                        int startY = std::stoi(matches[7].str());

                        Ship loadedShip(id, length, startX, startY, isHorizontal);
                        loadedShip.hits = hits;
                        loadedShip.isSunk = isSunk;

                        // Ręcznie umieść statek na planszy po wczytaniu
                        // BARDZO WAŻNE: To omija walidację PlaceShip i gwarantuje, że plansza zostanie odtworzona dokładnie tak, jak została zapisana.
                        loadedShip.occupiedCells.clear();
                        for (int k = 0; k < loadedShip.length; ++k) {
                            int cellX = loadedShip.isHorizontal ? loadedShip.startX + k : loadedShip.startX;
                            int cellY = loadedShip.isHorizontal ? loadedShip.startY : loadedShip.startY + k;
                            if (cellX >= 0 && cellX < BOARD_SIZE && cellY >= 0 && cellY < BOARD_SIZE) {
                                boardToLoad.board[cellY][cellX].hasShip = true;
                                boardToLoad.board[cellY][cellX].shipId = loadedShip.id;
                                loadedShip.occupiedCells.push_back({(float)cellX, (float)cellY});
                            }
                        }
                        boardToLoad.ships.push_back(loadedShip);

                    } catch (const std::exception& e) {
                        std::cerr << "Blad parsowania danych statku z linii: '" << line << "'. Blad: " << e.what() << std::endl;
                        return false;
                    }
                } else {
                    std::cerr << "Blad: Linia statku nie pasuje do wzorca regex: '" << line << "'" << std::endl;
                    return false;
                }
            }

            // Wczytaj łańcuch trafień pól
            std::getline(inFile, line); // Wczytaj linię z trafieniami (H/N)
            if (line.length() != BOARD_SIZE * BOARD_SIZE) {
                std::cerr << "Blad: Nieprawidlowa dlugosc lancucha trafien pól." << std::endl;
                return false;
            }
            for (int y = 0; y < BOARD_SIZE; ++y) {
                for (int x = 0; x < BOARD_SIZE; ++x) {
                    boardToLoad.board[y][x].isHit = (line[y * BOARD_SIZE + x] == 'H');
                }
            }
            return true;
        }; // Koniec lambda loadBoardData

        if (!loadBoardData(playerBoard)) {
            inFile.close();
            return false;
        }
        if (!loadBoardData(aiBoard)) {
            inFile.close();
            return false;
        }

        inFile.close();
        std::cout << "Gra wczytana z: " << std::filesystem::absolute(SAVE_FILE_PATH) << std::endl;
        return true;
    } else {
        std::cerr << "Blad: Nie mozna otworzyc pliku do wczytania: " << SAVE_FILE_PATH << std::endl;
        return false;
    }
}

void DeleteSaveGame() {
    if (std::filesystem::exists(SAVE_FILE_PATH)) {
        std::filesystem::remove(SAVE_FILE_PATH);
        std::cout << "Usunieto plik zapisu: " << SAVE_FILE_PATH << std::endl;
    }
}

