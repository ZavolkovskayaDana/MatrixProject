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
    lastStepTime = GetTickCount64();
}

bool Explosion::isFinished() const {
    return finished;
}

char Explosion::randomChar() const {
    return 33 + rand() % 94;
}

void Explosion::moveStep() {
    if (finished) return;

    unsigned long long now = GetTickCount64();
    if (now - lastStepTime < 500) //2 сим/сек
        return;

    lastStepTime = now;

    // Стираем предыдущие точки взрыва
    for (auto& p : lastPoints) {
        int x = p.first;
        int y = p.second;

        if (x >= 0 && x < SystemUtils::getConsoleWidth() &&
            y >= 0 && y < SystemUtils::getConsoleHeight())
        {
            SystemUtils::clearChar(x, y);
        }
    }
    lastPoints.clear();

    //  Если взрыв завершён — выходим
    if (currentRadius > radiusMax) {
        finished = true;
        return;
    }

    //  Рисуем текущее кольцо
    drawCircle(currentRadius);

    // Увеличиваем радиус
    currentRadius++;
}

void Explosion::drawCircle(int r) {
    WORD color = randomColor();

    int W = SystemUtils::getConsoleWidth();
    int H = SystemUtils::getConsoleHeight();

    auto safePut = [&](int x, int y) {
        if (x >= 0 && x < W && y >= 0 && y < H) {
            SystemUtils::writeChar(x, y, randomChar(), randomColor());
            lastPoints.push_back({ x, y });
        }
        };

    // Окружность Брезенхэма
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    while (y >= x) {
        safePut(centerX + x, centerY + y);
        safePut(centerX - x, centerY + y);
        safePut(centerX + x, centerY - y);
        safePut(centerX - x, centerY - y);

        safePut(centerX + y, centerY + x);
        safePut(centerX - y, centerY + x);
        safePut(centerX + y, centerY - x);
        safePut(centerX - y, centerY - x);

        x++;

        if (d > 0) {
            y--;
            d += 4 * (x - y) + 10;
        }
        else {
            d += 4 * x + 6;
        }
    }
}

WORD Explosion::randomColor() const {
    int base = 1 + (rand() % 7);
    WORD color = static_cast<WORD>(base);
    if (rand() % 2) color |= FOREGROUND_INTENSITY;
    return color;
}
