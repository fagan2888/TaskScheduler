#include "taskset.h"

#include <random>
#include <QString>


Taskset::Taskset()
{
    costFunction = std::numeric_limits<int>::max();

}

void Taskset::reassign(Task *t, int size, int number)
{
    for(int i = 0; i < size; i++)
    {
        set.append(t[i]);
    }
    this->size = size;
    this->setNumber = number;
}

void Taskset::reassign(Taskset t)
{
    set = t.getSet();
    size = t.getSize();
    setNumber = t.getSetNumber();
    costFunction = t.getCostFunction();
    milis = t.getMilis();
}

int Taskset::calculate()
{
    int time = 0;
    int cost = 0;
    for(int i = 0; i < size; i++)
    {
        time += set[i].getTime();
        cost += set[i].getWeight() * set[i].calcTard(time);
    }
    costFunction = cost;
    return cost;
}

void Taskset::createNeighbour()
{
    int choice = rand()%2;

    switch(choice)
    {
        case 0: doSwap();
                break;

        case 1: swapSegment();
                break;
    }
}

void Taskset::createNearNeighbour()
{
    int choice = rand()%2;

    switch(choice)
    {
        case 0: shiftRight();
                break;

        case 1: shiftLeft();
                break;
    }
}

int Taskset::getSize() const
{
    return size;
}

void Taskset::setSize(int value)
{
    size = value;
}
int Taskset::getSetNumber() const
{
    return setNumber;
}

void Taskset::setSetNumber(int value)
{
    setNumber = value;
}
int Taskset::getCostFunction() const
{
    return costFunction;
}

void Taskset::setCostFunction(int value)
{
    costFunction = value;
}

QString Taskset::toString()
{
    QString ret;

    ret = QString::number(milis) + "ms [";
    for(int i = 0; i < set.size(); i++)
    {
        ret += QString::number(set[i].getNumber());
        if(i != (set.size()-1)) ret+=",";
    }
    ret+="]\n";

    return ret;
}
int Taskset::getMilis() const
{
    return milis;
}

void Taskset::setMilis(int value)
{
    milis = value;
}
QVector<Task> Taskset::getSet() const
{
    return set;
}

void Taskset::setSet(const QVector<Task> &value)
{
    set = value;
}







void Taskset::doSwap()
{
    int idx1 = rand()%size;
    int idx2 = 0;
    
    do
    {
        idx2 = rand()%size;
    }while(idx1 == idx2);

    swap(idx1, idx2);
}

void Taskset::swap(int source, int dest)
{
    Task tmp;
    tmp = set[source];
    set[source] = set[dest];
    set[dest] = tmp;

}

void Taskset::swapSegment()
{
    int p = size/2;

    int width = rand()%p;
    int idx1 = rand()%(size - 2*width);
    int idx2 = rand()%(size-idx1-2*width) + idx1 + width;;

    for(int i = 0; i < width; i++)
    {
        swap(idx1+i, idx2+i);
    }

}

void Taskset::shiftRight()
{
    int shift = rand()%(size/2);

    for(int i = 0; i < shift; i++)
    {
        Task tmp = set.last();
        set.pop_back();
        set.prepend(tmp);
    }
}

void Taskset::shiftLeft()
{
    int shift = rand()%(size/2);

    for(int i = 0; i < shift; i++)
    {
         Task tmp = set.first();
         set.pop_front();
         set.append(tmp);
    }

}

