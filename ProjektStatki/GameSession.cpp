#include "GameSession.h"
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <algorithm> // For std::remove_if, std::find_if
#include <cmath>     // For std::round
#include "GameUtils.h"

// --- Stałe przesunięć plansz (używają globalnych stałych z GameUtils.h) ---
const int BOARD_DRAW_OFFSET_X_PLAYER = BOARD_PADDING;
const int BOARD_DRAW_OFFSET_Y_PLAYER = BOARD_PADDING;
const int BOARD_DRAW_OFFSET_X_AI = screenWidth / 2 + BOARD_PADDING;
const int BOARD_DRAW_OFFSET_Y_AI = BOARD_PADDING;

// --- Stałe palety statków ---
const int PALETTE_X = screenWidth - 200;
const int PALETTE_Y = 50;
const int PALETTE_WIDTH = 180;
const int PALETTE_SHIP_SPACING = 10;

GameSession::GameSession()
        : currentGameState(MENU_MAIN), placingShipsStartTime(0.0),
          isDraggingShip(false), currentDraggedShipPtr(nullptr), dragOffset({0,0}),
          currentDraggedShipHorizontal(true), currentDraggedShipPaletteIndex(-1),
          originalShipStartX(-1), originalShipStartY(-1), originalShipHorizontal(true),
          nextShipId(1) {

    player1 = std::make_unique<HumanPlayer>(1, "Gracz");
    playerAI = std::make_unique<AIPlayer>(2, "Komputer");

    InitializeShipPalette();
}

void GameSession::InitializeShipPalette() {
    availableShipsPalette.clear();
    availableShipsPalette.emplace_back(0, 5); // Lotniskowiec (Aircraft Carrier)
    availableShipsPalette.emplace_back(0, 4); // Pancernik (Battleship)
    availableShipsPalette.emplace_back(0, 3); // Niszczyciel (Destroyer)
    availableShipsPalette.emplace_back(0, 3); // Niszczyciel (Destroyer)
    availableShipsPalette.emplace_back(0, 2); // Łódź podwodna (Submarine)
}

void GameSession::Run() {
    while (!WindowShouldClose()) {
        // --- AKTUALIZACJA STANU GRY ---
        switch (currentGameState) {
            case MENU_MAIN: UpdateMenuMain(); break;
            case PLACING_SHIPS: UpdatePlacingShips(); break;
            case PLAYER_TURN: UpdatePlayerTurn(); break;
            case AI_TURN: UpdateAITurn(); break;
            case GAME_OVER: UpdateGameOver(); break;
        }

        // --- RYSOWANIE ---
        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (currentGameState) {
            case MENU_MAIN: DrawMenuMain(); break;
            case PLACING_SHIPS: DrawPlacingShips(); break;
            case PLAYER_TURN: DrawPlayerTurn(); break;
            case AI_TURN: DrawAITurn(); break;
            case GAME_OVER: DrawGameOver(); break;
        }

        DrawText("ESC aby wyjsc", screenWidth - MeasureText("ESC aby wyjsc", 15) - 10, screenHeight - 25, 15, GRAY);
        EndDrawing();
    }

    // --- LOGIKA ZAPISU PRZY ZAMKNIĘCIU GRY ---
    // Zapisz stan gry tylko, jeśli jest w trakcie rozgrywki (nie w menu, nie w fazie rozmieszczania, nie po zakończeniu)
    if (currentGameState == PLAYER_TURN || currentGameState == AI_TURN) {
        SaveGame(static_cast<int>(currentGameState), player1->GetBoard(), playerAI->GetBoard());
    }
}

void GameSession::ResetGame() {
    // Resetuje plansze do czystego stanu
    player1->GetBoard().Reset();
    playerAI->GetBoard().Reset();

    // Uzupełnia paletę statków dla gracza
    InitializeShipPalette();

    // Rozmieszcza statki AI losowo (dla nowej gry)
    playerAI->InitializeShips(playerAI->GetBoard());

    currentGameState = PLACING_SHIPS;
    placingShipsStartTime = GetTime();
    isDraggingShip = false;
    currentDraggedShipPtr = nullptr;
    currentDraggedShipPaletteIndex = -1;
    originalShipStartX = -1;
    originalShipStartY = -1;
    originalShipHorizontal = true;
    nextShipId = 1;
    std::cout << "Gra zresetowana. Przejscie do fazy rozmieszczania statkow." << std::endl;
}

void GameSession::UpdateMenuMain() {
    Rectangle newGameBtn = { (float)screenWidth / 2 - 100, (float)screenHeight / 2 - 50, 200, 40 };
    Rectangle loadGameBtn = { (float)screenWidth / 2 - 100, (float)screenHeight / 2 + 10, 200, 40 };
    Rectangle exitBtn = { (float)screenWidth / 2 - 100, (float)screenHeight / 2 + 70, 200, 40 };

    bool saveExists = CheckSaveFileExists(); // Sprawdź istnienie pliku zapisu

    if (DrawButton(newGameBtn, "Nowa Gra", BUTTON_COLOR, BUTTON_HOVER_COLOR)) {
        DeleteSaveGame(); // Usuń poprzedni zapis, jeśli rozpoczynamy nową grę
        ResetGame(); // Resetuje i przechodzi do PLACING_SHIPS
    }
    // Przycisk "Wczytaj Grę" jest aktywny tylko, jeśli istnieje plik zapisu
    if (DrawButton(loadGameBtn, "Wczytaj Gre", BUTTON_COLOR, BUTTON_HOVER_COLOR, saveExists)) {
        // Wczytaj pełny stan gry (stan tury, plansze i statki)
        if (LoadGame(reinterpret_cast<int&>(currentGameState), player1->GetBoard(), playerAI->GetBoard())) {
            std::cout << "Wczytano gre, stan: " << currentGameState << std::endl;
            // Po wczytaniu, paleta gracza powinna być pusta, ponieważ statki są już na planszy
            availableShipsPalette.clear();
            // Resetujemy stan drag&drop
            isDraggingShip = false;
            currentDraggedShipPtr = nullptr;
            currentDraggedShipPaletteIndex = -1;
            originalShipStartX = -1;
            originalShipStartY = -1;
            originalShipHorizontal = true;

            // Po udanym wczytaniu, usuwamy plik zapisu
            DeleteSaveGame();
        } else {
            std::cout << "Nie udalo sie wczytac gry." << std::endl;
        }
    }
    if (DrawButton(exitBtn, "Wyjdz", BUTTON_COLOR, BUTTON_HOVER_COLOR)) {
        CloseWindow();
    }
}

void GameSession::UpdatePlacingShips() {
    // Ta logika jest odpowiedzialna za drag&drop
    // dla nowych statków z palety i przemieszczania istniejących.

    Vector2 mousePos = GetMousePosition();
    bool leftClickPressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    bool leftClickReleased = IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
    bool rightClickPressed = IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);

    // --- Obsłuha drag&drop ---
    if (leftClickPressed && !isDraggingShip) {
        // 1. Sprawdzamy czy statek znajduje się na palecie
        int currentYOffset = PALETTE_Y;
        for (size_t i = 0; i < availableShipsPalette.size(); ++i) {
            const Ship& ship = availableShipsPalette[i];

            // Obliczamy efektywną wysokość i szerokość dla detekcji udeżenia na podstawie orientacji
            float effectiveWidth = ship.isHorizontal ? (float)ship.length * CELL_SIZE : (float)CELL_SIZE;
            float effectiveHeight = ship.isHorizontal ? (float)CELL_SIZE : (float)ship.length * CELL_SIZE;

            Rectangle shipRect = { (float)PALETTE_X + (PALETTE_WIDTH - effectiveWidth) / 2.0f, (float)currentYOffset, effectiveWidth, effectiveHeight };

            if (CheckCollisionPointRec(mousePos, shipRect)) {
                isDraggingShip = true;
                currentDraggedShipPaletteIndex = i;
                currentDraggedShipPtr = &availableShipsPalette[i];
                dragOffset = { mousePos.x - shipRect.x, mousePos.y - shipRect.y };
                currentDraggedShipHorizontal = currentDraggedShipPtr->isHorizontal;

                // Reset oryginalnych danych pozycji statku, ponieważ jest to nowy statek z palety
                originalShipStartX = -1;
                originalShipStartY = -1;
                originalShipHorizontal = true;
                break;
            }
            currentYOffset += effectiveHeight + PALETTE_SHIP_SPACING;
        }

        // 2. Sprawdzamy czy przesuwamy już ustawiony statek
        if (!isDraggingShip) {
            int boardGridX = (mousePos.x - BOARD_DRAW_OFFSET_X_PLAYER) / CELL_SIZE;
            int boardGridY = (mousePos.y - BOARD_DRAW_OFFSET_Y_PLAYER) / CELL_SIZE;

            if (boardGridX >= 0 && boardGridX < BOARD_SIZE && boardGridY >= 0 && boardGridY < BOARD_SIZE) {
                if (player1->GetBoard().board[boardGridY][boardGridX].hasShip) {
                    int shipId = player1->GetBoard().board[boardGridY][boardGridX].shipId;
                    // Szukamy statku na vectorze board.ships
                    auto it = std::find_if(player1->GetBoard().ships.begin(), player1->GetBoard().ships.end(),
                                           [shipId](const Ship& s) { return s.id == shipId; });

                    if (it != player1->GetBoard().ships.end()) {
                        isDraggingShip = true;
                        currentDraggedShipPtr = &(*it);
                        currentDraggedShipHorizontal = it->isHorizontal;
                        currentDraggedShipPaletteIndex = -1;

                        int shipTopLeftX = BOARD_SIZE;
                        int shipTopLeftY = BOARD_SIZE;
                        for (const auto& occupiedCell : it->occupiedCells) {
                            if (occupiedCell.x < shipTopLeftX) shipTopLeftX = (int)occupiedCell.x;
                            if (occupiedCell.y < shipTopLeftY) shipTopLeftY = (int)occupiedCell.y;
                        }
                        originalShipStartX = shipTopLeftX;
                        originalShipStartY = shipTopLeftY;
                        originalShipHorizontal = it->isHorizontal;

                        // Obliczamy dragOffSet od myszki do pozycji statku
                        float shipDrawX = BOARD_DRAW_OFFSET_X_PLAYER + originalShipStartX * CELL_SIZE;
                        float shipDrawY = BOARD_DRAW_OFFSET_Y_PLAYER + originalShipStartY * CELL_SIZE;
                        dragOffset = { mousePos.x - shipDrawX, mousePos.y - shipDrawY };


                        for (const auto& oc : it->occupiedCells) {
                            player1->GetBoard().board[(int)oc.y][(int)oc.x].hasShip = false;
                            player1->GetBoard().board[(int)oc.y][(int)oc.x].shipId = 0;
                        }
                        it->occupiedCells.clear();
                    }
                }
            }
        }
    }


    // --- Obsługa upuszczania statku (puszczenie myszy) ---
    if (isDraggingShip && leftClickReleased) {
        // Obliczenie surowej pozycji statku adekwatnie to myszki
        float unSnappedTL_X = mousePos.x - dragOffset.x;
        float unSnappedTL_Y = mousePos.y - dragOffset.y;

        // Obliczanie wizualnej pozycji statku
        float shipVisualHalfWidth = (currentDraggedShipHorizontal ? currentDraggedShipPtr->length * CELL_SIZE : CELL_SIZE) / 2.0f;
        float shipVisualHalfHeight = (currentDraggedShipHorizontal ? CELL_SIZE : currentDraggedShipPtr->length * CELL_SIZE) / 2.0f;

        // Obliczanie "środka ducha" na koordynaty relatywne do planszy
        float ghostCenterXRelativeToBoard = unSnappedTL_X + shipVisualHalfWidth - BOARD_DRAW_OFFSET_X_PLAYER;
        float ghostCenterYRelativeToBoard = unSnappedTL_Y + shipVisualHalfHeight - BOARD_DRAW_OFFSET_Y_PLAYER;

        // --- Logika przyciągania do grida ---
        float snappedCenterBoardPxX;
        float snappedCenterBoardPxY;

        if (currentDraggedShipHorizontal) {
            if (currentDraggedShipPtr->length % 2 != 0) {
                snappedCenterBoardPxX = std::round(ghostCenterXRelativeToBoard / CELL_SIZE - 0.5f) * CELL_SIZE + CELL_SIZE / 2.0f;
            } else {
                snappedCenterBoardPxX = std::round(ghostCenterXRelativeToBoard / CELL_SIZE) * CELL_SIZE;
            }

            snappedCenterBoardPxY = std::round(ghostCenterYRelativeToBoard / CELL_SIZE - 0.5f) * CELL_SIZE + CELL_SIZE / 2.0f;
        } else {

            snappedCenterBoardPxX = std::round(ghostCenterXRelativeToBoard / CELL_SIZE - 0.5f) * CELL_SIZE + CELL_SIZE / 2.0f;
            if (currentDraggedShipPtr->length % 2 != 0) {
                snappedCenterBoardPxY = std::round(ghostCenterYRelativeToBoard / CELL_SIZE - 0.5f) * CELL_SIZE + CELL_SIZE / 2.0f;
            } else {
                snappedCenterBoardPxY = std::round(ghostCenterYRelativeToBoard / CELL_SIZE) * CELL_SIZE;
            }
        }


        float snappedDrawX = snappedCenterBoardPxX - shipVisualHalfWidth + BOARD_DRAW_OFFSET_X_PLAYER;
        float snappedDrawY = snappedCenterBoardPxY - shipVisualHalfHeight + BOARD_DRAW_OFFSET_Y_PLAYER;


        int targetCellX = static_cast<int>(std::round((snappedDrawX - BOARD_DRAW_OFFSET_X_PLAYER) / CELL_SIZE));
        int targetCellY = static_cast<int>(std::round((snappedDrawY - BOARD_DRAW_OFFSET_Y_PLAYER) / CELL_SIZE));

        bool placedSuccessfully = false;

        if (snappedDrawX >= BOARD_DRAW_OFFSET_X_PLAYER && snappedDrawX < BOARD_DRAW_OFFSET_X_PLAYER + BOARD_SIZE * CELL_SIZE &&
            snappedDrawY >= BOARD_DRAW_OFFSET_Y_PLAYER && snappedDrawY < BOARD_DRAW_OFFSET_Y_PLAYER + BOARD_SIZE * CELL_SIZE) {

            // Jeżeli jest to statek z palety, nadaj nowe ID
            if (currentDraggedShipPaletteIndex != -1) {
                currentDraggedShipPtr->id = nextShipId++;
            }


            // Jeżeli przeciągamy istniejący statek, podaj ID to excluda
            int shipIdToExclude = (currentDraggedShipPaletteIndex == -1) ? currentDraggedShipPtr->id : 0;
            placedSuccessfully = player1->GetBoard().PlaceShip(*currentDraggedShipPtr, targetCellX, targetCellY, currentDraggedShipHorizontal);

            if (placedSuccessfully) {
                std::cout << "Statek umieszczony pomyslnie! ID: " << currentDraggedShipPtr->id << std::endl;
                // Jeżeli statek był z palety, usuwamy go z niej
                if (currentDraggedShipPaletteIndex != -1) {
                    availableShipsPalette.erase(availableShipsPalette.begin() + currentDraggedShipPaletteIndex);
                }
            } else {
                std::cerr << "Nie udalo sie umiescic statku. Kolizja lub poza plansza." << std::endl;
                // Jeżeli nie udało postawić statku, to wraca na oryginalną pozycję
                if (currentDraggedShipPaletteIndex == -1 && originalShipStartX != -1) {
                    player1->GetBoard().PlaceShip(*currentDraggedShipPtr, originalShipStartX, originalShipStartY, originalShipHorizontal);
                    std::cout << "Statek przywrócony do oryginalnej pozycji." << std::endl;
                }
            }
        } else {
            std::cerr << "Statek upuszczony poza plansza gracza." << std::endl;
            // Jeżeli nie udało postawić statku (ponieważ jest poza planszą), to wraca na oryginalną pozycję
            if (currentDraggedShipPaletteIndex == -1 && originalShipStartX != -1) {
                player1->GetBoard().PlaceShip(*currentDraggedShipPtr, originalShipStartX, originalShipStartY, originalShipHorizontal);
                std::cout << "Statek przywrócony do oryginalnej pozycji." << std::endl;
            }
            // Jeżeli był brany z palety, to nie trzeba nic robić bo sam tam wróci
        }

        isDraggingShip = false;
        currentDraggedShipPtr = nullptr;
        currentDraggedShipPaletteIndex = -1;
        originalShipStartX = -1;
        originalShipStartY = -1;
        originalShipHorizontal = true;
    }


    // --- Obracanie statku (prawy przycisk myszy) ---
    // Obracanie powinno być możliwe tylko gdy go nie przeciągamy
    if (rightClickPressed && !isDraggingShip) {
        // 1. Sprawdzamy czy klikneliśmy na statek na palecie
        int currentYOffset = PALETTE_Y;
        for (Ship& ship : availableShipsPalette) {
            float effectiveWidth = ship.isHorizontal ? (float)ship.length * CELL_SIZE : (float)CELL_SIZE;
            float effectiveHeight = ship.isHorizontal ? (float)CELL_SIZE : (float)ship.length * CELL_SIZE;
            Rectangle shipRect = { (float)PALETTE_X + (PALETTE_WIDTH - effectiveWidth) / 2.0f, (float)currentYOffset, effectiveWidth, effectiveHeight };

            if (CheckCollisionPointRec(mousePos, shipRect)) {
                ship.isHorizontal = !ship.isHorizontal; // Toggle orientation
                std::cout << "Obrócono statek w palecie. Nowa orientacja: " << (ship.isHorizontal ? "Pozioma" : "Pionowa") << std::endl;
                break; //bracamy tylko jeden statek
            }
            currentYOffset += effectiveHeight + PALETTE_SHIP_SPACING;
        }

        // 2. Sprawdzamy czy klikneliśmy na statek na plansz
        if (!isDraggingShip) {
            int boardGridX = (mousePos.x - BOARD_DRAW_OFFSET_X_PLAYER) / CELL_SIZE;
            int boardGridY = (mousePos.y - BOARD_DRAW_OFFSET_Y_PLAYER) / CELL_SIZE;

            if (boardGridX >= 0 && boardGridX < BOARD_SIZE && boardGridY >= 0 && boardGridY < BOARD_SIZE) {
                if (player1->GetBoard().board[boardGridY][boardGridX].hasShip) {
                    int shipId = player1->GetBoard().board[boardGridY][boardGridX].shipId;
                    auto it = std::find_if(player1->GetBoard().ships.begin(), player1->GetBoard().ships.end(),
                                           [shipId](const Ship& s) { return s.id == shipId; });

                    if (it != player1->GetBoard().ships.end()) {
                        Ship& shipToRotate = *it;

                        int shipTopLeftX = BOARD_SIZE;
                        int shipTopLeftY = BOARD_SIZE;
                        for(const auto& oc : shipToRotate.occupiedCells) {
                            if (oc.x < shipTopLeftX) shipTopLeftX = (int)oc.x;
                            if (oc.y < shipTopLeftY) shipTopLeftY = (int)oc.y;
                        }

                        bool originalPlacedOrientation = shipToRotate.isHorizontal;

                        for (const auto& oc : shipToRotate.occupiedCells) {
                            player1->GetBoard().board[(int)oc.y][(int)oc.x].hasShip = false;
                            player1->GetBoard().board[(int)oc.y][(int)oc.x].shipId = 0;
                        }
                        shipToRotate.occupiedCells.clear();

                        bool newOrientation = !originalPlacedOrientation;

                        bool rotateSuccess = player1->GetBoard().PlaceShip(
                                shipToRotate, shipTopLeftX, shipTopLeftY, newOrientation
                        );

                        if (rotateSuccess) {
                            shipToRotate.isHorizontal = newOrientation; // Update ship's orientation permanently
                            std::cout << "Obrócono umieszczony statek. Nowa orientacja: " << (shipToRotate.isHorizontal ? "Pozioma" : "Pionowa") << std::endl;
                        } else {
                            std::cerr << "Nie udalo sie obrocic umieszczonego statku. Kolizja lub poza plansza. Przywracanie oryginalnej pozycji." << std::endl;
                            player1->GetBoard().PlaceShip(
                                    shipToRotate, shipTopLeftX, shipTopLeftY, originalPlacedOrientation
                            );
                        }
                    }
                }
            }
        }
    }

    // --- Zakoncz Rozmieszczanie ---
    // Przycisk pojawia wtedy i tylko wtedy kiedy paleta jest pusta oraz 5 statków zostało rozmieszczonych
    if (availableShipsPalette.empty() && player1->GetBoard().ships.size() == 5) {
        Rectangle finishBtn = { (float)screenWidth / 2 - 150, (float)screenHeight - 40, 300, 30 };
        if (DrawButton(finishBtn, "Zakoncz Rozmieszczanie", GREEN, LIME)) {
            currentGameState = PLAYER_TURN; // Go to player's turn
            std::cout << "Wszystkie statki rozmieszczone. Rozpoczecie gry!" << std::endl;
        }
    }
}


void GameSession::UpdatePlayerTurn() {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();

        int aiBoardOffsetX = screenWidth / 2 + BOARD_PADDING;
        int aiBoardOffsetY = BOARD_PADDING;

        if (mousePos.x >= aiBoardOffsetX && mousePos.x < aiBoardOffsetX + BOARD_SIZE * CELL_SIZE &&
            mousePos.y >= aiBoardOffsetY && mousePos.y < aiBoardOffsetY + BOARD_SIZE * CELL_SIZE) {

            int cellX = (mousePos.x - aiBoardOffsetX) / CELL_SIZE;
            int cellY = (mousePos.y - aiBoardOffsetY) / CELL_SIZE;

            if (playerAI->GetBoard().ReceiveShot(cellX, cellY)) {
                if (playerAI->GetBoard().AreAllShipsSunk()) {
                    currentGameState = GAME_OVER;
                    std::cout << player1->GetName() << " wygral!" << std::endl;
                }
                SaveGame(static_cast<int>(currentGameState), player1->GetBoard(), playerAI->GetBoard());
            } else {
                currentGameState = AI_TURN;
                playerAI->StartThinking(playerAI->GetBoard(), player1->GetBoard());
                SaveGame(static_cast<int>(currentGameState), player1->GetBoard(), playerAI->GetBoard());
            }
        }
    }
}

void GameSession::UpdateAITurn() {
    if (playerAI->IsThinking()) {
        if (playerAI->aiMoveFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            Vector2 aiShot = playerAI->GetAIMoveResult();

            if (player1->GetBoard().ReceiveShot(aiShot.x, aiShot.y)) {
                if (player1->GetBoard().AreAllShipsSunk()) {
                    currentGameState = GAME_OVER;
                    std::cout << playerAI->GetName() << " wygral!" << std::endl;
                } else {
                    playerAI->StartThinking(playerAI->GetBoard(), player1->GetBoard());
                }
                SaveGame(static_cast<int>(currentGameState), player1->GetBoard(), playerAI->GetBoard());
            } else {
                currentGameState = PLAYER_TURN;
                SaveGame(static_cast<int>(currentGameState), player1->GetBoard(), playerAI->GetBoard());
            }
        }
    }
}

void GameSession::UpdateGameOver() {
    if (IsKeyPressed(KEY_R)) {
        ResetGame();
    }
    if (IsKeyPressed(KEY_M)) {
        currentGameState = MENU_MAIN;
    }
    DeleteSaveGame(); // Usuń plik zapisu po zakończeniu gry
}

// --- Rysowanie ---

void GameSession::DrawMenuMain() const {
    DrawText("Statki", screenWidth / 2 - MeasureText("Statki", 80) / 2, screenHeight / 2 - 180, 80, DARKBLUE);

    Rectangle newGameBtn = { (float)screenWidth / 2 - 100, (float)screenHeight / 2 - 50, 200, 40 };
    Rectangle loadGameBtn = { (float)screenWidth / 2 - 100, (float)screenHeight / 2 + 10, 200, 40 };
    Rectangle exitBtn = { (float)screenWidth / 2 - 100, (float)screenHeight / 2 + 70, 200, 40 };
    DrawButton(newGameBtn, "Nowa Gra", BUTTON_COLOR, BUTTON_HOVER_COLOR);
    DrawButton(loadGameBtn, "Wczytaj Gre", BUTTON_COLOR, BUTTON_HOVER_COLOR, CheckSaveFileExists()); // Przycisk aktywny tylko jeśli istnieje zapis
    DrawButton(exitBtn, "Wyjdz", BUTTON_COLOR, BUTTON_HOVER_COLOR);
}

void GameSession::DrawPlacingShips() const {
    DrawText("Rozmiesc swoje statki!", screenWidth / 2 - MeasureText("Rozmiesc swoje statki!", 30) / 2, screenHeight - 60, 30, DARKGREEN);
    DrawBattleshipBoard(player1->GetBoard(), BOARD_DRAW_OFFSET_X_PLAYER, BOARD_DRAW_OFFSET_Y_PLAYER, true);

    // Rysujemy palete
    DrawRectangle(PALETTE_X - 10, PALETTE_Y - 10, PALETTE_WIDTH + 20, screenHeight - PALETTE_Y + 20, LIGHTGRAY); // tło dla palety
    DrawText("Statki do wyboru:", PALETTE_X, PALETTE_Y - 30, 20, DARKBLUE);

    int currentYOffset = PALETTE_Y;
    for (const auto& ship : availableShipsPalette) {
        float drawX = PALETTE_X + (PALETTE_WIDTH - (ship.isHorizontal ? ship.length : 1) * CELL_SIZE) / 2.0f;
        DrawShipRect(ship, drawX, currentYOffset, SHIP_COLOR, ship.isHorizontal, CELL_SIZE);
        currentYOffset += (ship.isHorizontal ? CELL_SIZE : ship.length * CELL_SIZE) + PALETTE_SHIP_SPACING;
    }

    // Rysowanie przesuwanych statków
    if (isDraggingShip && currentDraggedShipPtr != nullptr) {
        Vector2 mousePos = GetMousePosition();

        float unSnappedTL_X = mousePos.x - dragOffset.x;
        float unSnappedTL_Y = mousePos.y - dragOffset.y;

        float shipVisualHalfWidth = (currentDraggedShipHorizontal ? currentDraggedShipPtr->length * CELL_SIZE : CELL_SIZE) / 2.0f;
        float shipVisualHalfHeight = (currentDraggedShipHorizontal ? CELL_SIZE : currentDraggedShipPtr->length * CELL_SIZE) / 2.0f;

        float ghostCenterXRelativeToBoard = unSnappedTL_X + shipVisualHalfWidth - BOARD_DRAW_OFFSET_X_PLAYER;
        float ghostCenterYRelativeToBoard = unSnappedTL_Y + shipVisualHalfHeight - BOARD_DRAW_OFFSET_Y_PLAYER;

        // --- Logika przyciągania do grida ---
        float snappedCenterBoardPxX;
        float snappedCenterBoardPxY;

        if (currentDraggedShipHorizontal) {
            if (currentDraggedShipPtr->length % 2 != 0) {
                snappedCenterBoardPxX = std::round(ghostCenterXRelativeToBoard / CELL_SIZE - 0.5f) * CELL_SIZE + CELL_SIZE / 2.0f;
            } else {
                snappedCenterBoardPxX = std::round(ghostCenterXRelativeToBoard / CELL_SIZE) * CELL_SIZE;
            }
            snappedCenterBoardPxY = std::round(ghostCenterYRelativeToBoard / CELL_SIZE - 0.5f) * CELL_SIZE + CELL_SIZE / 2.0f;
        } else {
            snappedCenterBoardPxX = std::round(ghostCenterXRelativeToBoard / CELL_SIZE - 0.5f) * CELL_SIZE + CELL_SIZE / 2.0f;
            if (currentDraggedShipPtr->length % 2 != 0) {
                snappedCenterBoardPxY = std::round(ghostCenterYRelativeToBoard / CELL_SIZE - 0.5f) * CELL_SIZE + CELL_SIZE / 2.0f;
            } else {
                snappedCenterBoardPxY = std::round(ghostCenterYRelativeToBoard / CELL_SIZE) * CELL_SIZE;
            }
        }

        float snappedDrawX = snappedCenterBoardPxX - shipVisualHalfWidth + BOARD_DRAW_OFFSET_X_PLAYER;
        float snappedDrawY = snappedCenterBoardPxY - shipVisualHalfHeight + BOARD_DRAW_OFFSET_Y_PLAYER;

        int targetCellX = static_cast<int>(std::round((snappedDrawX - BOARD_DRAW_OFFSET_X_PLAYER) / CELL_SIZE));
        int targetCellY = static_cast<int>(std::round((snappedDrawY - BOARD_DRAW_OFFSET_Y_PLAYER) / CELL_SIZE));

        Color ghostColor = RED;

        int shipIdToExclude = (currentDraggedShipPaletteIndex == -1) ? currentDraggedShipPtr->id : 0;
        if (CheckShipPlacementValidity(player1->GetBoard(), currentDraggedShipPtr->length, targetCellX, targetCellY, currentDraggedShipHorizontal, shipIdToExclude)) {
            ghostColor = GREEN;
        }

        DrawShipVisual(currentDraggedShipPtr->length, snappedDrawX, snappedDrawY, Fade(ghostColor, 0.7f), currentDraggedShipHorizontal, CELL_SIZE, true);
    }

    // "Zakoncz Rozmieszczanie"
    if (availableShipsPalette.empty() && player1->GetBoard().ships.size() == 5) {
        Rectangle finishBtn = { (float)screenWidth / 2 - 150, (float)screenHeight - 40, 300, 30 };
        DrawButton(finishBtn, "Zakoncz Rozmieszczanie", GREEN, LIME);
    }
}

void GameSession::DrawPlayerTurn() const {
    DrawText("Twoja Plansza", BOARD_DRAW_OFFSET_X_PLAYER + (BOARD_SIZE * CELL_SIZE) / 2 - MeasureText("Twoja Plansza", 20) / 2, BOARD_DRAW_OFFSET_Y_PLAYER - 25, 20, TEXT_COLOR);
    DrawBattleshipBoard(player1->GetBoard(), BOARD_DRAW_OFFSET_X_PLAYER, BOARD_DRAW_OFFSET_Y_PLAYER, true);

    DrawText("Plansza Przeciwnika", BOARD_DRAW_OFFSET_X_AI + (BOARD_SIZE * CELL_SIZE) / 2 - MeasureText("Plansja Przeciwnika", 20) / 2, BOARD_DRAW_OFFSET_Y_AI - 25, 20, TEXT_COLOR);
    DrawBattleshipBoard(playerAI->GetBoard(), BOARD_DRAW_OFFSET_X_AI, BOARD_DRAW_OFFSET_Y_AI, false);

    DrawText("Twoja tura! Kliknij na plansze przeciwnika.", screenWidth / 2 - MeasureText("Twoja tura! Kliknij na plansze przeciwnika.", 30) / 2, screenHeight - 60, 30, GREEN);
}

void GameSession::DrawAITurn() const {
    DrawText("Twoja Plansza", BOARD_DRAW_OFFSET_X_PLAYER + (BOARD_SIZE * CELL_SIZE) / 2 - MeasureText("Twoja Plansza", 20) / 2, BOARD_DRAW_OFFSET_Y_PLAYER - 25, 20, TEXT_COLOR);
    DrawBattleshipBoard(player1->GetBoard(), BOARD_DRAW_OFFSET_X_PLAYER, BOARD_DRAW_OFFSET_Y_PLAYER, true);

    DrawText("Plansza Przeciwnika", BOARD_DRAW_OFFSET_X_AI + (BOARD_SIZE * CELL_SIZE) / 2 - MeasureText("Plansja Przeciwnika", 20) / 2, BOARD_DRAW_OFFSET_Y_AI - 25, 20, TEXT_COLOR);
    DrawBattleshipBoard(playerAI->GetBoard(), BOARD_DRAW_OFFSET_X_AI, BOARD_DRAW_OFFSET_Y_AI, false);

    DrawText("Tura AI...", screenWidth / 2 - MeasureText("Tura AI...", 30) / 2, screenHeight - 60, 30, ORANGE);
    if (playerAI->IsThinking()) {
        DrawText("AI mysli...", screenWidth / 2 - MeasureText("AI mysli...", 20) / 2, screenHeight - 20, 20, DARKGRAY);
    }
}

void GameSession::DrawGameOver() const {
    DrawText("Twoja Plansza", BOARD_DRAW_OFFSET_X_PLAYER + (BOARD_SIZE * CELL_SIZE) / 2 - MeasureText("Twoja Plansza", 20) / 2, BOARD_DRAW_OFFSET_Y_PLAYER - 25, 20, TEXT_COLOR);
    DrawBattleshipBoard(player1->GetBoard(), BOARD_DRAW_OFFSET_X_PLAYER, BOARD_DRAW_OFFSET_Y_PLAYER, true);

    DrawText("Plansza Przeciwnika", BOARD_DRAW_OFFSET_X_AI + (BOARD_SIZE * CELL_SIZE) / 2 - MeasureText("Plansja Przeciwnika", 20) / 2, BOARD_DRAW_OFFSET_Y_AI - 25, 20, TEXT_COLOR);
    DrawBattleshipBoard(playerAI->GetBoard(), BOARD_DRAW_OFFSET_X_AI, BOARD_DRAW_OFFSET_Y_AI, true);

    if (player1->GetBoard().AreAllShipsSunk()) {
        DrawText("PRZEGRALES!", screenWidth / 2 - MeasureText("PRZEGRALES!", 50) / 2, screenHeight - 100, 50, RED);
    } else {
        DrawText("WYGRALES!", screenWidth / 2 - MeasureText("WYGRALES!", 50) / 2, screenHeight - 100, 50, GREEN);
    }
    DrawText("Nacisnij 'R' aby zagrac ponownie", screenWidth / 2 - MeasureText("Nacisnij 'R' aby zagrac ponownie", 20) / 2, screenHeight - 40, 20, DARKGRAY);
    DrawText("Nacisnij 'M' aby wrocic do menu", screenWidth / 2 - MeasureText("Nacisnij 'M' aby wrocic do menu", 20) / 2, screenHeight - 20, 20, DARKGRAY);
}

