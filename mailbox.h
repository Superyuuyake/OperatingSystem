#ifndef MAILBOX_H
#define MAILBOX_H
#include <QPair>
#include <QtCore>
#include <QCoreApplication>
#include <QObject>

class MailBox : public QObject
{

public:
    MailBox();
    ~MailBox();
    void Write(int, int, int);
    int Read(int, int);
private:
    QPair<int, int> messageHolder[5];
};

#endif // MAILBOX_H
