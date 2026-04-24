//
// Created by Xkacp on 17.04.2024.
//

#ifndef PROJECT_SUROWIEC_H
#define PROJECT_SUROWIEC_H
#include "iostream"
#include "sstream"
#include "ObiektBazowy.h"


class Surowiec : public ObiektBazowy
{
public:
    std::string nazwa;
    int id;
    int iloscSurowca{};

    Surowiec(std::string nazwa, int id);

    void wpisanie(int iloscSurowca) override;

    [[nodiscard]] std::string getNazwa() const override;

    int getinput() override;

    int getoutput() override;

    int getilosc() override;

    int getfailureChance() override;

    void wypisz() override;

    Surowiec operator-=(int value);
};


#endif //PROJECT_SUROWIEC_H
