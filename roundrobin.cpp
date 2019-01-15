#include "roundrobin.h"
RoundRobin::RoundRobin()
{

}

int RoundRobin::ShortTermSchedule()
{
    if(readyQueue.size() > 0)
        return 0;
    return -1;
}
