#include "Player.h"
#include <random> // Potrzebne do generowania liczb losowych dla AI

// --- Implementacja HumanPlayer ---

void HumanPlayer::InitializeShips(GameBoard& board) {
    board.Reset(); // Upewnij się, że plansza jest czysta dla interaktywnego rozmieszczania
    std::cout << GetName() << " gotowy do rozmieszczenia statkow." << std::endl;
    // Faktyczne rozmieszczanie statków będzie obsługiwane przez logikę drag&drop w GameSession
}

bool HumanPlayer::MakeMove(GameBoard& enemyBoard) {
    // Ta metoda jest wywoływana, gdy tura gracza-człowieka się rozpoczyna.
    // Faktyczna obsługa kliknięć myszy i logiczne wykonanie ruchu
    // będzie w głównej pętli Raylib w GameSession.cpp,
    // która będzie wywoływać enemyBoard.ReceiveShot().
    // Ta funkcja jedynie sygnalizuje gotowość do ruchu.
    return false; // Zawsze zwracamy false, bo ruch jest obsługiwany zewnętrznie
}

// --- Implementacja AIPlayer ---

void AIPlayer::InitializeShips(GameBoard& board) {
    // Losowe rozmieszczanie statków dla AI
    board.Reset(); // Upewnij się, że plansza jest czysta

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, BOARD_SIZE - 1);
    std::uniform_int_distribution<> orientDis(0, 1); // 0 = poziomo, 1 = pionowo

    // Definiowanie statków do umieszczenia (standardowa flota dla AI)
    std::vector<int> shipLengths = {5, 4, 3, 3, 2}; // Przykładowe statki

    int shipIdCounter = GetId() * 100; // Unikalne ID dla statków AI

    for (int length : shipLengths) {
        bool placed = false;
        int attempts = 0;
        const int MAX_ATTEMPTS = 500; // Zapobieganie nieskończonej pętli

        while (!placed && attempts < MAX_ATTEMPTS) {
            int startX = dis(gen);
            int startY = dis(gen);
            bool isHorizontal = (orientDis(gen) == 0); // Losowa orientacja

            // Tworzymy tymczasowy statek do próby umieszczenia
            Ship currentShip(shipIdCounter++, length);

            // Wywołujemy PlaceShip, która zawiera walidację
            if (board.PlaceShip(currentShip, startX, startY, isHorizontal)) {
                placed = true;
            }
            attempts++;
        }
        if (!placed) {
            std::cerr << "Ostrzezenie: Nie mozna bylo umiescic statku o dlugosci " << length << " dla " << GetName() << std::endl;
        }
    }
    std::cout << GetName() << " rozmiescil statki." << std::endl;
}

// Wewnętrzna metoda do obliczania ruchu AI (wykonywana w wątku)
Vector2 AIPlayer::ComputeAIMoveInternal(const GameBoard& ownBoard, const GameBoard& enemyBoard) {
    // Symulacja myślenia AI (np. dłuższego algorytmu)
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Opóźnienie 1 sekundy

    // TODO: Zastąp to swoim zaawansowanym algorytmem AI (mapa prawdopodobieństwa itp.)
    // Na razie losowy strzał na nie-trafione pole
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, BOARD_SIZE - 1);

    int targetX, targetY;
    int attempts = 0;
    const int MAX_AI_SHOT_ATTEMPTS = 100; // Avoid infinite loop if board is full
    do {
        targetX = dis(gen);
        targetY = dis(gen);
        attempts++;
    } while (enemyBoard.board[targetY][targetX].isHit && attempts < MAX_AI_SHOT_ATTEMPTS);

    if (attempts >= MAX_AI_SHOT_ATTEMPTS) {
        // Fallback: if cannot find an unhit spot after many attempts, just pick the first available
        for (int y = 0; y < BOARD_SIZE; ++y) {
            for (int x = 0; x < BOARD_SIZE; ++x) {
                if (!enemyBoard.board[y][x].isHit) {
                    targetX = x;
                    targetY = y;
                    break;
                }
            }
        }
    }

    std::cout << GetName() << " wybral pole: (" << targetX << ", " << targetY << ")" << std::endl;
    return {(float)targetX, (float)targetY};
}

void AIPlayer::StartThinking(const GameBoard& ownBoard, const GameBoard& enemyBoard) {
    if (!aiThinking) {
        aiThinking = true;
        // Uruchomienie funkcji ComputeAIMoveInternal w nowym wątku i zwrócenie future
        // std::async pozwala na asynchroniczne wywołanie funkcji
        aiMoveFuture = std::async(std::launch::async, &AIPlayer::ComputeAIMoveInternal, this, ownBoard, enemyBoard);
    }
}

Vector2 AIPlayer::GetAIMoveResult() {
    // Czekaj na wynik i pobierz go
    Vector2 result = aiMoveFuture.get();
    aiThinking = false; // Zakończono myślenie
    return result;
}

bool AIPlayer::MakeMove(GameBoard& enemyBoard) {
    // Ta metoda jest wywoływana, gdy AI ma wykonać ruch.
    // GameSession będzie sprawdzać IsThinking(), a gdy ruch będzie gotowy, pobierze go przez GetAIMoveResult().
    return false; // Zawsze zwracamy false, bo ruch jest zarządzany asynchronicznie
}


