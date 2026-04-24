#ifndef UNTITLED4_LISTA_H
#define UNTITLED4_LISTA_H

#include <iostream>
#include <memory>
#include <stdexcept>

// Klasa szablonowa dla listy jednokierunkowej
template <typename T>
class Lista {
private:
    struct Node {
        T data;                               // Dane przechowywane w węźle
        std::unique_ptr<Node> next = nullptr; // Wskaźnik na następny węzeł

        Node(const T& value) : data(value) {}
    };

    std::unique_ptr<Node> head = nullptr; // Wskaźnik na pierwszy element listy
    Node* tail = nullptr;                // Surowy wskaźnik na ostatni element listy (dla optymalizacji dodawania)

public:
    // Klasa iteratora
    class Iterator {
    private:
        Node* current; // Surowy wskaźnik na bieżący węzeł

    public:
        // Konstruktor domyślny
        Iterator() : current(nullptr) {}

        // Konstruktor z węzłem
        explicit Iterator(Node* node) : current(node) {}

        // Operator dereferencji
        T& operator*() {
            if (!current) throw std::out_of_range("Iterator out of range");
            return current->data;
        }

        // Operator strzałki (opcjonalny, dla zgodności z STL)
        T* operator->() {
            return &(**this);
        }

        // Operator inkrementacji (przejście do następnego elementu)
        Iterator& operator++() {
            if (current) {
                current = current->next.get();
            }
            return *this;
        }

        // Operator porównania (równość)
        bool operator==(const Iterator& other) const {
            return current == other.current;
        }

        // Operator porównania (różność)
        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
    };

    // Konstruktor bezargumentowy
    Lista() = default;

    // Konstruktor kopiujący
    Lista(const Lista& other) {
        Node* current = other.head.get();
        while (current) {
            push_back(current->data);
            current = current->next.get();
        }
    }

    // Konstruktor przenoszący
    Lista(Lista&& other) noexcept
    : head(std::move(other.head)), tail(other.tail) {
        other.tail = nullptr;
    }

    // Operator przypisania (kopiujący)
    Lista& operator=(const Lista& other) {
        if (this == &other) return *this;
        clear();
        Node* current = other.head.get();
        while (current) {
            push_back(current->data);
            current = current->next.get();
        }
        return *this;
    }

    // Operator przypisania (przenoszący)
    Lista& operator=(Lista&& other) noexcept {
        if (this == &other) return *this;
        clear();
        head = std::move(other.head);
        tail = other.tail;
        other.tail = nullptr;
        return *this;
    }



    // Dodawanie elementu na koniec listy
    void push_back(const T& value) {
        auto newNode = std::make_unique<Node>(value);
        if (!head) {
            head = std::move(newNode);
            tail = head.get();
        } else {
            tail->next = std::move(newNode);
            tail = tail->next.get();
        }
    }

    // Usuwanie wszystkich elementów
    void clear() {
        while (head) {
            head = std::move(head->next);
        }
        tail = nullptr;
    }

    // Pobranie iteratora na początek listy
    Iterator begin() {
        return Iterator(head.get());
    }

    // Pobranie iteratora wskazującego na "koniec" listy
    Iterator end() {
        return Iterator(nullptr);
    }

    // Wypisanie zawartości listy (pomocnicze)
    void print() const {
        Node* current = head.get();
        while (current) {
            std::cout << current->data << " ";
            current = current->next.get();
        }
        std::cout << std::endl;
    }
};


#endif //UNTITLED4_LISTA_H
