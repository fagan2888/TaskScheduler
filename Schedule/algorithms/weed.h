#ifndef WEED_H
#define WEED_H

#include "task/taskset.h"
#include "mainwindow.h"

class Weed
{
public:
    Weed();
    Weed(int gen, int init, int maxCol, int minSeed, int maxSeed, float mut, MainWindow* mw);
    int solve();
    void assign(Taskset s);
    void assign(Taskset *s);

    int getGenerations() const;
    void setGenerations(int value);

    int getInitialPop() const;
    void setInitialPop(int value);

    int getMaxColony() const;
    void setMaxColony(int value);

    int getMinSeeds() const;
    void setMinSeeds(int value);

    int getMaxSeeds() const;
    void setMaxSeeds(int value);

private:
    Taskset* myset;
    int generations;
    int initialPop;
    int maxColony;
    int minSeeds;
    int maxSeeds;
    float mutation;
    MainWindow *w;

};

#endif // WEED_H
