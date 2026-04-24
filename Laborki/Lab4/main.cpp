#include <iostream>
#include "walidator.h"
#include "formaplat.h"
#include "efekty.h"

int main()
{
    // Test walidatorów
    TelefonKom telefonKom;
    std::cout << "Telefon komorkowy poprawny: " << telefonKom.czyPoprawny("+48123456789") << std::endl;

    Kwota kwota;
    std::cout << "Kwota poprawna: " << kwota.czyPoprawny("123.45") << std::endl;

    Email email;
    std::cout << "Email poprawny: " << email.czyPoprawny("test@example.com") << std::endl;

    TelefonStat telefonStat;
    std::cout << "Telefon stacjonarny poprawny: " << telefonStat.czyPoprawny("123456789") << std::endl;

    LiczbaRzymska liczbaRzymska;
    std::cout << "Liczba rzymska poprawna: " << liczbaRzymska.czyPoprawny("XVI") << std::endl;

    // Test form płatności
    Przelew przelew1(100.0, 5.0, "PLN");
    Przelew przelew2(50.0, 2.5, "PLN");

    Przelew przelew3 = przelew1 + przelew2;
    przelew3.wyswietl();

    PrzyOdbiorze przyOdbiorze1(200.0, 10.0, "PLN");
    PrzyOdbiorze przyOdbiorze2(100.0, 5.0, "PLN");

    PrzyOdbiorze przyOdbiorze3 = przyOdbiorze1 - przyOdbiorze2;
    przyOdbiorze3.wyswietl();

    SystemyPlatnosciElektronicznej platnoscElektroniczna1(150.0, 7.5, "USD");
    SystemyPlatnosciElektronicznej platnoscElektroniczna2(100.0, 5.0, "USD");

    SystemyPlatnosciElektronicznej platnoscElektroniczna3 = platnoscElektroniczna1 + platnoscElektroniczna2;
    platnoscElektroniczna3.wyswietl();

    // Test efektów specjalnych
    Deszcz deszcz;
    deszcz.wykonajEfekt();

    Wybuch wybuch;
    wybuch.wykonajEfekt();

    Blyskawica blysk;
    blysk.wykonajEfekt();

    Ognien ognien;
    ognien.wykonajEfekt();

    Dym dym;
    dym.wykonajEfekt();

    return 0;
}

