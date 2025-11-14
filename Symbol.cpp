#include "Symbol.h"
#include <ctime>

// Конструктор
Symbol::Symbol(int x, int y, bool epilepsy)
    : x(x), y(y), epilepsy(epilepsy)
{
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned>(time(nullptr)));
        seeded = true;
    }
    randomizeChar();
    randomizeColor();
}

// Нарисовать символ
void Symbol::draw() {
    if (x < 0 || y < 0 ||
        x >= SystemUtils::getConsoleWidth() ||
        y >= SystemUtils::getConsoleHeight())
        return;
    SystemUtils::writeChar(x, y, ch, color);
}

// Стереть символ
void Symbol::clear() {
    if (x < 0 || y < 0 ||
        x >= SystemUtils::getConsoleWidth() ||
        y >= SystemUtils::getConsoleHeight())
        return;
    SystemUtils::clearChar(x, y);
}

// Обновить символ (новый знак и если нужно цвет)
void Symbol::update() {
    randomizeChar();
    if (epilepsy) randomizeColor();
    draw();
}

// Координаты
int Symbol::getX() const { return x; }
int Symbol::getY() const { return y; }

void Symbol::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
}

// Выбрать случайный печатный символ (от '!' до '~')
void Symbol::randomizeChar() {
    ch = 33 + rand() % 94;
}

void Symbol::randomizeColor() {
    if (epilepsy) {
        // выбираем случайный основной цвет и случайно добавляем интенсивность
        int base = 1 + (rand() % 7); // 1..7 (R,G,B combinations)
        WORD colorAttr = static_cast<WORD>(base);
        if (rand() % 2) colorAttr |= FOREGROUND_INTENSITY;
        color = colorAttr;
    }
    else {
        color = FOREGROUND_GREEN | FOREGROUND_INTENSITY; // ярко-зелёный
    }
}
