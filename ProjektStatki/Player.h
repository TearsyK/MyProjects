#ifndef ProjektStatki_PLAYER_H
#define ProjektStatki_PLAYER_H


#include "GameBoard.h"      // Potrzebne dla GameBoard
#include <string>           // Potrzebne dla std::string
#include <vector>           // Potrzebne dla std::vector
#include <thread>           // Potrzebne dla std::thread
#include <future>           // Potrzebne dla std::future
#include <chrono>           // Potrzebne dla std::chrono (opóźnienia)
#include <concepts>         // Potrzebne dla C++20 Concepts

// Koncept: PlayerConcept
// Definiuje wymagania dla dowolnej klasy, która ma być traktowana jako "gracz"
template <typename T>
concept PlayerConcept = requires(T player, GameBoard& ownBoard, GameBoard& enemyBoard, int x, int y) {
    { player.GetId() } -> std::same_as<int>; // Musi mieć metodę GetId zwracającą int
    { player.GetName() } -> std::same_as<std::string>; // Musi mieć metodę GetName zwracającą std::string
    { player.InitializeShips(ownBoard) } -> std::same_as<void>; // Metoda do inicjalizacji statków
    // MakeMove może być asynchroniczne, więc niekoniecznie zwraca bool natychmiast
    // { player.MakeMove(enemyBoard) } -> std::same_as<bool>;
    { player.StartThinking(ownBoard, enemyBoard) } -> std::same_as<void>;
    { player.IsThinking() } -> std::same_as<bool>;
    { player.GetAIMoveResult() } -> std::same_as<Vector2>;
};


// Abstrakcyjna klasa bazowa dla graczy
class Player {
protected:
    int id;
    std::string name;
    GameBoard playerBoard; // Każdy gracz ma swoją planszę

public:
    Player(int id, const std::string& name) : id(id), name(name) {}
    virtual ~Player() = default; // Wirtualny destruktor dla polimorfizmu

    int GetId() const { return id; }
    std::string GetName() const { return name; }
    GameBoard& GetBoard() { return playerBoard; }
    const GameBoard& GetBoard() const { return playerBoard; }

    // Metoda czysto wirtualna do inicjalizacji statków (implementacja w klasach pochodnych)
    virtual void InitializeShips(GameBoard& board) = 0;

    // Metoda czysto wirtualna do wykonania ruchu (implementacja w klasach pochodnych)
    // Zwraca true, jeśli ruch zakończył się trafieniem, false jeśli pudłem
    virtual bool MakeMove(GameBoard& enemyBoard) = 0;
};

// Klasa reprezentująca gracza-człowieka
class HumanPlayer : public Player {
public:
    HumanPlayer(int id, const std::string& name = "Gracz") : Player(id, name) {}

    // Implementacja inicjalizacji statków dla gracza-człowieka
    // W Raylib będzie to interaktywny proces drag&drop
    void InitializeShips(GameBoard& board) override;

    // Implementacja wykonania ruchu dla gracza-człowieka
    // Czeka na kliknięcie gracza na planszy przeciwnika
    bool MakeMove(GameBoard& enemyBoard) override;
};

// Klasa reprezentująca gracza komputerowego (AI)
class AIPlayer : public Player {
public: // Zmiana na public dla dostępu z GameSession.cpp
    std::future<Vector2> aiMoveFuture; // Obiekt do odbierania wyniku z wątku AI
private:
    bool aiThinking;            // Flaga informująca, czy AI myśli

    // Wewnętrzna metoda do obliczania ruchu AI (wykonywana w wątku)
    Vector2 ComputeAIMoveInternal(const GameBoard& ownBoard, const GameBoard& enemyBoard);

public:
    AIPlayer(int id, const std::string& name = "Komputer") : Player(id, name), aiThinking(false) {}

    // Implementacja inicjalizacji statków dla AI (np. losowe rozmieszczenie)
    void InitializeShips(GameBoard& board) override;

    // Metoda rozpoczynająca obliczanie ruchu AI w osobnym wątku
    void StartThinking(const GameBoard& ownBoard, const GameBoard& enemyBoard);

    // Metoda sprawdzająca, czy AI nadal myśli
    bool IsThinking() const { return aiThinking; }

    // Metoda pobierająca wynik ruchu AI (blokuje, jeśli wątek jeszcze nie zakończył)
    Vector2 GetAIMoveResult();

    // Implementacja wykonania ruchu dla AI (zarządza wątkiem)
    bool MakeMove(GameBoard& enemyBoard) override; // Ta metoda będzie zarządzać wątkiem i aplikować ruch
};


#endif //ProjektStatki_PLAYER_H
