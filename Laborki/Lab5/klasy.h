#ifndef KLASY_H
#define KLASY_H

#include <iostream>

class RadioFM
{
    int kompresja;
    int crc;
    std::string format;
public:
    RadioFM();
    RadioFM(int kompresja, int crc);
    int rozszyfruj(int wejscie);
    void pokazFormat();
};

class PlikMPG
{
    int kompresja;
    int crc;
    std::string format;
public:
    PlikMPG();
    PlikMPG(int kompresja, int crc);
    int rozszyfruj(int wejscie);
    void pokazFormat();
};

class PlikDabplus
{
    int kompresja;
    int crc;
    std::string format;
public:
    PlikDabplus();
    PlikDabplus(int kompresja, int crc);
    int rozszyfruj(int wejscie);
    void pokazFormat();
};

class PlikMP3
{
    int kompresja;
    int crc;
    std::string format;
public:
    PlikMP3();
    PlikMP3(int kompresja, int crc);
    int rozszyfruj(int wejscie);
    void pokazFormat();
};

class PlikDVBT
{
    int kompresja;
    int crc;
    std::string format;
public:
    PlikDVBT();
    PlikDVBT(int kompresja, int crc);
    int rozszyfruj(int wejscie);
    void pokazFormat();
};

class PlikDVBT2
{
    int kompresja;
    int crc;
    std::string format;
public:
    PlikDVBT2();
    PlikDVBT2(int kompresja, int crc);
    int rozszyfruj(int wejscie);
    void pokazFormat();
};

class Dekoder : public RadioFM, public PlikMPG, public PlikDabplus, public PlikMP3, public PlikDVBT, public PlikDVBT2
{
public:
    int moc;
    Dekoder();
    Dekoder(int kompresja, int crc);
};

class PlikMPG2 : public PlikMPG
{
public:
    PlikMPG2();
};

class PlikMPG4 : public PlikMPG, public PlikMPG2
{
public:
    PlikMPG4();
};

class Odtwarzacz : public PlikMPG4
{
public:
    Odtwarzacz();
};

#endif

