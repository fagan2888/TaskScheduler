#ifndef ANNEALING_H
#define ANNEALING_H

#include "task/taskset.h"
#include "mainwindow.h"
#include <QObject>

class Annealing : public QObject
{
    Q_OBJECT

public:
    Annealing();
    Annealing(float max, float min, int steps, float linear, int option, MainWindow *mw);
    void solve();
    void assign(Taskset s);
    void assign(Taskset *s);

    double getMaxTemp() const;
    void setMaxTemp(double value);

    double getMinTemp() const;
    void setMinTemp(double value);

    int getCooling() const;
    void setCooling(int value);

    int getIterations() const;
    void setIterations(int value);

    int getEpochs() const;
    void setEpochs(int value);

    double getFactor() const;
    void setFactor(double value);

signals:
    void sendUpdate();

private:
    Taskset* myset;
    double maxTemp;
    double minTemp;
    double curTemp;
    double factor;
    int cooling;
    int iterations;
    int curStep;
    int epochs;
    double cool(int taskCnt, int step, int option = 0);
    MainWindow *w;

};

#endif // ANNEALING_H
