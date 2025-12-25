//интерфейс(набор правил для контейнера)
#pragma once
#include <cstddef>   // size_t

// Абстрактный контейнер(интерфейс)
template <typename T>
class List {
public:
    // Добавить элемент в конец
    virtual void push_back(const T& value) = 0;

    // Вставить элемент в позицию index
    virtual void insert(size_t index, const T& value) = 0;

    // Удалить элемент по индексу
    virtual void erase(size_t index) = 0;

    // Доступ к элементу по индексу
    virtual T& at(size_t index) = 0;

    // Количество элементов
    virtual size_t size() const = 0;

    // Очистить контейнер
    virtual void clear() = 0;

    // Виртуальный деструктор
    virtual ~List() = default;
};