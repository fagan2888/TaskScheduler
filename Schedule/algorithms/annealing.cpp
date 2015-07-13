#include "annealing.h"
#include <QDebug>
#include <QTime>
Annealing::Annealing()
{
}

Annealing::Annealing(float max, float min, int steps, float linear, int option, MainWindow* mw)
    : maxTemp(max)
    , minTemp(min)
    , factor(linear)
    , cooling(option)
    , iterations(steps)
    , w(mw)

{
    myset = new Taskset();
    curStep = 0;
    epochs = 0;
}

void Annealing::solve()
{
    std::random_device rd;
    curTemp = maxTemp;
    int localStep = 0;
    int currentCost, delta;
    Taskset current;
    Taskset prim;
    current = *myset;
    int size = myset->getSize();
    epochs = 0;

    QTime myTimer;
    myTimer.start();

    do {
        epochs++;
        for(int i = 0; i<iterations; i++)
        {
            prim = current;
            prim.createNeighbour();
            currentCost = prim.calculate();
            if(currentCost < myset->calculate())
                *myset = prim;

            delta = currentCost - current.calculate();
            if(delta < 0)
                current = prim;
            else {
                std::uniform_real_distribution<double> d(0.0,1.0);
                if(d(rd) < exp(-delta/(double)curTemp))
                    current = prim;
            }
        }
        localStep++;
        curTemp = cool(size, localStep, cooling);
    } while(curTemp > minTemp);

    int nMilliseconds = myTimer.elapsed();
    myset->setMilis(nMilliseconds);
    int solutionCost = myset->calculate();
    //qDebug() <<"Epochs "<<epochs<<"\nSet "<<myset->getSetNumber()<<". -->"<<solutionCost;
    qDebug()<<myset->toString();
    int num = myset->getSetNumber();
    w->updateAnn(num, solutionCost, nMilliseconds);

}

double Annealing::getMaxTemp() const
{
    return maxTemp;
}

void Annealing::setMaxTemp(double value)
{
    maxTemp = value;
}
double Annealing::getMinTemp() const
{
    return minTemp;
}

void Annealing::setMinTemp(double value)
{
    minTemp = value;
}
int Annealing::getCooling() const
{
    return cooling;
}

void Annealing::setCooling(int value)
{
    cooling = value;
}
int Annealing::getIterations() const
{
    return iterations;
}

void Annealing::setIterations(int value)
{
    iterations = value;
}
int Annealing::getEpochs() const
{
    return epochs;
}

void Annealing::setEpochs(int value)
{
    epochs = value;
}
double Annealing::getFactor() const
{
    return factor;
}

void Annealing::setFactor(double value)
{
    factor = value;
}



double Annealing::cool(int taskCnt, int step, int option)
{
    double logs = log(step*0.01/taskCnt + 2.72);
    switch(option)
    {
        case 0: curTemp = factor*curTemp; break;
        case 1: curTemp = curTemp / logs;  break;
    }
    return curTemp;

}

void Annealing::assign(Taskset s)
{
    *myset = s;
}

void Annealing::assign(Taskset *s)
{
    myset = s;
}




