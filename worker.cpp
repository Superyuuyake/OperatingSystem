#include "worker.h"
#include <QMutexLocker>
#include <iostream>
#include <QtDebug>

Worker::Worker(QObject *parent) :
    QObject(parent),
    active(false),
    progressToggleTerminal(false),
    workDone(0)
{
}

void Worker::setThreadStatus(QString status)
{
    emit sendThreadStatus(status);
}
void Worker::setResourceAmount(int rec1, int rec2)
{
    emit sendResourcesUpdate(rec1, rec2);
}
void Worker::setMemoryAmount(int mem)
{
    emit sendMemoryUpdate(mem);
}
void Worker::setVirtualMemoryAmount(int vm)
{
    emit sendVirtualMemoryUpdate(vm);
}
/*
void Worker::doWork()
{
    // Do not put an infinite loop here! Otherwise the thread the object is running on will be 100% occupied with the loop and no signals will be processed!!!
    if(active)
    {
        setThreadStatus("Working");
        // Do work
        workChunk();
    }
    else setThreadStatus("    Idle    ");
}

void Worker::workChunk()
{
    QThread::msleep(workSpeed);
    if(workDone < workAmount)
    {
        workDone++;
        emit sendProgress(workDone, workSpeed);
    }
    else
    {
        emit sendFinished();
        active = false;
    }
}

void Worker::toggle()
{
    QMutexLocker locker(&lock);
    active = !active;

    if(workDone == workAmount)
    {
        workDone = 0;
        emit sendProgress(workDone, workSpeed);
    }
}
*/
