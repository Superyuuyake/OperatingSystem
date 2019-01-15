#ifndef OPERATINGSYSTEMWINDOW_H
#define OPERATINGSYSTEMWINDOW_H

#include <QMainWindow>

namespace Ui {
class OperatingSystemWindow;
}

class OperatingSystemWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit OperatingSystemWindow(QWidget *parent = nullptr);
    ~OperatingSystemWindow();

private:
    Ui::OperatingSystemWindow *ui;
};

#endif // OPERATINGSYSTEMWINDOW_H
