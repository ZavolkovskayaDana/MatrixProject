#include "AppManager.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>


using namespace std;

AppManager::AppManager()
    : frequency(5), epilepsy(false), speed(50), length(10), currentDelayIndex(0), lastSpawnTime(0)
{
   srand(static_cast<unsigned>(time(nullptr))); // инициализаци€ rand()
}

AppManager::AppManager(int frequency, int speed, int length, bool epilepsy)
    : frequency(frequency), speed(speed), length(length), epilepsy(epilepsy), currentDelayIndex(0),
    lastSpawnTime(0)
{
    srand(static_cast<unsigned>(time(nullptr)));
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

    system("cls"); // очистка экрана после ввода
}

void AppManager::run() {
    SystemUtils::initConsole(120, 35);

    SystemUtils::sleep(200); //  подождать, пока буфер установитс€

    // принудительно обновим данные о высоте
    int h = SystemUtils::getConsoleHeight();


    system("cls");

    Line line(speed, length, epilepsy);

    while (true) {
        line.moveStep();
    }
}

//генерируем случайное рвем€ задержек 
void AppManager::generateSpawnSchedule() {
    spawnDelays.clear();
    spawnDelays.reserve(frequency);

    for (int i = 0; i < frequency; i++) {
        spawnDelays.push_back(rand() % 1000); // 0Ц999 мс
    }

    std::sort(spawnDelays.begin(), spawnDelays.end()); //сортируем список
    currentDelayIndex = 0;
    lastSpawnTime = GetTickCount64(); //врем€ от начала секунды = врем€ от начала процесса 

}
