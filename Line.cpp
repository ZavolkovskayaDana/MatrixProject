#include "Line.h"
#include "AppManager.h"
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

Line::Line(int speed, int length, bool epilepsy,
        int explosionProbability,
        int radiusMin,
        int radiusMax)
    : speed(speed),
    length(length),
    epilepsy(epilepsy),
    explosionProbability(explosionProbability),
    radiusMin(radiusMin),
    radiusMax(radiusMax)

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

    // инициализация времени последнего шага, чтобы первая отрисовка произошла сразу
    lastStepTime = GetTickCount64();
    exiting = false;
    finished = false;

}

// проверка: можно ли делать шаг согласно скорости (отслеживает интервал между шагами)
bool Line::canStep() {
    // скорость — символов в секунду -> интервал в мс на шаг
    int frameDelay = 1000 / (speed > 0 ? speed : 1);
    unsigned long long now = GetTickCount64();
    if (now - lastStepTime >= static_cast<unsigned long long>(frameDelay)) {
        lastStepTime = now;
        return true; //пора сделать шаг линии
    }
    return false;
}

//линия считается завершённой только после полного стирания хвоста
bool Line::isFinished() const {
    return finished;
}

void Line::moveStep()
{
    if (finished) return;

    // 0. Если линия заморожена после взрыва — стоим
    if (exploding) {
        unsigned long long now = GetTickCount64();
        if (now < explodeLockEnd) {
            // ещё не прошло 0.5 секунды — линия стоит
            return;
        }
        else {
            // пауза закончилась — размораживаем линию
            exploding = false;
            lastStepTime = now;  // чтобы сразу не было рывка по скорости
        }
    }


    // 1. Тайминг скорости линии
    if (!canStep()) return;

    int height = SystemUtils::getConsoleHeight();
    int usableHeight = (height > 4) ? height - 3 : height;

    // 2. Попытка сделать взрыв (убирает только 1 символ)
    if (!symbols.empty()) //если у нас вектор не пустой (в линии есть символы)
    {
        int chance = rand() % 1000; //рандомное число

        if (chance < explosionProbability) //рандомное число < заданной вер-ти
        {
            int x = symbols.front().getX(); //координаты центра взрыва (1-ый символ)
            int y = symbols.front().getY();

            needExplosion = true; //нам нужен взры
            explosionX = x;
            explosionY = y;


            // удаляем 1-ый символ
            symbols.front().clear();
            symbols.erase(symbols.begin());
            length--;   //реальная длина линии уменьшается 

            //УДАЛИТЬ включаем "заморозку" линии на 0.5 секунды
            exploding = true;
            explodeLockEnd = GetTickCount64() + 500; // 500 мс = 2 шага в секунду у взрыва

            if (length <= 0 || symbols.empty()) { //длина 0
                finished = true;
                return;
            }
        }
    }


    // 3. Режим роста линии (пока не достигла полной длины)
    if (!exiting && symbols.size() < static_cast<size_t>(length))
    {
        int newX = symbols.empty() ? currentX : symbols.back().getX() + 1;

        int parity = (newX - baseX) & 1;
        int newY = startY + (parity ? zigDir : 0);
        newY = clampAndBounceY(newY, usableHeight, zigDir);

        symbols.emplace_back(newX, newY, epilepsy);
        symbols.back().draw();
        return;
    }

    // 4. Проверяем достижение правого края - начинаем выход
    int endX = SystemUtils::getEndX();
    int headX = symbols.empty() ? -1 : symbols.back().getX();

    if (!exiting && headX >= endX) {
        exiting = true;
    }


    // 5. Режим плавного удаление линии справа-налево
    if (exiting)
    {
        if (symbols.empty()) {
            finished = true;
            return;
        }

        // очищаем хвост
        symbols.front().clear();

        // сдвигаем всё влево
        for (size_t i = 0; i + 1 < symbols.size(); ++i) {
            symbols[i] = symbols[i + 1];
            symbols[i].draw();          // ПЕРЕРИСОВКА!
        }

        symbols.pop_back();

        if (symbols.empty()) {
            finished = true;
        }

        return;
    }


    // 6. Обычное движение линии (после достижения длины)
    if (symbols.size() >= static_cast<size_t>(length))
    {
        // стираем хвост
        symbols.front().clear();

        // сдвигаем всё влево
        for (size_t i = 0; i + 1 < symbols.size(); ++i) {
            symbols[i] = symbols[i + 1];
            symbols[i].draw();       // ПЕРЕРИСОВКА!
        }

        // создаём новую голову
        int newX = symbols.back().getX() + 1;

        int parity = (newX - baseX) & 1;
        int newY = startY + (parity ? zigDir : 0);
        newY = clampAndBounceY(newY, usableHeight, zigDir);

        symbols.back() = Symbol(newX, newY, epilepsy);
        symbols.back().draw();
    }
}
