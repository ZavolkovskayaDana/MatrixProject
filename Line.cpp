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
        int radiusMax,
        AppManager* owner)
    : speed(speed),
    length(length),
    epilepsy(epilepsy),
    explosionProbability(explosionProbability),
    radiusMin(radiusMin),
    radiusMax(radiusMax),
    owner(owner)     // сохраняем указатель на AppManager
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
/*void Line::moveStep()
{
    // 1. Пытаемся сделать взрыв
    if (!symbols.empty()) { //проверяем не пустой ли вектор (есть ли в линии символы)
        int chance = rand() % 1000; //случайная вероятность(шанс произойдет взрыв или нет)
        if (chance < explosionProbability) { //если случайное число < заданной вер-ти, то взрыв будет
            int x = symbols.front().getX(); //1-ый сивол 
            int y = symbols.front().getY();

            // создаём взрыв
            owner->createExplosion(x, y);

            // уменьшаем линию
            symbols.front().clear();       // стереть 1-ый символ
            symbols.erase(symbols.begin()); // удаляет 1-ый символ из вектора и сдвигает остальные 
            length--; //уменьшаем физ длину линии

            // если линия разрушена полностью — завершить
            if (length <= 0 || symbols.empty()) {
                finished = true;
                return;
            }
        }
    }


    // не делаем ничего, если уже пометили на удаление
    if (finished) return;

    // проверяем, наступил ли момент для этого шага по скорости
    if (!canStep()) return;

    int height = SystemUtils::getConsoleHeight();

    // упростим высоту под безопасную зону
    int usableHeight = (height > 4) ? height - 3 : height;

    // Растет если линия ещё строится (её длина меньше заданной), добавляем по одному символу (эффект "выползания")
    if (!exiting && symbols.size() < static_cast<size_t>(length)) {
        int newX = symbols.empty() ? currentX : symbols.back().getX() + 1;
        int parity = (newX - baseX) & 1;
        int newY = startY + (parity ? zigDir : 0);
        newY = clampAndBounceY(newY, usableHeight, zigDir);

        symbols.emplace_back(newX, newY, epilepsy);
        symbols.back().draw();
        return;
    }

    // если голова уже дошла (или мы в режиме exiting) — запускаем "плавное" удаление хвоста
    int endX = SystemUtils::getEndX();
    int headX = symbols.empty() ? -9999 : symbols.back().getX();

    if (!exiting && headX >= endX) {
        // голова достигла края — начинаем режим выхода
        exiting = true;
        // не добавляем новую голову — далее будем стирать хвост по одному символу
    }
    //затираем хвост 
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

    //  Обычный шаг: затираем хвост, смещаем все символы и добавляем новую голову
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
*/
/*void Line::moveStep()
{
    if (finished) return;

    // 1. Проверяем можно ли делать шаг
    if (!canStep()) return;

    int height = SystemUtils::getConsoleHeight();
    int usableHeight = (height > 4) ? height - 3 : height;

    // 2. Попытка сделать взрыв (когда symbols уже существуют)
    if (!symbols.empty()) {
        int chance = rand() % 1000;

        if (chance < explosionProbability) {
            int x = symbols.front().getX();
            int y = symbols.front().getY();

            owner->createExplosion(x, y);

            symbols.front().clear();
            symbols.erase(symbols.begin());
            length--;

            if (length <= 0 || symbols.empty()) {
                finished = true;
                return;
            }
        }
    }

    // 3. Если линия еще строится — растём
    if (!exiting && symbols.size() < static_cast<size_t>(length)) {

        int newX = symbols.empty() ? currentX : symbols.back().getX() + 1;

        int parity = (newX - baseX) & 1;
        int newY = startY + (parity ? zigDir : 0);

        newY = clampAndBounceY(newY, usableHeight, zigDir);

        symbols.emplace_back(newX, newY, epilepsy);
        symbols.back().draw();
        return;
    }

    // 4. Проверка достижения правого края
    int endX = SystemUtils::getEndX();
    int headX = symbols.empty() ? -9999 : symbols.back().getX();

    if (!exiting && headX >= endX) {
        exiting = true;
    }

    // 5. Плавное стирание в режиме exiting
    if (exiting) {
        if (symbols.empty()) {
            finished = true;
            return;
        }

        symbols.front().clear();

        for (size_t i = 0; i + 1 < symbols.size(); ++i) {
            symbols[i] = symbols[i + 1];
        }

        symbols.pop_back();

        if (symbols.empty()) {
            finished = true;
        }

        return;
    }

    // 6. Обычный шаг
    if (symbols.size() >= static_cast<size_t>(length)) {

        symbols.front().clear();

        for (size_t i = 0; i + 1 < symbols.size(); ++i) {
            symbols[i] = symbols[i + 1];
        }

        int newX = symbols.back().getX() + 1;
        int parity = (newX - baseX) & 1;
        int newY = startY + (parity ? zigDir : 0);

        newY = clampAndBounceY(newY, usableHeight, zigDir);

        symbols.back() = Symbol(newX, newY, epilepsy);
        symbols.back().draw();
    }
}
*/
void Line::moveStep()
{
    if (finished) return;

    // 1. Тайминг скорости
    if (!canStep()) return;

    int height = SystemUtils::getConsoleHeight();
    int usableHeight = (height > 4) ? height - 3 : height;

    // 2. Попытка сделать взрыв (только если линия уже есть)

    if (!symbols.empty())
    {
        int chance = rand() % 1000;

        if (chance < explosionProbability)
        {
            int x = symbols.front().getX();
            int y = symbols.front().getY();

            owner->createExplosion(x, y);

            // удаляем первый символ
            symbols.front().clear();
            symbols.erase(symbols.begin());
            length--;

            if (length <= 0 || symbols.empty()) {
                finished = true;
                return;
            }
        }
    }


    // 3. Если линия ещё растёт — добавляем голову справа
  
    if (!exiting && symbols.size() < static_cast<size_t>(length))
    {
        int newX = symbols.empty() ? currentX : symbols.back().getX() + 1;

        // правильный расчёт Y
        int parity = (newX - baseX) & 1;
        int newY = startY + (parity ? zigDir : 0);
        newY = clampAndBounceY(newY, usableHeight, zigDir);

        symbols.emplace_back(newX, newY, epilepsy);
        symbols.back().draw();
        return;
    }

  
    // 4. Проверка, дошли ли до правого края

    int endX = SystemUtils::getEndX();
    int headX = symbols.empty() ? -1 : symbols.back().getX();

    if (!exiting && headX >= endX)
    {
        exiting = true;
    }

    // 5. Режим "исчезновения" — смещаем и удаляем хвост
  
    if (exiting)
    {
        if (symbols.empty()) {
            finished = true;
            return;
        }

        symbols.front().clear();

        for (size_t i = 0; i + 1 < symbols.size(); ++i)
            symbols[i] = symbols[i + 1];

        symbols.pop_back();

        if (symbols.empty()) {
            finished = true;
        }

        return;
    }


    // 6. Обычное движение линии вправо

    if (symbols.size() >= static_cast<size_t>(length))
    {
        symbols.front().clear();

        for (size_t i = 0; i + 1 < symbols.size(); ++i)
            symbols[i] = symbols[i + 1];

        int newX = symbols.back().getX() + 1;

        int parity = (newX - baseX) & 1;
        int newY = startY + (parity ? zigDir : 0);

        newY = clampAndBounceY(newY, usableHeight, zigDir);

        symbols.back() = Symbol(newX, newY, epilepsy);
        symbols.back().draw();
    }
}
