//контейнер (наслденик List.h)
//шаблоны не компилируются сами по себе.
//Они компилируются только в момент использования, когда компилятор знает конкретный тип 

#pragma once

#pragma once
#include "List.h"
#include <stdexcept>

//контейнер - связан список из узлов
template<typename T>
class LinkedList : public List<T> {
private:
    struct Node {   //структура узла
        T value;    //данные
        Node* next; //указатель на след эл-т
        Node(const T& v) : value(v), next(nullptr) {}
    };

    Node* head; //первый элемент
    size_t count;   //кол-во элементов

public:
    LinkedList() : head(nullptr), count(0) {}   //конструктор
    ~LinkedList() override { clear(); }         //диструктор 

    void push_back(const T& value) override;    //переопределяем методы из базового класса 
    void erase(size_t index) override;
    T& at(size_t index) override;
    size_t size() const override;
    void clear() override;
};


//Реализация методов

// Конструктор
template<typename T>
LinkedList<T>::LinkedList()
    : head(nullptr), count(0) {}


// Деструктор
template<typename T>
LinkedList<T>::~LinkedList() {
    clear();
}


// Добавление элемента в конец
template<typename T>
void LinkedList<T>::push_back(const T& value) { 
    Node* newNode = new Node(value);    //создаем новый узел

    if (!head) {    //если спиоск пустой, то нов уз = 1
        head = newNode;
    }
    else {
        Node* cur = head;   //указатель на тек узел, нач с 1 узла
        while (cur->next) {
            cur = cur->next;    
        }
        cur->next = newNode;    //нов узел в конец
    }

    ++count;    //увеличиваем кол-во эл-тов
}


// Доступ по индексу
template<typename T>
T& LinkedList<T>::at(size_t index) {
    if (index >= count) {
        throw std::out_of_range("Index out of range");
    }

    Node* cur = head;
    for (size_t i = 0; i < index; ++i) {    //идем индекс раз вперед по нексту
        cur = cur->next;
    }

    return cur->value; //возвращаем ссылку на значение cur
}


// Удаление элемента по индексу
template<typename T>
void LinkedList<T>::erase(size_t index) {   //если индекс не прав -> ничего не делаем
    if (index >= count) return;

    Node* toDelete = nullptr; //указатель на узел, кот будем удалять

    if (index == 0) {
        toDelete = head;
        head = head->next;
    }
    else {      //ищем предыдущий сивол, так как он указывает на наш удаляемый символ
        Node* prev = head;
        for (size_t i = 0; i < index - 1; ++i) {
            prev = prev->next;
        }
        toDelete = prev->next;
        prev->next = toDelete->next;
    }

    delete toDelete;
    --count; //- кол-во символов
}


// Количество элементов
template<typename T>
size_t LinkedList<T>::size() const {
    return count;
}


// Очистка списка
template<typename T>
void LinkedList<T>::clear() {
    while (head) {
        Node* tmp = head;
        head = head->next;
        delete tmp;
    }
    count = 0;
}
