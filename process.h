#ifndef PROCESS_H
#define PROCESS_H
#include "pcb.h"
#include "line.h"
#include "underthehoodworker.h"
#include <QObject>
#include <QString>
#include <QVector>
#include <QCoreApplication>
#include <QtCore>
class Process :public QObject
{
    Q_OBJECT
public:
    Process();
    Process(QString processType);
    Process(const Process&);
    ~Process();
    void operator=(const Process& process);
    void swap(Process&);
    bool operator==(const Process& process);
    void SetTextBlock(QString processType);
    int SubtractAmount(int amount);
    int GetCurrentLineAmount();
    void CreateChildProcess(Process&);
    int totalTime;
    std::vector<Line> textBlock;
    UnderTheHoodWorker textUpdater;
    PCB PCB;
private:

signals:
    void SendToMain(QString);
};
#endif // PROCESS_H
