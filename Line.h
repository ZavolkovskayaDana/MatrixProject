#pragma once
#include <vector>
#include "Symbol.h"
#include "SystemUtils.h"
#include "Figure.h"
#include "AppManager.h"

class Line : public Figure { //Все public-методы Figure станут public-методами Line
private:
    AppManager* owner;   // ссылка на менеджер, то есть даем Line доступ к AppManager,так как Line не может сама создавать взрыв
    int explosionProbability;
    int radiusMin;
    int radiusMax;

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
    Line(int speed, int length, bool epilepsy,
        int explosionProbability,
        int radiusMin,
        int radiusMax,
        AppManager* owner);

    void moveStep() override;  // двигаем линию на один шаг
    //void initialize(int column);  // создать линию в начальной позиции

    bool isFinished() const override; //флаг окончания 

private:
    // вспомогательное: проверяем можно ли сделать шаг по времени
    bool canStep();
};
