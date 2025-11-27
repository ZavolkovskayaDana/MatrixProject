#include "Line.h"
#include "SystemUtils.h"
#include <cstdlib>
#include <algorithm>
#include <thread>
#include <chrono>
#include <iostream> 


// с отскоком 
static int clampAndBounceY(int y, int height, int& zigDir) {
    if (y <= 1) {
        y = 1;
        zigDir = +1; // отскок вниз
    }
    else if (y >= height - 2) {
        y = height - 2;
        zigDir = -1; // отскок вверх
    }
    return y;
}

Line::Line(int speed, int length, bool epilepsy)
    : speed(speed), length(length), epilepsy(epilepsy)
{
    baseX = SystemUtils::getStartX();
    currentX = baseX;

    int height = SystemUtils::getConsoleHeight() - 3;
    int margin = 2; // запас сверху и снизу

    if (height <= 3) {
        startY = height / 2;
    }
    else {
        // безопасная зона [margin, height - margin - 1]
        startY = rand() % (height - 2 * margin) + margin;
    }

    // направление зигзага — вверх или вниз
    zigDir = (startY <= height / 2) ? +1 : -1;

}

/*void Line::initialize(int column)
{
    currentX = column;
    baseX = column;
    symbols.clear();
}
*/
/*void Line::initialize() {
    int frameDelay = 1000 / (speed > 0 ? speed : 1);
    symbols.clear();
    currentX = baseX;

    int height = SystemUtils::getConsoleHeight() - 3;

    if (startY < 1) startY = 1;
    if (startY > height - 2) startY = height - 2;

    // постепенная отрисовка зигзага
    for (int i = 0; i < length; ++i) {
        int x = currentX + i;
        int parity = (x - baseX) & 1;
        int y = startY + (parity ? zigDir : 0);
        y = clampAndBounceY(y, height, zigDir);

        symbols.emplace_back(x, y, epilepsy);
        symbols.back().draw();

        SystemUtils::sleep(frameDelay);
    }
}
*/

/*void Line::initialize(int column)
{
    currentX = column;
    baseX = column;
    symbols.clear();
}*/

bool Line::isFinished() const {
    int endX = SystemUtils::getEndX();
    return !symbols.empty() && symbols.back().getX() >= endX;
}

void Line::moveStep()
{
    int frameDelay = 1000 / (speed > 0 ? speed : 1);
    int height = SystemUtils::getConsoleHeight() - 3;

    // линия ещё растёт
    if (symbols.size() < static_cast<size_t>(length)) {
        int newX = symbols.empty() ? currentX : symbols.back().getX() + 1;

        int parity = (newX - baseX) & 1;
        int newY = startY + (parity ? zigDir : 0);
        newY = clampAndBounceY(newY, height, zigDir);

        symbols.emplace_back(newX, newY, epilepsy);
        symbols.back().draw();

        //SystemUtils::sleep(frameDelay);
        return;
    }

    // линия завершена — больше не отрисовывать
    if (isFinished()) {
        for (auto& s : symbols)
            s.clear();
        return;
    }

    // обычный шаг — хвост очищается
    symbols.front().clear();
    for (size_t i = 0; i < symbols.size() - 1; i++)
        symbols[i] = symbols[i + 1];

    int newX = symbols.back().getX() + 1;

    int parity = (newX - baseX) & 1;
    int newY = startY + (parity ? zigDir : 0);
    newY = clampAndBounceY(newY, height, zigDir);

    symbols.back() = Symbol(newX, newY, epilepsy);
    symbols.back().draw();

    //SystemUtils::sleep(frameDelay);
}


