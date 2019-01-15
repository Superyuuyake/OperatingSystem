#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "process.h"
#include "pcb.h"
#include "line.h"
#include "cpu.h"
#include "mailbox.h"
#include "mutexlock.h"
#include "underthehoodworker.h"
#include <iostream>
#include <memory>
#include <algorithm>
#include <QObject>
#include <QtCore>
#include <QThread>
#include <QCoreApplication>
#include <QVector>
#include <QString>
#include <QPair>
#include <QList>

//Process created, if first instruction is calculate put into ready, if IO, put in waiting.
//Process ready queue done by short term scheduler
class Scheduler : public QObject
{
    Q_OBJECT
public:
    explicit Scheduler(QObject* = nullptr){continueRunning=false;}
    Scheduler(const Scheduler&);
    ~Scheduler();
    virtual void RunAllReady();
    virtual int ShortTermSchedule();
    virtual void TerminateProcess(std::shared_ptr<Process>, QList<int>& , QList<std::shared_ptr<Process>>&);
    virtual void AddProcess(std::shared_ptr<Process>);

    virtual void MoveProcess(std::shared_ptr<Process>, QList<std::shared_ptr<Process>>&, QList<std::shared_ptr<Process>>&, QString);
    //void SendToCore(std::shared_ptr<Process>, int amount);
    void NextRunState(QString, std::shared_ptr<Process>);
    virtual void SimulateIO(std::shared_ptr<Process>);
    void AddCycles(uint);
    void CirculateNew();
    void StopExecution();
    void SetCoreMMU();
    void SetUp();
    void printTerminatedProcesses();
    void ResetCritical();
    std::shared_ptr<Process> GrabParent(uint);

    QList<std::shared_ptr<Process>> newQueue;
    QList<std::shared_ptr<Process>> readyQueue;
    QList<std::shared_ptr<Process>> runningQueue;
    QList<std::shared_ptr<Process>> jobQueue;
    QList<std::shared_ptr<Process>> waitingQueue;
    QList<int> terminatedQueue;
    QList<std::shared_ptr<Process>> criticalQueue;
    QList<std::shared_ptr<Process>> parentQueue;
    QList<std::shared_ptr<Process>> parentNoMemQueue;

    std::shared_ptr<MMU>mmu = std::make_shared<MMU>();
    CPU processor;
    MailBox mailBox;
    UnderTheHoodWorker textUpdater;
    QThread* workerThread = new QThread();

    MutexLock mutex;
    QString nextState;
    uint totalCycles;
    bool continueRunning;
signals:
    void finished();

public slots:
    void beginExecution();
    void returnedFromCPU(std::shared_ptr<Process>, QString);
};

#endif // SCHEDULER_H

