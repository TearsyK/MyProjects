/*
Harmonogram zada� z std::packaged_task
Opis:
Wdro�enie harmonogramu zada�, kt�ry zawiera 3 typy zada�:
Dodawanie dw�ch liczb.
Mno�enie dw�ch liczb.
Znalezienie maksimum dw�ch liczb.
Ka�de zadanie jest opakowane w std::packaged_task i planowane asynchronicznie przy u�yciu std::async.
U�ytkownik wprowadza ci�g liter kt�re s� interpretowane nast�puj�co:
'd' - dodawanie,
'p' - pomnozeneie ,
'm' - maksimum,
'e' - wyjscie
Wyj�cie nast�puje po wykonaniu operacji ju� zaplanowanych (przed wystapieniem e)
najpierw powinny by� wszystkie operacje zaplanowane, nastepnie wszystkie rozpoczete a na koncu wyswietlone wyniki dla losowych liczb
*/
#include <thread>
#include <iostream>
#include <future>
#include <vector>
#include <queue>
#include <functional>
#include <random>
#include <string>
#include <chrono>

// Define the operations
int dodawanie(int arg1, int arg2) { return arg1 + arg2; }
int pomnozenie(int arg1, int arg2) { return arg1 * arg2; }
int maksimum(int arg1, int arg2) { return (arg1 > arg2) ? arg1 : arg2; }

int main() {
    bool running = true;
    while (running) {
        std::vector<std::tuple<char, int, int>> operacje;
        std::string wejscie;

        std::cout << "Wprowadz ciag operacji (gdzie interpretuje:" << std::endl
                  << "'d' - dodawanie, 'p' - pomnozeneie, 'm' - maksimum, 'e' - wyjscie)" << std::endl;
        std::getline(std::cin, wejscie);

        for (const char& c : wejscie) {
            if (!running) break;

            switch (c) {
                case 'd':
                case 'p':
                case 'm':
                    // Generate random arguments for the operation
                    srand(std::chrono::system_clock::now().time_since_epoch().count() % UINT_MAX);
                    operacje.push_back({c, rand() % 100, rand() % 100});
                    break;
                case 'e':
                    running = false;
                    wejscie.clear();
                    continue;
                default:
                    std::cout << "Nieinterpretowalny znak '" << c << "', pomijam." << std::endl;
                    continue;
            }
        }

        std::cout << std::endl << "Czekam na sygnal do rozpoczecia wykonywania zadan zaplanowanych..." << std::endl;
        system("pause");

        if (!operacje.empty()) {
            std::cout << std::endl << "Wykonuje wszystkie zadania..." << std::endl;

            std::vector<std::future<int>> futures;

            // Schedule all tasks asynchronously
            for (const auto& operacja : operacje) {
                char typ_operacji;
                int arg1, arg2;
                std::tie(typ_operacji, arg1, arg2) = operacja;

                // Directly use std::async without std::packaged_task
                switch (typ_operacji) {
                    case 'd': {
                        futures.push_back(std::async(std::launch::async, dodawanie, arg1, arg2));
                        break;
                    }
                    case 'p': {
                        futures.push_back(std::async(std::launch::async, pomnozenie, arg1, arg2));
                        break;
                    }
                    case 'm': {
                        futures.push_back(std::async(std::launch::async, maksimum, arg1, arg2));
                        break;
                    }
                }
            }

            std::cout << std::endl << "Wyniki dla argumentow: " << std::endl;

            // Wait for all tasks to finish and display results
            for (size_t i = 0; i < futures.size(); ++i) {
                int wynik = futures[i].get();
                char typ_operacji;
                int arg1, arg2;
                std::tie(typ_operacji, arg1, arg2) = operacje[i];

                std::cout << arg1 << ", " << arg2 << " |" << typ_operacji << "| = " << wynik << std::endl;
            }

            std::cout << std::endl;
        }
    }
return 0;
}