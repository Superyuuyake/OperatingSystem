#ifndef UNDERTHEHOODWORKER_H
#define UNDERTHEHOODWORKER_H

#include <QObject>

class UnderTheHoodWorker : public QObject
{
    Q_OBJECT
public:
    explicit UnderTheHoodWorker(QObject *parent = nullptr);
    void setThreadStatus(QString);
    void setReadyQ(int);
    void setWaitingQ(int);
    void setNewQ(int);
    void setCriticalQ(int);
    void setParentQ(int);
    void setTerminatedQ(int);

signals:
    void sendThreadStatus(QString);
    void sendReadyQ(int);
    void sendWaitingQ(int);
    void sendNewQ(int);
    void sendCriticalQ(int);
    void sendParentQ(int);
    void sendTerminatedQ(int);
public slots:

};

#endif // UNDERTHEHOODWORKER_H
