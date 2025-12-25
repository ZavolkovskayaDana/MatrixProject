#include "AppManager.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
//#include <algorithm>
#include "Line.h"
#include "Explosion.h"
#include "LinkedList.h"


using namespace std;

//по умолчанию
AppManager::AppManager()
    : frequency(5),
    speed(50),
    length(10),
    epilepsy(false),
    explosionProbability(100),   
    radiusMin(1),
    radiusMax(3),
    currentDelayIndex(0),
    lastSpawnTime(0)
{
    srand(static_cast<unsigned>(time(nullptr)));

    figures = new LinkedList<Figure*>();
    spawnDelays = new LinkedList<int>();
}

AppManager::AppManager(int frequency,
    int speed,
    int length,
    bool epilepsy,
    int explosionProbability,
    int radiusMin,
    int radiusMax)
    : frequency(frequency),
    speed(speed),
    length(length),
    epilepsy(epilepsy),
    explosionProbability(explosionProbability),
    radiusMin(radiusMin),
    radiusMax(radiusMax),
    currentDelayIndex(0),
    lastSpawnTime(0)
{
    srand(static_cast<unsigned>(time(nullptr)));

    figures = new LinkedList<Figure*>();
    spawnDelays = new LinkedList<int>();
}

AppManager::~AppManager() {
   
    /*for (Figure* f : figures) {
        delete f;
    }
    figures.clear();
    */
    //дл€ 4 лабы
    // удал€ем все фигуры
    while (figures->size() > 0) {
        delete figures->at(0);
        figures->erase(0);
    }

    delete figures;

    //ќчищаем контейнер задержек
    spawnDelays->clear();
    delete spawnDelays;
}


// ввод параметров от пользовател€
void AppManager::initialize() {
    //SystemUtils::initConsole(120, 35);

    cout << "¬ведите частоту генерации линий (1Ц30): ";
    cin >> frequency;

    cout << "¬ведите скорость линии (меньше = быстрее): ";
    cin >> speed;

    cout << "¬ведите длину линии: ";
    cin >> length;

    cout << "¬ключить режим эпилепсии? (1 - да, 0 - нет): ";
    cin >> epilepsy;

    cout << "¬ведите веро€тность взрыва линии (1Ц1000): ";
    cin >> explosionProbability;

    cout << "¬ведите минимальный радиус взрыва (1Ц10): ";
    cin >> radiusMin;

    cout << "¬ведите максимальный радиус взрыва (" << radiusMin << "Ц10): ";
    cin >> radiusMax;

    system("cls"); // очистка экрана после ввода
}

void AppManager::createNewLine() {
    //lines.emplace_back(speed, length, epilepsy);
    //figures.push_back(new Line(speed, length, epilepsy, explosionProbability, radiusMin, radiusMax));
    
    //figures Ч это указатель на контейнер ("переходим по адресу и вызываем метод у объ€екта")
    figures->push_back(new Line(speed, length, epilepsy,
        explosionProbability,
        radiusMin,
        radiusMax));

}

//создаем взрыв
void AppManager::createExplosion(int x, int y) {
    figures->push_back(new Explosion(x, y, radiusMin, radiusMax));
}

void AppManager::drawFrame() {

    // 1. ƒвигаем все фигуры, которые были в начале кадра
    size_t count = figures->size();

    for (size_t i = 0; i < count; ++i) {

        //Figure* f = figures[i];
        Figure* f = figures->at(i);
        f->moveStep();

        if (f->wantsExplosion()) {
            createExplosion(f->getExplosionX(), f->getExplosionY());
            f->resetExplosionRequest();
        }

    }

    /* // 2. “еперь удал€ем завершившиес€ фигуры
     for (auto it = figures.begin(); it != figures.end(); ) {

         if ((*it)->isFinished()) {
             delete* it;
             it = figures.erase(it);
         }
         else {
             ++it;
         }
     }*/

     // 2. ”дал€ем завершившиес€ фигуры
    for (size_t i = 0; i < figures->size(); ) {
        if (figures->at(i)->isFinished()) {
            delete figures->at(i);
            figures->erase(i);
        }
        else {
            ++i;
        }
    }
}

void AppManager::run() {
    SystemUtils::initConsole(120, 35);

    SystemUtils::sleep(200); //  подождать, пока буфер установитс€

    generateSpawnSchedule();

    while (true) {
        ULONGLONG now = GetTickCount64();
        ULONGLONG elapsed = now - lastSpawnTime;

        if (currentDelayIndex < spawnDelays->size() &&
            elapsed >= spawnDelays->at(currentDelayIndex))
        {
            createNewLine();
            currentDelayIndex++;
        }

        if (elapsed >= 1000) {
            generateSpawnSchedule();
        }

        drawFrame();
    }
}



/*//генерируем случайное врем€ задержек 
void AppManager::generateSpawnSchedule() {
    spawnDelays.clear();
    spawnDelays.reserve(frequency);

    for (int i = 0; i < frequency; i++) {
        spawnDelays.push_back(rand() % 1000); // 0Ц999 мс
    }

    std::sort(spawnDelays.begin(), spawnDelays.end()); //сортируем список
    currentDelayIndex = 0;
    lastSpawnTime = GetTickCount64(); //врем€ начала текущей секунды = количество миллисекунд, прошедших с момента запуска системы.ем€ от начала процесса 

}*/

void AppManager::generateSpawnSchedule() {

    // очищаем старые задержки
    while (spawnDelays->size() > 0) {
        //spawnDelays->erase(0);
        spawnDelays->clear();
    }

    for (int i = 0; i < frequency; i++) {
        spawnDelays->push_back(rand() % 1000);
    }

    // проста€ сортировка вместо std::sort(spawnDelays.begin(), spawnDelays.end()); //сортируем список
    for (size_t i = 0; i < spawnDelays->size(); ++i) {
        for (size_t j = i + 1; j < spawnDelays->size(); ++j) {
            if (spawnDelays->at(j) < spawnDelays->at(i)) {
                int tmp = spawnDelays->at(i);
                spawnDelays->at(i) = spawnDelays->at(j);
                spawnDelays->at(j) = tmp;
            }
        }
    }

    currentDelayIndex = 0;
    lastSpawnTime = GetTickCount64();
}
