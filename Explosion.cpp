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

    // Рисуем текущее кольцо
    drawCircle(currentRadius);

    // Увеличиваем текущий радиус
    currentRadius++;

    // Если радиус достиг максимума, то взрыв закончен
    if (currentRadius > radiusMax) {
        finished = true;
    }
}