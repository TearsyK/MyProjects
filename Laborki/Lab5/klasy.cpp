#include <iostream>
#include "klasy.h"

RadioFM::RadioFM() : kompresja(5), crc(10), format("radiofm")
{
    std::cout << "Wywolano konstruktor bezargumentowy klasy RadioFM" << std::endl;
}

RadioFM::RadioFM(int kompresja, int crc) : kompresja(kompresja), crc(crc), format("radiofm")
{
    std::cout << "Wywolano konstruktor wieloargumentowy klasy RadioFM" << std::endl;
}

int RadioFM::rozszyfruj(int wejscie)
{
    int wynik;
    wynik = wejscie / kompresja + crc;
    std::cout << "Rozszyfrowano -> " << wejscie << " / " << kompresja << " + " << crc << " = " << wynik << std::endl;
    return wynik;
}

void RadioFM::pokazFormat()
{
    std::cout << "Format tej klasy to: " << format << std::endl;
}

PlikMPG::PlikMPG() : kompresja(3), crc(15), format("MPG")
{
    std::cout << "Wywolano konstruktor bezargumentowy klasy PlikMPG" << std::endl;
}

PlikMPG::PlikMPG(int kompresja, int crc) : kompresja(kompresja), crc(crc), format("MPG")
{
    std::cout << "Wywolano konstruktor wieloargumentowy klasy PlikMPG" << std::endl;
}

int PlikMPG::rozszyfruj(int wejscie)
{
    int wynik;
    wynik = wejscie / (kompresja + crc);
    std::cout << "Rozszyfrowano -> " << wejscie << " / (" << kompresja << " + " << crc << ") = " << wynik << std::endl;
    return wynik;
}

void PlikMPG::pokazFormat()
{
    std::cout << "Format tej klasy to: " << format << std::endl;
}

PlikDabplus::PlikDabplus() : kompresja(7), crc(40), format("Dab+")
{
    std::cout << "Wywolano konstruktor bezargumentowy klasy PlikDabplus" << std::endl;
}

PlikDabplus::PlikDabplus(int kompresja, int crc) : kompresja(kompresja), crc(crc), format("Dab+")
{
    std::cout << "Wywolano konstruktor wieloargumentowy klasy PlikDabplus" << std::endl;
}

int PlikDabplus::rozszyfruj(int wejscie)
{
    int wynik;
    wynik = wejscie / (kompresja + crc) + wejscie / 2;
    std::cout << "Rozszyfrowano -> " << wejscie << " / ( " << kompresja << " + " << crc << " ) + " << wejscie << " / 2 = " << wynik << std::endl;
    return wynik;
}

void PlikDabplus::pokazFormat()
{
    std::cout << "Format tej klasy to: " << format << std::endl;
}

PlikMP3::PlikMP3() : kompresja(6), crc(30), format("MP3")
{
    std::cout << "Wywolano konstruktor bezargumentowy klasy PlikMP3" << std::endl;
}

PlikMP3::PlikMP3(int kompresja, int crc) : kompresja(kompresja), crc(crc), format("MP3")
{
    std::cout << "Wywolano konstruktor wieloargumentowy klasy PlikMP3" << std::endl;
}

int PlikMP3::rozszyfruj(int wejscie)
{
    int wynik;
    wynik = (wejscie + kompresja) * crc;
    std::cout << "Rozszyfrowano -> " << "( " << wejscie << " + " << kompresja << " ) * " << crc << " = " << wynik << std::endl;
    return wynik;
}

void PlikMP3::pokazFormat()
{
    std::cout << "Format tej klasy to: " << format << std::endl;
}

PlikDVBT::PlikDVBT() : kompresja(15), crc(8), format("DVBT")
{
    std::cout << "Wywolano konstruktor klasy PlikDVBT" << std::endl;
}

PlikDVBT::PlikDVBT(int kompresja, int crc) : kompresja(kompresja), crc(crc), format("DVBT")
{
    std::cout << "Wywolano konstruktor wieloargumentowy klasy PlikDVBT" << std::endl;
}

int PlikDVBT::rozszyfruj(int wejscie)
{
    int wynik;
    wynik = wejscie + kompresja + crc;
    std::cout << "Rozszyfrowano -> " << "( " << wejscie << " + " << kompresja << " ) * " << crc << " = " << wynik << std::endl;
    return wynik;
}

void PlikDVBT::pokazFormat()
{
    std::cout << "Format tej klasy to: " << format << std::endl;
}

PlikDVBT2::PlikDVBT2() : kompresja(15), crc(7), format("DVBT2")
{
    std::cout << "Wywolano konstruktor klasy PlikDVBT2" << std::endl;
}

PlikDVBT2::PlikDVBT2(int kompresja, int crc) : kompresja(kompresja), crc(crc), format("DVBT2")
{
    std::cout << "Wywolano konstruktor wieloargumentowy klasy PlikDVBT2" << std::endl;
}

int PlikDVBT2::rozszyfruj(int wejscie)
{
    int wynik;
    wynik = wejscie + kompresja + crc;
    std::cout << "Rozszyfrowano -> " << "( " << wejscie << " + " << kompresja << " ) * " << crc << " = " << wynik << std::endl;
    return wynik;
}

void PlikDVBT2::pokazFormat()
{
    std::cout << "Format tej klasy to: " << format << std::endl;
}

Dekoder::Dekoder() : RadioFM(), PlikMPG(), PlikDabplus(), PlikMP3(), PlikDVBT(), PlikDVBT2()
{
    std::cout << "Wywolano konstruktor bezargumentowy klasy Dekoder" << std::endl;
}

Dekoder::Dekoder(int kompresja, int crc) : RadioFM(kompresja, crc), PlikMPG(kompresja, crc), PlikDabplus(kompresja, crc), PlikMP3(kompresja, crc), PlikDVBT(kompresja, crc), PlikDVBT2(kompresja, crc)
{
    std::cout << "Wywolano konstruktor wieloargumentowy klasy Dekoder" << std::endl;
}

PlikMPG2::PlikMPG2() : PlikMPG()
{
    std::cout << "Wywolano konstruktor bezargumentowy klasy PlikMPG2" << std::endl;
}

PlikMPG4::PlikMPG4() : PlikMPG2(), PlikMPG()
{
    std::cout << "Wywolano konstruktor bezargumentowy klasy PlikMPG4" << std::endl;
}

Odtwarzacz::Odtwarzacz() : PlikMPG4()
{
    std::cout << "Wywolano konstruktor bezargumentowy klasy Odtwarzacz" << std::endl;
}



