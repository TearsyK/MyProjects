//
// Created by Xkacp on 17.04.2024.
//

#include "Surowiec.h"

#include <utility>

    Surowiec::Surowiec(std::string nazwa, int id)
    {
        this -> nazwa = std::move(nazwa);
        this -> id = id;
    }
    void Surowiec::wpisanie(int iloscSurowca)
    {
        this -> iloscSurowca = iloscSurowca;
    }
    std::string Surowiec::getNazwa() const {
        return nazwa;
    }
    int Surowiec::getinput()
    {
        return 0;
    };
    int Surowiec::getoutput()
    {
        return 0;
    }
    int Surowiec::getilosc()
    {
        return iloscSurowca;
    };
    int Surowiec::getfailureChance()
    {
        return 0;
    }
    void Surowiec::wypisz()
    {
        std::cout<<"Zuzycie surowca "<<nazwa<<": "<<iloscSurowca<<std::endl;
    }

    Surowiec Surowiec::operator-=(int value)
    {
        iloscSurowca-=value;
        return *this;
    }