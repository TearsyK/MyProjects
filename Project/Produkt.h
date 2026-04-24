//
// Created by Xkacp on 17.04.2024.
//

#ifndef PROJECT_PRODUKT_H
#define PROJECT_PRODUKT_H
#include "iostream"
#include "sstream"
#include "ObiektBazowy.h"


class Produkt : public ObiektBazowy
{
public:
    std::string nazwa;
    int id;
    int iloscProduktu{};

    Produkt(std::string nazwa, int id);

    void operator = (const int &temp);

    void wpisanie(int iloscProduktu) override;

    [[nodiscard]] std::string getNazwa() const override;

    int getinput() override;

    int getoutput() override;

    int getilosc() override;

    int getfailureChance() override;

    void wypisz() override;

    Produkt operator+=(int value);
};


#endif //PROJECT_PRODUKT_H
