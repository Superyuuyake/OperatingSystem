#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QThread>
#include "scheduler.h"
#include "roundrobin.h"
#include "highestpriority.h"
#include "worker.h"
#include "process.h"
#include "multilevelfeedbackschedule.h"
#include "shortestjobfirst.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void TakeInCommand(QString, int, uint);
    void AddCycles(uint numberOfCycles);
    QString schedulerType;
    std::shared_ptr<Scheduler> scheduler;
    std::shared_ptr<RoundRobin> roundRobin;
    std::shared_ptr<MultiLevelFeedbackSchedule>  mlq;
    std::shared_ptr<HighestPriority> highestPriority;
    std::shared_ptr<ShortestJobFirst> shortestJob;
    Worker* worker = new Worker;
    Worker* worker2 = new Worker();
    QThread* workerThread = new QThread();
    QThread* workerThread2 =new QThread();
    QThread* workerThread3 =new QThread();
signals:
    void StartExec();
    void AddProcesses();
private slots:


    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void printToTextEdit(QString);

    void on_resourceBar_valueChanged();

    void on_resources2Bar_valueChanged();

    void on_memoryBar_valueChanged();

    void on_virtualBar_valueChanged();

    void update_memory_bar(int);

    void update_virtual_bar(int);

    void update_resources_bar(int, int);

    void updateUnderTheHood(QString);

    void updateReadyQ(int);

    void updateWaitingQ(int);

    void updateNewQ(int);

    void updateTerminatedQ(int);

    void updateParentQ(int);

    void updateCriticalQ(int);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
