#include "operatingsystemwindow.h"
#include "ui_operatingsystemwindow.h"
#include <QtDebug>

OperatingSystemWindow::OperatingSystemWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OperatingSystemWindow)
{
    ui->setupUi(this);
}

OperatingSystemWindow::~OperatingSystemWindow()
{
    delete ui;
}
