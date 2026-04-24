//
// Created by Xkacp on 17.04.2024.
//

#ifndef LAB3_SIGMASILNIK_H
#define LAB3_SIGMASILNIK_H


class SigmaSilnik
{
private:
    void operator=(SigmaSilnik& sigma)
    {
        SigmaStan=sigma.SigmaStan;
        SigmaIdentyfikator=sigma.SigmaIdentyfikator;
        SigmaSzybkosc=sigma.SigmaSzybkosc;
    }
public:
    bool SigmaStan;
    int SigmaSzybkosc;
    int SigmaIdentyfikator;
};


#endif //LAB3_SIGMASILNIK_H
