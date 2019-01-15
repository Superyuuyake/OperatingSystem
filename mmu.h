#ifndef MMU_H
#define MMU_H
#include <QObject>
#include <QtCore>
#include <QList>
#include <QVector>
#include "process.h"
#include "worker.h"
#include "underthehoodworker.h"

class MMU : public QObject
{
    Q_OBJECT
public:
    MMU();
    MMU(const MMU&);
    void operator=(const MMU& pte);
    ~MMU();
    const static int PROCESS_SIZE_LIMIT = 250;
    const static int RESOURCE_1_2_Size = 35;
    const static int MAIN_MEMORY_SIZE = 2048;
    const static int VIRTUAL_MEMORY_SIZE = 4096;
    QQueue<int> availableResourceOnes;
    QQueue<int> availableResourceTwos;
    int mainMemFramesForCurrentProcesses;
    QString mainMemory[2048] = { };
    QQueue<QString>cache;
    QString virtualMemory[4096] { };
    QQueue<uint> freeFrames;
    QQueue<uint> freeFramePool;
    QQueue<uint> freeVirtualFrames;
    QList<std::shared_ptr<Process>> currentProcesses;
    Worker worker;
    UnderTheHoodWorker textUpdater;

    void FreeUnusedMemory(std::shared_ptr<Process>, uint);
    void FreeResources(std::shared_ptr<Process>);
    bool AllocateResources(std::shared_ptr<Process>);
    void FreeMemory(std::shared_ptr<Process>);
    uint VictimPage(std::shared_ptr<Process>, QList<uint>);

    bool LoadCurrentInstruction(std::shared_ptr<Process>, QList<uint>);
    bool AllocateFrame(std::shared_ptr<Process>, QList<uint>, uint);
    bool LoadProcess(std::shared_ptr<Process>);
    void StoreInCache(QString);
    bool LookInCache(QString);
    uint GetFromVirtual(QString);
    uint GetFromMain(QString);

    bool DeadlockAvoidance(std::shared_ptr<Process>);

    bool CheckValidation(std::shared_ptr<Process>, uint);
    bool VerifyLegality(std::shared_ptr<Process>);
    QString GetLogicalAddress(std::shared_ptr<Process>, uint);
    QString MakePhysicalAddress(std::shared_ptr<Process>, uint);
    int GetProcessByPID(uint);
    uint FindPageInTable(std::shared_ptr<Process>, uint);
    bool CheckCurrentProcesses(std::shared_ptr<Process>);

};

#endif // MMU_H
