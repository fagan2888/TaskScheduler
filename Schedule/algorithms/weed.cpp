#include "weed.h"

#include <QDebug>
#include <QTime>

Weed::Weed()
{
}

Weed::Weed(int gen, int init, int maxCol, int minSeed, int maxSeed, float mut, MainWindow* mw)
    : generations(gen)
    , initialPop(init)
    , maxColony(maxCol)
    , minSeeds(minSeed)
    , maxSeeds(maxSeed)
    , mutation(mut)
    , w(mw)
{
    myset = new Taskset();
}

bool lessThan(const Taskset &t1, const Taskset &t2)
{
    return t1.getCostFunction() < t2.getCostFunction();
}

int Weed::solve()
{
    QVector<Taskset> colony;
    for(int i = 0; i < initialPop; i++)
    {
        Taskset tmp = *myset;
        tmp.createNeighbour();
        tmp.calculate();
        colony.push_back(tmp);
    }

    QTime myTimer;
    myTimer.start();

    for(int curGen = 0; curGen < generations; curGen++)
    {
        int parents = colony.size();
        for(int i = 0; i < parents; i++)
        {
            Taskset tmp = colony[i];
            int seeds = qrand()%maxSeeds;
            if(seeds< minSeeds) seeds = minSeeds;

            for(int j = 0; j < seeds; j++)
            {
                tmp.createNearNeighbour();

                float chance = qrand()%101/100.0;

                if(chance < mutation)
                    tmp.createNeighbour();

                tmp.calculate();
                colony.append(tmp);
            }
        }

        qSort(colony.begin(), colony.end(), lessThan);

        if(colony.size() > maxColony)
        {
            do
            {
                colony.pop_back();

            }while(colony.size() != maxColony);
        }

    }
    int solutionCost = colony[0].calculate();
    int nMilliseconds = myTimer.elapsed();
    colony[0].setMilis(nMilliseconds);
    qDebug()<<colony[0].toString();
    w->updateIwo(myset->getSetNumber(), solutionCost, nMilliseconds);
    return solutionCost;


}

void Weed::assign(Taskset s)
{
    *myset = s;
}

void Weed::assign(Taskset *s)
{
    myset = s;
}
int Weed::getGenerations() const
{
    return generations;
}

void Weed::setGenerations(int value)
{
    generations = value;
}
int Weed::getInitialPop() const
{
    return initialPop;
}

void Weed::setInitialPop(int value)
{
    initialPop = value;
}
int Weed::getMaxColony() const
{
    return maxColony;
}

void Weed::setMaxColony(int value)
{
    maxColony = value;
}
int Weed::getMinSeeds() const
{
    return minSeeds;
}

void Weed::setMinSeeds(int value)
{
    minSeeds = value;
}
int Weed::getMaxSeeds() const
{
    return maxSeeds;
}

void Weed::setMaxSeeds(int value)
{
    maxSeeds = value;
}





