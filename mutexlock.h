#ifndef MUTEXLOCK_H
#define MUTEXLOCK_H

#include <QObject>

class MutexLock : public QObject
{
    Q_OBJECT
public:
    explicit MutexLock(QObject *parent = nullptr);
    void LockTheMutex();
    void UnlockTheMutex();

    bool isLocked = false;
signals:

public slots:
};

#endif // MUTEXLOCK_H
