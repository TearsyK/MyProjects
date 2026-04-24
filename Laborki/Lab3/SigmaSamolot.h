//
// Created by Xkacp on 17.04.2024.
//

#ifndef LAB3_SIGMASAMOLOT_H
#define LAB3_SIGMASAMOLOT_H
#include "iostream"
#include "vector"
#include "SigmaSilnik.h"
#include "SigmaRadio.h"
class SigmaSamolot
{

public:
    static const int SigmaMasa=1337;
    SigmaRadio& sigmaradio;
    SigmaSilnik sigmaSilnik;
    double SigmaPaliwo;
    double SigmaDystans;
    double SigmaX;
    double SigmaY;
    double SigmaZ;
    double SigmaWysokosc;
    std::vector<char> SigmaSygnatura;

    SigmaSamolot(double SigmaPaliwo, double SigmaDystans, double SigmaX, double SigmaY, double SigmaZ, double SigmaWysokosc)
    {
        this -> SigmaPaliwo = SigmaPaliwo;
        this -> SigmaDystans = SigmaDystans;
        this -> SigmaX = SigmaX;
        this -> SigmaY = SigmaY;
        this -> SigmaZ = SigmaZ;
        this -> SigmaWysokosc = SigmaWysokosc;
    }
    SigmaSamolot()
    {
        this -> SigmaPaliwo = 0;
        this -> SigmaDystans = 0;
        this -> SigmaX = 0;
        this -> SigmaY = 0;
        this -> SigmaZ = 0;
        this -> SigmaWysokosc = 0;
    }
    //Odp. do zadania 4: Nie, dla tej klasy nie istnieje jeszcze destruktor
    void SigmaPrint(SigmaSamolot sigma);
    SigmaSamolot SigmaKorekta(SigmaSamolot sigma, double sgmaKorekta);
};


#endif //LAB3_SIGMASAMOLOT_H
