/*
Asynchroniczny prosty wyscig obliczeniowy
Zaprogramuj wyscig między 2 Async Graczami, Gracz A musi obliczyc kwadrat numeru a Gracz B sześcian. Funkcja koordynująca podaje obu tę samą losową liczbę z przedziału 1-100, 
Obydwu graczy otrzymuje osobną liczbę. Wątek main wyczekuje wyników obydwu graczy i ogłasza zwyciężce
*/

#include <iostream>
#include <future>
#include <random>
#include <chrono>

int playerA_square(int x)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(100, 200);  // Zakres od 100ms do 200ms
    int delay = dist(gen);
    std::this_thread::sleep_for(std::chrono::milliseconds(delay)); //symulacja opóźnienia
    return x*x*x*x*x;
}

int playerB_cube(int x)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(100, 200);  // Zakres od 100ms do 200ms
    int delay = dist(gen);
    std::this_thread::sleep_for(std::chrono::milliseconds(delay)); //symulacja opóźnienia
    return x*x*x*x*x*x;
}

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    int a_input = 1 + rand() % 100;
    int b_input = 1 + rand() % 100;

    auto startA = std::chrono::steady_clock::now();
    auto futureA = std::async(std::launch::async, playerA_square, a_input);
    auto startB = std::chrono::steady_clock::now();
    auto futureB = std::async(std::launch::async, playerB_cube, b_input);

    bool win = 0;

    while (true) {
        if (futureA.wait_for(std::chrono::milliseconds(1)) == std::future_status::ready) {
            auto timeA = std::chrono::steady_clock::now();
            std::cout << "A finished first in "<< std::chrono::duration_cast<std::chrono::milliseconds>(timeA - startA).count() <<": " << futureA.get() << "\n";
            win = 1;
            break;
        }
        if (futureB.wait_for(std::chrono::milliseconds(1)) == std::future_status::ready) {
            auto timeB = std::chrono::steady_clock::now();
            std::cout << "B finished first in " << std::chrono::duration_cast<std::chrono::milliseconds>(timeB - startB).count() << ": " << futureB.get() << "\n";
            win = 0;
            break;
        }
    }

    if (win) {
        // A wygrało, wyświetl czas dla A
        auto timeB = std::chrono::steady_clock::now();
        std::cout<< "B finished in " << std::chrono::duration_cast<std::chrono::milliseconds>(timeB - startB).count() <<": "<<futureB.get() << "\n";
    } else {
        // B wygrało, wyświetl czas dla B
        auto timeA = std::chrono::steady_clock::now();
        std::cout<< "A finished in " << std::chrono::duration_cast<std::chrono::milliseconds>(timeA - startA).count() <<": "<<futureA.get() << "\n";
    }
    return 0;
}