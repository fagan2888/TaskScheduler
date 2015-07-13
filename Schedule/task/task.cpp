#include "task.h"

Task::Task()
    :   time(0)
    ,   weight(0)
    ,   deadline(0)
    ,   number(0)
{
}

Task::Task(int t, int w, int d, int n)
    :   time(t)
    ,   weight(w)
    ,   deadline(d)
    ,   number(n)
{
}

int Task::getTime() const
{
    return time;
}

void Task::setTime(int value)
{
    time = value;
}
int Task::getWeight() const
{
    return weight;
}

void Task::setWeight(int value)
{
    weight = value;
}
int Task::getDeadline() const
{
    return deadline;
}

void Task::setDeadline(int value)
{
    deadline = value;
}

int Task::calcTard(int val)
{
    val -= deadline;
    return (val>0) ? val : 0;
}
int Task::getNumber() const
{
    return number;
}

void Task::setNumber(int value)
{
    number = value;
}



