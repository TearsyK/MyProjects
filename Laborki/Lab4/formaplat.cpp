//
// Created by Xkacp on 12.06.2024.
//

#include "formaplat.h"

FormaPlatnosci::FormaPlatnosci(double kw, double pr, const std::string& wal) : kwota(kw), prowizja(pr), waluta(wal)
{
}

void FormaPlatnosci::wyswietl() const
{
    std::cout << "Kwota: " << kwota << ", Prowizja: " << prowizja << ", Waluta: " << waluta << std::endl;
}

FormaPlatnosci& FormaPlatnosci::operator=(const FormaPlatnosci& inna)
{
    if (this == &inna)
        return *this;
    kwota = inna.kwota;
    prowizja = inna.prowizja;
    waluta = inna.waluta;
    std::cout << "Przypisano" << std::endl;
    return *this;
}

FormaPlatnosci& FormaPlatnosci::operator+=(const FormaPlatnosci& inna)
{
    if (waluta != inna.waluta)
        std::cout << "Operacja += nieudana (rozne waluty)" << std::endl;
    else
    {
        kwota += inna.kwota;
        prowizja = (prowizja + inna.prowizja) / 2;
    }
    return *this;
}

FormaPlatnosci& FormaPlatnosci::operator-=(const FormaPlatnosci& inna)
{
    if (waluta != inna.waluta)
        std::cout << "Operacja -= nieudana (rozne waluty)" << std::endl;
    else
    {
        kwota -= inna.kwota;
        prowizja = (prowizja + inna.prowizja) / 2;
    }
    return *this;
}

void Przelew::wykonajPlatnosc()
{
    std::cout << "Wykonano przelew na kwote: " << kwota << " " << waluta << std::endl;
}

void Przelew::naliczOplate()
{
    std::cout << "Naliczono oplate za przelew: " << prowizja << " " << waluta << std::endl;
}

Przelew& Przelew::operator=(const Przelew& inna)
{
    if (this == &inna)
        return *this;
    FormaPlatnosci::operator=(inna);
    return *this;
}

Przelew& Przelew::operator+=(const Przelew& inna)
{
    FormaPlatnosci::operator+=(inna);
    return *this;
}

Przelew& Przelew::operator-=(const Przelew& inna)
{
    FormaPlatnosci::operator-=(inna);
    return *this;
}

Przelew operator+(const Przelew& p1, const Przelew& p2)
{
    if (p1.waluta != p2.waluta)
    {
        std::cout << "Operacja nieudana (rozne waluty)" << std::endl;
        return p1;
    }
    else
        return {p1.kwota + p2.kwota, (p1.prowizja + p2.prowizja) / 2, p1.waluta};
}

Przelew operator-(const Przelew& p1, const Przelew& p2)
{
    if (p1.waluta != p2.waluta)
    {
        std::cout << "Operacja nieudana (rozne waluty)" << std::endl;
        return p1;
    }
    else
        return {p1.kwota - p2.kwota, (p1.prowizja + p2.prowizja) / 2, p1.waluta};
}

void PrzyOdbiorze::wykonajPlatnosc()
{
    std::cout << "Wykonano platnosc przy odbiorze na kwote: " << kwota << " " << waluta << std::endl;
}

void PrzyOdbiorze::naliczOplate()
{
    std::cout << "Naliczono oplate za platnosc przy odbiorze: " << prowizja << " " << waluta << std::endl;
}

PrzyOdbiorze& PrzyOdbiorze::operator=(const PrzyOdbiorze& inna)
{
    if (this == &inna)
        return *this;
    FormaPlatnosci::operator=(inna);
    return *this;
}

PrzyOdbiorze& PrzyOdbiorze::operator+=(const PrzyOdbiorze& inna)
{
    FormaPlatnosci::operator+=(inna);
    return *this;
}

PrzyOdbiorze& PrzyOdbiorze::operator-=(const PrzyOdbiorze& inna)
{
    FormaPlatnosci::operator-=(inna);
    return *this;
}

PrzyOdbiorze operator+(const PrzyOdbiorze& p1, const PrzyOdbiorze& p2)
{
    if (p1.waluta != p2.waluta)
    {
        std::cout << "Operacja nieudana (rozne waluty)" << std::endl;
        return p1;
    }
    else
        return {p1.kwota + p2.kwota, (p1.prowizja + p2.prowizja) / 2, p1.waluta};
}

PrzyOdbiorze operator-(const PrzyOdbiorze& p1, const PrzyOdbiorze& p2)
{
    if (p1.waluta != p2.waluta)
    {
        std::cout << "Operacja nieudana (rozne waluty)" << std::endl;
        return p1;
    }
    else
        return {p1.kwota - p2.kwota, (p1.prowizja + p2.prowizja) / 2, p1.waluta};
}

void SystemyPlatnosciElektronicznej::wykonajPlatnosc()
{
    std::cout << "Wykonano platnosc elektroniczna na kwote: " << kwota << " " << waluta << std::endl;
}

void SystemyPlatnosciElektronicznej::naliczOplate()
{
    std::cout << "Naliczono oplate za platnosc elektroniczna: " << prowizja << " " << waluta << std::endl;
}

SystemyPlatnosciElektronicznej& SystemyPlatnosciElektronicznej::operator=(const SystemyPlatnosciElektronicznej& inna)
{
    if (this == &inna)
        return *this;
    FormaPlatnosci::operator=(inna);
    return *this;
}

SystemyPlatnosciElektronicznej& SystemyPlatnosciElektronicznej::operator+=(const SystemyPlatnosciElektronicznej& inna)
{
    FormaPlatnosci::operator+=(inna);
    return *this;
}

SystemyPlatnosciElektronicznej& SystemyPlatnosciElektronicznej::operator-=(const SystemyPlatnosciElektronicznej& inna)
{
    FormaPlatnosci::operator-=(inna);
    return *this;
}

SystemyPlatnosciElektronicznej operator+(const SystemyPlatnosciElektronicznej& p1, const SystemyPlatnosciElektronicznej& p2)
{
    if (p1.waluta != p2.waluta)
    {
        std::cout << "Operacja nieudana (rozne waluty)" << std::endl;
        return p1;
    }
    else
        return {p1.kwota + p2.kwota, (p1.prowizja + p2.prowizja) / 2, p1.waluta};
}

SystemyPlatnosciElektronicznej operator-(const SystemyPlatnosciElektronicznej& p1, const SystemyPlatnosciElektronicznej& p2)
{
    if (p1.waluta != p2.waluta)
    {
        std::cout << "Operacja nieudana (rozne waluty)" << std::endl;
        return p1;
    }
    else
        return {p1.kwota - p2.kwota, (p1.prowizja + p2.prowizja) / 2, p1.waluta};
}
