//
// Created by Xkacp on 07.06.2024.
//

#ifndef PROJECT_OBIEKTBAZOWY_H
#define PROJECT_OBIEKTBAZOWY_H
#include "sstream"

class ObiektBazowy
{
public:
    virtual ~ObiektBazowy();

    virtual void wpisanie(int) = 0;

    [[nodiscard]] virtual std::string getNazwa() const = 0;

    virtual int getinput() =0;

    virtual int getoutput() = 0;

    virtual int getilosc() = 0;

    virtual int getfailureChance() = 0;

    virtual void wypisz() = 0;
};


#endif //PROJECT_OBIEKTBAZOWY_H
