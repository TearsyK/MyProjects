#ifndef LAB4SEASON2_LISTA_H
#define LAB4SEASON2_LISTA_H

#include <memory>
#include <iostream>

template <typename T>
class Lista {
public:
    struct Node {
        T data;
        std::unique_ptr<Node> next;
        Node(T val) : data(val), next(nullptr) {}
    };

    Lista() : head(nullptr) {}

    ~Lista() = default; // Zarządzanie pamięcią odbywa się automatycznie

    void add(T value) {
        auto newNode = std::make_unique<Node>(value);
        if (!head) {
            head = std::move(newNode);
        } else {
            Node* temp = head.get();
            while (temp->next) {
                temp = temp->next.get();
            }
            temp->next = std::move(newNode);
        }
    }

    void print() const {
        Node* temp = head.get();
        while (temp) {
            std::cout << temp->data << " ";
            temp = temp->next.get();
        }
        std::cout << std::endl;
    }

    T getByIndex(int index) const {
        Node* temp = head.get();
        int currentIndex = 0;
        while (temp) {
            if (currentIndex == index) {
                return temp->data;
            }
            temp = temp->next.get();
            currentIndex++;
        }
        throw std::out_of_range("Index out of range");
    }


    int& begin()
    {
        return this->head;
    }

    int& end()
    {

        while ()
    }
private:
    std::unique_ptr<Node> head;
};


#endif //LAB4SEASON2_LISTA_H
