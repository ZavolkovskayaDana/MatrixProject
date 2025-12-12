#pragma once
class Figure {
public:
    virtual void moveStep() = 0; //виртуальный метод 
    virtual bool isFinished() const = 0;

    virtual bool wantsExplosion() const { return false; }
    virtual int getExplosionX() const { return 0; }
    virtual int getExplosionY() const { return 0; }
    virtual void resetExplosionRequest() {}
    virtual ~Figure() = default; //виртуальный диструктор для правильного удаления (без него удалится только указатель в векторе, а с ним сама фигура)
};
