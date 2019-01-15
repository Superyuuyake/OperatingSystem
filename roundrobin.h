#ifndef ROUNDROBIN_H
#define ROUNDROBIN_H
#include "scheduler.h"
#include <QObject>

class RoundRobin : public Scheduler
{
public:
    RoundRobin();
    int ShortTermSchedule() override;
};

#endif // ROUNDROBIN_H
