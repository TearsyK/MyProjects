//
// Created by Xkacp on 12.06.2024.
//

#ifndef FORMA_PLATNOSCI_H
#define FORMA_PLATNOSCI_H

#include <iostream>
#include <string>

class FormaPlatnosci
{
protected:
    double kwota;
    double prowizja;
    std::string waluta;

public:
    FormaPlatnosci(double kw, double pr, const std::string& wal);
    void wyswietl() const;

    // Metody wirtualne
    virtual void wykonajPlatnosc() = 0;
    virtual void naliczOplate() = 0;

    // Przeciążanie operatorów
    FormaPlatnosci& operator=(const FormaPlatnosci& inna);
    FormaPlatnosci& operator+=(const FormaPlatnosci& inna);
    FormaPlatnosci& operator-=(const FormaPlatnosci& inna);
};

class Przelew : public FormaPlatnosci
{
public:
    using FormaPlatnosci::FormaPlatnosci;
    void wykonajPlatnosc() override;
    void naliczOplate() override;

    Przelew& operator=(const Przelew& inna);
    Przelew& operator+=(const Przelew& inna);
    Przelew& operator-=(const Przelew& inna);

    friend Przelew operator+(const Przelew& p1, const Przelew& p2);
    friend Przelew operator-(const Przelew& p1, const Przelew& p2);
};

class PrzyOdbiorze : public FormaPlatnosci
{
public:
    using FormaPlatnosci::FormaPlatnosci;
    void wykonajPlatnosc() override;
    void naliczOplate() override;

    PrzyOdbiorze& operator=(const PrzyOdbiorze& inna);
    PrzyOdbiorze& operator+=(const PrzyOdbiorze& inna);
    PrzyOdbiorze& operator-=(const PrzyOdbiorze& inna);

    friend PrzyOdbiorze operator+(const PrzyOdbiorze& p1, const PrzyOdbiorze& p2);
    friend PrzyOdbiorze operator-(const PrzyOdbiorze& p1, const PrzyOdbiorze& p2);
};

class SystemyPlatnosciElektronicznej : public FormaPlatnosci
{
public:
    using FormaPlatnosci::FormaPlatnosci;
    void wykonajPlatnosc() override;
    void naliczOplate() override;

    SystemyPlatnosciElektronicznej& operator=(const SystemyPlatnosciElektronicznej& inna);
    SystemyPlatnosciElektronicznej& operator+=(const SystemyPlatnosciElektronicznej& inna);
    SystemyPlatnosciElektronicznej& operator-=(const SystemyPlatnosciElektronicznej& inna);

    friend SystemyPlatnosciElektronicznej operator+(const SystemyPlatnosciElektronicznej& p1, const SystemyPlatnosciElektronicznej& p2);
    friend SystemyPlatnosciElektronicznej operator-(const SystemyPlatnosciElektronicznej& p1, const SystemyPlatnosciElektronicznej& p2);
};

#endif // FORMA_PLATNOSCI_H

