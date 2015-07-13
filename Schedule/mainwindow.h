#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QThread>

#include <task/taskset.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void updateAnn(int row, int res, int time);
    void updateIwo(int row, int res, int time);

private slots:
    void on_pushButton_clicked();
    void on_solveButton_clicked();
    void on_solveButton_2_clicked();
    void solveAnnealing();
    void solveIWO();
    void updateResult(QTableWidget* t, int row, int res);
    void updateBrunel(QTableWidget* t, int row, int bru);
    void updateTime(QTableWidget* t, int row, int time);
    void addRow(QTableWidget* t, int i);
    void clearTable();
    void guiSlot();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_tableWidget_cellClicked(int row, int column);

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();


    void on_pushButton_6_clicked();

private:
    int instances;
    Ui::MainWindow *ui;
    Taskset* set, *anset, *wedset;
    QVector<int> results;
    bool dataLoaded;
    bool resultsLoaded;
    int currentRow;
    int currentCol;

    QString annParam();
    QString wedParam();

    enum table
    {
        colBrunel = 0,
        colRes = 1,
        colDiff = 2,
        colTim = 3
    };
};

#endif // MAINWINDOW_H
