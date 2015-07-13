#ifndef TASKSET_H
#define TASKSET_H

#include <QVector>
#include "task.h"

class Taskset
{
public:
    Taskset();
    void reassign(Task* t, int size, int number);
    void reassign(Taskset t);
    int  calculate();
    void createNeighbour();
    void createNearNeighbour();

    int getSize() const;
    void setSize(int value);

    int getSetNumber() const;
    void setSetNumber(int value);

    int getCostFunction() const;
    void setCostFunction(int value);

    QString toString();

    int getMilis() const;
    void setMilis(int value);

    QVector<Task> getSet() const;
    void setSet(const QVector<Task> &value);

private:
    QVector<Task> set;
    int size;
    int setNumber;
    int costFunction;
    int milis;
    void swap(int source, int dest);
    void swapSegment();
    void doSwap();
    void shiftLeft();
    void shiftRight();
};

#endif // TASKSET_H
