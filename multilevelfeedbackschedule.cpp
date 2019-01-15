#include "multilevelfeedbackschedule.h"
static uint currentCycle = 0;
MultiLevelFeedbackSchedule::MultiLevelFeedbackSchedule()
{

}

void MultiLevelFeedbackSchedule::AddProcess(std::shared_ptr<Process> process)
{
    newQueue.push_back(process);
    if(!processor.mmu->VerifyLegality(process))
    {
        TerminateProcess(process, this->terminatedQueue, this->newQueue);
    }
    process->PCB.mlQStatus = "high";

    if(process->textBlock[process->PCB.GetProgramCounter()].getName() != "wait" && process->textBlock[process->PCB.GetProgramCounter()].getName() != "yield")
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
void MultiLevelFeedbackSchedule::RunAllReady()
{

    while(continueRunning && totalCycles>0)
    {
        if((readyQueue.size()>0 || waitingQueue.size()>0))
        {
        while(processor.processesWaiting.size()<4 && !readyQueue.isEmpty())
        {
        if(!HighestPriorityQueue.isEmpty())
        {
            QString processId = QString::number(HighestPriorityQueue.front()->PCB.GetPID());
            QString processPriority = QString::number(HighestPriorityQueue.front()->PCB.GetPID());
            QString processSizeLeft = QString::number(HighestPriorityQueue.front()->PCB.GetPID());
            textUpdater.setThreadStatus("Selected HP-Process:: " + processId + "   Priority:: " + processPriority + "   Size remaining:: " + processSizeLeft);
                std::shared_ptr<Process> process = HighestPriorityQueue.takeFirst();
                runningQueue.push_back(process);
                processor.processesWaiting.push_back(process);
                readyQueue.removeOne(process);
                int nextLineAmount = process->textBlock[process->PCB.GetProgramCounter()].getAmount();
                process->PCB.SetLastCycle(currentCycle);
                if(nextLineAmount > HPQ_CYCLE_AMOUNT)
                {
                    process->PCB.mlQStatus = "second";
                }
        }
        else if((!SecondPriorityQueue.isEmpty()))
        {
            QString processId = QString::number(SecondPriorityQueue.front()->PCB.GetPID());
            QString processPriority = QString::number(SecondPriorityQueue.front()->PCB.GetPID());
            QString processSizeLeft = QString::number(SecondPriorityQueue.front()->PCB.GetPID());
            textUpdater.setThreadStatus("Selected SP-Process:: " + processId + "   Priority:: " + processPriority + "   Size remaining:: " + processSizeLeft);
                std::shared_ptr<Process> process = SecondPriorityQueue.takeFirst();
                runningQueue.push_back(process);
                processor.processesWaiting.push_back(process);
                readyQueue.removeOne(process);
                int nextLineAmount = process->textBlock[process->PCB.GetProgramCounter()].getAmount();
                process->PCB.SetLastCycle(currentCycle);
                if(nextLineAmount > SPQ_CYCLE_AMOUNT)
                {
                    process->PCB.mlQStatus = "least";
                }
        }
       else if(!LeastPriorityQueue.isEmpty())
        {
            QString processId = QString::number(LeastPriorityQueue.front()->PCB.GetPID());
            QString processPriority = QString::number(LeastPriorityQueue.front()->PCB.GetPID());
            QString processSizeLeft = QString::number(LeastPriorityQueue.front()->PCB.GetPID());
            textUpdater.setThreadStatus("Selected LP-Process:: " + processId + "   Priority:: " + processPriority + "   Size remaining:: " + processSizeLeft);
                std::shared_ptr<Process> process = LeastPriorityQueue.takeFirst();
                runningQueue.push_back(process);
                readyQueue.removeOne(process);
                processor.processesWaiting.push_back(process);
                process->PCB.SetLastCycle(currentCycle);
        }
        }
        if(processor.processesWaiting.size()>0)
        {
            processor.RunProcess();
            totalCycles--;
            currentCycle++;
        }
        if(currentCycle%250 == 0)
        {
            FeedTheProcesses();
        }
        }

        if(readyQueue.size() == 0 && waitingQueue.size()==0 && runningQueue.size() == 0)
        {
            printTerminatedProcesses();
            continueRunning=false;
        }
        if(waitingQueue.size()>0)
        {
            SimulateIO(waitingQueue.front());
        }
    }
    continueRunning=false;
}

void MultiLevelFeedbackSchedule::MoveProcess(std::shared_ptr<Process> process, QList<std::shared_ptr<Process>>& toQueue, QList<std::shared_ptr<Process>>& fromQueue, QString state)
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
            if((process->PCB.mlQStatus == "high" || (process->PCB.GetProcessPriority() < 3)) && (!HighestPriorityQueue.contains(process)))
                HighestPriorityQueue.push_back(process);
            else if(process->PCB.mlQStatus == "second" || (process->PCB.GetProcessPriority() < 5))
                SecondPriorityQueue.push_back(process);
            else if(process->PCB.mlQStatus == "least")
                LeastPriorityQueue.push_back(process);
        }
        else
        { }
        toQueue.push_back(process);
        fromQueue.removeOne(process);

}
void MultiLevelFeedbackSchedule::FeedTheProcesses()
{
    textUpdater.setThreadStatus("Feeding the hungry");
    for(std::shared_ptr<Process> p : SecondPriorityQueue)
    {
        uint priority = p->PCB.GetProcessPriority();
        QString processId = QString::number(p->PCB.GetPID());
        if((((currentCycle - p->PCB.GetLastCycle()) > 50) || (priority < 3)) && readyQueue.contains(p))
        {
            if(priority>3)
                p->PCB.SetProcessPriority(--priority);
            textUpdater.setThreadStatus("Moving " + processId + " from second to high");
            p->PCB.mlQStatus = "high";
            HighestPriorityQueue.push_back(p);
            SecondPriorityQueue.removeOne(p);
        }
    }
    for(std::shared_ptr<Process> p : LeastPriorityQueue)
    {
        uint priority = p->PCB.GetProcessPriority();
        QString processId = QString::number(p->PCB.GetPID());
        if(priority < 3 && readyQueue.contains(p))
        {
            //QString processId = QString::number(p->PCB.GetPID());
            textUpdater.setThreadStatus("Moving " + processId + " from least to high");
            HighestPriorityQueue.push_back(p);
            p->PCB.mlQStatus = "high";
            LeastPriorityQueue.removeOne(p);
        }
        else if((((currentCycle - p->PCB.GetLastCycle()) > 75) || (priority < 8)) && readyQueue.contains(p))
        {
            if(priority>6)
                p->PCB.SetProcessPriority(--priority);
            //QString processId = QString::number(p->PCB.GetPID());
            textUpdater.setThreadStatus("Moving " + processId + "  from least to second");
            SecondPriorityQueue.push_back(p);
            p->PCB.mlQStatus = "second";
            LeastPriorityQueue.removeOne(p);
        }
    }
}

void MultiLevelFeedbackSchedule::TerminateProcess(std::shared_ptr<Process> process, QList<int>& toQueue, QList<std::shared_ptr<Process>>& fromQueue)
{
    process->PCB.SetCurrentState(PCB::state::Exit);
    processor.mmu->FreeMemory(process);
    fromQueue.removeOne(process);
    toQueue.push_back(int(process->PCB.GetPID()));
    textUpdater.setTerminatedQ(1);
    if(HighestPriorityQueue.contains(process))
    {
        HighestPriorityQueue.removeOne(process);
    }
    else if(SecondPriorityQueue.contains(process))
    {
        SecondPriorityQueue.removeOne(process);
    }
    else if(LeastPriorityQueue.contains(process))
    {
        LeastPriorityQueue.removeOne(process);
    }
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
 * Simulates I/O for a given process
 **************************************/
void MultiLevelFeedbackSchedule::SimulateIO(std::shared_ptr<Process>process)
{
    QString processId = QString::number(process->PCB.GetPID());
    textUpdater.setThreadStatus("Processing I/O from process:: " + processId + "Amount: " + QString::number(process->GetCurrentLineAmount()));
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
        textUpdater.setReadyQ(1);
        textUpdater.setWaitingQ(-1);
        process->PCB.SetCurrentState(PCB::state::Ready);
        MoveProcess(process, readyQueue, waitingQueue, "ready");
    }
    else
    {
        MoveProcess(process, waitingQueue, waitingQueue, "wait");
    }
}

