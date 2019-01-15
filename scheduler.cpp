#include "scheduler.h"
#include "process.h"
#include "pcb.h"
#include "line.h"
#include "cpu.h"
#include <QObject>
#include <QtCore>
#include <QCoreApplication>
#include <QVector>
#include <mutex>
#include <QtDebug>
#include <QString>
#include <algorithm>

static std::mutex mtx;
static std::condition_variable cvCritical;
static bool inCritical=false;


Scheduler::Scheduler(const Scheduler& b1)
{
     readyQueue = b1.readyQueue;
     waitingQueue = b1.waitingQueue;
     jobQueue = b1.jobQueue;
     terminatedQueue = b1.terminatedQueue;
}
Scheduler::~Scheduler(){}

void Scheduler::AddProcess(std::shared_ptr<Process> process)
{

    newQueue.push_back(process);
    if(!processor.mmu->VerifyLegality(process))
    {
       textUpdater.setThreadStatus("Illegal process. Process aborted");
       TerminateProcess(process, this->terminatedQueue, this->newQueue);
    }
    QString name = process->textBlock[process->PCB.GetProgramCounter()].getName();
    if(name != "wait" && name != "yield")
    {
        bool ableToLoad = processor.mmu->LoadProcess(process);

        if(ableToLoad)
        {
            textUpdater.setReadyQ(1);
            MoveProcess(process, this->readyQueue, this->newQueue, "ready");
        }
        else
        {
            textUpdater.setNewQ(1);
        }
    }
}

void Scheduler::MoveProcess(std::shared_ptr<Process>process, QList<std::shared_ptr<Process>>&  toQueue, QList<std::shared_ptr<Process>>& fromQueue, QString state)
{
    if(state == "wait")
    {
        process->PCB.SetCurrentState(PCB::state::Wait);
    }
    else if(state == "critical")
    {
        process->PCB.SetCurrentState(PCB::state::Wait);
    }
    else if(state == "parent")
    {
        process->PCB.SetCurrentState(PCB::state::Wait);
    }
    else if(state == "nomem")
    {
        process->PCB.SetCurrentState(PCB::state::Wait);
    }
    else if(state == "ready")
    {
        process->PCB.SetCurrentState(PCB::state::Ready);
    }
    else   //EXITING
    {
        //TerminateProcess(process, terminatedQueue, fromQueue);


    }
    toQueue.push_back(process);
    fromQueue.removeOne(process);
}
/***************************************************************************************
 * Circulates through the new queue to see if there is enough memory to begin execution
 * Allovates the memory on the MMU if there is enough memory.
 ***************************************************************************************/
void Scheduler::CirculateNew()
{
    if(!newQueue.isEmpty())
    {
        textUpdater.setThreadStatus("Circulating New Queue");
        for(int i =0; i<newQueue.length(); i++)
        {
            std::shared_ptr<Process>process = newQueue[i];
            QString name = process->textBlock[process->PCB.GetProgramCounter()].getName();
            if(name != "wait" && name != "yield")
            {
                bool ableToLoad = processor.mmu->LoadProcess(process);
                if(ableToLoad)
                {
                    textUpdater.setNewQ(-1);
                    textUpdater.setReadyQ(1);
                    QString id = QString::number(process->PCB.pid);
                    QString processName = process->textBlock[process->PCB.GetProgramCounter()].getName();
                    textUpdater.setThreadStatus("Moving process " + id + " " + processName  +  " From newQ");
                    MoveProcess(process, this->readyQueue, this->newQueue, "ready");
                }
            }
        }
    }
}
/*********************************************************
 * Runs the ready processes for a given amount of cycles
 *********************************************************/
void Scheduler::RunAllReady()
{
    int hardwareInterrupt = 0;
    while(continueRunning && totalCycles>0)
    {
        while(processor.processesWaiting.size()<4 && !readyQueue.isEmpty())
        {

            int nextIndex = ShortTermSchedule();
            if(nextIndex >= 0)
            {
                std::shared_ptr<Process>nextProcess = readyQueue.takeAt(nextIndex);
                QString processId = QString::number(nextProcess->PCB.GetPID());
                QString processPriority = QString::number(nextProcess->PCB.GetProcessPriority());
                QString processSizeLeft = QString::number(nextProcess->PCB.GetTotalTimeRemaining());
                textUpdater.setThreadStatus("Process:: " + processId + " selected.   Priority:: " + processPriority + "   Size remaining:: " + processSizeLeft);
                nextProcess->PCB.SetCurrentState(PCB::state::Run);
                runningQueue.push_back(nextProcess);
                processor.AddToList(nextProcess);
            }
            hardwareInterrupt = rand()%10000;
            if(hardwareInterrupt == 1992 && readyQueue.size()>0)
            {
                textUpdater.setThreadStatus("SERVICING HARDWARE I/O INTERRUPT!!");
                processor.ProcessHardwareInterrupt((1+rand()%2));
            }
            if(readyQueue.size()==0 && waitingQueue.size() == 0 && runningQueue.size() == 0)
            {
                printTerminatedProcesses();
                continueRunning = false;
                totalCycles = 0;
            }
        }
        if(waitingQueue.size()>0)
        {
            SimulateIO(waitingQueue.front());
        }
        if(processor.processesWaiting.size()>0)
        {
            processor.RunProcess();
        }
        totalCycles--;
    }
    printTerminatedProcesses();
    continueRunning = false;
    totalCycles = 0;
}
/**************************************
 * Simulates I/O for a given process
 **************************************/
void Scheduler::SimulateIO(std::shared_ptr<Process>process)
{
    QString processId = QString::number(process->PCB.GetPID());
    textUpdater.setThreadStatus("Processing I/O from process:: " + processId + " Amount: " + QString::number(process->GetCurrentLineAmount()));
    process->SubtractAmount(process->GetCurrentLineAmount());
    QString NextLine = process->textBlock[process->PCB.GetProgramCounter()].getName();
    if(NextLine == "yield")
    {
        textUpdater.setWaitingQ(-1);
        TerminateProcess(process, terminatedQueue, waitingQueue);
        CirculateNew();
    }
    else if(NextLine == "calculate")
    {
        textUpdater.setWaitingQ(-1);
        textUpdater.setReadyQ(1);
        textUpdater.setThreadStatus("Moving " + processId + " to readyQ from waitQ");
        MoveProcess(process, readyQueue, waitingQueue, "ready");
    }
    else
    {
        MoveProcess(process, waitingQueue, waitingQueue, "wait");
    }
}
/**************************************
 * Terminates a given process
 * Frees the memory allocated on mmu
 **************************************/
void Scheduler::TerminateProcess(std::shared_ptr<Process>process, QList<int>& toQueue, QList<std::shared_ptr<Process>>& fromQueue)
{
    processor.mmu->FreeMemory(process);
    fromQueue.removeOne(process);
    textUpdater.setTerminatedQ(1);
    toQueue.push_back(int(process->PCB.GetPID()));
    process->PCB.SetCurrentState(PCB::state::Exit);
    if(process->PCB.isChild)
    {
        std::shared_ptr<Process> parentProcess = GrabParent(process->PCB.GetParentProcess());

        if(parentProcess->PCB.pageTable.pagesUsed==0)
        {
            bool ableToLoad = processor.mmu->LoadProcess(parentProcess);
            if(ableToLoad)
            {
                textUpdater.setReadyQ(1);
                textUpdater.setParentQ(-1);
                MoveProcess(parentProcess, this->readyQueue, this->parentQueue, "ready");
            }
            else
            {
                processor.mmu->FreeMemory(parentProcess);
                if(parentProcess->PCB.isChild)
                {
                    std::shared_ptr<Process> grandParent = GrabParent(process->PCB.GetParentProcess());
                    textUpdater.setParentQ(-1);
                    TerminateProcess(grandParent, this->terminatedQueue, this->parentQueue);
                }
                textUpdater.setParentQ(-1);
                TerminateProcess(parentProcess, this->terminatedQueue, this->parentQueue);
            }
        }
        else
        {
            textUpdater.setReadyQ(1);
            textUpdater.setParentQ(-1);
            MoveProcess(parentProcess, readyQueue, parentQueue, "ready");
        }
    }
}
/**************************************
 * Searches for the parent of a child process
 * returns the parent process.
 **************************************/
std::shared_ptr<Process> Scheduler::GrabParent(uint parentId)
{
    uint index = 0;
    for(uint i = 0; i<uint(parentQueue.size()); i++)
    {
        if(parentQueue[int(i)]->PCB.GetPID() == parentId)
        {
            QString id = QString::number(parentId);
            textUpdater.setThreadStatus("Parent:: " + id + " moving back to readyQ");
            index=i;
            break;
        }
    }
    return parentQueue[int(index)];
}

void Scheduler::SetCoreMMU()
{
    processor.SetMMU(mmu);
}
void Scheduler::returnedFromCPU(std::shared_ptr<Process> process, QString nextState)
{
    QString processId = QString::number(process->PCB.GetPID());
    if(nextState  == "io")
    {
        textUpdater.setThreadStatus("Moving:: " + processId + " to I/O waitQ");
        textUpdater.setReadyQ(-1);
        textUpdater.setWaitingQ(1);
        MoveProcess(process, this->waitingQueue, this->runningQueue, "wait");
    }
    else if(nextState == "exit")
    {
        textUpdater.setThreadStatus("Terminating:: " + processId);
        textUpdater.setReadyQ(-1);
        TerminateProcess(process, this->terminatedQueue, this->runningQueue);
        CirculateNew();
    }
    else if(nextState == "kill")
    {
        textUpdater.setThreadStatus("Killing illegal process:: " + processId);
        textUpdater.setReadyQ(-1);
        TerminateProcess(process, this->terminatedQueue, this->runningQueue);
        CirculateNew();
    }
    else if(nextState == "calculate")
    {
        MoveProcess(process, this->readyQueue, this->runningQueue, "ready");
    }
    else if(nextState == "critical")
    {
        if(process->PCB.inCritical)
        {
            process->PCB.IncreaseProgramCounter();
            MoveProcess(process, this->readyQueue, this->runningQueue, "ready");
        }
        else
        {
        //std::unique_lock<std::mutex> lck(mtx);
        //if(inCritical)
        if(mutex.isLocked)
        {
            textUpdater.setThreadStatus("Moving:: " + processId + " to criticalQ");
            textUpdater.setCriticalQ(1);
            textUpdater.setReadyQ(-1);
            MoveProcess(process, this->criticalQueue, this->runningQueue, "critical");
        }
        else
        {
        mutex.LockTheMutex();
        textUpdater.setThreadStatus("Process:: " + processId + " in critical section");

        process->PCB.inCritical = true;
        process->PCB.IncreaseProgramCounter();
         if(criticalQueue.contains(process))
         {
             textUpdater.setReadyQ(1);
             textUpdater.setCriticalQ(-1);
             MoveProcess(process, this->readyQueue, this->criticalQueue, "ready");
         }
         else
         {
             MoveProcess(process, this->readyQueue, this->runningQueue, "ready");
         }
        }
        }
    }
    else if(nextState == "out")
    {
         mutex.UnlockTheMutex();
         process->PCB.IncreaseProgramCounter();
         process->PCB.inCritical = false;
        if(criticalQueue.size()>0)
        {
            textUpdater.setCriticalQ(-1);
            textUpdater.setReadyQ(1);
            MoveProcess(criticalQueue.front(), this->readyQueue, this->criticalQueue, "ready");
        }
        MoveProcess(process, this->readyQueue, this->runningQueue, "ready");
        textUpdater.setThreadStatus("Moving:: " + processId + " finished in critical section");
    }
    else if(nextState == "communicate")
    {
        if(process->PCB.isChild)
        {
            std::shared_ptr<Pipe> pipe = process->PCB.GetPipe();
            process->PCB.SetSharedVariable(pipe->Read(process->PCB.GetPipeKey()));
            QString parentId = QString::number(process->PCB.GetParentProcess());
            QString sharedV = QString::number(process->PCB.GetSharedVariable());
            textUpdater.setThreadStatus("Child:: " + processId + " reading shared variable: " +sharedV+" from Parent: " +parentId);
        }
        else
        {
            int sharedVar = rand()%1000;
            process->PCB.SetSharedVariable(sharedVar);
            std::shared_ptr<Pipe> pipe = process->PCB.GetPipe();
            pipe->Write(process->PCB.GetPipeKey(), sharedVar);
            QString sharedV = QString::number(sharedVar);
            textUpdater.setThreadStatus("Parent:: " + processId + " writing shared variable: " + sharedV);
        }
        process->PCB.IncreaseProgramCounter();
        MoveProcess(process, this->readyQueue, this->runningQueue, "ready");
    }
    else if(nextState == "mailbox")
    {
        int mailboxAddress = process->PCB.GetMailBoxNumber();
        int key = process->PCB.GetMailBoxKey();
        QString mailbox = QString::number(mailboxAddress);

        if(process->PCB.GetPID()%2 ==0)
        {
            process->PCB.SetSharedVariable(mailBox.Read(mailboxAddress, key));
            QString sharedV = QString::number(process->PCB.GetSharedVariable());
            textUpdater.setThreadStatus("Process:: " + processId + " reading shared variable: " + sharedV + " from mailbox#: " + mailbox);
        }
        else
        {
            mailBox.Write(mailboxAddress, key, int(process->PCB.GetPID()));
            textUpdater.setThreadStatus("Process:: " + processId + " reading shared variable: " + processId + " from mailbox#: " + mailbox);
        }
        process->PCB.IncreaseProgramCounter();
        MoveProcess(process, this->readyQueue, this->runningQueue, "ready");
    }
    else if(nextState == "fork")
    {
        textUpdater.setThreadStatus("Process: " + processId + " forked");
        process->PCB.IncreaseProgramCounter();
        if(process->PCB.GetPipe() == nullptr)
        {
            int lock = rand()%(1000000); int sharedVariable = rand()%(1000);
            std::shared_ptr<Pipe> pipe = std::make_shared<Pipe>(lock, sharedVariable);
            process->PCB.SetPipe(pipe);
            process->PCB.SetPipeKey(lock);
            process->PCB.SetSharedVariable(sharedVariable);
        }
        std::shared_ptr<Process> newChildProcess = std::make_shared<Process>();
        Process copyProcess = *process;
        newChildProcess->CreateChildProcess(copyProcess);
        QString childId = QString::number(newChildProcess->PCB.GetPID());
        textUpdater.setThreadStatus("ChildProcess: " + childId + " created");
        newQueue.push_back(newChildProcess);
        if(!processor.mmu->VerifyLegality(process))
        {
           TerminateProcess(newChildProcess, this->terminatedQueue, this->newQueue);
           textUpdater.setReadyQ(-1);
           TerminateProcess(process, this->terminatedQueue, this->runningQueue);
        }
        else
        {
            if(processor.mmu->LoadProcess(newChildProcess))
            {
                textUpdater.setParentQ(1);
                MoveProcess(process, parentQueue, runningQueue, "parent");
                MoveProcess(newChildProcess, readyQueue, newQueue, "ready");
            }
            else
            {
                processor.mmu->FreeMemory(process);
                textUpdater.setParentQ(1);
                textUpdater.setNewQ(1);
                MoveProcess(process, parentQueue, runningQueue, "parent");
            }

        }

    }
}

void Scheduler::SetUp()
{
    processor.moveToThread(workerThread);
    qRegisterMetaType<std::shared_ptr<Process>>("std::shared_ptr<Process>");
    connect(&processor, SIGNAL(finishedProcessing(std::shared_ptr<Process>, QString)), this, SLOT(returnedFromCPU(std::shared_ptr<Process>, QString)));
    workerThread->start();
}

void Scheduler::AddCycles(uint cycles)
{
    totalCycles = (totalCycles + cycles);
}
void Scheduler::StopExecution()
{
    continueRunning = false;
}
int Scheduler::ShortTermSchedule()
{
    return 0;
}
void Scheduler::beginExecution()
{
    continueRunning=true;
    RunAllReady();
}
void Scheduler::printTerminatedProcesses()
{
    QString allProcesses;
    for(int i = 0; i<terminatedQueue.size(); i++)
    {
        allProcesses.append(QString::number(terminatedQueue[i]) + ", ");
    }
    textUpdater.setThreadStatus("Terminated Processes: " + allProcesses);
}
void Scheduler::ResetCritical()
{
    mtx.unlock();
    inCritical = false;
    cvCritical.notify_all();
}
