#ifndef CPU_H
#define CPU_H
#include <QCoreApplication>
#include <QtCore>
#include <thread>
#include "line.h"
#include "pcb.h"
#include "process.h"
#include "mmu.h"
#include <vector>
#include <QThread>
#include "corethread.h"
#include <QList>
#include <QObject>
#include <mutex>

class CPU : public QObject
{
    Q_OBJECT
public:
    explicit CPU(QObject* = nullptr){}
    ~CPU();
    const static int CYCLE_AMOUNT = 50;
    QList<std::shared_ptr<Process>> currentProcesses;
    QList<std::shared_ptr<Process>> processesWaiting;
    QList<std::shared_ptr<Process>> resourcesWaiting;
    QList<std::shared_ptr<Process>> processesWithMemory;
    void RunProcess();
    void RunProcesses();
    void DoCalculation(std::shared_ptr<Process>, int);
    void ProcessHardwareInterrupt(int timeNeeded);
    void SetMMU(std::shared_ptr<MMU>);
    bool LoadMemory(std::shared_ptr<Process>, QList<uint> );
    QList<uint> GetPages(std::shared_ptr<Process>);
    void DoSetup(QThread&);
    std::shared_ptr<MMU> mmu;

    std::shared_ptr<Process>process1; std::shared_ptr<Process>process2;
    std::shared_ptr<Process>process3;  std::shared_ptr<Process>process4;
signals:
    void cpuCountChanged();
    void finishedProcessing(std::shared_ptr<Process>, QString);
    void processIn();
public slots:
    void AddToList(std::shared_ptr<Process> process);
};

#endif // CPU_H
