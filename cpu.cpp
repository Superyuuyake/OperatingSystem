#include "cpu.h"
#include "pcb.h"
#include "process.h"
#include "line.h"
#include <QCoreApplication>
#include <QtCore>
#include <QString>
#include <QtDebug>
#include <thread>
#include <unistd.h>
#include <QObject>
#include <QThread>
#include <QCoreApplication>
#include <QtConcurrent/QtConcurrent>
#include <QSemaphore>

static std::mutex mtx;
static std::mutex mtx2;
static std::condition_variable accessingList;
static std::condition_variable accessingMemory;
//static bool listUsed =false;
//static bool memoryUsed =false;
CPU::~CPU(){}
void CPU::RunProcess()
{
      RunProcesses();
      std::shared_ptr<Process> process;
      while(!currentProcesses.isEmpty())
      {
        process = currentProcesses.front();
        mmu->FreeResources(process);

        currentProcesses.removeAll(process);
        if(process->textBlock[process->PCB.GetProgramCounter()].getName() == "io")
        {
            emit finishedProcessing(process, "io");
        }
        else if(process->textBlock[process->PCB.GetProgramCounter()].getName() == "yield")
        {
            emit finishedProcessing(process, "exit");
        }
        else if(process->textBlock[process->PCB.GetProgramCounter()].getName() == "critical")
        {
            emit finishedProcessing(process, "critical");
        }
        else if(process->textBlock[process->PCB.GetProgramCounter()].getName() == "fork")
        {
            emit finishedProcessing(process, "fork");
        }
        else if(process->textBlock[process->PCB.GetProgramCounter()].getName() == "communicate")
        {
            emit finishedProcessing(process, "communicate");

        }
        else if(process->textBlock[process->PCB.GetProgramCounter()].getName() == "mailbox")
        {
            emit finishedProcessing(process, "mailbox");

        }
        else if(process->textBlock[process->PCB.GetProgramCounter()].getName() == "out")
        {
            emit finishedProcessing(process, "out");
        }
        else
        {
            emit finishedProcessing(process, "calculate");
        }
      }
      if(!resourcesWaiting.isEmpty())
      {
          while(!resourcesWaiting.isEmpty())
          {
              processesWaiting.push_back(resourcesWaiting.takeFirst());
          }
          RunProcesses();
      }
}
bool CPU::LoadMemory(std::shared_ptr<Process>process, QList<uint> pages)
{
    if(!mmu->LoadCurrentInstruction(process, pages))
    {
        return false;
    }
    return true;
}
void CPU::RunProcesses()
{
    std::shared_ptr<Process>process;
    while(!processesWaiting.isEmpty())
    {
        process = processesWaiting.takeFirst();
        QList<uint> pages = GetPages(process);
        if(!LoadMemory(process,pages))
        {
            resourcesWaiting.push_back(process);
            process = nullptr;
        }
        else
        {
            currentProcesses.push_back(process);
            processesWithMemory.push_back(process);
        }
    }

        if(!processesWithMemory.isEmpty())
            process1 = processesWithMemory.takeFirst();
        else
            process1 = nullptr;
        if(!processesWithMemory.isEmpty())
            process2 = processesWithMemory.takeFirst();
        else
            process2 = nullptr;
        if(!processesWithMemory.isEmpty())
            process3 = processesWithMemory.takeFirst();
        else
            process3 = nullptr;
        if(!processesWithMemory.isEmpty())
            process4 = processesWithMemory.takeFirst();
        else
            process4 = nullptr;


        // INITIALIZES ALL THREADS FOR CORE
        QThread thread1; QThread thread2; QThread thread3; QThread thread4;
        CoreThread coreThread; CoreThread coreThread2; CoreThread coreThread3; CoreThread coreThread4;
        coreThread.moveToThread(&thread1); coreThread2.moveToThread(&thread1);
        coreThread3.moveToThread(&thread3); coreThread4.moveToThread(&thread4);



        if(process1 != nullptr)
        {
            coreThread.process = process1;
            coreThread.start();
        }
        if(process2 != nullptr)
        {
            coreThread2.process = process2;
            coreThread2.start();
        }
        if(process3 != nullptr)
        {
            coreThread3.process = process3;
            coreThread3.start();
        }
        if(process4 != nullptr)
        {
            coreThread4.process = process4;
            coreThread4.start();
        }
        coreThread.wait(); coreThread2.wait(); coreThread3.wait(); coreThread4.wait();
}
/****************************************************************************
 * Input: process
 * Purpose: Pages grabbed in sequential order based on the progress of the program
 * Returns: List of pages needed for next instruction
 ***************************************************************************/
QList<uint> CPU::GetPages(std::shared_ptr<Process>process)
{

    //list of all needed pages to be needed for next instruction.
    QList<uint> pages;
    uint totaltextBlock = uint(process->textBlock.size());
    double progressOfProgram = (double(process->PCB.programCounter)/(double(totaltextBlock)));

    //based on progress of program, the pageFrom changes
    int pagesFrom = int((process->PCB.pageTable.totalPages) * progressOfProgram);
    //Must be a valid page
    if((int(process->PCB.pageTable.pages.size())-pagesFrom) < 1)
    {
        pagesFrom = (int(process->PCB.pageTable.pages.size())-2);
    }
    //Change the amount of pages needed to continue execution
    if((int(process->PCB.pagesNeeded)+pagesFrom)>int(process->PCB.pageTable.pages.size()))
    {
        process->PCB.pagesNeeded = (uint(process->PCB.pageTable.pages.size()) - uint(pagesFrom));

    }
    for(uint i=0; i<process->PCB.pagesNeeded; i++)
    {
        uint currentPage = (uint(pagesFrom)+ i);
        if(currentPage<(process->PCB.pageTable.pages.size()))
        {
            uint page = currentPage;
            pages.push_back(page);
        }
    }

    return pages;

}

void CPU::AddToList(std::shared_ptr<Process> process)
{
    processesWaiting.push_back(process);
}
void CPU::SetMMU(std::shared_ptr<MMU> mmu)
{
    this->mmu = mmu;
}
void CPU::ProcessHardwareInterrupt(int timeNeeded)
{
    QThread::sleep(uint(timeNeeded));
}
void CPU::DoSetup(QThread& cpuThread)
{
    connect(&cpuThread, SIGNAL(started()), this, SLOT(RunProcesses()));
}

