#include "raylib.h"
#include "GameSession.h" // Dołączamy nagłówek sesji gry

int main() {
    // Inicjalizacja okna Raylib i ustawienia początkowe
    InitWindow(1200, 700, "Gra Statki - C++ i Raylib (Refaktoryzacja)");
    SetTargetFPS(144); // Ustawienie klatek na sekundę
    SetRandomSeed(GetTime()); // Inicjalizacja generatora liczb losowych

    // Tworzymy instancję GameSession i uruchamiamy główną pętlę gry
    GameSession game;
    game.Run(); // Metoda Run będzie zawierać główną pętlę while(!WindowShouldClose())

    // Zamknięcie okna Raylib i zwolnienie wszystkich zasobów
    CloseWindow();

    return 0;
}