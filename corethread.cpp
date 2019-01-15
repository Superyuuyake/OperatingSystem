#include "corethread.h"

CoreThread::CoreThread(QObject *parent)
{

}
void CoreThread::Calculation()
{
    if(process == nullptr)
    {
        return;
        //emit finished();
    }
        QString processId = QString::number(process->PCB.GetPID());

        int amountRemainder = CYCLE_TIME;
            while(amountRemainder > 0 && process->textBlock[process->PCB.GetProgramCounter()].getName() == "calculate")  // TODO: and there's no interrupts.
            {
                amountRemainder = process->SubtractAmount(amountRemainder);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds((70)));
}

