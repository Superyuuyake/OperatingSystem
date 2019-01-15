#include "mmu.h"
#include "process.h"
#include "pagetableentry.h"
#include <QtDebug>

MMU::MMU()
{
    for(int i = 0; i<RESOURCE_1_2_Size; i++)
    {
        availableResourceOnes.enqueue(i);
        availableResourceTwos.enqueue(i);
    }
    for(uint i = 0; i<sizeof(mainMemory)/sizeof (mainMemory[0]); i++)
    {
            freeFrames.enqueue(i);
            mainMemory[i] = "";
    }
    for(uint i = 0; i<sizeof(virtualMemory)/sizeof (virtualMemory[0]); i++)
    {
            freeVirtualFrames.enqueue(i);
            virtualMemory[i] = "";
    }
}
MMU::~MMU(){}
//Loads a process into memory. Returns true if successful, false if unsuccessful.
bool MMU::LoadProcess(std::shared_ptr<Process> process)
{
    int sizeOfProcess = int(process->PCB.pageTable.pages.size());
    QString processSize = QString(sizeOfProcess);
    QString VMFreeFrameSize = QString::number(freeVirtualFrames.size());
    QString processId = QString::number(process->PCB.GetPID());
    QString processName = process->textBlock[0].getName();
    if(freeVirtualFrames.size() <  sizeOfProcess)
    {
        textUpdater.setThreadStatus("Load attempt:: " + VMFreeFrameSize + " Size of process:: " + processSize);
        return false;
    }
    for(uint i = 0; i < process->PCB.pageTable.pages.size(); i++)
    {
       QString address = GetLogicalAddress(process, i);
       virtualMemory[freeVirtualFrames.head()] = address;
       freeVirtualFrames.dequeue();
       worker.setVirtualMemoryAmount(1);
       process->PCB.pageTable.pages[i].SetLogicalAddress(address);
    }
    textUpdater.setThreadStatus("Load attempt: " + processName + " PID:" + processId + " Successful.");
    return true;
}

bool MMU:: LoadCurrentInstruction(std::shared_ptr<Process> process, QList<uint> pages)
{

    if(!DeadlockAvoidance(process))
    {
        return false;
    }
    if(process->PCB.pagesNeeded < process->PCB.pageTable.pagesUsed)
    {
        FreeUnusedMemory(process, pages[0]);
        QString numberOfFrames = QString::number(freeFrames.size());
        textUpdater.setThreadStatus("Main memory size after unused memory freed:: " + numberOfFrames);
    }
    for(int i = 0; i<pages.size(); i++)
    {
        QString address = process->PCB.pageTable.pages[uint(i)].GetLogicalAddress();
        if(CheckValidation(process, uint(pages[i])))
        {
            if(LookInCache(address))
            {
                continue;
            }
            else
            {
                GetFromMain(address);
                StoreInCache(address);
                continue;
            }

        }
        else  //PAGE FAULT
        {
            if(AllocateFrame(process, pages, uint(pages[i])))
            {
                continue;
            }
            else
            {
                return false;
            }
        }
    }
    if(!currentProcesses.contains(process))
    {
        currentProcesses.push_back(process);
    }
    return true;
}

//Allocates the needed frame/page into memory. Will choose a victim if necessary.
bool MMU::AllocateFrame(std::shared_ptr<Process> process, QList<uint>pages, uint page)
{
     QString address = GetLogicalAddress(process, page);
    //If the size exceeds what's allowed in main memory, or a segfault occurs, KILL the proccess.
    if((process->PCB.baseRegisterVM + process->PCB.limitRegisterVM) < page)
    {
        return false;
    }
    if(freeFrames.length() > 0 && (process->PCB.pageTable.pagesUsed < process->PCB.pagesNeeded))
    {
        //Find the address in virtualMemory.


        //Store in Main Memory
        mainMemory[freeFrames.head()] = address;

        //Store in the cache
        StoreInCache(address);

        //Update the page table
        process->PCB.pageTable.pagesUsed++;
        process->PCB.pageTable.pages[page].valid = true;
        process->PCB.pageTable.pages[page].locationOfVA = GetFromVirtual(address);
        freeFrames.dequeue();
        worker.setMemoryAmount(1);
    }
    else
    {
        uint newPage = VictimPage(process, pages);

        mainMemory[newPage] = address;

        process->PCB.pageTable.pages[page].SetValidation(true);
    }
    return true;
}

uint MMU::VictimPage(std::shared_ptr<Process> process, QList<uint> pages)
{
    for(uint i=0; i<process->PCB.pageTable.pages.size(); i++)
    {
        if(process->PCB.pageTable.pages[i].valid && (!pages.contains(i)))
        {
            uint victimPage = i;
            QString address = GetLogicalAddress(process, victimPage);
            process->PCB.pageTable.pages[victimPage].SetValidation(false);
            uint mainMemAddr = GetFromMain(address);
            QString mainAddress = QString::number(mainMemAddr);
            textUpdater.setThreadStatus("MainMemory Page at: " + mainAddress + " Victimized");
            return mainMemAddr;
        }
    }
   return 0;
}

void MMU::FreeMemory(std::shared_ptr<Process>process)
{
    int freedFrames = 0;
    QString numberOfFrames = QString::number(freeFrames.size());
    textUpdater.setThreadStatus("Main memory size before free:: " + numberOfFrames);
    for(uint i=0; i<process->PCB.pageTable.pages.size(); i++)
    {

        //TODO: CHANGE TO BASE ADDRESS + PAGE;
        QString addressToFree = GetLogicalAddress(process,i);

            if(process->PCB.pageTable.pages[i].CheckValidation())
            {
                uint mainMemoryAddr = GetFromMain(addressToFree);
                freedFrames++;
                process->PCB.pageTable.pages[i].SetValidation(false);

                freeFrames.enqueue(mainMemoryAddr);
                worker.setMemoryAmount(-1);
            }
            uint virtualLocation = GetFromVirtual(addressToFree);
            freeVirtualFrames.enqueue(virtualLocation);
            worker.setVirtualMemoryAmount(-1);
    }
    currentProcesses.removeOne(process);
    process->PCB.pageTable.pagesUsed = 0;
    numberOfFrames = QString::number(freeFrames.size());
    textUpdater.setThreadStatus("Main memory size after free:: " + numberOfFrames);
}
void MMU::FreeUnusedMemory(std::shared_ptr<Process>process, uint firstPageNeeded)
{
    for(uint i=0; i<firstPageNeeded; i++)
    {
        QString addressToFree = GetLogicalAddress(process, i);
            if(process->PCB.pageTable.pages[i].CheckValidation())
            {
                uint mainMemoryAddr = GetFromMain(addressToFree);
                freeFrames.enqueue(mainMemoryAddr);
                worker.setMemoryAmount(-1);
                process->PCB.pageTable.pages[i].SetValidation(false);
                process->PCB.pageTable.pagesUsed--;
            }
    }
}
void MMU::FreeResources(std::shared_ptr<Process>process)
{
    int rec1 = process->PCB.resourceOneAllocated.size();
    int rec2 = process->PCB.resourceTwoAllocated.size();
    if(rec1>0 || rec2>0)
    {
    for(int i = 0; i<rec1; i++)
    {
       availableResourceOnes.enqueue(process->PCB.resourceOneAllocated.dequeue());

    }
    for(int i = 0; i<rec2; i++)
    {
        availableResourceTwos.enqueue(process->PCB.resourceTwoAllocated.dequeue());
    }
    worker.setResourceAmount(-rec1, -rec2);
    }
}
bool MMU::AllocateResources(std::shared_ptr<Process>process)
{
    if((availableResourceOnes.size() < process->PCB.resourceOneNeeded) || (availableResourceTwos.size() < process->PCB.resourceTwoNeeded))
    {
        FreeResources(process);
        QString r1Needed = QString::number(process->PCB.resourceOneNeeded);
        QString r2Needed = QString::number(process->PCB.resourceTwoNeeded);
        QString r1Available = QString::number(availableResourceOnes.size());
        QString r2Available = QString::number(availableResourceTwos.size());

        textUpdater.setThreadStatus("Not enough resources! R1Needed:: " + r1Needed + " R1Available:: " + r1Available + "R2Needed:: " + r2Needed + " R2Available:: " + r2Available);
        return false;
    }
    int rec1 = process->PCB.resourceOneNeeded;
    for(int i = 0; i < process->PCB.resourceOneNeeded; i++)
    {
        process->PCB.resourceOneAllocated.enqueue(availableResourceOnes.dequeue());
    }
    int rec2 = process->PCB.resourceTwoNeeded;
    for(int i = 0; i<process->PCB.resourceTwoNeeded; i++)
    {
        process->PCB.resourceTwoAllocated.enqueue(availableResourceTwos.dequeue());
    }
    worker.setResourceAmount(rec1, rec2);
    return true;
}
bool MMU::CheckCurrentProcesses(std::shared_ptr<Process> process)
{
    for(int j=0; j<currentProcesses.size(); j++)
    {
        if(currentProcesses[j] == process)
        {
            return true;
        }
    }
    return false;
}
bool MMU::CheckValidation(std::shared_ptr<Process> process, uint i)
{
    if(process->PCB.pageTable.pages[i].CheckValidation())
    {
        return true;
    }
    return false;
}
bool MMU::VerifyLegality(std::shared_ptr<Process>process)
{
    uint sizeOfProcess = unsigned(process->PCB.pageTable.pages.size());
    int rec1=process->PCB.resourceOneNeeded;
    int rec2=process->PCB.resourceTwoNeeded;
    if((sizeOfProcess > 100) ||(rec1>20) || (rec2>20))
    {
        return false;
    }
    return true;
}
//BankersAlgorithm
bool MMU::DeadlockAvoidance(std::shared_ptr<Process>process)
{

    if(int(process->PCB.pagesNeeded-process->PCB.pageTable.pagesUsed)>freeFrames.size())
    {
        FreeResources(process);
        return false;
    }
    if(!AllocateResources(process))
    {
        return false;
    }
    return true;
}
QString MMU::GetLogicalAddress(std::shared_ptr<Process>process, uint page)
{
    QString physicalAddress = "p" +QString::number(process->PCB.GetPID())+ "f" + QString::number(page);
    return physicalAddress;
}
bool MMU::LookInCache(QString physicalAddress)
{
    for(QString a:cache)
    {
        if(a == physicalAddress)
        {
            a.remove(physicalAddress);
            a.push_back(physicalAddress);
            return true;
        }
    }
    return false;
}
void MMU::StoreInCache(QString physicalAddress)
{
    if(cache.size()<20)
    {
        cache.push_back(physicalAddress);
    }
    else
    {
        cache.removeFirst();
        cache.push_back(physicalAddress);
    }
}
uint MMU::GetFromVirtual(QString phyicalAddress)
{
    for(uint i = 0; i<VIRTUAL_MEMORY_SIZE; i++)
    {
        if(virtualMemory[i] == phyicalAddress)
        {
            return i;
        }
    }
    return 0;
}
uint MMU::GetFromMain(QString address)
{
    for(uint i = 0; i<MAIN_MEMORY_SIZE; i++)
    {
        if(mainMemory[i] == address)
        {
            return i;
        }
    }
    return 0;
}
