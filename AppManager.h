#pragma once
#include "Line.h"
#include "SystemUtils.h"
#include <ctime>
#include <vector>

class AppManager {
private:
    bool epilepsy;
    int speed;
    int length;
    int frequency;

    int width;
    int height;

    std::vector<Figure*> figures; //новый вектор для всех фигур (линии, взрывы), хранит указатели 


    // Вектор активных линий
    //std::vector<Line> lines;

    std::vector<int> spawnDelays; //список задержек появляений линий в данную секунду
    int currentDelayIndex; //индекс задержки в списке, который надо обработать 
    ULONGLONG lastSpawnTime; //время начала новый секунды (64‑битное беззнаковое целое)

public:
    AppManager(); //  конструктор по умолчанию
    AppManager(int forequency, int speed, int length, bool epilepsy); //  новый конструктор
    void initialize();  // настройка параметров от пользователя
    void run();
private:
    void generateSpawnSchedule();
    void createNewLine();
    void drawFrame();
};
