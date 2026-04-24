#include <iostream>
#include <future>
#include <thread>
#include <vector>
#include <random>
#include <atomic>
#include <chrono>
#include <numeric>

std::atomic<bool> running(true);

void generatorWydarzen(std::promise<double> obietnica)
{
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<double> dist(15.0, 45.0);
    double odczyt = dist(gen);

    if (running)
    {
        obietnica.set_value(odczyt);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void alarm(std::shared_future<double> futureOdczyt)
{
    double odczyt = futureOdczyt.get();
    if (odczyt > 30.0)
    {
        std::cout << "OSTRZEŻENIE: Temperatura " << odczyt << "°C przekroczyła 30°C!" << std::endl;
    }
}

void zapiszOdczyt(std::shared_future<double> futureOdczyt, std::vector<double>& odczyty)
{
    double odczyt = futureOdczyt.get();
    odczyty.push_back(odczyt);
}

void obliczSrednia(std::shared_future<double> futureOdczyt, std::vector<double>& odczyty)
{
    double odczyt = futureOdczyt.get();
    int liczbaOdczytow = odczyty.size();
    int start = std::max(0, liczbaOdczytow - 5);
    int ilosc = std::min(5, liczbaOdczytow);

    double suma = 0.0;
    for (int i = start; i < start + ilosc; i++)
    {
        suma += odczyty[i];
    }

    double srednia = ilosc > 0 ? suma / ilosc : odczyt;
    std::cout << "Średnia z ostatnich " << ilosc << " odczytów: " << srednia << "°C" << std::endl;
}

void sluchajUzytkownika()
{
    std::string input;
    std::getline(std::cin, input);
    running = false;
    std::cout << "[Reakcja od uzytkownika, wylaczanie]" << std::endl;
}

int main()
{
    std::vector<double> odczyty;
    std::thread watekUzytkownika(sluchajUzytkownika);

    while (running)
    {
        std::promise<double> obietnica;
        std::shared_future<double> przyszlyOdczyt = obietnica.get_future();

        std::thread generator(generatorWydarzen, std::move(obietnica));

        auto alarmFuture = std::async(std::launch::async, alarm, przyszlyOdczyt);
        auto zapiszFuture = std::async(std::launch::async, zapiszOdczyt, przyszlyOdczyt, std::ref(odczyty));
        auto sredniaFuture = std::async(std::launch::async, obliczSrednia, przyszlyOdczyt, std::ref(odczyty));

        alarmFuture.get();
        zapiszFuture.get();
        sredniaFuture.get();

        if (generator.joinable())
            generator.join();
    }

    if (watekUzytkownika.joinable())
        watekUzytkownika.join();

    std::cout << "Zapisane odczyty: ";
    for (double odczyt : odczyty) {
        std::cout << odczyt << "°C ";
    }
    std::cout << std::endl;

    return 0;
}