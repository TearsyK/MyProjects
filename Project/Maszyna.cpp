//
// Created by Xkacp on 17.04.2024.
//

#include "Maszyna.h"

#include <utility>

Maszyna::Maszyna(std::string nazwa, int input, int output, int ilosc, int failureChance, int id)
    {
        this -> id = id;
        this -> nazwa = std::move(nazwa);
        this -> input = input;
        this -> output = output;
        this -> ilosc = ilosc;
        this -> failureChance = failureChance;
    }

void Maszyna::wpisanie(int iloscSurowca)
    {};

void Maszyna::wypisz()
    {
        std::cout<<"---------------------------------------------"<<std::endl;
        std::cout<<"Statystyki pracy maszyn " << nazwa<<std::endl;
    }
std::string Maszyna::getNazwa() const
    {
        return nazwa;
    }

int Maszyna::getinput()
    {
        return input;
    }

int Maszyna::getoutput()
    {
        return output;
    }

int Maszyna::getilosc()
    {
        return ilosc;
    }

int Maszyna::getfailureChance()
    {
        return failureChance;
    }

