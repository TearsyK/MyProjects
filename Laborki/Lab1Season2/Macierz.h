#ifndef UNTITLED2_MACIERZ_H
#define UNTITLED2_MACIERZ_H

#include "Lista.h"

template<typename T>
class Macierz
{
private:
T** dane;
int wiersze, kolumny;

public:
    Macierz(int w, int k) : wiersze(w), kolumny(k)
    {
        dane = new T*[wiersze];
        for (int i = 0; i < wiersze; i++)
        {
            dane[i] = new T[kolumny];
        }
    };

    ~Macierz()
    {
        for (int i = 0; i < wiersze; i++)
            delete dane [i];
        delete[] dane;
    }

    void set(int w, int k, T wartosc)
    {
        try
        {
            dane[w][k] = wartosc;
        }
        catch(...)
        {
            std::cerr<<"Nie można przypisać"<<std::endl;
        }
    }
    T get(int w, int k)
    {
        return dane[w][k];
    }

    void dodaj(const Macierz& inna)
    {
        for (int i = 0; i < wiersze; i++)
        {
            for (int j = 0; j < kolumny; j++)
                this -> dane[i][j] += inna.dane[i][j];
        }
    }
    void mnoz(const Macierz& inna)
    {
        Macierz temp(wiersze,inna.kolumny);
        for (int i = 0; i < temp.wiersze; i++)
        {
            for (int j = 0; j < temp.kolumny; j++)
            {
            temp.dane[i][j]=0;
            for (int k = 0; k < wiersze; k++)
                temp.dane[i][k] += dane[k][i]*inna.dane[j][k];
            }
        }
        this = temp;
    }
    void wypisz()
    {
        for (auto h: dane)
        {
            for (auto k : h)
                std::cout << k << " ";
            std::cout << std::endl;
        }
    }
};


#endif //UNTITLED2_MACIERZ_H
