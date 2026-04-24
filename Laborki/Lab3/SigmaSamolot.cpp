//
// Created by Xkacp on 17.04.2024.
//

#include "SigmaSamolot.h"


void SigmaSamolot::SigmaPrint(SigmaSamolot sigma)
{
    std::cout<<sigma.SigmaPaliwo<<std::endl<<sigma.SigmaDystans<<std::endl<<sigma.SigmaX<<std::endl<<sigma.SigmaY<<std::endl<<sigma.SigmaZ<<std::endl<<sigma.SigmaWysokosc<<std::endl;
}

SigmaSamolot SigmaSamolot::SigmaKorekta(SigmaSamolot sigma, double sigmaKorekta)
{
    sigma.SigmaWysokosc-=sigmaKorekta;
    return sigma;
}