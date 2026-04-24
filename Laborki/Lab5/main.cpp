#include <iostream>
#include "klasy.h"

int main()
{
    PlikMP3 c;
    Dekoder a;
    a.PlikMPG::rozszyfruj(50);
    a.PlikMPG::pokazFormat();
    Dekoder b(4, 20);
    b.PlikDabplus::rozszyfruj(100);
    b.PlikDabplus::pokazFormat();
    b.moc = b.RadioFM::rozszyfruj(100) + b.PlikMPG::rozszyfruj(100) + b.PlikDabplus::rozszyfruj(100) + b.PlikMP3::rozszyfruj(100) + b.PlikDVBT::rozszyfruj(100) + b.PlikDVBT2::rozszyfruj(100);
    std::cout << "Moc dekodera b wynosi: " << b.moc << std::endl;
    Odtwarzacz w;
    return 0;
}
