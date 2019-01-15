#include "pcb.h"

PCB::PCB()
{
    currentState = state::New;
    programCounter = 1;
    processPriority = 1 + qrand() %20;
    inCritical=false;
    isChild=false;
    pipe=nullptr;
    sharedVariable = INT_MAX;
};
PCB::PCB(const PCB& b1)
{
    pid = b1.pid;
    currentState = b1.currentState;
    parentProcess = b1.parentProcess;
    processPriority = b1.processPriority;
    programCounter = b1.programCounter;
    totalTimeRemaining = b1.totalTimeRemaining;
    pageTable = b1.pageTable;
    pagesNeeded = b1.pagesNeeded;
    baseRegisterVM = b1.baseRegisterVM;
    limitRegisterVM = b1.limitRegisterVM;
    inCritical = b1.inCritical;
    isChild = b1.isChild;
    mlQStatus  = b1.mlQStatus;
    lastCycle = b1.lastCycle;
    pipe = b1.pipe;
    sharedVariable = b1.sharedVariable;
    pipeKey = b1.pipeKey;
    resourceOneAllocated = b1.resourceOneAllocated;
    resourceTwoAllocated = b1.resourceTwoAllocated;
    resourceOneNeeded = b1.resourceOneNeeded;
    resourceTwoNeeded = b1.resourceTwoNeeded;
    mailBoxKey = b1.mailBoxKey;
    mailBoxNumber = b1.mailBoxNumber;
}
PCB::~PCB(){}
void PCB::operator=(const PCB& pcb)
{
    pid = pcb.pid;
    currentState = pcb.currentState;
    parentProcess = pcb.parentProcess;
    processPriority = pcb.processPriority;
    programCounter = pcb.programCounter;
    totalTimeRemaining = pcb.totalTimeRemaining;
    pageTable = pcb.pageTable;
    pagesNeeded = pcb.pagesNeeded;
    baseRegisterVM = pcb.baseRegisterVM;
    limitRegisterVM = pcb.limitRegisterVM;
    inCritical = pcb.inCritical;
    isChild = pcb.isChild;
    mlQStatus  = pcb.mlQStatus;
    lastCycle = pcb.lastCycle;
    pipe = pcb.pipe;
    sharedVariable = pcb.sharedVariable;
    pipeKey = pcb.pipeKey;
    resourceOneAllocated = pcb.resourceOneAllocated;
    resourceTwoAllocated = pcb.resourceTwoAllocated;
    resourceOneNeeded = pcb.resourceOneNeeded;
    resourceTwoNeeded = pcb.resourceTwoNeeded;
    mailBoxKey = pcb.mailBoxKey;
    mailBoxNumber = pcb.mailBoxNumber;
}
bool PCB::operator==(const PCB& pcb)
{
    if(pid == pcb.pid)
    {
        return true;
    }
    return false;
}
QString PCB::GetCurrentState()
{
    return PCB::stateNames[static_cast<int>(currentState)];
}
void PCB::SetCurrentState(enum state state)
{
    currentState = state;
}
unsigned int PCB::GetProcessPriority()
{
    return processPriority;
}
void PCB::SetProcessPriority(unsigned int priority)
{
    processPriority = priority;
}
unsigned int PCB::GetProgramCounter()
{
    return programCounter;
}
void PCB::IncreaseProgramCounter()
{
    this->programCounter++;
}
uint PCB::GetParentProcess()
{
    return parentProcess;
}
void PCB::SetParentProcess(uint parentProcess)
{
    this->parentProcess = parentProcess;
}
uint PCB::GetPID()
{
    return pid;
}
void PCB::SetPID(unsigned int pid)
{
    this->pid = pid;
}
int PCB::GetTotalTimeRemaining()
{
    return totalTimeRemaining;
}
void PCB::SetTotalTimeRemaining(int amount)
{
    totalTimeRemaining = amount;
}
void PCB::SetLastCycle(uint cycle)
{
    lastCycle = cycle;
}
uint PCB::GetLastCycle()
{
    return lastCycle;
}
void PCB::SetPipe(std::shared_ptr<Pipe> pipe)
{
    this->pipe = pipe;
}
std::shared_ptr<Pipe> PCB::GetPipe()
{
    return pipe;
}
void PCB::SetSharedVariable(int var)
{
    this->sharedVariable = var;
}
int PCB::GetSharedVariable()
{
    return sharedVariable;
}
void PCB::SetPipeKey(int key)
{
    this->pipeKey = key;
}
int PCB::GetPipeKey()
{
    return pipeKey;
}
void PCB::SetMailBoxNumber(int number)
{
    this->mailBoxNumber = number;
}
int PCB::GetMailBoxNumber()
{
    return mailBoxNumber;
}
void PCB::SetMailBoxKey(int key)
{
    this->mailBoxKey = key;
}
int PCB::GetMailBoxKey()
{
    return mailBoxKey;
}
