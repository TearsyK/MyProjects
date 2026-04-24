//
// Created by Xkacp on 12.06.2024.
//

#ifndef WALIDATOR_H
#define WALIDATOR_H

#include <iostream>
#include <regex>

class Walidator
{
public:
    virtual bool czyPoprawny(const std::string& wartosc) = 0;
};

class TelefonKom : public Walidator
{
public:
    bool czyPoprawny(const std::string& wartosc) override
    {
        return std::regex_match(wartosc, std::regex("\\+?[0-9]{10,15}"));
    }
};

class Kwota : public Walidator
{
public:
    bool czyPoprawny(const std::string& wartosc) override
    {
        return std::regex_match(wartosc, std::regex("\\d+(\\.\\d{1,2})?"));
    }
};

class Email : public Walidator
{
public:
    bool czyPoprawny(const std::string& wartosc) override
    {
        return std::regex_match(wartosc, std::regex("[\\w._%+-]+@[\\w.-]+\\.[a-zA-Z]{2,4}"));
    }
};

class TelefonStat : public Walidator
{
public:
    bool czyPoprawny(const std::string& wartosc) override
    {
        return std::regex_match(wartosc, std::regex("\\d{9}"));
    }
};

class LiczbaRzymska : public Walidator
{
public:
    bool czyPoprawny(const std::string& wartosc) override
    {
        return std::regex_match(wartosc, std::regex("^(?=[MDCLXVI])M*(C[MD]|D?C{0,3})(X[CL]|L?X{0,3})(I[XV]|V?I{0,3})$"));
    }
};

#endif // WALIDATOR_H
