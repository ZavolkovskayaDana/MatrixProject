#define NOMINMAX
#include <iostream>
#include <string>
#include "AppManager.h"
#include <limits>   // для std::numeric_limits
#include <algorithm> // для std::remove_if

// Функция для вывода справки 
void showHelp() {
    std::cout << "Программа 'Матрица'\n"
        << "Использование:\n"
        << "  matrix.exe [скорость] [длина] [режим_эпилепсии]\n"
        << "Параметры:\n"
        << "  frequency         1–30 линий/секунду\n"
        << "  скорость          1–30 символов/сек\n"
        << "  длина             1–30 символов\n"
        << "  режим_эпилепсии   Y/N\n"
        << "\nПример: matrix.exe 5 15 10 Y\n"
        << "Если параметры не указаны — программа запустится в диалоговом режиме.\n"
        << "  probability          1–1000 вероятность взрыва\n"
        << "  rMin                 1–10 минимальный радиус\n"
        << "  rMax                 rMin–10 максимальный радиус\n\n";
}

// Проверка корректности числа
bool isValidInt(const std::string& str, int min, int max, int& value) {
    try {
        value = std::stoi(str);
        return value >= min && value <= max;
    }
    catch (...) {
        return false;
    }
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");

    int frequency = 0;
    int speed = 0;
    int length = 0;
    bool epilepsy = false;
    int explosionProbability = 0;
    int radiusMin = 0;
    int radiusMax = 0;


    // Проверка на режим помощи 
    if (argc > 1 &&
        (std::string(argv[1]) == "--help" || std::string(argv[1]) == "/?")) {
        showHelp();
        return 0;
    }

    // Режим командной строки 
    if (argc == 8) {

        if (!isValidInt(argv[1], 1, 30, frequency)) {
            std::cerr << "Ошибка: частота должна быть в диапазоне 1–30.\n";
            return 1;
        }

        if (!isValidInt(argv[2], 1, 30, speed)) {
            std::cerr << "Ошибка: скорость должна быть в диапазоне 1–30. Обратитесь за помощью --help.\n";
            return 1;
        }
        if (!isValidInt(argv[3], 1, 30, length)) {
            std::cerr << "Ошибка: длина должна быть в диапазоне 1–30. Обратитесь за помощью --help.\n";
            return 1;
        }

        std::string ep = argv[4];

        if (ep.size() != 1) {
            std::cerr << "Ошибка: режим эпилепсии должен быть одним символом (Y или N).\n";
            return 1;
        }

        if (ep == "Y" || ep == "y") epilepsy = true;
        else if (ep == "N" || ep == "n") epilepsy = false;
        else {
            std::cerr << "Ошибка: режим эпилепсии должен быть Y или N. Обратитесь за помощью --help.\n";
            return 1;
        }

        // новые параметры Лабы 3
        if (!isValidInt(argv[5], 1, 1000, explosionProbability)) {
            std::cerr << "Ошибка: вероятность должна быть 1–1000.\n";
            return 1;
        }

        if (!isValidInt(argv[6], 1, 10, radiusMin)) {
            std::cerr << "Ошибка: минимальный радиус 1–10.\n";
            return 1;
        }

        if (!isValidInt(argv[7], radiusMin, 10, radiusMax)) {
            std::cerr << "Ошибка: максимальный радиус должен быть в диапазоне "
                << radiusMin << "–10.\n";
            return 1;
        }
        AppManager app(frequency, speed, length, epilepsy, explosionProbability, radiusMin, radiusMax);
        app.run();
        return 0;
    }
    
    // Диалоговый режим 
    else {

        std::cout << "Введите частоту линий (1–30): ";
        while (!(std::cin >> frequency) || frequency < 1 || frequency > 30) {
            std::cout << "Ошибка! Введите число от 1 до 30: ";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
        std::cout << "Введите скорость линий (1–30): ";
        while (!(std::cin >> speed) || speed < 1 || speed > 30) {
            std::cout << "Ошибка! Введите число от 1 до 30: ";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }

        std::cout << "Введите длину линий (1–30): ";
        while (!(std::cin >> length) || length < 1 || length > 30) {
            std::cout << "Ошибка! Введите число от 1 до 30: ";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }

        char mode;
        std::string input;

        std::cout << "Включить режим эпилепсии? (Y/N): ";

        while (true) {
            std::cin >> input;


            // Проверяем, что введён ровно один символ и он корректный
            if (input.size() == 1) {
                mode = input[0];
                if (mode == 'Y' || mode == 'y') {
                    epilepsy = true;
                    break;
                }
                else if (mode == 'N' || mode == 'n') {
                    epilepsy = false;
                    break;
                }
            }

            std::cout << "Ошибка! Введите только одну букву (Y или N): ";
            std::cin.clear();  // сбрасываем флаг ошибки
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // очищаем остатки ввода
        }

        std::cout << "Введите вероятность взрыва линии (1–1000): ";
        while (!(std::cin >> explosionProbability) || explosionProbability < 1 || explosionProbability > 1000) {
            std::cout << "Ошибка! Введите число от 1 до 1000: ";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }

        std::cout << "Введите минимальный радиус взрыва (1–10): ";
        while (!(std::cin >> radiusMin) || radiusMin < 1 || radiusMin > 10) {
            std::cout << "Ошибка! Введите число от 1 до 10: ";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }

        std::cout << "Введите максимальный радиус взрыва (" << radiusMin << "–10): ";
        while (!(std::cin >> radiusMax) || radiusMax < radiusMin || radiusMax > 10) {
            std::cout << "Ошибка! Введите число от " << radiusMin << " до 10: ";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }


        //  Инициализация и запуск приложения 
        AppManager app(frequency, speed, length, epilepsy, explosionProbability, radiusMin, radiusMax);
        app.run();

        return 0;

    }
}


