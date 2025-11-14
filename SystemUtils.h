#pragma once
#pragma once
#include <windows.h>
#include <string>

class SystemUtils {
private:
    static HANDLE hConsole; // дескриптор консоли
    static int windowWidth;
    static int windowHeight;

public:
    // Инициализация и настройка окна
    static void initConsole(int width = 120, int height = 35);
    static void setConsoleSize(int width, int height);

    // Позиционирование курсора
    static void setCursorPosition(int x, int y);

    // Цвет текста
    static void setTextColor(WORD color);

    // Запись и очистка символов
    static void writeChar(int x, int y, char ch, WORD color);
    static void clearChar(int x, int y);

    // Получение размеров
    static int getConsoleWidth();
    static int getConsoleHeight();

    // Границы для линий
    static int getStartX();  // начальная позиция (слева)
    static int getEndX();    // конечная позиция (справа)

    // Пауза и курсор
    static void sleep(int ms);
    static void hideCursor(bool hide);
};

