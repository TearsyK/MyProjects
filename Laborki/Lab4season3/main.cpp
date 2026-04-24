#include <iostream>
#include <ranges>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>


//1. Ze słów zapisanych w wektorze, wypisz pierwsze 3,
// w których pierwsze 3 litery zawierają literę 'z’. (0.1 pkt)
void ZAD1()
{
    std::vector<std::string> slowa = {"pies", "dom", "kot", "zebra", "zamek", "sok", "lew", "kzajak", "mysz"};
    auto CzyZwPierwszychTrzech = [](const std::string &slowo)
    {
        return slowo.size() >= 3 &&
               std::ranges::find(slowo.begin(), slowo.begin() + 3, 'z') != slowo.begin() + 3;
    };
    auto przefiltrowane = slowa
                          | std::views::filter(CzyZwPierwszychTrzech)
                          | std::views::take(3);
    for (const auto &slowo: przefiltrowane)
        std::cout << slowo << std::endl;
}

//2. Mając listę liczb całkowitych, wykonaj następujące operacje: usuń duplikaty,
// wybierz tylko liczby podzielne przez 3, podnieś je do kwadratu,
// posortuj malejąco, a następnie oblicz i wypisz średnią arytmetyczną przetworzonych wartości. (0.15 pkt)

void ZAD2()
{
    std::vector<int> liczby = {3,6,9,12,3,15,10,18,6,21,5};
    std::ranges::sort(liczby);
    auto nowyOstatni = std::ranges::unique(liczby).begin();
    liczby.erase(nowyOstatni,liczby.end());

    auto poPotegowaniu = liczby
            |std::views::filter([](int x){return x%3 == 0;})
            |std::views::transform([](int x){return x = x * x;});
    std::vector<int> noweLiczby(poPotegowaniu.begin(), poPotegowaniu.end());

    std::ranges::sort(noweLiczby,std::ranges::greater{});

    if (!noweLiczby.empty())
    {
        double suma = std::accumulate(noweLiczby.begin(),noweLiczby.end(),0.0);
        double srednia = suma/noweLiczby.size();
        std::cout << "Przetworzone wartosci: \n";
        for (int val : noweLiczby)
            std::cout << val << " ";
        std::cout << std::endl << "Srednia arytmetyczna: " << srednia << std::endl;
    }
    else
    {
        std::cout << "Brak przetworzonych wartosci. \n";
    }
}

char naDuzaLitere(char c)
{
    return (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c;
}

std::string naDuza(const std::string& str) {
    std::string wynik;
    wynik.reserve(str.size());
    for (char c : str)
        wynik += naDuzaLitere(c);
    return wynik;
}

void ZAD3()
{
    std::vector<std::string> v1 = {"ania", "tomek", "kasia", "jan", "ala"};
    std::vector<std::string> v2 = {"kuba", "tomek", "ola", "jan", "ania"};

    std::vector<std::string> upper_v1, upper_v2;

    for (const auto& imie : v1)
        upper_v1.push_back(naDuza(imie));
    for (const auto& imie : v2)
        upper_v2.push_back(naDuza(imie));

    std::ranges::sort(upper_v1);
    std::ranges::sort(upper_v2);

    std::vector<std::string> wspolne;
    std::ranges::set_intersection(upper_v1, upper_v2, std::back_inserter(wspolne));

    // Usuń ewentualne duplikaty
    auto last = std::ranges::unique(wspolne).begin();
    wspolne.erase(last, wspolne.end());

    // Posortuj alfabetycznie
    std::ranges::sort(wspolne);

    // Wypisz każde imię z długością, zsumuj długości
    int wszystkieLitery = 0;
    for (const auto& imie : wspolne) {
        int len = imie.size();
        wszystkieLitery += len;
        std::cout << imie << "(" << len << ")\n";
    }

    std::cout << "Laczna liczba liter: " << wszystkieLitery << "\n";
}

int main()
{
    ZAD1();
    ZAD2();
    ZAD3();
}
