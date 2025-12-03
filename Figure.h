#pragma once
class Figure {
public:
    virtual void moveStep() = 0;
    virtual bool isFinished() const = 0;
    virtual ~Figure() = default; //виртуальный диструктор для правильного удаления (без него удалится только указатель в векторе, а с ним сама фигура)
};
