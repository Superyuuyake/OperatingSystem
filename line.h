#ifndef LINE_H
#define LINE_H
#include <QObject>
#include <QString>
#include <QtCore>
#include <QCoreApplication>

class Line : public QObject
{
    Q_OBJECT
public:
    Line();
    Line(QString name, int amount);
    Line(const Line&);
    ~Line();
    void operator=(const Line& line);
    void setAmount(int amount);
    int getAmount();
    void setName(QString name);
    QString getName();
private:
    int amount;
    QString name;
};

#endif // LINE_H
