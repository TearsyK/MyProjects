#include <iostream>
#include "SigmaSamolot.h"

int main()
{
    SigmaSamolot sigma;
    SigmaSamolot uusigma(23,21,53,23,41,23);
    sigma=uusigma; //Odp. do zadania 7: Tak, działa, jak XYZ są prywatne to też działa
    sigma.SigmaPrint(sigma); //Odp. do zadania 15: Nie, ponieważ nie stworzyliśmy operatora przypisania dla SigmaRadia
    return 0;
}
