
#include "underthehoodworker.h"

UnderTheHoodWorker::UnderTheHoodWorker(QObject *parent) : QObject(parent)
{

}
void UnderTheHoodWorker::setThreadStatus(QString newMessage)
{
    emit sendThreadStatus(newMessage);
}
void UnderTheHoodWorker::setReadyQ(int value)
{
    emit sendReadyQ(value);
}
void UnderTheHoodWorker::setWaitingQ(int value)
{
    emit sendWaitingQ(value);
}
void UnderTheHoodWorker::setNewQ(int value)
{
     emit sendNewQ(value);
}
void UnderTheHoodWorker::setCriticalQ(int value)
{
     emit sendCriticalQ(value);
}
void UnderTheHoodWorker::setParentQ(int value)
{
     emit sendParentQ(value);
}
void UnderTheHoodWorker::setTerminatedQ(int value)
{
     emit sendTerminatedQ(value);
}
