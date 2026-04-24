//
// Created by Xkacp on 17.04.2024.
//

#include "Produkt.h"



    Produkt::Produkt(std::string nazwa, int id)
    {
        this -> nazwa = std::move(nazwa);
        this -> id = id;
    }

    void Produkt::operator = (const int &temp)
    {
        iloscProduktu = temp;
    }

    void Produkt::wpisanie(int iloscProduktu)
    {
        this -> iloscProduktu = iloscProduktu;
    }

    std::string Produkt::getNazwa() const
    {
        return nazwa;
    }

    int Produkt::getinput()
    {
        return 0;
    };

    int Produkt::getoutput()
    {
        return 0;
    }

    int Produkt::getilosc()
    {
        return iloscProduktu;
    };

    int Produkt::getfailureChance()
    {
        return 0;
    }

    void Produkt::wypisz()
    {
        std::cout<<"Wytworzony produkt "<<nazwa<<": "<<iloscProduktu<<std::endl;
        std::cout<<"---------------------------------------------"<<std::endl;
    }

    Produkt Produkt::operator+=(int value)
    {
        iloscProduktu += value;
        return *this;
    }