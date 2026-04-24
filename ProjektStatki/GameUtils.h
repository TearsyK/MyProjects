#ifndef ProjektStatki_GAMEUTILS_H
#define ProjektStatki_GAMEUTILS_H

#include "raylib.h"     // Potrzebne dla Color, Rectangle, Vector2
#include <string>       // Potrzebne dla std::string
#include <filesystem>   // C++17 File System
#include <fstream>      // Do operacji na plikach
#include <regex>        // Nowe: Dodano do użycia std::regex

// Deklaracja wyprzedzająca dla klasy GameBoard i struktury Ship
class GameBoard;
struct Ship; // Potrzebne dla DrawShipRect i DrawShipVisual

// --- GLOBALNE DEFINICJE I STAŁE ---
// Rozmiary ekranu
const int screenWidth = 1200;
const int screenHeight = 700;

// Definicje rozmiaru planszy
const int BOARD_SIZE = 10;          // Plansza 10x10
const int CELL_SIZE = 40;           // Rozmiar pojedynczego pola w pikselach
const int BOARD_PADDING = 20;       // Odstęp od krawędzi okna

// Kolory
const Color WATER_COLOR = SKYBLUE;
const Color GRID_LINE_COLOR = DARKBLUE;
const Color HIT_COLOR = RED;
const Color MISS_COLOR = LIGHTGRAY;
const Color SHIP_COLOR = DARKGRAY;
const Color SUNK_SHIP_COLOR = BLACK;
const Color BUTTON_COLOR = LIGHTGRAY;
const Color BUTTON_HOVER_COLOR = GRAY;
const Color TEXT_COLOR = DARKBLUE;

// Funkcja rysująca planszę bitewną
void DrawBattleshipBoard(const GameBoard& boardData, int offsetX, int offsetY, bool showShips);

// Nowa funkcja do sprawdzania, czy statek może być umieszczony w danej pozycji
// Używana do walidacji ducha statku i podczas faktycznego umieszczania/obracania.
// excludedShipId: ID statku, który ma być ignorowany podczas sprawdzania kolizji (używane przy przemieszczaniu statku)
bool CheckShipPlacementValidity(const GameBoard& board, int shipLength, int startX, int startY, bool isHorizontal, int excludedShipId = 0);

// Nowa funkcja pomocnicza do rysowania statku wizualnie (bez konkretnego obiektu Ship, tylko jego wymiarów)
void DrawShipVisual(int length, float drawX, float drawY, Color color, bool isHorizontal, int cellSize, bool drawOutline = false);


// Funkcja pomocnicza do rysowania pojedynczego statku jako prostokąta
// Ta funkcja będzie teraz korzystać z DrawShipVisual
void DrawShipRect(const Ship& ship, float drawX, float drawY, Color color, bool isHorizontal, int cellSize, bool drawOutline = false);


// Funkcja pomocnicza do rysowania przycisku
// Zwraca true, jeśli przycisk został kliknięty
bool DrawButton(Rectangle bounds, const char* text, Color color, Color hoverColor, bool enabled = true); // Dodano parametr enabled

// --- Funkcje do zapisu i wczytywania gry (FileSystem) ---

// Ścieżka do pliku zapisu
const std::string SAVE_FILE_PATH = "battleship_save.txt";

// Funkcja do sprawdzenia, czy plik zapisu istnieje
bool CheckSaveFileExists();

// Funkcja do zapisu gry
// currentGameState: aktualny stan gry
// playerBoard: plansza gracza do zapisu
// aiBoard: plansza AI do zapisu
// Zapisuje pełny stan plansz i statków
void SaveGame(int currentGameState, const GameBoard& playerBoard, const GameBoard& aiBoard);

// Funkcja do wczytywania gry
// currentGameState: referencja do zmiennej, do której zostanie wczytany stan gry
// playerBoard: referencja do planszy gracza, do której zostaną wczytane dane
// aiBoard: referencja do planszy AI, do której zostaną wczytane dane
// Zwraca true, jeśli wczytano poprawnie, false w przeciwnym wypadku
// Wczytuje pełny stan plansz i statków
bool LoadGame(int& currentGameState, GameBoard& playerBoard, GameBoard& aiBoard);

// Funkcja do usuwania pliku zapisu
void DeleteSaveGame();

#endif //ProjektStatki_GAMEUTILS_H
