//
// Created by Xkacp on 12.06.2024.
//

#ifndef EFEKTY_H
#define EFEKTY_H

#include <iostream>
#include "fstream"

class EfektSpecjalny
{
public:
    virtual void wykonajEfekt() = 0;
};

class Deszcz : public EfektSpecjalny
{
public:
    void wykonajEfekt() override
    {
        std::cout << "Efekt deszczu" << std::endl;
        std::ifstream plikDeszcz("deszcz.txt");
        std::string linia;
        if (plikDeszcz)
        {
            while (getline(plikDeszcz, linia))
            {
                std::cout << linia << std::endl;
            }
        }
        plikDeszcz.close();
    }
};

class Wybuch : public EfektSpecjalny
{
public:
    void wykonajEfekt() override
    {
        std::cout << "Efekt wybuchu" << std::endl;
        std::ifstream plikWybuch("wybuch.txt");
        std::string linia;
        if (plikWybuch)
        {
            while (getline(plikWybuch, linia))
            {
                std::cout << linia << std::endl;
            }
        }
        plikWybuch.close();
    }
};

class Blyskawica : public EfektSpecjalny
{
public:
    void wykonajEfekt() override
    {
        std::cout << "Efekt blyskawicy" << std::endl;
        std::ifstream plikBlysk("blyskawica.txt");
        std::string linia;
        if (plikBlysk)
        {
            while (getline(plikBlysk, linia))
            {
                std::cout << linia << std::endl;
            }
        }
        plikBlysk.close();
    }
};

class Ognien : public EfektSpecjalny
{
public:
    void wykonajEfekt() override
    {
        std::cout << "Efekt ognia" << std::endl;
        std::ifstream plikOgien("ogien.txt");
        std::string linia;
        if (plikOgien)
        {
            while (getline(plikOgien, linia))
            {
                std::cout << linia << std::endl;
            }
        }
        plikOgien.close();
    }
};

class Dym : public EfektSpecjalny
{
public:
    void wykonajEfekt() override
    {
        std::cout << "Efekt dymu" << std::endl;
        std::ifstream plikDym("dym.txt");
        std::string linia;
        if (plikDym)
        {
            while (getline(plikDym, linia))
            {
                std::cout << linia << std::endl;
            }
        }
        plikDym.close();
    }
};

#endif // EFEKTY_H
