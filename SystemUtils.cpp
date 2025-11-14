
#include "SystemUtils.h"
#include <windows.h>
#include <thread>
#include <chrono>
#include <iostream> 

HANDLE SystemUtils::hConsole = nullptr;
int SystemUtils::windowWidth = 120;
int SystemUtils::windowHeight = 35;

// Инициализация консоли
void SystemUtils::initConsole(int width, int height) {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    setConsoleSize(width, height);
    hideCursor(true);
    system("cls");

}


void SystemUtils::setConsoleSize(int width, int height) {
    // Установка буфера
    COORD bufferSize = { static_cast<SHORT>(width), static_cast<SHORT>(height) };
    SetConsoleScreenBufferSize(hConsole, bufferSize);

    // Установка размера окна
    SMALL_RECT windowSize = { 0, 0, static_cast<SHORT>(width - 1), static_cast<SHORT>(height - 1) };
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

    Sleep(50); 

    // реальные размеры окна
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        SHORT top = csbi.srWindow.Top;
        SHORT bottom = csbi.srWindow.Bottom;
        windowHeight = bottom - top + 1;
        windowWidth = csbi.dwSize.X;
    }
    else {
        //  если что-то пошло не так
        windowWidth = width;
        windowHeight = height;
    }
}


// Перемещение курсора
void SystemUtils::setCursorPosition(int x, int y) {
    COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(hConsole, pos);
}

// Цвет текста
void SystemUtils::setTextColor(WORD color) {
    SetConsoleTextAttribute(hConsole, color);
}

// Запись символа
void SystemUtils::writeChar(int x, int y, char ch, WORD color) {
    if (x < 0 || x >= windowWidth || y < 0 || y >= windowHeight) return;

    DWORD writtenChars = 0;
    COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    WriteConsoleOutputCharacterA(hConsole, &ch, 1, pos, &writtenChars);

    DWORD writtenAttrs = 0;
    WriteConsoleOutputAttribute(hConsole, &color, 1, pos, &writtenAttrs);
}

// Очистка символа
void SystemUtils::clearChar(int x, int y) {
    if (x < 0 || x >= windowWidth || y < 0 || y >= windowHeight) return;

    DWORD writtenChars = 0;
    COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    char space = ' ';
    WriteConsoleOutputCharacterA(hConsole, &space, 1, pos, &writtenChars);

    WORD attr = 7;
    WriteConsoleOutputAttribute(hConsole, &attr, 1, pos, &writtenChars);
}

// Реальные размеры видимой части консоли
int SystemUtils::getConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        SHORT left = csbi.srWindow.Left;
        SHORT right = csbi.srWindow.Right;
        return right - left + 1;
    }
    return windowWidth;
}

int SystemUtils::getConsoleHeight() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        SHORT top = csbi.srWindow.Top;
        SHORT bottom = csbi.srWindow.Bottom;
        return bottom - top + 1;
    }
    return windowHeight;
}

int SystemUtils::getStartX() { return 0; }

int SystemUtils::getEndX() {
    return getConsoleWidth() - 1;
}

// Задержка
void SystemUtils::sleep(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

// Скрыть/показать курсор
void SystemUtils::hideCursor(bool hide) {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = !hide;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}
