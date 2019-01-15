#ifndef SHORTESTJOBFIRST_H
#define SHORTESTJOBFIRST_H
#include "scheduler.h"

class ShortestJobFirst : public Scheduler
{
public:
    ShortestJobFirst();
    int ShortTermSchedule() override;
};

#endif // SHORTESTJOBFIRST_H
