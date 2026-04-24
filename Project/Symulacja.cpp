//
// Created by Xkacp on 08.06.2024.
//

#include "Symulacja.h"



Symulacja::Symulacja()
    {
        generator.seed(time(nullptr));
    }
void Symulacja::TrybCzasowy(int CzasSymulacji, Lista& Maszyny, Lista& Surowce, Lista& Produkty)
    {
        ListaNode *currentM = Maszyny.head;
        ListaNode *currentS = Surowce.head;
        ListaNode *currentP = Produkty.head;
        this -> CzasSymulacji = CzasSymulacji;
        std::ofstream file("Wyniki.txt");
        if (!file.is_open()) {
            std::cerr << "Nie mozna otworzyc pliku Results.txt do zapisu" << std::endl;
            return;
        }
        file<<"Czas pracy fabryki: "<<CzasSymulacji<<std::endl;
        while (currentM != nullptr)
        {
            int efektywnailosc = 0;
            for (int i = 0; i < CzasSymulacji; i++)
            {
                for (int j = 0; j < currentM->data->getilosc(); j++)
                    if (generator() % 100 > currentM->data->getfailureChance())
                        efektywnailosc++;
            }
            currentS->data->wpisanie(currentM->data->getilosc() * currentM->data->getinput() * CzasSymulacji);
            currentP->data->wpisanie(efektywnailosc * currentM->data->getoutput());
            //------------------------------------------------------

            file << "Maszyna: " << currentM->data->getNazwa() << std::endl
                 <<"Ilosc wykorzystanego surowcu " << currentS->data->getNazwa() << ":  " << currentS->data->getilosc() << std::endl
                 <<"Ilosc wyprodukowanego produktu " << currentP->data->getNazwa() <<": " << currentP->data->getilosc() << std::endl;

            //------------------------------------------------------
            currentM->data->wypisz();
            currentS->data->wypisz();
            currentP->data->wypisz();



            currentM = currentM->next;
            currentS = currentS->next;
            currentP = currentP->next;

        }
        file.close();
    }

    //rozdzielenie funkcji

void Symulacja::TrybSurowcowy(Lista& Maszyny, Lista& Surowce, Lista& Produkty)
    {
        int liczDnie = 0;
        int liczDnieMax = 0;
        ListaNode *currentM = Maszyny.head;
        ListaNode *currentS = Surowce.head;
        ListaNode *currentP = Produkty.head;
        int poczatkowySurowiec = currentS->data->getilosc();
        std::ofstream file("Wyniki.txt");
        if (!file.is_open()) {
            std::cerr << "Nie mozna otworzyc pliku Results.txt do zapisu" << std::endl;
            return;
        }
        while (currentM != nullptr)
        {
            liczDnie = 0;
            while (currentS->data->getilosc() >= currentM->data->getinput())
            {
                liczDnie++;
                int efektywnailosc = 0;
                for (int i = 0; i < currentM->data->getilosc(); i++)
                    if (generator() % 100 > currentM->data->getfailureChance())
                        efektywnailosc++;

                if (currentS->data->getilosc() / currentM->data->getilosc() > currentM->data->getinput())
                {
                    auto *surowiecPtr = dynamic_cast<Surowiec *>(currentS->data);
                    if (surowiecPtr)
                    {
                        *surowiecPtr -= efektywnailosc * currentM->data->getinput();
                    } else
                    {
                        std::cerr << "Błąd: currentS->data nie wskazuje na obiekt klasy Surowiec" << std::endl;
                    }
                    auto *produktPtr = dynamic_cast<Produkt *>(currentP->data);
                    if (produktPtr)
                    {
                        *produktPtr += efektywnailosc * currentM->data->getoutput();
                    } else
                    {
                        std::cerr << "Błąd: currentP->data nie wskazuje na obiekt klasy Produkt" << std::endl;
                    }
                }
                else
                {
                    int i=0;
                    while (currentS->data->getilosc() >= currentM->data->getinput() && i<efektywnailosc)
                    {
                        auto *surowiecPtr = dynamic_cast<Surowiec *>(currentS->data);
                        if (surowiecPtr)
                        {
                            *surowiecPtr -= currentM->data->getinput();
                        } else
                        {
                            std::cerr << "Błąd: currentS->data nie wskazuje na obiekt klasy Surowiec" << std::endl;
                        }
                        auto *produktPtr = dynamic_cast<Produkt *>(currentP->data);
                        if (produktPtr)
                        {
                            *produktPtr += currentM->data->getoutput();
                        } else
                        {
                            std::cerr << "Błąd: currentP->data nie wskazuje na obiekt klasy Produkt" << std::endl;
                        }
                    }
                }
                if (liczDnie>liczDnieMax)
                    liczDnieMax=liczDnie;
            }
            currentM->data->wypisz();
            std::cout<<"Pozostaly surowiec "<< currentS->data->getNazwa() << ": " << currentS->data->getilosc()<<std::endl;
            currentP->data->wypisz();
            //-----------------------------------------------------------------------
            file << "Maszyna: " << currentM->data->getNazwa() << std::endl
            <<"Ilosc pozostalego surowca " << currentS->data->getNazwa() << ":  " << currentM->data->getilosc() << std::endl
            <<"Ilosc wykorzystanego surowca " << currentS->data->getNazwa() << ":  " << poczatkowySurowiec-currentM->data->getilosc() << std::endl
            <<"Ilosc wyprodukowanego produktu " << currentP->data->getNazwa() <<": " << currentP->data->getilosc() << std::endl;
            //-----------------------------------------------------------------------
            currentM = currentM->next;
            currentS = currentS->next;
            currentP = currentP->next;
        }
        this -> CzasSymulacji = liczDnieMax;
        std::cout<<"Praca fabryki zajela: "<< liczDnieMax <<" dni" << std::endl;
        file << "Praca fabryki zajela: " << liczDnieMax << std::endl;
        file.close();
    }
