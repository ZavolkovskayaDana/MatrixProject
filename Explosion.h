#pragma once
#include "Figure.h"
#include "SystemUtils.h"
#include <windows.h>
#include <vector>

class Explosion : public Figure { //также как и Line наследник Figure
private:
    int centerX;            // координата X центра взрыва (первый символ линии)
    int centerY;            // координата Y центра взрыва

    int radiusMin;          // минимальный радиус взрыва
    int radiusMax;          // максимальный радиус взрыва
    int currentRadius;      // текущий растущий радиус

    unsigned long long lastStepTime;   // время последнего шага
    bool finished;                      // взрыв завершён

    std::vector<std::pair<int, int>> lastPoints;

public:
    Explosion(int x, int y, int radiusMin, int radiusMax);

    // Переопределяем методы Figure
    void moveStep() override;
    bool isFinished() const override;

private:
    void drawCircle(int radius);     // рисуем кольцо
    WORD randomColor() const;        // случайный цвет, независищий от эпилепсии
};