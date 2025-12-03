#include "Explosion.h"
#include <cstdlib>
#include <ctime>

Explosion::Explosion(int x, int y, int radiusMin, int radiusMax)
    : centerX(x),
    centerY(y),
    radiusMin(radiusMin),
    radiusMax(radiusMax),
    currentRadius(radiusMin),
    finished(false)
{
    lastStepTime = GetTickCount64(); //время от начала процесса 
}

bool Explosion::isFinished() const {
    return finished;
}

void Explosion::moveStep() {
    if (finished) return;

    // задержка 500 мс (2 симв/сек)
    unsigned long long now = GetTickCount64();
    if (now - lastStepTime < 500)
        return;

    lastStepTime = now;

    // Рисуем текущую окружность
    drawCircle(currentRadius);

    // Увеличиваем текущий радиус
    currentRadius++;

    // Если радиус достиг максимума, то взрыв закончен
    if (currentRadius > radiusMax) {
        finished = true;
    }
}

//рисуем точки вокруг цента 
void Explosion::drawCircle(int r) {
    WORD color = randomColor();

    // 8 направлений 
    SystemUtils::writeChar(centerX + r, centerY, '*', color);
    SystemUtils::writeChar(centerX - r, centerY, '*', color);
    SystemUtils::writeChar(centerX, centerY + r, '*', color);
    SystemUtils::writeChar(centerX, centerY - r, '*', color);

    // диагонали
    SystemUtils::writeChar(centerX + r, centerY + r, '*', color);
    SystemUtils::writeChar(centerX + r, centerY - r, '*', color);
    SystemUtils::writeChar(centerX - r, centerY + r, '*', color);
    SystemUtils::writeChar(centerX - r, centerY - r, '*', color);
}

WORD Explosion::randomColor() const {
    int base = 1 + (rand() % 7);  // случайный цвет
    WORD colorAttr = static_cast<WORD>(base);
    if (rand() % 2) colorAttr |= FOREGROUND_INTENSITY;
    return colorAttr;
}