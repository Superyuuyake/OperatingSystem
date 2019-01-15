#include "mutexlock.h"

MutexLock::MutexLock(QObject *parent) : QObject(parent)
{

}
void MutexLock::LockTheMutex()
{
    isLocked = true;
}
void MutexLock::UnlockTheMutex()
{
    isLocked = false;
}
