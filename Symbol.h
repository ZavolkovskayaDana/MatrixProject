#pragma once
#include <windows.h>
#include "SystemUtils.h"
#include <cstdlib>

class Symbol {
private:
    int x;          // координата X
    int y;          // координата Y
    char ch;        // сам символ
    WORD color;     // цвет символа
    bool epilepsy;  // флаг режима эпилепсии

public:
    // Конструктор
    Symbol(int x, int y, bool epilepsy = false);

    // Основные методы
    void draw();             // нарисовать символ
    void clear();            // затереть символ
    void update();           // обновить символ 

   
    int getX() const;
    int getY() const;
    void setPosition(int newX, int newY);

private:
    void randomizeChar();    // выбрать случайный символ
    void randomizeColor();   // выбрать случайный цвет
};
