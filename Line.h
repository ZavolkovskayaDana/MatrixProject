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

    // тестирование время последнего шага для данной линии 
    unsigned long long lastStepTime = 0;

    // Состояния: когда линия дошла до правого края — начинаем плавное затирание хвоста
    bool exiting = false;
    bool finished = false;
public:
    Line(int speed, int length, bool epilepsy);

    void moveStep();  // двигаем линию на один шаг
    //void initialize(int column);  // создать линию в начальной позиции

    bool isFinished() const; //флаг окончания 

private:
    // вспомогательное: проверяем можно ли сделать шаг по времени
    bool canStep();
};
