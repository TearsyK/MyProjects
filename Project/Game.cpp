//
// Created by Xkacp on 17.04.2024.
//

#include "Game.h"

Game::Game() : trybPracy(0) {}

void Game::BaseUstawienia(Lista &Maszyny, Lista &Surowce, Lista &Produkty)
{
    int i = 0;
    while (true)
    {
        bool koniecPetli = false;
        std::string nazwa;
        int input;
        int output;
        int ilosc;
        int failureChance;
        std::string nazwaSurowca;
        std::string nazwaProduktu;

        std::cout << "Podaj nazwe maszyny " << i + 1 << ", ilosc jej egzemplarzy, "<< std::endl << "ile surowcu pobiera dziennie, ile zwraca produktu," << std::endl << "szanse na awarie pojedynczej maszyny (wartosc od 0 do 100)," << std::endl << "jaki surowiec pobiera i jaki produkt zwraca:" << std::endl;

        std::cout << "Nazwa maszyny: ";
        std::getline(std::cin >> std::ws, nazwa);

        std::cout << "Ilosc egzemplarzy maszyny: ";
        while (!(std::cin >> ilosc))
        {
            std::cout << "Blad: Wprowadzono niepoprawna wartosc. Podaj liczbe całkowita: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        std::cout << "Pobor surowcow maszyny: ";
        while (!(std::cin >> input))
        {
            std::cout << "Blad: Wprowadzono niepoprawna wartosc. Podaj liczbe całkowita: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        std::cout << "Ile produktu wytwarza: ";
        while (!(std::cin >> output))
        {
            std::cout << "Blad: Wprowadzono niepoprawna wartosc. Podaj liczbe całkowita: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        std::cout << "Szansa na awarie: ";
        while (!(std::cin >> failureChance) || failureChance < 0 || failureChance > 100)
        {
            std::cout << "Blad: Wprowadzono niepoprawna wartosc. Podaj liczbe calkowita z zakresu 0-100: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Nazwa surowca ktory bedzie pobierac: ";
        std::getline(std::cin, nazwaSurowca);

        std::cout << "Nazwa produktu ktory bedzie wytwarzac: ";
        std::getline(std::cin, nazwaProduktu);

        auto *maszyna = new Maszyna(nazwa, input, output, ilosc, failureChance, i);
        auto *surowiec = new Surowiec(nazwaSurowca, i);
        auto *produkt = new Produkt(nazwaProduktu, i);
        Maszyny.insertAtEnd(maszyna);
        Surowce.insertAtEnd(surowiec);
        Produkty.insertAtEnd(produkt);
        std::cout << "Czy chcesz dodac nastepna maszyna?" << std::endl << "T/N" << std::endl;
        std::string confirmation;

        while (true)
        {
            std::cin >> confirmation;
            if (confirmation == "N" || confirmation == "n")
            {
                koniecPetli = true;
                break;
            } else if (confirmation == "T" || confirmation == "t")
                break;
            else
                std::cout << "Wprowadzono zla wartosc" << std::endl;
        }
        if (koniecPetli)
            break;
        i++;
    }
}

void Game::TrybCzasowy(Lista &Maszyny, Lista &Surowce, Lista &Produkty)
{
    BaseUstawienia(Maszyny, Surowce, Produkty);
    std::cout << "Jak dlugo pracowac ma fabryka? (Czas podaj w dniach)" << std::endl;
    int CzasSymulacji;
    while (!(std::cin >> CzasSymulacji))
    {
        std::cout << "Bład: Wprowadzono niepoprawna wartosc. Podaj liczbe całkowita: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    symulacja.TrybCzasowy(CzasSymulacji, Maszyny, Surowce, Produkty);
}

void Game::TrybSurowcowy(Lista &Maszyny, Lista &Surowce, Lista &Produkty)
{
    BaseUstawienia(Maszyny, Surowce, Produkty);
    Lista::modifyListForSurowce(Surowce);
    symulacja.TrybSurowcowy(Maszyny, Surowce, Produkty);
}

void Game::start(Lista &Maszyny, Lista &Surowce, Lista &Produkty)
{
    std::cout << "Wybierz tryb pracy fabryki:" << std::endl << "Czasowy: 0, Surowcowy: 1, Wczytanie z Config.txt: 2" << std::endl;
    while (!(std::cin >> trybPracy) || trybPracy < 0 || trybPracy > 2)
    {
        std::cout << "Blad: Wprowadzono niepoprawna wartosc. Wybierz 0 dla trybu czasowego,1 dla trybu surowcowego lub 2 dla trybu wczytywania: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }


    switch (trybPracy)
    {
        case 0:
            TrybCzasowy(Maszyny, Surowce, Produkty);
            break;
        case 1:
            TrybSurowcowy(Maszyny, Surowce, Produkty);
            break;
        case 2:
            loadConfig();
            break;
        default:
            break;
    }
    saveConfig();

}

void Game::saveConfig()
{
    std::ofstream file("Config.txt");
    if (!file.is_open()) {
        std::cerr << "Nie mozna otworzyc pliku Config.txt do zapisu" << std::endl;
        return;
    }

    auto *currentM = Maszyny.head;
    while (currentM != nullptr) {
        file << "Maszyna " << currentM->data->getNazwa() << " "
             << currentM->data->getinput() << " "
             << currentM->data->getoutput() << " "
             << currentM->data->getilosc() << " "
             << currentM->data->getfailureChance() << std::endl;
        currentM = currentM->next;
    }

    auto *currentS = Surowce.head;
    while (currentS != nullptr) {
        file << "Surowiec " << currentS->data->getNazwa() << " "
             << currentS->data->getilosc() << std::endl;
        currentS = currentS->next;
    }

    auto *currentP = Produkty.head;
    while (currentP != nullptr) {
        file << "Produkt " << currentP->data->getNazwa() << " "
             << currentP->data->getilosc() << std::endl;
        currentP = currentP->next;
    }

    file.close();
}

void Game::loadConfig()
{
    std::ifstream file("Config.txt");
    if (!file.is_open()) {
        std::cerr << "Nie mozna otworzyc pliku Config.txt do odczytu" << std::endl;
        return;
    }

    std::string type;
    while (file >> type) {
        if (type == "Maszyna") {
            std::string nazwa;
            int input, output, ilosc, failureChance;
            file >> nazwa >> input >> output >> ilosc >> failureChance;
            auto *maszyna = new Maszyna(nazwa, input, output, ilosc, failureChance, 0);
            Maszyny.insertAtEnd(maszyna);
        } else if (type == "Surowiec") {
            std::string nazwa;
            int ilosc;
            file >> nazwa >> ilosc;
            auto *surowiec = new Surowiec(nazwa, 0);
            surowiec->wpisanie(ilosc);
            Surowce.insertAtEnd(surowiec);
        } else if (type == "Produkt") {
            std::string nazwa;
            int ilosc;
            file >> nazwa >> ilosc;
            auto *produkt = new Produkt(nazwa, 0);
            produkt->wpisanie(ilosc);
            Produkty.insertAtEnd(produkt);
        }
    }

    file.close();
}
