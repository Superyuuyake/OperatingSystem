#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "operatingsystemwindow.h"
#include <QtDebug>
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    srand(unsigned(time(NULL)));
    ui->setupUi(this);
    ui->processType->setReadOnly(true);
    ui->numberOfProcesses->setReadOnly(true);
    ui->numberOfCycles->setReadOnly(true);
    ui->textEdit->setReadOnly(true);
    ui->underTheHood->setReadOnly(true);
    ui->memoryBar->setRange(0,2048);
    ui->virtualBar->setRange(0,4096);
    ui->resourceBar->setRange(0, 35);
    ui->resources2Bar->setRange(0,35);
    ui->memoryBar->setValue(0);
    ui->resourceBar->setValue(0);
    ui->resources2Bar->setValue(0);
    ui->virtualBar->setValue(0);
    ui->readyQ->setEnabled(true);
    ui->numberOfCycles->setPlaceholderText("Please select a scheduler to begin...");
    shortestJob = nullptr;
    roundRobin = nullptr;
    highestPriority = nullptr;
    mlq = nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::AddCycles(uint cycles)
{
    scheduler->AddCycles(cycles);
}
void MainWindow::TakeInCommand(QString processType, int numberOfProcesses, uint numberOfCycles)
{
    if(processType != "")
    {
    for(int i = 0; i<numberOfProcesses; i++)
    {
        std::shared_ptr<Process> process = std::make_shared<Process>(processType);

        if(process->textBlock.size() > 0)
        {
        scheduler->AddProcess(process);

        }
    }
    }

    if(numberOfCycles == 0)
    {
        numberOfCycles = UINT_MAX;
    }


    if(!scheduler->continueRunning)
    {
        scheduler->AddCycles(numberOfCycles);
        scheduler->continueRunning = true;
        emit StartExec();
    }
}
void MainWindow::on_pushButton_clicked()
{
    ui->numberOfCycles->setPlaceholderText("Number of cycles");
    ui->pushButton->setDisabled(true);
    ui->processType->setReadOnly(false);
    ui->numberOfProcesses->setReadOnly(false);
    ui->numberOfCycles->setReadOnly(false);
if(ui->ShortestJobFirst->isChecked())
{
    ui->RoundRobin->setCheckable(false);
    ui->HighestPriority->setCheckable(false);
    ui->MultiLevelFeedbackSchedule->setCheckable(false);
    schedulerType = "SJF";
    std::shared_ptr<ShortestJobFirst> scheduler(new ShortestJobFirst);
    shortestJob = scheduler;
}
else if(ui->HighestPriority->isChecked())
{   
    ui->RoundRobin->setCheckable(false);
    ui->ShortestJobFirst->setCheckable(false);
    ui->MultiLevelFeedbackSchedule->setCheckable(false);
    schedulerType = "HP";
    std::shared_ptr<HighestPriority> scheduler(new HighestPriority);
    highestPriority = scheduler;
}
else if(ui->RoundRobin->isChecked())
{
    ui->HighestPriority->setCheckable(false);
    ui->ShortestJobFirst->setCheckable(false);
    ui->MultiLevelFeedbackSchedule->setCheckable(false);
    schedulerType = "RR";
    std::shared_ptr<RoundRobin> scheduler(new RoundRobin);
    roundRobin = scheduler;
}
else if(ui->MultiLevelFeedbackSchedule->isChecked())
{
    ui->HighestPriority->setCheckable(false);
    ui->ShortestJobFirst->setCheckable(false);
    ui->MultiLevelFeedbackSchedule->setCheckable(false);
    schedulerType = "MLQ";
    std::shared_ptr<MultiLevelFeedbackSchedule> scheduler(new MultiLevelFeedbackSchedule);
    mlq = scheduler;
}
if(roundRobin)
{
    scheduler = roundRobin;
}
else if(highestPriority)
    scheduler = highestPriority;
else if(mlq)
    scheduler = mlq;
else
    scheduler = shortestJob;

scheduler->moveToThread(workerThread);
workerThread->start();
connect(this, SIGNAL(StartExec()), scheduler.get(), SLOT(beginExecution()));
connect(scheduler.get(), SIGNAL(finished()), scheduler.get(), SLOT(deleteLater()));
connect(workerThread, SIGNAL(finished()), workerThread, SLOT(deleteLater()));

connect(&scheduler->mmu->worker, SIGNAL(sendMemoryUpdate(int)), this, SLOT(update_memory_bar(int)));
connect(&scheduler->mmu->worker, SIGNAL(sendVirtualMemoryUpdate(int)), this, SLOT(update_virtual_bar(int)));
connect(&scheduler->mmu->worker, SIGNAL(sendResourcesUpdate(int, int)), this, SLOT(update_resources_bar(int, int)));
connect(&scheduler->mmu->textUpdater, SIGNAL(sendThreadStatus(QString)), this, SLOT(updateUnderTheHood(const QString)));

//connect(workerThread, SIGNAL(finished()), workerThread, SLOT(deleteLater()));

connect(&scheduler->textUpdater, SIGNAL(sendThreadStatus(QString)), this, SLOT(updateUnderTheHood(const QString)));
connect(&scheduler->textUpdater, SIGNAL(sendReadyQ(int)), this, SLOT(updateReadyQ(int)));
connect(&scheduler->textUpdater, SIGNAL(sendWaitingQ(int)), this, SLOT(updateWaitingQ(int)));
connect(&scheduler->textUpdater, SIGNAL(sendNewQ(int)), this, SLOT(updateNewQ(int)));
connect(&scheduler->textUpdater, SIGNAL(sendTerminatedQ(int)), this, SLOT(updateTerminatedQ(int)));
connect(&scheduler->textUpdater, SIGNAL(sendParentQ(int)), this, SLOT(updateParentQ(int)));
connect(&scheduler->textUpdater, SIGNAL(sendCriticalQ(int)), this, SLOT(updateCriticalQ(int)));

scheduler->totalCycles=0;
scheduler->SetCoreMMU();
scheduler->SetUp();
}

void MainWindow::on_pushButton_2_clicked()
{
    scheduler->ResetCritical();
    if(shortestJob!=nullptr)
        shortestJob = nullptr;
    if(roundRobin!=nullptr)
        roundRobin = nullptr;
    if(highestPriority!=nullptr)
        highestPriority = nullptr;
    if(mlq!=nullptr)
        mlq = nullptr;
    if(scheduler!=nullptr)
        scheduler=nullptr;
    ui->pushButton->setDisabled(false);
    ui->numberOfCycles->setPlaceholderText("Please select a scheduler to begin...");
    ui->processType->setReadOnly(true);
    ui->numberOfProcesses->setReadOnly(true);
    ui->numberOfCycles->setReadOnly(true);
    ui->ShortestJobFirst->setChecked(true);
    ui->ShortestJobFirst->setCheckable(true);
    ui->HighestPriority->setCheckable(true);
    ui->RoundRobin->setCheckable(true);
    ui->MultiLevelFeedbackSchedule->setCheckable(true);
    ui->criticalQ->display(0);
    ui->terminatedQ->display(0);
    ui->waitingQ->display(0);
    ui->readyQ->display(0);
    ui->parentQ->display(0);
    ui->newQ->display(0);
    ui->memoryBar->setValue(0);
    ui->virtualBar->setValue(0);
}

void MainWindow::on_pushButton_3_clicked()
{
    QString nextCommand = ui->processType->text();

    int numberOfProcesses = ui->numberOfProcesses->text().toInt();
    uint numberOfCycles = ui->numberOfCycles->text().toUInt();
    ui->processType->clear();
    ui->numberOfProcesses->clear();
    ui->numberOfCycles->clear();

    printToTextEdit("CMD(): " + nextCommand + " #Processes: " + QString::number(numberOfProcesses) + " #Cycles: " +QString::number(numberOfCycles));
    TakeInCommand(nextCommand, numberOfProcesses, numberOfCycles);
}

void MainWindow::printToTextEdit(const QString printStatement)
{
    ui->textEdit->append(printStatement + "\n");
}
void MainWindow::updateUnderTheHood(const QString printStatement)
{
    ui->underTheHood->append(printStatement + "\n");
}
void MainWindow::on_memoryBar_valueChanged()
{
    ui->memoryBar->show();
}
void MainWindow::on_virtualBar_valueChanged()
{
    ui->virtualBar->show();
}
void MainWindow::on_resourceBar_valueChanged()
{
   ui->resourceBar->show();
}
void MainWindow::on_resources2Bar_valueChanged()
{
    ui->resources2Bar->show();
}
void MainWindow::update_memory_bar(int value)
{
    ui->memoryBar->setValue((ui->memoryBar->value() + value));
}
void MainWindow::update_resources_bar(int rec1, int rec2)
{
     ui->resourceBar->setValue((ui->resourceBar->value() + rec1));
     ui->resources2Bar->setValue((ui->resources2Bar->value() + rec2));
}
void MainWindow::update_virtual_bar(int value)
{
     ui->virtualBar->setValue((ui->virtualBar->value() + value));
}
void MainWindow::updateReadyQ(int value)
{
    ui->readyQ->display((ui->readyQ->intValue() + value));
}
void MainWindow::updateWaitingQ(int value)
{
    ui->waitingQ->display((ui->waitingQ->intValue() + value));
}
void MainWindow::updateNewQ(int value)
{
    ui->newQ->display((ui->newQ->intValue() + value));
}
void MainWindow::updateTerminatedQ(int value)
{
    ui->terminatedQ->display((ui->terminatedQ->intValue() + value));
}
void MainWindow::updateParentQ(int value)
{
    ui->parentQ->display((ui->parentQ->intValue() + value));
}
void MainWindow::updateCriticalQ(int value)
{
    ui->criticalQ->display((ui->criticalQ->intValue() + value));
}
