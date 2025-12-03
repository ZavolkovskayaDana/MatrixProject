#include "AppManager.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>


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
}

AppManager::~AppManager() {
    // ѕо-хорошему можно удалить все фигуры, если они остались (но их нет, потому что run() Ч бесконечный цикл)
    for (Figure* f : figures) {
        delete f;
    }
    figures.clear();
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
    figures.push_back(new Line(speed, length, epilepsy, explosionProbability, radiusMin, radiusMax, this));
        //функци€ возвращает указатель и кладет во внутренний массив
}

//создаем взрыв
void AppManager::createExplosion(int x, int y) {
    figures.push_back(new Explosion(x, y, radiusMin, radiusMax));
}

void AppManager::drawFrame() {
    // двигаем все, кто был в начале кадра
    size_t count = figures.size();
    for (size_t i = 0; i < count; ++i) {
        figures[i]->moveStep();
    }

    // удал€ем завершившиес€
    for (auto it = figures.begin(); it != figures.end(); )
    {
        if ((*it)->isFinished()) {
            delete* it;
            it = figures.erase(it);
        }
        else {
            ++it;
        }
    }
}



void AppManager::run() {
    SystemUtils::initConsole(120, 35);

    SystemUtils::sleep(200); //  подождать, пока буфер установитс€

    generateSpawnSchedule();
    // принудительно обновим данные о высоте
    /*int h = SystemUtils::getConsoleHeight();


    system("cls");

    Line line(speed, length, epilepsy);

    while (true) {
        line.moveStep();
    }*/
    while (true) {
        DWORD now = GetTickCount64();
        DWORD elapsed = now - lastSpawnTime;

        if (currentDelayIndex < spawnDelays.size() &&
            elapsed >= spawnDelays[currentDelayIndex])
        {
            createNewLine();
            currentDelayIndex++;
        }

        if (elapsed >= 1000) {
            generateSpawnSchedule();
        }

        drawFrame();
        //SystemUtils::sleep(10); //ограничение количесвто кадрров в сек
    }
}



//генерируем случайное врем€ задержек 
void AppManager::generateSpawnSchedule() {
    spawnDelays.clear();
    spawnDelays.reserve(frequency);

    for (int i = 0; i < frequency; i++) {
        spawnDelays.push_back(rand() % 1000); // 0Ц999 мс
    }

    std::sort(spawnDelays.begin(), spawnDelays.end()); //сортируем список
    currentDelayIndex = 0;
    lastSpawnTime = GetTickCount64(); //врем€ начала текущей секунды = количество миллисекунд, прошедших с момента запуска системы.ем€ от начала процесса 

}
