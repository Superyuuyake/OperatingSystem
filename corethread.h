#ifndef CORETHREAD_H
#define CORETHREAD_H
#include <QObject>
#include <QThread>
#include "process.h"
#include "underthehoodworker.h"

class CoreThread : public QThread
{
    Q_OBJECT
public:
    explicit CoreThread(QObject *parent = nullptr);
    void run() override
    {
        Calculation();
    }
    const static int CYCLE_TIME = 150;
    void Calculation();
    std::shared_ptr<Process> process;
signals:
    //void finished();
public slots:
};

#endif // CORETHREAD_H
