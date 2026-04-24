//
// Created by Xkacp on 17.04.2024.
//

#ifndef PROJECT_MASZYNA_H
#define PROJECT_MASZYNA_H
#include "iostream"
#include "sstream"
#include "ObiektBazowy.h"


class Maszyna : public ObiektBazowy
{
public:
    std::string nazwa;
    int id;
    int input;
    int output;
    int ilosc;
    int failureChance;


    Maszyna(std::string nazwa, int input, int output, int ilosc, int failureChance, int id);

    void wpisanie(int iloscSurowca) override;

    void wypisz() override;

    [[nodiscard]] std::string getNazwa() const override;

    int getinput() override;

    int getoutput() override;

    int getilosc() override;

    int getfailureChance() override;
};


#endif //PROJECT_MASZYNA_H
