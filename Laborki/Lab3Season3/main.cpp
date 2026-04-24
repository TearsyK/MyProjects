#include <iostream>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

// ============================================================
// Zadanie 1: Sprawdzanie istnienia pliku dane.txt
// ============================================================

void zadanie1() {
    std::cout << "=== Zadanie 1: Sprawdzanie istnienia pliku ===\n";
    fs::path filePath = "dane.txt";

    if (fs::exists(filePath))
        std::cout << "Plik istnieje\n";
    else
        std::cout << "Plik nie istnieje\n";

    std::cout << "===============================================\n\n";
}

// ============================================================
// Zadanie 2: Tworzenie folderu backup i kopiowanie pliku
// ============================================================

void zadanie2() {
    std::cout << "=== Zadanie 2: Tworzenie folderu backup i kopiowanie pliku ===\n";
    fs::path backupFolder = "backup";
    fs::path sourceFile = "dane.txt";
    fs::path destFile = backupFolder / "dane_kopia.txt";  // Można zmienić nazwę, np. <nazwa>_kopia.txt

    try {
        // Utwórz folder backup, jeśli nie istnieje
        if (!fs::exists(backupFolder)) {
            if (fs::create_directory(backupFolder))
                std::cout << "Folder backup utworzony.\n";
            else {
                std::cout << "Nie udalo sie utworzyc folderu backup.\n";
                return;
            }
        }

        // Sprawdzenie, czy plik źródłowy istnieje
        if (!fs::exists(sourceFile)) {
            std::cout << "Plik dane.txt nie istnieje w katalogu roboczym.\n";
            return;
        }

        // Kopiowanie pliku do folderu backup
        fs::copy_file(sourceFile, destFile, fs::copy_options::overwrite_existing);

        // Weryfikacja kopiowania
        if (fs::exists(destFile))
            std::cout << "Kopiowanie powiodlo sie.\n";
        else
            std::cout << "Kopiowanie nie powiodlo sie.\n";
    }
    catch (const fs::filesystem_error &ex) {
        std::cerr << "Bład: " << ex.what() << "\n";
    }

    std::cout << "===============================================================\n\n";
}

// ============================================================
// Zadanie 3: Usuwanie folderu wraz z zawartością
// ============================================================

void zadanie3() {
    std::cout << "=== Zadanie 3: Usuwanie folderu wraz z zawartoscia ===\n";
    std::string folderName;
    std::cout << "Podaj nazwe folderu do usuniecia: ";
    std::cin >> folderName;

    fs::path folderPath = folderName;

    // Sprawdzenie, czy katalog istnieje
    if (!fs::exists(folderPath)) {
        std::cout << "Katalog nie istnieje.\n";
        return;
    }

    // Sprawdzenie, czy to jest folder
    if (!fs::is_directory(folderPath)) {
        std::cout << "Podana ściezka nie jest folderem.\n";
        return;
    }

    // Wyświetlenie plików w folderze wraz z rozmiarami oraz sumowanie rozmiarów
    uintmax_t totalSize = 0;
    std::cout << "Pliki w folderze \"" << folderName << "\":\n";
    for (const auto& entry : fs::recursive_directory_iterator(folderPath)) {
        if (fs::is_regular_file(entry.status())) {
            auto fileSize = fs::file_size(entry);
            std::cout << " - " << entry.path().filename().string() << " : " << fileSize << " bajtow\n";
            totalSize += fileSize;
        }
    }
    std::cout << "Suma rozmiarow plikóow: " << totalSize << " bajtow\n";

    // Usuwanie folderu wraz z całą zawartością
    try {
        fs::remove_all(folderPath);
        std::cout << "Folder usuniety.\n";
    }
    catch (const fs::filesystem_error &ex) {
        std::cerr << "Bład przy usuwaniu folderu: " << ex.what() << "\n";
    }

    std::cout << "=====================================================\n\n";
}

// ============================================================
// Funkcja główna z prostym menu do wyboru zadania
// ============================================================

int main() {
    int choice;
    do {
        std::cout << "Wybierz zadanie do wykonania:\n";
        std::cout << "1. Sprawdzanie istnienia pliku (Zadanie 1)\n";
        std::cout << "2. Tworzenie folderu backup i kopiowanie pliku (Zadanie 2)\n";
        std::cout << "3. Usuwanie folderu (Zadanie 3)\n";
        std::cout << "0. Wyjscie\n";
        std::cout << "Twoj wybor: ";
        std::cin >> choice;
        std::cout << "\n";

        switch(choice) {
            case 1:
                zadanie1();
                break;
            case 2:
                zadanie2();
                break;
            case 3:
                zadanie3();
                break;
            case 0:
                std::cout << "Koniec programu.\n";
                break;
            default:
                std::cout << "Nieprawidlowy wybor, sprobuj ponownie.\n";
        }
    } while(choice != 0);

    return 0;
}

