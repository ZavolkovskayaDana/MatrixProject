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
        << "  скорость          1–30 символов/сек\n"
        << "  длина             1–30 символов\n"
        << "  режим_эпилепсии   Y/N\n"
        << "\nПример: matrix.exe 15 10 Y\n"
        << "Если параметры не указаны — программа запустится в диалоговом режиме.\n";
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

    int speed = 0;
    int length = 0;
    bool epilepsy = false;

    // Проверка на режим помощи 
    if (argc > 1 &&
        (std::string(argv[1]) == "--help" || std::string(argv[1]) == "/?")) {
        showHelp();
        return 0;
    }

    // Режим командной строки 
    if (argc == 4) {
        if (!isValidInt(argv[1], 1, 30, speed)) {
            std::cerr << "Ошибка: скорость должна быть в диапазоне 1–30. Обратитесь за помощью --help.\n";
            return 1;
        }
        if (!isValidInt(argv[2], 1, 30, length)) {
            std::cerr << "Ошибка: длина должна быть в диапазоне 1–30. Обратитесь за помощью --help.\n";
            return 1;
        }

        std::string ep = argv[3];

        if (ep.size() != 1) {
            std::cerr << "Ошибка: режим эпилепсии должен быть OДНИМ символом (Y или N).\n";
            return 1;
        }

        if (ep == "Y" || ep == "y") epilepsy = true;
        else if (ep == "N" || ep == "n") epilepsy = false;
        else {
            std::cerr << "Ошибка: режим эпилепсии должен быть Y или N. Обратитесь за помощью --help.\n";
            return 1;
        }
    }
    // Диалоговый режим 
    else {
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


        //  Инициализация и запуск приложения 
        AppManager app(speed, length, epilepsy);
        app.run();

        return 0;

    }
}


