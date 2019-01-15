#ifndef PAGETABLEENTRY_H
#define PAGETABLEENTRY_H
#include <QtCore>
#include <QCoreApplication>
#include <QObject>

class PageTableEntry : public QObject
{
    Q_OBJECT
public:
    PageTableEntry(unsigned int);
    PageTableEntry(const PageTableEntry&);
    ~PageTableEntry();
    void operator=(const PageTableEntry& pte);

    uint pageNumber;
    QString logicalAddress;
    uint locationOfVA;
    bool valid;
    bool beingShared;
    uint lastUsed;

    unsigned int GetPageNumber();
    QString GetLogicalAddress();
    void SetLogicalAddress(QString);
    bool CheckValidation();
    void SetValidation(bool);

signals:

public slots:
};

#endif // PAGETABLEENTRY_H
