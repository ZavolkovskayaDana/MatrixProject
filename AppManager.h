#pragma once
#include "Line.h"
#include "SystemUtils.h"
#include <ctime>
//#include <vector>
#include "Explosion.h"
#include "List.h"

class AppManager {
private:
    bool epilepsy;
    int speed;
    int length;
    int frequency;

    int width;
    int height;

    int explosionProbability;//вероятность взрыва 
    int radiusMin;//мин радиус 
    int radiusMax;//макс радиус 


    //std::vector<Figure*> figures; //новый вектор для всех фигур (линии, взрывы), хранит указатели 

    List<Figure*>* figures; //заменили 


    // Вектор активных линий
    //std::vector<Line> lines;

    //std::vector<int> spawnDelays; //список задержек появляений линий в данную секунду
    List<int>* spawnDelays;

    int currentDelayIndex; //индекс задержки в списке, который надо обработать 
    ULONGLONG lastSpawnTime; //время начала новый секунды (64‑битное беззнаковое целое)

public:
    AppManager(); //  конструктор по умолчанию

    AppManager(int frequency,
        int speed,
        int length,
        bool epilepsy,
        int explosionProbability,
        int radiusMin,
        int radiusMax);

    ~AppManager(); //для очистки всех фигур при выходе

    void initialize();  // настройка параметров от пользователя
    void run();

    void drawFrame();     // отрисовка и движение всех фигур
    void createNewLine();   //срздаем линию
    void createExplosion(int x, int y); //создаем взрыв


private:
    void generateSpawnSchedule();
};
