#ifndef PCB_H
#define PCB_H
#include <QtCore>
#include <QObject>
#include <QCoreApplication>
#include "line.h"
#include "pipe.h"
#include "pagetable.h"
#include<QtDebug>
#include <QString>

class PCB : public QObject
{
    Q_OBJECT
public:
    enum class state : int{New, Ready, Run, Wait, Exit };
    PCB();
    PCB(const PCB&);
    ~PCB();
    void operator=(const PCB& pcb);
    bool operator==(const PCB& pcb);
    void SetCurrentState(state);
    QString GetCurrentState();
    void SetProcessPriority(unsigned int priority);
    unsigned int GetProcessPriority();
    void IncreaseProgramCounter();
    unsigned int GetProgramCounter();
    void SetParentProcess(uint);
    uint GetParentProcess();
    void SetPID(unsigned int pid);
    unsigned int GetPID();
    void SetTotalTimeRemaining(int);
    int GetTotalTimeRemaining();
    void SetLastCycle(uint);
    void SetPipe(std::shared_ptr<Pipe>);
    std::shared_ptr<Pipe> GetPipe();
    uint GetLastCycle();
    void SetSharedVariable(int);
    int GetSharedVariable();
    void SetPipeKey(int);
    int GetPipeKey();
    void SetMailBoxNumber(int);
    int GetMailBoxNumber();
    void SetMailBoxKey(int);
    int GetMailBoxKey();

    PageTable pageTable;
    uint pagesNeeded;
    unsigned int baseRegisterVM;
    unsigned int limitRegisterVM;

    int resourceOneNeeded;
    int resourceTwoNeeded;
    QQueue<int> resourceOneAllocated;
    QQueue<int> resourceTwoAllocated;

    unsigned int programCounter;
    bool isChild;
    bool inCritical;
    unsigned int pid;
    QString mlQStatus;
    const char* stateNames[5] = {"New", "Ready", "Run", "Wait", "Exit" };
private:
    state currentState;
    std::shared_ptr<Pipe> pipe;
    int pipeKey;
    int sharedVariable;
    unsigned int processPriority;
    uint parentProcess;
    int totalTimeRemaining;
    uint lastCycle;
    int mailBoxKey;
    int mailBoxNumber;

};

#endif // PCB_H
