#pragma once
#include "Line.h"
#include "SystemUtils.h"
#include <ctime>

class AppManager {
private:
    bool epilepsy;
    int speed;
    int length;
    int frequency;

public:
    AppManager(); //  конструктор по умолчанию
    AppManager(int speed, int length, bool epilepsy); //  новый конструктор
    void initialize();  // настройка параметров от пользователя
    void run();
};
