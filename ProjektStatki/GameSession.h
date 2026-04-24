#ifndef UNTITLED7_GAMESESSION_H
#define UNTITLED7_GAMESESSION_H


#include "raylib.h"     // Potrzebne dla Raylib funkcji
#include "GameBoard.h"  // Potrzebne dla GameBoard
#include "Player.h"     // Potrzebne dla Player (HumanPlayer, AIPlayer)
#include "GameUtils.h"  // Potrzebne dla funkcji pomocniczych (w tym globalnych stałych)
#include <memory>       // Potrzebne dla std::unique_ptr
#include <vector>       // Potrzebne dla std::vector

// Stan gry
enum GameState {
    MENU_MAIN,
    PLACING_SHIPS,
    PLAYER_TURN,
    AI_TURN,
    GAME_OVER
};

class GameSession {
private:
    std::unique_ptr<HumanPlayer> player1; // Gracz 1 (człowiek)
    std::unique_ptr<AIPlayer> playerAI;   // Gracz AI

    GameState currentGameState; // Aktualny stan gry
    double placingShipsStartTime; // Czas rozpoczęcia fazy rozmieszczania statków (dla symulacji)

    // --- Drag&Drop dla PLACING_SHIPS ---
    bool isDraggingShip;
    Ship* currentDraggedShipPtr;
    Vector2 dragOffset;
    bool currentDraggedShipHorizontal;
    int currentDraggedShipPaletteIndex;

    // Zmienne używane do obracania oraz przemieszczania uwcześnie postawionych statków
    // Uzywane to przechowywania oryginalnych pozycji statków jeżeli są przemieszczane
    int originalShipStartX;
    int originalShipStartY;
    bool originalShipHorizontal;

    // Statki dostępne na palecie
    std::vector<Ship> availableShipsPalette;
    int nextShipId; // Zmienna pomocnicza, która upewnia się że każdy statek ma unikalne ID

    // --- Prywatne metody ---
    void InitializeShipPalette();
    void ResetGame(); // Reset stanu gry i planszy

    // Prywatne metody sterujące aktualnym stanem gry
    void UpdateMenuMain();
    void UpdatePlacingShips();
    void UpdatePlayerTurn();
    void UpdateAITurn();
    void UpdateGameOver();

    // Prywatne metody rysujące aktualny stan gry
    void DrawMenuMain() const;
    void DrawPlacingShips() const;
    void DrawPlayerTurn() const;
    void DrawAITurn() const;
    void DrawGameOver() const;

public:
    GameSession();  // Konstruktor
    void Run();     // Główna metoda uruchamiająca pętlę gry Raylib
};

#endif //ProjektStatki_GAMESESSION_H
