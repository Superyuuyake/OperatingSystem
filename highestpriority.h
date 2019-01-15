#ifndef HIGHESTPRIORITY_H
#define HIGHESTPRIORITY_H

#include <QObject>
#include "scheduler.h"

class HighestPriority : public Scheduler
{
public:
    HighestPriority();
    int ShortTermSchedule() override;
    static const int PRIORITY_CYCLE_AMOUNT = 100;
};

#endif // HIGHESTPRIORITY_H
