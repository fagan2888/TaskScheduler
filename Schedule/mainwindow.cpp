#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "task/taskset.h"
#include "algorithms/annealing.h"
#include "algorithms/weed.h"

#include <QtConcurrent/QtConcurrentRun>
#include <QFile>
#include <QFuture>
#include <QDateTime>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QTime>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QList<QString> columnName;
    columnName.push_back("Brunel");
    columnName.push_back("Computation");
    columnName.push_back("Difference");
    columnName.push_back("Time");
    dataLoaded = false;
    resultsLoaded = false;
    instances = 0;
    currentCol = 0;
    currentRow = 0;

    for(quint8 i = 0; i < ui->tableWidget->columnCount(); i++)
    {
        ui->tableWidget->setHorizontalHeaderItem(i, new QTableWidgetItem(columnName.at(i)));
        ui->tableWidget_2->setHorizontalHeaderItem(i, new QTableWidgetItem(columnName.at(i)));
    }


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    instances = 125;
    clearTable();

    QString name = ui->comboBox->currentText();
    QString filepath = "../input/wt"+name+".txt";
    QString resultpath = "../results/wt"+name+"opt.txt";

    QFile source(filepath);
    source.open( QIODevice::ReadOnly | QIODevice::Text );

    if(source.isOpen())
    {
        qDebug() <<"File opened properly\n";
        QTextStream in(&source);
        int taskcnt = name.toInt();
        Task* tasks = new Task[taskcnt];
        set = new Taskset[instances];
        anset = new Taskset[instances];
        wedset = new Taskset[instances];

        for(int k = 0; k < instances; k++)
        {
            int time = 0;
            for(int i = 0; i < taskcnt; i++)
            {
                in >> time;
                tasks[i].setTime(time);
            }

            int weight = 0;
            for(int i = 0; i < taskcnt; i++)
            {
                in >> weight;
                tasks[i].setWeight(weight);
            }

            int deadline = 0;
            for(int i = 0; i < taskcnt; i++)
            {
                in >> deadline;
                tasks[i].setDeadline(deadline);
            }

            for(int i = 0; i < taskcnt; i++)
            {
                tasks[i].setNumber(i);
            }

            set[k].reassign(tasks, taskcnt, k);

        }
        source.close();
        delete[] tasks;

        QFile result(resultpath);
        result.open( QIODevice::ReadOnly | QIODevice::Text );

        if(result.isOpen())
        {
            qDebug() <<"File opened properly\n";
            QTextStream in(&result);
            int res;
            for(int k = 0; k < instances; k++)
            {
                in >> res;
                results.append(res);
                updateBrunel(ui->tableWidget,k,res);
                updateBrunel(ui->tableWidget_2,k,res);
            }
            result.close();
        }

        else
        {
            qDebug() <<"Result not opened\n";
        }

        dataLoaded = true;
        resultsLoaded = true;
    }

    else
    {
        qDebug() <<"File not opened\n";
    }
}

void MainWindow::on_solveButton_clicked()
{
    if(dataLoaded == false)
    {
        QMessageBox::information(this, tr("Load data!"), tr("Input file not chosen"));
        return;
    }
    solveAnnealing();
}

void MainWindow::on_solveButton_2_clicked()
{
    if(dataLoaded == false)
    {
        QMessageBox::information(this, tr("Load data!"), tr("Input file not chosen"));
        return;
    }
    solveIWO();
}

void MainWindow::solveAnnealing()
{
    double minT = ui->minTbox->value();
    double maxT = ui->maxTbox->value();
    int steps = ui->stepBox->value();
    double linear = ui->linBox->value();

    int option = 0;
    if(ui->radioLinear->isChecked())
    {
        option = 0;
    }
    else if(ui->radioLogar->isChecked())
    {
        option = 1;
    }

    for(int i = ui->fromBox1->value()-1; i < ui->ToBox1->value(); i++)
    {
        if(i >= instances) break;

        Annealing solver(maxT, minT, steps, linear, option, this);
        anset[i].reassign(set[i]);
        solver.assign(&anset[i]);
        solver.solve();
    }
}

void MainWindow::solveIWO()
{
    int generations = ui->genBox->value();
    int initialPop = ui->popBox->value();
    int maxColony = ui->maxWeedBox->value();
    int minSeeds = ui->minSeedBox->value();
    int maxSeeds = ui->maxSeedBox->value();
    float mutation = ui->mutBox->value();

    Weed solver(generations,initialPop,maxColony,minSeeds,maxSeeds,mutation, this);

    for(int i = ui->FromBox2->value()-1; i < ui->ToBox2->value(); i++)
    {
        if(i >= instances) break;
        wedset[i].reassign(set[i]);
        solver.assign(&wedset[i]);
        solver.solve();

    }
}

void MainWindow::updateBrunel(QTableWidget *t, int row, int bru)
{
    QTableWidgetItem* i = t->item(row, colBrunel);
    QString brunel = QString::number(bru);
    i->setText(brunel);
}

void MainWindow::updateTime(QTableWidget *t, int row, int time)
{
    QTableWidgetItem* i = t->item(row, colTim);
    QString timer = QString::number(time);
    i->setText(timer);
}

void MainWindow::updateAnn(int row, int res, int time)
{
     updateResult(ui->tableWidget, row, res);
     updateTime(ui->tableWidget, row, time);
}

void MainWindow::updateIwo(int row, int res, int time)
{
     updateResult(ui->tableWidget_2, row, res);
     updateTime(ui->tableWidget_2, row, time);
}

void MainWindow::updateResult(QTableWidget *t, int row, int res)
{
    QTableWidgetItem* i = t->item(row, colRes);
    i->setText(QString::number(res));

    if(resultsLoaded)
    {
        int diff = res - t->item(row, colBrunel)->text().toInt();
        i = t->item(row, colDiff);
        i->setText(QString::number(diff));
    }
}


void MainWindow::addRow(QTableWidget* t, int i)
{
    t->insertRow(i);

    QString Brunel, Result, Diff, Time;

    Brunel = "0";
    Result = "-";
    Diff = "-";
    Time = "-";

    QTableWidgetItem* empty1 = new QTableWidgetItem();
    QTableWidgetItem* empty2 = new QTableWidgetItem();
    QTableWidgetItem* empty3 = new QTableWidgetItem();
    QTableWidgetItem* empty4 = new QTableWidgetItem();

    empty1->setText(Brunel);
    empty2->setText(Result);
    empty3->setText(Diff);
    empty4->setText(Time);

    t->setItem(i,colBrunel,empty1);
    t->setItem(i,colRes,empty2);
    t->setItem(i,colDiff,empty3);
    t->setItem(i,colTim,empty4);
}

void MainWindow::clearTable()
{
    while (ui->tableWidget->rowCount() > 0)
    {
        ui->tableWidget->removeRow(0);
        ui->tableWidget_2->removeRow(0);
    }

    for(int i = 0; i < instances; i++)
    {
        addRow(ui->tableWidget, i);
        addRow(ui->tableWidget_2, i);
    }
}

void MainWindow::guiSlot()
{
    QTextStream(stdout)<<"Signal caught\n";
}

void MainWindow::on_pushButton_2_clicked()
{
    QString filepath = QFileDialog::getOpenFileName(this, tr("Open File"),
    "",
    tr(".txt"));

    QFile source(filepath);
    source.open( QIODevice::ReadOnly | QIODevice::Text );

    if(source.isOpen())
    {
        qDebug() <<"File opened properly\n";
        QTextStream in(&source);

        int taskcnt;

        in >> instances;
        clearTable();
        in >> taskcnt;

        Task* tasks = new Task[taskcnt];
        set = new Taskset[instances];

        for(int k = 0; k < instances; k++)
        {
            int time = 0;
            for(int i = 0; i < taskcnt; i++)
            {
                in >> time;
                tasks[i].setTime(time);
            }

            int weight = 0;
            for(int i = 0; i < taskcnt; i++)
            {
                in >> weight;
                tasks[i].setWeight(weight);
            }

            int deadline = 0;
            for(int i = 0; i < taskcnt; i++)
            {
                in >> deadline;
                tasks[i].setDeadline(deadline);
            }

            for(int i = 0; i < taskcnt; i++)
            {
                tasks[i].setNumber(i);
            }

            set[k].reassign(tasks, taskcnt, k);

        }
        source.close();
        delete[] tasks;

        dataLoaded = true;
    }

    else
    {
        qDebug() <<"File not opened\n";
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    if(dataLoaded == false)
    {
        QMessageBox::information(this, tr("Load data!"), tr("Input file not chosen"));
        return;
    }

    QString message = anset[currentRow].toString();

    QMessageBox::information(this, tr("Solution!"), tr(message.toStdString().c_str()));
    return;
}

void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    currentRow = row;
    currentCol = column;
}

void MainWindow::on_pushButton_4_clicked()
{
    if(dataLoaded == false)
    {
        QMessageBox::information(this, tr("Load data!"), tr("Input file not chosen"));
        return;
    }

    QString message = wedset[currentRow].toString();

    QMessageBox::information(this, tr("Solution!"), tr(message.toStdString().c_str()));
    return;
}

void MainWindow::on_pushButton_5_clicked()
{
    QDateTime now = QDateTime::currentDateTime();
    QString path =  now.toString("yyyy-MM-dd-hh-mm-ss");
    QFile f("Ann" + path+".csv");
    f.open(QIODevice::WriteOnly | QIODevice::Text);
    bool cond = true;
    cond = f.isOpen();

    if(cond)
    {
        QTextStream data(&f);
        QStringList strList;
        strList<<annParam();
        data <<strList.join(";") << "\n";
        strList.clear();
        strList <<"Brunel"<<"Computation"<<"Difference"<<"Time [ms]";
        data << strList.join(";") << "\n";

        for( int r = 0; r < ui->tableWidget->rowCount(); ++r )
        {
            strList.clear();
            QTableWidget * table = ui->tableWidget;

            for( int c = 0; c < ui->tableWidget->columnCount(); ++c )
            {
                strList <<table->item(r,c)->text();

            }

            data << strList.join(";") << "\n";
        }
        f.close();
    }
}

QString MainWindow::annParam()
{
    QString minT = QString::number(ui->minTbox->value());
    QString maxT = QString::number(ui->maxTbox->value());
    QString steps = QString::number(ui->stepBox->value());
    QString linear = QString::number(ui->linBox->value());

    if(ui->radioLinear->isChecked())
    {
        return "Min T = " + minT +
                " Max T = " + maxT +
                " Inner steps = " + steps +
                " Linear factor = " + linear;
    }
    else if(ui->radioLogar->isChecked())
    {
        return "Min T = " + minT +
                " Max T = " + maxT +
                " Steps = " + steps +
                " Logarithmic cooling";
    }


}

QString MainWindow::wedParam()
{
    QString generations = QString::number(ui->genBox->value());
    QString initialPop = QString::number(ui->popBox->value());
    QString maxColony = QString::number(ui->maxWeedBox->value());
    QString minSeeds = QString::number(ui->minSeedBox->value());
    QString maxSeeds = QString::number(ui->maxSeedBox->value());
    QString mutation = QString::number(ui->mutBox->value());

    return "Generations = " + generations +
            " Initial population = " + initialPop +
            " Colony max = " + maxColony +
            " Minimum seeds = " + minSeeds +
            " Maximum seeds = " + maxSeeds +
            " Mutation factor = " + mutation;
}

void MainWindow::on_pushButton_6_clicked()
{
    QDateTime now = QDateTime::currentDateTime();
    QString path =  now.toString("yyyy-MM-dd-hh-mm-ss");
    QFile f("Weed" + path+".csv");
    f.open(QIODevice::WriteOnly | QIODevice::Text);
    bool cond = true;
    cond = f.isOpen();

    if(cond)
    {
        QTextStream data(&f);
        QStringList strList;
        strList<<wedParam();
        data <<strList.join(";") << "\n";
        strList.clear();
        strList <<"Brunel"<<"Computation"<<"Difference"<<"Time [ms]";
        data << strList.join(";") << "\n";

        for( int r = 0; r < ui->tableWidget_2->rowCount(); ++r )
        {
            strList.clear();
            QTableWidget * table = ui->tableWidget_2;

            for( int c = 0; c < ui->tableWidget_2->columnCount(); ++c )
            {
                strList <<table->item(r,c)->text();

            }

            data << strList.join(";") << "\n";
        }
        f.close();
    }
}
