#include "process.h"
#include <QString>
#include "pcb.h"
#include "line.h"
#include <QCoreApplication>
#include <ctime>
static uint pid = 0;
Process::Process()
{
}
Process::Process(QString processType)
{

    SetTextBlock(processType);
    PCB.SetPID(pid);
    pid++;
}
Process::Process(const Process& b1)
{
    textBlock = b1.textBlock;
    PCB = b1.PCB;

}
Process::~Process() {}
void Process::operator=(const Process& process)
{
  textBlock = process.textBlock;
  PCB = process.PCB;
}
bool Process::operator==(const Process& process)
{
    if(this->PCB == process.PCB)
    {
        return true;
    }
        return false;
}
void Process::SetTextBlock(QString processType)
{
    if(processType == "random")
    {
        int newType = (rand()%(5));
        if(newType == 0)
        {
            processType = "wordProcessor";
        }
        else if(newType == 1)
        {
            processType = "virusScan";
        }
        else if(newType == 2)
        {
            processType = "webBrowser";
        }
        else if(newType == 3)
        {
            processType = "parent";
        }
        else
        {
            processType = "mediaPlayer";
        }
    }
    int totalTime = 0;
    QString path = "";
    path = ":processes/"+processType+".xml";
    QFile *file = new QFile(path);
    if(file->open(QIODevice::ReadOnly | QIODevice::Text))
    {

        textBlock.emplace_back(processType, 0);
        QXmlStreamReader xml(file);
        while(!xml.atEnd() & !xml.hasError())
        {
            QXmlStreamReader::TokenType token = xml.readNext();\
            if(xml.name() == "" || xml.name() == " ")
            {
                continue;
            }
            if(token == QXmlStreamReader::StartDocument)
            {

                continue;
            }
            if(token == QXmlStreamReader::StartElement)
            {
                if(xml.name() == "size")
                {
                    uint numOfPages = xml.readElementText().toUInt();
                    PCB.pageTable.InitializePages(numOfPages);
                    PCB.limitRegisterVM = numOfPages;
                    PCB.pageTable.totalPages = numOfPages;
                    PCB.baseRegisterVM = 0;
                }
                if(xml.name() == "pages")
                {
                    uint pagesNeeded = xml.readElementText().toUInt();
                    PCB.pagesNeeded = pagesNeeded;
                }
                if(xml.name() == "calculate")
                {
                    int random = (1+ rand() % (1000));
                    totalTime = totalTime + random;
                    textBlock.emplace_back(xml.name().toString(), random);
                }
                if(xml.name() == "io")
                {
                    int random = (25 + rand() % (50));
                    totalTime = totalTime + random;
                    textBlock.emplace_back(xml.name().toString(), random);
                }
                if(xml.name() == "yield")
                {
                    textBlock.emplace_back(xml.name().toString(), 0);
                }
                if(xml.name() == "critical" || xml.name() == "out")
                {
                    textBlock.emplace_back(xml.name().toString(), 0);
                }
                if(xml.name() == "fork")
                {
                    textBlock.emplace_back(xml.name().toString(), 0);
                }
                if(xml.name() == "communicate")
                {
                    textBlock.emplace_back(xml.name().toString(), 0);
                }
                if(xml.name() == "mailbox")
                {
                    textBlock.emplace_back(xml.name().toString(), 0);
                }
            }

        }
        textBlock.front().setAmount(totalTime);
        PCB.resourceOneNeeded= rand()%10;
        PCB.resourceTwoNeeded= rand()%10;
        PCB.SetTotalTimeRemaining(totalTime);
        PCB.SetProcessPriority((1+rand()%20));
        if(textBlock[0].getName() != "parent")
        {
            int mailBoxNumber = rand()%4;
            if(mailBoxNumber == 0)
            {
                PCB.SetMailBoxKey(958038);
                PCB.SetMailBoxNumber(0);
            }
            else if(mailBoxNumber == 1)
            {
                PCB.SetMailBoxKey(204895);
                PCB.SetMailBoxNumber(1);
            }
            else if(mailBoxNumber == 2)
            {
                PCB.SetMailBoxKey(586832);
                PCB.SetMailBoxNumber(2);
            }
            else if(mailBoxNumber == 3)
            {
                PCB.SetMailBoxKey(139583);
                PCB.SetMailBoxNumber(3);
            }
            else
            {
                PCB.SetMailBoxKey(585920);
                PCB.SetMailBoxNumber(4);
            }
        }
        if(textBlock[0].getName() == "largeResource")
        {
            PCB.resourceOneNeeded = 10;
            PCB.resourceTwoNeeded = 10;
        }
        if(textBlock[0].getName() == "highPriority")
        {
            PCB.SetProcessPriority(0);
        }
    }
}
int Process::SubtractAmount(int amount)
{
    QString lineAmount = QString::number(GetCurrentLineAmount());
    QString cycleTimeAllotted = QString::number(amount);
    QString processId = QString::number(PCB.GetPID());

    int newAmount = GetCurrentLineAmount() - amount;
    int totalTime = PCB.GetTotalTimeRemaining();
    int amountAfterSubtract;
    //No cycle time remaining
    if(newAmount > 0)
    {
    amountAfterSubtract = (totalTime-amount);
    textBlock[PCB.GetProgramCounter()].setAmount(newAmount);
    PCB.SetTotalTimeRemaining(amountAfterSubtract);
    return 0;
    }
    //Perfect amount to finish process
    else if(newAmount == 0)
    {
        amountAfterSubtract = (totalTime-amount);
        PCB.SetTotalTimeRemaining(amountAfterSubtract);
        textBlock[PCB.GetProgramCounter()].setAmount(0);
        PCB.IncreaseProgramCounter();
        return 0;
    }
    //Cycle time remains
    else
    {
        int cycleTimeRemaining =  (amount-GetCurrentLineAmount());
        amountAfterSubtract = (totalTime-GetCurrentLineAmount());
        PCB.SetTotalTimeRemaining(amountAfterSubtract);
        textBlock[PCB.GetProgramCounter()].setAmount(0);
        PCB.IncreaseProgramCounter();
        return cycleTimeRemaining;
    }

}
int Process::GetCurrentLineAmount()
{
    return textBlock[PCB.GetProgramCounter()].getAmount();
}
void Process::CreateChildProcess(Process& process)
{
    PCB.SetPID(pid); pid++;
    PCB.SetParentProcess(process.PCB.GetPID());
    PCB.isChild = true;
    PCB.SetProcessPriority(process.PCB.GetProcessPriority());
    PCB.programCounter = process.PCB.programCounter;
    PCB.SetTotalTimeRemaining(process.PCB.GetTotalTimeRemaining());
    totalTime = process.totalTime;
    PCB.baseRegisterVM = process.PCB.baseRegisterVM;
    PCB.SetCurrentState(PCB::state::Ready);
    PCB.limitRegisterVM = process.PCB.limitRegisterVM;
    PCB.inCritical = process.PCB.inCritical;
    PCB.pageTable.pagesUsed = 0;
    PCB.pageTable.InitializePages(PCB.limitRegisterVM);
    PCB.pagesNeeded = process.PCB.pagesNeeded;
    PCB.resourceOneNeeded = process.PCB.resourceOneNeeded;
    PCB.resourceTwoNeeded = process.PCB.resourceTwoNeeded;
    PCB.mlQStatus = process.PCB.mlQStatus;
    textBlock = process.textBlock;
    if(process.PCB.GetPipe() != nullptr)
    {
        PCB.SetPipe(process.PCB.GetPipe());
        PCB.SetPipeKey(process.PCB.GetPipeKey());
    }
}
