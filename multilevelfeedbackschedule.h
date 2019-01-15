#ifndef MULTILEVELFEEDBACKSCHEDULE_H
#define MULTILEVELFEEDBACKSCHEDULE_H
#include "scheduler.h"
#include <QObject>

class MultiLevelFeedbackSchedule : public Scheduler
{

public:
    MultiLevelFeedbackSchedule();
    static const int HPQ_CYCLE_AMOUNT = 50;
    static const int SPQ_CYCLE_AMOUNT = 100;
    static const int LPQ_CYCLE_AMOUNT = 250;

    void AddProcess(std::shared_ptr<Process>) override;
    void RunAllReady() override;
    void TerminateProcess(std::shared_ptr<Process>, QList<int>& , QList<std::shared_ptr<Process>>&) override;
    void MoveProcess(std::shared_ptr<Process> , QList<std::shared_ptr<Process>>&, QList<std::shared_ptr<Process>>&, QString) override;
    void SimulateIO(std::shared_ptr<Process>process) override;
    void FeedTheProcesses();

    QList<std::shared_ptr<Process>> HighestPriorityQueue;
    QList<std::shared_ptr<Process>> SecondPriorityQueue;
    QList<std::shared_ptr<Process>> LeastPriorityQueue;

};

#endif // MULTILEVELFEEDBACKSCHEDULE_H
