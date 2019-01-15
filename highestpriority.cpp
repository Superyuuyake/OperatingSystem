#include "highestpriority.h"

HighestPriority::HighestPriority()
{

}
int HighestPriority::ShortTermSchedule()
{
    int index = 0;
    bool inCritical = false; int criticalIndex=0;
    bool isImportant = false; int importantIndex=0;
    if(!readyQueue.isEmpty()){
        for(int i = 1; i<readyQueue.size(); i++)
        {
            if(readyQueue[i]->PCB.GetProcessPriority() < readyQueue[index]->PCB.GetProcessPriority())
            {
                index = i;
                if(readyQueue[i]->PCB.GetProcessPriority() == 0)
                {
                    inCritical = false;
                    isImportant = true;
                    importantIndex = i;
                    break;
                }
            }
            if(readyQueue[i]->PCB.inCritical)
            {
                criticalIndex = i;
                inCritical = true;
            }
        }
        if(inCritical)
        {
            index = criticalIndex;
        }
        if(isImportant)
        {
            index = importantIndex;
        }
    return index;
    }
    return -1;
}
