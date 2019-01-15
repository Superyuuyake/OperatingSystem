#include "shortestjobfirst.h"
ShortestJobFirst::ShortestJobFirst()
{
}
int ShortestJobFirst::ShortTermSchedule()
{
    int index = 0;
    bool inCritical = false; int criticalIndex=0;
    bool isImportant = false; int importantIndex=0;
    if(!readyQueue.isEmpty()){
        for(int i = 1; i<readyQueue.size(); i++)
        {
            if(readyQueue[i]->PCB.GetTotalTimeRemaining() < readyQueue[index]->PCB.GetTotalTimeRemaining())
            {
                index = i;
            }
            if(readyQueue[i]->PCB.inCritical)
            {
                criticalIndex = i;
                inCritical = true;
            }
            if(readyQueue[i]->PCB.GetProcessPriority() == 0)
            {
                importantIndex = i;
                isImportant = true;
                inCritical = false;
                break;
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
