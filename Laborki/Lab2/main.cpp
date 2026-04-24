#include <iostream>
#include <string>
#include <regex>

int main()
{
    std::cout<<"mail"<<std::endl;
    std::string mail;
    std::getline(std::cin, mail);
    std::regex wzorzecMail(R"(\D{2}\d{6}@student\.polsl\.pl)");
    if (std::regex_match(mail,wzorzecMail))
        std::cout<<"dobry mail"<<std::endl;
    else
        std::cout<<"zly mail"<<std::endl;
    //sprawdź maila


    std::cout<<"numer"<<std::endl;
    std::string numer;
    std::getline(std::cin, numer);
    std:: regex wzorzecNumer(R"(\+48\s(\d{3})\s(\d{3})\s(\d{3}))");
    if (std::regex_match(numer,wzorzecNumer))
        std::cout<<"dobry numer"<<std::endl;
    else
        std::cout<<"zly numer"<<std::endl;
    //sprawdź numer

    std::cout<<"pesel"<<std::endl;
    std::string pesel;
    std::getline(std::cin, pesel);
    std::regex wzorzecPesel(R"(([0-9]{2})([0-9]{2})([0-9]{2})([0-9]{3})([0-9])([0-9]))");
    std::smatch match;
    if (std::regex_match(pesel, match, wzorzecPesel))
    {
        std::cout<<"Rok: " << match[1].str() << std::endl;
        std::cout<<"Miesiac: " << match[2].str() << std::endl;
        std::cout<<"Dzien: " << match[3].str() << std::endl;
        if (stoi(match[5].str())%2==0)
            std::cout<<"Plec: " << "Kobieta" << std::endl;
        else
            std::cout<<"Plec: " << "Mezczyzna" << std::endl;
    }
    else
        std::cout<<"niepoprawny pesel"<<std::endl;

    return 0;
}
