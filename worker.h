#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>
#include <QMutex>

class Worker : public QObject
{
    Q_OBJECT
private:
    bool active;
    int progressToggleTerminal;
    int workDone;
    QMutex lock;

    void setThreadStatus(QString status);
    void workChunk();

public:
    explicit Worker(QObject *parent = 0); 
    void setResourceAmount(int, int);
    void setMemoryAmount(int);
    void setVirtualMemoryAmount(int);

signals:
    void sendThreadStatus(QString msg);
    //void sendProgress(int workDone, int workSpeed);
    void sendMemoryUpdate(int);
    void sendResourcesUpdate(int, int);
    void sendVirtualMemoryUpdate(int);

};

#endif // WORKER_H
