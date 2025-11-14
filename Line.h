#pragma once
#include <vector>
#include "Symbol.h"
#include "SystemUtils.h"

class Line {
private:
    int length;                   // длина линии
    int speed;                    // скорость (символов в секунду)
    bool epilepsy;                // режим эпилепсии
    int startY;                   // базовая высота (по Y)
    int currentX;                 // текущая X позиция линии
    int zigDir = -1;
    int baseX = 0;
    std::vector<Symbol> symbols;  // символы линии
public:
    Line(int speed, int length, bool epilepsy);

    void moveStep();  // двигаем линию на один шаг


    void initialize();  // создать линию в начальной позиции
};
