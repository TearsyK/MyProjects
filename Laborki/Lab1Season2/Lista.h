#ifndef UNTITLED2_LISTA_H
#define UNTITLED2_LISTA_H

#include "iostream"

template <typename T>
class Lista {
public:
    struct Node {
        T data;
        Node* next;
        Node(T val) : data(val), next(nullptr) {}
    };

    Lista() : head(nullptr) {}

    ~Lista() {
        // Usuń wszystkie elementy listy
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void add(T value) {
        Node* newNode = new Node(value);
        if (!head) {
            head = newNode;
        } else {
            Node* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    T getByIndex(int index) const {
        Node* temp = head;
        int currentIndex = 0;
        while (temp) {
            if (currentIndex == index) {
                return temp->data;
            }
            temp = temp->next;
            currentIndex++;
        }
        throw std::out_of_range("Index out of range");
    }

    void print() const {
        Node* temp = head;
        while (temp) {
            std::cout << temp->data << " ";
            temp = temp->next;
        }
        std::cout << std::endl;
    }

    T getLast() const
    {
        Node* temp = head;
        while (temp->next) {
            temp = temp->next;
        }
        return temp->data;
    }

    Node* head;
};


#endif //UNTITLED2_LISTA_H
