#ifndef TASK_H
#define TASK_H

class Task
{
public:
    Task();
    Task(int t, int w, int d, int n=0);
    int getTime() const;
    void setTime(int value);

    int getWeight() const;
    void setWeight(int value);

    int getDeadline() const;
    void setDeadline(int value);

    int calcTard(int val);

    int getNumber() const;
    void setNumber(int value);

private:
    int time;
    int weight;
    int deadline;
    int number;
};

#endif // TASK_H
