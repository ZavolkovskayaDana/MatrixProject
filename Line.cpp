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

    // инициализация времени последнего шага, чтобы первая отрисовка произошла сразу
    lastStepTime = GetTickCount64();
    exiting = false;
    finished = false;

}

// проверка: можно ли делать шаг согласно скорости
bool Line::canStep() {
    // скорость — символов в секунду -> интервал в мс на шаг
    int frameDelay = 1000 / (speed > 0 ? speed : 1);
    unsigned long long now = GetTickCount64();
    if (now - lastStepTime >= static_cast<unsigned long long>(frameDelay)) {
        lastStepTime = now;
        return true;
    }
    return false;
}

bool Line::isFinished() const {
    return finished;
}
void Line::moveStep()
{
    // не делаем ничего, если уже пометили на удаление
    if (finished) return;

    // проверяем, наступил ли момент для этого шага по скорости
    if (!canStep()) return;

    int height = SystemUtils::getConsoleHeight();

    // упростим высоту под безопасную зону
    int usableHeight = (height > 4) ? height - 3 : height;

    // 1) если линия ещё строится (её длина меньше заданной), добавляем по одному символу (эффект "выползания")
    if (!exiting && symbols.size() < static_cast<size_t>(length)) {
        int newX = symbols.empty() ? currentX : symbols.back().getX() + 1;
        int parity = (newX - baseX) & 1;
        int newY = startY + (parity ? zigDir : 0);
        newY = clampAndBounceY(newY, usableHeight, zigDir);

        symbols.emplace_back(newX, newY, epilepsy);
        symbols.back().draw();
        return;
    }

    // 2) если голова уже дошла (или мы в режиме exiting) — запускаем "плавное" удаление хвоста
    int endX = SystemUtils::getEndX();
    int headX = symbols.empty() ? -9999 : symbols.back().getX();

    if (!exiting && headX >= endX) {
        // голова достигла края — начинаем режим выхода
        exiting = true;
        // не добавляем новую голову — далее будем стирать хвост по одному символу
    }

    if (exiting) {
        // если нет символов — помечаем как finished
        if (symbols.empty()) {
            finished = true;
            return;
        }

        // затираем хвост (первый элемент)
        symbols.front().clear();

        // сдвигаем влево
        for (size_t i = 0; i + 1 < symbols.size(); ++i) {
            symbols[i] = symbols[i + 1];
        }

        // удаляем последний дубликат (теперь последний символ продублировался)
        symbols.pop_back();

        // если после удаления элементов массив пуст — считаем линию завершённой
        if (symbols.empty()) {
            finished = true;
            return;
        }

        // ничего больше не рисуем на этом шаге (уже сдвинули и затирали)
        return;
    }

    // 3) Обычный шаг: затираем хвост, смещаем все символы и добавляем новую голову
    // (должно выполняться только если !exiting и длина уже достигнута)
    if (symbols.size() >= static_cast<size_t>(length)) {
        // затираем первый(хвост)
        symbols.front().clear();

        // сдвигаем все элементы влево
        for (size_t i = 0; i + 1 < symbols.size(); ++i) {
            symbols[i] = symbols[i + 1];
        }

        // вычисляем новый символ для конца
        int newX = symbols.back().getX() + 1;
        int parity = (newX - baseX) & 1;
        int newY = startY + (parity ? zigDir : 0);
        newY = clampAndBounceY(newY, usableHeight, zigDir);

        // помещаем новую "голову" (перезаписывая последний элемент)
        symbols.back() = Symbol(newX, newY, epilepsy);
        symbols.back().draw();
    }
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



/*bool Line::isFinished() const {
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

*/
