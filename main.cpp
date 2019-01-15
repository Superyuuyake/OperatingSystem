#include "scheduler.h"
#include "process.h"
#include <QtCore>
#include <QXmlStreamReader>
#include <QString>
#include <QtDebug>
#include <QApplication>
#include "pcb.h"
#include "mainwindow.h"
#include "multilevelfeedbackschedule.h"
#include "line.h"
#include "shortestjobfirst.h"
#include <unistd.h>
#include "cpu.h"
#include "roundrobin.h"
#include "highestpriority.h"
#include <vector>
#include <memory>
#include <algorithm>
#include <utility>
#include <QTextStream>
#include <QThread>
#include <csignal>
#include <mmu.h>





/***********************************
 * TODO: Implement HIGH priority I/O
 * TODO: Change child processes to random generated process
 *
 ***********************************/
static ShortestJobFirst schedule;
static std::mutex mtx;
static std::condition_variable cv;
static bool ready =false;

void call_thread(int amountOfProcesses, QString nameOfProcess)
{
    for(int i = 0; i<amountOfProcesses; i++)
    {
        std::shared_ptr<Process> process = std::make_shared<Process>(nameOfProcess);

        if(process->textBlock.size() > 0)
        {
        schedule.AddProcess(process);
        }
        else
        {
            break;
        }
    }
}
void run_thread()
{
    std::unique_lock<std::mutex> lck(mtx);
    while(!ready){ cv.wait(lck);}

    //.schedule.RunAllReady(unsigned(INT_MAX));
    ready=false;
    cv.notify_all();
}
/**************************************************************************
 * Going to take in a name. Based off the name grab an XML file
 * Once XML file is grabbed. Parse file.
 * Grab NAME and Generate randomINT
 * Make LineOBJECT and store in a vector and return back to PCB
 *************************************************************************/
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
/*
    //srand(time(NULL));
    std::thread thread_Obj2(run_thread);

    (
    ShortestJobFirst scheduler;
    RoundRobin sched;

    for(unsigned int i = 0; i<request.textBlock.size(); i++)
    {
        qDebug() <<(request.textBlock[i].getName());
        qDebug() << (request.textBlock[i].getAmount());
    }


    QTextStream out(stdout);
    QTextStream in(stdin);
    QString nameOfProcess;
    QString scheduleType;
    int amountOfProcesses;
    bool b= true;
    while(b){
    out << "Hello, which type of processes would you like to run?" << endl;
    out << "Choices: type as seen [wordProcessor, mediaPlayer, virusScan, or webBrowser] type exit to leave" << endl;

    in >> nameOfProcess << endl;
    if(nameOfProcess == "exit")
    {
        b=false;
        exit(0);

    }
    out << "how many " << nameOfProcess << " programs would you like to simulate? " << endl;
    in >> amountOfProcesses << endl;
    QThread thread1;
    CPU cpu;
    cpu.DoSetup(thread1);
    cpu.moveToThread(&thread1);
    thread1.start();

    thread1.wait();
    std::thread threadObj1(call_thread, amountOfProcesses, nameOfProcess);
    threadObj1.join();
    if(schedule.readyQueue.size() >0 && ready == false)
    {
        std::unique_lock<std::mutex> lck(mtx);
        ready = true;
        cv.notify_all();
    }

    }
    thread_Obj2.join();\
    */
}
