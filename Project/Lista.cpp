//
// Created by Xkacp on 07.06.2024.
//

#include "Lista.h"



void Lista::insertAtEnd(ObiektBazowy* val) {
        Node* newNode = new Node(val);
        if (head == nullptr) {
            head = newNode;
            return;
        }
        Node* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newNode;
    }

void Lista::modifyListForSurowce(Lista& surowce)
    {
        Lista::Node *current = surowce.head;
        std::cout<<"Podaj po kolei ilosc surowcow:"<<std::endl;
        int iloscSurowca;
        while (current != nullptr)
        {
            std::cout << current->data->getNazwa() << ":" << std::endl;
            std::cin>>iloscSurowca;
            current->data->wpisanie(iloscSurowca);
            current = current->next;
        }
    }

Lista::~Lista() {
        Node* current = head;
        while (current != nullptr) {
            Node* nextNode = current->next;
            delete current->data;
            delete current;
            current = nextNode;
        }
    }
